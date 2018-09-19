#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>
#include <ansi_c.h> 
#include <utility.h> 
#include "type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "cvi_util.h" 

#include "AXHS.h"
#include "AXM.h"
#include "AXD.h"

#include "tcs_ajin.h"
#include "initiation.h" 
#include "st4s_lib.h" 

#include "gv_tel.h" 
extern int start_manu_parking (void);
extern void display_message(char buff[]);
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void show_message(char message[]);
static int check_init(void);
static int start_init(void);
static display_status_x(char caxis);
int CVICALLBACK ThreadFunction1 (void *functionData);
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
static int TPANEL;
static int CHECKINITA,CHECKINITB;
static int CHECKPARKA,CHECKPARKB;
static double encoder,counts,velocity;
static char buff[400];
int iThreadExit;
static int TCSPANELOPEN=0;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int load_tcs()																   
//-----------------------------------------------------------------------------
{   
	TPANEL = LoadPanel(0,"tcs_ajin.uir",PANEL);
	DisplayPanel (TPANEL);
	
	sprintf(buff,"TCS MONITORING NO. %d",NODENO+1);
    SetPanelAttribute (TPANEL, ATTR_TITLE, buff);
    SetCtrlVal(TPANEL,PANEL_NODENO,NODENO);   
	
    SetCtrlVal(TPANEL,PANEL_TIMERONOFF,1);   
    SetCtrlAttribute(TPANEL,PANEL_TIMER,ATTR_ENABLED,1);
	
    TCSPANELOPEN=1;
	
	return 0;
}
//-------------------------------------------------------------------------------
int CVICALLBACK Panel_proc (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
//-------------------------------------------------------------------------------
{
	switch (event)
	{
//		case EVENT_GOT_FOCUS:
//			break;
//		case EVENT_LOST_FOCUS:
//			break;
		case EVENT_CLOSE:
			save_motfile("motion_para.out");
//			CloseDevice(); 
            TCSPANELOPEN=0;
			DiscardPanel (TPANEL);  
			break;
	}
	return 0;
}
//-------------------------------------------------------------------------------
int CVICALLBACK BASEBUTTON (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-------------------------------------------------------------------------------
{
 DWORD code,state;
 double steps, velocity;
 int dir;
 char caxis;
 
	switch (event)
	{
		case EVENT_COMMIT:

		    if(control==PANEL_cmdInit)
			{
			 init_tcs();
			 
 			 motor_stop('A');
			 motor_stop('B');
			 
             set_axis_para(0);
             set_axis_para(1);
			 
             set_home_search_para();
            }
			
			else if(PANEL_CONNECT== AXT_RT_SUCCESS) show_message("라이브러리 초기화 되었습니다.\n");            

			else if(control==PANEL_MOVESLEW)
			 {
              GetCtrlVal(TPANEL, PANEL_RING,&state);   
              GetCtrlVal(TPANEL, PANEL_STEPS,&steps);   
			  caxis='A'+state;
			  slew_stop(caxis,steps);
			 }
			else if(control==PANEL_MOVEACCE)
			 {
              GetCtrlVal(TPANEL, PANEL_RING,&state);   
              GetCtrlVal(TPANEL,PANEL_VELO,&velocity);  
			  caxis='A'+state;
     	      go_const(caxis,velocity);
			 }
			
			else if(control==PANEL_STOP)
			{
              GetCtrlVal(TPANEL, PANEL_RING,&state);   
			  caxis='A'+state;
			  motor_stop(caxis);
			}
			
//			else if(control==PANEL_cmdExit) 
//			  {
//             save_motfile("motion_para.out");
//			   CloseDevice();   
// 		       DiscardPanel(TPANEL);
//            }

			else if(control==PANEL_TIMERONOFF)
			 {
              GetCtrlVal(TPANEL, control,&state);   
              SetCtrlAttribute (TPANEL,PANEL_TIMER, ATTR_ENABLED, state);
             }
             
			else if(control==PANEL_COUNTA)
			 {
              GetCtrlVal(TPANEL, control,&counts); 
              set_counter('A',counts);
             }
			
			else if(control==PANEL_COUNTB)
			 {
              GetCtrlVal(TPANEL, control,&counts); 
              set_counter('B',counts);
             }
              
			else if(control==PANEL_ENCODERA)
			 {
              GetCtrlVal(TPANEL, control,&encoder); 
              set_encoder('A',encoder);
             }
			
			else if(control==PANEL_ENCODERB)
			 {
              GetCtrlVal(TPANEL, control,&encoder); 
              set_encoder('B',encoder);
             }
			
			else if(control==PANEL_CLEARC)
			 {
              SetCtrlVal(TPANEL,PANEL_COUNTA  ,0); set_counter('A',0);
              SetCtrlVal(TPANEL,PANEL_COUNTB  ,0); set_counter('B',0);
              SetCtrlVal(TPANEL,PANEL_ENCODERA,0); set_encoder('A',0);
              SetCtrlVal(TPANEL,PANEL_ENCODERB,0); set_encoder('B',0);
             }

			else if(control==PANEL_PARK) start_manu_parking(); 
			
			else if(control==PANEL_INIT) start_init();
			 
			else if(control==PANEL_U8)
			 {
              GetCtrlVal(TPANEL, PANEL_U8,&state);   
              code=write_cpuoutput_bit(8,state);
			 }
			 
			else if(control==PANEL_U9)
			 {
              GetCtrlVal(TPANEL, PANEL_U9,&state);   
              code=write_cpuoutput_bit(9,state);
			 }
			
			else if(control==PANEL_NODENO) GetCtrlVal(TPANEL, control,&NODENO);   
			
	        break;
	 }
	 return 0;
}

//------------------------------------------------------------------------
void CVICALLBACK menuBar (int menuBar, int menuItem, void *callbackData,int panel)
//------------------------------------------------------------------------
{

//      if     (menuItem==Main_menuTheSky_menuConnect)      ActivateServer(); 
//      else if(menuItem==Main_menuTheSky_menuDisConnect)   DeactivateServer();
}

//-------------------------------------------------------------------------------
int CVICALLBACK ThreadFunction1 (void *functionData)
//-------------------------------------------------------------------------------
{
    int i = 0;
    
    while (iThreadExit)
    {   
		SetCtrlVal (TPANEL, PANEL_NT1, i++);   
		Delay (0.1); 
    }
    
    return 0;
}

//-------------------------------------------------------------------------------
int CVICALLBACK ThreadFunction2 (void *functionData)
//-------------------------------------------------------------------------------
{   
    int i = 0;
    
//    while (iThreadExit)
//    {
//		SetCtrlVal (TPANEL, PANEL_numThreadfFunction2, i++);   
//		Delay (0.1);
//    }
    
    return 0;
}
//-------------------------------------------------------------------------------
void show_message(char message[])
//-------------------------------------------------------------------------------
{
	if(TCSPANELOPEN==1) SetCtrlVal(TPANEL, PANEL_TEXTBOX,message);  
	else				display_message(message);
}	
//-------------------------------------------------------------------------------
int CVICALLBACK TIMERCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-------------------------------------------------------------------------------
{
 int i;
 DWORD mode;
 int imode,dir;
 int cwlimit,ccwlimit,homea,homeb;
 static DWORD mode_out=0;
 long counts,encoder;
 char caxis;
 DWORD upHomeResult;
 
	switch (event)
	{
     case EVENT_TIMER_TICK:

		  read_slew_mode('A',&mode);
		  SetCtrlVal(TPANEL, PANEL_MODEA,mode);

  		  read_dir('A',&dir);
		  SetCtrlVal(TPANEL, PANEL_DIRA,dir);
		  
		  read_counter('A',&counts);
		  read_encoder('A',&encoder);
		  read_speed('A',&velocity);
		  if(dir==0) velocity = -velocity;
		  
		  SetCtrlVal(TPANEL, PANEL_COUNTA,counts);
		  SetCtrlVal(TPANEL, PANEL_ENCODERA,encoder);
		  SetCtrlVal(TPANEL, PANEL_VELOCITYA,velocity);
		  
 		  read_cw_limit  ('A',&cwlimit);
 		  read_ccw_limit ('A',&ccwlimit);
 		  read_home_limit('A',&homea);
		  SetCtrlVal(TPANEL, PANEL_CWA,cwlimit);
		  SetCtrlVal(TPANEL, PANEL_CCWA,ccwlimit);
		  SetCtrlVal(TPANEL, PANEL_HOMEA,homea);

		  read_slew_mode('B',&mode);
		  SetCtrlVal(TPANEL, PANEL_MODEB,mode);

  		  read_dir('B',&dir);
		  SetCtrlVal(TPANEL, PANEL_DIRB,dir);
		  
		  read_counter('B',&counts);
		  read_encoder('B',&encoder);
		  read_speed('B',&velocity);
		  if(dir==0) velocity = -velocity;
		  SetCtrlVal(TPANEL, PANEL_COUNTB,counts);
		  SetCtrlVal(TPANEL, PANEL_ENCODERB,encoder);
		  SetCtrlVal(TPANEL, PANEL_VELOCITYB,velocity);
		  
 		  read_cw_limit  ('B',&cwlimit);
 		  read_ccw_limit ('B',&ccwlimit);
 		  read_home_limit('B',&homeb);
		  SetCtrlVal(TPANEL, PANEL_CWB,cwlimit);
		  SetCtrlVal(TPANEL, PANEL_CCWB,ccwlimit);
		  SetCtrlVal(TPANEL, PANEL_HOMEB,homeb);

		  Delay(0.01);
		  read_cpuinput_bit(0,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI0,mode);
		  read_cpuinput_bit(1,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI1,mode);
		  read_cpuinput_bit(2,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI2,mode);
		  read_cpuinput_bit(3,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI3,mode);
		  read_cpuinput_bit(4,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI4,mode);
		  read_cpuinput_bit(5,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI5,mode);
		  read_cpuinput_bit(6,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI6,mode);
		  read_cpuinput_bit(7,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI7,mode);
		  read_cpuinput_bit(8,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI8,mode);
		  read_cpuinput_bit(9,&mode);
		  SetCtrlVal(TPANEL, PANEL_UI9,mode);
		  
//		  AxmHomeGetResult(0, &upHomeResult);
//		  sprintf(buff,"home %d\n",upHomeResult);
//		  show_message(buff);
		  
//          AxdoReadOutportBit(0,8,&mode);  
//          SetCtrlVal(TPANEL,PANEL_U8,mode); 
//          AxdoReadOutportBit(0,9,&mode);  
//          SetCtrlVal(TPANEL,PANEL_U9,mode); 
		  
//		  SetCtrlVal(TPANEL, PANEL_UI0,0);
		  
		  GetCtrlVal(TPANEL, PANEL_RING,&i);
		  caxis = i+'A';
		  display_status_x(caxis);
		  
		  if(CHECKINITA==1 || CHECKINITB==1 ) check_init();
			  
		break;
	}
	return 0;
}
//-------------------------------------------------------------------------------
static int display_status_x(char caxis)
//-------------------------------------------------------------------------------
{
 int i;
 DWORD mode;
 int imode,dir;
 int cwlimit,ccwlimit,homea,homeb;
 static DWORD mode_out=0;
 long counts,encoder;
// DWORD upHomeResult;
 
		  read_slew_mode(caxis,&mode);
		  SetCtrlVal(TPANEL, PANEL_MODEX,mode);

  		  read_dir(caxis,&dir);
		  SetCtrlVal(TPANEL, PANEL_DIRX,dir);
		  
		  read_counter(caxis,&counts);
		  read_encoder(caxis,&encoder);
		  read_speed  (caxis,&velocity);
		  if(dir==0) velocity = -velocity;
		  
		  SetCtrlVal(TPANEL, PANEL_COUNTX,counts);
		  SetCtrlVal(TPANEL, PANEL_ENCODERX,encoder);
		  SetCtrlVal(TPANEL, PANEL_VELOCITYX,velocity);
		  
 		  read_cw_limit  (caxis,&cwlimit);
 		  read_ccw_limit (caxis,&ccwlimit);
 		  read_home_limit(caxis,&homea);
		  SetCtrlVal(TPANEL, PANEL_CWX,cwlimit);
		  SetCtrlVal(TPANEL, PANEL_CCWX,ccwlimit);
		  SetCtrlVal(TPANEL, PANEL_HOMEX,homea);
return 0;		  
}
//-------------------------------------------------------------------------------
int CVICALLBACK TEXTAPPLY (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-------------------------------------------------------------------------------
{
	switch (event)
	{
		case EVENT_COMMIT:
			break;
		
		case EVENT_LEFT_CLICK:
			break;
			
		case EVENT_RIGHT_CLICK:
		     ResetTextBox(panel,control,"");

			break;
	}
	return 0;
}
//-------------------------------------------------------------------------------
static int check_init()
//-------------------------------------------------------------------------------
{
 int homea,homeb;
 long count;
    if(CHECKINITA==1)
	  {read_home_limit('A',&homea);
	   if(homea==0)
	     {motor_stop('A'); read_counter('A',&count); set_counter('A',0); 
	      sprintf(buff,"count_A = %ld\n",count);  show_message(buff);CHECKINITA=0;}
	  }
	

    if(CHECKINITB==1)
	  {read_home_limit('B',&homea);
	   if(homea==0)
	     {motor_stop('B'); read_counter('B',&count); set_counter('B',0); 
	      sprintf(buff,"count_B = %ld\n",count);  show_message(buff);CHECKINITB=0;}
	  }
	
	
	if(CHECKINITA==0 && CHECKINITB==0) {set_timer_interval(TPANEL,PANEL_TIMER,1.0);}	
	
	return 0;
}
//-------------------------------------------------------------------------------
static int start_init()
//-------------------------------------------------------------------------------
{
 int homea,homeb;
 
     if(CHECKINITA==1 || CHECKINITB==1)
       {motor_stop('A');motor_stop('B');
        set_timer_interval(TPANEL,PANEL_TIMER,1.0);
	    CHECKINITA=CHECKINITB=0;
	   }
 
    read_home_limit('A',&homea);
    read_home_limit('B',&homeb);
	if(homea==0 || homeb==0)
	  {MessagePopup("Init warning","Parking first");
	   CHECKINITA=CHECKINITB=0; 
	  return -1;}
	
    CHECKINITA=CHECKINITB=1;
	go_const('A',INIT_FREQ[0]);
	go_const('B',INIT_FREQ[1]);
    set_timer_interval(TPANEL,PANEL_TIMER,0.1);
 return 0;
} 
//-------------------------------------------------------------------------------
static int start_park()
//-------------------------------------------------------------------------------
{
 int homea,homeb;
 
     if(CHECKPARKA==1 || CHECKPARKB==1)
       {motor_stop('A');motor_stop('B');
        set_timer_interval(TPANEL,PANEL_TIMER,1.0);
	    CHECKPARKA=CHECKPARKB=0;
	   }
 
    read_home_limit('A',&homea);
	//if(homea==0
    read_home_limit('B',&homeb);
	if(homea==1 || homeb==0)
	  {MessagePopup("Init warning","Parking first");
	   CHECKINITA=CHECKINITB=0; 
	  return -1;}
	
    CHECKINITA=CHECKINITB=1;
	go_const('A',SLEW_FREQ[0]);
	go_const('B',SLEW_FREQ[1]);
    set_timer_interval(TPANEL,PANEL_TIMER,0.1);
 return 0;
} 
//-------------------------------------------------------------------------------
 int initiation_tcs()
//-----------------------------------------------------------------------------
{ 
	init_tcs();
			 
 	motor_stop('A');
	motor_stop('B');
			 
    set_axis_para(0);
    set_axis_para(1);
			 
    set_home_search_para();
	
	return 0;
}	
//-----------------------------------------------------------------------------
