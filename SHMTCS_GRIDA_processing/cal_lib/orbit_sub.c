//-------------------------------------------------------------------------------
// dE = [(t-t_0) dn - n dt_0 + sin_E de ] / (1 - e cos E) 
//    = [-twopi*(t-t_0)/p**2 dp - n dt_0 + sin_E de ] / (1 - e cos E)
//
//  fac = 1.0/(1.0-ecc*cos_E);
//  dE_dT =-fac * mm; 
//  dE_dP =-fac * twopi * (t-t_0)/(per*per);
//  dE_de = fac * sin_E;
//-------------------------------------------------------------------------------
#include <userint.h>
#include <ansi_c.h>
//#include "type_def.h"
#include "cpabtsub.h"

static double  PI314           = 3.14159265358979323846;
static double  TWOPI           = 3.14159265358979323846*2.0;
static double  FACT_DEG_TO_RAD = 3.14159265358979323846/180.0;
static double  FACT_RAD_TO_DEG = 180.0/3.14159265358979323846;

//-------------------------------------------------------------------------------
double get_ea(double t,double t0,double mm,double e,double limit);
int get_eanew(double t,double t0,double mm,double e,double limit, double *ea);
int calc_rvd(double k1,double w,double t_0,double per,double ecc,double v0,double ti,double *vrc);
int calc_ksi_eta(double sma,double an,double inc,double w,double t_0,double per,double ecc,double ti,double *ksi,double *eta);
int calc_rvorbit(double k0,double per,double ecc,double w,double t_0,double ti, double *vrc);
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
double get_ea(double t,double t0,double mm,double e, double limit)
//-------------------------------------------------------------------------------
//  Subroutine to calculate eccentric anomaly from the input variables
//
//		Written   by Han Inwoo, 1993. Oct. 19
//		Modified  by Han Inwoo, 1994. Oct. 19
//-------------------------------------------------------------------------------
{
 double ea1,ea2,delea,dabs,modpi2,m;
 int    count;

	m  = mm*(t-t0);
//	m  = modpi2(m);
    m   =fmod(m,TWOPI);

//	e2 = e*e
//	e3 = e2*e
//	e4 = e3*e
//	e5 = e4*e
//	e6 = e5*e
//	e7 = e6*e

//	ea1 = m + (e-e3/8.0 + e5/192.0 - e7/9216.0)*dsin(m)
//     &  + (e2/2.0 - e4/6.0 + e6/48.0)*dsin(2.0*m)
//     &  + (e3*3.0/8.0 - e5*27.0/128.0 + e7*243.0/5120.0)*dsin(3.0*m)
//     &  + (e4/3.0 - e6*4.0/15.0)*dsin(4.0*m)
//     &  + (e5*125.0/384.0 - e7*3125.0/9216.0)*dsin(5.0*m)
//     &  + e6*27.0/80.0*dsin(6.0*m)
//     &  + 16807.0/46080.0*e7*dsin(7.0*m)

	ea1 = m + e*sin(m);

	count = 0;
    do{
//	   ea2 = m + e*dsin(ea1)
       ea2 = ea1 + (m+e*sin(ea1)-ea1) / (1.0-e*cos(ea1));

       delea = fabs(ea2-ea1);
//     delea = fmod(fabs(ea2-ea1),TWOPI);
       ea1   = ea2;
       count = count + 1;

       if(count>20)
         {
//        printf("m = %lf ea1 = %lf del EA = %lf arcsec\n",m,ea1,delea*206265.0);
          MessagePopup(""," no convergence of EA");
//          exit(0);
          return ea1;
         }
         
	  } while(delea>limit);

	return ea1;
}	
//-------------------------------------------------------------------------------
int get_eanew(double t,double t0,double mm,double e, double limit,double *ea)
//-------------------------------------------------------------------------------
//  Subroutine to calculate eccentric anomaly from the input variables
//
//		Written   by Han Inwoo, 1993. Oct. 19
//		Modified  by Han Inwoo, 1994. Oct. 19
//-------------------------------------------------------------------------------
{
 double ea1,ea2,delea,dabs,modpi2,m;
 int    count;

	m  = mm*(t-t0);
//	m  = modpi2(m);
    m   =fmod(m,TWOPI);

//	e2 = e*e
//	e3 = e2*e
//	e4 = e3*e
//	e5 = e4*e
//	e6 = e5*e
//	e7 = e6*e

//	ea1 = m + (e-e3/8.0 + e5/192.0 - e7/9216.0)*dsin(m)
//     &  + (e2/2.0 - e4/6.0 + e6/48.0)*dsin(2.0*m)
//     &  + (e3*3.0/8.0 - e5*27.0/128.0 + e7*243.0/5120.0)*dsin(3.0*m)
//     &  + (e4/3.0 - e6*4.0/15.0)*dsin(4.0*m)
//     &  + (e5*125.0/384.0 - e7*3125.0/9216.0)*dsin(5.0*m)
//     &  + e6*27.0/80.0*dsin(6.0*m)
//     &  + 16807.0/46080.0*e7*dsin(7.0*m)

	ea1 = m + e*sin(m);

	count = 0;
    do{
//	   ea2 = m + e*dsin(ea1)
       ea2 = ea1 + (m+e*sin(ea1)-ea1) / (1.0-e*cos(ea1));

       delea = fabs(ea2-ea1);
//     delea = fmod(fabs(ea2-ea1),TWOPI);
       ea1   = ea2;
       count = count + 1;

       if(count>20)
         {
//        printf("m = %lf ea1 = %lf del EA = %lf arcsec\n",m,ea1,delea*206265.0);
          MessagePopup(""," no convergence of EA");
          return -1;
         }
         
	  } while(delea>limit);

    *ea = ea1;
//	return ea1;
    return 0;
}	
//-------------------------------------------------------------------------------
int calc_rvorbit(double k0,double per,double ecc,double w,double t_0,double ti, double *vrc)
//-------------------------------------------------------------------------------
//   Vr    = K1(cos(w+nu) + e cos_w ) + V0
//   dV_dK = cos(w+nu) + e cos(w) 
//   dV_dw = K1 ( -sin(w+nu) - e sin_w)
//   dV_dT = -K1 * sin(w+nu) * dnu_dT
//         = -K1 * sin(w+nu) * dnu_dE * dE_dT
//   dV_dP = -K1 * sin(w+nu) * dnu_dP
//         = -K1 * sin(w+nu) * dnu_dE * dE_dP
//   dV_de = K1 ( -win(w+nu) * dnu_de + cos_w )
//         = K1 ( -win(w+nu) * dnu_dE * dE_de + cos_w )
//   dnu_dE= sqrt(1-e*e) / (1 - e*cos_E)
//-------------------------------------------------------------------------------
{
 double EA,mm,nu;
 double limit=1.0e-10;
//--------------------------------------------------------------------
	mm  = TWOPI/per;     // mean motion
//--------------------------------------------------------------------
//	calculate eccentric anomaly
    if(get_eanew(ti,t_0,mm,ecc,limit,&EA)!=0) return -1;
    nu = atan2( sqrt(1.0-ecc*ecc)*sin(EA), cos(EA)-ecc);
    *vrc = k0*(cos(w+nu) + ecc*cos(w));
    
    return 0;
}
//-------------------------------------------------------------------------------
int calc_rvd(double k1,double w,double t_0,double per,double ecc,double v0,double ti,double *vrc)
//-------------------------------------------------------------------------------
//   Vr    = K1(cos(w+nu) + e cos_w ) + V0
//   dV_dK = cos(w+nu) + e cos(w) 
//   dV_dw = K1 ( -sin(w+nu) - e sin_w)
//   dV_dT = -K1 * sin(w+nu) * dnu_dT
//         = -K1 * sin(w+nu) * dnu_dE * dE_dT
//   dV_dP = -K1 * sin(w+nu) * dnu_dP
//         = -K1 * sin(w+nu) * dnu_dE * dE_dP
//   dV_de = K1 ( -win(w+nu) * dnu_de + cos_w )
//         = K1 ( -win(w+nu) * dnu_dE * dE_de + cos_w )
//   dnu_dE= sqrt(1-e*e) / (1 - e*cos_E)
//-------------------------------------------------------------------------------
{
 double EA,mm,nu;
 double limit=1.0e-10;
//--------------------------------------------------------------------
	mm  = TWOPI/per;     // mean motion
//--------------------------------------------------------------------
//	calculate eccentric anomaly
    if(get_eanew(ti,t_0,mm,ecc,limit,&EA)!=0) return -1;
    nu = atan2( sqrt(1.0-ecc*ecc)*sin(EA), cos(EA)-ecc);
    *vrc = k1*(cos(w+nu) + ecc*cos(w)) + v0;
    
    return 0;
}
//-------------------------------------------------------------------------------
int calc_ksi_eta(double sma,double an,double inc,double w,double t_0,double per,double ecc,double ti,double *ksi,double *eta)
//-------------------------------------------------------------------------------
//  Input variable
//    t    : epoch
//    PARA : vector to contain orbital elements
//    IOD  : vector to match each element of PARA to orbital element, see the code below
//
//  Output variable
//    ksi  : x coordinates of the orbit calculated by t and PARA
//    eta  : y coordinates of the orbit calculated by t and PARA
//-------------------------------------------------------------------------------
{
 double mm;
 double EA,cos_E,sin_E,limit=1.0e-10;
 double x,y,X_1,X_2,Y_1,Y_2;
 double cos_w,sin_w,cos_i,sin_i,cos_A,sin_A;
//--------------------------------------------------------------------
//	sma = para[IOD[0]];  // semi-major axis
//	an  = para[IOD[1]];  // ascending node angel in radian
//	inc = para[IOD[2]];  // inclination
//	w   = para[IOD[3]];  // periastron angle
//	t_0 = para[IOD[4]];  // epoch of periastron passage
//	per = para[IOD[5]];  // period
//	ecc = para[IOD[6]];  // eccentricity
	
	mm  = TWOPI/per;     // mean motion
//--------------------------------------------------------------------
//	calculate eccentric anomaly
    if(get_eanew(ti,t_0,mm,ecc,limit,&EA)!=0) return -1;
	cos_E = cos(EA);
	sin_E = sin(EA);

	x = cos_E - ecc;
	y = sqrt(1-ecc*ecc) * sin_E;
	
	cos_w = cos(w);
	sin_w = sin(w);
	cos_A = cos(an);
	sin_A = sin(an);
	cos_i = cos(inc);
	sin_i = sin(inc);
//----------------------------------------------------
	X_1 =  cos_w*sin_A + sin_w*cos_A*cos_i;
	X_2 = -sin_w*sin_A + cos_w*cos_A*cos_i;
	Y_1 =  cos_w*cos_A - sin_w*sin_A*cos_i;
	Y_2 = -sin_w*cos_A - cos_w*sin_A*cos_i;
	
	*ksi = sma * ( x*X_1 + y*X_2 );
	*eta = sma * ( x*Y_1 + y*Y_2 );
	
	return 0;
	
//	printf("%10.3lf %10.3lf %10.3lf %10.3lf %10.3lf\n",x,y,sma,*ksi,*eta);
}
//-------------------------------------------------------------------------------
