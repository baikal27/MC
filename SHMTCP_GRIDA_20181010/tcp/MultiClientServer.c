/*---------------------------------------------------------------------------*/
/*																			 */
/* FILE:    MultiClientServer.c												 */
/*                                                                           */
/* PURPOSE:	This TCP server program allows multiple TCP clients	to connect 	 */
/*			to it. The program allows the user to write to any of the		 */
/*			connected clients. The program receives and displays any data	 */
/*			sent by the clients. The program uses worker threads to manage	 */
/*			reading and writing to each client. You can use the TCP client	 */
/*			sample program to interact with this server.					 */
/*																			 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <tcpsupp.h>
#include <ansi_c.h>
#include <utility.h>
#include <toolbox.h>
#include "MultiClientServer.h"

/*---------------------------------------------------------------------------*/
/* Macros and constants                                                      */
/*---------------------------------------------------------------------------*/
#define SERVER_PORT_NUM		10000
#define tcpChk(f) if ((tcpErr=(f))<0) {ReportTCPError(tcpErr); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Type definitions                                                          */
/*---------------------------------------------------------------------------*/
typedef struct ClientInfo
{
	unsigned int	handle;			/* TCP conversation handle */
	unsigned int	threadId;		/* ID of worker thread */
	int				threadFuncId;	/* ID of worker thread function */
	int				stopFlag;		/* Flag used to stop worker thread */
	char			name[256];		/* Descriptive name of client connection */
	int				readingData;	/* Indicates server is reading from client */
	int				panel;			/* Handle of panel to display client data */
	int             threadInited;   /* Flag used to inform the server that the thread's initialization has completed */
} ClientInfo, *ClientInfoPtr;

/*---------------------------------------------------------------------------*/
/* Module-globals                                                            */
/*---------------------------------------------------------------------------*/
static int 			gPanel = 0;			/* Main user interface panel handle */
static ListType		gClientList = 0;	/* List of connected clients */

/*---------------------------------------------------------------------------*/
/* Internal function prototypes                                              */
/*---------------------------------------------------------------------------*/
static int ConnectClient (unsigned int handle);
static int DisconnectClient (unsigned int handle);
static int Disconnect (ClientInfoPtr clientInfoPtr, size_t index, int removeFromList);
static int CVICALLBACK ClientThreadFunction (void *data);
static int CVICALLBACK CompareClientInfoPtr (void *item1, void *item2);
static int CVICALLBACK DisconnectClientListItem (int index, void *itemPtr, 
												 void *data);
static int CVICALLBACK ServerCallback (unsigned int handle, int xType, 
									   int errCode, void *cbData);
static void CVICALLBACK DeferredSend (void *data);
static void CVICALLBACK DeferredReceive (void *data);
static void ReportTCPError (int error);

/*---------------------------------------------------------------------------*/
/* This is the application's entry-point.                                    */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	int		tcpErr = 0;
	int		unregisterServer = 0;
	
	if (InitCVIRTE(0, argv, 0) == 0)
		return -1;
	
	/* Load user interface */
	if ((gPanel = LoadPanel (0, "MultiClientServer.uir", PANEL)) < 0)
		return -1;

	/* Create list to manage client connections */
	gClientList = ListCreate (sizeof (ClientInfoPtr));
	
	/* Register the server */
	tcpChk (RegisterTCPServer (SERVER_PORT_NUM, ServerCallback, NULL));
	unregisterServer = 1;
	
	DisplayPanel (gPanel);
	RunUserInterface ();

	/* Clean up */
	if (gClientList)
		{
		ListApplyToEach (gClientList, 0, DisconnectClientListItem, 0);
		ListDispose (gClientList);
		}

Done:
	if (unregisterServer)
		UnregisterTCPServer (SERVER_PORT_NUM);
	DiscardPanel (gPanel);
	
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Respond to the panel closure to quit the UI loop.                         */
/*---------------------------------------------------------------------------*/
int CVICALLBACK PanelCallback (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
		}
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Send data to connected client.    	 	 		 						 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK SendCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	ClientInfoPtr		clientInfoPtr = NULL;
	long long			value;
	
	switch (event)
		{
		case EVENT_COMMIT:
			/*
			 * Get current client from listbox, and post the call 
			 * to its worker thread.
			 */
			GetCtrlVal (gPanel, PANEL_CLIENT_LIST, &value);
			clientInfoPtr = (ClientInfoPtr)(uintptr_t)value;
			PostDeferredCallToThread (DeferredSend, clientInfoPtr, 
				clientInfoPtr->threadId);
			break;
		}
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Disconnect a client.														 */ 
/*---------------------------------------------------------------------------*/
int CVICALLBACK DisconnectCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	ClientInfoPtr		clientInfoPtr = NULL;
	long long				value;
	size_t 				index;
	
	switch (event)
		{
		case EVENT_COMMIT:
			/* Get current client from listbox. */
			GetCtrlVal (gPanel, PANEL_CLIENT_LIST, &value);
			clientInfoPtr = (ClientInfoPtr)(uintptr_t)value;
			/* Find the index of the client in the client list. */
			index = ListFindItem (gClientList, &clientInfoPtr, 
				FRONT_OF_LIST, CompareClientInfoPtr);
			/* Disconnect the client. */
			if (index > 0)
				Disconnect (clientInfoPtr, index, 1);
			break;
		}
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Worker thread function for clients.										 */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK ClientThreadFunction (void *data)
{
	ClientInfoPtr clientInfoPtr = (ClientInfoPtr) data;
	
	clientInfoPtr->threadId = CmtGetCurrentThreadID ();

	/* Load a new panel for this client message. */
	clientInfoPtr->panel = LoadPanel (0, "MultiClientServer.uir", REC_PANEL);
	SetPanelAttribute (clientInfoPtr->panel, ATTR_CLOSE_ITEM_VISIBLE, 0);
	
	/* Set the client's name in the panel. */
	SetCtrlVal (clientInfoPtr->panel, REC_PANEL_CLIENT_NAME, 
		clientInfoPtr->name);
	DisplayPanel (clientInfoPtr->panel);

	/* Mark the thread as initialized. */
	clientInfoPtr->threadInited = 1;
	
	/* Process user-interface, TCP and other system events. */
	while (!clientInfoPtr->stopFlag)
		{
		ProcessSystemEvents ();
		}
	
	/* Discard the client panel. */
	DiscardPanel (clientInfoPtr->panel);
	
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Connects the client identified by handle.								 */
/*---------------------------------------------------------------------------*/

static int ConnectClient (unsigned int handle)
{
	int				tcpErr = 0;
	ClientInfoPtr	clientInfoPtr = 0;
	char			peerName[128], peerAddress[128];
	
	/* Create client information data-structure. */
	clientInfoPtr = calloc (1, sizeof (ClientInfo));
	if (clientInfoPtr == NULL)
		return -1;
	clientInfoPtr->handle = handle;
	
	/* Get descriptive name for client. */
	tcpChk (GetTCPPeerName (handle, peerName, sizeof (peerName)));
	tcpChk (GetTCPPeerAddr (handle, peerAddress, sizeof (peerAddress)));
	sprintf (clientInfoPtr->name, "Client name: %s, address: %s", 
		peerName, peerAddress);

	/* Create worker thread for this client. */
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, 
		ClientThreadFunction, clientInfoPtr, &clientInfoPtr->threadFuncId);
	
	/* Add the client to the list. */
	ListInsertItem (gClientList, &clientInfoPtr, END_OF_LIST);
	
	/* Add client to user interface and update disabled controls. */
	InsertListItem (gPanel, PANEL_CLIENT_LIST, -1, clientInfoPtr->name, 
		(long long)(uintptr_t)clientInfoPtr);
	SetCtrlAttribute (gPanel, PANEL_DISCONNECT, ATTR_DIMMED, 0);
	SetCtrlAttribute (gPanel, PANEL_SEND, ATTR_DIMMED, 0);
	SetCtrlAttribute (gPanel, PANEL_DATA, ATTR_DIMMED, 0);
	
Done:	
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Compares two client info data-structures.								 */
/* Used by the ListFindItem function.										 */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK CompareClientInfoPtr (void *item1, void *item2)
{
	return ((*(ClientInfoPtr *) item1)->handle 
				- (*(ClientInfoPtr *) item2)->handle);
}

/*---------------------------------------------------------------------------*/
/* Disconnects the client identified by the info data-structure.			 */
/*---------------------------------------------------------------------------*/
static int Disconnect (ClientInfoPtr clientInfoPtr, size_t index, int removeFromList)
{
	int numUIListItems;
	int	intIndex;
	
	/* Signal client's worker thread to stop. */
	clientInfoPtr->stopFlag = 1;
	/* Wait for client's worker thread to stop. */
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, 
		clientInfoPtr->threadFuncId, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	/* Release the client's worker thread function. */
	CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, 
		clientInfoPtr->threadFuncId);

	/* Disconnect the client conversation handle. */
	DisconnectTCPClient (clientInfoPtr->handle);
	
	if (removeFromList)
		ListRemoveItem (gClientList, NULL, index);

	/* Remove client entry from user interface and update controls. */
	GetIndexFromValue (gPanel, PANEL_CLIENT_LIST, &intIndex, (long long)(uintptr_t)clientInfoPtr);
	index = intIndex;
	DeleteListItem (gPanel, PANEL_CLIENT_LIST, intIndex, 1);
	GetNumListItems (gPanel, PANEL_CLIENT_LIST, &numUIListItems);
	SetCtrlAttribute (gPanel, PANEL_DISCONNECT, ATTR_DIMMED, numUIListItems == 0);
	SetCtrlAttribute (gPanel, PANEL_SEND, ATTR_DIMMED, numUIListItems == 0);
	SetCtrlAttribute (gPanel, PANEL_DATA, ATTR_DIMMED, numUIListItems == 0);
	
	/* Dispose client information data-structure. */
	free (clientInfoPtr);
	
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Disconnects the client identified by the handle.							 */
/*---------------------------------------------------------------------------*/
static int DisconnectClient (unsigned int handle)
{
	ClientInfo		clientInfo = {0};
	ClientInfoPtr	clientInfoPtr = &clientInfo;
	size_t			index;
	
	/* Find the client information from TCP conversation handle. */
	clientInfoPtr->handle = handle;
	index = ListFindItem (gClientList, &clientInfoPtr, 
		FRONT_OF_LIST, CompareClientInfoPtr);
	if (index > 0)
		{
		/* Get the stored client information and disconnect the client. */
		ListGetItem (gClientList, &clientInfoPtr, index);
		Disconnect (clientInfoPtr, (int)index, 1);
		}
		
	return 0;
}

/*---------------------------------------------------------------------------*/
/* TCP callback function for the server.									 */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK ServerCallback (unsigned int handle, int xType, 
									   int errCode, void *cbData)
{
	if (xType == TCP_CONNECT)
		{
		/* Connect new client. */
		ConnectClient (handle);
		}
	else if (xType == TCP_DISCONNECT)
		{
		/* Client is disconnecting. Update program state. */
		DisconnectClient (handle);
		}
	else if (xType == TCP_DATAREADY)
		{
		ClientInfo		clientInfo = {0};
		ClientInfoPtr	clientInfoPtr = &clientInfo;
		size_t			index;
	
		/* Find the client information from TCP conversation handle. */
		clientInfoPtr->handle = handle;
		index = ListFindItem (gClientList, &clientInfoPtr, 
			FRONT_OF_LIST, CompareClientInfoPtr);
		if (index > 0)
			{
			/* Get the stored client information. */
			ListGetItem (gClientList, &clientInfoPtr, index);
			
			/*
			 * NOTE - Because the reading is done in the worker thread,
			 * this thread (the main thread) is not blocked, and will 
			 * continue to receive TCP_DATAREADY events, until all the 
			 * data is read. This program uses the readingData flag to 
			 * ignore these events, until all the data is read by the 
			 * worker thread.
			 */
			if (clientInfoPtr->threadInited && !clientInfoPtr->readingData)
				{
				clientInfoPtr->readingData = 1;
				PostDeferredCallToThread (DeferredReceive, clientInfoPtr, 
					clientInfoPtr->threadId);
				}
			}
		}	

	return 0;
}

/*---------------------------------------------------------------------------*/
/* Disconnects an item in the connected clients list.						 */
/* Used by the ListApplyToEach function.									 */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK DisconnectClientListItem (int index, void *itemPtr, 
												 void *data)
{
	Disconnect (*(ClientInfo **) itemPtr, index, 0);
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Sends data to client.													 */
/*---------------------------------------------------------------------------*/
static void CVICALLBACK DeferredSend (void *data)
{
	int				tcpErr = 0;
	ClientInfoPtr	clientInfoPtr = (ClientInfoPtr) data;
	char			*dataBuf = NULL;
	int				dataBufSize;
	
	/* Get the size of the data to send. */
	GetCtrlAttribute (gPanel, PANEL_DATA, ATTR_STRING_TEXT_LENGTH, &dataBufSize);
	if (dataBufSize > 0)
		{
		/* Allocate a buffer to hold the data. */
		dataBuf = malloc (sizeof (char) * (++dataBufSize));
		if (dataBuf)
			{
			/* Pointer to current data to send. */
			char *currData = dataBuf;
			
			/* Read the data from the user interface. */
			GetCtrlVal (gPanel, PANEL_DATA, dataBuf);
			
			/* Write the data in a loop, until there is no more data to send. */
			while (dataBufSize > 0)
				{
				int bytesSent = ServerTCPWrite (clientInfoPtr->handle, 
					currData, dataBufSize, 0);
				if (bytesSent >= 0)
					{
					/* Subtract number of bytes sent from amount of data left. */
					dataBufSize -= bytesSent;
					/* Update current data. */
					currData += bytesSent;
					}
				else
					tcpChk (bytesSent);
				}
				
			/* Free the data buffer. */
			free (dataBuf);
			}
		}
Done:
	return;
}

/*---------------------------------------------------------------------------*/
/* Receives data from client.												 */
/*---------------------------------------------------------------------------*/
static void CVICALLBACK DeferredReceive (void *data)
{
	ClientInfoPtr	clientInfoPtr = (ClientInfoPtr) data;
	char			dataBuf[256];
	int				bytesRead;
	
	assert (clientInfoPtr->readingData == 1);
	
	/*
	 * Disable library error checking as we are going to read until
	 * there is no more data left (read call times out).
	 */
	DisableBreakOnLibraryErrors ();
	while (1)
		{
		bytesRead = ServerTCPRead (clientInfoPtr->handle, 
			dataBuf, sizeof (dataBuf) - 1, 100);
		if (bytesRead > 0)
			{
			/* Update user interface with the new data. */
			dataBuf [bytesRead] = '\0';
			SetCtrlVal (clientInfoPtr->panel, REC_PANEL_DATA, dataBuf);
			}
		else
			{
			/* No more data to read. Update flag, and exit loop. */
			clientInfoPtr->readingData = 0;
			break;
			}
		}
	/* Enable library error checking. */
	EnableBreakOnLibraryErrors ();
}

/*---------------------------------------------------------------------------*/
/* Report TCP Errors.                      								 	 */
/*---------------------------------------------------------------------------*/
static void ReportTCPError (int error)
{
	char	messageBuffer[1024];

	if (error < 0)
		{
		sprintf(messageBuffer, 
			"TCP library error message: %s\nSystem error message: %s", 
			GetTCPErrorString (error), GetTCPSystemErrorString());
		MessagePopup ("Error", messageBuffer);
		}
}
/*---------------------------------------------------------------------------*/
