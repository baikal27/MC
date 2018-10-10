/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    MessageWriter.c                                                  */
/*                                                                           */
/* PURPOSE: This TCP server shows you how to send data of varying sizes with */
/*			the size encoded in the data.									 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <tcpsupp.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "MessageWriter.h"

/*---------------------------------------------------------------------------*/
/* Macros						                                             */
/*---------------------------------------------------------------------------*/
#define tcpChk(f) if ((gTCPError=(f)) < 0) {ReportTCPError(); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Constants                                              					 */
/*---------------------------------------------------------------------------*/
#define SERVER_PORT		((unsigned int)10000)

/*---------------------------------------------------------------------------*/
/* Global variables				                                             */
/*---------------------------------------------------------------------------*/
static int 				gPanel = 0;
static int				gTCPError = 0;
static int				gConnected = 0;
static unsigned int		gConversationHandle;

/*---------------------------------------------------------------------------*/
/* Internal function prototypes                                              */
/*---------------------------------------------------------------------------*/
int CVICALLBACK TCPCallback (unsigned handle, int event, int error, 
							 void *callbackData);
static void ReportTCPError (void);
static void UpdateUserInterface (void);
                             
/*---------------------------------------------------------------------------*/
/* This is the application's entry-point.                                    */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	int		serverRegistered = 0;
	
	if (InitCVIRTE (0, argv, 0) == 0)
		goto Done;
	
	/* Initialize TCP server. */
	tcpChk (RegisterTCPServer (SERVER_PORT, TCPCallback, NULL));
	serverRegistered = 1;
	
	/* Load and initialize user interface. */
	if ((gPanel = LoadPanel (0, "MessageWriter.uir", PANEL)) < 0)
		goto Done;
	UpdateUserInterface ();
	DisplayPanel (gPanel);
	
	/* Run program. */
	RunUserInterface ();
	
Done:	
	/* Clean up */
	if (gPanel)
		DiscardPanel (gPanel);
	if (serverRegistered)
		UnregisterTCPServer (SERVER_PORT);
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
/* Read the size, generate data, and send it to connected client.            */
/*---------------------------------------------------------------------------*/
int CVICALLBACK SendCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		numPoints;
	size_t	dataSize;
	int		*data = NULL;
	int		index;
	size_t	bytesToWrite;
	int		bytesWritten;
	
	switch (event)
		{
		case EVENT_COMMIT:
			/* Get the number of points to write and allocate memory. */
			GetCtrlVal (gPanel, PANEL_NUM_POINTS, &numPoints);
			dataSize = (numPoints + 1) * sizeof (int);
			data = malloc (dataSize);
			if (data == NULL)
				{
				MessagePopup ("Error", "Could not allocate memory for data!");
				goto Done;
				}
				
			/* Set the number of points and generate the data. */
			data[0] = numPoints;
			for (index = 1; index <= numPoints; ++index)
				data[index] = rand() % 100;
				
			/* Send all the data. */
			bytesToWrite = dataSize;
			while (bytesToWrite > 0)
				{
				tcpChk (bytesWritten = ServerTCPWrite (gConversationHandle, 
					&data[dataSize - bytesToWrite], bytesToWrite, 0));
				bytesToWrite -= bytesWritten;
				}

			/* Display the data. */
			DeleteGraphPlot (gPanel, PANEL_GRAPH, -1, 0);
			PlotY (gPanel, PANEL_GRAPH, &data[1], numPoints, VAL_INTEGER,
				VAL_CONNECTED_POINTS, VAL_EMPTY_SQUARE, 
				VAL_SOLID, 1, VAL_YELLOW);
			
			break;
		}
		
Done:
	free (data);
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Handle incoming and dropped TCP connections.                     		 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK TCPCallback (unsigned handle, int event, int error, 
							 void *callbackData)
{
    switch (event)
        {
        case TCP_CONNECT:
            if (gConnected)
                {
                /* We already have one client; don't accept another. */
                tcpChk (DisconnectTCPClient (handle));
                }
            else
                {
                /* Handle this new client connection */
                gConversationHandle = handle;
                /*
                 * Set the disconect mode, so we do not need to close 
                 * the connection ourselves.
                 */
                tcpChk (SetTCPDisconnectMode (gConversationHandle, 
                	TCP_DISCONNECT_AUTO));
                gConnected = 1;
                UpdateUserInterface ();
                }
            break;
        case TCP_DATAREADY:
        	/* Ignore data sent to the writer. */
            break;
        case TCP_DISCONNECT:
            if (handle == gConversationHandle)
                {
                /* The client we were talking to has disconnected. */
                gConnected = 0;
                UpdateUserInterface ();
                }
            break;
    }

Done:    
    return 0;
}

/*---------------------------------------------------------------------------*/
/* Report TCP Errors.                      								 	 */
/*---------------------------------------------------------------------------*/
static void ReportTCPError (void)
{
	char	messageBuffer[1024];

	if (gTCPError < 0)
		{
		sprintf(messageBuffer, 
			"TCP library error message: %s\nSystem error message: %s", 
			GetTCPErrorString (gTCPError), GetTCPSystemErrorString());
		MessagePopup ("Error", messageBuffer);
		gTCPError = 0;
		}
}

/*---------------------------------------------------------------------------*/
/* Update user interface dimming.           								 */
/*---------------------------------------------------------------------------*/
static void UpdateUserInterface (void)
{
	DeleteGraphPlot (gPanel, PANEL_GRAPH, -1, 1);
	SetCtrlAttribute (gPanel, PANEL_SEND, ATTR_DIMMED, !gConnected);
	SetCtrlAttribute (gPanel, PANEL_NUM_POINTS, ATTR_DIMMED, !gConnected);
}

/*---------------------------------------------------------------------------*/
