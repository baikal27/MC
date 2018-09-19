typedef struct H_M_S
        {int sign; int h; int m; double s; double rh; double rad;} h_m_s;
typedef struct D_M_S{
	int sign; int d; int m; double s; double rd; double rad;} d_m_s;

//------------   Prototyping of Functions in this File  -------------------
extern void   get_alt_azi_psi(double ra_2000, double dec_2000, double jd_ut1, double longi, double lati, int flag, double *azi, double *alt, double *psi);
extern void   get_aposi(double ra_2000, double dec_2000, double jd_ut1, double longi,double lati, int flag,
                 double *last, double *ap_ha, double *ap_dec, double *ap_alt, double *ap_az);
extern void   get_apradec(double ra_2000, double dec_2000, double jd, double *ra_app, double *dec_app);
extern void   equtohor(double ra, double dec,double last,double phi, double *az,double *alt);
extern void   hortoequ(double az, double alt, double last, double phi, double *ra, double *dec);
extern void   altazi_to_hadec(double alt, double azi, double phi, double *ha, double *dec);
extern void   hadec_to_altazi(double ha, double dec, double phi, double *alt,double *azi);
//extern void   Hadec_to_altazi(double ha, double dec, double phi, double *alt,double *azi);
extern void   equtoecl(double ra, double dec, double obl, double *lamda, double *beta);
extern void   ecltoequ(double lamda,double beta,double obl, double *ra, double *dec);

extern void   gctotc(double *ra, double *dec, double *dist, double stime, double lat, double h);
extern double funha(double ra, double last);
extern void   getxo(double longi,double lat, double h, double xo[],double *latgc);

extern int correct_rv(double jd, double longi, double lati, double height,
                      double rarad, double decrad, double *jd_bc, double *rv_obs);

//------------     Annual Aberration and Refgraction         ----------------
extern void   precess(double jd, double x0[], double xt[]);
extern void   nutate(double jd, double x0[], double x[]);
extern void   aberration(double jd, double *RA, double *Decl);
extern void   anuaber(double jd, double p[]);
extern void   refract(double *alt);
extern double funmobl(double jd);
extern void   nutconst(double jd, double *delphi, double *deleps, double *eps);
extern void   SunLonAndEcc(double T, double *Lon, double *e);

extern void inverse_precess(double jd, double xt[], double x0[]);
extern void inverse_nutate (double jd, double x[],  double x0[]);

extern int  get_earth_pos_apr(double jd_ut, double *XI, double *YI, double *ZI);
extern int  get_planet_position_apr(int nbody, double jd_ut, double *ra_2000, double *de_2000, double *disti);
extern void get_earth_vel_apr(double jd, double v[]);
extern void adjust_ra(double ra[], int ndata);

extern int  get_pi_factor(double jd_ut, double RA, double Dec, double *pi_alp, double *pi_dec);
//------------ Routines to convert various types of time --------------------
extern double get_jd(int year,int month,int day, double rhour);
extern void   jdtoc(double jd, int flag, int *year, int *month, int *day, double *rhour);
extern double funtdb(double tdt);
extern double jdtogmst(double jd);
extern double jdtogast(double jd);
extern double jdtolast(double jd, double longi);
extern double jdtolmst(double jd, double longi);

extern double tdttokst(double tdt);
extern double adeltat(double jd);

extern double get_jdutc_clock(void);
extern void   read_computer_time(int *year, int *month, int *day, int *hour, int *minute ,int *second, int *hsecond);

//------------ Routines to convert various types of angle --------------------
extern void   hms_to_rad(h_m_s *hms);
extern void   dms_to_rad(d_m_s *dms);
extern void   rad_to_hms(h_m_s *hms);
extern void   rad_to_dms(d_m_s *dms);
extern double rad_to_deg(double radian);
extern double deg_to_rad(double degree);

//------------ Routines used for linear algebra        -----------------------
extern void rotx(double THETA, double X[], double XP[]);
extern void roty(double THETA, double X[], double XP[]);
extern void rotz(double THETA, double X[], double XP[]);
extern void stor(double ra, double dec, double x[]);
extern void rtos(double x[], double *ra, double *dec);
extern void cproduct(double a[], double b[], double c[]);
extern void make_unit(double x[]);
extern void vectsub(const double a[], const double b[], double c[]);
extern double funr(double q[]);
extern double dproduct(double a[], double b[]);

//------------ Routines used for Least Squares Solution -----------------------
extern int clsq(double a[],double y[],double q[],double aty[],
                int ndmax,int nd,int npmax,int np,
                double par[],double r[],double *use);
                
extern int clsq_adj(double lambda,double a[],double y[],double q[],double aty[],
         int ndmax,int nd,int npmax,int np,
         double par[],double r[],double *use);
                
extern int csqtinv(double q[], double dum[],int nc,int n);

//-----------------------------------------------------------------------------
extern int  polint(double XA[], double YA[], int N, double X, double *Y, double *DY);
extern void get_stco(double ALPHA0, double DELTA0, double ALPHA, double DELTA,double *XSI, double *ETA);
extern void st_to_sp(double ALPHA0, double DELTA0, double XSI, double ETA, double *ALPHA, double *DELTA);
extern int  linear_fit(double y[], double x[], int ndata, double *slope, double *offset, double res[], double *use);
extern void teat_apradec(double ra_2000, double dec_2000, double jd, double *ra_app, double *dec_app);

extern double find_max(double x[],int ndata);
extern double find_min(double x[],int ndata);
extern double find_mean(double x[],int ndata);
extern double find_mean_sigma(double x[],int ndata, double *mean);
extern double dotproduct(double a[], double b[], int ndata);

extern void   getdhms(double rday, int *day, int *hour, double *minute);
extern double funroot(double f0, double x1, double x2, double facc);
extern double modpi2(double a);
extern double modpi (double a);


