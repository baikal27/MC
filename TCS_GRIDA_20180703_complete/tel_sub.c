#include "shmtcs.h"
#include <utility.h>
#include <userint.h>
#include <ansi_c.h>

#include "type_def.h"
#include "cal_lib\astrometry.h"

#include "st4s_lib.h "  
#include "cvi_util.h" 
#include "cal_sub.h"
#include "gv_tel.h"    // global variables for telescope control

extern int STOP_TELESCOPE(void);
extern void display_message(char buff[]);
//------------------------------------------------------------------------
//        Prototype of Functions
//------------------------------------------------------------------------
#include "tel_sub.h"    // global variables for telescope control
/*  
int read_telescope_parameter(void);
int init_telescope_parameter(void);
int read_tcs_parameter(void);

int slew_to_radec(h_m_s ra_2000, d_m_s dec_2000);

int slew_to_hadec(h_m_s ap_ha  , d_m_s ap_dec  );
int slew_to_altazi(d_m_s alt, d_m_s azi);

int slew_to_home(void);
double calc_expt (long steps, ramp_data *ramp);

int correct_servo_ra (h_m_s ra_2000, d_m_s dec_2000,long *count_ha ,double *error_ha ,double *track_speed);
int correct_servo_dec(h_m_s ra_2000, d_m_s dec_2000,long *count_dec,double *error_dec);
int correct_servo_alt(h_m_s ra_2000, d_m_s dec_2000,long *count_alt,double *error_alt,double *track_speed);
int correct_servo_azi(h_m_s ra_2000, d_m_s dec_2000,long *count_azi,double *error_azi,double *track_speed);

int do_guide_hp(void);
int do_guide_ew(hp_status hp);
int do_guide_ns(hp_status hp);
*/

static int do_guide_axis(char caxis, int dir, int gst_mode);
int check_guiding(void);
//------------------------------------------------------------------------


//------------------------------------------------------------------------
//                 Global Variables for internal use 
//------------------------------------------------------------------------
static const double  PI314           = 3.14159265358979323846;
static const double  TWOPI           = 3.14159265358979323846*2.0;
static const double  FACT_DEG_TO_RAD = 3.14159265358979323846/180.0;
static const double  FACT_RAD_TO_DEG = 180.0/3.14159265358979323846;

static int year,month,day,hour,minute,second,hsecond;
static h_m_s last,ap_ha;
static d_m_s ap_dec,alt,azi;
static double jd_ut1,jd_ut2;
//static double speed;
static char buff[300];
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int read_telescope_parameter()
//------------------------------------------------------------------------
{
 FILE *fin;
 int i,ii;
 char ssss[150];

//  read site parameter
    if((fin = fopen("site.inp","r"))==NULL )
      {MessagePopup(""," CANNOT OPEN SITE.INP"); exit(0);}

    fgets(ssss,100,fin);
    sscanf(ssss,"%d %d %lf", &SITE_LONGI.d, &SITE_LONGI.m, &SITE_LONGI.s);
    fgets(ssss,100,fin);
    sscanf(ssss,"%d %d %lf", &SITE_LATI.d,  &SITE_LATI.m,  &SITE_LATI.s);
    fgets(ssss,100,fin);
    sscanf(ssss,"%lf",  &SITE_HEIGHT);
    fclose(fin);
    dms_to_rad(&SITE_LONGI); dms_to_rad(&SITE_LATI);

//  read dut1 data
    if( (fin = fopen("dut1.inp","r"))==NULL )
      {Beep(); MessagePopup(""," CANNOT OPEN DUT1.INP \n");
       return -1;
      }
    fscanf(fin,"%lf", &DUT1_SEC); DUT1_DAY = DUT1_SEC/(24.0*3600.0);
    fclose(fin);

//  read mount model
    read_mount_model("mount.inp");
    
    return 0;
}
//------------------------------------------------------------------------
int read_tcs_parameter()
//------------------------------------------------------------------------
{
 FILE *fin;
 double a,b,c,speed;
 int i,nstair;
 int  wdivisor;
 long int li;
 char ans,ssss[250];
 long count_0 = 0;
 char caxis;
 
    if( (fin = fopen("init_tel.inp","r"))==NULL)
      {MessagePopup(""," CANNOT OPEN INIT_TEL.INP"); exit(0);}

    NO_OF_AXIS =2;

//  SLEW, GUIDE, SET and INIT speed for R.A. and DEC.
    for(i=0;i<2;i++)
       {
        fgets(ssss,200,fin);
        sscanf(ssss,"%lf %lf %lf %lf %lf %lf",&SLEW_SPEED[i],&VPROFILE[i].fmin,&VPROFILE[i].time,&GUIDE_SPEED[i],&GSET_SPEED[i],&INIT_SPEED[i]);

//		VPROFILE[i].fmax = SLEW_FREQ[i];
//	    VPROFILE[i].acce = (SLEW_FREQ[i]-VPROFILE[i].fmin)/VPROFILE[i].time;
	   }

//  COUNTS of HOME and INIT point     
    for(i=0;i<NO_OF_AXIS;i++)
       {
        fgets(ssss,200,fin);
        sscanf(ssss,"%ld %ld", &HOME_COUNT[i],&INIT_COUNT[i]);
       }
       
//  limit switch status
    for(i=0;i<NO_OF_AXIS;i++)
       {
        fgets(ssss,200,fin);
        sscanf(ssss,"%d %d %d", &SWSTATUS[i].cw,&SWSTATUS[i].home,&SWSTATUS[i].ccw);
       }
//  limit HA in hour
     fgets(ssss,200,fin);
     sscanf(ssss,"%lf %lf", &HALIMITMIN,&HALIMITMAX);
	
// HOMEP[0].v2 = 1000.0;  //2단계에서의 속도
// HOMEP[0].v3 = 1000.0;  //3단계에서의 속도
// HOMEP[0].v4 = 1000.0;  // index검색및 정밀하게 검색하기위한 속도 (Offset값 이동시 적용)
// HOMEP[0].a1 = VPROFILE[0].acce;  //1단계에서의 가속도
// HOMEP[0].a2 =  400.0;  //2단계에서의 가속도
	
    fgets(ssss,200,fin);
    sscanf(ssss,"%lf %lf %lf %lf", &HOMEP[0].v2,&HOMEP[0].v3,&HOMEP[0].v4,&HOMEP[0].a2);
    fgets(ssss,200,fin);
    sscanf(ssss,"%lf %lf %lf %lf", &HOMEP[1].v2,&HOMEP[1].v3,&HOMEP[1].v4,&HOMEP[1].a2);
	
//  SERVO UPDATE SEC
    fgets(ssss,200,fin);
    sscanf(ssss,"%lf", &SERVO_UPDATE_SEC);
        
//  maximum error of dec axis to sevo correction
    fgets(ssss,200,fin);
    sscanf(ssss,"%lf %lf", &MAX_ERROR_RA_ARCSEC,&MAX_ERROR_DEC_ARCSEC);
    
//  dome control
    fgets(ssss,200,fin);
    sscanf(ssss,"%d", &LINKDOME);
    
    fgets(ssss,200,fin);
    sscanf(ssss,"%d", &SW_LIMIT);
    
    if(TELESCOPE_TYPE==_ALT_AZI)
      {
       fgets(ssss,200,fin);
       sscanf(ssss,"%lf", &PSI_FACTOR);
      }
   
    fclose(fin);
    
    return 0;
}
//------------------------------------------------------------------------
int init_telescope_parameter()
//------------------------------------------------------------------------
{
 int i;
 
//  initialize offset ha and dec        
    OFFSET_HA_COUNT = OFFSET_DEC_COUNT = 0.0;

//  calculate mean tracking frequency
    MEAN_TRACKING_FREQ[0] = MEAN_TRACKING_FREQ[1] = 0;

    if(TELESCOPE_TYPE==_RA_DEC)
      {
       MEAN_TRACKING_FREQ[0] = MOUNT_HA[1]*(360.0/23.9344696);
       if(ENCODER_MODE==1) MEAN_TRACKING_FREQ[0] *= ENC_TO_STEP[0];
      }

//  claculate frequency from RA and DEC speed
     SLEW_FREQ[0] =  SLEW_SPEED[0] *3600.0* MOUNT_HA[1];  // from deg into arcsecond/sec
     GUIDE_FREQ[0] = GUIDE_SPEED[0] * 60.0 * MOUNT_HA[1];  // from arcminute into arcsecond/sec
     GSET_FREQ[0] =  GSET_SPEED[0] * 60.0 * MOUNT_HA[1];  // from arcminute into arcsecond/sec
     INIT_FREQ[0] =  INIT_SPEED[0]        * MOUNT_HA[1];  // arcsec / sec

     SLEW_FREQ[1] =  SLEW_SPEED[1] *3600.0* MOUNT_DEC[1]; // from deg into arcsecond/sec
    GUIDE_FREQ[1] = GUIDE_SPEED[1] * 60.0 * MOUNT_DEC[1]; // from arcminute into arcsecond/sec
     GSET_FREQ[1] =  GSET_SPEED[1] * 60.0 * MOUNT_DEC[1]; // from arcminute into arcsecond/sec
     INIT_FREQ[1] =  INIT_SPEED[1]        * MOUNT_DEC[1]; // arcsec / sec
     
     PARK_FREQ[0] = SLEW_FREQ[0];
     PARK_FREQ[1] = SLEW_FREQ[1];
     
//  scale adjustment of the frequency        
    if(ENCODER_MODE==1)
    {
		for(i=0;i<2;i++)
       {
         SLEW_FREQ[i] *= ENC_TO_STEP[i];
        GUIDE_FREQ[i] *= ENC_TO_STEP[i];
         GSET_FREQ[i] *= ENC_TO_STEP[i];
         INIT_FREQ[i] *= ENC_TO_STEP[i];
         PARK_FREQ[i] *= ENC_TO_STEP[i];
         
   	    VPROFILE[i].fmax = SLEW_FREQ[i];
	    VPROFILE[i].acce = (SLEW_FREQ[i]-VPROFILE[i].fmin)/VPROFILE[i].time;
       }
	}
    else
	{
    	for(i=0;i<2;i++)
       	{
   	    	VPROFILE[i].fmax = SLEW_FREQ[i];
	    	VPROFILE[i].acce = (SLEW_FREQ[i]-VPROFILE[i].fmin)/VPROFILE[i].time;
        }
	}

    sprintf(buff,"mean tracking f=%lf\n",MEAN_TRACKING_FREQ[0]);
    display_message(buff);
    sprintf(buff,"fmax=%lf\n",SLEW_FREQ[0]);
    display_message(buff);

    sprintf(buff,"VPROFILE_MAX=%lf\n",VPROFILE[0].fmax);
    display_message(buff);
    sprintf(buff,"VPROFILE_MIN=%lf\n",VPROFILE[0].fmin);
    display_message(buff);
    sprintf(buff,"VPROFILE_ACC=%lf\n",VPROFILE[0].acce);
    display_message(buff);

    return 0;      
}      
//------------------------------------------------------------------------
int slew_to_hadec(h_m_s ap_ha, d_m_s ap_dec)
//------------------------------------------------------------------------
{
 double goto_ha, goto_dec, step_ha, step_dec;
 long ha0, dec0, step1,step2, lRef1, lRef2;
 double  alt_deg,azi_deg,last,temp;

//  Calculate azimuth and altitude                       
    if(ap_ha.rad> PI314) ap_ha.rad = ap_ha.rad - TWOPI;
    if(ap_ha.rad<-PI314) ap_ha.rad = ap_ha.rad + TWOPI;
    last = 0.0;
    equtohor(-ap_ha.rad,ap_dec.rad,last,SITE_LATI.rad,&azi.rad,&alt.rad);
    alt_deg = rad_to_deg(alt.rad);
    azi_deg = rad_to_deg(azi.rad);

//  Check whether altitude is too low
    if(alt_deg < 10)
      {Beep();
       MessagePopup("","Altitude is too low..");
       return -1;
      }

//  Calcualate the current position of the telescope
    if(ENCODER_MODE==0)
      {read_counter('A',&ha0); read_counter('B',&dec0);}
    else
      {read_encoder('A',&ha0); read_encoder('B',&dec0);}

//  Calculate the # of steps to move 
    goto_ha  = calc_ha_count (ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT;
    goto_dec = calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_DEC_COUNT;
    step_ha  = goto_ha  -  ha0;
    step_dec = goto_dec - dec0;

    if(ENCODER_MODE==1)
      {
       step_ha  = step_ha  * ENC_TO_STEP[0];
       step_dec = step_dec * ENC_TO_STEP[1];
      }

// -------------   Calcuate # of steps for R.A. axis. -----------------
	lRef1   = step_ha + 0.5;
//  temp = calc_expt(lRef1,&RAMP[0]);
	temp = lRef1/VPROFILE[0].fmax;
	
    if(ENCODER_MODE==0)
       step_ha += temp*15.0*MOUNT_HA[1];
    else
       step_ha += temp*15.0*MOUNT_HA[1]*ENC_TO_STEP[0];

    lRef1   = step_ha + 0.5;

// -------------   Calcuate # of steps for Dec. axis. -----------------
	lRef2 = step_dec + 0.5;

//  Motor go	
    slew_stop('B',lRef2);
    slew_stop('A',lRef1);
    
    return 0;
}
//------------------------------------------------------------------------
int slew_to_radec(h_m_s ra_2000, d_m_s dec_2000)
//------------------------------------------------------------------------
//  Input
//    ra_2000 & dec_2000   : R.A. and Dec. in J2000 coordiantes.
//------------------------------------------------------------------------
{
 double step_azi,step_alt,step_der,rhour,jd_ut;
 long   azi0,alt0,der0,step1,step2,lRef1 = 0, lRef2 = 0;
 double temp,delay_azi,delay_alt;
 int iii;

//    read_computer_time(&year, &month, &day, &hour, &minute, &second, &hsecond);
//    rhour  = hour + minute/60.0 + (second + hsecond/100.0) / 3600.0;
//    jd_ut1 = get_jd(year,month,day,rhour) - 9.0/24.0 + DUT1_DAY;
//    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    
        
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;

//     sprintf(buff,"JD=%lf \n",jd_ut1);
//     display_message(buff);

//  read the current position of the telescope
    if(ENCODER_MODE==0)
      {read_counter('A',&azi0); read_counter('B',&alt0);}
    else
      {read_encoder('A',&azi0); read_encoder('B',&alt0);}
      
    delay_azi = 0;
    delay_alt = 0;
    for(iii=0;iii<2;iii++)
       {
//      Azimuth       
        jd_ut = jd_ut1 + delay_azi;
        get_aposi(ra_2000.rad, dec_2000.rad,jd_ut,SITE_LONGI.rad,SITE_LATI.rad,1,
                  &last.rad,&ap_ha.rad,&ap_dec.rad,&alt.rad,&azi.rad);
                        
        
        if(azi.rad> PI314) azi.rad -= TWOPI;
        if(azi.rad<-PI314) azi.rad += TWOPI;
        
        step_azi = calc_ha_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT - azi0;
        
        if(ENCODER_MODE==1) step_azi = step_azi * ENC_TO_STEP[0];
        
//      Altitude                       
        jd_ut = jd_ut1 + delay_alt;
        get_aposi(ra_2000.rad, dec_2000.rad,jd_ut,SITE_LONGI.rad,SITE_LATI.rad,1,
                  &last.rad,&ap_ha.rad,&ap_dec.rad,&alt.rad,&azi.rad);
              
        if( rad_to_deg(alt.rad) < 10)
          {Beep(); 
           if(ConfirmPopup("고도가 너무 낮습니다","돌아갈까요 ?")==1)
             {
              STOP_TELESCOPE();
              return -1;
             }
          }
          
        step_alt = calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_DEC_COUNT - alt0;
        
        if(ENCODER_MODE==1) step_alt = step_alt * ENC_TO_STEP[1];
   
// -------------   Calcuate # of steps  ------------------
	    lRef1 = step_azi + 0.5;
	    lRef2 = step_alt + 0.5;
	    
//        delay_azi = calc_expt(lRef1,&RAMP[0])/(24.0*3600.0);
//        delay_alt = calc_expt(lRef2,&RAMP[1])/(24.0*3600.0);
     }

//  Turn on motor
//    sprintf(buff,"%d %d\n",lRef1,lRef2);
//    display_message(buff);

    slew_stop('A',lRef1);
    Delay(0.1);
    slew_stop('B',lRef2);
    Delay(0.1);
    
    return 0;
}
//------------------------------------------------------------------------
int slew_to_altazi(d_m_s alt, d_m_s azi)
//------------------------------------------------------------------------
//  Input
//    apparent alt & azi
//------------------------------------------------------------------------
{
 double step_azi,step_alt;
 long   azi0,alt0,step1,step2,lRef1,lRef2;

//  check low altitue
    if(rad_to_deg(alt.rad) < 10)
      {Beep(); 
       if( ConfirmPopup("고도가 너무 낮습니다","돌아갈까요 ?")==1 ) return -1;
      }

//  read the current position of the telescope
    if(ENCODER_MODE==0)
      {read_counter('A',&azi0); read_counter('B',&alt0);}
    else
      {read_encoder('A',&azi0); read_encoder('B',&alt0);}
      
//  azimuth        
    altazi_to_hadec(alt.rad,azi.rad,SITE_LATI.rad,&ap_ha.rad,&ap_dec.rad);
    step_azi = calc_ha_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT  - azi0;
    if(ENCODER_MODE==1) step_azi = step_azi * ENC_TO_STEP[0];
              
//  altitude        
    step_alt = calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_DEC_COUNT - alt0;
    if(ENCODER_MODE==1) step_alt = step_alt * ENC_TO_STEP[1];
   
// -------------   Calcuate # of steps  ------------------
    lRef1 = step_azi + 0.5;
    lRef2 = step_alt + 0.5;
	    
//  Turn on motor	
    slew_stop('A',lRef1); slew_stop('B',lRef2);
    
    return 0;
}
//------------------------------------------------------------------------
double calc_expt(long steps, ramp_data *ramp)
//------------------------------------------------------------------------
{
 int i, n_peak, nstairs;
 long int peak_steps = 0;
 double t_exp;

	if(steps<0) steps = -steps;
	nstairs = (*ramp).nstairs;

//	Calculate (1) the peak speed and the # of pulses
	if(steps <=  (*ramp).sum[4]*2 )
      {n_peak = 0; peak_steps = steps;
       t_exp  = steps/(*ramp).freq[0];
       return t_exp;
      }

    n_peak = 1;
    for(i=nstairs-1;i>0;i--)
       {
        peak_steps = steps - (*ramp).sum[i]*2;
        if(peak_steps>0) {n_peak = i+1; break;}
       }
    t_exp = (*ramp).rise_t * n_peak/nstairs * 2.0 + peak_steps/(*ramp).freq[n_peak];

    return t_exp;
}
//------------------------------------------------------------------------


//------------------------------------------------------------------------
int correct_servo_ra(h_m_s ra_2000, d_m_s dec_2000, long *count_ha, double *error_ha, double *track_speed)
//------------------------------------------------------------------------
//       Input :
//          h_m_a  ra_2000   coordinates of R.A. in J2000 system
//          d_m_s dec_2000   coordinates of Dec. in J2000 system
//
//                                             created by Han Inwoo 1998. 3. 7.
//------------------------------------------------------------------------
{
 long lRef,step_ha;
 static long counts,encoder,counts_old,encoder_old;
 static double dc,de,temp,speed_old;
 double maxspeed,speed;
 
    maxspeed=600;
 
    read_counter('A',&counts);
    read_encoder('A',&encoder);

    if(ENCODER_MODE==0) step_ha = counts;
    else                step_ha = encoder;

//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);
    *error_ha  = calc_ha_count (ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT  - step_ha ;

    jd_ut2 = jd_ut1 + SERVO_UPDATE_SEC/3600.0/24.0;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut2, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    speed = (calc_ha_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT - step_ha )
            / SERVO_UPDATE_SEC;
              
//  if(fabs(*error_ha) < MAX_ERROR_RA_ARCSEC) speed = MEAN_TRACKING_FREQ[0];
              
//  calculate tracking speed
    *track_speed = speed/MOUNT_HA[1];
    
//    if     (*track_speed> maxspeed) {speed = maxspeed*MOUNT_HA[1];*track_speed= maxspeed;}
//    else if(*track_speed<-maxspeed) {speed =-maxspeed*MOUNT_HA[1];*track_speed=-maxspeed;}

    if(ENCODER_MODE==1) speed *= ENC_TO_STEP[0];

    go_const('A',speed);

//  convert error to arcsec
    *error_ha  = *error_ha  /MOUNT_HA[1];

    *count_ha = step_ha;
    
    dc = (counts-counts_old*1.0);
    de = (encoder-encoder_old*1.0);
    temp = dc/de;
    
//    sprintf(buff,"e_to_c %7.1lf %7.1lf %7.4lf",speed_old,*error_ha,temp);
//    display_message(buff);
    
    counts_old  = counts;
    encoder_old = encoder;
    speed_old = speed;
    
    return 0;
}
//------------------------------------------------------------------------
int correct_servo_dec(h_m_s ra_2000, d_m_s dec_2000, long *count_dec,double *error_dec)
//------------------------------------------------------------------------
//       Input :
//          h_m_a  ra_2000   coordinates of R.A. in J2000 system
//          d_m_s dec_2000   coordinates of Dec. in J2000 system
//
//                                             created by Han Inwoo 1998. 3. 7.
//------------------------------------------------------------------------
{
 long lRef,step_dec;
 int state;
 double speed;

    if(ENCODER_MODE==0) read_counter('B',&step_dec);
    else                read_encoder('B',&step_dec);

//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    *error_dec = calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_DEC_COUNT - step_dec;

    jd_ut2 = jd_ut1 + SERVO_UPDATE_SEC/3600.0/24.0;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut2, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

//  CORRECT DEC. AXIS
    if(ENCODER_MODE==0)  lRef = *error_dec + 0.5;
    else                 lRef = *error_dec * ENC_TO_STEP[1] + 0.5;

//  convert error to arcsec
    *error_dec = *error_dec /MOUNT_DEC[1];

//  Correct Dec. axis

    if( fabs(*error_dec) > MAX_ERROR_DEC_ARCSEC)
      {
//     read_gate('B',&state);
//     if(state==0) slew_stop('B',lRef);
	   speed = lRef/SERVO_UPDATE_SEC;
	   if(fabs(speed)>GUIDE_FREQ[1])
	   if(lRef>0) speed = GUIDE_FREQ[1]; else speed =-GUIDE_FREQ[1];
       go_const('B',speed);	   
      }
	else motor_stop('B');

    *count_dec= step_dec;
        
    return 0;
}
//------------------------------------------------------------------------
int correct_servo_azi(h_m_s ra_2000, d_m_s dec_2000, long *count_ha, double *error_ha, double *track_speed)
//------------------------------------------------------------------------
//  Input :
//    h_m_a  ra_2000   coordinates of R.A. in J2000 system
//    d_m_s dec_2000   coordinates of Dec. in J2000 system
//
//                       created by Han Inwoo 1999. 1. 11.
//------------------------------------------------------------------------
{
 long lRef,step_ha;
 char caxis;
 int iaxis;
 double speed;

    caxis = 'A'; iaxis = caxis - 'A';
    
    if(ENCODER_MODE==0) read_counter(caxis,&step_ha);
    else                read_encoder(caxis,&step_ha);

//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);
    *error_ha  = calc_ha_count (ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT  - step_ha ;

    jd_ut2 = jd_ut1 + SERVO_UPDATE_SEC/3600.0/24.0;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut2, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    speed = (calc_ha_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_HA_COUNT - step_ha ) / SERVO_UPDATE_SEC;

//  calculate tracking speed
    *track_speed = speed/MOUNT_HA[1];

    if(ENCODER_MODE==1) speed *= ENC_TO_STEP[iaxis];

     go_const(caxis,speed);

//  convert error to arcsec
    *error_ha = *error_ha/MOUNT_HA[1];
    *count_ha = step_ha;

    return 0;
}
//------------------------------------------------------------------------
int correct_servo_alt(h_m_s ra_2000, d_m_s dec_2000, long *count_dec, double *error_dec, double *track_speed)
//------------------------------------------------------------------------
//    Input :
//      h_m_a  ra_2000   coordinates of R.A. in J2000 system
//      d_m_s dec_2000   coordinates of Dec. in J2000 system
//
//                      created by Han Inwoo 1999. 1. 11.
//------------------------------------------------------------------------
{
 long lRef,step_dec,steps;
 double speed;
 char caxis;
 int iaxis;
 double fact;
 
    caxis = 'B';
    iaxis = caxis - 'A';

    if(ENCODER_MODE==0) read_counter(caxis,&step_dec);
    else                read_encoder(caxis,&step_dec);

//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    *error_dec = calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_DEC_COUNT - step_dec;

    jd_ut2 = jd_ut1 + SERVO_UPDATE_SEC/3600.0/24.0;
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut2, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);

    speed = (calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad) + OFFSET_DEC_COUNT - step_dec) / SERVO_UPDATE_SEC;

//  calculate tracking speed
	*track_speed = speed/MOUNT_DEC[1];
	
    if(ENCODER_MODE==1) speed *= ENC_TO_STEP[iaxis];
//      if(ENCODER_MODE==1) 
//	    {fact=ENCTOSTEP_DE[1]+2.0*ENCTOSTEP_DE[2]*step_dec + 3.0*ENCTOSTEP_DE[3]*step_dec*step_dec;
//		speed *= fact;
//	    }
     go_const(caxis,speed);
	 
//	 sprintf(buff,"alt speed = %d\n",speed);
//	 display_message(buff);
	 
//  convert error to arcsec
    *error_dec   = *error_dec/MOUNT_DEC[1];
    *count_dec   = step_dec;
   
    return 0;
}
//------------------------------------------------------------------------


//------------------------------------------------------------------------
int check_guiding()
//------------------------------------------------------------------------
{
 static hp_status hpstatus;
 
    read_hp_status(&hpstatus);
        
    if     (hpstatus.ew==-1) OFFSET_HA_COUNT  -= GUIDE_FREQ[0]/cos(CURRENT_DEC.rad)/5.0;
    else if(hpstatus.ew==+1) OFFSET_HA_COUNT  += GUIDE_FREQ[0]/cos(CURRENT_DEC.rad)/5.0;
    
    if     (hpstatus.ns==-1) OFFSET_DEC_COUNT -= GUIDE_FREQ[1]/5.0;
    else if(hpstatus.ns== 1) OFFSET_DEC_COUNT += GUIDE_FREQ[1]/5.0;
      
    return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK AGTIMER_CB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_TIMER_TICK:
             if(TELESCOPE_MODE==TEL_MODE_TRACK) check_guiding();
			break;
		}
	return 0;
}
//------------------------------------------------------------------------
int slew_to_home()
//------------------------------------------------------------------------
//  Input
//    ra_2000 & dec_2000   : R.A. and Dec. in J2000 coordiantes.
//------------------------------------------------------------------------
{
 long enca,encb;

    read_encoder('A',&enca); read_encoder('B',&encb);
    enca *= ENC_TO_STEP[0];
    encb *= ENC_TO_STEP[1];
    slew_stop('A',-enca);
    Delay(0.1);
    slew_stop('B',-encb);
//    SLEW_MODE[0] = SLEW_MODE[1] = _SLEW_MODE_ON;
    
    TELESCOPE_MODE = TEL_MODE_PARK;

    return 0;
}
//------------------------------------------------------------------------
