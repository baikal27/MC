#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>
#include "exam_cvi.h"
//#include "MotionTest.h"
//#include "initiation.h"
#include "AXL.h"
#include "AXHS.h"
#include "AXM.h"
#include "AXD.h"

#ifdef _DEBUG
#define new DEBBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = _FILE_;
#endif


void init(void);

static int panelHandle;

//**********************************************************************************
int main (int argc, char *argv[])
//**********************************************************************************

{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "exam_cvi.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}
//**********************************************************************************
int CVICALLBACK Panel_proc (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
//**********************************************************************************

{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}
//**********************************************************************************
int CVICALLBACK cmdOK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//**********************************************************************************

{
	switch (event)
	{
		case EVENT_COMMIT:
			
			//SetCtrlVal(panel, PANEL_strOutput,"HI");

			init();

			break;
	}
	return 0;
}

//**********************************************************************************
void init()
//**********************************************************************************

{
	DWORD dwStatus;
	
	
	if(AxlOpen()==AXT_RT_SUCCESS)			  //axl.h
	//if(AxlRingOpen()==AXT_RT_SUCCESS)
	{

		if(AxmInfoIsMotionModule(&dwStatus)==AXT_RT_SUCCESS) //axm.h
		{
			if(dwStatus==STATUS_EXIST)
			{
				MessagePopup("info","exist motion module");
			}
			
			else
			{
				MessagePopup("info","not exist motion module");
			}
		}
	
	
	
		if(AxdInfoIsDIOModule (&dwStatus)==AXT_RT_SUCCESS)		//axd.h
		{
			if(dwStatus==STATUS_EXIST)
			{
				MessagePopup("info","exist DIO module");
			}
			else
			{
				MessagePopup("info","not exist DIO module");
			}
		}
	}
	
	else
	{
		MessagePopup("info","not initialize library");
	}
	
	
	if(AxlIsOpened())  //AXL.h
	{
		AxlClose();
	}


}
