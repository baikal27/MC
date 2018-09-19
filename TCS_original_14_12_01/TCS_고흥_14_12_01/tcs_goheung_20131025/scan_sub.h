extern int make_scan_file   (double lmag, int catoption);
extern int find_equator_star(double lmag, double dec_min, double dec_max);
extern int show_scan_lists   (int handle,int control);
extern int get_star_coordiantes(int nstar, double *ra, double *dec);
extern double find_nearby_star(h_m_s ra_target, d_m_s dec_target,double lmag,h_m_s *ra_near, d_m_s *dec_near);

