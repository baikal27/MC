#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <userint.h>
#include <ansi_c.h>
#include <math.h>
#include "type_def.h"
#include "cvi_util.h"
#include "st4s_lib.h"

//----------------------------------------------------------------------------
extern int  DOME_DIR,UP_SLIT_DIR,DOWN_SLIT_DIR,LAMP_STATUS;
//----------------------------------------------------------------------------
/*
30 cm Telescope Dome control bit assignemnt.

Output

  USEROUT bit
    
    0 : dome cw     2 sejong
    1 : dome ccw	3 sejong 	 

    2 : upper slit open
    3 : upper slit close
    4 : upper slit stop
  
    5 : lower slit open
    6 : lower slit close
    7 : lower slit stop
  
  IOPORT1 bit
    0 : lamp on
    1 : lamp off		 

Input
    Home switch : ioport0.7    

All bits of USEROUT and IOPORT1 are normally high.
To do a operation, make the coresponding bit low.
For example, to turn on the lamp, make IPORT1 bit 0 low.

                       Code created by Han Inwoo, 1998, Dec. 28.
*/                       
//----------------------------------------------------------------------------
static double dtime = 3.0;
//----------------------------------------------------------------------------
int  dome_cw(void); 
int  dome_ccw(void); 
int  dome_stop(void);

int  upper_slit_open (void);
int  upper_slit_close(void);
int  upper_slit_stop (void);

int  lower_slit_open (void);
int  lower_slit_close(void);
int  lower_slit_stop (void);

int  lamp_on (void);
int  lamp_off(void);

int  read_dome_home(int *DOME_HOME_SWITCH);
int  read_dome_encoder(long *encoder);
int  set_dome_encoder (long  encoder);
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int dome_cw()
//------------------------------------------------------------------------
{
//  if(DOME_DIR==-1) {dome_stop(); Delay(2);}
//    if(set_userout_bit('A',2,0)!=0)
//      {
//       MessagePopup("","Error in dome_cw");
//       return -1;
//      }
    DOME_DIR = 1;
    return 0;
}	    
//------------------------------------------------------------------------
int dome_ccw()
//------------------------------------------------------------------------
{
//  if(DOME_DIR== 1) {dome_stop(); Delay(2);}
//    if(set_userout_bit('A',3,0)!=0)
//      {
//       MessagePopup("","Error in dome_ccw");
//       return -1;
//      }
      
    DOME_DIR = -1;
    return 0;
}	    
//------------------------------------------------------------------------
int dome_stop()
//------------------------------------------------------------------------
{
//    if(set_userout_bit('A',2,1)!=0) return -1;
//    if(set_userout_bit('A',3,1)!=0) return -2;
    DOME_DIR = 0;
    return 0;
}	    
//------------------------------------------------------------------------
int upper_slit_open()
//------------------------------------------------------------------------
{
//    if(UP_SLIT_DIR==0 && DOWN_SLIT_DIR==0) 
//      {
//       SetWaitCursor(1);
//       set_userout_bit('A',2,0); Delay(1); set_userout_bit('A',2,1);
//       Delay(dtime); SetWaitCursor(0);
//       return 0;
//      }
//    else
//      {
//       MessagePopup("","Slit is not stop");
//       return -1;
//      }
	return 0;
}
//------------------------------------------------------------------------
int upper_slit_close()
//------------------------------------------------------------------------
{
/*
    if(UP_SLIT_DIR==0 && DOWN_SLIT_DIR==0) 
      {
       SetWaitCursor(1);
       set_userout_bit('A',3,0); Delay(1); set_userout_bit('A',3,1);
       Delay(dtime);
       SetWaitCursor(0); return 0;
      }
    else
      {
       MessagePopup("","Slit is not stop");
       return -1;
      }
*/
	return 0;
}
//------------------------------------------------------------------------
int upper_slit_stop()
//------------------------------------------------------------------------
{
/*
	SetWaitCursor(1);
    set_userout_bit('A',4,0); Delay(1); set_userout_bit('A',4,1);
    Delay(dtime); SetWaitCursor(0);
    return 0;
*/
	return 0;
}
//------------------------------------------------------------------------
int lower_slit_open()
//------------------------------------------------------------------------
{
/*
    if(UP_SLIT_DIR==0 && DOWN_SLIT_DIR==0) 
      {
       SetWaitCursor(1);
       set_userout_bit('A',5,0); Delay(1); set_userout_bit('A',5,1);
       Delay(dtime); SetWaitCursor(0);
       return 0;
      }
    else
      {
       MessagePopup("","Slit is not stop");
       return -1;
      }
*/
	return 0;
}
//------------------------------------------------------------------------
int lower_slit_close()
//------------------------------------------------------------------------
{
/*
    if(UP_SLIT_DIR==0 && DOWN_SLIT_DIR==0) 
      {
       SetWaitCursor(1);
       set_userout_bit('A',6,0); Delay(1); set_userout_bit('A',6,1);
       Delay(dtime); SetWaitCursor(0);
	   return 0;
      }
    else
      {
       MessagePopup("","Slit is not stop");
       return -1;
      }
*/
	return 0;
}
//------------------------------------------------------------------------
int lower_slit_stop()
//------------------------------------------------------------------------
{
/*
    SetWaitCursor(1);
    set_userout_bit('A',7,0); Delay(1); set_userout_bit('A',7,1);
    Delay(dtime); SetWaitCursor(0);
    return 0;
*/
	return 0;
}
//------------------------------------------------------------------------
int lamp_on()
//------------------------------------------------------------------------
{
/*
    SetWaitCursor(1);
    set_ioport1_bit(1,0); Delay(3); set_ioport1_bit(1,1); 
    Delay(dtime); SetWaitCursor(0);
    return 0;
*/
	return 0;
}
//------------------------------------------------------------------------
int lamp_off()
//------------------------------------------------------------------------
{
/*
    SetWaitCursor(1);
    set_ioport1_bit(0,0); Delay(3); set_ioport1_bit(0,1); 
    Delay(dtime); SetWaitCursor(0);
    return 0;
*/
	return 0;
}
//------------------------------------------------------------------------
int read_dome_encoder(long *encoder)
//------------------------------------------------------------------------
{
    return read_encoder('B',encoder);
}
//------------------------------------------------------------------------
int set_dome_encoder(long encoder)
//------------------------------------------------------------------------
{
/*
    return set_timer2(encoder);
*/
	return 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int read_dome_home(int *DOME_HOME_SWITCH)
//------------------------------------------------------------------------
{
//    return read_ioport0_bit(7,DOME_HOME_SWITCH);
	return 0;
}
//------------------------------------------------------------------------
int set_domeold()
//------------------------------------------------------------------------
{
/*
 int iii;
 FILE *fin;
 char buff[200];
    if((fin = fopen("domeencold.dat","r"))!=NULL )
      {
       fgets(buff,100,fin);
       sscanf(buff,"%d",&iii);
       fclose(fin);
      }
     set_timer2(iii); 
return 0;
*/
	return 0;
}
//------------------------------------------------------------------------
int save_domeold()
//------------------------------------------------------------------------
{
/*
 int iii;
 FILE *fin;
    fin = fopen("domeencold.dat","w");
    read_dome_encoder(&iii);
    fprintf(fin,"%d",iii);
    fclose(fin);
return 0;
*/
	return 0;
}
//------------------------------------------------------------------------
