#ifndef _CAL_SUB_H
#define _CAL_SUB_H
double calc_ha_count (double ha, double dec,double azi, double alt);
double calc_dec_count(double ha, double dec,double azi, double alt);
double calc_der_count(double psi);
int  calc_radec(double *ra, double *dec);
int  calc_hadec(double *ha, double *dec);
int  calc_position1(long count_ha, long count_de, double *ha, double *dec);

int  cal_mount_model_radec (int np_ra, int np_dec, int encoder_mode);
int  cal_mount_model_altazi(int np_ra, int np_dec, int encoder_mode);
//int  cal_mount_model_new(int np_ra, int np_dec, int encoder_mode);

void calc_error_count_radec (h_m_s ra, d_m_s dec, double *error_ha, double *error_dec);
void calc_error_count_altazi(h_m_s ra, d_m_s dec, double *error_azi, double *error_alt);
int  read_mount_model(char file_name[]);
int  calc_mean_tracking_freq(h_m_s ra_2000, d_m_s dec_2000);

double speed_to_freq(char caxis, double speed);
double freq_to_speed(char caxis, double freq );
#endif
