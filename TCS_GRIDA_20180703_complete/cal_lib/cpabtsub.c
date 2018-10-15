#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "ssbsub.h"
#include "mathsub.h"
#include "cpabtsub.h"

static double  PI314           = 3.14159265358979323846;
static double  TWOPI           = 3.14159265358979323846*2.0;
static double  FACT_DEG_TO_RAD = 3.14159265358979323846/180.0;
static double  FACT_RAD_TO_DEG = 180.0/3.14159265358979323846;

//-----------------------------------------------------------------------------
//	BEGIN :	Coordinates Transformation routines section
//-----------------------------------------------------------------------------
void   get_alt_azi_psi(double ra_2000, double dec_2000, double jd_ut1, double longi, double lati, int flag, double *az,double *alt, double *psi)
//-----------------------------------------------------------------------------
//  Claculate altitude, azimuth and derotator angle for controlling
//  an Alt-Az type mount including derotator.
//  
//  Input 
//    ra_2000,dec_2000 : R.A. & Dec. in J2000, Unit in radian
//    jd_ut1           : Julian date in UT1 scale
//    longi, lati      : longitude and latitude in radian
//    flag             : flag for refraction correction. 
//                       if flag = 0, not correct for refraction 
//                       else         correct for refraction
//  Output 
//    lat, az     : apparent altitude, azimuth in radian
//    psi         : so called parallactic angle, angle of EAST fixed on the celestian sphere
//                  from east direction fixed at the telescope mount.
//
//  Note          : azimuth measured from south toward west
//
//                    Created by Han Inwoo, 1997. Aug. 21
//-------------------------------------------------------------------------------
{double last,ha, ra,dec;
        get_apradec(ra_2000,dec_2000,jd_ut1, &ra,&dec);
        last = jdtolast(jd_ut1,longi);
        ha   = last - ra;
        *alt = asin ( sin(lati)*sin(dec) + cos(lati)*cos(dec)*cos(ha) );
        *az  = atan2( cos(dec)*sin(ha),
                     -cos(lati)*sin(dec)+sin(lati)*cos(dec)*cos(ha) );
//      if(*az<0.0) *az = *az + TWOPI;

//      correct for refraction, if flag is not 0.
        if(flag != 0) refract(alt);

//      calculate the derotator angle
        *psi = atan2( cos(dec)*sin(ha)*cos(lati),
                      sin(lati)-sin(dec)*sin(*alt) );
}
//-------------------------------------------------------------------------------
void get_aposi(double ra_2000, double dec_2000, double jd_ut1, double longi,double lati, int flag,
              double *last, double *ap_ha, double *ap_dec, double *ap_alt, double *ap_az)
//-------------------------------------------------------------------------------
//   This function claculate apparent position (hour angle, declination, altitude
//   azimuth, local astronomical time from R.A. & Dec. given in J2000 coordinates
//   system. Note that it also corrects for atmospheric refraction.
//   It is useful to control a telescope with equatorial mount.
//   
//	Input variable
//	   real*8 ra_2000, dec_2000 : RA. and Dec. (radian) in J2000 coordinate system
//	   real*8 jd_ut1  : Julian date in UT1 scale
//	   longi, lati    : Longitude and Latitude in radian
//         flag           : flag for refraction correction. 
//                          if flag = 0, not correct for refraction 
//                          else         correct for refraction
//	Output variable
//	   last           : Local Apparent Sidereal time in radian
//	   ap_ha , ap_dec : apparent hour angle and dec. in radian
//	   ap_alt, ap_az  : Altitude and azimuth in radian.
//
//     Note that the output position is corrected for refraction,
//               azimuth is measured from south toward south,
//               hour angle is given between -180 deg. and 180 deg
//
//                Created  by Inwoo Han on Jun. 25, 1996
//                Modified by Inwoo Han on Aug. 22, 1997
//-------------------------------------------------------------------------------
{
 double ap_ra;

    *last  = jdtolast(jd_ut1,longi);
    if(*last<0.0) *last = *last + TWOPI;

//  calculate apparent ra and dec
    get_apradec(ra_2000,dec_2000,jd_ut1, &ap_ra, ap_dec);

//  claculate altitude and azimuth
    equtohor(ap_ra,*ap_dec,*last,lati,ap_az,ap_alt);

//  correct for refraction
    if(flag != 0)
      {
       if(rad_to_deg(*ap_alt) > -5.0)  refract(ap_alt);
       hortoequ(*ap_az,*ap_alt,*last,lati,&ap_ra,ap_dec);
      }

//  calculate and adjust the range of the apparent hour angle
	*ap_ha = funha(ap_ra,*last);
	if(*ap_ha > PI314) *ap_ha = *ap_ha - TWOPI;
	if(*ap_ha <-PI314) *ap_ha = *ap_ha + TWOPI;
}
//-------------------------------------------------------------------------------
void get_apradec(double ra_2000, double dec_2000, double jd, double *ra_app, double *dec_app)
//-------------------------------------------------------------------------------
//   This routine compute the apparent position corrected for precession and
//   nutation at the given epoch.
//
//   Input
//      ra_2000, dec_2000 : R.A. & Dec. (in radian) given for J2000
//      jd                : Julian date (in UT1 scale) for the desired epoch
//   Output
//      ra_app , dec_app  : apparent R.A. & Dec. (in radian) 
//
//   Note : refraction is not crrected
//-------------------------------------------------------------------------------
{
 double x0[3],x[3];
	stor(ra_2000,dec_2000, x0);
    anuaber(jd,x0);
	precess(jd, x0, x );
	nutate (jd, x , x0);
	rtos(x0, ra_app, dec_app);
	if(*ra_app<0.0) *ra_app = *ra_app + TWOPI;
}
//-------------------------------------------------------------------------------
void teat_apradec(double ra_2000, double dec_2000, double jd, double *ra_app, double *dec_app)
//-------------------------------------------------------------------------------
//   This routine compute the apparent position corrected for precession and
//   nutation at the given epoch.
//
//   Input
//      ra_2000, dec_2000 : R.A. & Dec. (in radian) given for J2000
//      jd                : Julian date (in UT1 scale) for the desired epoch
//   Output
//      ra_app , dec_app  : apparent R.A. & Dec. (in radian) 
//
//   Note : refraction is not crrected
//-------------------------------------------------------------------------------
{
 double x0[3],x[3];
	stor(ra_2000,dec_2000, x0);
//    anuaber(jd,x0);
	precess(jd, x0, x );
	rtos(x, ra_app, dec_app);
	
	nutate (jd, x , x0);
	rtos(x0, ra_app, dec_app);

	if(*ra_app<0.0) *ra_app = *ra_app + TWOPI;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void equtoecl(double ra, double dec, double obl, double *lamda, double *beta)
//-------------------------------------------------------------------------------
//  Subroutine to transform from equatorial coordinates, (ra,dec) to
//  ecliptic system (lamda, beta)
//
//  Input  : ra,dec, obliquity (all in radian)
//  Output : lamda, beta       (all in radian)
//-------------------------------------------------------------------------------
{
	*beta =  asin( cos(obl)*sin(dec) - sin(obl)*cos(dec)*sin(ra) );
	*lamda= atan2( sin(obl)*sin(dec)+cos(obl)*cos(dec)*sin(ra),
					   cos(dec)*cos(ra) );
	if(*lamda<0.0) *lamda = *lamda + PI314*2.0;
}
//-------------------------------------------------------------------------------
void ecltoequ(double lamda,double beta,double obl, double *ra, double *dec)
//-------------------------------------------------------------------------------
//  Coordinates transform from ECLiptic system  EQUatorial system
//
//  Input  : lamda,beta, obliquity (all in radian)
//  Output : ra, dec (all in radian)
//-------------------------------------------------------------------------------
{
	*dec = asin(  cos(obl)*sin(beta) + sin(obl)*cos(beta)*sin(lamda) );
	*ra  = atan2( -sin(obl)*sin(beta) + cos(obl)*cos(beta)*sin(lamda),
					   cos(beta)*cos(lamda) );
	if(*ra<0.0) *ra = *ra + PI314*2.0;
}
//-------------------------------------------------------------------------------
void equtohor(double ra, double dec,double last,double phi, double *az,double *alt)
//-------------------------------------------------------------------------------
//  Coordinates transform from equatorial system to horizontal system
//  Input 
//    ra,dec : R.A. & Dec. in radian
//    last   : Local Astronomical Siderail Time in radian
//    phi    : latitude in radian
//  Output 
//    az, alt : azimuth, altitude in radian
//
//  Note   : azimuth measured from south toward west
//-------------------------------------------------------------------------------
{   double ha;

	ha  = funha(ra,last);
	*alt =  asin( sin(phi)*sin(dec) + cos(phi)*cos(dec)*cos(ha) );
	*az  = atan2( cos(dec)*sin(ha),
                     -cos(phi)*sin(dec)+sin(phi)*cos(dec)*cos(ha) );
//      if(*az<0.0) *az = *az + TWOPI;
}
//-------------------------------------------------------------------------------
void hadec_to_altazi(double ha, double dec, double phi, double *alt,double *azi)
//-------------------------------------------------------------------------------
//  Coordinates transform from equatorial system to horizontal system
//  Input 
//    ha,dec : H.A. & Dec. in radian
//    phi    : latitude in radian
//  Output 
//    azi, alt : azimuth, altitude in radian
//
//  Note   : azimuth measured from south toward west
{
	*alt =  asin( sin(phi)*sin(dec) + cos(phi)*cos(dec)*cos(ha) );
	*azi = atan2( cos(dec)*sin(ha),
                      -cos(phi)*sin(dec)+sin(phi)*cos(dec)*cos(ha) );
}

//------------------  JISUNG TEST -------------------------------------------------------------
//{
//	double temp ;
//	if (sin(ha) < 0.)
//	{
//		*alt =  asin( sin(phi)*sin(dec) + cos(phi)*cos(dec)*cos(ha) );
//		*azi = atan2( cos(dec)*sin(ha),
//                      -cos(phi)*sin(dec)+sin(phi)*cos(dec)*cos(ha) );
//	}
//	else
//	{
//		*alt =  asin( sin(phi)*sin(dec) + cos(phi)*cos(dec)*cos(ha) );
//		*azi = TWOPI - (atan2( cos(dec)*sin(ha),
//                     -cos(phi)*sin(dec)+sin(phi)*cos(dec)*cos(ha) ));
//	}
		
//}
//-----------------   JISUNG TEST  ---------------------------------------------------------------


//-------------------------------------------------------------------------------
void hortoequ(double az, double alt, double last, double phi, double *ra, double *dec)
//-------------------------------------------------------------------------------
//  Coordinates transform from horizontal system to equatorial system
//  Input 
//    az, alt : azimuth, altitude in radian
//    last   : Local Astronomical Siderail Time in radian
//    phi    : latitude in radian
//  Output 
//    ra,dec : R.A. & Dec. in radian
//
//  Note   : azimuth measured from south toward west
//-------------------------------------------------------------------------------
{double ha;
        *dec =  asin( sin(phi)*sin(alt) - cos(phi)*cos(alt)*cos(az) );
        ha   =  atan2( cos(alt)*sin(az),
                       cos(phi)*sin(alt) + sin(phi)*cos(alt)*cos(az) );
	*ra = last - ha;
}
//-------------------------------------------------------------------------------
void altazi_to_hadec(double alt, double azi, double phi, double *ha, double *dec)
//-------------------------------------------------------------------------------
//  Coordinates transform from horizontal system to equatorial system
//  Input 
//    az, alt : azimuth, altitude in radian
//    phi    : latitude in radian
//  Output 
//    ha,dec : H.A. & Dec. in radian
//
//  Note   : azimuth measured from south toward west
//-------------------------------------------------------------------------------
{
        *dec =  asin( sin(phi)*sin(alt) - cos(phi)*cos(alt)*cos(azi) );
        *ha  =  atan2( cos(alt)*sin(azi),
                       cos(phi)*sin(alt) + sin(phi)*cos(alt)*cos(azi) );
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void gctotc(double *ra, double *dec, double *dist, double stime, double lat, double h)
//-------------------------------------------------------------------------------
//   Subroutine to convert GeoCentric coordinates to TopoCentric coordinates.
//   Input
//    ra,dec,dist : Geocentric RA & DEC in radian, distance in A.U.
//    stime : sidereal time (radian)
//    lat   : latitude (radian)
//    h     : height (meter)
// Output
//    At the exit, topoCentric RA, DEC and dist are returned
//-------------------------------------------------------------------------------
{	double xo[3],xx[3],latgc, rap,decp;
	const  double aumeter=1.4959787066e11;
	getxo(stime,lat,h, xo, &latgc);
	xx[0] = *dist*cos(*dec)*cos(*ra) - xo[0]/aumeter;
	xx[1] = *dist*cos(*dec)*sin(*ra) - xo[1]/aumeter;
	xx[2] = *dist*sin(*dec)          - xo[2]/aumeter;

	rtos(xx,&rap,&decp); *ra = rap; *dec = decp;
	*dist = sqrt( xx[0]*xx[0] + xx[1]*xx[1] + xx[2]*xx[2] );
}
//-------------------------------------------------------------------------------
int correct_rv(double jd, double longi, double lati, double height,
               double rarad, double decrad, double *jd_bc, double *rv_obs)
//-----------------------------------------------------------------------------
//  height in meter
//  rv_obs in km/s
//-----------------------------------------------------------------------------
{
 double v_ec[3],v_te[3],v_tc[3],v_obs[3];
 double r_ec[3];
 
 double omega,r_obs[3];
 double latgc;
 const  double aumeter=1.4959787066e11;
 double temp,lmst;
 
 double n0[3],n1[3];
 double rv1,rv2;
 
//  rotation velocity of Earth
    omega=7.2921151467e-5;  // radian/sec
     
//  unit vector to target in J2000
    n0[0] = cos(decrad)*cos(rarad);
    n0[1] = cos(decrad)*sin(rarad);
    n0[2] = sin(decrad);

//  unit vector to target in precession
    precess(jd,n0,n1);

//  velocity of earth center
//  init_jpl("c:\\lib\\jplephs.dos");
    geterv(jd, r_ec, v_ec);
//  close_jpl();
    
//  convert au/day to m/s
    v_ec[0] *= aumeter/86400;
    v_ec[1] *= aumeter/86400;
    v_ec[2] *= aumeter/86400;
    
    rv1 = dproduct(v_ec,n0)/1000.0;
    
//  velocity of topocenter
    lmst = jdtolmst(jd,longi);
    getxo(lmst,lati,height,r_obs,&latgc);
    temp = r_obs[0]*r_obs[0] + r_obs[1]*r_obs[1];
    temp = sqrt(temp)*omega/1000.0;
    
    rv2 = temp*(-sin(lmst)*n1[0] + cos(lmst)*n1[1]);
    
    *rv_obs = rv1 + rv2; // unit is km/sec
             
//  position of earth's center
    temp = aumeter*dproduct(r_ec,n0) + dproduct(r_obs,n1);
//  temp = aumeter*dproduct(r_ec,n0);
    *jd_bc = jd + temp/299792458.0/86400;
    
//    sprintf(buff," rv correction %15.9lf %14.9lf %14.9lf\n",rv1,rv2,rv1+rv2);
//    display_message(buff);   
    
    return 0;
}
//-----------------------------------------------------------------------------
//
//           [-sin GAST, -cos GAST, 0]
//  v(t) = w [ cos GAST, -sin GAST, 0] R1(yp) R2(xp) r
//           [       0,          0, 0]
//
//  w = 7.2921151467 *10^-5 radians/second
//
//  Transform to space fixed frame
//
//  V(t) = P^-1 (t) N^-1 (t) v(t)
//-----------------------------------------------------------------------------
double funha(double ra, double last)
//-------------------------------------------------------------------------------
//  Input  : RA (radian), LAST (radian)
//  Output : Hour Angle (radian)
//-------------------------------------------------------------------------------
{
double temp;
	temp = last - ra;
	return fmod(temp,TWOPI);
}
//-------------------------------------------------------------------------------
void getxo(double longi,double lat, double h, double xo[],double *latgc)
//-------------------------------------------------------------------------------
//  Subroutine to calculate the observer's coordinates.
//
//  Input
//     long,lat : geodetic longitude & latitude in radian
//     h        : geodetic height in meter
//
// Output
//     xo       : rectangular coordinates (meter) in geocentric frame
//   latgc      : geocentric latitude
//
//  Note : If long is set to sidereal time, then xo is the observer's
//         rectangular coordinates in equatorial system
//-------------------------------------------------------------------------------
{
 double	c,s,coslon,sinlon,coslat,sinlat;
 const double   rearth = 6378.140e3;
 const double   e_flat = 0.00335281;

	coslat=cos(lat);
	sinlat=sin(lat);
	coslon=cos(longi);
	sinlon=sin(longi);

//	c = 1.0d0/sqrt(coslat**2 + (sinlat*(1.0d0-e_flat))**2);
	c = sinlat*(1.0-e_flat);
	c = c*c;
	c = 1.0/sqrt(c + coslat*coslat);

//	s= c * (1.0 - e_flat)**2
	s= c * (1.0 - e_flat)*(1.0-e_flat);

	c= rearth*c;
	s= rearth*s;

	xo[0] = (c+h)*coslat*coslon;
	xo[1] = (c+h)*coslat*sinlon;
	xo[2] = (s+h)*sinlat;

	*latgc = atan( xo[2]/sqrt(xo[0]*xo[0]+xo[1]*xo[1]));
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void precess(double jd, double x0[], double xt[])
//-------------------------------------------------------------------------------
//  input
//    jd : Julian date
//    x0 : vector in J2000 coordinates system
//  output
//    xt : vector corrected for precession from J2000 to the given epoch JD
//-------------------------------------------------------------------------------
{
 double t, a1,a2,a3,a4,a5,a6,a7,a8;
 double e1,e2,e3, se1,ce1,se2,ce2,se3,ce3;
 double p[3][3];
 int i,j;

	t = (jd-2451545.0)/36525.0;

	a1 =  FACT_DEG_TO_RAD*0.6406161;
	a2 =  FACT_DEG_TO_RAD*0.0000839;
	a3 =  FACT_DEG_TO_RAD*0.0000050;
	a4 =  0.0003041*FACT_DEG_TO_RAD;
	a5 =  0.0000051*FACT_DEG_TO_RAD;
	a6 =  0.5567530*FACT_DEG_TO_RAD;
	a7 = -0.0001185*FACT_DEG_TO_RAD;
	a8 = -0.0000116*FACT_DEG_TO_RAD;

	e1 = a1*t + a2*t*t + a3*t*t*t;
	e2 = a1*t + a4*t*t + a5*t*t*t;
	e3 = a6*t + a7*t*t + a8*t*t*t;

	se1 = sin(e1);
	ce1 = cos(e1);
	se2 = sin(e2);
	ce2 = cos(e2);
	se3 = sin(e3);
	ce3 = cos(e3);

//  transformation matrix p(i,j) from J2000.0 to the date

	p[0][0] =  ce1*ce3*ce2-se1*se2;
	p[0][1] = -se1*ce3*ce2-ce1*se2;
	p[0][2] = -se3*ce2;
	p[1][0] =  ce1*ce3*se2+se1*ce2;
	p[1][1] = -se1*ce3*se2+ce1*ce2;
	p[1][2] = -se3*se2;
	p[2][0] =  ce1*se3;
	p[2][1] = -se1*se3;
	p[2][2] =  ce3;

	xt[0] =  p[0][0] * x0[0] +  p[0][1] * x0[1] + p[0][2] * x0[2];
	xt[1] =  p[1][0] * x0[0] +  p[1][1] * x0[1] + p[1][2] * x0[2];
	xt[2] =  p[2][0] * x0[0] +  p[2][1] * x0[1] + p[2][2] * x0[2];
}
//-------------------------------------------------------------------------------
void nutate(double jd, double x0[], double x[])
//-------------------------------------------------------------------------------
//  For a given vector x0, this routine gives x, the vector corrected for
//  the effect of the nutation.
//
// Input 
//   JD    : Julian date
//   x0(3) : mean position vector
// Output 
//   x(3)  : true position vector, i.e. vector corrected for nutation
//------------------------------------------------------------------
{
 double dpsi,deps,eps;
	nutconst(jd,&dpsi,&deps,&eps);

	x[0] = x0[0] - dpsi*( x0[1]*cos(eps) + x0[2]*sin(eps) );
	x[1] = x0[1] + x0[0]*dpsi*cos(eps) - x0[2]*deps;
	x[2] = x0[2] + x0[0]*dpsi*sin(eps) + x0[1]*deps;
}
//-------------------------------------------------------------------------------
void  aberration(double jd, double *RA, double *Decl)
//-------------------------------------------------------------------------------
//      Purpose: Corrects equatorial coordinates for the effect of aberration.
//      Arguments
//        JD       : Julian date
//        Obl      : obliquity of the ecliptic
//        System   : either FK4System or FK5System
//        RA, Decl : coordinates to be corrected
//
//	approximate aberration correction
//-------------------------------------------------------------------------------
{
 double	Obl, cosObl, sinObl;
 double  Lon, e, pi, Tmp;
 double  cosRA, sinRA;
 double  cosDecl, sinDecl;
 double  cosLon, sinLon;

 double  AbConst, T;

	AbConst = 20.49552/3600.0;
	AbConst = deg_to_rad(AbConst);
	T = (jd - 2452645.0)/36525.0;

	Obl = funmobl(jd);
	cosObl = cos(Obl) ; sinObl = sin(Obl);
	cosRA  = cos(*RA)  ; sinRA  = sin(*RA);
	cosDecl= cos(*Decl); sinDecl= sin(*Decl);
	SunLonAndEcc(T, &Lon, &e);
	cosLon = cos(Lon) ; sinLon = sin(Lon);

	pi = (102.93735 + T*(1.71953 + T*0.00046));
	pi = deg_to_rad(pi);
	cosLon = cosLon - e*cos(pi);
	sinLon = sinLon - e*sin(pi);

	*RA  = *RA - AbConst*(cosRA*cosLon*cosObl+sinRA*sinLon) / cosDecl;
	Tmp  = cosLon*(sinObl*cosDecl-sinRA*sinDecl*cosObl) + cosRA*sinDecl*sinLon;
	*Decl= *Decl - AbConst*Tmp;
}
//-------------------------------------------------------------------------------
void anuaber(double jd, double p[])
//-------------------------------------------------------------------------------
//  This function corrects for the annual aberration.
//  For the correction it calculates first the velocity of the Earth.
//  The algorithm to correct for the annual aberratrion is based on
//  the rigorous formula in Astronomical Almanac.
//
//    Input
//       JD   : Julian date in TDB scale.
//       p[3] : there dimensional position vector
//    Output
//       p[3] : position vector corrected for the annual aberration.
//       
//                  Created by Han Inwoo  1998. Aug. 26
//-------------------------------------------------------------------------------
{
 double dist,ev[3],p2[3],beta,fundp;
 int i;

//  Make a unit vector of the input vector
    dist = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
    p[0] = p[0]/dist;
    p[1] = p[1]/dist;
    p[2] = p[2]/dist;

//  Calcualte the earth velocity
//  get_earth_vel(jd,ev);
    geterv       (jd,p2,ev);

    ev[0]= ev[0]/173.1446;
    ev[1]= ev[1]/173.1446;
    ev[2]= ev[2]/173.1446;

	beta = sqrt(1.0-(ev[0]*ev[0]+ev[1]*ev[1]+ev[2]*ev[2]));
	fundp = p[0]*ev[0] + p[1]*ev[1] + p[2]*ev[2];

	for(i=0;i<3;i++) p2[i] = (beta*p[i] + ev[i]*(1.0 + fundp/(1.0+beta)))
					   / (1.0 + fundp);

//  Recover the norm of the input vector
	p[0]= dist*p2[0];
	p[1]= dist*p2[1];
	p[2]= dist*p2[2];
}
//-------------------------------------------------------------------------------
void get_earth_vel_apr(double jd, double v[])
//-------------------------------------------------------------------------------
//  This function calculates the velocity of the Earth with respective to
//  the solar system barycenter.
//  The algorithm to calculate the velocity is Fourier coefficients of the
//  velocity. The coefficients are calculated by the program "calevcoe.for".
//  The coefficients is stored in "EV_COE.DAT".
//  For the detail of the algorithm, refer to the comments in "CALEVCOE.FOR".
//
//  Input
//   JD : Julian date in TDB scale
//  Output
//   v  : the earth velocity vector with respect to the solar system barycenter.
//        unit is A.U. / day.
//
//                    Created by Han Inwoo  1998. Aug. 26
//-------------------------------------------------------------------------------
{
 static double coeff[9][3],jd0,omega,omega2,omega3,omega4;
 static int first = 0;
 FILE *fin;
 double rms,dt;
 int i,j;

	if(first==0)
      {
	   if((fin = fopen("ev_coe.dat","r"))==NULL)
         {puts("cannot open EV_COE.DAT....\n");
          exit(0);}

       fscanf(fin,"%lf %lf", &omega, &jd0);
       omega2 = omega*2.0;
       omega3 = omega*3.0;
       omega4 = omega*4.0;

       for(j=0;j<3;j++)
          {for(i=0;i<9;i++) fscanf(fin,"%lf", &coeff[i][j]);
           fscanf(fin,"%lf", &rms);
          }
       first = 1;
       fclose(fin);
      }

      dt = jd - jd0;
      for(j=0;j<3;j++)
         {v[j] = coeff[0][j]
               + coeff[1][j]*cos(omega *dt) + coeff[2][j]*sin(omega *dt) 
               + coeff[3][j]*cos(omega2*dt) + coeff[4][j]*sin(omega2*dt) 
               + coeff[5][j]*cos(omega3*dt) + coeff[6][j]*sin(omega3*dt) 
               + coeff[7][j]*cos(omega4*dt) + coeff[8][j]*sin(omega4*dt);
         }
}
//-------------------------------------------------------------------------------
int get_earth_pos_apr(double jd_ut, double *XI, double *YI, double *ZI)
//-------------------------------------------------------------------------------
//   Calculate the Earth's rectangular position w.r.t. the SSB. unit is A.U.
//   The input data file is "CALEPCOE.DAT" from CALEPCOE.FOR
//
//                   created by  Han Inwoo                   1998. Dec. 29.
//-------------------------------------------------------------------------------
{
 #define NIN 10
 int ndata,ii;
 double t[NIN],x[NIN],y[NIN],z[NIN];
 double del_x,del_y,del_z;
 char TEXT_BUFF[200];
 

 FILE *fin;
 int i,e_code;

 double delta;

    if((fin = fopen("calepcoe.dat","r"))==NULL)
       {MessagePopup(""," CANNOT OPEN CALEPCOE.DAT");
        return -1;
       }

    fgets(TEXT_BUFF,150,fin);
    sscanf(TEXT_BUFF,"%d",&ndata);

    e_code = -2;
    i = 0;
    while(fgets(TEXT_BUFF,150,fin)!=NULL)
	     {
          sscanf(TEXT_BUFF,"%d %lf %lf %lf %lf",&ii,&t[i],&x[i],&y[i],&z[i]);
          if(t[i]>(jd_ut-NIN/2.0)) {e_code=0; break;}
         }

    if(e_code!=0) {fclose(fin); return e_code;}

    for(i=1;i<NIN;i++)
       {
        if(fgets(TEXT_BUFF,150,fin)==NULL) {e_code=3; break;}
        sscanf(TEXT_BUFF,"%d %lf %lf %lf %lf",&ii,&t[i],&x[i],&y[i],&z[i]);
       }
       
    fclose(fin);
    if(e_code!=0) return e_code;

    polint(t,x ,NIN,jd_ut,XI,&del_x);
    polint(t,y ,NIN,jd_ut,YI,&del_x);
    polint(t,z ,NIN,jd_ut,ZI,&del_x);
    
    return 0;
}
//-------------------------------------------------------------------------------
int get_pi_factor(double jd_ut, double RA, double Dec, double *pi_alp, double *pi_dec)
//-------------------------------------------------------------------------------
//                   created by  Han Inwoo                   1998. Dec. 29.
//-------------------------------------------------------------------------------
{
 double X,Y,Z,rrd[6];
 int e_code; 
 
//    e_code = get_earth_pos(jd_ut,&X,&Y,&Z);
//    if(e_code!=0) return e_code;

//  pleph(jd_ut,3,11,rrd);
    geterv(jd_ut, rrd,&rrd[3]);
    
    X = rrd[0];
    Y = rrd[1];
    Z = rrd[2];
    
//	*pi_alp  = Y*dcos(RA) - X*dsin(RA)
//	*pi_dec  = Z*dcos(Dec)- X*dcos(RA)*dsin(Dec) - Y*dsin(RA)*dsin(Dec)

	*pi_alp  =-Y*cos(RA)  + X*sin(RA);
	*pi_dec  =-Z*cos(Dec) + X*cos(RA)*sin(Dec)   + Y*sin(RA)*sin(Dec);
	
	return 0;
}	
//-------------------------------------------------------------------------------
void refract(double *alt)
//-------------------------------------------------------------------------------
//  Correct the atmospheric refraction.
//  Formula from Astronomical Algorithm p 102
//  When exit, the input altitude is corrected. Altitude in radian.
//-------------------------------------------------------------------------------
{
 double dalt, altp;
	dalt =  1.02/tan( *alt + deg_to_rad(10.3/(rad_to_deg(*alt)+5.11)) );
	*alt  =  *alt + deg_to_rad( (dalt+0.0019279)/60.0 );
}
//-------------------------------------------------------------------------------
double funmobl(double jd)
//-------------------------------------------------------------------------------
//      This function calculates the mean obiliquity at t = jd.
//-------------------------------------------------------------------------------
{
 double t, funmob;

	t = (jd-2451545.0)/36525.0;
	funmob = FACT_DEG_TO_RAD
	   * ( 23.439291 - 0.0130042*t - 0.00000016*t*t + 0.000000504*t*t*t);
	return funmob;
}
//-------------------------------------------------------------------------------
void nutconst(double jd, double *delphi, double *deleps, double *eps)
//-------------------------------------------------------------------------------
{
 static double nt[64][10];
 double eps0,t,t2,t3;
 double m,mp,d,f,omega;
 int i,j;
 static int first = 0;
 FILE *fin;

	if(first==0)
	  {
	   if((fin = fopen("nutconst.dat","r"))==NULL)
         {puts("cannot open NUTCONST.DAT....\n");
          exit(0);
         }
//     printf(" reading nutconst.dat .... \n");

	for(i=1;i<=63;i++)
       { for(j=1;j<=9;j++)  fscanf(fin,"%lf", &nt[i][j]);
//               for(j=1;j<=9;j++)  printf("%lf",nt[i][j]); 
//                   printf(" \n");
                 }
		first = 1;
		fclose(fin);
	  }

		t=(jd-2451545.0)/36525.0;
		t2 = t*t;
		t3 = t2*t;

/*
c        d=297.85036d0+445267.111480d0*t-0.19142d-2*t**2
c     &     +t**3/189474.d0
c        m=357.52772d0+35999.050340d0*t-0.1603d-3*t**2-t**3/3.d5
c        mp=134.96298d0+477198.867398d0*t+0.86972d-2*t**2+t**3/56250d0
c        f=93.27191d0+483202.017538d0*t-0.36825d0*t**2+t**3/327270.d0
c        omega=125.04452d0-1934.136261d0*t+0.20708d-2*t**2+t**3/45.d4
*/

        d     = 297.85036 +445267.11148*t -0.19142e-2*t2 + t3/189474.0;
        m     = 357.52772 +35999.050340*t -0.1603e-3*t2 -t3/3.0e5;
        mp    = 134.96298 +477198.867398*t +0.86972e-2*t2 +t3/56250;
        f     = 93.27191 +483202.017538*t -0.36825*t2 +t3/327270.0;
        omega = 125.04452 -1934.136261*t +0.20708e-2*t2 +t3/45.0e4;

        d     = FACT_DEG_TO_RAD*d;
        m     = FACT_DEG_TO_RAD*m;
        mp    = FACT_DEG_TO_RAD*mp;
        f     = FACT_DEG_TO_RAD*f;
        omega = FACT_DEG_TO_RAD*omega;

        *delphi = 0.0;  *deleps = 0.0;

        for(i=1;i<=63;i++)
           { *delphi = *delphi+(nt[i][6]+nt[i][7]*0.1*t)*1.0e-4
                       * sin(nt[i][1]*mp+nt[i][2]*m+nt[i][3]*f+nt[i][4]*d
                       + nt[i][5]*omega);
             *deleps = *deleps+(nt[i][8]+nt[i][9]*0.1*t)*1.0e-4
                       * cos(nt[i][1]*mp+nt[i][2]*m+nt[i][3]*f+nt[i][4]*d
                       + nt[i][5]*omega);
           }

        *delphi = FACT_DEG_TO_RAD*(*delphi/3600.0);
        *deleps = FACT_DEG_TO_RAD*(*deleps/3600.0);

        eps0  = funmobl(jd);
        *eps  = eps0 + *deleps;
}
//-------------------------------------------------------------------------------
void SunLonAndEcc(double T, double *Lon, double *e)
//-------------------------------------------------------------------------------
{
	double	L, M, C, e2, tem;

	L  = (280.46646 + T*(36000.76983 + T*0.0003032));
	M  = (357.52910 + T*(35999.05028 - T*0.0001561));
	L  = deg_to_rad(L);
	M  = deg_to_rad(M);
	tem= 0.016708617 - T*(0.000042040 + T*0.0000001236);
	*e = tem;
	e2 = tem*tem;
//      Equation of the center, in terms of e and M
	C =  tem*(2-0.25*e2) * sin(M) + 1.25*e2 * sin(2.0*M)
           + 1.0833333333*tem*e2*sin(3.0*M);
	*Lon = L + C;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void inverse_precess(double jd, double xt[], double x0[])
//-------------------------------------------------------------------------------
//  input
//    jd : Julian date
//    xt : vector corrected for precession from J2000 to the given epoch JD
//  output
//    x0 : vector in J2000 coordinates system
//
//                 Created and Verified by Han Inwoo, 1997. Sep. 10
//-------------------------------------------------------------------------------
{
	double t, a1,a2,a3,a4,a5,a6,a7,a8;
	double e1,e2,e3, se1,ce1,se2,ce2,se3,ce3;
	double p[3][3];
	int i,j;

	t = (jd-2451545.0)/36525.0;

		a1 =  FACT_DEG_TO_RAD*0.6406161;
		a2 =  FACT_DEG_TO_RAD*0.0000839;
		a3 =  FACT_DEG_TO_RAD*0.0000050;
		a4 =  0.0003041*FACT_DEG_TO_RAD;
		a5 =  0.0000051*FACT_DEG_TO_RAD;
		a6 =  0.5567530*FACT_DEG_TO_RAD;
		a7 = -0.0001185*FACT_DEG_TO_RAD;
		a8 = -0.0000116*FACT_DEG_TO_RAD;

		e1 = a1*t + a2*t*t + a3*t*t*t;
		e2 = a1*t + a4*t*t + a5*t*t*t;
		e3 = a6*t + a7*t*t + a8*t*t*t;

		se1 = sin(e1);
		ce1 = cos(e1);
		se2 = sin(e2);
		ce2 = cos(e2);
		se3 = sin(e3);
		ce3 = cos(e3);

//      transformation matrix p(i,j) from J2000.0 to the date

		p[0][0] =  ce1*ce3*ce2-se1*se2;
		p[0][1] = -se1*ce3*ce2-ce1*se2;
		p[0][2] = -se3*ce2;
		p[1][0] =  ce1*ce3*se2+se1*ce2;
		p[1][1] = -se1*ce3*se2+ce1*ce2;
		p[1][2] = -se3*se2;
		p[2][0] =  ce1*se3;
		p[2][1] = -se1*se3;
		p[2][2] =  ce3;

//		xt[0] =  p[0][0] * x0[0] +  p[0][1] * x0[1] + p[0][2] * x0[2];
//		xt[1] =  p[1][0] * x0[0] +  p[1][1] * x0[1] + p[1][2] * x0[2];
//		xt[2] =  p[2][0] * x0[0] +  p[2][1] * x0[1] + p[2][2] * x0[2];

		x0[0] =  p[0][0] * xt[0] +  p[1][0] * xt[1] + p[2][0] * xt[2];
		x0[1] =  p[0][1] * xt[0] +  p[1][1] * xt[1] + p[2][1] * xt[2];
		x0[2] =  p[0][2] * xt[0] +  p[1][2] * xt[1] + p[2][2] * xt[2];

}
//-------------------------------------------------------------------------------
void inverse_nutate(double jd, double x[], double x0[])
//-------------------------------------------------------------------------------
//
// Input 
//   JD    : Julian date
//   x(3)  : true position vector, i.e. vector corrected for nutation
// Output 
//   x0(3) : mean position vector
//
//                 Created and Verified by Han Inwoo, 1997. Sep. 10
//-------------------------------------------------------------------------------
{
	double dpsi,deps,eps,m[3][3];
	nutconst(jd,&dpsi,&deps,&eps);

//	x[0] = x0[0] - dpsi*( x0[1]*cos(eps) + x0[2]*sin(eps) );
//	x[1] = x0[1] + x0[0]*dpsi*cos(eps) - x0[2]*deps;
//	x[2] = x0[2] + x0[0]*dpsi*sin(eps) + x0[1]*deps;

        m[0][0] = 1.0;
        m[0][1] = -dpsi*cos(eps);
        m[0][2] = -dpsi*sin(eps);

        m[1][0] = dpsi*cos(eps);
        m[1][1] = 1.0;
        m[1][2] = -deps;

        m[2][0] = dpsi*sin(eps);
        m[2][1] = deps;
        m[2][2] = 1.0;

	x0[0] = m[0][0]*x[0] + m[1][0]*x[1] + m[2][0]*x[2];
	x0[1] = m[0][1]*x[0] + m[1][1]*x[1] + m[2][1]*x[2];
	x0[2] = m[0][2]*x[0] + m[1][2]*x[1] + m[2][2]*x[2];
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
double get_jd(int year,int month,int day, double rhour)
//-------------------------------------------------------------------------------
{
 double a,b,m,y;
 double d;
 int i1,i2;
 double funjd, ff;

    d = day;

	if(month > 2) {	y = year;     m = month;     }
	else          {	y = year - 1; m = month + 12;}

	a = floor(y/100.0);

	if ( (year<1582)      || ((year==1582) && (month<10))
                          || (year==1582 && month==10 && day<=4) ) b = 0;
	else
	{
		b = 2 - a + floor(a/4.0);
	}

	funjd =    floor(365.25 * (y + 4716))
	         + floor(30.6001 * (m + 1))
		     + d + b - 1524.5
             + rhour/24.0;

    return funjd;
}
//-------------------------------------------------------------------------------
void jdtoc(double jd, int flag, int *year, int *month, int *day, double *rhour)
//-------------------------------------------------------------------------------
//        flag :  -1  ==>  All dates expressed in Julian    calendar
//                +1  ==>  All dates expressed in Gregorian calendar
//                 0  ==>  Automatic mode.
//                         Julian    calendar before Oct. 4, 1582
//                         Gregorian calendar after  Oct. 4, 1582
//-------------------------------------------------------------------------------
{
 long int date[6];
 double secs;
 double dt,dja;
 long int j,iy,idy,m;


	dt = jd + 0.5000000001000;
	j = floor(dt);
	dja=j*1.0;
	dt=(dt-dja)*24.0;
	date[4]=floor(dt);

	dja=date[4];
	dt=(dt-dja)*60.0;
	date[5]=floor(dt);

	dja = date[5];
	secs= (dt-dja)*60.0;
                                                                               
/*  IF(FLAG.LT.0 .OR. (FLAG.EQ.0 .AND. JD.LT.2299160.5)) */
	if( (flag<0) || ((flag==0)&&(jd<2299160.50)) )
	{
	iy =4*(j+105498)/1461-5001;
	idy=j-(36525*(iy+5000))/100+105133;
	}
	else
	{iy =4*(j-1720754+3*(4*(j-1721119)/146097+1)/4)/1461-1;
        idy=j-36525*iy/100+3*(iy/100+1)/4-1721119;
	}

	m=(5*idy-3)/153 + 3;
	date[2]=idy-(153*(m-3)+2)/5;
	if(m>=13) m=m-12;
	if(m<=2) iy=iy+1;
                                                                               
	date[1]=m;
	date[3]=iy;

	*year  = date[3];
	*month = date[1];
	*day   = date[2];
	*rhour = date[4] + date[5]/60.0 + secs/3600.0;
}
//-------------------------------------------------------------------------------
double funtdb(double tdt)
//-------------------------------------------------------------------------------
{
 double g,tdb;

	g   = deg_to_rad(357.53 + 0.9856003*(tdt-2451545.0) );
	tdb = tdt + (0.001658*sin(g)+0.000014*sin(2.0*g))/24.0/3600.0;
	return tdb;
}
//-------------------------------------------------------------------------------
double jdtogmst(double jd)
//-------------------------------------------------------------------------------
//      Input  : jd in UT scale
//      Output : Greenwich Mean Siderial Time (radian)
//-------------------------------------------------------------------------------
{
	double ut,gmst0ut,temp,Tu,modpi2,dtor;

/*	calculate jd at UT = 0  
	ijd = jd;
	temp = ijd + 0.5;
*/
	modf(jd,&temp); temp = temp + 0.5;
	
/*	Calculate UT in hour     */
	ut = (jd - temp)*24.0;
	Tu = ( temp - 2451545.0 ) / 36525.0;

	gmst0ut = ( 24110.54841 + 8640184.812866*Tu
                 + 0.093104*Tu*Tu - 6.2e-6*Tu*Tu*Tu ) / 3600.0
                 + 1.0027379093*ut;

/*	convert hour to radian               */
/*	return  modpi2( dtor(gmst0ut*15.0) ) */
	temp = deg_to_rad(gmst0ut*15.0);
	dtor = fmod(temp,TWOPI);
	if(dtor < 0.0) dtor = dtor + TWOPI;
	return dtor;
}
//-------------------------------------------------------------------------------
double jdtogast(double jd)
//-------------------------------------------------------------------------------
//  Input  : jd in UT scale
//  Output : Greenwich Apparent Siderial Time (radian)
//-------------------------------------------------------------------------------
{
	double dpsi,deps,eps,temp;
	nutconst(jd,&dpsi,&deps,&eps);
	temp = jdtogmst(jd) + dpsi*cos(eps); 
	return	fmod(temp, TWOPI);
}
//-------------------------------------------------------------------------------
double jdtolast(double jd, double longi)
//-------------------------------------------------------------------------------
//  Input  : jd in UT scale
//  Output : Greenwich Apparent Siderial Time (radian)
//-------------------------------------------------------------------------------
{
	double dpsi,deps,eps,temp; int ii;
	nutconst(jd,&dpsi,&deps,&eps);
	temp = jdtogmst(jd) + dpsi*cos(eps) + longi; 

	return	fmod(temp, TWOPI);
//	ii = temp/TWOPI;
//	temp = temp - TWOPI*ii;
//	if(temp<0.0) temp = temp + TWOPI;
//	return temp;
}
//-------------------------------------------------------------------------------
double jdtolmst(double jd, double longi)
//-------------------------------------------------------------------------------
//  Input  : jd in UT scale
//  Output : Greenwich Apparent Siderial Time (radian)
//-------------------------------------------------------------------------------
{
 double temp;
	temp = jdtogmst(jd) + longi; 
	return	fmod(temp, TWOPI);
}
//-------------------------------------------------------------------------------
double get_jdutc_clock()
//-------------------------------------------------------------------------------
{
 double temp,rhour;
 int year,month,day,hour,minute,second,hsecond;
 
    read_computer_time(&year,&month,&day,&hour,&minute,&second,&hsecond);
    rhour = hour + minute/60.0 + (second + hsecond/100.0) / 3600.0;
    temp  = get_jd(year,month,day,rhour) - 9.0/24.0;

    return temp;
}
//-------------------------------------------------------------------------------
double get_jdutc_clock2(double interv_sec)
//-------------------------------------------------------------------------------
{
 double temp,rhour;
 int year,month,day,hour,minute,second,hsecond;
 
    read_computer_time(&year,&month,&day,&hour,&minute,&second,&hsecond);
    rhour = hour + minute/60.0 + (second + interv_sec + hsecond/100.0) / 3600.0;
    temp  = get_jd(year,month,day,rhour) - 9.0/24.0;

    return temp;
}
//-------------------------------------------------------------------------------
void  read_computer_time(int *year, int *month, int *day,
	     int *hour, int *minute ,int *second, int *hsecond)
//-------------------------------------------------------------------------------
{   GetSystemDate(month,day,year);
    GetSystemTime(hour,minute,second);
    *hsecond = 50;
}
//-------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
int  get_planet_position_apr(int nbody, double jd_ut,double *ra_2000, double *de_2000, double *disti)
//-----------------------------------------------------------------------------
//   calculate a planet's astrometric position w.r.t. J2000 equinox.
//
//                                          created by Han Inwoo, 97. Nov. 29
//-----------------------------------------------------------------------------
//    1 = MERCURY           8 = NEPTUNE                              
//    2 = VENUS             9 = PLUTO                                
//    3 = EARTH            10 = MOON                                 
//    4 = MARS             11 = SUN                                  
//    5 = JUPITER          12 = SOLAR-SYSTEM BARYCENTER              
//    6 = SATURN           13 = EARTH-MOON BARYCENTER                
//    7 = URANUS  
//-----------------------------------------------------------------------------
{
 #define NIN 10
 double t[NIN],ra[NIN],dec[NIN],dist[NIN];
 double tdt0,tdtend;

 FILE *fin;
 int i,e_code;

 double delta;
 char fname[15];

        sprintf(fname,"planet%2d.dat",nbody);
        if(fname[6]==' ')fname[6]='0';

        if((fin = fopen(fname,"r"))==NULL)
          {printf(" CANNOT OPEN %s \n\a",fname);
//           getch();
           return -1;}

        fscanf(fin,"%lf %lf",&tdt0,&tdtend);
        if(jd_ut<tdt0 || jd_ut>tdtend)
          {printf(" Epoch out of range of Planet interpolation....\n\a");
		   printf(" Hit any key to continue>");
//		   getch(); 
           return -1;
          }

        e_code = -2;
        i = 0;
        while(fscanf(fin,"%lf %lf %lf %lf",&t[i],&ra[i],&dec[i],&dist[i])!=EOF)
             {//printf("%3d %15.7lf %15.7lf %15.7lf\n",i,t[i],ra[i],dec[i]);
              if(t[i]>(jd_ut-NIN/2.0))
                {//printf("%3d %15.7lf %15.7lf %15.7lf %15.7lf\n",
                 // i,t[i],ra[i],dec[i],dist[i]);
                 e_code=0; break;
                }
             }

        if(e_code!=0) {fclose(fin); return e_code;}

        for(i=1;i<NIN;i++)
           {if(fscanf(fin,"%lf %lf %lf %lf",&t[i],&ra[i],&dec[i],&dist[i])==EOF)
              {e_code = -3; break;}
            }

        fclose(fin);
        if(e_code!=0) return e_code;

        adjust_ra(ra,NIN);
        polint(t,ra  ,NIN,jd_ut,ra_2000,&delta);
        polint(t,dec ,NIN,jd_ut,de_2000,&delta);
        polint(t,dist,NIN,jd_ut,disti  ,&delta);
        return 0;
}
//----------------------------------------------------------------------------
void adjust_ra(double ra[], int ndata)
//-----------------------------------------------------------------------------
{
 int i;

        for(i=1;i<ndata;i++)
           {
            if((ra[i]-ra[0]) >  PI314) ra[i] = ra[i] - TWOPI;
            if((ra[i]-ra[0]) < -PI314) ra[i] = ra[i] + TWOPI;
           }
}
//-----------------------------------------------------------------------------
void get_stco(double ALPHA0, double DELTA0, double ALPHA, double DELTA,double *XSI, double *ETA)
//-----------------------------------------------------------------------------
{
 double DEN;

	DEN = sin(DELTA0)*sin(DELTA) 
            + cos(DELTA0)*cos(DELTA)*cos(ALPHA-ALPHA0);

	*XSI =   cos(DELTA)*sin(ALPHA - ALPHA0)/DEN;
	*ETA = ( cos(DELTA0)*sin(DELTA) - sin(DELTA0)*cos(DELTA)*cos(ALPHA-ALPHA0) )/DEN;
}
//-----------------------------------------------------------------------------
void st_to_sp(double ALPHA0, double DELTA0, double XSI, double ETA, double *ALPHA, double *DELTA)
//-----------------------------------------------------------------------------
//  INPUT : ALPHA0, DELTA0  ; TANGENT POINT             
//	    XSI   , ETA     ; STANDARD COORDINATES      
//  OUTPUT : ALPHA , DELTA   : SPHERICAL COORDINATES    
//  ALL THE UNITS ARE IN RADIAN		        
//-----------------------------------------------------------------------------
{
 double cosD, sinD, B, A;

	cosD = cos(DELTA0);
	sinD = sin(DELTA0);

	B = cosD - ETA*sinD;

	*ALPHA = ALPHA0 + atan(XSI/B);

	A = ( sinD + ETA*cosD )* cos(ALPHA0 - *ALPHA);

	*DELTA = atan(A/B);
}
//-----------------------------------------------------------------------------
int correct_rv_test(double jd, double longi, double lati, double height,
               double rarad, double decrad, double *jd_bc, double *rv_1, double *rv_2)
//-----------------------------------------------------------------------------
//  height in meter
//  rv_obs in km/s
//-----------------------------------------------------------------------------
{
 double v_ec[3],v_te[3],v_tc[3],v_obs[3];
 double r_ec[3];
 
 double omega,r_obs[3];
 double latgc;
 const  double aumeter=1.4959787066e11;
 double temp,lmst;
 
 double n0[3],n1[3];
 double rv1,rv2;
 
//  rotation velocity of Earth
    omega=7.2921151467e-5;  // radian/sec
     
//  unit vector to target in J2000
    n0[0] = cos(decrad)*cos(rarad);
    n0[1] = cos(decrad)*sin(rarad);
    n0[2] = sin(decrad);

//  unit vector to target in precession
    precess(jd,n0,n1);

//  velocity of earth center
//  init_jpl("c:\\lib\\jplephs.dos");
    geterv(jd, r_ec, v_ec);
//  close_jpl();
    
//  convert au/day to m/s
    v_ec[0] *= aumeter/86400;
    v_ec[1] *= aumeter/86400;
    v_ec[2] *= aumeter/86400;
    
    rv1 = dproduct(v_ec,n0)/1000.0;
//  rv1 = dproduct(v_ec,n1)/1000.0;
    *rv_1 = rv1;
    
//  velocity of topocenter
    lmst = jdtolmst(jd,longi);
    getxo(lmst,lati,height,r_obs,&latgc);
    temp = r_obs[0]*r_obs[0] + r_obs[1]*r_obs[1];
    temp = sqrt(temp)*omega/1000.0;
    
    rv2 = temp*(-sin(lmst)*n1[0] + cos(lmst)*n1[1]);
    *rv_2 = rv2;
    
//  position of earth's center
    temp = aumeter*dproduct(r_ec,n0) + dproduct(r_obs,n1);
//  temp = aumeter*dproduct(r_ec,n0);
    *jd_bc = jd + temp/299792458.0/86400;
    
//    sprintf(buff," rv correction %15.9lf %14.9lf %14.9lf\n",rv1,rv2,rv1+rv2);
//    display_message(buff);   
    
    return 0;
}
//-----------------------------------------------------------------------------
void getdhms(double rday, int *day, int *hour, double *minute)
//--------------------------------------------------------------------------//
{
	double temp;

	*day = (int)rday;
	temp = (rday - *day) * 24.0;
	*hour = (int)temp;
	*minute = (temp - *hour) * 60.0;

	//return 0;
}
//--------------------------------------------------------------------------//
//tdt -> ut -> kst
//--------------------------------------------------------------------------//
double tdttokst(double tdt)
//--------------------------------------------------------------------------//
{
	double ut, result;

	ut = tdt - adeltat(tdt)/86400.0;
	result = ut + 9.0/24.0;
	return result;
}
//--------------------------------------------------------------------------//
double adeltat(double jd)
//--------------------------------------------------------------------------//
//input : Julian Date
//output : deltat(sec) = TDT - UT at JD(1984-2000)
//output : deltat (sec) = TT - UT at JD(after 2001)
{
	double t;
	double y;
	int index;
	double result;
	double DTable[194] = {   
		  12400,11500,10600, 9800, 9100, 8500, 7900, 7400, 7000, 6500,
          6200, 5800, 5500, 5300, 5000, 4800, 4600, 4400, 4200, 4000,
          3700, 3500, 3300, 3100, 2800, 2600, 2400, 2200, 2000, 1800,
          1600, 1400, 1300, 1200, 1100, 1000,  900,  900,  900,  900,
           900,  900,  900,  900, 1000, 1000, 1000, 1000, 1000, 1100,
          1100, 1100, 1100, 1100, 1100, 1100, 1100, 1200, 1200, 1200,
          1200, 1200, 1300, 1300, 1300, 1300, 1400, 1400, 1400, 1500,
          1500, 1500, 1500, 1600, 1600, 1600, 1600, 1600, 1700, 1700,
          1700, 1700, 1700, 1700, 1700, 1700, 1600, 1600, 1500, 1400,
          1370, 1310, 1270, 1250, 1250, 1250, 1250, 1250, 1250, 1230,
          1200, 1140, 1060,  960,  860,  750,  660,  600,  570,  560,
           570,  590,  620,  650,  680,  710,  730,  750,  770,  780,
           788,  754,  640,  541,  292,  161, -102, -269, -364, -471,
          -540, -520, -546, -563, -580, -587, -619, -644, -609, -466, 
          -272,   -2,  264,  537,  775, 1046, 1336, 1601, 1824, 2025,
          2116, 2241, 2349, 2386, 2434, 2402, 2387, 2386, 2373, 2396,
          2433, 2530, 2624, 2728, 2825, 2915, 2997, 3072, 3135, 3218, 
          3315, 3400, 3503, 3654, 3829, 4018, 4223, 4449, 4646, 4853,
          5054, 5217, 5379, 5487, 5582, 5686, 5831, 5998, 6163, 6297,
          6400, 6600, 6800, 7000 
		  };

	t = (jd - 2451545.0)/36525.0;
	y = 2000.0 + t * 100.0;;

	if(y > 2007.0)
		result = 102.3 + t*(123.5 + t*32.5);
	else
	{
		if(y < 1620.0)
		{
			if(y < 948.0)
				result = 2715.6 + t*(573.36 + t*46.5);
			else 
				result = 50.6 + t*(67.5 + t*22.5);
		}
		else
		{
			index = (int)((y - 1620.0)/2.0 + 0.5);
			if(index > 192)
				index = 192;

			y = y/2.0 - index - 810;
			result = (DTable[index] + (DTable[index+1] - DTable[index])*y)/100.0;
		}
	}

	return result;
}
//--------------------------------------------------------------------------//

