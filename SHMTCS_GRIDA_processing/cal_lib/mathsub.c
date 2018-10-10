#include <analysis.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "mathsub.h"

static double  PI314           = 3.14159265358979323846;
static double  TWOPI           = 3.14159265358979323846*2.0;
static double  FACT_DEG_TO_RAD = 3.14159265358979323846/180.0;
static double  FACT_RAD_TO_DEG = 180.0/3.14159265358979323846;

//-------------------------------------------------------------------------------
void hms_to_rad(h_m_s *hms)
//-------------------------------------------------------------------------------
//  This function calculates the rad and rh part of TIME_TYPE_DATA from 
//  the h, m, s part of TIME_TYPE_DATA, which is struct h_m_s type variable.
//-------------------------------------------------------------------------------
{
        if( (*hms).h < 0 || (*hms).m < 0 || (*hms).s < 0.0 )
	      {(*hms).rh  = -( abs((*hms).h) + abs((*hms).m)/60.0
                            + fabs((*hms).s)/3600.0);
          }
        else
           (*hms).rh  =  (*hms).h + (*hms).m/60.0 + (*hms).s/3600.0;

        (*hms).rad =  (*hms).rh*15.0*FACT_DEG_TO_RAD;
}
//-------------------------------------------------------------------------------
void dms_to_rad(d_m_s *dms)
//-------------------------------------------------------------------------------
//  This function calculates the rad and rd part of DEG_TYPE_DATA from 
//  the d, m, s part of DEG_TYPE_DATA, which is struct d_m_s type variable.
//-------------------------------------------------------------------------------
{
   if( (*dms).d < 0 || (*dms).m < 0 || (*dms).s < 0.0 )
		  (*dms).rd  = -(  abs((*dms).d) + abs((*dms).m)/60.0
						+ fabs((*dms).s)/3600.0);
   else
		  (*dms).rd  =  (*dms).d + (*dms).m/60.0 + (*dms).s/3600.0;

		  (*dms).rad =  (*dms).rd*FACT_DEG_TO_RAD;
}
//-------------------------------------------------------------------------------
void rad_to_hms(h_m_s *hms)
//-------------------------------------------------------------------------------
//  This function calculates the h, m, s and rs part of TIME_TYPE_DATA from 
//  the rad part of TIME_TYPE_DATA, which is struct h_m_s type variable.
//-------------------------------------------------------------------------------
{
   double temp;
   (*hms).rh = (*hms).rad*FACT_RAD_TO_DEG/15.0;

   if((*hms).rh < 0) (*hms).rh =  24.0 + (*hms).rh;
   (*hms).h  = floor( (*hms).rh );
   temp      = ( (*hms).rh - (*hms).h*1.0 ) * 60.0;
   (*hms).m  = floor(temp);
   (*hms).s  = ( temp - (*hms).m ) * 60.0;
}
//-------------------------------------------------------------------------------
void rad_to_dms(d_m_s *dms)
//-------------------------------------------------------------------------------
//  This function calculates the d, m, s and rd part of DEG_TYPE_DATA from 
//  the rad part of DEG_TYPE_DATA, which is struct d_m_s type variable.
//-------------------------------------------------------------------------------
{  double temp;

        (*dms).rd = (*dms).rad*FACT_RAD_TO_DEG;
        temp      = (*dms).rd;

        if(temp < 0.0) temp = -temp;
        (*dms).d  = floor(temp);
        temp      = ( temp - (*dms).d ) * 60.0;
        (*dms).m  = floor(temp);
        (*dms).s  = ( temp - (*dms).m ) * 60.0;

        if((*dms).rd < 0.0)
          { if( (*dms).d > 0 )  (*dms).d = -(*dms).d;
            else
             {  if(  (*dms).m > 0 )  (*dms).m = -(*dms).m;
                else                 (*dms).s = -(*dms).s;
	     }
          }
}
//-------------------------------------------------------------------------------
double rad_to_deg(double radian)
//-------------------------------------------------------------------------------
{
	return radian*FACT_RAD_TO_DEG;
}
//-------------------------------------------------------------------------------
double deg_to_rad(double degree)
//-------------------------------------------------------------------------------
{
	return degree*FACT_DEG_TO_RAD;
}
//-------------------------------------------------------------------------------


//--------------------------------------------------------------------------//
void stor(double ra, double dec, double x[])
//-------------------------------------------------------------------------------
{
	x[0] = cos(dec)*cos(ra);
	x[1] = cos(dec)*sin(ra);
	x[2] = sin(dec);
}
//-------------------------------------------------------------------------------
void rtos(double x[], double *ra, double *dec)
//-------------------------------------------------------------------------------
{	double dist;
	*ra  = atan2(x[1], x[0]);
	dist = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
	*dec = asin(x[2]/dist);
}
//-------------------------------------------------------------------------------
void rotx(double theta, double X[], double XP[])
//-------------------------------------------------------------------------------
// To rotate the coordinates axis around x-axis by theta
//  Input : theta, rotation angle in radian
//          x, input vector
// Output : xp, x after the rotation of the coordinates frame.
//-------------------------------------------------------------------------------
{
	XP[0] =  X[0];
	XP[1] =  cos(theta)*X[1] + sin(theta)*X[2];
	XP[2] = -sin(theta)*X[1] + cos(theta)*X[2];
}
//-------------------------------------------------------------------------------
void   roty(double theta, double X[], double XP[])
//-------------------------------------------------------------------------------
{
	XP[1] =  X[1];
	XP[2] =  cos(theta)*X[2] + sin(theta)*X[0];
	XP[0] = -sin(theta)*X[2] + cos(theta)*X[0];
}
//-------------------------------------------------------------------------------
void   rotz(double theta, double X[], double XP[])
//-------------------------------------------------------------------------------
{
	XP[2] =  X[2];
	XP[0] =  cos(theta)*X[0] + sin(theta)*X[1];
	XP[1] = -sin(theta)*X[0] + cos(theta)*X[1];
}

//-------------------------------------------------------------------------------
double dproduct(double a[], double b[])
//-----------------------------------------------------------------------------
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
//-----------------------------------------------------------------------------
void cproduct(double a[], double b[], double c[])
//-----------------------------------------------------------------------------
{
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}
//-----------------------------------------------------------------------------
double dotproduct(double a[], double b[], int ndata)
//-----------------------------------------------------------------------------
{
 int i;
 double temp;
 
    temp = 0;
    for(i=0;i<ndata;i++) temp += a[i]*b[i];
    
    return temp;
}
//-----------------------------------------------------------------------------
void vectsub(const double a[], const double b[], double c[])
//-------------------------------------------------------------------------------
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}
//-------------------------------------------------------------------------------
void make_unit(double x[])
//-----------------------------------------------------------------------------
{
 double temp;
 
    temp = sqrt( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
    x[0] /= temp;
    x[1] /= temp;
    x[2] /= temp;
}    
//-----------------------------------------------------------------------------
double funr(double q[])
//-------------------------------------------------------------------------------
{
 double result;
		
	result = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
	return result;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// int clsq(a,y,q,par,r,use,ndmax,nd,npmax,np)
// a[ndmax][npmax]  ; condition matrix
// y[ndmax]         ; observation vector
// q[npmax][npmax]  ; covariance matrix
// dum[npmax]       ; At y
// par[npmax]       ; parameter vector
// r[ndmax]         ; residual vector = y_observed - y_caluclated
// use              ; unit standard error
// ndmax            ; maximum # of data points
// nd               ; actual  # of data points used
// npmax            : maximun # of parameters
// np               ; actual  # of parameters
//
// In the main program call as follows.
// clsq(*a,y,*q,par,r, &use, ndmax,nd,npmax,np);
//-------------------------------------------------------------------------------
int clsq(double a[],double y[],double q[],double aty[],
         int ndmax,int nd,int npmax,int np,
         double par[],double r[],double *use)
//-------------------------------------------------------------------------------
{
 int i,j,k,eflag;
 double yci,sum;

    if(nd<np) return -10;

//  form AtA matrix
    for(i=0;i<np;i++)
    for(j=0;j<np;j++)
       {
        q[npmax*i +j] = 0.0;
        for(k=0;k<nd;k++) q[npmax*i +j] += a[npmax*k +i]*a[npmax*k +j];
       }

//  form Aty vector
    for(i=0;i<np;i++)
       {
        aty[i] = 0.0;
        for(k=0;k<nd;k++) aty[i] += a[npmax*k +i]*y[k];
       }

//  invert the matrix
    eflag = csqtinv(q,par,npmax,np);
    
    if(eflag!=0) return eflag;

//  calculate the parameter vector, par = q aty
    for(i=0;i<np;i++)
       {
        par[i] = 0;
        for(k=0;k<np;k++) par[i] += q[i*npmax + k] * aty[k];
       }

//  calculate the residual
    sum = 0.0;
    for(i=0;i<nd;i++)
       {
        yci = 0.0;
        for(j=0;j<np;j++) yci += a[npmax*i +j]*par[j];
        r[i] = y[i] - yci;
        sum += r[i]*r[i];
       }
       
//  calculate the unit standard error
    *use = sqrt(sum/(nd-np));

    return eflag;
}
//-------------------------------------------------------------------------------
int clsq_adj(double lambda,double a[],double y[],double q[],double aty[],
         int ndmax,int nd,int npmax,int np,
         double par[],double r[],double *use)
//-------------------------------------------------------------------------------
{
 int i,j,k,eflag;
 double yci,sum;

    if(nd<np) return -10;

//  form AtA matrix
    for(i=0;i<np;i++)
    for(j=0;j<np;j++)
       {
        q[npmax*i +j] = 0.0;
        for(k=0;k<nd;k++) q[npmax*i +j] += a[npmax*k +i]*a[npmax*k +j];
       }
       
    for(i=0;i<np;i++) q[npmax*i +i] *= (1.0+lambda);
       
//  form Aty vector
    for(i=0;i<np;i++)
       {
        aty[i] = 0.0;
        for(k=0;k<nd;k++) aty[i] += a[npmax*k +i]*y[k];
       }

//  invert the matrix
    eflag = csqtinv(q,par,npmax,np);
    
    if(eflag!=0) return eflag;

//  calculate the parameter vector, par = q aty
    for(i=0;i<np;i++)
       {
        par[i] = 0;
        for(k=0;k<np;k++) par[i] += q[i*npmax + k] * aty[k];
       }

//  calculate the residual
    sum = 0.0;
    for(i=0;i<nd;i++)
       {
        yci = 0.0;
        for(j=0;j<np;j++) yci += a[npmax*i +j]*par[j];
        r[i] = y[i] - yci;
        sum += r[i]*r[i];
       }
       
//  calculate the unit standard error
    *use = sqrt(sum/(nd-np));

    return eflag;
}
//-------------------------------------------------------------------------------
int csqtinv(double q[], double dum[],int nc,int n)
//-------------------------------------------------------------------------------
// q[?][nc]: matrix to be inverted
// nc      : # of (physical) columns of x
// dum[]   : dummy vector
// n       : dimension of q actually inverted
//-------------------------------------------------------------------------------
{
 int i,j,k,jp;

    if( q[0] <= 0.0 ) return 1;
    
    q[0] = sqrt(1.0/q[0]);
    for(j=1;j<=n-1;j++)
       {
        jp = j - 1;
        for(i=0;i<=jp;i++)
           {
            dum[i] = 0.0;
            for(k=0;k<=i;k++) dum[i] += q[k*nc +i]*q[k*nc +j];
           }

        for(k=0;k<=jp;k++) q[j*nc +j] -= dum[k]*dum[k];
        if(q[j*nc +j] <= 0.0 ) return (j+1);
        q[j*nc +j] = sqrt(1.0/q[j*nc +j]);

        for(i=0;i<=jp;i++)
           {
            q[i*nc +j]= 0.0;
            for(k=i;k<=jp;k++) q[i*nc +j] += q[i*nc +k]*dum[k];
            q[i*nc +j] = -q[i*nc +j]*q[j*nc +j];
           }
       }

    for(i=0;i<=n-1;i++)
       {
        for(j=i;j<=n-1;j++)
           {
            dum[j] = 0.0;
            for(k=j;k<=n-1;k++) dum[j] += q[i*nc +k]*q[j*nc +k];
            q[i*nc +j] = dum[j]; q[j*nc +i] = dum[j];
           }
       }

    return 0;
}
//-------------------------------------------------------------------------------
int linear_fit(double y[], double x[], int ndata, double *slope, double *offset, double res[], double *use)
//-------------------------------------------------------------------------------
{
 double qmat[2][2],para[2],aty[2],temp;
 int i,j;
 
    if(ndata<3) return -1;
    
    for(i=0;i<2;i++) for(j=0;j<2;j++) qmat[i][j] = 0.0;
    for(i=0;i<2;i++) aty[i]=0;
    
    for(i=0;i<ndata;i++)
       {
        qmat[0][0] += x[i]*x[i];
        qmat[0][1] += x[i];

        qmat[1][1] += 1.0;
        
        aty[0] += x[i]*y[i];
        aty[1] +=      y[i];

//      qmat[0][2] += x[i]*y[i];
//      qmat[1][2] += y[i];
       }

    qmat[1][0]=qmat[0][1];
    
    csqtinv(&qmat[0][0],para,2,2);
    
    para[1] = qmat[1][1]*aty[1] + qmat[1][0]*aty[0];
    para[0] = qmat[0][1]*aty[1] + qmat[0][0]*aty[0];
    
    *slope  = para[0];
    *offset = para[1];
    
    temp = 0.0;
    for(i=0;i<ndata;i++)
       {res[i] = y[i] - (para[0]*x[i] + para[1]);
        temp = temp + res[i]*res[i];
       }
           
    *use = sqrt(temp/(ndata-2.0));
    
    return 0;
}       
//-------------------------------------------------------------------------------
int polint(double XA[], double YA[], int N, double X, double *Y, double *DY)
//----------------------------------------------------------------------------
//                                          created by Han Inwoo, 97. Nov. 29
//-----------------------------------------------------------------------------
{
 #define NMAX 10
 double C[NMAX],D[NMAX];
 double DIF,DIFT,HO,HP,W,DEN;
 int NS,i,M;

        if(N>NMAX) return -1;

        NS=1;
        DIF=fabs(X-XA[0]);
        for(i=0;i<N;i++)
           {DIFT=fabs(X-XA[i]);
            if(DIFT<DIF){NS=i;DIF=DIFT;}
            C[i]=YA[i];
            D[i]=YA[i];
           }
        *Y=YA[NS-1];
        NS=NS-1;

        for(M=1;M<=N-1;M++)
           {
            for(i=0;i<N-M;i++)
               {HO=XA[i]-X;
                HP=XA[i+M]-X;
                W=C[i+1]-D[i];
                DEN=HO-HP;
                if(DEN==0.0) return -2;
                DEN=W/DEN;
                D[i]=HP*DEN;
                C[i]=HO*DEN;
               }

            if(2*NS<(N-M)){*DY=C[NS  ];}
            else          {*DY=D[NS-1]; NS=NS-1;}
            *Y = *Y + *DY;
           }

        return 0;
}
//----------------------------------------------------------------------------
double funroot(double f0, double x1, double x2, double facc)
//--------------------------------------------------------------------------//
// Function to calculate func(x) = f0, where x should be between x1 and x2
// input : 
// func --> function, it should be declared as external in the calling program
// f0 --> func(x) = f0
// x1, x2 --> x is between x1 and x2;
// facc : accuracy of the calculation
//--------------------------------------------------------------------------//
{
	double twopi, pi;
	double f1, f2, ff, x0, dx;
	double result;
	int j;
	static int first; 
	const int jmax = 40;

	first = 0;

	if(first == 0)
	{
		pi = acos(-1.0);
		twopi = pi*2.0;
		first = 1;
	}

	j = 0;

	do {
	j = j + 1;

//	f1 = modpi(slong(x1) - f0);
//	f2 = modpi(slong(x2) - f0);

	if(f1*f2 >= 0.0)
	{
		printf("root must be bracketed for bisection\n");
	
	}
	
	x0 = x1 - f1*(x2-x1)/(f2-f1);

//	ff = modpi(slong(x0) - f0);

	if(ff > 0)
		x2 = x0;
	else
		x1 = x0;

	dx = x2 - x1;
	} while((j < jmax) && (fabs(ff) > facc));

	result = x0;
	return result;
}
//--------------------------------------------------------------------------//

//-------------------------------------------------------------------------------
double find_max(double x[],int ndata)
//-------------------------------------------------------------------------------
{
 int i;
 double max;
    max = x[0];
    for(i=1;i<ndata;i++) if(x[i]>max) max = x[i];
    
    return max;
}
//-------------------------------------------------------------------------------
double find_min(double x[],int ndata)
//-------------------------------------------------------------------------------
{
 int i;
 double min;
    min = x[0];
    for(i=1;i<ndata;i++) if(x[i]<min) min = x[i];
    
    return min;
}
//-------------------------------------------------------------------------------
double find_mean(double x[],int ndata)
//-------------------------------------------------------------------------------
{
 int i;
 double mean;
 
    mean = 0;
    for(i=0;i<ndata;i++) mean += x[i];
    mean = mean/ndata;
    
    return mean;
}
//-------------------------------------------------------------------------------
double find_mean_sigma(double x[],int ndata, double *mean)
//-------------------------------------------------------------------------------
{
 int i;
 double sum,sigma;
 
    sum = 0;
    for(i=0;i<ndata;i++) sum += x[i];
    *mean = sum/ndata;
    
    sum = 0;
    for(i=0;i<ndata;i++) sum += (x[i]-*mean)*(x[i]-*mean);
    if(ndata>1)
    sigma = sqrt(sum/(ndata-1.0));
    else
    sigma=0;
    
    return sigma;
}
//-------------------------------------------------------------------------------
double modpi2(double a)
//--------------------------------------------------------------------------//
//임의의 각 a에 2pi의 정수배를 빼줌으로서 각 a가 0 - 2pi 사이에 있게 한다..
//--------------------------------------------------------------------------//
{
	double pi2;
	int i;
	double result;

	pi2 = acos(-1.0)*2.0;
	i = (int)(a/pi2);
	result = a - i*pi2;
	if(result < 0.0)
		result = result + pi2;
	return result;
}
//--------------------------------------------------------------------------//
//임의의 각도를 -pi ~ pi사이로 변환시킨다..
double modpi(double a)
//--------------------------------------------------------------------------//
{
	double result;
	double pi, pi2;
	int i;

	pi = acos(-1.0);
	pi2 = pi*2.0;

	i = (int)(a/pi2);
	result = a - i*pi2;
	
	if(result > pi)
		result = result - pi2;
	if(result < -pi)
		result = result + pi2;

	return result;
}
//--------------------------------------------------------------------------//

//-----------------------------------------------------------------------------
int sort_double_old(int ndata, double yinp[], int indx[])
//-----------------------------------------------------------------------------
{
 int i,l,j,ir;
 double q;
 int indxt;

    for(j=1;j<=ndata;j++) indx[j] = j;

	l  = ndata/2 +1;// L = N/2 + 1
	ir = ndata;     // IR = N

        for(;;)
           {
            if(l>1)                   // IF(L.GT.1) THEN
              {l = l - 1;             // L = L - 1
               indxt = indx[l];       // INDXT = INDX(L)
               q     = yinp[indxt];   // Q = ARRIN(INDXT)
              }
            else
              {
               indxt = indx[ir];      //INDXT = INDX(IR)
               q = yinp[indxt];       //Q = ARRIN(INDXT)
               indx[ir] = indx[1];    //INDX(IR) = INDX(1)
               ir = ir - 1;           //IR = IR - 1
               if(ir==1)              //IF(IR.EQ.1) THEN
                 {indx[1] = indxt;    //INDX(1) = INDXT
                  return 0;           // RETURN
                 }                    // ENDIF
              }

            i = l;     //I = L
            j = l + l; //J = L + L

           //20	IF(J.LE.IR) THEN
            while(j<=ir)
                 {
                  if(j<ir)  //IF(J.LT.IR) THEN
                    {
//                   IF(ARRIN(INDX(J)).LT. ARRIN(INDX(J+1)) ) J = J+1
                     if( yinp[indx[j]] <   yinp [indx[j+1]] ) j = j+1;
                    } // ENDIF

                  if(q < yinp[indx[j]]) //IF(Q.LT.ARRIN(INDX(J))) THEN
                    {indx[i] = indx[j]; //INDX(I) = INDX(J)
                     i = j;             //I = J
                     j = j + j;         //J = J + J
                    }
                  else                  //ELSE
                     j = ir + 1;        //J = IR + 1
                 //ENDIF
                }
	      indx[i] = indxt; //INDX(I) = INDXT
         }
}
//-----------------------------------------------------------------------------
int sort_double(int ndata, double yinpp[], int indxp[])
//-----------------------------------------------------------------------------
{
 int i,l,j,ir;
 double q;
 int indxt;
 double *yinp;
 int *indx;
 int k;
 
    indx = &indxp[0];
    yinp = &yinpp[0];
    indx--;
    yinp--;

    for(j=1;j<=ndata;j++) indx[j] = j;

	l  = ndata/2 +1;// L = N/2 + 1
	ir = ndata;     // IR = N

        for(;;)
           {
            if(l>1)                   // IF(L.GT.1) THEN
              {l = l - 1;             // L = L - 1
               indxt = indx[l];       // INDXT = INDX(L)
               q     = yinp[indxt];   // Q = ARRIN(INDXT)
              }
            else
              {
               indxt = indx[ir];      //INDXT = INDX(IR)
               q = yinp[indxt];       //Q = ARRIN(INDXT)
               indx[ir] = indx[1];    //INDX(IR) = INDX(1)
               ir = ir - 1;           //IR = IR - 1
               if(ir==1)              //IF(IR.EQ.1) THEN
                 {indx[1] = indxt;    //INDX(1) = INDXT
//                return 0;           // RETURN
				  for(k=1;k<=ndata;k++)indx[k]--;return 0;
                 }                    // ENDIF
              }

            i = l;     //I = L
            j = l + l; //J = L + L

           //20	IF(J.LE.IR) THEN
            while(j<=ir)
                 {
                  if(j<ir)  //IF(J.LT.IR) THEN
                    {
//                   IF(ARRIN(INDX(J)).LT. ARRIN(INDX(J+1)) ) J = J+1
                     if( yinp[indx[j]] <   yinp [indx[j+1]] ) j = j+1;
                    } // ENDIF

                  if(q < yinp[indx[j]]) //IF(Q.LT.ARRIN(INDX(J))) THEN
                    {indx[i] = indx[j]; //INDX(I) = INDX(J)
                     i = j;             //I = J
                     j = j + j;         //J = J + J
                    }
                  else                  //ELSE
                     j = ir + 1;        //J = IR + 1
                 //ENDIF
                }
	        indx[i] = indxt; //INDX(I) = INDXT
           }
}
//-----------------------------------------------------------------------------
int intsort(int n, int arrin[], int indx[])
//-----------------------------------------------------------------------------
{
 int i,q,l,j,ir;
 int indxt;

    for(j=0;j<n;j++) indx[j] = j;

	l  = n/2 +1;// L = N/2 + 1
	ir = n;     // IR = N

   for(;;)
          {
            if(l>1)                   // IF(L.GT.1) THEN
              {l = l - 1;             // L = L - 1
               indxt = indx[l];     // INDXT = INDX(L)
               q     = arrin[indxt];// Q = ARRIN(INDXT)
              }
            else
              {
               indxt = indx[ir];    // INDXT = INDX(IR)
               q = arrin[indxt];    // Q = ARRIN(INDXT)
               indx[ir] = indx[0];  // INDX(IR) = INDX(1)
               ir = ir - 1;           // IR = IR - 1
               if(ir==1)              // IF(IR.EQ.1) THEN
                 {indx[1] = indxt;    // INDX(1) = INDXT
                  return 0;           // RETURN
                 }                    // ENDIF
              }

            i = l;     //I = L
            j = l + l; //J = L + L

           //20	IF(J.LE.IR) THEN
            while(j<=ir)
                 {
                  if(j<ir)  //IF(J.LT.IR) THEN
                    {
//                   IF( ARRIN(INDX(J))    .LT.ARRIN(INDX(J+1)) ) J = J+1
                     if( arrin[indx[j]] <  arrin[indx[j]] ) j = j + 1;
                    } // ENDIF

                  if(q < arrin[indx[j]]) //IF(Q.LT.ARRIN(INDX(J))) THEN
                    {indx[i] = indx[j];  //INDX(I) = INDX(J)
                     i = j;                  //I = J
                     j = j+j;                //J = J + J
                    }
                  else                     //ELSE
                     j = ir + 1;           //J = IR + 1
                 //ENDIF
                }

	      indx[i] = indxt; //INDX(I) = INDXT
         }
}
//-----------------------------------------------------------------------------
int cubic_spline_interpolate(int n1, double y1[], int df, double y2[])
//-------------------------------------------------------------------------------
{
 double *x, *yp;
 double xi,yi,dx;
 int i,n2;
 
    n2 = (n1-1)*df + 1;
    
    if( ( x=(double *)malloc(n1*8)) == NULL)  return -1;
    if( (yp=(double *)malloc(n1*8)) == NULL)  return -2;
    
    for(i=0;i<n1;i++) x[i]=i;
    Spline(x,y1,n1,0.0,0.0,yp);
    
    dx = 1.0/df;
    for(i=0;i<n2;i++)
       {
        xi = i*dx;
        SpInterp (x,y1,yp,n1,xi,&y2[i]);
       }
       
    free(x);
    free(yp);
    
    return 0;
}
//-------------------------------------------------------------------------------
int remove_outlier(double x[],int ndata, double range, double *me, double *si, int *nn)
//-----------------------------------------------------------------------------
{
 int i,j,nd,ndold,icycle,ireturn;
 double sigma,mean;
 double sum1,sum2,sumw,wmean;
 
//  initialize    
    sum1 = sum2 = 0;
    for(i=0;i<ndata;i++) {sum1 += x[i];sum2 += x[i]*x[i];}
    mean = sum1/ndata;
    sigma= sqrt(sum2/ndata - mean*mean) ;
    
    ndold = ndata;

    icycle=0;
    ireturn = 0;
    for(;;)
       {
        sum1 = sum2 = 0; nd = 0;
        sumw=wmean=0;
        for(i=0;i<ndata;i++)
            if( fabs(x[i]-mean) < sigma*range) {sum1 += x[i];sum2 += x[i]*x[i]; nd++;}
        mean = sum1/nd;
        sigma= sqrt( sum2/nd - mean*mean);
        if(nd==ndold) break;
        else ndold = nd;
        
        icycle++;
        if(icycle>10) {ireturn=-1; break;}
       }
       
 	*me = mean;
 	*si = sigma;
 	*nn = nd;

    return ireturn;
}
//--------------------------------------------------------------------------------
double remove_trend(double epoch[], double rvd[], double para[],int ndata, int npara)
//--------------------------------------------------------------------------------
{
 int i;
 double *cmat,*qmat,*aty,*resi,use,ti,tm;
 
    if((cmat = (double *)malloc(ndata*npara*8)) == NULL)  return -1;
    if((qmat = (double *)malloc(npara*npara*8)) == NULL)  return -1;
    if((aty  = (double *)malloc(npara*      8)) == NULL)  return -1;
    if((resi = (double *)malloc(ndata*      8)) == NULL)  return -1;

//  initialize cmat;
    for(i=0;i<ndata*npara;i++) cmat[i]=0;
 
    tm = (epoch[0]+epoch[ndata-1])/2.0;
//  construct cmat    
    for(i=0;i<ndata;i++)
       {
        ti = epoch[i]-tm;
        cmat[i*npara  ] = 1.0;
        cmat[i*npara+1] = ti;
        if(npara==3)
        cmat[i*npara+2] = ti*ti;
       }
    
    clsq(cmat,rvd,qmat,aty,ndata,ndata,npara,npara,para,resi,&use);
    
    for(i=0;i<ndata;i++) rvd[i] = resi[i];
    
    free(cmat);
    free(qmat);
    free(aty);
    free(resi);

 return use;
}
//-------------------------------------------------------------------------------
int calc_ccf(double template[], int ntempl, double comp[],int ncomp, double ccf[], int s1, int s2)
//-------------------------------------------------------------------------------
{
 int i,s,k;
 int i1,i2;
 
//   CCF(s) = INT [ TEMP(x)COMP(x+s) dx]
//   CCF(s) = SUM(j<=j1;j<j2)[TEMP(j)COMP(j+s)]
//
//   0<=(i1+s1) && (i2+s1)<=nc
//   0<=(i1+s2) && (i2+s2)<=nc
//   -s2<-s1<=i1     && i2<=nc-s2<=nc-s1

    i1=-s1;
    i2=ncomp-s2;

    if(i1<0     ) i1=0;
    if(i2>ntempl) i2=ntempl;
 
    for(s=s1;s<=s2;s++)
       {
        ccf[s-s1]=0;
        for(i=i1;i<i2;i++) ccf[s-s1] += comp[i+s]*template[i];
       }

 return i2-i1;    
}
//-------------------------------------------------------------------------------
int convolve_old(double y[], int ny, double profile[], int np, double y_conv[])
//-------------------------------------------------------------------------------
{
 int i,j,n;
// Conv(y,profile)(i) = SUM(j) y[j  ]profile[i-j]
//                    = SUM(j) y[i-j]profile[j  ]
//
// np = 2*n+1;
   n = np/2;  
   for(i=np;i<ny;i++)
      {
       y_conv[i-n]=0;
       for(j=0;j<np;j++) y_conv[i-n]+= y[i-j]*profile[j];
      }
 return 0;
}
//-------------------------------------------------------------------------------
int convolve(double y[], int ny, double profile[], int np, double y_conv[])
//-------------------------------------------------------------------------------
{
 int i,j,n;
// Conv(y,profile)(i) = SUM(j) y[j  ]profile[i-j]
//                    = SUM(j) y[i-j]profile[j  ]
//
// np = 2*n+1;
   n = np/2;  
   for(i=n;i<ny-n;i++)
      {
       y_conv[i-n]=0;
       for(j=-n;j<=n;j++) y_conv[i-n]+= y[i+j]*profile[np-(j+n)];
      }
 return 0;
}
//-------------------------------------------------------------------------------

