/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    MultiThreadingClient.c                                           */
/*                                                                           */
/* PURPOSE: This program shows how to create and run a TCP client in its own */
/*			thread, and thus separate TCP tasks from user interface tasks.	 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <analysis.h>
#include <ansi_c.h>
#include <utility.h>
#include <tcpsupp.h>
#include <cvirte.h>		
#include <userint.h>
#include "MultiThreadingClient.h"

/*---------------------------------------------------------------------------*/
/* Macros and constants                                                      */
/*---------------------------------------------------------------------------*/
#define SERVER_PORT_NUM		10000
#define tcpChk(f) if ((tcpErr=(f))<0) {ReportTCPError(tcpErr); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Module-globals                                                            */
/*---------------------------------------------------------------------------*/
static int				gPanel = 0;
static int				gRunning = 0;
static int				gTCPThreadFunctionId = 0;
static unsigned int		gTCPConnectionHandle;
static int				gTCPConnected = 0;
static double			gDelay = 0.1;

/*---------------------------------------------------------------------------*/
/* Internal function prototypes                                              */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK TCPThreadFunction (void *functionData);
static int CVICALLBACK TCPCallback (unsigned int handle, int xType, 
	int errCode, void *callbackData);
static void Connect (void);
static void Disconnect (void);
static void ReportTCPError (int error);

/*---------------------------------------------------------------------------*/
/* This is the application's entry-point.                                    */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		goto Done;
	if ((gPanel = LoadPanel (0, "MultiThreadingClient.uir", PANEL)) < 0)
		goto Done;
	DisplayPanel (gPanel);
	RunUserInterface ();
	
Done:
	if (gPanel > 0)
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
			/* If there is an existing client, close it. */
			if (gRunning)
				RunCallback (gPanel, PANEL_RUN, EVENT_COMMIT, 0, 0, 0);
			/* Quit the user interface loop. */
			QuitUserInterface (0);
			break;
		}
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Create and run a new client, or stop and close current client.    	 	 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK RunCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		multiThreaded;
	
	switch (event)
		{
		case EVENT_COMMIT:
			if (gRunning)
				{
				/* Client is already running. Stop and close it. */
				gRunning = 0;
				if (gTCPThreadFunctionId != 0)
					{
					/*
					 * Client is running on its own thread.
					 * Wait for TCP thread to exit and clean up.
					 */
					CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
						gTCPThreadFunctionId, 0);
					CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE,
						gTCPThreadFunctionId);
					gTCPThreadFunctionId = 0;
					}
				else
					{
					/* Client is running on UI thread. Disconnect it. */
					Disconnect ();
					}
				/* Update user interface */
				SetCtrlAttribute (gPanel, PANEL_MULTITHREAD, ATTR_DIMMED, 0);
				SetCtrlAttribute (gPanel, PANEL_RUN, ATTR_LABEL_TEXT, "__Start");
				}
			else
				{
				/* No existing client. Run a new client. */
				gRunning = 1;
				GetCtrlVal (gPanel, PANEL_MULTITHREAD, &multiThreaded);
				SetCtrlAttribute (gPanel, PANEL_MULTITHREAD, ATTR_DIMMED, 1);
				SetCtrlAttribute (gPanel, PANEL_RUN, ATTR_LABEL_TEXT, "__Stop");
				if (multiThreaded)
					{
					/* Run client on a new TCP thread. */
					CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, 
						TCPThreadFunction, NULL, &gTCPThreadFunctionId);
					}
				else
					{
					/* Run client on user interface thread. */
					Connect ();
					}
				}
			break;
		}
		
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Block the user interface thread.                          				 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK DelayCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double delay;
	
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (gPanel, PANEL_DELAYTIME, &delay);
			Delay (delay);
			break;
		}
	return 0;
}

/*---------------------------------------------------------------------------*/
/* Connect to the TCP server.                         						 */
/*---------------------------------------------------------------------------*/
static void Connect (void)
{
	int		tcpErr = 0;
	
	tcpChk (ConnectToTCPServer (&gTCPConnectionHandle, SERVER_PORT_NUM, NULL, 
		TCPCallback, 0, 0));
	gTCPConnected = 1;
	
Done:
	return;
}

/*---------------------------------------------------------------------------*/
/* Disconnect from the TCP server.                         					 */
/*---------------------------------------------------------------------------*/
static void Disconnect (void)
{
	int		tcpErr = 0;
	
	if (gTCPConnected)
		{
		gTCPConnected = 0;
		DisconnectFromTCPServer (gTCPConnectionHandle);
		}

Done:
	return;
}

/*---------------------------------------------------------------------------*/
/* TCP thread entry-point function.                         				 */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK TCPThreadFunction (void *functionData)
{
	Connect ();
	while (gRunning)
		{
		ProcessSystemEvents ();
		}
	Disconnect ();
	return 0;
}

/*---------------------------------------------------------------------------*/
/* TCP callback. This will be called in the thread that connected to server. */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK TCPCallback (unsigned int handle, int xType, 
	int errCode, void *callbackData)
{
	char	data;
	
	switch (xType)
		{
		case TCP_DISCONNECT:
			/* Server disconnected. Notify user. */
			MessagePopup ("MultiThreadingClient", "Server disconnected!");
			gTCPConnected = 0;
			break;
		case TCP_DATAREADY:
			/*
			 * We can just read a single data point for each TCP_DATAREADY
			 * event. But as we are not doing anything else in this thread
			 * we can also read as many points as available when the event
			 * occurs. In this case, we call ClientTCPRead iteratively
			 * until a timeout error occurs (as there is no more data
			 * available). Note that the timeout period is set to 50 ms
			 * so as not to wait too long in a read call. Also, we disable
			 * runtime error checking to prevent the CVI debugger from
			 * catching the timeout error.
			 */
			DisableBreakOnLibraryErrors ();
			while (1)
				{
				if (ClientTCPRead (gTCPConnectionHandle, &data, 
						sizeof (data), 50) < 0)
					break;
				PlotStripChartPoint (gPanel, PANEL_CHART, (char) data);
				}
			EnableBreakOnLibraryErrors ();
			break;
		}
		
	return 0;
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
