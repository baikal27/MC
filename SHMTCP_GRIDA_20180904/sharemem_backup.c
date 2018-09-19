//----------------------------------------------------------------------------
// This sample demonstrates how to share memory between two applications, and 
// how to use a lock to protect access to the shared memory.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <cvirte.h> 
#include <cvi2009compat.h>
#include <windows.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>

#include "sharemem.h"

#include <stdio.h>
#include <stdlib.h>
#include <tcpsupp.h>
#include <string.h>
//#include "server.h"

/*---------------------------------------------------------------------------*/
/* Macros for TCP/IP			                                             */
/*---------------------------------------------------------------------------*/
#define tcpChk(f) if ((g_TCPError=(f)) < 0) {ReportTCPError(); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Macros for SharedMemory                             */
/*---------------------------------------------------------------------------*/
//#define MAXCLIENTS 10
#define CLIENT_RECEIVE_BUFFER_SIZE 1024
#define SERVER_RECEIVE_BUFFER_SIZE 2048
#define COORD_BUFFER_SIZE 256
#define LOCK_NAME  "SHAREDLOCK"
#define MEMORY_NAME "SHAREDMEMORY"
#define SHARED_MEMORY_SIZE (int)sizeof(tSharedMemType)

/*---------------------------------------------------------------------------*/
/* Internal function prototypes for TCP/IP                                   */
/*---------------------------------------------------------------------------*/
int CVICALLBACK ServerTCPCB (unsigned handle, int event, int error,
							 void *callbackData);
static void ReportTCPError (void);

/*---------------------------------------------------------------------------*/
/* Internal function prototypes for SharedMemory                             */
/*---------------------------------------------------------------------------*/
int UploadServerToSHM(char receiveBuf[]);
int DownloadSHMToTcs(void);

//----------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------
/*
typedef struct clientAreaRec
{
    DWORD clientProcessID;
    char clientReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
} tClientArea;
*/
typedef struct ServerAreaRec
{
	char now_az[COORD_BUFFER_SIZE], now_el[COORD_BUFFER_SIZE];
	char now_ra[COORD_BUFFER_SIZE], now_dec[COORD_BUFFER_SIZE];
	int cw_limit_az, ccw_limit_az, home_limit_az, cw_limit_el, ccw_limit_el, home_limit_el;
} tServerArea;

typedef struct tcsAreaRec
{
	DWORD tcsProcessID;
	char tcsReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
	int cmd_kmap;
	char cmd_x[COORD_BUFFER_SIZE], cmd_y[COORD_BUFFER_SIZE];
	int cmd_tcsinit, cmd_park, cmd_init, cmd_stop, cmd_go_tel, cmd_man_w, cmd_man_e, cmd_man_up, cmd_man_down, cmd_go_off_go;
	double cmd_off_x, cmd_off_y;
} tTcsArea;

typedef struct sharedMemTypeRec
{
    int serverRunning;
    int tcsConnected;
    // Message for Server
    int sendingClientProcessID;
    char serverReceiveBuffer[SERVER_RECEIVE_BUFFER_SIZE];
    // Message for Client
//    tClientArea client[MAXCLIENTS];
//	tClientArea client;
	tServerArea server;
	tTcsArea tcs;
} tSharedMemType;


/*---------------------------------------------------------------------------*/
/* Variables for shared memory												 */
/*---------------------------------------------------------------------------*/
static int panelHandle;
static CmtThreadLockHandle lock = 0;
static HANDLE hMemMapFile = 0;
static int clientIndex    = -1;
static int bServerApp     = 0;
static DWORD dwWaitResult = 0;
static tSharedMemType *sharedMemory = NULL;
static char msgBuffer[256];

/*---------------------------------------------------------------------------*/
/* Module-globals for TCP/IP                                                 */
/*---------------------------------------------------------------------------*/
static unsigned int tcp_hconversation;
static int			g_TCPError = 0;
static int      	portNum;
static int		 	tcp_registered = 0;

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------
tSharedMemType *CreateOrOpenMapFile(HANDLE *hMemMapFile, char *mapName, int create, int size, int *alreadyExist);		// 구조체의 pointer를 가지고, 각 요소에 접근하겠다는 심산.
int CloseMapping(HANDLE *hMemMapFile, void **memPtr);

int SetupServerOrClient(void);
int CleanupServerOrClient(void);


//----------------------------------------------------------------------------
// Main
//----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	int status;

	if (InitCVIRTE (0, argv, 0) == 0)	  /* Initialize CVI libraries */
		return -1;						  /* out of memory */ 
	
//	DisableBreakOnLibraryErrors();
    
    // Ask user if this is a Server or Client
    status = GenericMessagePopup ("Start a Server or Client?",
                                  "Choose which type of application this is:",
                                  "Server", "Client", "Cancel", NULL, 0,
                                  0, VAL_GENERIC_POPUP_BTN1,
                                  VAL_GENERIC_POPUP_BTN1,
                                  VAL_GENERIC_POPUP_BTN3);
	
	// Check results of prompt                                  
    if (status == VAL_GENERIC_POPUP_BTN1)
        bServerApp = 1;
    else if (status == VAL_GENERIC_POPUP_BTN2)
	{
        bServerApp = 0;
    }
	else goto Done;
	
	// Display and run panel 
	if ((panelHandle = LoadPanel(0, "sharemem.uir", PANEL)) < 0)
	{
		MessagePopup("Server Error","Failed to load UIR panel.");
		goto Done;
	}
                                 
    // Update UIR panel title
    sprintf(msgBuffer, "SHAREMEM Sample %s (Process ID:%8x)", 
        (bServerApp)?"Server":"Client", 
        (int)GetCurrentProcessId() );									 // Getting a ProcessId
    SetPanelAttribute (panelHandle, ATTR_TITLE, msgBuffer);
    
    // Display UIR
    DisplayPanel (panelHandle);
    
    // Setup Server or Client
    if (SetupServerOrClient()) 
    {
        SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
        RunUserInterface ();
    }    

	// cleanup for TCP/IP
	if (tcp_registered)
		UnregisterTCPServer (portNum);

    // Cleanup for shared memory
    CleanupServerOrClient();
    CloseMapping(&hMemMapFile, (void **)(&sharedMemory));
    CmtDiscardLock(lock);
    DiscardPanel(panelHandle);
//	CloseCVIRTE();
	    
Done:
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the TCP server's TCP callback.  This function will receive event  */
/* notification, similar to a UI callback, whenever a TCP event occurs.      */
/* We'll respond to CONNECT and DISCONNECT messages and indicate to the user */
/* when a client connects to or disconnects from us.  when we have a client  */
/* connected, we'll respond to the DATAREADY event and read in the available */
/* data from the client and display it.                                      */
/*---------------------------------------------------------------------------*/
int CVICALLBACK ServerTCPCB (unsigned handle, int event, int error,
							 void *callbackData)
{
	char receiveBuf[256] = {0};
	ssize_t dataSize        = sizeof (receiveBuf) - 1;
	char addrBuf[31];

	switch (event)
		{
		case TCP_CONNECT:
			if (tcp_hconversation)
				{
				/* We already have one client, don't accept another... */
				tcpChk (GetTCPPeerAddr (handle, addrBuf, 31));
				sprintf (receiveBuf, "-- Refusing conection request from "
									 "%s --\n", addrBuf); 
				SetCtrlVal (panelHandle, PANEL_RECEIVELIST, receiveBuf);
				tcpChk (DisconnectTCPClient (handle));
				}
			else
				{
				/* Handle this new client connection */
				tcp_hconversation = handle;
				SetCtrlVal (panelHandle, PANEL_TCP_CONNECTED, 1);
				tcpChk (GetTCPPeerAddr (tcp_hconversation, addrBuf, 31));
				SetCtrlVal (panelHandle, PANEL_CLIENT_IP, addrBuf);
				tcpChk (GetTCPPeerName (tcp_hconversation, receiveBuf, 256));
				SetCtrlVal (panelHandle, PANEL_CLIENT_NAME, receiveBuf);
				sprintf (receiveBuf, "-- New connection from %s --\n",
						 addrBuf); 
				SetCtrlVal (panelHandle,PANEL_RECEIVELIST, receiveBuf);
				SetCtrlAttribute (panelHandle, PANEL_TRANSMIT, ATTR_DIMMED,
								  0);
				
				/* Set the disconect mode so we do not need to terminate */
				/* connections ourselves. */
				tcpChk (SetTCPDisconnectMode (tcp_hconversation, 
											  TCP_DISCONNECT_AUTO));
				}
			break;
		case TCP_DATAREADY:
			if ((dataSize = ServerTCPRead (tcp_hconversation, receiveBuf,
										   dataSize, 1000))
				< 0)
				{
				SetCtrlVal (panelHandle, PANEL_RECEIVELIST, "Receive Error\n");				// from tcp to shm function
				}
			else
				{
				receiveBuf[dataSize] = '\0';
				SetCtrlVal (panelHandle, PANEL_RECEIVELIST, receiveBuf);
				UploadServerToSHM(receiveBuf);
				}									  
			break;
		case TCP_DISCONNECT:
			if (handle == tcp_hconversation)
				{
				/* The client we were talking to has disconnected... */
				SetCtrlVal (panelHandle, PANEL_TCP_CONNECTED, 0);
				tcp_hconversation = 0;
				SetCtrlVal (panelHandle, PANEL_CLIENT_IP, "");
				SetCtrlVal (panelHandle, PANEL_CLIENT_NAME, "");
				SetCtrlVal (panelHandle, PANEL_RECEIVELIST,
							"-- Client disconnected --\n");
				SetCtrlAttribute (panelHandle, PANEL_TRANSMIT, ATTR_DIMMED,
								  1);
				
				/* Note that we do not need to do any more because we set the*/
				/* disconnect mode to AUTO. */
				}
			break;
	}
	
Done:    
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Report TCP Errors if any                         						 */
/*---------------------------------------------------------------------------*/
static void ReportTCPError (void)
{
	if (g_TCPError < 0)
		{
		char	messageBuffer[1024];
		sprintf(messageBuffer, 
			"TCP library error message: %s\nSystem error message: %s", 
			GetTCPErrorString (g_TCPError), GetTCPSystemErrorString());
		MessagePopup ("Error", messageBuffer);
		}
}

//----------------------------------------------------------------------------
// SetupServerOrClient - Sets up shared memory and lock and initializes it
//----------------------------------------------------------------------------
int SetupServerOrClient(void)
{
    int success = 0;
    int i;
    int alreadyExist;
	char     tempBuf[256] = {0};
    
    if (CmtNewLock(LOCK_NAME, OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &lock) != 0)
    {
    	if (bServerApp)
    		MessagePopup("Server Error", "Failed to create Server lock.");
    	else
    		MessagePopup("Client Error", "Failed to create Client lock.");
    	goto Done;
    }
    if (bServerApp)
	{
		CmtGetLock(lock);
				/* Prompt for the port number on which to receive connections */
		
		PromptPopup ("Port Number?",
					 "Type the port number on which you would like to register "
					 "this server application.\n\n(example: 10000)",
					 tempBuf, 31);
		portNum = atoi (tempBuf);

		/* Attempt to register as a TCP server... */
		SetWaitCursor (1);
		if (RegisterTCPServer (portNum, ServerTCPCB, 0) < 0)
			MessagePopup("TCP Server", "TCP Server registration failed!");
		else
		{
			tcp_registered = 1;
			SetWaitCursor (0);
		
			/* We are successfully registered -- gather info */
			SetCtrlVal (panelHandle, PANEL_ONLINE, 1);
			if (GetTCPHostAddr (tempBuf, 256) >= 0)
	        	SetCtrlVal (panelHandle, PANEL_SERVER_IP, tempBuf);
			if (GetTCPHostName (tempBuf, 256) >= 0)
	        	SetCtrlVal (panelHandle, PANEL_SERVER_NAME, tempBuf);
			SetCtrlAttribute (panelHandle, PANEL_TRANSMIT, ATTR_DIMMED, 1);
			SetCtrlVal (panelHandle, PANEL_TCP_CONNECTED, 0); 
		
			/* Display the panel and run the UI */
//			SetActiveCtrl (panelHandle, PANEL_TRANSMIT);
		}

	}
    	
    // Create or open shared memory    
    if (!(sharedMemory = CreateOrOpenMapFile(&hMemMapFile, MEMORY_NAME, bServerApp, SHARED_MEMORY_SIZE, &alreadyExist)))
    {
       	if (bServerApp) 
		{
           	MessagePopup("Server Error","Failed to create Server shared memory.");
			CmtReleaseLock(lock);
		}
       	else
			MessagePopup("Client Error","Server is not running.");
       	goto Done;
    }
        
    // Initialize shared memory if creator, and release lock
    if (sharedMemory)
    {
       	// If server
       	if (bServerApp) 
       	{
			if (!alreadyExist)
               	memset(sharedMemory, 0, SHARED_MEMORY_SIZE);
           	sharedMemory->serverRunning = 1;
           	CmtReleaseLock(lock);
           	success = 1;
			SetCtrlVal (panelHandle, PANEL_CONNECTED, 1);
       	}
       	// If client initialize client area 
       	else 
       	{
       		CmtGetLock(lock);
           	// Find opening in client list
		    
           	// If found, take ownership of it                                
           
            // Increment counter in shared memory
           	sharedMemory->tcsConnected = 1;
           	sharedMemory->tcs.tcsProcessID = GetCurrentProcessId();
           	sharedMemory->tcs.tcsReceiveBuffer[0] = 0;
			clientIndex++;
		
			SetCtrlVal (panelHandle, PANEL_CONNECTED, 1);
           	success = 1;								 
			CmtReleaseLock(lock);
       	}
    }
	else
        CmtReleaseLock(lock);
		MessagePopup("Shared Memory Error", "Failed to create Shared Memory.");   
    
Done:
    return success;
}


//----------------------------------------------------------------------------
// CleanupServerOrClient - Cleanup shared memory entry for server or client
//----------------------------------------------------------------------------
int CleanupServerOrClient(void)
{
    int success = 0;
   
    CmtGetLock(lock);
	if (sharedMemory)
    {
        if (bServerApp)
        {
            sharedMemory->serverRunning = 0;
        }
        else if (clientIndex>=0) 
        {
            sharedMemory->tcs.tcsProcessID = 0;
            sharedMemory->tcsConnected = 0;
        }
        
        success = 1;
    }
    CmtReleaseLock(lock);

    return success;
}

//----------------------------------------------------------------------------
// CreateOrOpenMapFile
//----------------------------------------------------------------------------
tSharedMemType *CreateOrOpenMapFile(HANDLE *hMemMapFile, char *mapName, int create, int size, int *alreadyExist)
{
    void *memPtr = NULL;
 
    if (hMemMapFile) 
    { 
        if (create)
        {
            // CreateFileMapping
            *hMemMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE, 
                NULL, 
                PAGE_READWRITE, 
                (DWORD)(size>>16), 
                (DWORD)(size & 0xFFFFFFFF),
                mapName);
        } 
        else 
        {        
            *hMemMapFile = OpenFileMapping(
                FILE_MAP_ALL_ACCESS, 
                FALSE, 
                mapName);

        }
        if (*hMemMapFile == NULL) 
            return 0;
        else 
        {
            if (alreadyExist) 
            {
                if (create)
                     *alreadyExist = (GetLastError()==ERROR_ALREADY_EXISTS);
                else *alreadyExist = 1;
            }    
        
            // Map to memory
            memPtr = (LPSTR)MapViewOfFile(*hMemMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            return memPtr;
        }    
    } 
    else return 0;
}

//----------------------------------------------------------------------------
// CloseMapping
//----------------------------------------------------------------------------
int CloseMapping(HANDLE *hMemMapFile, void **memPtr)
{
    // Unmap Memory
    if (memPtr) 
    {
        UnmapViewOfFile(*memPtr);
        *memPtr = 0;
    }    
    
    // Close Mapping
    if (hMemMapFile)
    {
        CloseHandle(*hMemMapFile);
        *hMemMapFile = 0;
    }
    return 1;
}    

//----------------------------------------------------------------------------
// TransmitCallback
//----------------------------------------------------------------------------
int CVICALLBACK TransmitCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    static int i;
    static size_t currentLen, appendLen;
    static char *buffer;
    static char messageOut[256];
    static int fullBuffers;
    
    switch (event) 
    {
      case EVENT_COMMIT:
        if (!sharedMemory)
            return 0;
            
        CmtGetLock(lock);
            // Sent message by updating shared memory
            if (bServerApp)
            {
                // Send server message to all clients by adding text
                // to each client receive buffer
                fullBuffers = 0;
                
                if (sharedMemory->tcs.tcsProcessID) 
                {
                    GetCtrlVal (panel, PANEL_TRANSMIT, messageOut);
                    appendLen = strlen(messageOut);
                        
                    buffer = sharedMemory->tcs.tcsReceiveBuffer;
                    currentLen = strlen(buffer);
                        
                    if (currentLen+appendLen<CLIENT_RECEIVE_BUFFER_SIZE)
                    {
                        strcat(buffer, messageOut);
                        strcat(buffer, "\n");
                    }    
                    else fullBuffers++;
                        
                    InsertTextBoxLine (panel, PANEL_TRANSMITLIST, -1, messageOut);
                }
                    
                
                SetCtrlVal (panel, PANEL_TRANSMIT, "");                
                
                CmtReleaseLock(lock);
        
                if (fullBuffers)
                    MessagePopup("Server Error", "Client receive buffer full, cannot transmit");
            }
            // If client
            else if ((sharedMemory->tcsConnected) && (sharedMemory->tcs.tcsProcessID)) 
            {
                // Add text to server receive buffer
                fullBuffers = 0;
                GetCtrlVal (panel, PANEL_TRANSMIT, messageOut);
                appendLen = strlen(messageOut);
                
                buffer = sharedMemory->serverReceiveBuffer;
                currentLen = strlen(buffer);
                
                if (currentLen+appendLen+20<SERVER_RECEIVE_BUFFER_SIZE)
                {
                    sprintf(&buffer[currentLen], "From Client[%d]: ", clientIndex);
                    strcat(buffer, messageOut);
                    strcat(buffer, "\n");
                }    
                else fullBuffers = 1;
                
                InsertTextBoxLine (panel, PANEL_TRANSMITLIST, -1, messageOut);
                
                SetCtrlVal (panel, PANEL_TRANSMIT, "");
            
            	CmtReleaseLock(lock);
    
                if (fullBuffers)
                    MessagePopup("Client Error", "Server receive buffer full, cannot transmit");
            }
            else
            	CmtReleaseLock(lock);
        break;
    }    
    return 0;
}

//----------------------------------------------------------------------------
// UploadCallback
//----------------------------------------------------------------------------
int CVICALLBACK UploadCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    static int i;
    static size_t currentLen, appendLen;
    static char *buffer;
    static char messageOut[256];
    static int fullBuffers;
    
    switch (event) 
    {
      case EVENT_COMMIT:
        if (!sharedMemory)
            return 0;
            
        CmtGetLock(lock);
            // Sent message by updating shared memory
            if (bServerApp)
            {
                // Send server message to all clients by adding text
                // to each client receive buffer
                fullBuffers = 0;
                
                if (sharedMemory->tcs.tcsProcessID) 
                {
                    GetCtrlVal (panel, PANEL_RA, messageOut);
                    appendLen = strlen(messageOut);
                        
                    buffer = sharedMemory->tcs.cmd_x;
                    currentLen = strlen(buffer);
                        
                    if (currentLen+appendLen<CLIENT_RECEIVE_BUFFER_SIZE)
                    {
                        strcat(buffer, messageOut);
                        strcat(buffer, "\n");
                    }    
                    else fullBuffers++;
                        
                    InsertTextBoxLine (panel, PANEL_TRANSMITLIST, -1, messageOut);
					
					
					GetCtrlVal (panel, PANEL_DEC, messageOut);
                    appendLen = strlen(messageOut);
                        
                    buffer = sharedMemory->tcs.cmd_y;
                    currentLen = strlen(buffer);
                        
                    if (currentLen+appendLen<CLIENT_RECEIVE_BUFFER_SIZE)
                    {
                        strcat(buffer, messageOut);
                        strcat(buffer, "\n");
                    }    
                    else fullBuffers++;
                        
                    InsertTextBoxLine (panelHandle, PANEL_TRANSMITLIST, -1, messageOut);
                }
                    
                
                SetCtrlVal (panelHandle, PANEL_TRANSMIT, "");                
                
                CmtReleaseLock(lock);
        
                if (fullBuffers)
                    MessagePopup("Server Error", "Client receive buffer full, cannot transmit");
            }
            // If client
            else if ((sharedMemory->tcsConnected) && (sharedMemory->tcs.tcsProcessID)) 
            {
                // Add text to server receive buffer
                fullBuffers = 0;
                GetCtrlVal (panel, PANEL_RA, messageOut);
                appendLen = strlen(messageOut);
                
                buffer = sharedMemory->server.now_ra;
                currentLen = strlen(buffer);
                
                if (currentLen+appendLen+20<SERVER_RECEIVE_BUFFER_SIZE)
                {
                    sprintf(&buffer[currentLen], "From Client[%d]: ", clientIndex);
                    strcat(buffer, messageOut);
                    strcat(buffer, "\n");
                }    
                else fullBuffers = 1;
                
                InsertTextBoxLine (panel, PANEL_TRANSMITLIST, -1, messageOut);
				
				GetCtrlVal (panel, PANEL_DEC, messageOut);
                appendLen = strlen(messageOut);
                
                buffer = sharedMemory->server.now_dec;
                currentLen = strlen(buffer);
                
                if (currentLen+appendLen+20<SERVER_RECEIVE_BUFFER_SIZE)
                {
                    sprintf(&buffer[currentLen], "From Client[%d]: ", clientIndex);
                    strcat(buffer, messageOut);
                    strcat(buffer, "\n");
                }    
                else fullBuffers = 1;
                
                InsertTextBoxLine (panel, PANEL_TRANSMITLIST, -1, messageOut);
                
                SetCtrlVal (panel, PANEL_TRANSMIT, "");
            
            	CmtReleaseLock(lock);
    
                if (fullBuffers)
                    MessagePopup("Client Error", "Server receive buffer full, cannot transmit");
            }
            else
            	CmtReleaseLock(lock);
        break;
    }    
    return 0;
}

//----------------------------------------------------------------------------
// TimerCallback
//----------------------------------------------------------------------------
int CVICALLBACK TimerCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    static char *buffer;
    
    switch (event) 
    {
      case EVENT_TIMER_TICK:
        if (!sharedMemory)
            return 0;
		
		DownloadSHMToTcs();
		break;

/*		
        SetCtrlVal (panel, PANEL_CONNECTED, (bServerApp)?
            sharedMemory->tcsConnected:sharedMemory->serverRunning);
       
        CmtGetLock(lock);
        // Read and clear receive buffer
        if (bServerApp)
		{
            buffer = sharedMemory->server.now_ra;
			SetCtrlVal(panel, PANEL_RECEIVELIST, buffer);
			 
			buffer = sharedMemory->server.now_dec;
			SetCtrlVal(panel, PANEL_RECEIVELIST, buffer);
		}
        else 
		{     
			buffer = sharedMemory->tcs.cmd_x;
			SetCtrlVal(panel, PANEL_RECEIVELIST, buffer);
			
			buffer = sharedMemory->tcs.cmd_y;
			SetCtrlVal(panel, PANEL_RECEIVELIST, buffer);
		}
            
        // InsertTextBoxLine (panel, PANEL_RECEIVELIST, -1, buffer);
        
        buffer[0] = 0;
                
        CmtReleaseLock(lock);
        break;
*/
    }
    return 0;
}

//----------------------------------------------------------------------------
// Server to SharedMemory upload
//----------------------------------------------------------------------------
int UploadServerToSHM(char receiveBuf[])
{
//	char receiveBuf[256] = {0};
	ssize_t dataSize        = sizeof (receiveBuf) - 1;
	char addrBuf[31];
	int rah, ram, decd, decm;
	double ras, decs;
	
	static int i;
    static size_t currentLen, appendLen;
    static char *buffer;
//  static char messageOut[256];   대신 ra[256], dec[256]을 사용한다.
	char ra[256];
	char dec[256];
    static int fullBuffers;
	
/*		
	GetCtrlVal(tel_handle, PANEL_RA_INPUT, buff);
	sscanf(buff, "%d %d %lf", &ra.h, &ra.m, &ra.s);
	sprintf(buffer, "ra: %d %d %lf\n", ra.h, ra.m, ra.s) ;
	display_message(buffer) ;
*/
	sscanf(receiveBuf, "%d %d %lf %d %d %lf", &rah, &ram, &ras, &decd, &decm, &decs);
	sprintf(ra, "%d %d %lf\n", rah, ram, ras);
	sprintf(dec, "%d %d %lf\n", decd, decm, decs); 

    if (!sharedMemory)
            return 0;
       
            // Sent message by updating shared memory
    if (bServerApp)
    {
                // Send server message to all clients by adding text
                // to each client receive buffer
    	
		CmtGetLock(lock);
		fullBuffers = 0;
    
		if (sharedMemory->tcs.tcsProcessID) 
    	{
        	appendLen = strlen(ra);
			buffer = sharedMemory->tcs.cmd_x;
			currentLen = strlen(buffer);
			
			if (currentLen+appendLen<CLIENT_RECEIVE_BUFFER_SIZE)
        	{
            	strcat(buffer, ra);
            	strcat(buffer, "\n");
        	}    
        	else fullBuffers++;
        	{            
            	InsertTextBoxLine (panelHandle, PANEL_TRANSMITLIST, -1, ra);
			}
			
			appendLen = strlen(dec);
			buffer = sharedMemory->tcs.cmd_y;
			currentLen = strlen(buffer);
			
			if (currentLen+appendLen<CLIENT_RECEIVE_BUFFER_SIZE)
        	{
            	strcat(buffer, dec);
            	strcat(buffer, "\n");
        	}    
        	else fullBuffers++;
        	{            
            	InsertTextBoxLine (panelHandle, PANEL_TRANSMITLIST, -1, dec);
			}
					
			SetCtrlVal (panelHandle, PANEL_TRANSMIT, "");
		}
        
        if (fullBuffers)
            MessagePopup("Server Error", "Client receive buffer full, cannot transmit");
		
		CmtReleaseLock(lock);
	}
	
    return 0;
}

//----------------------------------------------------------------------------
// download SharedMemory to tcs (이건 tcs program에서 해야할 일 같은데) 
//----------------------------------------------------------------------------
int DownloadSHMToTcs()
{
	static char *buffer;
	
	SetCtrlVal (panelHandle, PANEL_CONNECTED, (bServerApp)?
            sharedMemory->tcsConnected:sharedMemory->serverRunning);
       
    CmtGetLock(lock);
    // Read and clear receive buffer
    if (bServerApp)
	{
        return 0;
	}
    else 
	{     
		buffer = sharedMemory->tcs.cmd_x;
		SetCtrlVal(panelHandle, PANEL_RA, buffer);
			 
		buffer = sharedMemory->tcs.cmd_y;
		SetCtrlVal(panelHandle, PANEL_DEC, buffer);
	}
            
        // InsertTextBoxLine (panel, PANEL_RECEIVELIST, -1, buffer);
    buffer[0] = 0;
                
    CmtReleaseLock(lock);
    
    return 0;
}

/*
//----------------------------------------------------------------------------
// download SharedMemory to Server 
//----------------------------------------------------------------------------
int DownloadSHMToServer()
{
	static char *buffer1;
	static char *buffer2;
	
    if (!sharedMemory)
        return 0;
            
    SetCtrlVal (panel, PANEL_CONNECTED, (bServerApp)?
        sharedMemory->tcsConnected:sharedMemory->serverRunning);
        
    CmtGetLock(lock);

//////////////////////////////////////////////////////   
	buffer1 = sharedMemory->server.now_ra;
	buffer2 = sharedMemory->server.now_dec;
	buffer3 = sharedMemory->server.now_az;
	buffer4 = sharedMemory->server.now_el;
	intbuffer1 = sharedMemory->server.cw_limit_az;
	intbuffer2 = sharedMemory->server.ccw_limit_az;
	intbuffer3 = sharedMemory->server.home_limit_az;
	intbuffer4 = sharedMemory->server.cw_limit_el;
	intbuffer5 = sharedMemory->server.ccw_limit_el;
	intbuffer6 = sharedMemory->server.home_limit_el;
	
	sum = buffer1 + buffer2 + ... + intbuffer6
		  
	CmtReleaseLock(lock);
	
	return sum
	// 스트링으로 모두 한데 모아서~~ tcp/ip로 전송
//////////////////////////////////////////////////////	
	
    CmtReleaseLock(lock);
    
	return 0;
}



//----------------------------------------------------------------------------
// upload tcs to SharedMemory   (이건 tcs program에서 해야할 일 같은데)
//----------------------------------------------------------------------------
int UploadTcsToSHM()
{
    
    return 0;
}
*/
//----------------------------------------------------------------------------
// ClearReceiveScreen
//----------------------------------------------------------------------------
int CVICALLBACK ClearReceiveScreen (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            ResetTextBox (panel, PANEL_RECEIVELIST, "");
            break;
    }
    return 0;
}

//----------------------------------------------------------------------------
// Quit
//----------------------------------------------------------------------------
int CVICALLBACK Quit (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) 
	{
        case EVENT_COMMIT:
            SetCtrlAttribute (panel, PANEL_TIMER, ATTR_ENABLED, 0);
            QuitUserInterface(0);
            break;
    }
    return 0;
}
