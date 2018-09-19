int read_telescope_parameter(void);
int init_telescope_parameter(void);
int read_tcs_parameter(void);
void copy_tel_para(void);

int slew_to_radec (h_m_s ra_2000, d_m_s dec_2000);
//int slew_to_hadec (h_m_s ap_ha  , d_m_s ap_dec  );
//int slew_to_altazi(d_m_s alt, d_m_s azi);
int slew_to_home(void);

int correct_servo_ra (h_m_s ra_2000, d_m_s dec_2000, long *count_ha ,double *error_ha ,double *track_speed);
int correct_servo_dec(h_m_s ra_2000, d_m_s dec_2000, long *count_dec,double *error_dec);
int correct_servo_alt(h_m_s ra_2000, d_m_s dec_2000, long *count_ha ,double *error_ha ,double *track_speed);
int correct_servo_azi(h_m_s ra_2000, d_m_s dec_2000, long *count_dec,double *error_dec,double *track_speed);

int park_ra (int dir);
int park_dec(int dir);
int park_der(int dir);
int get_parking_status(void);

int do_guide_hp(void);
int do_guide_ew(hp_status hp);
int do_guide_ns(hp_status hp);

//extern int read_vprofile(void);


