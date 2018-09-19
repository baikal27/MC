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

#define AZI_VEL_MAX	1.0		// Unit of degree per second
#define ALT_VEL_MAX	1.0		// Unit of degree per second
#define SETTLING_VEL	1.0		// Settling range
#define TOLERANCE_VEL	0.001		// Unit of defree

static const double TWOPI = 3.14159265358979323846*2.0;
static const double PI314 = 3.14159265358979323846;
static const double dAccel=400.; // initial value
static const double dDecel=400.; // initial value    


// slew_to_radec  : comparison
int slew_track_radec(h_m_s target_ra, d_m_s target_dec)
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
	calpos_radec(target_ra, target_dec, cmd_azi, cmd_alt);			  // cmd_azi, cmd_azi�� cmd_azi[3], cmd_alt[3] �迭�� 1��° ��� ��巹���̴�. 
	
	d_azi = cmd_azi[1] - now_azi + 0.5;	// 0.5�� ���� ���� tel_sub.c�� slew_to_radec �Լ����� ������ ��. �� �� ���� ���ϴ����� �𸣰���.	 // cmd_azi[1]�� �迭 ù��° ��� ���̴�.
	d_alt = cmd_alt[1] - now_alt + 0.5; // cmd_azi[1]�� �迭 ù��° ��� ���̴�. cmd_azi[]��� �迭�� ��ȣ�� ������ ����, �׳� cmd_azi, cmd_azi+1, cmd_azi+2 �̷� ���̸� �̰� �����͸� ��Ÿ���� ��.
																													  
//Velocity routine  ( plus / minus have to be concerned)
	cal_azi_vel = cmd_azi[2] - cmd_azi[1];	
	cal_alt_vel = cmd_alt[2] - cmd_alt[1];
	cmd_azi_vel = d_azi;	// unit of deg per second  ; deg�� �ƴ϶� count ��
	cmd_alt_vel = d_alt;	// unit of deg per second  ; deg�� �ƴ϶� count ��

	if(d_azi>0)
	{
		if(cmd_azi_vel>AZI_VEL_MAX) cmd_azi_vel=AZI_VEL_MAX;
		if(cmd_azi_vel<SETTLING_VEL) cmd_azi_vel*=0.5;
		if(cmd_azi_vel<TOLERANCE_VEL) cmd_azi_vel*=0.02;
	
		req_azi_vel=cal_azi_vel+cmd_azi_vel;
	
	}
	if(d_azi<0)
	{
		if(fabs(cmd_azi_vel)>AZI_VEL_MAX) cmd_azi_vel=-AZI_VEL_MAX;
		if(fabs(cmd_azi_vel)<SETTLING_VEL) cmd_azi_vel*=-0.5;
		if(fabs(cmd_azi_vel)<TOLERANCE_VEL) cmd_azi_vel*=-0.02;
	
		req_azi_vel=cal_azi_vel+cmd_azi_vel;	
	}	
	if(d_alt>0)
	{
		if(cmd_alt_vel>ALT_VEL_MAX) cmd_alt_vel=ALT_VEL_MAX;
		if(cmd_alt_vel<SETTLING_VEL) cmd_alt_vel*=0.5;
		if(cmd_alt_vel<TOLERANCE_VEL) cmd_alt_vel*=0.02;
	
		req_alt_vel=cal_alt_vel+cmd_alt_vel;
	}
	if(d_alt<0)
	{
		if(fabs(cmd_alt_vel)>ALT_VEL_MAX) cmd_alt_vel=-ALT_VEL_MAX;
		if(fabs(cmd_alt_vel)<SETTLING_VEL) cmd_alt_vel*=-0.5;
		if(fabs(cmd_alt_vel)<TOLERANCE_VEL) cmd_alt_vel*=-0.02;
	
		req_alt_vel=cal_alt_vel+cmd_alt_vel;
	}
		
	slew_stop2("A", d_azi, req_azi_vel);			   // dAccel, dDecel �̰� ��� �ؾ� ����?? �׳� �״�� ��
	Delay(0.1);
    slew_stop2("B", d_alt, req_alt_vel);
	Delay(0.1);
		
	return 0;
}

int calpos_radec(h_m_s ra_2000, d_m_s dec_2000, double *step_azi, double *step_alt)
{
	int iii;
	double interv_sec[3], jd_ut1, jd_ut;
	h_m_s last, ap_ha;
	d_m_s ap_dec, alt, azi;
	
    jd_ut = get_jdutc_clock();
	
	interv_sec[0] = -1.; interv_sec[1] = 0.; interv_sec[2] = 1.;	// �ð����� 1�ʾ�
		
	for(iii=0;iii<3;iii++)
	{
//		Azimuth
		jd_ut1 = get_jdutc_clock2(interv_sec[iii]) ;
				  
		get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad, 1,
				  &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad, &azi.rad);
		
		if(azi.rad > PI314) azi.rad -= TWOPI;
		if(azi.rad < -PI314) azi.rad += TWOPI;
		
		step_azi[iii] = calc_ha_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_HA_COUNT;			// guide mode���� offazi�� ���� ��. �׸��� MOUNT.INP���� MOUNT_HA[20]�� �о ���. �̰� ���߿� ��������.
		
		if(ENCODER_MODE==1) step_azi[iii] = step_azi[iii] * ENC_TO_STEP[0];		// ENC_TO_STEP[2] �̰͵� MOUNT.INP���� �о���� ��.
		
//		Altitude
//		get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad, 1,
//				  &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad, &azi.rad);
		
		step_alt[iii] = calc_dec_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_DEC_COUNT;	// guide mode���� offdec�� ���� ��. �׸��� MOUNT.INP���� MOUNT_DEC[20]�� �о ���.
		
		if(ENCODER_MODE==1) step_alt[iii] = step_alt[iii] * ENC_TO_STEP[1];
		
		if( rad_to_deg(alt.rad) < 10)
		{
//			Beap();
//			STOP_TELESCOPE();
			return -1;
		}
	}

	return 0;
	
}

// slew_to_altazi
int slew_track_altazi(d_m_s target_alt, d_m_s target_azi)
{
	double cmd_azi, cmd_alt;
	long now_azi, now_alt;
	double d_azi, 
		   d_alt;
	double cmd_azi_vel, cmd_alt_vel;
//	double cal_azi_vel, cal_alt_vel;
	double req_azi_vel, req_alt_vel;
//	double d_azioff, d_altoff;
	
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
	
	d_azi = cmd_azi - now_azi + 0.5;	// 0.5�� ���� ���� tel_sub.c�� slew_to_radec �Լ����� ������ ��. �� �� ���� ���ϴ����� �𸣰���.
	d_alt = cmd_alt - now_alt + 0.5;
																													  
//Velocity routine  ( plus / minus have to be concerned)
//	cal_azi_vel = *cmd_azi[2] - *cmd_azi[1];	
//	cal_alt_vel = *cmd_alt[2] - *cmd_alt[1];
	cmd_azi_vel = d_azi;	// unit of deg per second  ; deg�� �ƴ϶� count ��
	cmd_alt_vel = d_alt;	// unit of deg per second  ; deg�� �ƴ϶� count ��

	if(d_azi>0)
	{
		if(cmd_azi_vel>AZI_VEL_MAX) cmd_azi_vel=AZI_VEL_MAX;
		if(cmd_azi_vel<SETTLING_VEL) cmd_azi_vel*=0.5;
		if(cmd_azi_vel<TOLERANCE_VEL) cmd_azi_vel*=0.02;
	
//		req_azi_vel=cal_azi_vel+cmd_azi_vel;
		req_azi_vel=cmd_azi_vel;	
	}
	if(d_azi<0)
	{
		if(fabs(cmd_azi_vel)>AZI_VEL_MAX) cmd_azi_vel=-AZI_VEL_MAX;
		if(fabs(cmd_azi_vel)<SETTLING_VEL) cmd_azi_vel*=-0.5;
		if(fabs(cmd_azi_vel)<TOLERANCE_VEL) cmd_azi_vel*=-0.02;
	
//		req_azi_vel=cal_azi_vel+cmd_azi_vel;	
		req_azi_vel=cmd_azi_vel;
	}	
	if(d_alt>0)
	{
		if(cmd_alt_vel>ALT_VEL_MAX) cmd_alt_vel=ALT_VEL_MAX;
		if(cmd_alt_vel<SETTLING_VEL) cmd_alt_vel*=0.5;
		if(cmd_alt_vel<TOLERANCE_VEL) cmd_alt_vel*=0.02;
	
//		req_alt_vel=cal_alt_vel+cmd_alt_vel;
		req_alt_vel=cmd_alt_vel;
	}
	if(d_azi<0)
	{
		if(fabs(cmd_alt_vel)>ALT_VEL_MAX) cmd_alt_vel=-ALT_VEL_MAX;
		if(fabs(cmd_alt_vel)<SETTLING_VEL) cmd_alt_vel*=-0.5;
		if(fabs(cmd_alt_vel)<TOLERANCE_VEL) cmd_alt_vel*=-0.02;
	
//		req_alt_vel=cal_alt_vel+cmd_alt_vel;
		req_alt_vel=cmd_alt_vel;
	}
										   
	slew_stop2("A", d_azi, req_azi_vel);			   // dAccel, dDecel �̰� ��� �ؾ� ����?? �׳� �״�� ��
	Delay(0.1);
    slew_stop2("B", d_alt, req_alt_vel);
	Delay(0.1);	
		
	return 0;
}
		
		
int calpos_altazi(d_m_s alt, d_m_s azi, double *step_azi, double *step_alt)
{
	h_m_s ap_ha;
	d_m_s ap_dec;
	
	if(rad_to_deg(alt.rad) < 10.)
	{
		Beep();
		if( ConfirmPopup("���� �ʹ� �����ϴ�", "���ư����?") == 1) return -1;
	}
					
//	Azimuth
	altazi_to_hadec(alt.rad, azi.rad, SITE_LATI.rad, &ap_ha.rad, &ap_dec.rad);
//	if(azi.rad > PI314) azi.rad -= TWOPI;
//	if(azi.rad < -PI314) azi.rad += TWOPI;
	*step_azi = calc_ha_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_HA_COUNT;			// guide mode���� offazi�� ���� ��. �׸��� MOUNT.INP���� MOUNT_HA[20]�� �о ���. �̰� ���߿� ��������.
	if(ENCODER_MODE==1) *step_azi = *step_azi * ENC_TO_STEP[0];		// ENC_TO_STEP[2] �̰͵� MOUNT.INP���� �о���� ��.
		
//	Altitude
	*step_alt = calc_dec_count(ap_ha.rad, ap_dec.rad, azi.rad, alt.rad) + OFFSET_DEC_COUNT;	// guide mode���� offdec�� ���� ��. �׸��� MOUNT.INP���� MOUNT_DEC[20]�� �о ���.
	if(ENCODER_MODE==1) *step_alt = *step_alt * ENC_TO_STEP[1];
	
	if( rad_to_deg(alt.rad) < 10)
	{
//		Beap();
//		STOP_TELESCOPE();
		return -1;
	}	

    return 0;
}
