//------------------------------------------------------------------------
#include <formatio.h>
#include <utility.h>
#include <userint.h>
#include <ansi_c.h>
#include <math.h>
#include "type_def.h"
#include "cvi_util.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "gv_tel.h"
//#include "cpabtsub.h"

//#include "hw.h"
#include "dome_sub.h"
#include "dome_control.h"
extern void display_message(char buff[]);

extern d_m_s GAZI;
static const double  PI314           = 3.14159265358979323846;
static const double  TWOPI           = 3.14159265358979323846*2.0;

extern int DOME_TRACKING;
enum {DOME_TRACKING_ON,DOME_TRACKING_OFF};

//------------------------------------------------------------------------
int           DOME_HOME_STATUS;  // it is used in TCS.C
static int    DOME_HOME_SWITCH;
static int    CHECK_MODE;

int DOME_DIR,UP_SLIT_DIR,DOWN_SLIT_DIR,LAMP_STATUS;  // variables used in ST4s_dome_sub.c

static long   DOME_ENCODER_VALUE;
static double DOME_CYCLE, DOME_ZERO,DOME_ANGLE,MAX_DOME_ERROR;
static double DOME_PARA[6], COUNT360;
//static double BRAKE_ANGLE;
//------------------------------------------------------------------------
static int panel_handle,parking_handle;
static char uir_file[30] = "dome_control.uir";
//------------------------------------------------------------------------
int link_to_dome(void);
int load_dome_panel(void);
double track_dome(double azi);

static int display_dome_status(void);
static int set_dome_led(int DOME_DIR);
static int check_dome_parking(void);

static int    dome_move(double azi);
static int    read_dome_parameter(void);
static double get_dome_angle(long count);
static long   calc_dome_count(double theta);
static double calc_dome_error(double angle);
static void   adjust_dome_offset(void);
//------------------------------------------------------------------------
static double AZI;
static int PARK_DIR;
static int DOME_PANEL = 0;

static char buff[300];
//------------------------------------------------------------------------
/*
//------------------------------------------------------------------------
//   These variables are not really necessary.
//   Only needed for linkging ser_sub1.obj file 
//   double F_OSCIL, F_1;  
//   ramp_data RAMP[Number_Of_Axis];
//------------------------------------------------------------------------
int main (int argc, char *argv[])
//------------------------------------------------------------------------
{
 int e_code;
 char out_buffer[20];
 FILE *FILE_IN;
 
//  Needed if linking in external compiler; harmless otherwise 
    if (InitCVIRTE (0, argv, 0) == 0)  
        return -1;                     // out of memory

    link_to_dome();
    load_dome_panel();
    RunUserInterface ();

    return 0;
}
//------------------------------------------------------------------------
*/
//------------------------------------------------------------------------
int quit_program()
//------------------------------------------------------------------------
{	
 int status;
 
    turn_off_timer(panel_handle,MAIN_TIMER);
    DiscardPanel(panel_handle);
    DOME_PANEL = 0;

/*
//  close serial port       
    QuitUserInterface (0);
    status = close_dome_com_port();
    if(status != 0) MessagePopup("","Error   in CLOSE_COM\n\a");
*/
   return 0;
}    
//------------------------------------------------------------------------
int link_to_dome()
//------------------------------------------------------------------------
{
 int e_code;
 char out_buffer[20];
 FILE *FILE_IN;

//  init_dome_tcs();            
    read_dome_parameter();
    CHECK_MODE = 0;        
    DOME_DIR   = 0;

    return 0;
}
//------------------------------------------------------------------------
int load_dome_panel()
//------------------------------------------------------------------------
{
    read_dome_parameter();
	
    if(DOME_PANEL != 0) return -1;
    panel_handle = LoadPanel(0,uir_file,MAIN);
    DisplayPanel(panel_handle);
    SetCtrlVal  (panel_handle,MAIN_LOOP,1);
    
    DOME_PANEL = 1;
    
    return 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int CVICALLBACK DOME_TIMER_CALLBACK(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
        switch(event)
              {
               case EVENT_TIMER_TICK:
                    display_dome_status();
                    break;
              }
        return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK PARKING_CALLBACK(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
 long counts;
 
	switch (event)
		{
		case EVENT_COMMIT:
		     if     (control==PARKING_BEGIN)
		      {GetCtrlVal(panel,PARKING_RING,&PARK_DIR);
		       if(PARK_DIR==1) dome_cw  (); else dome_ccw();
               turn_on_timer(panel_handle,MAIN_PARKING_TIMER);
		       CHECK_MODE = -1;
               DiscardPanel(parking_handle);
		      }
		      
		     else if(control==PARKING_STOP) {dome_stop(); CHECK_MODE =  0;}

		     else if(control==PARKING_QUIT)  
               {
			    turn_off_timer (panel_handle,MAIN_PARKING_TIMER);
			    CHECK_MODE = 0;
                dome_stop();
                DiscardPanel(parking_handle);
                close_stdio();
               }
               
			break;
		}
	return 0;
}
//------------------------------------------------------------------------


//------------------------------------------------------------------------
static int display_dome_status(void)
//------------------------------------------------------------------------
{
 char caxis;
 int  encoder;
 
//  display encoder value
    read_dome_encoder(&DOME_ENCODER_VALUE);
    encoder = DOME_ENCODER_VALUE;
    SetCtrlVal(panel_handle,MAIN_ENCODER      ,encoder     );    
	
    DOME_ANGLE = rad_to_deg(get_dome_angle(DOME_ENCODER_VALUE));
    SetCtrlVal(panel_handle,MAIN_DOME_POSITION,DOME_ANGLE  );    
    SetCtrlVal(panel_handle,MAIN_AZI          ,DOME_ANGLE  );    
/*    
//  display home switch status
    read_dome_home(&DOME_HOME_SWITCH);
    if(DOME_HOME_SWITCH==DOME_HOME_STATUS) SetCtrlVal(panel_handle,MAIN_LED_HOME, 1);
    else                                   SetCtrlVal(panel_handle,MAIN_LED_HOME, 0);
    
//  display dome led
    set_dome_led(DOME_DIR);
    
//  display upper slit led
    if     (UP_SLIT_DIR== 1){SetCtrlVal(panel_handle,MAIN_LED_US_OPEN , 1);}
    else if(UP_SLIT_DIR==-1){SetCtrlVal(panel_handle,MAIN_LED_US_CLOSE, 1);}
    else                    {SetCtrlVal(panel_handle,MAIN_LED_US_OPEN , 0);
                             SetCtrlVal(panel_handle,MAIN_LED_US_CLOSE, 0);}

//  display lower slit led
    if     (DOWN_SLIT_DIR== 1){SetCtrlVal(panel_handle,MAIN_LED_DS_OPEN , 1);}
    else if(DOWN_SLIT_DIR==-1){SetCtrlVal(panel_handle,MAIN_LED_DS_CLOSE, 1);}
    else                      {SetCtrlVal(panel_handle,MAIN_LED_DS_OPEN , 0);
                               SetCtrlVal(panel_handle,MAIN_LED_DS_CLOSE, 0);}

//  display lamp led
    if     (LAMP_STATUS == 1)  SetCtrlVal(panel_handle,MAIN_LED_LAMP_ON, 1);
    else                       SetCtrlVal(panel_handle,MAIN_LED_LAMP_ON, 0);
*/
    return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK DOME_CBF(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
 double interval,error;
 int status;
 long value,count;
 double theta;
 
	switch (event)
		{
		case EVENT_COMMIT:

		     if     (control==MAIN_DOME_CW  ) {dome_cw  (); DOME_DIR = 1;}
		     else if(control==MAIN_DOME_CCW ) {dome_ccw (); DOME_DIR =-1;}
		     else if(control==MAIN_DOME_STOP) {dome_stop(); DOME_DIR = 0;
		     			                       turn_off_timer (panel_handle,MAIN_PARKING_TIMER);
		                                       CHECK_MODE=0;}
		     
		     else if(control==MAIN_US_OPEN )  {upper_slit_open (); UP_SLIT_DIR = 1;}
		     else if(control==MAIN_US_CLOSE)  {upper_slit_close(); UP_SLIT_DIR =-1;}
		     else if(control==MAIN_US_STOP )  {upper_slit_stop (); UP_SLIT_DIR = 0;}

		     else if(control==MAIN_DS_OPEN )  {lower_slit_open (); DOWN_SLIT_DIR = 1;}
		     else if(control==MAIN_DS_CLOSE)  {lower_slit_close(); DOWN_SLIT_DIR =-1;}
		     else if(control==MAIN_DS_STOP )  {lower_slit_stop (); DOWN_SLIT_DIR = 0;}

		     else if(control==MAIN_LAMP_ON )  {lamp_on (); LAMP_STATUS = 1;}
		     else if(control==MAIN_LAMP_OFF)  {lamp_off(); LAMP_STATUS = 0;}
		     
		     else if(control==MAIN_TIMERINTERVAL)
		       {
                GetCtrlVal(panel_handle,MAIN_TIMERINTERVAL,&interval);
//              ResetTimer(panel_handle,MAIN_TIMER);
                SetCtrlAttribute(panel_handle,MAIN_TIMER,ATTR_INTERVAL,interval);
               }
		     
		     else if(control==MAIN_ENCODER)
		       {GetCtrlVal(panel_handle,MAIN_ENCODER,&DOME_ENCODER_VALUE);
		        set_dome_encoder(DOME_ENCODER_VALUE);
		       }

		     else if(control==MAIN_PARKING)
			   {
			    read_dome_home(&status);
			    if(status==DOME_HOME_STATUS) break;
			    parking_handle = LoadPanel(0, uir_file,PARKING);
			    InstallPopup(parking_handle);
			    read_dome_encoder(&value);
			    error = DOME_ZERO - rad_to_deg(get_dome_angle(value));
			    if(error> 180.0) error -= 360.0;
			    if(error<-180.0) error += 360.0;
			    
			    if(error>0) SetCtrlVal(parking_handle,PARKING_RING, 1);
			    else        SetCtrlVal(parking_handle,PARKING_RING,-1);
			   }
			   
		     else if(control==MAIN_LOOP)
			   {
			    GetCtrlVal(panel_handle,control,&status);
			    if(status==1) turn_on_timer (panel_handle,MAIN_TIMER);
			    else          turn_off_timer(panel_handle,MAIN_TIMER);
			   }
			   
		     else if(control==MAIN_LOOPSYNC)
			   {
			    GetCtrlVal(panel_handle,control,&status);
			    if(status==1) turn_on_timer (panel_handle,MAIN_PARKING_TIMER);
			    else          turn_off_timer(panel_handle,MAIN_PARKING_TIMER);
			   }
			   
		     else if(control==MAIN_GO || control==MAIN_GOAL)
			   {GetCtrlVal(panel_handle,MAIN_GOAL,&AZI);
			    dome_move(AZI);
			   }
			   
//           adjust zero point		        
		     else if(control==MAIN_OFFSET) adjust_dome_offset();
		     
//		     else if(control==MAIN_THETAT)
//		       {
//		        GetCtrlVal(panel_handle,control,&theta); 
//		        theta = deg_to_rad(theta);
//		        count = calc_dome_count(theta);
//		        SetCtrlVal(panel_handle,MAIN_COUNTT,count); 
//               }
               
//		     else if(control==MAIN_COUNTT)
//		       {
//		        GetCtrlVal(panel_handle,control,&count); 
//		        theta = get_dome_angle(count);
//		        theta = rad_to_deg(theta);
//		        SetCtrlVal(panel_handle,MAIN_THETAT,theta); 
//               }
               
		     else if(control==MAIN_QUIT) quit_program();
		     
   		     else if(control==MAIN_DOME_ONOFF)
		       {
		        GetCtrlVal(panel,control,&status);
		        if(status==0) {DOME_TRACKING = DOME_TRACKING_OFF; dome_stop();}
                else           DOME_TRACKING = DOME_TRACKING_ON ;
               }
		       
			break;
		}
	return 0;
}
//------------------------------------------------------------------------
static int set_dome_led(DOME_DIR)
//------------------------------------------------------------------------
{
    if     (DOME_DIR== 1)
           {SetCtrlVal(panel_handle,MAIN_LED_CW  ,1);
            SetCtrlVal(panel_handle,MAIN_LED_CCW ,0);
            SetCtrlVal(panel_handle,MAIN_LED_STOP,0);
           }
    else if(DOME_DIR==-1)
           {SetCtrlVal(panel_handle,MAIN_LED_CW  ,0);
            SetCtrlVal(panel_handle,MAIN_LED_CCW ,1);
            SetCtrlVal(panel_handle,MAIN_LED_STOP,0);
           }
    else if(DOME_DIR== 0)
           {SetCtrlVal(panel_handle,MAIN_LED_CW  ,0);
            SetCtrlVal(panel_handle,MAIN_LED_CCW ,0);
            SetCtrlVal(panel_handle,MAIN_LED_STOP,1);
           }
    return 0;                         
}
//------------------------------------------------------------------------


//------------------------------------------------------------------------
int CVICALLBACK PARKING_TIMER_CALLBACK(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_TIMER_TICK:
		
//		     if(CHECK_MODE==1)
//		      {
		       track_dome(GAZI.rad);
//		       if(DOME_DIR==0) turn_off_timer(panel_handle,MAIN_PARKING_TIMER);
//            }
              
//		     else if(CHECK_MODE==-1) check_dome_parking();
		     
			break;
		}
	return 0;
}
//------------------------------------------------------------------------
static int check_dome_parking()
//------------------------------------------------------------------------
{
 long enc_value;
 char buff[30];
 long  DOME_ENCODER_VALUE_0;
 
    read_dome_home(&DOME_HOME_SWITCH);
    if(DOME_HOME_SWITCH==DOME_HOME_STATUS)
      {
       read_dome_encoder(&DOME_ENCODER_VALUE_0);
       dome_stop(); 
//     SetCtrlVal(parking_handle,PARKING_LED,1);

//     check whether the dome overun the home position
       Delay(1.0);
       read_dome_home(&DOME_HOME_SWITCH);
       if(DOME_HOME_SWITCH != DOME_HOME_STATUS)
         {
          if(PARK_DIR==1) {dome_ccw(); PARK_DIR = 0;}
          else            {dome_cw (); PARK_DIR = 1;}
          return -1;
         }
       
       turn_off_timer(panel_handle,MAIN_PARKING_TIMER);
       
       read_dome_encoder(&enc_value);
       sprintf(buff,"Home enoder %ld",DOME_ENCODER_VALUE_0);
       
       MessagePopup("Dome parking",buff);
       close_stdio();
       
       enc_value = enc_value - DOME_ENCODER_VALUE_0;
       set_dome_encoder(enc_value);
       DiscardPanel(parking_handle);
      }
      
    return 0;      
}      
//------------------------------------------------------------------------
double track_dome(double azirad)
//------------------------------------------------------------------------
{
 static double error, old_error;
 
    error = calc_dome_error(azirad);
    
    if(fabs(error) < MAX_DOME_ERROR)
      {dome_stop();
       DOME_DIR = 0;
//       turn_off_timer(panel_handle,MAIN_PARKING_TIMER);       
//       SetCtrlVal(panel_handle,MAIN_LOOPSYNC ,0);
      }
    else if(error>0.0) {dome_cw (); DOME_DIR = 1;}
    else               {dome_ccw(); DOME_DIR =-1;}
    
    if(DOME_PANEL==1) SetCtrlVal(panel_handle,MAIN_ERROR ,error);
      
    return error;
}
//------------------------------------------------------------------------
double track_dome_old(double azi)
//------------------------------------------------------------------------
{
 static double error, old_error;
 
    error = calc_dome_error(azi);
    
    if(DOME_DIR==0 && fabs(error) < MAX_DOME_ERROR) return error;
          
    if(DOME_DIR == 0)
      {
       if(error>0.0) {dome_cw (); DOME_DIR = 1;}
       else          {dome_ccw(); DOME_DIR =-1;}
      }
    else
      {
       if( (fabs(error) >= old_error) || (fabs(error) < MAX_DOME_ERROR/2.0) )
         {dome_stop(); DOME_DIR = 0;}
      }
      
    old_error = fabs(error);
      
    return error;
}
//------------------------------------------------------------------------
static int dome_move(double azi)
//------------------------------------------------------------------------
{
    AZI = azi;
    CHECK_MODE = 1;
    turn_on_timer(panel_handle,MAIN_PARKING_TIMER);
    return 0;
}    
//------------------------------------------------------------------------

//-----------------------------------------------------------------------------
static int  read_dome_parameter()
//------------------------------------------------------------------------
{
 FILE *fin;
// char ssss[200];
 int i;
 
    if( (fin = fopen("init_dome.inp","r"))==NULL)
      {MessagePopup("","cannot opne DOME_INIT.INP file");
       return -1;
      }
      
    for(i=0;i<6;i++)
       {fgets(buff,150,fin); sscanf(buff,"%lf",&DOME_PARA[i]);}
       
    fgets(buff,150,fin);
    sscanf(buff,"%lf", &MAX_DOME_ERROR);
    
    fgets(buff,150,fin);
    sscanf(buff,"%d", &DOME_HOME_STATUS);
    
//  fgets(buff,150,fin);
//  sscanf(buff,"%lf", &BRAKE_ANGLE);

    fclose(fin);

//  counts for one revolution

    DOME_PARA[1] = DOME_PARA[1]/deg_to_rad(1.0);


    COUNT360 = DOME_PARA[1]*TWOPI;
	    
    return 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static double calc_dome_error(double angle)
//------------------------------------------------------------------------
{
 long count,old_count;
 double old_angle,delta_angle;
 
    angle = rad_to_deg(angle);
    
    read_dome_encoder(&old_count);
    old_angle = rad_to_deg(get_dome_angle(old_count));
    delta_angle = fmod(angle-old_angle,360.0);
    if     (delta_angle > 180.0) delta_angle -= 360.0;
    else if(delta_angle <-180.0) delta_angle += 360.0;

    return delta_angle;
}
//------------------------------------------------------------------------
static double get_dome_angle(long count)
//-----------------------------------------------------------------------------
// count = para[0] + para[1]*theta + para[2]*cos(theta) + para[3]*sin(theta) + para[4]*cos(2theta) + para[5]*sin(2theta)
// dcount = (para[1] - para[2]*sin(theta) + para[3]*cos(theta) -2*para[4]*sin(2theta) + 2*para[5]*cos(2theta) ) * d_theta
//-----------------------------------------------------------------------------
{
 double dome_anglem,theta0;
 double dcount,dtheta,theta,count1;
 int cycle;
 double limit;
 
    limit = deg_to_rad(1.0);
 
    theta0 = (count- DOME_PARA[0])/DOME_PARA[1];
    count1 = DOME_PARA[1]*TWOPI;
    
    cycle = 0;
    for(;;)
       {
        dcount = count - calc_dome_count(theta0);
        dcount = fmod(dcount,count1);
        if(dcount<-count1/2.0) dcount+=count1;
        if(count1/2.0<dcount ) dcount-=count1;
        
	    dtheta = dcount/(DOME_PARA[1] - DOME_PARA[2]*sin(theta0) + DOME_PARA[3]*cos(theta0) -2*DOME_PARA[4]*sin(2.0*theta0) + 2*DOME_PARA[5]*cos(2.0*theta0));
	    theta  = theta0+dtheta;
	
	    if( fabs(dtheta) < limit ) break;
	    if( cycle > 10 )           break;
	
       theta0 = theta;
      }
	
      theta = fmod(theta,TWOPI);
      if(theta <-PI314) theta += TWOPI;
      if(theta > PI314) theta -= TWOPI;

//        if(theta <0     ) theta += TWOPI;
//        if(theta > TWOPI) theta -= TWOPI;
    
    return theta;
}
//-----------------------------------------------------------------------------
static double get_dome_angle_old(long count)
//-----------------------------------------------------------------------------
// count = para[0] + para[1]*theta + para[2]*cos(theta) + para[3]*sin(theta) + para[4]*cos(2theta) + para[5]*sin(2theta)
// dcount = (para[1] - para[2]*sin(theta) + para[3]*cos(theta) -2*para[4]*sin(2theta) + 2*para[5]*cos(2theta) ) * d_theta
//-----------------------------------------------------------------------------
{
 double dome_anglem,theta0;
 double dcount,dtheta,theta;
 int cycle;
 double limit;
 
    limit  = deg_to_rad(1.0);
 
    theta0 = (count- DOME_PARA[0])/DOME_PARA[1];
    
    cycle = 0;
    for(;;)
       {
        dcount = count - calc_dome_count(theta0);
	    dtheta = dcount/(DOME_PARA[1] - DOME_PARA[2]*sin(theta0) + DOME_PARA[3]*cos(theta0) -2*DOME_PARA[4]*sin(2.0*theta0) + 2*DOME_PARA[5]*cos(2.0*theta0));
	    theta  = theta0+dtheta;
	
	    if( fabs(dtheta) < limit ) break;
	    if( cycle > 10 )           break;
	
       theta0 = theta;
      }
	
      theta = fmod(theta,TWOPI);
      if(theta <-PI314) theta += TWOPI;
      if(theta > PI314) theta -= TWOPI;

//        if(theta <0     ) theta += TWOPI;
//        if(theta > TWOPI) theta -= TWOPI;
    
    return theta;
}
//-----------------------------------------------------------------------------
static long calc_dome_count(double theta)
//-----------------------------------------------------------------------------
//  theta in radian
//-----------------------------------------------------------------------------

{
 long count;
 
    count = DOME_PARA[0] + DOME_PARA[1]*theta + DOME_PARA[2]*cos(theta) + DOME_PARA[3]*sin(theta)
          + DOME_PARA[4]*cos(2*theta) + DOME_PARA[5]*sin(2*theta);
           
    return count;
}    
//-----------------------------------------------------------------------------
static void adjust_dome_offset(void)
//-----------------------------------------------------------------------------
{
 double jd_ut1,last,ap_ha,ap_dec,alt,azi;
 long encoder;
 
//  jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;

    get_aposi(TARGET_RA_2000.rad, TARGET_DEC_2000.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
              &last,&ap_ha,&ap_dec,&alt,&azi);
    
    encoder = calc_dome_count(azi);
    set_dome_encoder (encoder);
}
//------------------------------------------------------------------------
int test_dome_count()
//-----------------------------------------------------------------------------
// count = para[0] + para[1]*theta + para[2]*cos(theta) + para[3]*sin(theta) + para[4]*cos(2theta) + para[5]*sin(2theta)
// dcount = (para[1] - para[2]*sin(theta) + para[3]*cos(theta) -2*para[4]*sin(2theta) + 2*para[5]*cos(2theta) ) * d_theta
//-----------------------------------------------------------------------------
{
 double theta_deg,theta_rad,dtheta;
 long count;
 int i;
  
    read_dome_parameter();
    for(i=-12;i<=12;i++)
       {
        theta_deg = i*30;
        theta_rad = deg_to_rad(theta_deg);
        count=calc_dome_count(theta_rad);
        theta_rad = get_dome_angle(count);
        dtheta = theta_deg - rad_to_deg(theta_rad);
        sprintf(buff,"deg=%6.1lf delta=%6.1lf\n",theta_deg,dtheta);
        display_message(buff);
       }
  return 0;       
}        
//-----------------------------------------------------------------------------
