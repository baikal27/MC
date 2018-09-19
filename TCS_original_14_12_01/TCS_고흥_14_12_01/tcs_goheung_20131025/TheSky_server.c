#include <utility.h>
#include <ddesupp.h>    
#include <userint.h>
#include <ansi_c.h>
#include "TheSky_dde.h"
#include "type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "gv_tel.h"

extern int  SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000);
extern int  STOP_TELESCOPE(void);
//extern void put_message(char message[]);
extern void display_message(char message[]);

void ActivateServer  (void);
void DeactivateServer(void);
static int CVICALLBACK SrvCallback (unsigned int handle, char *topicName, char *itemName,
           int xType, int dataFmt, int dataSize, void *dataPtr, void *callbackData);

static void execute_command    (char command[]);
static void send_data_to_client(char data_name[]);
static void receive_data(char data_name[], void *dataPtr);
static void calc_ra_dec_2000(double ra, double dec, double *ra_2000, double *dec_2000);

static unsigned convID;
static char buff[100];

static double  TWOPI = 3.14159265358979323846*2.0;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void ActivateServer (void)
//-----------------------------------------------------------------------------
{
    if(RegisterDDEServer (NAME,SrvCallback,0)==0)
       MessagePopup("The Sky","connected");
    else
       MessagePopup("The Sky","connection failure");
}
//-----------------------------------------------------------------------------
void DeactivateServer (void)
//-----------------------------------------------------------------------------
{
    UnregisterDDEServer(NAME);  
    MessagePopup("The Sky","disconnected");
    
}
//-----------------------------------------------------------------------------
static int CVICALLBACK SrvCallback (unsigned int handle, char *topicName, char *itemName,
           int xType, int dataFmt, int dataSize, void *dataPtr, void *callbackData)
//-----------------------------------------------------------------------------
{
 double value;
 
    if(handle!=convID) return FALSE;

//  determines DDE message type
    switch (xType)
        {
        case DDE_CONNECT:
            convID = handle;  // assigns new conversation handle to convID
//          put_message("TheSky is connected  .....\n");
            display_message("TheSky is connected  .....\n");
            break;

        case DDE_DISCONNECT:
            convID = -1;      // eliminates old conversation handle
//          put_message("TheSky is disconnected  .....\n");
            display_message("TheSky is disconnected  .....\n");
            break;

        case DDE_ADVISELOOP:
            break;

        case DDE_DATAREADY:
            receive_data(itemName, dataPtr);
            break;

        case DDE_REQUESTDATA:
            send_data_to_client(itemName);
            break;

        case DDE_ADVISESTOP:
            break;

        case DDE_EXECUTE:
             execute_command(itemName);
            break;

        default:
            return FALSE;       /* unrecognized client message */
        }

    return TRUE;                /* TRUE --> server processed message successfully */
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
static void execute_command(char command[])
//-----------------------------------------------------------------------------
{
    if(strcmp(command, DDE_STOP) == 0)
     {
      sprintf(buff,"STOP command is received\n");
//    put_message(buff);
      display_message(buff);
	  STOP_TELESCOPE();
     }
}    
//-----------------------------------------------------------------------------
static void send_data_to_client(char data_name[])
//-----------------------------------------------------------------------------
{
 double return_data[10];
 int status;

    if     (strcmp(data_name,DDE_GET_RADEC) == 0)
     {
//    sprintf(buff,"GET position from TheSky\n");
//    put_message(buff);
     
      return_data[0] = CURRENT_RA.rh;
      return_data[1] = CURRENT_DEC.rd;
      ServerDDEWrite(convID,data_name,CF_TEXT,&return_data[0],sizeof(return_data),DDE_TIMEOUT);
     }
     
    else if(strcmp(data_name,DDE_STATUS) == 0)
     {
      sprintf(buff,"SLEW STATUS from TheSky\n");
      display_message(buff);
     
      if(TELESCOPE_MODE==TEL_MODE_SLEW) status = 0;
      else                              status = 1;
     
      ServerDDEWrite(convID,data_name,CF_TEXT,&status,sizeof(status),DDE_TIMEOUT);
     }
}    
//-----------------------------------------------------------------------------
static void receive_data(char data_name[], void *dataPtr)
//-----------------------------------------------------------------------------
{
 h_m_s ra, ra_2000;
 d_m_s dec, dec_2000;
 int focus_speed,focus_dir;
 
    if(strcmp(data_name,DDE_GOTO_RADEC) == 0) 
     {
      ra.rh  = * (double *) dataPtr;
      dec.rd = *((double *) dataPtr+1);
      
      ra.rad  = deg_to_rad(ra.rh*15.0);
      rad_to_hms(&ra);
      dec.rad = deg_to_rad(dec.rd);
      rad_to_dms(&dec);
      
      calc_ra_dec_2000(ra.rad, dec.rad, &TARGET_RA_2000.rad, &TARGET_DEC_2000.rad);
      rad_to_hms(&TARGET_RA_2000 );
      rad_to_dms(&TARGET_DEC_2000);

      strcpy(TARGET_NAME,"Object from TheSky");
      SLEW_AND_TRACK(TARGET_RA_2000, TARGET_DEC_2000);
      
      sprintf(buff,"GOTO from TheSky\n");
//    put_message(buff);
      display_message(buff);
     }
     
    else if(strcmp(data_name,DDE_FOCUS) == 0) 
     {
      focus_speed = * (int *) dataPtr;
      focus_dir   = *((int *) dataPtr+1);
      
      sprintf(buff,"FOCUS from TheSky : speed %d  dir %d",focus_speed,focus_dir);
//    put_message(buff);
      display_message(buff);
     }
     
      
//    else if(strcmp(data_name,DDE_SET_RADEC) == 0) 
//     {
//      SetCtrlVal(server_panel,SERVER_DATAX, * (double *) dataPtr);
//      SetCtrlVal(server_panel,SERVER_DATAY, *((double *) dataPtr+1));
//     }
}
//-----------------------------------------------------------------------------
static void calc_ra_dec_2000(double ra, double dec, double *ra_2000, double *dec_2000)
//  Convert the RA and Dec. passed from TheSky to JD2000 position.
//  The RA and Dec. passed from TheSky is not corrected for atmospheric refraction.
//                                         Created by Han Inwoo, June 19, 20001
//-----------------------------------------------------------------------------
{
int year, month, day, hour, minute ,second, hsecond;
double rhour,jd_ut1;
double last,ap_ha,ap_dec,alt,azi,ap_ra;

    dms_to_rad(&SITE_LONGI);
    dms_to_rad(&SITE_LATI );

    read_computer_time(&year,&month,&day,&hour,&minute,&second,&hsecond);
    rhour  = hour + minute/60.0 + (second + hsecond/100.0) / 3600.0;
    jd_ut1 = get_jd(year,month,day,rhour) - 9.0/24.0;
	     
    get_aposi(ra,dec,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,0,
              &last,&ap_ha,&ap_dec,&alt,&azi);
              
    ap_ra = last - ap_ha;
    if(ap_ra<0.0  ) ap_ra += TWOPI;
    if(ap_ra>TWOPI) ap_ra -= TWOPI;
    
    *ra_2000 = ra + (ra - ap_ra );
    *dec_2000= dec+ (dec- ap_dec);
    
//  printf("alt = %10.3lf azi = %10.3lf\n",rad_to_deg(alt),rad_to_deg(azi));
}	     
//-----------------------------------------------------------------------------
