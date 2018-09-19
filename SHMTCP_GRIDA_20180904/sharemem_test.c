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


//----------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------
#define MAXCLIENTS 10
#define CLIENT_RECEIVE_BUFFER_SIZE 1024
#define SERVER_RECEIVE_BUFFER_SIZE 2048
#define LOCK_NAME  "SHAREDLOCK"
#define MEMORY_NAME "SHAREDMEMORY"
#define SHARED_MEMORY_SIZE (int)sizeof(tSharedMemType)


//----------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------
typedef struct clientAreaRec
{
    DWORD clientProcessID;
    char clientReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
} tClientArea;

typedef struct sharedMemTypeRec
{
    int TCP_Running;
    int TCS_Running;

	int kmap;	// 1: AZ/EL, 2:RA/DEC
	double cmd_az, cmd_el;
	double cmd_ra, cmd_dec;
	double now_az, now_el;
	double now_ra, now_dec;
	   	// telescope status structure (limit, home position, stow, error, ....)
} tSharedMemType;

typedef struct TCPcommand
{
	int rx_kmap;
	double rx_az, rx_el;
	double rx_ra, rx_dec;
	//.....
} tTCPcommand;

//----------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------
static int panelHandle;
static CmtThreadLockHandle lock = 0;
static HANDLE hMemMapFile = 0;
static int clientIndex    = -1;
static int bServerApp     = 0;
static DWORD dwWaitResult = 0;
static tSharedMemType *sharedMemory = NULL;
static tTCPcommand rxTCP=[0,];
static char msgBuffer[256];


//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------
tSharedMemType *CreateOrOpenMapFile(HANDLE *hMemMapFile, char *mapName, int create, int size, int *alreadyExist);		// 구조체의 pointer를 가지고, 각 요소에 접근하겠다는 심산.
int CloseMapping(HANDLE *hMemMapFile, void **memPtr);

//----------------------------------------------------------------------------
// Main (TCP/IP Server)
//----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
    int status;
    
    if (InitCVIRTE (0, argv, 0) == 0)    /* Initialize CVI libraries */
        return -1;    /* out of memory */
        
 
    // Setup Shared memory
    if (CmtNewLock(LOCK_NAME, OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &lock) != 0)
    {
    		MessagePopup("Shared Memory Error", "Failed to create lock.");
    	goto Done;
    }
    CmtGetLock(lock);
       
    // Create or open shared memory    
    if (!(sharedMemory = CreateOrOpenMapFile(&hMemMapFile, MEMORY_NAME, bServerApp, SHARED_MEMORY_SIZE, &alreadyExist)))
    {
		MessagePopup("Shared Memory Error","Cannot create memory");
        goto Done;
    }
        
    // Initialize shared memory if creator, and release lock
    if (sharedMemory)
    {
        sharedMemory->TCP_Running=1;
        CmtReleaseLock(lock);
        
    }
    else
        CmtReleaseLock(lock);
    
	//TCP Server...... Process
   
    // Cleanup
    shredMemory->TCP_Running=0;
    CloseMapping(&hMemMapFile, (void **)(&sharedMemory));
    CmtDiscardLock(lock);
    DiscardPanel(panelHandle);
    
Done:
    return 0;
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
// TransmitCallback (To send command to TCS)
//----------------------------------------------------------------------------
int WriteToMemory(int kmap, double cmd_x, double cmd_y)
{
    CmtGetLock(lock);
           
	sharedMemory->kmap=kmap;
	switch(kmap){
		case 1:	// az/el type
			sharedMemory->cmd_az=cmd_x;
			sharedMemory->cmd_el=cmd_y;
			break;
		case 2:	// ra/dec type
			sharedMemory->cmd_ra=cmd_x;
			sharedMemory->cmd_dec=cmd_y;
			break;
	}
	CmtReleaseLock(lock);
         
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

