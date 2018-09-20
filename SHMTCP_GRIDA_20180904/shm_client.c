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

/*---------------------------------------------------------------------------*/
/* Macros for TCP/IP			                                             */
/*---------------------------------------------------------------------------*/
#define tcpChk(f) if ((g_TCPError=(f)) < 0) {ReportTCPError(); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Macros for SharedMemory                             */
/*---------------------------------------------------------------------------*/
#define MAXCLIENTS 10
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
int TransmitTCPIP (char transmitBuf[]);

/*---------------------------------------------------------------------------*/
/* Internal function prototypes for SharedMemory                             */
/*---------------------------------------------------------------------------*/
int UploadServerToSHM(const char data[]);
int DownloadSHMToTcs(void);
int UploadTcsToSHM();
int DownloadSHMToServer(char text_buffer[]);

/*---------------------------------------------------------------------------*/
/* Typedefs
/*---------------------------------------------------------------------------*/

typedef struct clientAreaRec
{
    DWORD clientProcessID;
    char clientReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
} tClientArea;

typedef struct ServerAreaRec
{
//	char now_az[COORD_BUFFER_SIZE], now_el[COORD_BUFFER_SIZE];
//	char now_ra[COORD_BUFFER_SIZE], now_dec[COORD_BUFFER_SIZE];
	int now_az_h, now_az_m, now_el_d, now_el_m;
	double now_az_s, now_el_s;
	int now_ra_h, now_ra_m, now_dec_d, now_dec_m;
	double now_ra_s, now_dec_s;
	int cw_limit_az, ccw_limit_az, home_limit_az, cw_limit_el, ccw_limit_el, home_limit_el;
} tServerArea;

typedef struct tcsAreaRec
{
	DWORD tcsProcessID;
	char tcsReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
	int cmd_kmap;
//	char cmd_x[COORD_BUFFER_SIZE], cmd_y[COORD_BUFFER_SIZE];
	int cmd_x_h, cmd_x_m, cmd_y_d, cmd_y_m;
	double cmd_x_s, cmd_y_s;
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
//static char tcpBuffer[1024] ={0};

/*---------------------------------------------------------------------------*/
/* Module-globals for TCP/IP                                                 */
/*---------------------------------------------------------------------------*/
static unsigned int tcp_hconversation;
static int			g_TCPError = 0;
static int      	portNum;
static int		 	tcp_registered = 0;

/*---------------------------------------------------------------------------*/
/* Prototypes																 */
/*---------------------------------------------------------------------------*/
tSharedMemType *CreateOrOpenMapFile(HANDLE *hMemMapFile, char *mapName, int create, int size, int *alreadyExist);		// 구조체의 pointer를 가지고, 각 요소에 접근하겠다는 심산.
int CloseMapping(HANDLE *hMemMapFile, void **memPtr);
int SetupServerOrClient(void);
int CleanupServerOrClient(void);

/*---------------------------------------------------------------------------*/
/* Main																		 */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
    int status;
    
    if (InitCVIRTE (0, argv, 0) == 0)    /* Initialize CVI libraries */
        return -1;    /* out of memory */
        

        bServerApp = 0;

    
    // Display and run panel    
    if ((panelHandle = LoadPanel (0, "sharemem.uir", PANEL)) < 0)
    {
        MessagePopup("Server Error","Failed to load UIR panel.");
        goto Done;
    }    
    
    // Update UIR panel title
    sprintf(msgBuffer, "SHAREMEM Sample %s (Process ID:%8x)", 
        "Client", 
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
    
    // Cleanup
    CleanupServerOrClient();
    CloseMapping(&hMemMapFile, (void **)(&sharedMemory));
    CmtDiscardLock(lock);
    DiscardPanel(panelHandle);
    
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
//	ssize_t tcpSize 		= sizeof (tcpBuffer) - 1;
	char addrBuf[31];
	char portBuf[31];

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
//					tcpChk (GetTCPPeerName (tcp_hconversation, receiveBuf, 256));
//					SetCtrlVal (panelHandle, PANEL_CLIENT_NAME, receiveBuf);
					if (portNum)
					{
						sprintf(portBuf, "%d", portNum);
						SetCtrlVal (panelHandle, PANEL_CLIENT_PORTNUM, portBuf);
					}
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
//					SetCtrlVal (panelHandle, PANEL_CLIENT_NAME, "");
					SetCtrlVal (panelHandle, PANEL_CLIENT_PORTNUM, "");
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
	char    tempBuf[256] = {0};
	char	portBuf[31];
    
    if (CmtNewLock(LOCK_NAME, OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &lock) != 0)
    {
    	if (bServerApp)
    		MessagePopup("Server Error", "Failed to create Server lock.");
    	else
    		MessagePopup("Client Error", "Failed to create Client lock.");
    	goto Done;
    }
    if (bServerApp)
    	CmtGetLock(lock);
        
    // Create or open shared memory    
    if (!(sharedMemory = CreateOrOpenMapFile(&hMemMapFile, MEMORY_NAME, bServerApp, SHARED_MEMORY_SIZE, &alreadyExist)))
    {
        if (bServerApp) {
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
			
			// setting up the TCP/IP environment
			//Prompt for the port number on which to receive connections 
			PromptPopup ("Port Number?",
						 "Type the port number on which you would like to register "
						 "this server application.\n\n(example: 10000)",
						 tempBuf, 31);
			portNum = atoi (tempBuf);

			//Attempt to register as a TCP server... 
			SetWaitCursor (1);
			if (RegisterTCPServer (portNum, ServerTCPCB, 0) < 0)
				MessagePopup("TCP Server", "TCP Server registration failed!");
			else
			{
				tcp_registered = 1;
				SetWaitCursor (0);
		
				//We are successfully registered -- gather info 
				SetCtrlVal (panelHandle, PANEL_ONLINE, 1);
				if (GetTCPHostAddr (tempBuf, 256) >= 0)
	        		SetCtrlVal (panelHandle, PANEL_SERVER_IP, tempBuf);
//				if (GetTCPHostName (tempBuf, 256) >= 0)
//	        		SetCtrlVal (panelHandle, PANEL_SERVER_NAME, tempBuf);
				if (portNum)
					sprintf(portBuf, "%d", portNum);
					SetCtrlVal (panelHandle, PANEL_SERVER_PORTNUM, portBuf);
				SetCtrlAttribute (panelHandle, PANEL_TRANSMIT, ATTR_DIMMED, 1);
				SetCtrlVal (panelHandle, PANEL_TCP_CONNECTED, 0); 
		
				//Display the panel and run the UI 
				//SetActiveCtrl (panelHandle, PANEL_TRANSMIT);
			}
			
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
           	success = 1;								 
			CmtReleaseLock(lock);
            
        }
    }
    else
        CmtReleaseLock(lock);
    
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
// TimerCallback
//----------------------------------------------------------------------------
int CVICALLBACK TimerCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    static char *buffer;
	static char text_buffer[256]={0};

    
    switch (event) 
    {
      case EVENT_TIMER_TICK:
        if (!sharedMemory)
            return 0;
	
		
		SetCtrlVal (panel, PANEL_CONNECTED, (bServerApp)?
            sharedMemory->tcsConnected:sharedMemory->serverRunning);
									 
		if (!bServerApp)
		{
			
			DownloadSHMToTcs();
			UploadTcsToSHM(); 
			
		}
		else
		{
			UploadTcsToSHM();
			DownloadSHMToServer(text_buffer);
			InsertTextBoxLine (panel, PANEL_RECEIVELIST, -1, text_buffer);
			TransmitTCPIP(text_buffer);
		}
		
        break;
    }
    return 0;
}


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
    switch (event) {
        case EVENT_COMMIT:
            SetCtrlAttribute (panel, PANEL_TIMER, ATTR_ENABLED, 0);
            QuitUserInterface(0);
            break;
    }
    return 0;
}

//----------------------------------------------------------------------------
// Server to SharedMemory upload
//----------------------------------------------------------------------------
int UploadServerToSHM(const char data[])
{
//	char receiveBuf[256] = {0};
	ssize_t dataSize        = sizeof (data) - 1;
	char addrBuf[31];
	int rah, ram, decd, decm;
	double ras, decs;
	
	static int i;
    static size_t currentLen, appendLen;
    static char *buffer;
    static int fullBuffers;
	
	sscanf(data, "%d %d %lf %d %d %lf", &rah, &ram, &ras, &decd, &decm, &decs);

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

			sharedMemory->tcs.cmd_x_h = rah ;
			sharedMemory->tcs.cmd_x_m = ram ;
			sharedMemory->tcs.cmd_x_s = ras ;
			sharedMemory->tcs.cmd_y_d = decd ;
			sharedMemory->tcs.cmd_y_m = decm ;
			sharedMemory->tcs.cmd_y_s = decs ;
		}
/*
//			currentLen = strlen(buffer);
			
			if (currentLen+appendLen<CLIENT_RECEIVE_BUFFER_SIZE)
        	{
//            	strcat(buffer, ra);
//           	strcat(buffer, "\n");
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
		
        
        if (fullBuffers)
            MessagePopup("Server Error", "Client receive buffer full, cannot transmit");
*/
	
		CmtReleaseLock(lock);
	}
	
    return 0;
}

//----------------------------------------------------------------------------
// download SharedMemory to tcs (이건 tcs program에서 해야할 일 같은데) 
//----------------------------------------------------------------------------
int DownloadSHMToTcs()
{
	static char text_buff[256];
	int rah, ram, decd, decm;
	double ras, decs;
	
	if (!sharedMemory)
        return 0;

	if (!bServerApp)
    {
                // Send server message to all clients by adding text
                // to each client receive buffer
    	
		CmtGetLock(lock);
		if (sharedMemory->tcs.tcsProcessID)
		{
			// Coordinate
			rah = sharedMemory->tcs.cmd_x_h ;
			ram = sharedMemory->tcs.cmd_x_m ;
			ras = sharedMemory->tcs.cmd_x_s ;
			decd = sharedMemory->tcs.cmd_y_d ;
			decm = sharedMemory->tcs.cmd_y_m ;
			decs = sharedMemory->tcs.cmd_y_s ;
				
			sprintf(text_buff, "%2d %2d %3.1lf\n", rah, ram, ras) ;
			SetCtrlVal(panelHandle, PANEL_RA, text_buff) ;
				
			sprintf(text_buff, "%2d %2d %3.1lf\n", decd, decm, decs) ;
			SetCtrlVal(panelHandle, PANEL_DEC, text_buff) ;
			// Coordinate
		
		}				  
		CmtReleaseLock(lock);
	}
		
	return 0;
}

//----------------------------------------------------------------------------
// TCS to SharedMemory upload		 ; 이건 TCS 프로그램에서 구현해야 할 것.
//----------------------------------------------------------------------------
int UploadTcsToSHM()
{
	int now_azh, now_azm, now_eld, now_elm;
	double now_azs, now_el_s;
	int now_rah, now_ram, now_decd, now_decm;
	double now_ras, now_decs;
	static char text_buff[256];
	
	GetCtrlVal(panelHandle, PANEL_RA, text_buff);
	sscanf(text_buff, "%d %d %lf", &now_rah, &now_ram, &now_ras);
	GetCtrlVal(panelHandle, PANEL_DEC, text_buff);
	sscanf(text_buff, "%d %d %lf", &now_decd, &now_decm, &now_decs);

    if (!sharedMemory)
            return 0;
       
            // Sent message by updating shared memory
    if (!bServerApp)
    {
                // Send server message to all clients by adding text
                // to each client receive buffer
    	
		CmtGetLock(lock);
    
		if (sharedMemory->tcs.tcsProcessID) 
    	{

			sharedMemory->server.now_ra_h = now_rah ;
			sharedMemory->server.now_ra_m = now_ram ;
			sharedMemory->server.now_ra_s = now_ras ;
			sharedMemory->server.now_dec_d = now_decd ;
			sharedMemory->server.now_dec_m = now_decm ;
			sharedMemory->server.now_dec_s = now_decs ;
		}
	
		CmtReleaseLock(lock);
	}
	
    return 0;
}

//----------------------------------------------------------------------------
// download SharedMemory to server  
//----------------------------------------------------------------------------
int DownloadSHMToServer(char text_buff[])
{
	int now_azh, now_azm, now_eld, now_elm;
	double now_azs, now_el_s;
	int now_rah, now_ram, now_decd, now_decm;
	double now_ras, now_decs;
//	static char text_buff[256];

	if (!sharedMemory)
            return 0;
       
            // Sent message by updating shared memory
    if (bServerApp)
    {
		
		CmtGetLock(lock);
		if (sharedMemory->tcs.tcsProcessID)
		{
			// Coordinate
			now_rah = sharedMemory->server.now_ra_h ;
			now_ram = sharedMemory->server.now_ra_m ;
			now_ras = sharedMemory->server.now_ra_s ;
			now_decd = sharedMemory->server.now_dec_d ;
			now_decm = sharedMemory->server.now_dec_m ;
			now_decs = sharedMemory->server.now_dec_s ;
				
			sprintf(text_buff, "%2d %2d %3.1lf %2d %2d %3.1lf\n", now_rah, now_ram, now_ras, now_decd, now_decm, now_decs) ;			
//			sprintf(buff, "%2d %2d %3.1lf\n", now_rah, now_ram, now_ras) ;
//			strcat(buff, "%2d %2d %3.1lf\n", now_decd, now_decm, now_decs) ;
//			strcat(buff, "\0");
			// Coordinate
		}
		
	}				  
	CmtReleaseLock(lock);
		
	return 0;
}

//---------------------------------------------------------------------------//
// When the user hits ENTER after typing some text, send it to the client... //
// Note that the string control will be dimmed unless there is a client      //
// Connected.                                                                //
//---------------------------------------------------------------------------//
int TransmitTCPIP (char transmitBuf[])
{
//	GetCtrlVal (panelHandle, MAINPNL_STRING, transmitBuf);
//	strcat (transmitBuf, "\n");
//	SetCtrlVal (panelHandle, MAINPNL_TRANSMIT, transmitBuf);
//	SetCtrlVal (panelHandle, MAINPNL_STRING, "");
	if (bServerApp)
	{
		if (tcp_hconversation)
		{
			if (ServerTCPWrite (tcp_hconversation, transmitBuf,
						strlen (transmitBuf), 1000) < 0)
				SetCtrlVal (panelHandle, PANEL_TRANSMITLIST,
						"Transmit Error\n");
		}				
	}
	return 0;
}
