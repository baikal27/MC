//-----------------------------------------------------------------------------
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include "guide_sub.h"

#include "type_def.h"
#include "st4s_lib.h" 
#include "tel_sub.h"
#include "cal_sub.h"
//#include "cpabtsub.h"
#include "c:\lib\cal_lib\cpabtsub.h"
#include "cvi_util.h"
#include "gv_tel.h"

static double GUIDE_TIME_ST4=0.1, GUIDE_TIME_HP=0.1, GUIDE_TIME_MOUSE=0.5;
static double GUIDE_SPEED_ST4 = 2.0;
static double OFFSET_HA_ORIGINAL,OFFSET_DEC_ORIGINAL;

extern int STOP_TELESCOPE(void);
extern void display_track_speed(double track_speed_azi,double track_speed_alt);
//-----------------------------------------------------------------------------
static h_m_s RA_2000; static d_m_s DEC_2000;
static char UIR_FILE_NAME[20] = "guide_sub.uir";
static int guide_handle;
//-----------------------------------------------------------------------------
int load_guide_panel(h_m_s ra_2000, d_m_s dec_2000);
int adjust_offset(h_m_s ra_2000, d_m_s dec_2000);
static int check_hand_paddle(void);
static int check_st4_guide(void);
static int display_offset(void);
int stop_guide(void);
static void set_guide_mode(void);
static int do_mouse_guide_old(int control);
static int do_mouse_guide_new(void);
void init_hp(hp_status *hp);

static int PANEL_EXIST = 0;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int load_guide_panel(h_m_s ra_2000, d_m_s dec_2000)
//-----------------------------------------------------------------------------
{
    if(PANEL_EXIST != 0) return -1;

	if ((guide_handle = LoadPanel(0,UIR_FILE_NAME, GUIDE)) < 0) return -1;
	
//  save RA and Dec of target
     RA_2000.rad =  ra_2000.rad; rad_to_hms( &RA_2000);
    DEC_2000.rad = dec_2000.rad; rad_to_dms(&DEC_2000);

//  save the original offset counts    
    OFFSET_HA_ORIGINAL = OFFSET_HA_COUNT;
    OFFSET_DEC_ORIGINAL= OFFSET_DEC_COUNT;
	
//  flag of panel exists	
    PANEL_EXIST = 1;
	
//  InstallPopup (guide_handle);
    DisplayPanel (guide_handle);

//  turn on guide timer
    turn_on_timer(guide_handle, GUIDE_TIMER);
    
    GUIDE_MODE = GUIDE_MODE_HP;
    set_guide_mode();
        
    SetCtrlVal(guide_handle,GUIDE_SPEED_RA ,GUIDE_SPEED[0]);
    SetCtrlVal(guide_handle,GUIDE_SPEED_DEC,GUIDE_SPEED[1]);
        
    SetCtrlAttribute(guide_handle,GUIDE_E,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute(guide_handle,GUIDE_E,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    SetCtrlAttribute(guide_handle,GUIDE_W,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute(guide_handle,GUIDE_W,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    SetCtrlAttribute(guide_handle,GUIDE_N,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute(guide_handle,GUIDE_N,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    SetCtrlAttribute(guide_handle,GUIDE_S,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute(guide_handle,GUIDE_S,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    
    SetCtrlVal(guide_handle,GUIDE_SPEED_RA , GUIDE_SPEED[0]);
    SetCtrlVal(guide_handle,GUIDE_SPEED_DEC, GUIDE_SPEED[1]);

    if(TELESCOPE_TYPE==_ALT_AZI)
      {motor_stop('A'); motor_stop('B');
       MEAN_TRACKING_FREQ[0] = 0.0;
       MEAN_TRACKING_FREQ[1] = 0.0;
      }
      
//  initialize track on/off switch                    
    SetCtrlAttribute(guide_handle,GUIDE_TRACK_ON_OFF,ATTR_ON_COLOR ,VAL_RED);
    SetCtrlAttribute(guide_handle,GUIDE_TRACK_ON_OFF,ATTR_OFF_COLOR,VAL_PANEL_GRAY);
    if(TELESCOPE_MODE == TEL_MODE_TRACK) SetCtrlVal(guide_handle, GUIDE_TRACK_ON_OFF, 1);
    else                                 SetCtrlVal(guide_handle, GUIDE_TRACK_ON_OFF, 0);
    
//    if(GUIDE_MODE==GUIDE_MODE_HP) SetCtrlVal(guide_handle,GUIDE_METHOD,1);
//    else                          SetCtrlVal(guide_handle,GUIDE_METHOD,0);
    
    return guide_handle;
}
//-----------------------------------------------------------------------------
int stop_guide()
//-----------------------------------------------------------------------------
{
    if(PANEL_EXIST==0) return -1;
//  copy_panel_to_clipboard(guide_handle);
    turn_off_timer(guide_handle,GUIDE_TIMER);

//  restore the offset count    
//    printf("d HA %10.1lf d Dec %10.1lf\n",
//    OFFSET_HA_COUNT-OFFSET_HA_ORIGINAL,OFFSET_DEC_COUNT-OFFSET_DEC_ORIGINAL);

    OFFSET_HA_COUNT = OFFSET_HA_ORIGINAL;
    OFFSET_DEC_COUNT= OFFSET_DEC_ORIGINAL;
    
    GUIDE_MODE=GUIDE_MODE_OFF;
    
    DiscardPanel(guide_handle);
    PANEL_EXIST = 0;
    
    return 0;
}
//-----------------------------------------------------------------------------
static void set_guide_mode()
//-----------------------------------------------------------------------------
{
    if     (GUIDE_MODE==GUIDE_MODE_HP)    // guide by hand paddle
	  {
       set_timer_interval(guide_handle, GUIDE_TIMER,GUIDE_TIME_HP);
       SetCtrlVal(guide_handle,GUIDE_RING,0);       
      }

    else if(GUIDE_MODE==GUIDE_MODE_MOUSE) // guide by mouse
      {
       set_timer_interval(guide_handle, GUIDE_TIMER,GUIDE_TIME_MOUSE);
       SetCtrlVal(guide_handle,GUIDE_RING,1);       
      }
      
      
    else if(GUIDE_MODE==GUIDE_MODE_ST4)   // guide by ST4
	  {
       set_timer_interval(guide_handle, GUIDE_TIMER,GUIDE_TIME_ST4);
       SetCtrlVal(guide_handle,GUIDE_RING,2);       
      }
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
int CVICALLBACK CB_GUIDE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-----------------------------------------------------------------------------
{
 int status;
 hp_status hp;
 
	switch (event) {
		case EVENT_COMMIT:
		
		    do_mouse_guide_old(control);
		    
			break;
	}
	return 0;
}
//-----------------------------------------------------------------------------
int CVICALLBACK GUIDE_CBF (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-----------------------------------------------------------------------------
{
 int status;
 
	switch (event)
		{
		case EVENT_COMMIT:
             if     (control==GUIDE_QUIT  ) stop_guide();
             else if(control==GUIDE_STOP  ) STOP_TELESCOPE();
             else if(control==GUIDE_OFFSET) adjust_offset(RA_2000, DEC_2000);
             else if(control==GUIDE_RING  )
               {GetCtrlVal(panel,control,&status);
                if     (status==0) {GUIDE_MODE = GUIDE_MODE_HP   ;}
                else if(status==1) {GUIDE_MODE = GUIDE_MODE_MOUSE;}
                else if(status==2) {GUIDE_MODE = GUIDE_MODE_ST4  ;}
               }
               
             else if(control==GUIDE_TRACK_ON_OFF)
               {GetCtrlVal(panel,control,&status);
                if(status==0) {TELESCOPE_MODE = TEL_MODE_STOP, STOP_TELESCOPE();}
                else          {TELESCOPE_MODE = TEL_MODE_TRACK;}
               }
/*               
             else if(control==GUIDE_METHOD)
              {
               GetCtrlVal(panel,control,&status);
               if(status==1) GUIDE_MODE = GUIDE_MODE_HP;
               else          GUIDE_MODE = GUIDE_MODE_MOUSE;
              }
*/              
               
			break;
		}
	return 0;
}
//-----------------------------------------------------------------------------
int CVICALLBACK SET_SPEED (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-----------------------------------------------------------------------------
{
 double speed;
 
	switch (event) {
		case EVENT_COMMIT:
		
			  GetCtrlVal(panel,control,&speed);
			  
			  if(control==GUIDE_SPEED_RA)			 
			    {GUIDE_SPEED[0] = speed;
			     GUIDE_FREQ [0] = speed*60.0*MOUNT_HA[1];
			     if(ENCODER_MODE==1) GUIDE_FREQ[0] *= ENC_TO_STEP[0];
			    }

			  if(control==GUIDE_SPEED_DEC)
			    {GUIDE_SPEED[1] = speed;
		         GUIDE_FREQ [1] = speed*60.0*MOUNT_DEC[1];
			     if(ENCODER_MODE==1) GUIDE_FREQ[1] *= ENC_TO_STEP[1];
			    }
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
int CVICALLBACK GUIDE_TIMER_CBF (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-----------------------------------------------------------------------------
{
 int e_code, status;
 double track_speed_azi,track_speed_alt;
 
	switch (event)
		{
		case EVENT_TIMER_TICK:
		
		     if     (GUIDE_MODE ==GUIDE_MODE_HP   ) check_hand_paddle();
		     else if(GUIDE_MODE ==GUIDE_MODE_MOUSE) do_mouse_guide_new();
		     else if(GUIDE_MODE ==GUIDE_MODE_ST4  ) check_st4_guide();
		     
		     if(TELESCOPE_MODE==TEL_MODE_TRACK) display_offset();
                
//           check tracking              
             if(TELESCOPE_MODE==TEL_MODE_TRACK && TELESCOPE_TYPE==_RA_DEC)
               {
                read_gate('A',&status);
//              if(status==0) go_const('A',MEAN_TRACKING_FREQ[0]);
               }
            
			break;
		}
	return 0;
}
//-----------------------------------------------------------------------------
static int check_hand_paddle()
//-----------------------------------------------------------------------------
{
 hp_status NEW;
 char str[15];
 int panel;
 
    do_guide_hp();
    read_hp_status(&NEW);

    panel = guide_handle;
    if     (NEW.ew==-1) {SetCtrlVal(panel,GUIDE_E,1); SetCtrlVal(panel,GUIDE_W,0);}
    else if(NEW.ew== 1) {SetCtrlVal(panel,GUIDE_E,0); SetCtrlVal(panel,GUIDE_W,1);}
    else if(NEW.ew== 0) {SetCtrlVal(panel,GUIDE_E,0); SetCtrlVal(panel,GUIDE_W,0);}

    if     (NEW.ns==-1) {SetCtrlVal(panel,GUIDE_S,1); SetCtrlVal(panel,GUIDE_N,0);}
    else if(NEW.ns== 1) {SetCtrlVal(panel,GUIDE_S,0); SetCtrlVal(panel,GUIDE_N,1);}
    else if(NEW.ns== 0) {SetCtrlVal(panel,GUIDE_S,0); SetCtrlVal(panel,GUIDE_N,0);}

//  motor selct switch
    if     (NEW.motor==1) {SetCtrlVal(panel,GUIDE_RING_MOTOR,0);}   // RA/DEC
    else if(NEW.motor==3) {SetCtrlVal(panel,GUIDE_RING_MOTOR,1);}   // FOCUS
                          
//  speed switch
    if     (NEW.speed==1){SetCtrlVal(panel,GUIDE_RING_SPEED  ,1);}  // GUIDE
    else if(NEW.speed==2){SetCtrlVal(panel,GUIDE_RING_SPEED  ,2);}  // SET
    else if(NEW.speed==3){SetCtrlVal(panel,GUIDE_RING_SPEED  ,3);}  // SLEW

    return 0;
}
//-----------------------------------------------------------------------------
static int display_offset()
//-----------------------------------------------------------------------------
{
 long count_a,count_b;
 h_m_s ap_ha,last;
 d_m_s ap_dec,alt,azi;
 double oFFSET_HA_COUNT,oFFSET_DEC_COUNT,
        oFFSET_HA_ARCMINUTE,oFFSET_DEC_ARCMINUTE; 
 double jd_ut1;        
        
    if(ENCODER_MODE==1)
	  {read_encoder('A',&count_a); read_encoder('B',&count_b);}
	else
	  {read_counter('A',&count_a); read_counter('B',&count_b);}
		       
//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    get_aposi(RA_2000.rad, DEC_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    if(TELESCOPE_TYPE==_RA_DEC)
      {
       oFFSET_HA_COUNT  = count_a - calc_ha_count (ap_ha.rad,ap_dec.rad);
       oFFSET_DEC_COUNT = count_b - calc_dec_count(ap_ha.rad,ap_dec.rad);
      }
    else
      {
       oFFSET_HA_COUNT  = count_a - calc_ha_count (azi.rad,alt.rad);
       oFFSET_DEC_COUNT = count_b - calc_dec_count(azi.rad,alt.rad);
      }
             
	oFFSET_HA_ARCMINUTE  = oFFSET_HA_COUNT /MOUNT_HA [1]/60.0;
	oFFSET_DEC_ARCMINUTE = oFFSET_DEC_COUNT/MOUNT_DEC[1]/60.0;
			 
	PlotPoint(guide_handle, GUIDE_GRAPH,oFFSET_HA_ARCMINUTE,oFFSET_DEC_ARCMINUTE,
              VAL_SMALL_EMPTY_SQUARE,VAL_RED);
                       
//  printf("offset (arcminute) R.A. %10.2lf Dec. %10.2lf\n",oFFSET_HA_COUNT,oFFSET_DEC_COUNT);
    
	return 0;
}
//-----------------------------------------------------------------------------
static int check_st4_guide()
//-----------------------------------------------------------------------------
{
 hp_status NEW;
 int panel;
 double delta_ha_count, delta_dec_count;
 
    delta_ha_count  = GUIDE_SPEED_ST4*GUIDE_TIME_ST4 * MOUNT_HA [1] / cos(TARGET_RA_2000.rad);
    delta_dec_count = GUIDE_SPEED_ST4*GUIDE_TIME_ST4 * MOUNT_DEC[1];
    
    panel = guide_handle;
 
    read_hp_status(&NEW);

    if     (NEW.ew==-1) OFFSET_HA_COUNT -= delta_ha_count;
    else if(NEW.ew== 1) OFFSET_HA_COUNT += delta_ha_count;

    if     (NEW.ns==-1) OFFSET_DEC_COUNT -= delta_dec_count;
    else if(NEW.ns== 1) OFFSET_DEC_COUNT += delta_dec_count;
    
    if(PANEL_EXIST==0) return 0;

//  display button status
    if     (NEW.ew==-1) {SetCtrlVal(panel,GUIDE_E,1); SetCtrlVal(panel,GUIDE_W,0);}
    else if(NEW.ew== 1) {SetCtrlVal(panel,GUIDE_E,0); SetCtrlVal(panel,GUIDE_W,1);}
    else if(NEW.ew== 0) {SetCtrlVal(panel,GUIDE_E,0); SetCtrlVal(panel,GUIDE_W,0);}

    if     (NEW.ns==-1) {SetCtrlVal(panel,GUIDE_S,1); SetCtrlVal(panel,GUIDE_N,0);}
    else if(NEW.ns== 1) {SetCtrlVal(panel,GUIDE_S,0); SetCtrlVal(panel,GUIDE_N,1);}
    else if(NEW.ns== 0) {SetCtrlVal(panel,GUIDE_S,0); SetCtrlVal(panel,GUIDE_N,0);}

    return 0;
}
//-----------------------------------------------------------------------------
static int do_mouse_guide_old(int control)
//-----------------------------------------------------------------------------
{
 int status, panel;
 hp_status hp;
 
 
    panel = guide_handle;
    
    init_hp(&hp);
    
	if(GUIDE_MODE==GUIDE_MODE_HP || GUIDE_MODE==GUIDE_MODE_ST4) return 0;
		    
    GetCtrlVal(panel,GUIDE_RING_SPEED,&status);
	if     (status==1) hp.speed = 1;  // guide
    else if(status==2) hp.speed = 2;  // set
    else if(status==3) hp.speed = 3;  // slew
            
    GetCtrlVal(panel,GUIDE_RING_MOTOR,&status);
    if     (status==0) {hp.motor = 1;}   // RA/DEC
    else if(status==1) {hp.motor = 3;}   // FOCUS
		    
	if(control==GUIDE_RING_SPEED)
      {
       GetCtrlVal(panel,control,&status);
		       
       if     (status==1)  // GUIDE
        {
         hp.speed = 1;
         SetCtrlVal(guide_handle,GUIDE_SPEED_RA ,GUIDE_SPEED[0]);
         SetCtrlVal(guide_handle,GUIDE_SPEED_DEC,GUIDE_SPEED[1]);
        }
		      
       else if(status==2)  // SET
        {
         hp.speed = 2;
         SetCtrlVal(guide_handle,GUIDE_SPEED_RA ,GSET_SPEED[0]);
         SetCtrlVal(guide_handle,GUIDE_SPEED_DEC,GSET_SPEED[1]);
        }
		      
       else if(status==3)  // SLEW
         {
          hp.speed = 3;
          SetCtrlVal(guide_handle,GUIDE_SPEED_RA ,SLEW_SPEED[0]);
          SetCtrlVal(guide_handle,GUIDE_SPEED_DEC,SLEW_SPEED[1]);
         }
       }

    else if(control==GUIDE_RING_MOTOR)
	  {
	   GetCtrlVal(panel,control,&status);
	   if     (status==0) {hp.motor = 1;} // RA/DEC
	   else if(status==1) {hp.motor = 3;} // FOCUS
	  }

    else if(control==GUIDE_W)
      {
       GetCtrlVal(panel,GUIDE_W,&status);
       if(status==1)
         {GetCtrlVal(panel,GUIDE_E,&status);
          if(status==1) SetCtrlVal(panel,GUIDE_E,0);
          hp.ew =  1; do_guide_ew(hp);
         }
       else  // stop
         {hp.ew = 0; do_guide_ew(hp);}
      }

    else if(control==GUIDE_E)
      {
       GetCtrlVal(panel,GUIDE_E,&status);
       if(status==1)
         {GetCtrlVal(panel,GUIDE_W,&status);
          if(status==1) SetCtrlVal(panel,GUIDE_W,0);
          hp.ew =-1; do_guide_ew(hp);
         }
       else
         {hp.ew = 0; do_guide_ew(hp);}
      }
		      
    else if(control==GUIDE_N)
      {
       GetCtrlVal(panel,GUIDE_N,&status);
       if(status==1)
         {GetCtrlVal(panel,GUIDE_S,&status);
          if(status==1) SetCtrlVal(panel,GUIDE_S,0);
          hp.ns = 1; do_guide_ns(hp);
         }
       else
         {hp.ns = 0; do_guide_ns(hp);}
      }

    else if(control==GUIDE_S)
	  {
       GetCtrlVal(panel,GUIDE_S,&status);
       if(status==1)
         {GetCtrlVal(panel,GUIDE_N,&status);
          if(status==1) SetCtrlVal(panel,GUIDE_N,0);
          hp.ns =-1; do_guide_ns(hp);
         }
       else
         {hp.ns = 0; do_guide_ns(hp);}
      }
		      
	return 0;
}
//-----------------------------------------------------------------------------
static int do_mouse_guide_new()
//-----------------------------------------------------------------------------
{
 hp_status NEW;
 int panel, status;
 double delta_ha_count, delta_dec_count;
 
    panel = guide_handle;

//  check speed 
    GetCtrlVal(panel,GUIDE_RING_SPEED,&status); 
    
    if     (status==3) return 0;
    else if(status==1) // guide
      {
       delta_ha_count  = 60.0*GUIDE_SPEED[0]*GUIDE_TIME_MOUSE * MOUNT_HA [1] / cos(TARGET_RA_2000.rad);
       delta_dec_count = 60.0*GUIDE_SPEED[1]*GUIDE_TIME_MOUSE * MOUNT_DEC[1];
      }
    
    else if(status==2) // set
      {
       delta_ha_count  = 60.0*GSET_SPEED[0]*GUIDE_TIME_MOUSE * MOUNT_HA [1] / cos(TARGET_RA_2000.rad);
       delta_dec_count = 60.0*GSET_SPEED[1]*GUIDE_TIME_MOUSE * MOUNT_DEC[1];
      }
      
//  check West button      
    GetCtrlVal(panel,GUIDE_W,&status);
    if(status==1) OFFSET_HA_COUNT += delta_ha_count;
    
//  check East button      
    GetCtrlVal(panel,GUIDE_E,&status);
    if(status==1) OFFSET_HA_COUNT -= delta_ha_count;

//  check North button      
    GetCtrlVal(panel,GUIDE_N,&status);
    if(status==1) OFFSET_DEC_COUNT += delta_dec_count;
    
//  check South button      
    GetCtrlVal(panel,GUIDE_S,&status);
    if(status==1) OFFSET_DEC_COUNT -= delta_dec_count;
    
    return 0;
}
//-----------------------------------------------------------------------------
int adjust_offset(h_m_s ra_2000, d_m_s dec_2000)
//-----------------------------------------------------------------------------
{
 h_m_s last,ap_ha;
 d_m_s ap_dec,alt,azi;
 double jd_ut1,jd_ut2;
 long step_ha, step_dec;
 FILE *fin;
 
//  read current position
    if(ENCODER_MODE==0) {read_counter('A',&step_ha); read_counter('B',&step_dec);}
    else                {read_encoder('A',&step_ha); read_encoder('B',&step_dec);}

//  calculate the error at the moment
//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    if(TELESCOPE_TYPE==_ALT_AZI)
      {
       OFFSET_HA_COUNT  = step_ha  - calc_ha_count (azi.rad,alt.rad);
       OFFSET_DEC_COUNT = step_dec - calc_dec_count(azi.rad,alt.rad);
      }
    else
      {
       OFFSET_HA_COUNT  = step_ha  - calc_ha_count (ap_ha.rad,ap_dec.rad);
       OFFSET_DEC_COUNT = step_dec - calc_dec_count(ap_ha.rad,ap_dec.rad);
      }

//  printf("OFFSET count   R.A. %10.1lf    DEC %10.1lf\n",OFFSET_HA_COUNT,OFFSET_DEC_COUNT);

    MOUNT_HA [0] += OFFSET_HA_COUNT;
    MOUNT_DEC[0] += OFFSET_DEC_COUNT;

//  printf("Mount 0count   R.A. %10.1lf    DEC %10.1lf\n",MOUNT_HA[0],MOUNT_DEC[0]);
    fin = fopen("offset.out","w");
    fprintf(fin,"%10.1lf %10.1lf offset     of R.A. and DEC.\n", OFFSET_HA_COUNT, OFFSET_DEC_COUNT);
    fprintf(fin,"%10.1lf %10.1lf zero point of R.A. and DEC.", MOUNT_HA[0],MOUNT_DEC[0]);
    fclose(fin);
    
    OFFSET_HA_COUNT = OFFSET_DEC_COUNT = 0;
    
    OFFSET_HA_ORIGINAL = OFFSET_HA_COUNT;
    OFFSET_DEC_ORIGINAL=OFFSET_DEC_COUNT;
    
    return 0;
}
//-----------------------------------------------------------------------------
void init_hp(hp_status *hp)
//-----------------------------------------------------------------------------
{
// typedef  struct HHHH {int ew; int ns; int speed; int mode; int motor;} hp_status;
 hp->ew    =0;
 hp->ns    =0;
 hp->speed =1;  //guide
 hp->mode  =0;
 hp->motor =1;  //ra_dec
}
//-----------------------------------------------------------------------------
