//-----------------------------------------------------------------------------
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
#include <ansi_c.h>
#include <utility.h>
#include <tcpsupp.h>

//#include "type_def.h"
#include "c:\lib\cal_lib\type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "cvi_util.h"
#include "st4s_lib.h" 
#include "tel_sub.h"
#include "cal_sub.h"

#include "tcs.h"
#include "ec_test.h"
//#include "st4s_dome_sub.h"
//#include "tcp_server_sub.h"

extern int adjuat_focus(int dir);

extern int jjjjj=9;
#include "AXM.h"

static char UIR_FILE_NAME[20] = "tcs.uir";
extern void ActivateServer(void);
extern void DeactivateServer(void);

extern jjjj=0;

extern int load_tcs(void);
extern int calc_sun(void);
//------------------------------------------------------------------------
extern int load_tcp_server(void);
extern int load_util_panel(void);
extern void begin_encoder_test(void);

extern int dome_move(double azi);
extern int link_to_dome(void);
extern int load_dome_panel(void);
extern double check_dome_angle(double azi);
//extern double track_dome(double azi);

extern int load_cat_panel(int panel_handle);
extern int tcp_write(char buffer[]);
extern int load_guide_panel(h_m_s ra_2000, d_m_s dec_2000);
extern int load_scan_panel (int panel_handle);

extern void begin_encoder_test(void);

extern	char CloseDevice(void);
extern int write_sky_scan_out_new(int star_no); 
extern int adjust_offset(h_m_s ra_2000, d_m_s dec_2000);  

int CVICALLBACK THREAD_PARKING(void *data);
int CVICALLBACK THREAD_INIT   (void *data);
//------------------------------------------------------------------------
extern int    DOME_HOME_STATUS;
//------------------------------------------------------------------------
//                         Global Variables 
//                 Declared as EXTERNAL in gv_tel.h      
//------------------------------------------------------------------------
/*
  TELESCOPE_MODE에 대한 설명  설명 
  _TEL_MODE_TRACK
    어떤 별을 추적하라고 명령하면 TELESCPE_MODE = _TEL_MODE_TRACK으로 새팅된다. 
    동시에 SLEW_MODE[0] = SLEW_MOE[1] = SLEW_MOE[2] = _SLEW_MODE_ON으로 새팅된다.
    각 축의 SLEW가 종료되고 정밀 트래킹 모드에 들어가면 그축의 SLEW_MODE는 _SLEW_MODE_OFF로 새팅된다.
    따라서 모든 축의 SLEW_MODE가 _SLEW_MODE_OFF로 되면 비로소 정밀 트래킹이 진행중임을 알 수 있다.

  _TEL_MODE_SLEW
    망원경을 어떤 특정 위치로 이동시키고, 이동이 끝나면 정지시키는 모드이다.

  _TEL_MODE_STOP
    현재 망원경이 정지 상태인 것을 의미한다.

  _TEL_MODE_PARK
    현재 망원경이 파킹 위치로 이동 중인 것을 의미한다.

  _TEL_MODE_INIT
    현재 망원경이 초기화 위치로 이동 중인 것을 의미한다.
    
  OFFSET COUNTS
    OFFSET = OBSERVED COUNTS - CALCULATED COUNTS from MOUNT MODEL
    
*/    
//------------------------------------------------------------------------
#include "gv_tel.h"

/*
int  TELESCOPE_TYPE;
enum {_ALT_AZI, _RA_DEC};

int  TELESCOPE_MODE;                            
enum {TEL_MODE_STOP, TEL_MODE_SLEW, TEL_MODE_TRACK, TEL_MODE_PARK, TEL_MODE_INIT};


double MOUNT_HA[20],MOUNT_DEC[20],ENC_TO_STEP[2];  // mount.inp
double PSI_FACTOR;
long   INIT_COUNT[4], HOME_COUNT[4];

int    ENCODER_MODE;
double OFFSET_HA_COUNT,OFFSET_DEC_COUNT;
double OFFSET_HA_ARCMINUTE, OFFSET_DEC_ARCMINUTE ;
double DUT1_SEC,DUT1_DAY;                          // dut1.inp

double MEAN_TRACKING_FREQ[4];                      // calculated during init_tel
double PARK_FREQ [4], PARK_SPEED [4],              // init_tel.inp
       INIT_FREQ [4], INIT_SPEED [4],
       GUIDE_FREQ[4], GUIDE_SPEED[4],
       GSET_FREQ [4], GSET_SPEED [4],
       SLEW_FREQ [4], SLEW_SPEED [4];
//int    HOME_SW_STATUS[4],CW_SW_STATUS[4],CCW_SW_STATUS[4];
       
double SERVO_UPDATE_SEC;
char   STAR_STRING[150];
double MAX_ERROR_RA_ARCSEC, MAX_ERROR_DEC_ARCSEC;
double JDUTC_0;
//double F_1_SLEW, F_1_TRACK = 100000.0;

double HALIMITMAX,HALIMITMIN; 

int   DER_TRACKING;
enum {DER_TRACKING_ON,DER_TRACKING_OFF};


int SW_LIMIT;
int LINKDOME;
int NO_OF_AXIS;

ramp_data     VPROFILE[4];
switch_status SWSTATUS[4];
home_para HOMEP[2];
int USEPRELOAD;
*/
static const double  TWOPI = 3.14159265358979323846*2.0;
int DOME_TRACKING;
enum {DOME_TRACKING_ON,DOME_TRACKING_OFF};

static int CHECK_PARKINGA,CHECK_PARKINGB;
static int CHECK_INITA,CHECK_INITB;
int CALCSUN=0;
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//                      Prototype of functions    
//------------------------------------------------------------------------
int SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000);
int STOP_TELESCOPE(void);
int display_input_window(h_m_s ra_2000, d_m_s dec_2000, char star_name[]);
void display_track_speed(double track_speed_azi,double track_speed_alt);
int send_status_to_client(void);
int read_input_files(void);

static int check_slew_track(char caxis);
static int check_tracking_altazi(void);
static int check_tracking_radec (void);

int start_manu_parking (void);
static int check_parking(void);

static int start_init(void);
       int auto_init(void);
       int check_init(void);

static int check_slew    (void);
static int display_tel_status(void);
static void display_telescope_mode(void);
static void sync_clock(void);
//static void set_main_timer(void);
static int  check_dome(h_m_s ra_2000, d_m_s dec_2000);
int read_limit_status(void);

int check_limit(void);
void display_message(char buff[]);

//static void message_popup(char message[]);
static int close_program(void);

int save_axpara(void);
int read_axpara(void);

int goto_target(void); 

int set_offset(void);
int set_offset2(void);

int focusadjust(void);
int focus_tick(int control);

int read_nodeno(void);

int initiation_tcs(void);

char GBUFF[500];

//------------------------------------------------------------------------
//                         static variables
//------------------------------------------------------------------------
h_m_s  TARGET_RA_2000 ,CURRENT_RA, CURRENT_LAST ;
d_m_s  TARGET_DEC_2000,CURRENT_DEC;
char   TARGET_NAME[100];

static h_m_s  HA;
static d_m_s  DEC_APP,ALT,AZI;
double ALT_MIN = 10;

d_m_s  GAZI;
static double jd_ut1,azi,alt;
int   tel_handle,park_handle,slew_handle,cat_handle;
static int    tcp_handle,scan_handle,guide_handle;
static char   text_buffer[30];
//static int    tcp_server_port_no = 10000;
static int    COM_PORT;
static double DOME_ERROR;
static double ERROR_DER;
static double ERROR_AXIS[4];
static int    CW_LIMIT[4],CCW_LIMIT[4],HOME_LIMIT[4];
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int main (int argc, char *argv[])
//------------------------------------------------------------------------
{
 FILE *fin;
 
//  initialize paramters	
    TELESCOPE_TYPE = _ALT_AZI;
//  TELESCOPE_TYPE = _RA_DEC;
    TELESCOPE_MODE = TEL_MODE_STOP;
    SLEW_MODE[0]   = SLEW_MODE[1] = SLEW_MODE[2] = _SLEW_MODE_OFF;
    GUIDE_MODE     = GUIDE_MODE_OFF;
    DOME_TRACKING  = DOME_TRACKING_OFF;
    DER_TRACKING   = DER_TRACKING_OFF; 
    
	NODENO = 0;
	
    if(InitCVIRTE (0, argv, 0) == 0) return -1;	/* out of memory */
    
//  load the control panel    
    if((tel_handle = LoadPanel (0, UIR_FILE_NAME, TEL)) < 0) return -1;
    DisplayPanel (tel_handle);
    
    read_input_files();
	sprintf(GBUFF,"TCS Telscopse No. %d",NODENO+1);
    SetPanelAttribute (tel_handle, ATTR_TITLE, GBUFF);
	
    sync_clock();
    
//  open JPL ephmeris calculation    
    init_jpl("c:\\lib\\jpleph.dos");
     
    turn_on_timer(tel_handle, TEL_MAIN_TIMER); 
    set_timer_interval(tel_handle, TEL_MAIN_TIMER,SERVO_UPDATE_SEC); 

//  SW_LIMIT = 0;
	
	USEPRELOAD=0;
	

/*    
    if(get_parking_status()!=1) MessagePopup("","망원경이 파킹 위치에 있지 않습니다...");

	
//  initialize some controls    
    SetCtrlAttribute(tel_handle,TEL_DOME_ONOFF,ATTR_ON_COLOR ,VAL_RED       );
    SetCtrlAttribute(tel_handle,TEL_DOME_ONOFF,ATTR_OFF_COLOR,VAL_PANEL_GRAY);
    if(DOME_TRACKING==DOME_TRACKING_ON) SetCtrlVal(tel_handle,TEL_DOME_ONOFF,1);
    else                                SetCtrlVal(tel_handle,TEL_DOME_ONOFF,0);

//  open_tcp_server(&tcp_handle);
    
//  set_control_dim(tel_handle, TEL_DOME_ONOFF); 
    
    set_domeold();
*/ 
	
	
	
    RunUserInterface();
    
    DiscardPanel(tel_handle);

    return 0;
}
//------------------------------------------------------------------------
int read_input_files(void)
//------------------------------------------------------------------------
{
    read_telescope_parameter();
    read_tcs_parameter();
    init_telescope_parameter();
	
	sprintf(GBUFF,"SLEW_FREQ[1] = %10.3lf\n",SLEW_FREQ[0]); display_message(GBUFF);
	sprintf(GBUFF,"SLEW_FREQ[2] = %10.3lf\n",SLEW_FREQ[1]); display_message(GBUFF);
	
	read_nodeno();
    
    return 0;
}
//------------------------------------------------------------------------
int read_nodeno()
//------------------------------------------------------------------------
{
 FILE *fin;
 
    if( (fin = fopen("nodeno.inp","r"))==NULL)
      {MessagePopup(""," CANNOT OPEN NODENO.INP"); return -1;}

    fgets (GBUFF,200,fin);
    sscanf(GBUFF,"%d",&NODENO);
	
    fgets(GBUFF,200,fin);
    sscanf(GBUFF,"%lf %lf %lf %lf",&ENCTOSTEP_RA[0],&ENCTOSTEP_RA[1],&ENCTOSTEP_RA[2],&ENCTOSTEP_RA[3]);

	fgets(GBUFF,200,fin);
    sscanf(GBUFF,"%lf %lf %lf %lf",&ENCTOSTEP_DE[0],&ENCTOSTEP_DE[1],&ENCTOSTEP_DE[2],&ENCTOSTEP_DE[3]);
	
	fclose(fin);
	
	return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK APPLY (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
 char buff[100]; int status;
 double offazi,offalt;
 
	switch (event) {
		case EVENT_COMMIT:
		     if(control==TEL_PARKING) start_manu_parking();
		     if(control==TEL_INIT   ) start_init();
		     if(control==TEL_STOP   )
			  {	 
			   if(TELESCOPE_MODE==TEL_MODE_PARK)
			    {AxmHomeSetResult(0,HOME_SUCCESS); AxmHomeSetResult(1,HOME_SUCCESS);} 
			   STOP_TELESCOPE();
			  }	 
			 
		     else if(control==TEL_GUIDE)
	           {											   
		        GUIDE_MODE = GUIDE_MODE_MOUSE;
	            guide_handle = load_guide_panel(TARGET_RA_2000,TARGET_DEC_2000);
	           }
			 
	         else if(control==TEL_GOTO) goto_target();
			 
 	         else if(control==TEL_GOOFF)
               {
                  GetCtrlVal(tel_handle,TEL_OFFRA ,&offazi);
				  GetCtrlVal(tel_handle,TEL_OFFDEC,&offalt);
				  OFFSET_HA_COUNT  = offazi*3600.0*MOUNT_HA [1];
				  OFFSET_DEC_COUNT = offalt*3600.0*MOUNT_DEC[1];   
               }
			 
 	         else if(control==TEL_INITTCS) initiation_tcs();
 	         else if(control==TEL_CALCSUN) GetCtrlVal(tel_handle,TEL_CALCSUN,&CALCSUN);
			 
			  else if(control==TEL_SAVEOFF) {write_sky_scan_out_new(1); adjust_offset(TARGET_RA_2000, TARGET_DEC_2000);}

			break;
	}
	return 0;
}
//------------------------------------------------------------------------
void CVICALLBACK MBCB (int menuBar, int menuItem, void *callbackData,int panel)
//------------------------------------------------------------------------
{
    if(menuItem==MBFILE_File_TCS ) load_tcs();
    else if(menuItem==MBFILE_File_SCAN) scan_handle = load_scan_panel(0);
    else if(menuItem==MBFILE_File_CATALOGUE) cat_handle = load_cat_panel(0);    
    else if(menuItem==MBFILE_File_READAXP) read_axpara();    
    else if(menuItem==MBFILE_File_SAVEAXP) save_axpara();    
    else if(menuItem==MBFILE_File_ECTEST)begin_encoder_test();
	
    else if(menuItem==MBFILE_File_PRELOAD_CONNECT   ) USEPRELOAD=1;
    else if(menuItem==MBFILE_File_PRELOAD_DISCONNECT) USEPRELOAD=0;
    else if(menuItem==MBFILE_File_THESKY_CONNECT   ) ActivateServer();
    else if(menuItem==MBFILE_File_THESKY_DISCONNECT) DeactivateServer();
    
    else if(menuItem==MBFILE_File_DOME   )load_dome_panel();
//    else if(menuItem==MBFILE_File_NETWORK)load_tcp_server();
//    else if(menuItem==MBFILE_File_UTIL   )load_util_panel();
//    else if(menuItem==MBFILE_File_READINPUTS) {read_input_files(); 
//                                              // init_tcs(COM_PORT);
//                                              }
                                              
}
//------------------------------------------------------------------------
int CVICALLBACK PanelCB (int panel, int event, void *callbackData,int eventData1, int eventData2)
//------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
             close_program();
			break;
		}
	return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK TEXTCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_COMMIT:

			break;
		case EVENT_RIGHT_CLICK:
		     ResetTextBox(panel,control,"");
			break;
		}
	return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK CB_TIMER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
 char limit_status;
 
	switch (event) {
		case EVENT_TIMER_TICK:
//           calculate LAST
             jd_ut1 = get_jdutc_clock() + DUT1_DAY;
             CURRENT_LAST.rad = jdtolast(jd_ut1,SITE_LONGI.rad);
             rad_to_hms(&CURRENT_LAST);

			 if(GUIDE_MODE==GUIDE_MODE_OFF) 
			  { 
               if(SLEW_MODE[0]==_SLEW_MODE_ON) set_preload_off('A');
               else                            set_preload_on ('A');
				 
               if(SLEW_MODE[1]==_SLEW_MODE_ON) set_preload_off('B');
               else                            set_preload_on ('B');
			  }
             
             if(TELESCOPE_MODE==TEL_MODE_TRACK)
               {
                if(GUIDE_MODE==GUIDE_MODE_OFF ||GUIDE_MODE==GUIDE_MODE_ST4)
				  {
				   if(TELESCOPE_TYPE==_RA_DEC) check_tracking_radec();       
				   else                        check_tracking_altazi();
				  }
                   
//              check dome position                      
                if(DOME_TRACKING==DOME_TRACKING_ON) check_dome(TARGET_RA_2000,TARGET_DEC_2000);
               }
               
             else if(TELESCOPE_MODE == TEL_MODE_SLEW ) {
				 check_slew();
		
			 }
             else if(TELESCOPE_MODE == TEL_MODE_PARK ) {preload_off(); check_parking();}
             else if(TELESCOPE_MODE == TEL_MODE_INIT )  check_parking(); //check_init();
//           check limit switch status
             Delay(0.05);
//           if(SW_LIMIT==1) check_limit();

             display_tel_status();
			break;
	}
	
	return 0;
}
//------------------------------------------------------------------------
static int close_program()
//------------------------------------------------------------------------
{
 int state1,state2, state3;
 
    copy_panel_to_clipboard(tel_handle);		
	
    STOP_TELESCOPE();
    
    read_home_limit('A',&state1);
    read_home_limit('B',&state2);
    
    if(state1!=SWSTATUS[0].home || state2!= SWSTATUS[1].home)
      {Beep();
//     if(ConfirmPopup("","망원경을 파킹시킬까요 ? ")==1){start_manu_parking(); return 0;}
      }

//  close_serial_port(COM_PORT);
    close_stdio();

//  close JPL ephmeris calculation    
    close_jpl();
    
//  UnregisterTCPServer(tcp_server_port_no);

//    save_domeold(); 
	
	CloseDevice();


    QuitUserInterface(0);

	return 0;
}
//------------------------------------------------------------------------
void display_message(char buff[])
//------------------------------------------------------------------------
{
//  SetCtrlVal(tel_handle,TEL_MESSAGE,"\n");
    SetCtrlVal(tel_handle,TEL_MESSAGE,buff);
}
//------------------------------------------------------------------------
void reset_message(char buff[])
//------------------------------------------------------------------------
{
    ResetTextBox(tel_handle,TEL_MESSAGE,buff);
}
//------------------------------------------------------------------------
static void  sync_clock()
//------------------------------------------------------------------------
{
 int year,month,day,hour,minute,second,old_sec;
 int i;

    GetSystemTime(&hour,&minute,&old_sec);
    i =0;
    do
     {
      GetSystemTime(&hour,&minute,&second);
      i++;
     } while(second==old_sec);

    GetSystemDate(&month,&day,&year);
    Timer();
        
    JDUTC_0 = get_jd(year,month,day,0.0);
    JDUTC_0 = JDUTC_0 -9.0/24.0 + (hour+minute/60.0+second/3600.0)/24.0;
}
//------------------------------------------------------------------------
static int display_tel_status()
//------------------------------------------------------------------------
{
 int year,month,day,hour,minute,second;
 int status;
 long count_ha,count_de;
 
//    if(LOOP_COUNT<0) util_test();
 
//  display telescope mode
    display_telescope_mode();
    
//  display date and time    
    GetSystemDate(&month,&day,&year);
    GetSystemTime(&hour,&minute,&second);
    
//  Date    
    sprintf(text_buffer,"%2d월%2d일",month,day);
    SetCtrlVal(tel_handle,TEL_DATE,text_buffer);
    
//  KST        
    sprintf(text_buffer,"%2dh%2dm%2ds",hour,minute,second);
    SetCtrlVal(tel_handle,TEL_KST,text_buffer);

//  display LAST        
    sprintf(text_buffer,"%2dh%2dm%2.0lfs",CURRENT_LAST.h,CURRENT_LAST.m,CURRENT_LAST.s);
    SetCtrlVal(tel_handle,TEL_LAST ,text_buffer);

//  read counter and calculate HA and DEC
    if(ENCODER_MODE==0)
      {read_counter('A',&count_ha); read_counter('B',&count_de);}
    else
      {read_encoder('A',&count_ha); read_encoder('B',&count_de);}

    if(TELESCOPE_TYPE==_RA_DEC)          
      {
       status=calc_position1(count_ha,count_de, &HA.rad, &DEC_APP.rad);
//	   rad_to_hms(&HA); 
//	   sprintf(GBUFF,"%5.2lf\n",HA.rh);
//     display_message(GBUFF);

       hadec_to_altazi(HA.rad,DEC_APP.rad,SITE_LATI.rad, &ALT.rad,&AZI.rad);
       
       CURRENT_RA.rad = CURRENT_LAST.rad - HA.rad;
       if(CURRENT_RA.rad<0.0  ) CURRENT_RA.rad += TWOPI;
       if(CURRENT_RA.rad>TWOPI) CURRENT_RA.rad -= TWOPI;
       rad_to_hms(&CURRENT_RA);
       
       CURRENT_DEC.rad= DEC_APP.rad;
       rad_to_dms(&CURRENT_DEC);
      }
    else if(TELESCOPE_TYPE==_ALT_AZI)          
      {
       calc_position1(count_ha,count_de, &AZI.rad, &ALT.rad);
       altazi_to_hadec(ALT.rad,AZI.rad,SITE_LATI.rad, &HA.rad,&DEC_APP.rad);
      }

    rad_to_hms(&HA); 
    rad_to_dms(&DEC_APP);
    rad_to_dms(&ALT);
    rad_to_dms(&AZI);
    GAZI.rad = AZI.rad;
//  rad_to_dms(&ALT);

//  display HA        
    sprintf(text_buffer,"%3dh%2dm%2.0lfs",HA.h,HA.m,HA.s);
    SetCtrlVal(tel_handle,TEL_HA ,text_buffer);

//  display DEC
    sprintf(text_buffer,"%3dd%2dm%2.0lfs",DEC_APP.d,DEC_APP.m,DEC_APP.s);
    SetCtrlVal(tel_handle,TEL_DEC,text_buffer);

//  display ALT
    sprintf(text_buffer,"%3dd%2dm%2.0lfs",ALT.d,ALT.m,ALT.s);
    SetCtrlVal(tel_handle,TEL_ALT,text_buffer);

//  display AZI
    sprintf(text_buffer,"%3dd%2dm%2.0lfs",AZI.d,AZI.m,AZI.s);
    SetCtrlVal(tel_handle,TEL_AZI,text_buffer);

//  limit switch status
    read_limit_status();

    SetCtrlVal(tel_handle,TEL_RA_CW   ,  CW_LIMIT[0]);
    SetCtrlVal(tel_handle,TEL_RA_CCW  , CCW_LIMIT[0]);
    SetCtrlVal(tel_handle,TEL_RA_HOME ,HOME_LIMIT[0]);
        
    SetCtrlVal(tel_handle,TEL_DEC_CW  ,  CW_LIMIT[1]);
    SetCtrlVal(tel_handle,TEL_DEC_CCW , CCW_LIMIT[1]);
    SetCtrlVal(tel_handle,TEL_DEC_HOME,HOME_LIMIT[1]);
    
    return 0;
}             
//------------------------------------------------------------------------
static void display_telescope_mode(void)
//------------------------------------------------------------------------
{
 static int count;
 
    if(count>0)
      {
       SetCtrlVal(tel_handle,TEL_TEXTMSG_7,  "         ");
       count = 0;
      }
    else
      {
//     display telescope mode
       if(TELESCOPE_MODE==TEL_MODE_STOP)
         SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"망원경 정지");
       
       else if(TELESCOPE_MODE==TEL_MODE_SLEW)
	   { SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"망원경 슬루");
	    sprintf(GBUFF,"망원경 슬루");
	   				
				
    SetPanelAttribute (tel_handle, ATTR_TITLE, GBUFF);}

       else if(TELESCOPE_MODE==TEL_MODE_TRACK)
         {if(SLEW_MODE[0]==_SLEW_MODE_OFF && SLEW_MODE[1]==_SLEW_MODE_OFF)
          SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"망원경 추적");
          else
          SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"망원경 슬루");
         }
       else if(TELESCOPE_MODE==TEL_MODE_INIT)
         SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"초기화진행중");

       else if(TELESCOPE_MODE==TEL_MODE_PARK)
         SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"파킹 진행중");
         
       count++;
      }
}      
//------------------------------------------------------------------------
static void set_main_timer()
//------------------------------------------------------------------------
{
     set_timer_interval(tel_handle,TEL_MAIN_TIMER,SERVO_UPDATE_SEC);
}       
//------------------------------------------------------------------------


//------------------------------------------------------------------------
int CVICALLBACK CB_PARKING (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
 int dir,state;
 
 
	switch (event) {
		case EVENT_COMMIT:
		
		     if(control==PARK_START)
		       {
                SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"파킹 진행중");
                GetCtrlVal(park_handle,PARK_DIR_RA,&dir);
				if(dir==1) go_const('A', PARK_FREQ[0]);
				else       go_const('A',-PARK_FREQ[0]);

//                goto_home(0,dir);
                  
                GetCtrlVal(park_handle,PARK_DIR_DEC,&dir);
//                goto_home(1,dir);
				if(dir==1) go_const('B', PARK_FREQ[1]);
				else       go_const('B',-PARK_FREQ[1]);
                  
                TELESCOPE_MODE = TEL_MODE_PARK;
	            CHECK_PARKINGA=CHECK_PARKINGB=1;
 	            CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,THREAD_PARKING,NULL,NULL);
				
                DiscardPanel(park_handle);
               }
               
             else if(control==PARK_CLOSE)
               {
//              turn_off_timer(tel_handle,TEL_TIMER_2);
                DiscardPanel(park_handle);
               }
             
			 break;
	}
	return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK THREAD_PARKING(void *data)
//------------------------------------------------------------------------
{
 int i,state;
 char ccc[100];
    i=0;
    for(;;)
	   {
        Delay(0.1);
	    if(TELESCOPE_MODE==TEL_MODE_STOP) break;
		
	    if(CHECK_PARKINGA==1 || CHECK_PARKINGB==1) check_parking();
	    if(CHECK_PARKINGA==0 && CHECK_PARKINGB==0) {TELESCOPE_MODE=TEL_MODE_STOP; break;}
	   }
 return 0;	
}
//------------------------------------------------------------------------
static int check_parking()
//------------------------------------------------------------------------
{
 DWORD result1,result2;
 int state0,state1;
 
	  if(CHECK_PARKINGA==1)
	    {
	     read_home_limit('A',&state0);
		 if(state0==SWSTATUS[0].home) {CHECK_PARKINGA=0; motor_stop('A');}
		}
	  if(CHECK_PARKINGB==1)
	    {  
	     read_home_limit('B',&state1);
		 if(state1==SWSTATUS[1].home) {CHECK_PARKINGB=0; motor_stop('B');}
		}
	  
	return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK THREAD_INIT(void *data)
//------------------------------------------------------------------------
{
 int i,state;
 char ccc[100];
    i=0;
    for(;;)
	   {
        Delay(0.01);
	    if(TELESCOPE_MODE==TEL_MODE_STOP) break;
		
	    if(CHECK_INITA==1 || CHECK_INITB==1) check_init();
	    if(CHECK_INITA==0 && CHECK_INITB==0) {TELESCOPE_MODE=TEL_MODE_STOP; break;}
	   }
 return 0;	
}
//------------------------------------------------------------------------
static int check_init()
//------------------------------------------------------------------------
{
 DWORD result1,result2;
 int state0,state1;
 
	  if(CHECK_INITA==1)
	    {
	     read_home_limit('A',&state0);
		 if(state0!=SWSTATUS[0].home) {set_counter('A',10000); set_encoder('A',INIT_COUNT[0]);CHECK_INITA=0; motor_stop('A');}
		}
	  if(CHECK_INITB==1)
	    {  
	     read_home_limit('B',&state1);
		 if(state1!=SWSTATUS[1].home) {set_counter('B',10000); set_encoder('B',INIT_COUNT[1]); CHECK_INITB=0; motor_stop('B');}
		}
	  
	return 0;
}
//------------------------------------------------------------------------



//------------------------------------------------------------------------
static int check_slew_track(char caxis)
//------------------------------------------------------------------------
{
 long count_ha, count_dec, steps,lll;
 double error[2];
 int iaxis,mode;

    iaxis = caxis - 'A';
    
    if(TELESCOPE_TYPE==_RA_DEC)
       calc_error_count_radec (TARGET_RA_2000,TARGET_DEC_2000,&error[0],&error[1]);
    else
       calc_error_count_altazi(TARGET_RA_2000,TARGET_DEC_2000,&error[0],&error[1]);
       
    if     (caxis=='A') ERROR_AXIS[0] = error[iaxis]/(MOUNT_HA [1]);
    else if(caxis=='B') ERROR_AXIS[1] = error[iaxis]/(MOUNT_DEC[1]);

    if(ENCODER_MODE==1) steps = error[iaxis] * ENC_TO_STEP[iaxis];
    else                steps = error[iaxis] + 0.5;
    
//  display error            
    if(caxis=='A')
      {
       sprintf(text_buffer,"%8.2lfdeg",ERROR_AXIS[0]/3600.0);
       SetCtrlVal(tel_handle,TEL_HA_ERROR ,text_buffer);
      }
    else if(caxis=='B')
      {
       sprintf(text_buffer,"%8.2lfdeg",ERROR_AXIS[1]/3600.0);
       SetCtrlVal(tel_handle,TEL_DEC_ERROR,text_buffer);
      }
    
    read_slew_mode(caxis,&mode);
    if(mode!=0) return 0;

//  even though slew is done, the error may be too large. Then correct the error    
    if(fabs(ERROR_AXIS[iaxis]/3600.0) > 0.2)
      {slew_stop(caxis,steps);}
    else 
       SLEW_MODE[iaxis] = _SLEW_MODE_OFF;

	return 0;
}
//------------------------------------------------------------------------
static int check_tracking_radec()
//------------------------------------------------------------------------
{
 long count_ra, count_dec, steps;
 double track_speed;
 int mode;
 
    if(SLEW_MODE[0]==_SLEW_MODE_ON || SLEW_MODE[1]==_SLEW_MODE_ON)
      {
       if(SLEW_MODE[0]==_SLEW_MODE_ON) check_slew_track('A');
       else correct_servo_ra(TARGET_RA_2000,TARGET_DEC_2000, &count_ra,&ERROR_AXIS[0],&track_speed);

       if(SLEW_MODE[1]==_SLEW_MODE_ON) check_slew_track('B');
       else correct_servo_dec(TARGET_RA_2000,TARGET_DEC_2000, &count_dec,&ERROR_AXIS[1]);
      }

    else
      {
       correct_servo_ra(TARGET_RA_2000,TARGET_DEC_2000, &count_ra,&ERROR_AXIS[0],&track_speed);
       sprintf(text_buffer,"%8.2lfarcsec",ERROR_AXIS[0]);
	   SetCtrlVal(tel_handle,TEL_HA_ERROR,text_buffer);
	   if(fabs(track_speed)>100.0) sprintf(text_buffer,"%8.2lfarcmin",track_speed/60.0);
	   else                        sprintf(text_buffer,"%8.2lfarcsec",track_speed     );
	   SetCtrlVal(tel_handle,TEL_HA_SPEED,text_buffer);
	   
       correct_servo_dec(TARGET_RA_2000,TARGET_DEC_2000, &count_dec,&ERROR_AXIS[1]);
       sprintf(text_buffer,"%8.2lfarcsec",ERROR_AXIS[1]);
	   SetCtrlVal(tel_handle,TEL_DEC_ERROR,text_buffer);
	  }
	
	return 0;
}
//------------------------------------------------------------------------
static int check_tracking_altazi()
//------------------------------------------------------------------------
{
 long count_azi, count_alt, steps;
 double error_azi,error_alt,track_speed_azi,track_speed_alt;
 int mode;
 
    if(SLEW_MODE[0]==_SLEW_MODE_ON || SLEW_MODE[1]==_SLEW_MODE_ON || SLEW_MODE[2]==_SLEW_MODE_ON)
//  if(SLEW_MODE[0]==_SLEW_MODE_ON || SLEW_MODE[1]==_SLEW_MODE_ON)
      {
       if(SLEW_MODE[0]==_SLEW_MODE_ON)
          check_slew_track('A');
       else
          correct_servo_azi(TARGET_RA_2000,TARGET_DEC_2000,&count_azi,&ERROR_AXIS[0],&track_speed_azi);
       
       if(SLEW_MODE[1]==_SLEW_MODE_ON)
          check_slew_track('B');
       else
          correct_servo_alt(TARGET_RA_2000,TARGET_DEC_2000, &count_alt,&ERROR_AXIS[1],&track_speed_alt);
          
//     change the frequency for low speed tracking       
//       if(SLEW_MODE[0]==_SLEW_MODE_OFF && SLEW_MODE[1]==_SLEW_MODE_OFF && SLEW_MODE[2]==_SLEW_MODE_OFF)
//          set_f1(F_1_TRACK);
      }

    else
      {
		  
       if(CALCSUN==1){ calc_sun();
	                  // display_message("calc sun \n");
	                  }
	   
       correct_servo_azi(TARGET_RA_2000,TARGET_DEC_2000,&count_azi,&ERROR_AXIS[0],&track_speed_azi);
       sprintf(text_buffer,"%8.2lfarcsec",ERROR_AXIS[0]);
	   SetCtrlVal(tel_handle,TEL_HA_ERROR,text_buffer);
	   
	   if(fabs(track_speed_azi)>100.0) sprintf(text_buffer,"%8.2lfarcmin",track_speed_azi/60.0);
	   else                            sprintf(text_buffer,"%8.2lfarcsec",track_speed_azi     );
	   SetCtrlVal(tel_handle,TEL_HA_SPEED,text_buffer);
	   
       correct_servo_alt(TARGET_RA_2000,TARGET_DEC_2000, &count_alt,&ERROR_AXIS[1],&track_speed_alt);
       sprintf(text_buffer,"%8.2lf arcsec",ERROR_AXIS[1]);
	   SetCtrlVal(tel_handle,TEL_DEC_ERROR,text_buffer);
	   
	   if(fabs(track_speed_alt)>100.0) sprintf(text_buffer,"%10.2lfarcmin",track_speed_alt/60.0);
	   else                            sprintf(text_buffer,"%10.2lfarcsec",track_speed_alt     );
	   SetCtrlVal(tel_handle,TEL_DEC_SPEED,text_buffer);
	  }
	
	return 0;
}
//------------------------------------------------------------------------
static int check_dome(h_m_s ra_2000, d_m_s dec_2000)
//------------------------------------------------------------------------
{
 double last,ap_ha,ap_dec,alt,azirad;
 
    get_aposi(ra_2000.rad, dec_2000.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
              &last,&ap_ha,&ap_dec,&alt,&azirad);
//  azi = rad_to_deg(azi);              
    
//    DOME_ERROR = track_dome(azirad);
    
    sprintf(text_buffer,"%6.2lf degree",DOME_ERROR);
    SetCtrlVal(tel_handle, TEL_DOME_ERROR,text_buffer);

    return 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static int start_init()
//------------------------------------------------------------------------
{
 int state1, state2;
		
    read_home_limit('A',&state1);
    read_home_limit('B',&state2);
	if(state1!= 1 || state2!= 1)
	  {
	   Beep(); MessagePopup("", "파킹 수행 후 초기화할 것........ ");
	   return -1;
	  }
	  
    SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"초기화진행중");
    
	go_const('A', INIT_FREQ[0]);
	go_const('B', INIT_FREQ[1]);
                  
    TELESCOPE_MODE = TEL_MODE_INIT;
	CHECK_INITA=CHECK_INITB=1;
    CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,THREAD_INIT,NULL,NULL);

	return 0;
}
//------------------------------------------------------------------------
int auto_init()
//------------------------------------------------------------------------
{
 int state;

    TELESCOPE_MODE = TEL_MODE_INIT;
    
    goto_home(0,1);
    goto_home(1,1);
    
    return 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int start_manu_parking()
//------------------------------------------------------------------------
{
 long count;
 int dir,state1,state2,state3;

    if(TELESCOPE_MODE!=TEL_MODE_STOP) STOP_TELESCOPE();

//  check telescope is already at home position
    read_home_limit('A',&state1);
    read_home_limit('B',&state2);
    
    if(DER_TRACKING==DER_TRACKING_OFF)
      {if(state1 == 1 && state2 == 1)
       return 0;
      }
    
    if( (park_handle = LoadPanel(tel_handle,UIR_FILE_NAME, PARK)) < 0)		
    return -1;

    InstallPopup(park_handle);
    SetPanelAttribute (park_handle, ATTR_TITLE, "망원경 파킹 위치로 이동");
	
	read_counter('A',&count);
	if(count>0) SetCtrlVal(park_handle,PARK_DIR_RA,0);
	else        SetCtrlVal(park_handle,PARK_DIR_RA,1);

	read_counter('B',&count);
	if(count>0) SetCtrlVal(park_handle,PARK_DIR_DEC,0);
	else        SetCtrlVal(park_handle,PARK_DIR_DEC,1);
	
	
	return 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static int check_slew()
//------------------------------------------------------------------------
{
 int state1,state2;
 
    read_slew_mode('A',&state1);
    read_slew_mode('B',&state2);
    if(state1==0 && state2==0) TELESCOPE_MODE = TEL_MODE_STOP;
	return 0;
}
//------------------------------------------------------------------------
int send_status_to_client(void)
//------------------------------------------------------------------------
{
 char buffer[100];
 int mode;
 
    if(SLEW_MODE[0]==_SLEW_MODE_ON || SLEW_MODE[1]==_SLEW_MODE_ON)
       mode = TEL_MODE_SLEW;
    else 
       mode = TELESCOPE_MODE;
 
    sprintf(buffer,"STATUS %3d %10.5lf %10.5lf %10.5lf %10.5lf %8.2lf %8.2lf %d %d %d %d %d %d %6.2lf;",
    mode, HA.rh,DEC_APP.rd, ALT.rd, AZI.rd, ERROR_AXIS[0], ERROR_AXIS[1],
    CW_LIMIT[0],CCW_LIMIT[0],HOME_LIMIT[0],
    CW_LIMIT[1],CCW_LIMIT[1],HOME_LIMIT[1],
    DOME_ERROR
    );
//    return tcp_server_write(buffer);
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000)
//------------------------------------------------------------------------
{
 double error_deg;
 
    if(TELESCOPE_MODE != TEL_MODE_STOP) STOP_TELESCOPE();
	
    if(GUIDE_MODE != GUIDE_MODE_OFF)
      {
       MessagePopup("","GUIDE/수동키 를 중지하시오");
       return -1;
      }

	preload_off();
    
    slew_to_radec(ra_2000,dec_2000);
    
    SLEW_MODE[0] = SLEW_MODE[1] = _SLEW_MODE_ON;
    
    if(DER_TRACKING==DER_TRACKING_ON) SLEW_MODE[2] = _SLEW_MODE_ON;
    
    TELESCOPE_MODE = TEL_MODE_TRACK;
    
    TARGET_RA_2000.rad  =  ra_2000.rad; rad_to_hms( &TARGET_RA_2000);
    TARGET_DEC_2000.rad = dec_2000.rad; rad_to_dms(&TARGET_DEC_2000);
    
    SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"망원경 슬루");
    
    display_input_window(ra_2000,dec_2000,TARGET_NAME);
    
    return 0;
}            
//------------------------------------------------------------------------
int display_input_window(h_m_s ra_2000, d_m_s dec_2000, char star_name[])
//------------------------------------------------------------------------
{
    sprintf(text_buffer, "%3d%3d%5.1lf",ra_2000.h,ra_2000.m, ra_2000.s);
    SetCtrlVal(tel_handle, TEL_RA_INPUT, text_buffer);
        
    sprintf(text_buffer, "%3d%3d%5.1lf",dec_2000.d,dec_2000.m, dec_2000.s);
    SetCtrlVal(tel_handle, TEL_DEC_INPUT, text_buffer);
        
    SetCtrlVal(tel_handle, TEL_NAME_INPUT, star_name);
        
    return 0;
}
//------------------------------------------------------------------------
void display_track_speed(double track_speed_azi,double track_speed_alt)
//------------------------------------------------------------------------
{
	if(fabs(track_speed_azi)>100.0) sprintf(text_buffer,"%8.2lfarcmin",track_speed_azi/60.0);
	else                            sprintf(text_buffer,"%8.2lfarcsec",track_speed_azi     );
	SetCtrlVal(tel_handle,TEL_HA_SPEED,text_buffer);

	if(fabs(track_speed_alt)>100.0) sprintf(text_buffer,"%8.2lfarcmin",track_speed_alt/60.0);
	else                            sprintf(text_buffer,"%8.2lfarcsec",track_speed_alt     );
	SetCtrlVal(tel_handle,TEL_DEC_SPEED,text_buffer);
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int STOP_TELESCOPE(void)
//------------------------------------------------------------------------
{		     
    motor_stop('A');
	motor_stop('B');
//  if(DOME_TRACKING == DOME_TRACKING_ON) dome_stop();
    TELESCOPE_MODE = TEL_MODE_STOP;
//  set_main_timer();
    return 0;
}             
//------------------------------------------------------------------------
int read_limit_status()
//------------------------------------------------------------------------
{
 char limit_status;
 int status;
 
    read_cw_limit  ('A',&CW_LIMIT[0]);
 	read_ccw_limit ('A',&CCW_LIMIT[0]);
 	read_home_limit('A',&HOME_LIMIT[0]);
 
    read_cw_limit  ('B',&CW_LIMIT[1]);
 	read_ccw_limit ('B',&CCW_LIMIT[1]);
 	read_home_limit('B',&HOME_LIMIT[1]);
 
    return 0;
}
//------------------------------------------------------------------------
int check_limit()
//------------------------------------------------------------------------
{
 int i,state,dir;
 char caxis;
 
    if(rad_to_deg(ALT.rad)<ALT_MIN)
      {
       STOP_TELESCOPE();
       Beep();
       MessagePopup(""," alt is too low");
       SW_LIMIT=0;
       return 0;
      }
 
    for(i=0;i<NO_OF_AXIS;i++)
       {
        caxis = i+'A';
                 
	    if(CW_LIMIT[i]==1)
	      {read_dir(caxis,&dir); if(dir==1) motor_estop(caxis);
	       Beep();
	       sprintf(GBUFF," Axis %c CW limit\n",caxis);
	       display_message(GBUFF);
	      }
			 
	    if(CCW_LIMIT[i]==1)
	      {read_dir(caxis,&dir); if(dir==0) motor_estop(caxis);
	       Beep();
	       sprintf(GBUFF," Axis %c CCW limit\n",caxis);
  	       display_message(GBUFF);
	      }
       }
       
    return 0;
}
//------------------------------------------------------------------------
int read_axpara()    
//------------------------------------------------------------------------
{
 char fname[300];
 
    file_select("inp",fname);
    return AxmMotLoadParaAll(fname);
 }
//------------------------------------------------------------------------
int save_axpara()    
//------------------------------------------------------------------------
{
 char fname[300];
 
    file_select("*.inp",fname);
    return AxmMotSaveParaAll(fname);
 }
//------------------------------------------------------------------------
int goto_target()
//------------------------------------------------------------------------
{
 char buff[300];
 h_m_s ra;
 d_m_s dec;
 
         GetCtrlVal(tel_handle,TEL_RA_INPUT,buff);
         sscanf (buff,"%d %d %lf",&ra.h,&ra.m,&ra.s);
			 
         GetCtrlVal(tel_handle,TEL_DEC_INPUT,buff);
		 sscanf (buff,"%d %d %lf",&dec.d,&dec.m,&dec.s);
		 
         hms_to_rad(&ra);
		 if(dec.d>0)
         dms_to_rad(&dec);
		 else
		 {
		  dec.d=-dec.d;
		  dms_to_rad(&dec);
          dec.rad = -dec.rad; rad_to_dms(&dec);
		 }
                 
///       slew and track the selected star             
         return SLEW_AND_TRACK(ra,dec);
}
//------------------------------------------------------------------------
int set_offset()
//-----------------------------------------------------------------------------
{
 double value;
 
	GetCtrlVal(tel_handle,TEL_OFFRA,&value);
    OFFSET_HA_COUNT += -value*MOUNT_HA[1]/cos(CURRENT_DEC.rad);
								  
	GetCtrlVal(tel_handle,TEL_OFFDEC,&value);
	OFFSET_DEC_COUNT +=  value*MOUNT_DEC[1]; 
		   								
    return 0;
}
//-----------------------------------------------------------------------------
int set_offset2()
//-----------------------------------------------------------------------------
{
 double value;
 h_m_s ra;
 d_m_s dec;
 
	GetCtrlVal(tel_handle,TEL_OFFRA,&value);
	ra.rad=TARGET_RA_2000.rad+value/cos(TARGET_DEC_2000.rad)/206265.0;
    rad_to_hms(&ra);
		
	GetCtrlVal(tel_handle,TEL_OFFDEC,&value);
	dec.rad=TARGET_DEC_2000.rad+value/206265.0;
	rad_to_dms(&dec);
	
	SLEW_AND_TRACK(ra,dec);
		   								
    return 0;
}
//-----------------------------------------------------------------------------
int focusadjust()
//-----------------------------------------------------------------------------
{
 int state1,state2;
 
	
	return 0;
}
//-----------------------------------------------------------------------------
