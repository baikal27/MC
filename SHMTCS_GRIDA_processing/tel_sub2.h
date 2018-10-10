#ifndef _TEL_SUB2_H
#define _TEL_SUB2_H

int slew_track_radec(h_m_s target_ra, d_m_s target_dec);
int slew_track_altazi(d_m_s target_alt, d_m_s target_azi);
int calpos_radec(h_m_s ra_2000, d_m_s dec_2000, double *step_azi, double *step_alt);
int calpos_altazi(d_m_s alt, d_m_s azi, double *step_azi, double *step_alt);
int check_slew_track_altazi(h_m_s target_ra, d_m_s target_dec);  // TELESCOPE_TYPE == _ALT_AZI
//int check_slew_track_radec(void);   // TELESCOPE_TYPE == _RA_DEC

#endif
