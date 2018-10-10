#ifndef TYPE_HMS
typedef struct H_M_S
        {int sign; int h; int m; double s; double rh; double rad;} h_m_s;
typedef struct D_M_S{
	int sign; int d; int m; double s; double rd; double rad;} d_m_s;
#define TYPE_HMS	
#endif	

//------------ Routines to convert various types of angle --------------------
void   hms_to_rad(h_m_s *hms);
void   dms_to_rad(d_m_s *dms);
void   rad_to_hms(h_m_s *hms);
void   rad_to_dms(d_m_s *dms);
double rad_to_deg(double radian);
double deg_to_rad(double degree);
void   getdhms(double rday, int *day, int *hour, double *minute);

//------------     Routines used for linear algebra    -----------------------
void   stor(double ra, double dec, double x[]);
void   rtos(double x[], double *ra, double *dec);
void   rotx(double THETA, double X[], double XP[]);
void   roty(double THETA, double X[], double XP[]);
void   rotz(double THETA, double X[], double XP[]);
double dproduct(double a[], double b[]);
void   cproduct(double a[], double b[], double c[]);
double dotproduct(double a[], double b[], int ndata);
void   vectsub(const double a[], const double b[], double c[]);
void   make_unit(double x[]);
double funr(double q[]);

//------------ Routines used for Least Squares Solution -----------------------
int clsq(double a[],double y[],double q[],double aty[],
         int ndmax,int nd,int npmax,int np,
         double par[],double r[],double *use);
         
int clsq_adj(double lambda,double a[],double y[],double q[],double aty[],
         int ndmax,int nd,int npmax,int np,
         double par[],double r[],double *use);
int csqtinv(double q[], double dum[],int nc,int n);
int  linear_fit(double y[], double x[], int ndata, double *slope, double *offset, double res[], double *use);
int polint(double XA[], double YA[], int N, double X, double *Y, double *DY);
           double funroot(double f0, double x1, double x2, double facc);
double funroot(double f0, double x1, double x2, double facc);
int sort_double(int ndata, double yinpp[], int indxp[]);
int intsort(int n, int arrin[], int indx[]);

int cubic_spline_interpolate(int n1, double y1[], int df, double y2[]);
int remove_outlier(double x[],int ndata, double range, double *me, double *si, int *nn);
double remove_trend(double epoch[], double rvd[], double para[],int ndata, int npara);


//-----------------   Miscellaneous     ---------------------------------------
double find_max(double x[],int ndata);
double find_min(double x[],int ndata);
double find_mean(double x[],int ndata);
double find_mean_sigma(double x[],int ndata, double *mean);
double modpi2(double a);
double modpi (double a);
//-----------------------------------------------------------------------------
int calc_ccf(double template[], int ntempl, double comp[],int ncomp, double ccf[], int s1, int s2);


