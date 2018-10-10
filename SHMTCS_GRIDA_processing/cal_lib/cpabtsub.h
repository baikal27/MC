#ifndef TYPE_HMS
typedef struct H_M_S
        {int sign; int h; int m; double s; double rh; double rad;} h_m_s;
typedef struct D_M_S{
	int sign; int d; int m; double s; double rd; double rad;} d_m_s;
#define TYPE_HMS	
#endif	

//------------   Prototyping of Functions in this File  -------------------
void   get_alt_azi_psi(double ra_2000, double dec_2000, double jd_ut1, double longi, double lati, int flag, double *az,double *alt, double *psi);
void   get_aposi(double ra_2000, double dec_2000, double jd_ut1, double longi,double lati, int flag,
                 double *last, double *ap_ha, double *ap_dec, double *ap_alt, double *ap_az);
void   get_apradec(double ra_2000, double dec_2000, double jd, double *ra_app, double *dec_app);
void   equtohor(double ra, double dec,double last,double phi, double *az,double *alt);
void   hortoequ(double az, double alt, double last, double phi, double *ra, double *dec);
void   altazi_to_hadec(double alt, double azi, double phi, double *ha, double *dec);
void   hadec_to_altazi(double ha, double dec, double phi, double *alt,double *azi);
//void   Hadec_to_altazi(double ha, double dec, double phi, double *alt,double *azi);

void   equtoecl(double ra, double dec, double obl, double *lamda, double *beta);
void   ecltoequ(double lamda,double beta,double obl, double *ra, double *dec);

void   gctotc(double *ra, double *dec, double *dist, double stime, double lat, double h);
double funha(double ra, double last);
void   getxo(double longi,double lat, double h, double xo[],double *latgc);

int correct_rv(double jd, double longi, double lati, double height,
               double rarad, double decrad, double *jd_bc, double *rv_obs);

//------------     Annual Aberration and Refgraction         ----------------
void   precess(double jd, double x0[], double xt[]);
void   nutate(double jd, double x0[], double x[]);
void   aberration(double jd, double *RA, double *Decl);
void   anuaber(double jd, double p[]);
void   refract(double *alt);
double funmobl(double jd);
void   nutconst(double jd, double *delphi, double *deleps, double *eps);
void   SunLonAndEcc(double T, double *Lon, double *e);

void inverse_precess(double jd, double xt[], double x0[]);
void inverse_nutate (double jd, double x[],  double x0[]);

int  get_pi_factor(double jd_ut, double RA, double Dec, double *pi_alp, double *pi_dec);
int  get_earth_pos_apr(double jd_ut, double *XI, double *YI, double *ZI);
int  get_planet_position_apr(int nbody, double jd_ut, double *ra_2000, double *de_2000, double *disti);
void get_earth_vel_apr(double jd, double v[]);
void adjust_ra(double ra[], int ndata);

double adeltat(double jd);

//------------ Routines to convert various types of time --------------------
double get_jd(int year,int month,int day, double rhour);
void   jdtoc(double jd, int flag, int *year, int *month, int *day, double *rhour);
double funtdb(double tdt);
double jdtogmst(double jd);
double jdtogast(double jd);
double jdtolmst(double jd, double longi);
double jdtolast(double jd, double longi);
double tdttokst(double tdt);

double get_jdutc_clock(void);
double get_jdutc_clock2(double interv_sec);
void   read_computer_time(int *year, int *month, int *day, int *hour, int *minute ,int *second, int *hsecond);


