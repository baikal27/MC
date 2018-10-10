#include <ansi_c.h>
#include <utility.h>
#include <userint.h>

#include "type_def.h"
#include "cal_lib\astrometry.h"   // including cpabtsub.h, mathsub.h and so on.
#include "cvi_util.h"
#include "AXM.h"
#include "st4s_lib.h"
#include "gv_tel.h"
#include "tel_sub.h"
#include "cal_sub.h"
#include "tel_sub2.h"

#define AZI_VEL_MAX	26132.0		// Unit of degree per second
//#define ALT_VEL_MAX	530532.0		// Unit of degree per second
#define ALT_VEL_MAX	600532.0		// Unit of degree per second

#define SETTLING_VEL1	4000.0		// Settling range
#define SETTLING_VEL2	600.0
//#define SETTLING_VEL3	200.0
//#define SETTLING_VEL4	150.0
//#define SETTLING_VEL5	100.0
#define TOLERANCE_VEL	200.0		// Unit of degree
/*
#define SETTLING_VEL1	60000.0		// Settling range
#define SETTLING_VEL2	20000.0
#define SETTLING_VEL3	8000.0
#define TOLERANCE_VEL	100.0
*/
static const double TWOPI = 3.14159265358979323846*2.0;
static const double PI314 = 3.14159265358979323846;
static const double dAccel=400.; // initial value
static const double dDecel=400.; // initial value   

extern void display_message(char buff[]) ;
extern void display_message2(char buff[]) ;
extern float off_ra, off_dec, off_azi, off_alt;

/////////////////////////////////////////////////////////////////////////////////
// slew_to_radec  : comparison
int slew_track_radec(h_m_s target_ra, d_m_s target_dec)
{
	double cmd_azi[3]={0.,0.,0.}, cmd_alt[3]={0.,0.,0.};
//	double 
	long now_azi=0, now_alt=0;
	double d_azi=0., d_alt=0.;
	double cmd_azi_vel=0., cmd_alt_vel=0.;
	double cal_azi_vel=0., cal_alt_vel=0.;
	double sum_azi_vel=0., sum_alt_vel=0.;
	double req_azi_vel=0., req_alt_vel=0.;
	double d_azioff, d_altoff;
	int is_track=0;
	char GBUFF[500] ;
	int dir_azi=0, dir_alt=0;
//	h_m_s target_ra ;
//	d_m_s target_dec ;

	target_ra.rad += off_ra;	  // radian
	target_dec.rad += off_dec;	  // radian
				 
	rad_to_hms(& target_ra);
	rad_to_dms(& target_dec);
	
	
	sprintf(GBUFF, "offset RA: %f, offset DEC: %f \n", target_ra.rad, target_dec.rad);
	display_message2(GBUFF);

// calculate the current position of target with -1sec, 0sec, 1sec
	calpos_radec(target_ra, target_dec, cmd_azi, cmd_alt);			  // cmd_azi, cmd_azi는 cmd_azi[3], cmd_alt[3] 배열의 1번째 요소 어드레스이다. 
	
// read the current position of the telescope
	if(ENCODER_MODE==0)
		{read_counter('A', &now_azi); read_counter('B', &now_alt);}
	else
		{read_encoder('A', &now_azi); now_azi *= ENC_TO_STEP[0]; read_encoder('B', &now_alt); now_alt *= ENC_TO_STEP[1];}		 // convert the encoder value to step
		
	d_azi = cmd_azi[1] - now_azi + 0.5;	// 0.5를 더한 것은 tel_sub.c의 slew_to_radec 함수에서 보정한 값. 왜 이 값을 더하는지는 모르겠음.	 // cmd_azi[1]은 배열 첫번째 요소 값이다.
	d_alt = cmd_alt[1] - now_alt + 0.5; // cmd_azi[1]은 배열 첫번째 요소 값이다. cmd_azi[]라는 배열에 괄호가 붙으면 값을, 그냥 cmd_azi, cmd_azi+1, cmd_azi+2 이런 식이면 이건 포인터를 나타내는 것.
																													  
//Velocity routine  ( plus / minus have to be concerned)
	cal_azi_vel = cmd_azi[2] - cmd_azi[1];	
	cal_alt_vel = cmd_alt[2] - cmd_alt[1];
	cmd_azi_vel = d_azi;	// unit of deg per second  ; deg가 아니라 count 값
	cmd_alt_vel = d_alt;	// unit of deg per second  ; deg가 아니라 count 값
	sum_azi_vel = cmd_azi_vel + cal_azi_vel;
	sum_alt_vel = cmd_alt_vel + cal_alt_vel;
	
// calculate the velocity to chase the target 

	if(sum_azi_vel>0)
	{
		if(sum_azi_vel > 3*AZI_VEL_MAX) 											 {req_azi_vel = AZI_VEL_MAX;		is_track = 1; go_const('A', req_azi_vel);}
		else if((100*cal_azi_vel < sum_azi_vel) && (sum_azi_vel < 3*AZI_VEL_MAX))    {req_azi_vel = 0.3*sum_azi_vel;	is_track = 2; go_const('A', req_azi_vel);}
		else if((10*cal_azi_vel < sum_azi_vel) && (sum_azi_vel < 100*cal_azi_vel))   {req_azi_vel = 0.2*sum_azi_vel;	is_track = 3; go_const('A', req_azi_vel);}
		else if((cal_azi_vel < sum_azi_vel) && (sum_azi_vel < 10*cal_azi_vel))	     {req_azi_vel = 0.2*sum_azi_vel; 	is_track = 0; go_const('A', req_azi_vel);}
		else if(									sum_azi_vel < cal_azi_vel)	     {req_azi_vel = 0.;				 	is_track = 0; go_const('A', req_azi_vel);}

	}
	
	else if(sum_azi_vel<0)
	{
		if(fabs(sum_azi_vel) > 3*AZI_VEL_MAX) 												 	  {req_azi_vel = -AZI_VEL_MAX;   	 is_track = 1; go_const('A', req_azi_vel);}
		else if((100*cal_azi_vel < fabs(sum_azi_vel)) && (fabs(sum_azi_vel) < 3*AZI_VEL_MAX))	  {req_azi_vel = 0.3*sum_azi_vel;  	 is_track = 2; go_const('A', req_azi_vel);} 
		else if((10*cal_azi_vel < fabs(sum_azi_vel)) && (fabs(sum_azi_vel) < 100*cal_azi_vel)) 	  {req_azi_vel = 0.2*sum_azi_vel;    is_track = 3; go_const('A', req_azi_vel);}
		else if((cal_azi_vel < fabs(sum_azi_vel)) && (fabs(sum_azi_vel) < 10*cal_azi_vel)) 	 	  {req_azi_vel = 0.1*sum_azi_vel;    is_track = 0; go_const('A', req_azi_vel);}
		else if(										  fabs(sum_azi_vel) < cal_azi_vel) 	 	  {req_azi_vel = 0.;			     is_track = 0; go_const('A', req_azi_vel);}		
		
	}																												  
	
	if(sum_alt_vel>0)
	{
		if(sum_alt_vel > 3*ALT_VEL_MAX) 											   {req_alt_vel = ALT_VEL_MAX;		is_track = 1; go_const('B', req_alt_vel);}
		else if((1000*cal_alt_vel < sum_alt_vel) && (sum_alt_vel < 3*ALT_VEL_MAX))     {req_alt_vel = 0.3*sum_alt_vel;	is_track = 2; go_const('B', req_alt_vel);}
		else if((100*cal_alt_vel < sum_alt_vel) && (sum_alt_vel < 1000*cal_alt_vel))   {req_alt_vel = 0.2*sum_alt_vel;	is_track = 3; go_const('B', req_alt_vel);}
		else if((cal_alt_vel < sum_alt_vel) && (sum_alt_vel < 100*cal_alt_vel))	       {req_alt_vel = 0.2*sum_alt_vel; 	is_track = 0; go_const('B', req_alt_vel);}
		else if(									sum_alt_vel < cal_alt_vel)	       {req_alt_vel = 0.;				is_track = 0; go_const('B', req_alt_vel);}

	}
	
	else if(sum_alt_vel<0)
	{
		if(fabs(sum_alt_vel) > 3*ALT_VEL_MAX) 												 	  	  {req_alt_vel = -ALT_VEL_MAX;   	 is_track = 1; go_const('B', req_alt_vel);}
		else if((1000*cal_alt_vel < fabs(sum_alt_vel)) && (fabs(sum_alt_vel) < 3*ALT_VEL_MAX))	  	  {req_alt_vel = 0.3*sum_alt_vel;  	 is_track = 2; go_const('B', req_alt_vel);} 
		else if((100*cal_alt_vel < fabs(sum_alt_vel)) && (fabs(sum_alt_vel) < 1000*cal_alt_vel))      {req_alt_vel = 0.2*sum_alt_vel;    is_track = 3; go_const('B', req_alt_vel);}
		else if((cal_alt_vel < fabs(sum_alt_vel)) && (fabs(sum_alt_vel) < 100*cal_alt_vel)) 	 	  {req_alt_vel = 0.1*sum_alt_vel;    is_track = 0; go_const('B', req_alt_vel);}
		else if(										  fabs(sum_alt_vel) < cal_alt_vel) 	 	  	  {req_alt_vel = 0.;			     is_track = 0; go_const('B', req_alt_vel);}		
		
	}

//	slew_stop2('A', d_azi, req_azi_vel);			   // dAccel, dDecel 이건 어떻게 해야 하지?? 그냥 그대로 써
//	go_const('A', req_azi_vel);
//req_azi_vel = 50. ;
//go_const('A', req_azi_vel);
//	Delay(0.1);
//  slew_stop2('B', d_alt, req_alt_vel);
//	Delay(0.1);
//	go_const('B', req_alt_vel);
	
//	sprintf(GBUFF, "now_azi = %d, cmd_azi = %lf, now_alt = %d, cmd_alt = %lf \n", now_azi, cmd_azi[1], now_alt, cmd_alt[1]);
//	sprintf(GBUFF, "now_azi = %d, cmd_azi = %lf, d_azi = %lf, sum_azi_vel = %lf, req_azi_vel = %lf \n", now_azi, cmd_azi[1], d_azi, sum_azi_vel, req_azi_vel);
	sprintf(GBUFF, "now_alt = %d, cmd_alt = %lf, d_alt = %lf, sum_alt_vel = %lf, req_alt_vel = %lf \n", now_alt, cmd_alt[1], d_alt, cmd_alt_vel, req_alt_vel);
	display_message(GBUFF);
		
	return is_track;
}
//----------------------------------------------------------------------------------------------------------------
int calpos_radec(h_m_s ra_2000, d_m_s dec_2000, double *step_azi, double *step_alt)
//----------------------------------------------------------------------------------------------------------------
{
	int iii;
	double interv_sec[3], jd_ut1, jd_ut;
	h_m_s last, ap_ha;
	d_m_s ap_dec, alt, azi;
	char GBUFF[500] ;
	
//    jd_ut = get_jdutc_clock();
	
	interv_sec[0] = -1.; interv_sec[1] = 0.; interv_sec[2] = 1.;	// 시간간격 1초씩
	
	for(iii=0;iii<3;iii++)
	{
		
		//		Azimuth
		
		jd_ut1 = get_jdutc_clock2(interv_sec[iii]) ;
		
//		sprintf(GBUFF, "number is %d \n", kk); display_message(GBUFF);
				  
		get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad, 1,
				  &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad, &azi.rad);
		
		if(azi.rad > PI314) azi.rad -= TWOPI;
		if(azi.rad < -PI314) azi.rad += TWOPI;
		
		azi.rad += off_azi ;
		alt.rad += off_alt ;
		
		step_azi[iii] = calc_ha_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_HA_COUNT;			// guide mode에서 offazi를 읽을 때. 그리고 MOUNT.INP에서 MOUNT_HA[20]을 읽어서 사용. 이건 나중에 정리하자.
		
		if(ENCODER_MODE==1) step_azi[iii] = step_azi[iii] * ENC_TO_STEP[0];		// ENC_TO_STEP[2] 이것도 MOUNT.INP에서 읽어오는 것.
		
//		Altitude
//		get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad, 1,
//				  &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad, &azi.rad);
		
		step_alt[iii] = calc_dec_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_DEC_COUNT;	// guide mode에서 offdec를 읽을 때. 그리고 MOUNT.INP에서 MOUNT_DEC[20]을 읽어서 사용.
		
		if(ENCODER_MODE==1) step_alt[iii] = step_alt[iii] * ENC_TO_STEP[1];
		
//		sprintf(GBUFF, "ENC_TO_STEP[0] = %lf, ENC_TO_STEP[1] = %lf \n", ENC_TO_STEP[0], ENC_TO_STEP[1]);
//		display_message(GBUFF);

		if( rad_to_deg(alt.rad) < 10)
		{
//			Beap();
//			STOP_TELESCOPE();
			return -1;
		}

	}
		
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------
// slew_to_altazi
/*
int slew_track_altazi(d_m_s target_alt, d_m_s target_azi)
//-----------------------------------------------------------------------------------------------------------------------------------
{
	double cmd_azi, cmd_alt;
	long now_azi, now_alt;
	double d_azi, 
		   d_alt;
	double cmd_azi_vel, cmd_alt_vel;
//	double cal_azi_vel, cal_alt_vel;
	double req_azi_vel, req_alt_vel;
//	double d_azioff, d_altoff;
	int is_track=0;
	
//	d_m_s target_alt, target_azi;
	
//	target_alt.rad =53514.5;
//	target_azi.rad =-52230.7;
	rad_to_dms( &target_alt);
	rad_to_dms( &target_azi);
	
// read the current position of the telescope
	if(ENCODER_MODE==0)
		{read_counter('A', &now_azi); read_counter('B', &now_alt);}
	else
		{read_encoder('A', &now_azi); read_encoder('B', &now_alt);}

// calculate the current position of target with -1sec, 0sec, 1sec
	calpos_altazi(target_alt, target_azi, &cmd_azi, &cmd_alt);
	
	d_azi = cmd_azi - now_azi + 0.5;	// 0.5를 더한 것은 tel_sub.c의 slew_to_radec 함수에서 보정한 값. 왜 이 값을 더하는지는 모르겠음.
	d_alt = cmd_alt - now_alt + 0.5;
																													  
//Velocity routine  ( plus / minus have to be concerned)
//	cal_azi_vel = *cmd_azi[2] - *cmd_azi[1];	
//	cal_alt_vel = *cmd_alt[2] - *cmd_alt[1];
	cmd_azi_vel = d_azi;	// unit of deg per second  ; deg가 아니라 count 값
	cmd_alt_vel = d_alt;	// unit of deg per second  ; deg가 아니라 count 값

		if(d_azi>0)
	{
		if(cmd_azi_vel > 3*AZI_VEL_MAX) cmd_azi_vel=AZI_VEL_MAX;
		else if(AZI_VEL_MAX < cmd_azi_vel < 3*AZI_VEL_MAX) cmd_azi_vel*=0.7;
		else if(SETTLING_VEL1 < cmd_azi_vel < AZI_VEL_MAX) cmd_azi_vel*=0.5;		
		else if(SETTLING_VEL2 < cmd_azi_vel < SETTLING_VEL1) cmd_azi_vel*=0.3;
		else if(TOLERANCE_VEL < cmd_azi_vel < SETTLING_VEL2) cmd_azi_vel*=0.2;
		else if(cmd_azi_vel < TOLERANCE_VEL)    
		{
			cmd_azi_vel*=0.01;
			is_track++;
		}
	
		req_azi_vel=cal_azi_vel+cmd_azi_vel;
	
	}
	if(d_azi<0)
	{
		if(fabs(cmd_azi_vel) > 3*AZI_VEL_MAX) cmd_azi_vel=-AZI_VEL_MAX;
		else if(AZI_VEL_MAX < fabs(cmd_azi_vel) < 3*AZI_VEL_MAX) cmd_azi_vel*=0.7;
		else if(SETTLING_VEL1 < fabs(cmd_azi_vel) < AZI_VEL_MAX) cmd_azi_vel*=0.5;		
		else if(SETTLING_VEL2 < fabs(cmd_azi_vel) < SETTLING_VEL1) cmd_azi_vel*=0.3;
		else if(TOLERANCE_VEL < fabs(cmd_azi_vel) < SETTLING_VEL2) cmd_azi_vel*=0.2;
		else if(fabs(cmd_azi_vel) < TOLERANCE_VEL)    
		{
			cmd_azi_vel*=0.01;
			is_track++;
		}
	
		req_azi_vel=cal_azi_vel+cmd_azi_vel;	
	}	
	if(d_alt>0)
	{
		if(cmd_alt_vel > 3*ALT_VEL_MAX) cmd_alt_vel=ALT_VEL_MAX;
		else if(ALT_VEL_MAX < cmd_alt_vel < 3*ALT_VEL_MAX) cmd_alt_vel*=0.7;
		else if(SETTLING_VEL1 < cmd_alt_vel < ALT_VEL_MAX) cmd_alt_vel*=0.5;		
		else if(SETTLING_VEL2 < cmd_alt_vel < SETTLING_VEL1) cmd_alt_vel*=0.3;
		else if(TOLERANCE_VEL < cmd_alt_vel < SETTLING_VEL2) cmd_alt_vel*=0.2;
		else if(cmd_azi_vel < TOLERANCE_VEL)    
		{
			cmd_alt_vel*=0.01;
			is_track++;
		}
	
		req_alt_vel=cal_alt_vel+cmd_alt_vel ;
	}
	if(d_alt<0)
	{
		if(fabs(cmd_alt_vel) > 3*ALT_VEL_MAX) cmd_alt_vel=-ALT_VEL_MAX;
		else if(ALT_VEL_MAX < fabs(cmd_alt_vel) < 3*ALT_VEL_MAX) cmd_alt_vel*=0.7;
		else if(SETTLING_VEL1 < fabs(cmd_alt_vel) < ALT_VEL_MAX) cmd_alt_vel*=0.5;		
		else if(SETTLING_VEL2 < fabs(cmd_alt_vel) < SETTLING_VEL1) cmd_alt_vel*=0.3;
		else if(TOLERANCE_VEL < fabs(cmd_alt_vel) < SETTLING_VEL2) cmd_alt_vel*=0.2;
		else if(fabs(cmd_azi_vel) < TOLERANCE_VEL)    
		{
			cmd_alt_vel*=0.01;
			is_track++;
		}
	
		req_alt_vel=cal_alt_vel+cmd_alt_vel ;
	}
										   
	slew_stop2('A', d_azi, req_azi_vel);			   // dAccel, dDecel 이건 어떻게 해야 하지?? 그냥 그대로 써
//	Delay(0.1);
    slew_stop2('B', d_alt, req_alt_vel);
//	Delay(0.1);	
		
	return 0;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
int calpos_altazi(d_m_s alt, d_m_s azi, double *step_azi, double *step_alt)
//-----------------------------------------------------------------------------------------------------------------------
{
	h_m_s ap_ha;
	d_m_s ap_dec;
	
	if(rad_to_deg(alt.rad) < 10.)
	{
		Beep();
		if( ConfirmPopup("고도가 너무 낮습니다", "돌아갈까요?") == 1) return -1;
	}
					
//	Azimuth
	altazi_to_hadec(alt.rad, azi.rad, SITE_LATI.rad, &ap_ha.rad, &ap_dec.rad);
//	if(azi.rad > PI314) azi.rad -= TWOPI;
//	if(azi.rad < -PI314) azi.rad += TWOPI;
	*step_azi = calc_ha_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_HA_COUNT;			// guide mode에서 offazi를 읽을 때. 그리고 MOUNT.INP에서 MOUNT_HA[20]을 읽어서 사용. 이건 나중에 정리하자.
	if(ENCODER_MODE==1) *step_azi = *step_azi * ENC_TO_STEP[0];		// ENC_TO_STEP[2] 이것도 MOUNT.INP에서 읽어오는 것.
		
//	Altitude
	*step_alt = calc_dec_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_DEC_COUNT;	// guide mode에서 offdec를 읽을 때. 그리고 MOUNT.INP에서 MOUNT_DEC[20]을 읽어서 사용.
	if(ENCODER_MODE==1) *step_alt = *step_alt * ENC_TO_STEP[1];
	
	if( rad_to_deg(alt.rad) < 10)
	{
//		Beap();
//		STOP_TELESCOPE();
		return -1;
	}	

    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------
int check_slew_track_altazi(h_m_s target_ra, d_m_s target_dec)
//-----------------------------------------------------------------------------------------------------------------------
{
	double cmd_azi[3]={0.,0.,0.}, cmd_alt[3]={0.,0.,0.};
	double 
	long now_azi, now_alt;
	double d_azi, 
		   d_alt;
	double cmd_azi_vel, cmd_alt_vel;
	double cal_azi_vel, cal_alt_vel;
	double req_azi_vel=0., req_alt_vel=0.;
	double d_azioff, d_altoff;
	int is_track=0;
//	h_m_s target_ra ;
//	d_m_s target_dec ;


//	target_ra.rad = 53514.5;
//	target_dec.rad = -52230.7;
	rad_to_hms( &target_ra);
	rad_to_dms( &target_dec);
	
// read the current position of the telescope
	if(ENCODER_MODE==0)
		{read_counter('A', &now_azi); read_counter('B', &now_alt);}
	else
		{read_encoder('A', &now_azi); read_encoder('B', &now_alt);}

// calculate the current position of target with -1sec, 0sec, 1sec
	calpos_radec(target_ra, target_dec, cmd_azi, cmd_alt);			  // cmd_azi, cmd_azi는 cmd_azi[3], cmd_alt[3] 배열의 1번째 요소 어드레스이다. 
	
	d_azi = cmd_azi[1] - now_azi + 0.5;	// 0.5를 더한 것은 tel_sub.c의 slew_to_radec 함수에서 보정한 값. 왜 이 값을 더하는지는 모르겠음.	 // cmd_azi[1]은 배열 첫번째 요소 값이다.
	d_alt = cmd_alt[1] - now_alt + 0.5; // cmd_azi[1]은 배열 첫번째 요소 값이다. cmd_azi[]라는 배열에 괄호가 붙으면 값을, 그냥 cmd_azi, cmd_azi+1, cmd_azi+2 이런 식이면 이건 포인터를 나타내는 것.
																													  
//Velocity routine  ( plus / minus have to be concerned)
	cal_azi_vel = cmd_azi[2] - cmd_azi[1];	
	cal_alt_vel = cmd_alt[2] - cmd_alt[1];
	cmd_azi_vel = d_azi;	// unit of deg per second  ; deg가 아니라 count 값
	cmd_alt_vel = d_alt;	// unit of deg per second  ; deg가 아니라 count 값

	if(d_azi>0)
	{
		if(cmd_azi_vel > 3*AZI_VEL_MAX) cmd_azi_vel=AZI_VEL_MAX;
		else if(AZI_VEL_MAX < cmd_azi_vel < 3*AZI_VEL_MAX) cmd_azi_vel*=0.7;
		else if(SETTLING_VEL1 < cmd_azi_vel < AZI_VEL_MAX) cmd_azi_vel*=0.5;		
		else if(TOLERANCE_VEL < cmd_azi_vel < SETTLING_VEL1) cmd_azi_vel*=0.3;
//		else if(TOLERANCE_VEL < cmd_azi_vel < SETTLING_VEL2) cmd_azi_vel*=0.2;
		else if(cmd_azi_vel < TOLERANCE_VEL)    
		{
			cmd_azi_vel*=0.01;
			is_track++;
		}
	
		req_azi_vel=cal_azi_vel+cmd_azi_vel;
	
	}
	if(d_azi<0)
	{
		if(fabs(cmd_azi_vel) > 3*AZI_VEL_MAX) cmd_azi_vel=-AZI_VEL_MAX;
		else if(AZI_VEL_MAX < fabs(cmd_azi_vel) < 3*AZI_VEL_MAX) cmd_azi_vel*=-0.7;
		else if(SETTLING_VEL1 < fabs(cmd_azi_vel) < AZI_VEL_MAX) cmd_azi_vel*=-0.5;		
		else if(TOLERANCE_VEL < fabs(cmd_azi_vel) < SETTLING_VEL1) cmd_azi_vel*=-0.3;
//		else if(TOLERANCE_VEL < fabs(cmd_azi_vel) < SETTLING_VEL2) cmd_azi_vel*=-0.2;
		else if(fabs(cmd_azi_vel) < TOLERANCE_VEL)    
		{
			cmd_azi_vel*=-0.01;
			is_track++;
		}
	
		req_azi_vel=cal_azi_vel+cmd_azi_vel;	
	}	
	if(d_alt>0)
	{
		if(cmd_alt_vel > 3*ALT_VEL_MAX) cmd_alt_vel=ALT_VEL_MAX;
		else if(ALT_VEL_MAX < cmd_alt_vel < 3*ALT_VEL_MAX) cmd_alt_vel*=0.7;
		else if(SETTLING_VEL1 < cmd_alt_vel < ALT_VEL_MAX) cmd_alt_vel*=0.5;		
		else if(TOLERANCE_VEL < cmd_alt_vel < SETTLING_VEL1) cmd_alt_vel*=0.3;
//		else if(TOLERANCE_VEL < cmd_alt_vel < SETTLING_VEL2) cmd_alt_vel*=0.2;
		else if(cmd_azi_vel < TOLERANCE_VEL)    
		{
			cmd_alt_vel*=0.01;
			is_track++;
		}
	
		req_alt_vel=cal_alt_vel+cmd_alt_vel ;
	}
	if(d_alt<0)
	{
		if(fabs(cmd_alt_vel) > 3*ALT_VEL_MAX) cmd_alt_vel=-ALT_VEL_MAX;
		else if(ALT_VEL_MAX < fabs(cmd_alt_vel) < 3*ALT_VEL_MAX) cmd_alt_vel*=-0.7;
		else if(SETTLING_VEL1 < fabs(cmd_alt_vel) < ALT_VEL_MAX) cmd_alt_vel*=-0.5;		
		else if(TOLERANCE_VEL < fabs(cmd_alt_vel) < SETTLING_VEL1) cmd_alt_vel*=-0.3;
//		else if(TOLERANCE_VEL < fabs(cmd_alt_vel) < SETTLING_VEL2) cmd_alt_vel*=-0.2;
		else if(fabs(cmd_azi_vel) < TOLERANCE_VEL)    
		{
			cmd_alt_vel*=-0.01;
			is_track++;
		}
	
		req_alt_vel=cal_alt_vel+cmd_alt_vel ;
	}
		
	slew_stop2('A', d_azi, req_azi_vel);			   // dAccel, dDecel 이건 어떻게 해야 하지?? 그냥 그대로 써
//	Delay(0.1);
    slew_stop2('B', d_alt, req_alt_vel);
//	Delay(0.1);
		
	return 0;
}
//----------------------------------------------------------------------------------------------------------------

