/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    MultiThreadingServer.c                                           */
/*                                                                           */
/* PURPOSE: This program creates a simple TCP server to be used with the     */
/*			MultiThreadingClient sample program.	 						 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <ansi_c.h>
#include <tcpsupp.h>
#include <cvirte.h>		
#include <userint.h>
#include "MultiThreadingServer.h"

/*---------------------------------------------------------------------------*/
/* Macros						                                             */
/*---------------------------------------------------------------------------*/
#define tcpChk(f) if ((gTCPError=(f)) < 0) {ReportTCPError(); goto Done;} else

/*---------------------------------------------------------------------------*/
/* Module-globals                                                            */
/*---------------------------------------------------------------------------*/
static int		gPanel = 0;
static int		gTCPConnected = 0;
static int		gTCPConnectionHandle;
static char		gData;
static int		gTCPError = 0;

/*---------------------------------------------------------------------------*/
/* Internal function prototypes                                              */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK TCPCallback (unsigned int handle, int xType, 
	int errCode, void *callbackData);
static void ReportTCPError (void);

/*---------------------------------------------------------------------------*/
/* This is the application's entry-point.                                    */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	int registered = 0;
	
	if (InitCVIRTE (0, argv, 0) == 0)
		goto Done;
	
	tcpChk (RegisterTCPServer (10000, TCPCallback, 0));
	registered = 1;
	
	if ((gPanel = LoadPanel (0, "MultiThreadingServer.uir", PANEL)) < 0)
		goto Done;
	DisplayPanel (gPanel);
	RunUserInterface ();
	
Done:
	if (gPanel > 0)
		DiscardPanel (gPanel);
	if (registered)
		UnregisterTCPServer (10000);
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
/* Send data to connected client on each timer tick event.    	 	 		 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK TimerCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_TIMER_TICK:
			if (gTCPConnected)
				{
				/* Client is connected. Generate and send data. */
				gData = (char)(rand() % 128);
				tcpChk (ServerTCPWrite (gTCPConnectionHandle, &gData, 
					sizeof (gData), 0));
				PlotStripChartPoint (gPanel, PANEL_CHART, (double)gData);
				}
			break;
		}
	
Done:	
	return 0;
}

/*---------------------------------------------------------------------------*/
/* TCP callback function.    	 	 										 */
/*---------------------------------------------------------------------------*/
static int CVICALLBACK TCPCallback (unsigned int handle, int xType, 
	int errCode, void *callbackData)
{
	switch (xType)
		{
		case TCP_CONNECT:
			if (gTCPConnected)
				{
				/* There is an existing client. Disconnect new client. */
				tcpChk (DisconnectFromTCPServer (handle));
				}
			else
				{
				/* Accept new connection, as there is no existing client. */
				gTCPConnectionHandle = handle;
				gTCPConnected = 1;
				SetCtrlAttribute (gPanel, PANEL_TIMER, ATTR_ENABLED, 1);
				}
			break;
		case TCP_DISCONNECT:
			if (gTCPConnected && handle == gTCPConnectionHandle)
				{
				/* Client disconnected. */
				SetCtrlAttribute (gPanel, PANEL_TIMER, ATTR_ENABLED, 0);
				gTCPConnected = 0;
				}
			break;
		case TCP_DATAREADY:
			/* Ignore any data sent to server. */
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
