//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <cvirte.h>		
#include <userint.h>
#include <ansi_c.h>
#include <windows.h> 
#include <utility.h>
#include "cvi_util.h"
#include "shmtcs.h"
#include "AXL.h"
#include "AXHS.h"
#include "AXM.h"
#include "AXD.h"
#include "type_def.h"
#include "gv_tel.h"
#include "st4s_lib.h"
#include "tel_sub.h"
#include "tel_sub2.h"	 // js made slew_track_radec, slew_track_altazi, calpos_radec, calpose_altazi functions

#include <cvi2009compat.h>
#include <stdio.h>
#include <stdlib.h>
#include <tcpsupp.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Macros for SharedMemory                             */
/*---------------------------------------------------------------------------*/
#define MAXCLIENTS 10
#define CLIENT_RECEIVE_BUFFER_SIZE 1024
#define SERVER_RECEIVE_BUFFER_SIZE 2048
#define COORD_BUFFER_SIZE 256
#define LOCK_NAME  "SHAREDLOCK"
#define MEMORY_NAME "SHAREDMEMORY"
#define SHARED_MEMORY_SIZE (int)sizeof(tSharedMemType)

/*---------------------------------------------------------------------------*/
/* Typedefs for SharedMeory													 */
/*---------------------------------------------------------------------------*/
typedef struct clientAreaRec
{
    DWORD clientProcessID;
    char clientReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
} tClientArea;

typedef struct ServerAreaRec
{
	int now_az_h, now_az_m, now_el_d, now_el_m;
	double now_az_s, now_el_s;
	int now_ra_h, now_ra_m, now_dec_d, now_dec_m;
	double now_ra_s, now_dec_s;
	int cw_limit_az, ccw_limit_az, home_limit_az, cw_limit_el, ccw_limit_el, home_limit_el;
} tServerArea;

typedef struct tcsAreaRec
{
	DWORD tcsProcessID;
	char tcsReceiveBuffer[CLIENT_RECEIVE_BUFFER_SIZE];
	int cmd_kmap;
	int cmd_x_h, cmd_x_m, cmd_y_d, cmd_y_m;
	double cmd_x_s, cmd_y_s;
	int cmd_go_flag, cmd_stop_flag, cmd_park_flag, cmd_tcsinit_flag, cmd_init_flag;     
	double cmd_off_x, cmd_off_y;
} tTcsArea;

typedef struct sharedMemTypeRec
{
    int serverRunning;
    int tcsConnected;
    // Message for Server
    int sendingClientProcessID;
    char serverReceiveBuffer[SERVER_RECEIVE_BUFFER_SIZE];
	tServerArea server;
	tTcsArea tcs;
} tSharedMemType;

/*---------------------------------------------------------------------------*/
/* Variables for shared memory												 */
/*---------------------------------------------------------------------------*/
static int panelHandle;
static CmtThreadLockHandle lock = 0;
static HANDLE hMemMapFile = 0;
static int clientIndex    = -1;
static int bServerApp     = 0;
static DWORD dwWaitResult = 0;
static tSharedMemType *sharedMemory = NULL;
static char msgBuffer[256];

/*---------------------------------------------------------------------------*/
/* Internal function prototypes for SharedMemory                             */
/*---------------------------------------------------------------------------*/
tSharedMemType *CreateOrOpenMapFile(HANDLE *hMemMapFile, char *mapName, int create, int size, int *alreadyExist);		// 구조체의 pointer를 가지고, 각 요소에 접근하겠다는 심산.
int CloseMapping(HANDLE *hMemMapFile, void **memPtr);
int SetupServerOrClient(void);
int CleanupServerOrClient(void);
int DownloadSHMToTcs(void);
int UploadTcsToSHM();

// From now, belonging in TCS

/*---------------------------------------------------------------------------*/
/* Macros for TCS                                                            */
/*---------------------------------------------------------------------------*/
int CVICALLBACK THREAD_PARKING(void *data);
int CVICALLBACK THREAD_INIT	  (void *data);
int CVICALLBACK THREAD_DISP_TEL_STATUS(void *data);

/*---------------------------------------------------------------------------*/
/* Static Variables for TCS                                                  */
/*---------------------------------------------------------------------------*/
h_m_s TARGET_RA_2000,  CURRENT_RA, CURRENT_LAST ;
d_m_s TARGET_DEC_2000, CURRENT_DEC ;
char  TARGET_NAME[100] ;
static h_m_s HA;
static d_m_s DEC_APP, ALT, AZI;
double ALT_MIN = 10;
static double jd_ut1;
static int tel_handle, limit_handle=-1 ;
static int CHECK_PARKINGA, CHECK_PARKINGB ;
static int CHECK_INITA, CHECK_INITB ;
static int CW_LIMIT[4], CCW_LIMIT[4], HOME_LIMIT[4];
static int IS_TRACK=0;
float off_ra=0., off_dec=0., off_azi=0., off_alt=0.;
static char buffer[200] ;
char GBUFF[500] ;

/*---------------------------------------------------------------------------*/
/*  Internal functions prototypes for TCS                                    */
/*---------------------------------------------------------------------------*/
static int start_init(void);

int start_inittcs(void) ;
int start_parking(void) ;
int goto_target(void);

static int check_slew(void);
static int check_parking(void);
//static int check_slew_track_altazi(void);			   // move to tel_sub2.c file
//static int check_slew_track_radec(void);			   // move to tel_sub2.c file
int check_init(void);

int SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000);
int STOP_TELESCOPE(void) ;
	   
void display_message(char buff[]) ;
void display_message2(char buff[]) ;

int read_input_files(void) ;
int detect_module(void) ;
int set_axis_para(long laxis) ;
int read_nodeno(void);

static int display_tel_status(void);
																			  
/*---------------------------------------------------------------------------*/
/* Main	Function															 */
/*---------------------------------------------------------------------------*/
DWORD Code = -1 ;

int main (int argc, char *argv[])
{
// initialize parameters
	TELESCOPE_TYPE = _ALT_AZI ;
	TELESCOPE_MODE = TEL_MODE_STOP ;
	
	if (InitCVIRTE (0, argv, 0) == 0) 		/* Initialize CVI libraries */
		return -1;							/* out of memory */

	USEPRELOAD=0;
	
// Load the control panel
	if ((tel_handle = LoadPanel (0, "shmtcs.uir", PANEL)) < 0)
	{
		MessagePopup("TCS Error", "Failed to load UIR panel.");
		goto Done;
	}
// Update UIR panel title
    sprintf(msgBuffer, "SHAREMEM TCS %s (Process ID:%8x)", 
        "TCS", 
        (int)GetCurrentProcessId() );									 // Getting a ProcessId
    SetPanelAttribute (tel_handle, ATTR_TITLE, msgBuffer);	
// Display UIR    
	DisplayPanel (tel_handle);
// Read input files
	read_input_files() ;
// open JPL ephmeris calculation
	init_jpl("d:\\TCS_GRIDA_20180403\\cal_lib\\Jpleph.dos");
// Set up the timer in UIR	
	turn_on_timer(tel_handle, PANEL_TIMER);
	set_timer_interval(tel_handle, PANEL_TIMER, SERVO_UPDATE_SEC);

// Claim this app is client
	bServerApp = 0;
// Set up SharedMemory Environment & lock, initializes it
    SetupServerOrClient() ;
// Run TCS program
	RunUserInterface ();
// Cleanup for SharedMemory Environment & Handle
    CleanupServerOrClient();
    CloseMapping(&hMemMapFile, (void **)(&sharedMemory));
    CmtDiscardLock(lock);
    DiscardPanel(tel_handle);
    
Done:
    return 0;	
	
}

/*---------------------------------------------------------------------------*/
/* Closing Function															 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK Panel_Proc (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
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

/*---------------------------------------------------------------------------*/
/* Callback Function of APPLY												 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK APPLY (int panel, int control, int event, 
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			if (control == PANEL_PARK) start_parking();
			if (control == PANEL_INIT) start_init();
			if (control == PANEL_STOP)
			{
				if (TELESCOPE_MODE == TEL_MODE_PARK)
				{
					AxmHomeSetResult(0, HOME_SUCCESS); AxmHomeSetResult(1, HOME_SUCCESS);
				}
				STOP_TELESCOPE() ;
			}
			
			else if (control == PANEL_GOTO) goto_target(); 
			
			else if (control == PANEL_TCSINIT) start_inittcs(); 
			
			else if (control == PANEL_CONST_WEST)
			{
				STOP_TELESCOPE();
				TELESCOPE_MODE = TEL_MODE_GUIDE;
				go_const('A', PARK_FREQ[0]); 
				SetCtrlVal(tel_handle, PANEL_TEXTBOX, "Go to the WEST \n"); 	 // display_message("Go to the WEST \n"); 
			}
			else if (control == PANEL_CONST_EAST) 
			{
				STOP_TELESCOPE();
				TELESCOPE_MODE = TEL_MODE_GUIDE;
				go_const('A', -PARK_FREQ[0]); 
				SetCtrlVal(tel_handle, PANEL_TEXTBOX, "Go to the EAST \n"); 
			} 
			else if (control == PANEL_CONST_UP)   
			{
				STOP_TELESCOPE();
				TELESCOPE_MODE = TEL_MODE_GUIDE;
				go_const('B', PARK_FREQ[1]);
				SetCtrlVal(tel_handle, PANEL_TEXTBOX, "Go to the UP \n");
			} 
			else if (control == PANEL_CONST_DOWN) 
			{
				STOP_TELESCOPE();
				TELESCOPE_MODE = TEL_MODE_GUIDE;
				go_const('B', -PARK_FREQ[1]);
				SetCtrlVal(tel_handle, PANEL_TEXTBOX, "Go to the DOWN \n"); 
			}
			
		break ; 
	}
	
	return 0;
}
/*
int CVICALLBACK OpenLimit (int panel, int control, int event, 
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(limit_handle>0) return 0;
			if ((limit_handle = LoadPanel(0, "initiation.uir", PANEL2)) < 0) return -1;

			DisplayPanel(limit_handle) ;
			display_message("Limit Window is opend. \n") ;
			
			RunUserInterface() ;
			DiscardPanel(limit_handle) ;
			limit_handle = -1 ;			 // limit_handle을 비활성화 시키는 작업. THREAD_LIMIT_STATUS에서는 계속 단추값을 넣으려고 하는데 핸들은 이미 사라졌기 때문. 인위적으로 사라짐을 -1로 표시해 주어야 함.
			
			break ;
	}
	return 0 ;
}
*/
/////////////////////////////////////////////////////////////////////
int CVICALLBACK CB_TIMER(int panel, int control, int event, 
					   void *callbackData, int eventData1, int eventData2)
{
//	int is_track=0;
	float off_zero = 0.0;
	
	switch (event) 
	{
		case EVENT_COMMIT:
			if(control == PANEL_GOOFF)
			{
				GetCtrlVal(tel_handle, PANEL_OFF_RA, &off_ra);  // deg
				GetCtrlVal(tel_handle, PANEL_OFF_DEC, &off_dec);  // deg
			
				off_ra = deg_to_rad(off_ra);		  // radian
				off_dec = deg_to_rad(off_dec);		  // radian
				
				GetCtrlVal(tel_handle, PANEL_OFF_AZI, &off_azi);  // deg
				GetCtrlVal(tel_handle, PANEL_OFF_ALT, &off_alt);  // deg
				
				off_azi = deg_to_rad(off_azi);		  // radian
				off_alt = deg_to_rad(off_alt);		  // radian
				
			}
			if(control == PANEL_OFF_CLEAR)
			{
				SetCtrlVal(tel_handle, PANEL_OFF_RA, off_zero);  // deg
				SetCtrlVal(tel_handle, PANEL_OFF_DEC, off_zero);  // deg
				SetCtrlVal(tel_handle, PANEL_OFF_AZI, off_zero);  // deg
				SetCtrlVal(tel_handle, PANEL_OFF_ALT, off_zero);  // deg
				
//				off_ra = off_zero ; off_dec = off_zero ; off_azi = off_zero ; off_alt = off_zero ; 
				
			}
		break ;
		
		case EVENT_TIMER_TICK:
//	calculate LAST
			 jd_ut1 = get_jdutc_clock() + DUT1_DAY;
			 SetCtrlVal(tel_handle, PANEL_JDATE, jd_ut1);
			 CURRENT_LAST.rad = jdtolast(jd_ut1, SITE_LONGI.rad);
			 rad_to_hms(&CURRENT_LAST);

			 if(TELESCOPE_MODE==TEL_MODE_SLEW || TELESCOPE_MODE==TEL_MODE_TRACK) 
			 {   
//	sscanf(buff, "%d %d %lf", &ra.h, &ra.m, &ra.s);azioff
				 
				 IS_TRACK=slew_track_radec(TARGET_RA_2000,TARGET_DEC_2000);
				 
				 if(IS_TRACK>=1) {TELESCOPE_MODE=TEL_MODE_SLEW; sprintf(buffer, "TELESCOPE is SLEWING %d \n", IS_TRACK); display_message(buffer);}
				 else 			{TELESCOPE_MODE=TEL_MODE_TRACK;  sprintf(buffer, "TELESCOPE is TRACKING  %d \n", IS_TRACK); display_message(buffer);}
			 }
			 
//			 Delay(0.05);
			
			 display_tel_status();
			
		 break ; 
	}
	
	return 0;
}
/////////////////////////////////////////////////////////////////////
static int display_tel_status()
////////////////////////////////////////////////////////////////////
{
DWORD mode;
int dir;
long counts, encoder;
double velocity;

//	if (limit_handle < 0) return -1;
//	else{
//	Delay(0.01);
//	READ LIMIT STATUS ----------------------------------------------		
	read_cw_limit('A', &CW_LIMIT[0]);
	read_ccw_limit('A', &CCW_LIMIT[0]);
	read_home_limit('A', &HOME_LIMIT[0]);
	SetCtrlVal(tel_handle, PANEL_AZI_CW, CW_LIMIT[0]);
	SetCtrlVal(tel_handle, PANEL_AZI_CCW, CCW_LIMIT[0]);
	SetCtrlVal(tel_handle, PANEL_AZI_HOME, HOME_LIMIT[0]);
		
	read_cw_limit('B', &CW_LIMIT[1]);
	read_ccw_limit('B', &CCW_LIMIT[1]);
	read_home_limit('B', &HOME_LIMIT[1]);
	SetCtrlVal(tel_handle, PANEL_ALT_CW, CW_LIMIT[1]);
	SetCtrlVal(tel_handle, PANEL_ALT_CCW, CCW_LIMIT[1]);
	SetCtrlVal(tel_handle, PANEL_ALT_HOME, HOME_LIMIT[1]);

//	READ DIRECTION AND SLEW MODE
	read_slew_mode('A', &mode);
	SetCtrlVal(tel_handle, PANEL_AZI_MODE, mode);
	read_dir('A', &dir);
	SetCtrlVal(tel_handle, PANEL_AZI_DIR, dir);
		
	read_slew_mode('B', &mode);
	SetCtrlVal(tel_handle, PANEL_ALT_MODE, mode);
	read_dir('B', &dir);
	SetCtrlVal(tel_handle, PANEL_ALT_DIR, dir);
		
//	READ COUNT/ENCODER/VELOCITY STATUS --------------------------------------------
	read_counter('A', &counts);
	read_encoder('A', &encoder);
	read_speed('A', &velocity);
	if(dir==0) velocity = -velocity;
		
	SetCtrlVal(tel_handle, PANEL_AZI_COUNTER, counts);
	SetCtrlVal(tel_handle, PANEL_AZI_ENCODER, encoder);
	SetCtrlVal(tel_handle, PANEL_AZI_VELOCITY, velocity);
	
	read_counter('B', &counts);
	read_encoder('B', &encoder);
	read_speed('B', &velocity);
	if(dir==0) velocity = -velocity;
		
	SetCtrlVal(tel_handle, PANEL_ALT_COUNTER, counts);
	SetCtrlVal(tel_handle, PANEL_ALT_ENCODER, encoder);
	SetCtrlVal(tel_handle, PANEL_ALT_VELOCITY, velocity);
//	}
	return 0;
}
/////////////////////////////////////////////////////////////////////
static int start_init()
/////////////////////////////////////////////////////////////////////
{
	int state1, state2;
	
	read_home_limit('A', &state1);
	read_home_limit('B', &state2);
	if(state1 != 1 || state2 != 1)
	{
		Beep(); MessagePopup("", "initialize after Parking Tel.");
		return -1;
	}
	
	SetCtrlVal(tel_handle, PANEL_TEXTBOX, "Going On Initializing \n");
	
	go_const('A', INIT_FREQ[0]);
	go_const('B', INIT_FREQ[1]);
	
	TELESCOPE_MODE = TEL_MODE_INIT;
	CHECK_INITA = CHECK_INITB = 1;
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, THREAD_INIT, NULL, NULL);
	
	return 0;
}
/////////////////////////////////////////////////////////////////////
int CVICALLBACK THREAD_INIT(void *data)
////////////////////////////////////////////////////////////////////
{
	for(;;)
	{
//		Delay(0.01);
		if(TELESCOPE_MODE==TEL_MODE_STOP) break;
		
		if(CHECK_INITA==1 || CHECK_INITB==1) check_init();
		if(CHECK_INITA==0 && CHECK_INITB==0) 
		{
			TELESCOPE_MODE==TEL_MODE_STOP; 
			sprintf(buffer, "Initialzing completed!! \n");
			display_message(buffer);
			break;
		}
	}
	
	return 0;
}
/////////////////////////////////////////////////////////////////////
int check_init()
/////////////////////////////////////////////////////////////////////
{
	int state0, state1;
	
	if(CHECK_INITA==1)
	{
		read_home_limit('A',&state0);
		if(state0!=SWSTATUS[0].home) {set_counter('A', 10000); set_encoder('A', INIT_COUNT[0]); CHECK_INITA=0; motor_stop('A');}
	}
	if(CHECK_INITB==1)
	{
		read_home_limit('B',&state1);
		if(state1!=SWSTATUS[1].home) {set_counter('B', 10000); set_encoder('B', INIT_COUNT[1]); CHECK_INITB=0; motor_stop('B');}
	}
	
	return 0;
}
/////////////////////////////////////////////////////////////////////
void display_message(char buff[])
/////////////////////////////////////////////////////////////////////
{
	SetCtrlVal(tel_handle, PANEL_TEXTBOX, buff);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void display_message2(char buff[])
/////////////////////////////////////////////////////////////////////
{
	SetCtrlVal(tel_handle, PANEL_TEXTBOX_2, buff);
}
/////////////////////////////////////////////////////////////////////
int goto_target()
/////////////////////////////////////////////////////////////////////
{
char buff[300];
h_m_s ra;
d_m_s dec;
	
	GetCtrlVal(tel_handle, PANEL_RA_INPUT, buff);
	sscanf(buff, "%d %d %lf", &ra.h, &ra.m, &ra.s);
	
	GetCtrlVal(tel_handle, PANEL_DEC_INPUT, buff);
	sscanf(buff, "%d %d %lf", &dec.d, &dec.m, &dec.s);
	
	hms_to_rad(&ra);
	if(dec.d > 0) dms_to_rad(&dec);
	else
	{
		dec.d = -dec.d ;
		dms_to_rad(&dec);
		dec.rad = -dec.rad; rad_to_dms(&dec);
	}
	
	sprintf(buffer, "ra: %d %d %lf\n", ra.h, ra.m, ra.s) ;
	display_message(buffer) ;
	sprintf(buffer, "dec: %d %d %lf\n", dec.d, dec.m, dec.s) ;
	display_message(buffer) ;
	
//	return 0;
	return SLEW_AND_TRACK(ra, dec);
}
/////////////////////////////////////////////////////////////////////
int SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000)
//------------------------------------------------------------------------
{
//	int is_track;
 //double error_deg;
 
    if(TELESCOPE_MODE != TEL_MODE_STOP) STOP_TELESCOPE();
/*	
    if(GUIDE_MODE != GUIDE_MODE_OFF)
      {
       MessagePopup("","Stop GUIDE/Manual Key");
       return -1;
      }
*/
//	preload_off();
    
    IS_TRACK = slew_track_radec(ra_2000, dec_2000);
	
	sprintf(buffer, "ra_2000: %d %d %lf %lf \n", ra_2000.h, ra_2000.m, ra_2000.s, ra_2000.rh) ;
	display_message(buffer) ;
	sprintf(buffer, "dec_2000: %d %d %lf %lf \n", dec_2000.d, dec_2000.m, dec_2000.s, dec_2000.rd) ;
	display_message(buffer) ;

	// nothing //
//    SLEW_MODE[0] = SLEW_MODE[1] = _SLEW_MODE_ON;
    // nothing //
//    if (DER_TRACKING==DER_TRACKING_ON) SLEW_MODE[2] = _SLEW_MODE_ON;
    // nothing //
	
    if(IS_TRACK>1) TELESCOPE_MODE = TEL_MODE_TRACK;
	else TELESCOPE_MODE=TEL_MODE_SLEW;
    
    TARGET_RA_2000.rad  =  ra_2000.rad; rad_to_hms( &TARGET_RA_2000);
    TARGET_DEC_2000.rad = dec_2000.rad; rad_to_dms(&TARGET_DEC_2000);
    
    SetCtrlVal(tel_handle, PANEL_TEXTBOX,"Going to the Target \n");

// Test original coordinate
//	sprintf(buffer, "original RA: %f, original DEC: %f \n", TARGET_RA_2000.rad, TARGET_DEC_2000.rad);
//	SetCtrlVal(tel_handle, PANEL_TEXTBOX_2, buffer);
	
//    display_input_window(ra_2000,dec_2000,TARGET_NAME);
    
    return 0;
}
/////////////////////////////////////////////////////////////////////
int start_inittcs()
/////////////////////////////////////////////////////////////////////
{
	if(Code!=AXT_RT_SUCCESS) Code = AxlOpen();
	
	if (Code == AXT_RT_SUCCESS) 
		{
			display_message("success to initialize the library. \n");
			detect_module();
			
			motor_stop('A') ;		// st4s_lib.c
			motor_stop('B') ;		// st4s_lib.c
			
			set_axis_para(0) ;
			set_axis_para(1) ;		
			
			set_home_search_para() ;	// st4s_lib.c
		}
	else
		{
			display_message("failed to initialize the library. \n");
			sprintf(buffer, "AxlOpen() : ERROR CODE %d\n", Code);
			display_message(buffer);
			
			return -1;
		}
		
	return 0;
}
/////////////////////////////////////////////////////////////////////
int start_parking()
/////////////////////////////////////////////////////////////////////
{
	long count ;
	int state1, state2 ; 
//  int dir
	int dir_ra, dir_dec ;
	
	if (TELESCOPE_MODE != TEL_MODE_STOP) STOP_TELESCOPE() ;
	
//	check telescope is already at home position
	read_home_limit('A', &state1) ;			// active(state1 = 1) or inactive(state1 = 0)
	read_home_limit('B', &state2) ;
	
	if (DER_TRACKING == DER_TRACKING_OFF)
	{
		if (state1 == 1 && state2 == 1) return 0 ;
	}

//  if( (park_handle = LoadPanel(tel_handle,UIR_FILE_NAME, PARK)) < 0) return -1;
//	InstallPopup(park_handle);
//  SetPanelAttribute (park_handle, ATTR_TITLE, "망원경 파킹 위치로 이동");
	
	read_counter('A', &count) ;
	if (count >= 0)  dir_ra = 0 ;		  // count 값에 0을 포함시킨 이유는, WEST LIMIT(CW LIMIT)에 걸렸을 때, COUNT를 계속 0으로 읽기 때문에 망원경을 동쪽으로 옮기기 위한 꼼수
	else 			dir_ra = 1 ;
	
	read_counter('B', &count) ;			  // AxmStatusGetCmdPos : 'A' 축의 명령 위치를 count로 반환. count의 부호에 따라 방향 결정
	if (count > 0)  dir_dec = 0 ;		  
	else			dir_dec = 1 ;

	
	sprintf(buffer, "Going on Parking(Home Position)\n");
	display_message(buffer);

//  SetCtrlVal(tel_handle,TEL_TEXTMSG_7,"파킹 진행중");
//  GetCtrlVal(park_handle,PARK_DIR_RA,&dir);	
	
	if (dir_ra == 1) go_const('A', PARK_FREQ[0]) ;
	else			 go_const('A', -PARK_FREQ[0]) ;
	
	if (dir_dec == 1) go_const('B', PARK_FREQ[1]) ;
	else			  go_const('B', -PARK_FREQ[1]) ;
	
	TELESCOPE_MODE = TEL_MODE_PARK ;
	CHECK_PARKINGA = CHECK_PARKINGB = 1 ;
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,THREAD_PARKING,NULL,NULL);
	
	return 0 ;
}
//////////////////////////////////////////////////////////////////////
static int check_slew()
//////////////////////////////////////////////////////////////////////
{
	int state1, state2;
	
	read_slew_mode('A', &state1);
	read_slew_mode('B', &state2);
	if(state1==0 && state2==0) TELESCOPE_MODE = TEL_MODE_STOP;
	return 0;
}
//////////////////////////////////////////////////////////////////////
int CVICALLBACK THREAD_PARKING(void *data)
//////////////////////////////////////////////////////////////////////
{
//	int i, state;
//	char ccc[100];
	for(;;)
	{
		Delay(0.1);
		if(TELESCOPE_MODE==TEL_MODE_STOP) break;
		
		if(CHECK_PARKINGA==1 || CHECK_PARKINGB==1) check_parking();
		if(CHECK_PARKINGA==0 && CHECK_PARKINGB==0) 
		{
			TELESCOPE_MODE=TEL_MODE_STOP;
			sprintf(buffer, "Parking completed!!\n");
			display_message(buffer);
			break;
		}
	}
	
	return 0;
}
//////////////////////////////////////////////////////////////////////
static int check_parking()
//////////////////////////////////////////////////////////////////////
{
	DWORD result1, result2;
	int state0, state1;
	
	if(CHECK_PARKINGA==1)
	{
		read_home_limit('A', &state0);
		if(state0==SWSTATUS[0].home) {CHECK_PARKINGA=0; motor_stop('A');}
	}
	if(CHECK_PARKINGB==1)
	{
		read_home_limit('B', &state1);
		if(state1==SWSTATUS[1].home) {CHECK_PARKINGB=0; motor_stop('B');}
	}
	
	return 0;
}
/////////////////////////////////////////////////////////////////////
int detect_module()
/////////////////////////////////////////////////////////////////////
{
	DWORD dwStatus ;
	DWORD Code ; 
	long lpAxisCount, lpModuleCount ;
//	long lNodeNo, lModulePos ;
	long lModulePos ;
	DWORD dwModuleID ;
	long lAxisNo, lNodeNo ;
	long lFirstAxisNo ;
	long lpCounts ;

// checking the existence of the motion modules 
	Code = AxmInfoIsMotionModule(&dwStatus) ;
	if (Code != AXT_RT_SUCCESS)
		{
			sprintf(buffer, "AxmInfoIsMotionModule() : ERROR ( Return FALSE ) code %d\n", Code) ;
			display_message(buffer) ;
			return -1 ;
		}
	if (dwStatus != STATUS_EXIST)
		{
			sprintf(buffer, "AxmInfoIsMotionModule() : ERROR ( NOT STATUS_EXIST ) code %d\n", Code) ;
			display_message(buffer) ;
			return -2 ;
		}
	
	AxmInfoGetAxisCount(&lpAxisCount) ;
	sprintf(buffer, "There are Motion %d Modules(Axies).\n", lpAxisCount) ;
	display_message(buffer) ;

// check the number of axies attached on the system
	lAxisNo = 0 ;
	AxmInfoGetAxis(lAxisNo, &lNodeNo, &lModulePos, &dwModuleID) ;
	sprintf(buffer, "Axis number: %d, Node number: %x, the position of module: %x, ID of module: %x\n", lAxisNo, lNodeNo, lModulePos, dwModuleID) ;
	display_message(buffer) ;
	
	lAxisNo = 1 ;
	AxmInfoGetAxis(lAxisNo, &lNodeNo, &lModulePos, &dwModuleID) ;
	sprintf(buffer, "Axis number: %d, Node number: %x, the position of module: %x, ID of module: %x\n", lAxisNo, lNodeNo, lModulePos, dwModuleID) ;
	display_message(buffer) ;
		
	lAxisNo = 2 ;
	AxmInfoGetAxis(lAxisNo, &lNodeNo, &lModulePos, &dwModuleID) ;
	sprintf(buffer, "Axis number: %d, Node number: %x, the position of module: %x, ID of module: %x\n", lAxisNo, lNodeNo, lModulePos, dwModuleID) ;
	display_message(buffer) ;
	
	lAxisNo = 3 ;
	AxmInfoGetAxis(lAxisNo, &lNodeNo, &lModulePos, &dwModuleID) ;
	sprintf(buffer, "Axis number: %d, Node number: %x, the position of module: %x, ID of module: %x\n", lAxisNo, lNodeNo, lModulePos, dwModuleID) ;
	display_message(buffer) ;
	
	lAxisNo = 4 ;
	AxmInfoGetAxis(lAxisNo, &lNodeNo, &lModulePos, &dwModuleID) ;
	sprintf(buffer, "Axis number: %d, Node number: %x, the position of module: %x, ID of module: %x\n", lAxisNo, lNodeNo, lModulePos, dwModuleID) ;
	display_message(buffer) ;
	
	lAxisNo = 5 ;
	AxmInfoGetAxis(lAxisNo, &lNodeNo, &lModulePos, &dwModuleID) ;
	sprintf(buffer, "Axis number: %d, Node number: %x, the position of module: %x, ID of module: %x\n", lAxisNo, lNodeNo, lModulePos, dwModuleID) ;
	display_message(buffer) ;
	

// check the first axis number at node number of 0, module position(number) of 1 or 2
	lNodeNo = 2 ;
	lModulePos = 1 ;
	AxmInfoGetFirstAxisNo(lNodeNo, lModulePos, &lFirstAxisNo) ;
	sprintf(buffer, "the number of first axis at the node number %d, the %d module : %d\n", lNodeNo, lModulePos, lFirstAxisNo) ;
	display_message(buffer) ;

	lNodeNo = 1 ; 
	lModulePos = 1 ;
	AxmInfoGetFirstAxisNo(lNodeNo, lModulePos, &lFirstAxisNo) ;
	sprintf(buffer, "the number of first axis at the node number %d, the %d module : %d\n", lNodeNo, lModulePos, lFirstAxisNo) ;
	display_message(buffer) ;
	
// check the number of modules of DIO I/O
	Code = AxdInfoIsDIOModule(&dwStatus) ;
	if (Code != AXT_RT_SUCCESS)
		{
			sprintf(buffer, "AxmInfoIsMotionModule() : ERROR (Return FALSE) code %d\n", Code) ;
			display_message(buffer) ;
			return -1 ;
		}
		
	if (dwStatus == STATUS_EXIST)
		{
			AxdInfoGetModuleCount(&lpModuleCount) ;
			sprintf(buffer, "the count number of DIO module %d \n", lpModuleCount) ;
			display_message(buffer) ;
			
			AxdInfoGetInputCount(0, &lpCounts) ;
			sprintf(buffer, "the count number of DIO input %d\n", lpCounts) ;
			display_message(buffer) ;
			
			AxdInfoGetOutputCount(0, &lpCounts) ;
			sprintf(buffer, "the count number of DIO output %d\n", lpCounts) ;
			display_message(buffer) ;
		}
	else
		{
			sprintf(buffer, "no DIO module exists\n") ;
			display_message(buffer) ;
		}
		
	return 0 ;
}
 /////////////////////////////////////////////////////////////////////
int set_axis_para(long laxis)
/////////////////////////////////////////////////////////////////////
{
	DWORD dwStatus, mode ;
	int iaxis ;
	iaxis = laxis ;
	laxis = NODENO*2 + laxis ;
	
	mode = AxmInfoIsMotionModule(&dwStatus) ;
	if (mode != AXT_RT_SUCCESS) return -1 ;
	if (dwStatus != STATUS_EXIST) return -2 ;
	
	//  펄스 출력모드 설정 : 2펄스 모드
//    OneHighLowHigh          = 0x0   // 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
//    OneHighHighLow          = 0x1   // 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
//    OneLowLowHigh           = 0x2   // 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
//    OneLowHighLow           = 0x3   // 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
//    TwoCcwCwHigh            = 0x4   // 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High     
//    TwoCcwCwLow             = 0x5   // 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low     
//    TwoCwCcwHigh            = 0x6   // 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
//    TwoCwCcwLow             = 0x7   // 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
//    TwoPhase                = 0x8   // 2상(90' 위상차),  PULSE lead DIR(CW: 정방향), PULSE lag DIR(CCW:역방향)
//    TwoPhaseReverse         = 0x9   // 2상(90' 위상차),  PULSE lead DIR(CCW: 정방향), PULSE lag DIR(CW:역방향)
	AxmMotSetPulseOutMethod(laxis, OneHighLowHigh) ;
	
// Home signal level
	AxmHomeSetSignalLevel(laxis, SWSTATUS[iaxis].home) ;
// Inposition use or not
	AxmSignalSetInpos(laxis, UNUSED) ;
// Alarm use or not
	AxmSignalSetServoAlarm(laxis, UNUSED) ;
// Servo On signal setting to HIGH
	AxmSignalSetServoOnLevel(laxis, HIGH) ;
// set_axis_relative(laxis) ;
	AxmMotSetAbsRelMode(laxis, 1) ;
// Distance per 1 Pulse
	AxmMotSetMoveUnitPerPulse(laxis, 1, 1) ;
	
// ac/deceleration profile is tripezium
	AxmMotSetProfileMode(laxis, SYM_TRAPEZOIDE_MODE) ;
// maximum speed(지정축 구동함수에서의 최고속도)
	AxmMotSetMaxVel(laxis, VPROFILE[iaxis].fmax*1.1) ;  
// start/stop/speed(지정축에 초기속도 설정)
	AxmMotSetMinVel(laxis, VPROFILE[iaxis].fmin) ;
// 지정축에 오버라이드하기전 오버라이드할 최고 속도 설정
	AxmOverrideSetMaxVel(laxis, VPROFILE[iaxis].fmax*1.1) ;
// Pulse counter clear (지정축의 command 위치를 원점으로 한다)
	AxmStatusSetCmdPos(laxis, 0) ;
// Encoder counter clear(지정축의 actual 위치를 원점으로 한다)
	AxmStatusSetActPos(laxis, 0.0) ;
// encoder input method setting to 4체배
	AxmMotSetEncInputMethod(laxis, ObverseSqr4Mode) ;
// CW, CCW level
	AxmSignalSetLimit(laxis, EMERGENCY_STOP, SWSTATUS[iaxis].cw, SWSTATUS[iaxis].ccw) ;
	
	return 0 ;
}
/////////////////////////////////////////////////////////////////////
int read_input_files(void)
/////////////////////////////////////////////////////////////////////
{
	read_telescope_parameter() ;
	read_tcs_parameter() ;
	init_telescope_parameter() ;
	
	sprintf(GBUFF, "SLEW_FREQ[0] = %10.3lf\n", SLEW_FREQ[0]) ; display_message(GBUFF) ;
	sprintf(GBUFF, "SLEW_FREQ[1] = %10.3lf\n", SLEW_FREQ[1]) ; display_message(GBUFF) ;
	
	read_nodeno() ;
	
	return 0 ;
}
/////////////////////////////////////////////////////////////////////
int read_nodeno()
/////////////////////////////////////////////////////////////////////
{
	FILE *fin ;
	
	if ((fin = fopen("nodeno.inp", "r")) == NULL)
	{MessagePopup("", "CANNOT OPEN NODENO.INP") ; return -1 ;}
	
	fgets(GBUFF, 200, fin) ;
	sscanf(GBUFF, "%d", &NODENO) ;

	fgets(GBUFF, 200, fin) ;
	sscanf(GBUFF, "%lf %lf %lf %lf", &ENCTOSTEP_RA[0], &ENCTOSTEP_RA[1], &ENCTOSTEP_RA[2], &ENCTOSTEP_RA[3]) ;
	
	fgets(GBUFF, 200, fin) ;
	sscanf(GBUFF, "%lf %lf %lf %lf", &ENCTOSTEP_DE[0], &ENCTOSTEP_DE[1], &ENCTOSTEP_DE[2], &ENCTOSTEP_DE[3]) ;
	
	fclose(fin) ;
	
	return 0 ;
}
/////////////////////////////////////////////////////////////////////
int STOP_TELESCOPE(void)
/////////////////////////////////////////////////////////////////////
{
	
	  motor_stop('A') ;
	  motor_stop('B') ;
	  TELESCOPE_MODE = TEL_MODE_STOP ;
	  sprintf(buffer, "Telesope stopped.\n");
	  display_message(buffer);
	  return 0 ;
}
/////////////////////////////////////////////////////////////////////
