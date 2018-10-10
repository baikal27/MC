/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    MessageReader.c                                                  */
/*                                                                           */
/* PURPOSE: This TCP client shows you how to get data of varying sizes,      */
/*          where the size is encoded in the data.                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <tcpsupp.h>
#include <ansi_c.h>
#include <utility.h>
#include <cvirte.h>     
#include <userint.h>
#include "MessageReader.h"

/*---------------------------------------------------------------------------*/
/* Macros                                                                    */
/*---------------------------------------------------------------------------*/
#define tcpChk(f) if ((gTCPError=(f)) < 0) {ReportTCPError(); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Constants                                                                 */
/*---------------------------------------------------------------------------*/
#define SERVER_NAME     "localhost"
#define SERVER_PORT     ((unsigned int)10000)

/*---------------------------------------------------------------------------*/
/* Global variables                                                          */
/*---------------------------------------------------------------------------*/
static int              gPanel = 0;
static int              gTCPError = 0;
static int				gConnected = 0;
static unsigned int     gConversationHandle;

/*---------------------------------------------------------------------------*/
/* Internal function prototypes                                              */
/*---------------------------------------------------------------------------*/
int CVICALLBACK TCPCallback (unsigned handle, int event, int error, 
                             void *callbackData);
static void ReportTCPError (void);

/*---------------------------------------------------------------------------*/
/* This is the application's entry-point.                                    */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        goto Done;

    /* Initialize TCP client. */
    if (ConnectToTCPServer (&gConversationHandle, SERVER_PORT, 
        SERVER_NAME, TCPCallback, NULL, 0) < 0)
        MessagePopup ("Error", "Could not connect to server!\n"
            "Launch the MessageWriter program before running this program.");
    else
		gConnected = 1;
	
    /* Load and initialize user interface. */
    if ((gPanel = LoadPanel (0, "MessageReader.uir", PANEL)) < 0)
        goto Done;
    DisplayPanel (gPanel);

    /* Run program. */
    RunUserInterface ();
    
Done:   
    /* Clean up */
    if (gConnected)
        DisconnectFromTCPServer (gConversationHandle);
    if (gPanel)
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
/* Handle incoming data and dropped TCP connections.                         */
/*---------------------------------------------------------------------------*/
int CVICALLBACK TCPCallback (unsigned handle, int event, int error, 
                             void *callbackData)
{
    size_t     bytesToRead, bytesRead, dataSize;
    int     numPoints;
    char    *data = NULL;

    switch (event)
        {
        case TCP_DATAREADY:
            /* Read the number of points in this data. */
            tcpChk (ClientTCPRead (gConversationHandle, &numPoints, 
                sizeof (numPoints), 0));
            
            /* Clear the graph and update number of points. */
            DeleteGraphPlot (gPanel, PANEL_GRAPH, -1, 0);
            SetCtrlVal (gPanel, PANEL_NUM_POINTS, numPoints);
            
            /* Read the data. Note that the data could be empty. */
            if (numPoints > 0)
                {
                dataSize = bytesToRead = sizeof (int) * numPoints;
                data = malloc (dataSize);
                if (data == NULL)
                    {
                    MessagePopup ("Error", "Could not allocate memory for data!");
                    goto Done;
                    }
                
                /*
                 * NOTE:
                 *      It is possible that not all the data has
                 *      been received. So we want to ignore timeout
                 *      errors, and continue reading until we get
                 *      all the data.
                 */
                DisableBreakOnLibraryErrors ();
                while (bytesToRead > 0)
                    {
                    bytesRead = ClientTCPRead (gConversationHandle, 
                        &data[dataSize - bytesToRead], bytesToRead, 0);
                    if (bytesRead > 0)
                        bytesToRead -= bytesRead;
                    else if (bytesRead != -kTCP_TimeOutErr)
                        tcpChk ((int)bytesToRead);
                    }
                EnableBreakOnLibraryErrors ();
                
                /* Display the data */
                PlotY (gPanel, PANEL_GRAPH, (int *) data, numPoints, 
                    VAL_INTEGER, VAL_CONNECTED_POINTS, VAL_EMPTY_SQUARE, 
                    VAL_SOLID, 1, VAL_YELLOW);
                }
            break;
        case TCP_DISCONNECT:
            if (handle == gConversationHandle)
                {
                /* The server we were talking to has disconnected. */
                MessagePopup ("Message", "Server has disconnected!");
				gConnected = 0;
				QuitUserInterface (0);
                }
            break;
    }

Done:
    free (data);
    return 0;
}

/*---------------------------------------------------------------------------*/
/* Report TCP Errors.                                                        */
/*---------------------------------------------------------------------------*/
static void ReportTCPError (void)
{
    char    messageBuffer[1024];

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
