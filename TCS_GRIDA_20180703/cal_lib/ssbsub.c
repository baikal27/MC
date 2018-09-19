////////////////////////////////////////////////////////////////////////////////////////////////
//Program name : SSBSUB.for
//  SSBSUB.FOR���� PLEPH���� ���� õü�� J2000 �������� ��ġ�� ������� ���̿� �µ��� ��ǥ��
//  ��ȯ�Ͽ� �ִ� �����α׷����� ���ִ�.
//  SSBSUB������ ��κ��� ���� ����ڴ� GCSBPOS (GeoCentric Sola System Body's Position)����
//  �̿��� ���̴�. �ٸ� ���α׷����� GCSBPOS ���࿡ �ʿ��� ���α׷����̴�. 
//  GCSBPOS�� ���� �߽ɿ��� �� �¾�� õü���� ��ġ�� ����Ͽ� �ִ� ��,
//  ����ڴ� �������� ������ ��ǥ���� ������ �� �ִ�.
//  GCSBPOS�� ������ ����� �Ķ��Ÿ�� ������ ����.
//
//  ������ : SUBROUTINE GCSBPOS(TDT,NBODY,OPT1,OPT2,OPT3, RPOS)
//
//  ���Է� �Ķ��Ÿ 
//    real*8  TDT   : �ð�, PLEPH�� ���� ���� ����
//    integer NBODY : õü��ȣ, PLEPH�� ���� ���� ����
//    integer OPT1, OPT2, OPT3 : ��ǥ�� ���� ���� ����
//      opt1 : 1 => Geometric, 2 => Astrometric,  Others => Apparent
//      opt2 : 1 => J2000    , 2 => Mean equinox, Others => True equinox
//      opt3 : 1 => ������ǥ, �ٸ� ��� => ����ǥ 
//  
//  ����� �Ķ��Ÿ 
//    real*8 rpos(3) : õü�� ��ǥ
//    OPT3=1�̸� RPOS�� ������ǥ (x,y,z)�� �־����� �ٸ� ���� ����ǥ (ra,dec,distance)�� �־�����.
//          (������ ���� A.U.�� radian.)
//
// ����� ��
//   J1994.2 (TDT)�� �ݼ��� ��ǥ�� �Ѻ��� ��ġ��, �� �ð��� ��� ����� ��ǥ�迡 ���Ͽ�,
//   ������ǥ�� ���·� ��� ������ ������ ���� �Ķ���͸� �����Ѵ�.
//
//          TDT = 1984.2, NBODY = 2, OPT1 = 3, OPT2 = 2, OPT3 = 1
////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#include "type_def.h"
#include "jpl_sub.h"
#include "ssbsub.h"
#include "cpabtsub.h"
#include "mathsub.h"

//-----------------------------------------------------------------------------
void gcsbpos(double tdt, int nbody, int opt1, int opt2, int opt3, double rpos[])
//-----------------------------------------------------------------------------
// This routine calculates Geocentric Solar system body's position..
//
// input: tdt -> time in TDT scale
//		  nbody : 
//		  1:Mercury, 2:Venus, 3:Earth, 4:Mars, 5:Jupiter, 6:Saturn
//		  7:Uranus, 8:Neptune, 9:Pluto, 10:Moon, 11:Sun, 12:Solar-System Barycenter
//		  13 : Earth-Moon Barycenter
//		  opt1 -> Origin      1:Geometric, 2:Astrometric, Others:Apparent
//		  opt2 -> Equinxox    1:J2000, 2:Mean equinox, Others : True equinox
//		  opt3 -> Coordinates 1:Cartesian coordimates, 
//							  Others : polar coordinates, ra, dec, distance
//
//output : rpos[3] : coordinates,(x,y,z) or (ra, dec, dist)
//Distance of rpos is taken as geometric distance of the body at TDT
//-----------------------------------------------------------------------------
{
	if(nbody == 3 || nbody > 13)
	{
		printf("Wrong body no. in GCSBPOS...\n");
		return;
	}

	if(nbody == 11)
		ssunpos(tdt, opt1, opt2, opt3, rpos);
	else
		splpos(tdt, nbody, opt1, opt2,opt3, rpos);
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void geterv(double tdt, double erb[], double evb[])
//-----------------------------------------------------------------------------
{
int targ, cent;
double rrd[6];
	
    cent = 12;
    targ = 3;
    pleph(tdt, targ, cent, rrd);
	
    erb[0] = rrd[0];
    erb[1] = rrd[1];
    erb[2] = rrd[2];
	
    evb[0] = rrd[3];
    evb[1] = rrd[4];
    evb[2] = rrd[5];
}
//-----------------------------------------------------------------------------
void getsb(double tdt, double sb[])
//-----------------------------------------------------------------------------
//input : Julian date in TDT scale
//output : sb --> ��ġ���� x,y,z�� ����
{
	int targ, cent;
	double rrd[6];

	cent = 12;
	targ = 11;

	pleph(tdt, targ, cent, rrd);
	sb[0] = rrd[0];
	sb[1] = rrd[1];
	sb[2] = rrd[2];
}
//-----------------------------------------------------------------------------
void getqb(double tdb, int npl, double qb[])
//-----------------------------------------------------------------------------
{
 int targ, cent;
 double rrd[6];

	cent = 12;
	targ = npl;
	pleph(tdb, targ, cent, rrd);
	qb[0] = rrd[0];
	qb[1] = rrd[1];
	qb[2] = rrd[2];
}
//-----------------------------------------------------------------------------
void ssunpos(double tdt, int opt1, int opt2, int opt3, double rpos[])
//-----------------------------------------------------------------------------
{
 const int npl = 11;
 const double cvel = 173.1446;
 const double muc2 = 9.87e-9;
 double ra, dec, dist;

 double er[3], ev[3], sb[3], p[3];
 double p1[3], p2[3], p3[3];
 double tau0, tau, dtau, beta, time;

 double tdb;
 int i;
	
	tdb= tdt;

	geterv(tdb, er, ev);
	getsb(tdb, sb);

	vectsub(sb, er, p);
	tau0 = funr(p) / cvel;

	dist = funr(p);

	//opt1�� ���� ���� ���...
	//Geometric position
	if(opt1 == 1)
	  {
	   p2[0] = p[0];
	   p2[1] = p[1];
	   p2[2] = p[2];
	   make_unit(p2);
	   goto go1;
	  }
	
// Light time correction
	do{
	   time = tdb - tau0;
       getsb(time, sb);
	   vectsub(sb, er, p);

	   tau = funr(p) / cvel;
	   dtau = tau - tau0;

	   if(fabs(dtau)> 1.0e-10) tau0 = tau;
       else  break;
      } while(1);

    make_unit(p);
	
// Astrometric position
	if(opt1 == 2)
	{
		p2[0] = p[0];
		p2[1] = p[1];
		p2[2] = p[2];
		goto go1;
	}
	
// Light deflection correction
	
	for(i = 0; i<3; i++) p1[i] = p[i];
		
// Aberration correction
	ev[0] = ev[0] / cvel;
	ev[1] = ev[1] / cvel;
	ev[2] = ev[2] / cvel;

	beta = sqrt(1.0 - funr(ev)*funr(ev));

	for(i=0; i< 3; i++)
	   p2[i] = (beta*p1[i] + ev[i]*(1.0 + dproduct(ev, p1)/(1.0 + beta)))/(1.0 + dproduct(p1, ev));
	
// opt1�� ���� ���� ��곡.....
	
// (opt2�� ���� ���� ���)
// J2000 equinox;
go1:if(opt2 == 1)
	  {
	   p3[0] = p2[0];
	   p3[1] = p2[1];
	   p3[2] = p2[2];
	   goto go2;
	  }
	
	precess(tdt, p2, p1);

// Mean equinox
	if(opt2 == 2)
	  {
	   p3[0] = p1[0];
	   p3[1] = p1[1];
	   p3[2] = p1[2];
	   goto go2;
	  }
	
	nutate(tdt, p1, p3);
	
// opt2�� ���� ���� ��� ��...

	
// opt3�� ���� ���� ���...
// opt3 = 1 -> cartesian coordinates 
go2:if(opt3 == 1)
      {
	   rpos[0] = dist*p3[0];
	   rpos[1] = dist*p3[1];
	   rpos[2] = dist*p3[2];
	  }
	else
	  {
	   rtos(p3, &ra, &dec);
	   rpos[0] = ra;
	   rpos[1] = dec;
	   rpos[2] = dist;
	  }
//opt3�� ���� ���� ��� ��
}
//-----------------------------------------------------------------------------
void splpos(double tdt, int npl, int opt1, int opt2, int opt3, double rpos[])
//-----------------------------------------------------------------------------
{
 double ra, dec, dist;
 const double cvel = 173.1446;
 const double muc2 = 9.87e-9;
 double er[3], ev[3], sb[3], e[3], qb[3], p[3], q[3];
 double p1[3],p2[3], p3[3];
 double tau0, tau, dtau, beta, time;
 double tdb;
 int i;

	tdb = tdt;
	
	geterv(tdb, er, ev);
	getsb(tdb, sb);
	getqb(tdb, npl, qb);

	vectsub(er, sb, e);
	vectsub(qb, sb, q);
	vectsub(qb, er, p);

	tau0 = funr(p) + 2.0*muc2 * log((funr(e) + funr(p) + funr(q))/(funr(e) - funr(p) + funr(q)));
	tau0 = tau0/cvel;

	dist = funr(p);

// opt1�� ���� ���� ���...
// Geometric position, no light time correction
	if(opt1 == 1)
      {
	   p2[0] = p[0];
	   p2[1] = p[1];
       p2[2] = p[2];
       make_unit(p2);
       goto go1;
      }
	
// Light time correction
	do{
       time = tdb - tau0;

	   getsb(time, sb);
	   getqb(time, npl, qb);

	   vectsub(qb, sb, q);
	   vectsub(qb, er, p);

	   tau = funr(p) + 2.0*muc2 * log((funr(e) + funr(p) + funr(q))/(funr(e) - funr(p) + funr(q)));
	   tau = tau/cvel;

	   dtau = tau - tau0;
		
	   if(fabs(dtau) > 1.0e-10) tau0 = tau;
       else  break;
      }while(1);

	make_unit(p);
	make_unit(q);
	make_unit(e);
	

// Astometric position, no Aberration correction
	if(opt1 == 2)
      {
	   p2[0] = p[0];
	   p2[1] = p[1];
	   p2[2] = p[2];
	   goto go1;
	  }
	
// Light deflection correction
	for(i=0; i<3 ; i++)
	p1[i] = p[i] + 2.0*muc2*(dproduct(p,q)*e[i] - dproduct(e,p)*q[i])/(1.0 + dproduct(q,e));

// Aberration correction
	ev[0] = ev[0]/cvel;
	ev[1] = ev[1]/cvel;
	ev[2] = ev[2]/cvel;

	beta = sqrt(1.0 - funr(ev)*funr(ev));

	for(i=0; i<3 ; i++)
		p2[i] = (beta*p1[i] + ev[i]*(1.0 + dproduct(ev, p1)/(1.0 + beta)))/(1.0 + dproduct(p1, ev));
	
// opt1�� ���� ���� ��� ��...�Ѥ�

		 
// (opt2�� ���� ���� ���)
// Reference Frame : 1 J2000, 2 Mean, Others Apparent
go1:if(opt2 == 1)
	  {
		p3[0] = p2[0];
		p3[1] = p2[1];
		p3[2] = p2[2];
		goto go2;
	  }
	
	precess(tdt, p2, p1);

	if(opt2 == 2)
	  {
		p3[0] = p1[0];
		p3[1] = p1[1];
		p3[2] = p1[2];
		goto go2;
	  }
	
		nutate(tdt, p1, p3);
// opt2�� ���� ���� ��� ��...

// opt3�� ���� ���� ���...
// opt3 = 1 -> cartesian coordinates
go2:if(opt3 == 1)
	  {
		rpos[0] = dist*p3[0];
		rpos[1] = dist*p3[1];
		rpos[2] = dist*p3[2];
	  }
	else
	  {
		rtos(p3, &ra, &dec);
		rpos[0] = ra;
		rpos[1] = dec;
		rpos[2] = dist;
	  }
//opt3�� ���� ���� ��곡...
}
//-----------------------------------------------------------------------------
void aplapos(double tdt, int npl, double *ra, double *dec, double *dist)
//-----------------------------------------------------------------------------
// input -> tdt : JD in TDT scale
//		   npl : planet no.
//			1:Mercury, 2:Venus, 3:Earth, 4:Mars, 5:Jupiter, 6:Saturn
//  		7:Uranus, 8:Neptune, 9:Pluto, 10:Moon, 11:Sun, 12:Solar-System Barycenter
//			13:Earth-Moon Barycenter, 14:Nutations(Longitude and Obliq)
//			15:Librations, If in eph file
//
// output -> ra, dec : Geocentric Apparent RA, DEC and Distance of the planet
//-----------------------------------------------------------------------------
{
 const double cvel = 173.1446;
 const double muc2 = 9.87e-9;

	double er[3], ev[3], sb[3], e[3], qb[3], p[3], q[3];
	double p1[3], p2[3], p3[3];
	double tau0, tau, dtau, beta, time;

	double tdb;

	int i;

	tdb = tdt;

	geterv(tdb, er, ev);
	getsb (tdb, sb);
	getqb (tdb, npl, qb);

	vectsub(er, sb, e);
	vectsub(qb, sb, q);
	vectsub(qb, er, p);
	tau0 = funr(p) + 2.0*muc2*log((funr(e)+funr(p)+funr(q))/(funr(e)-funr(p)+funr(q)));
	tau0 = tau0/cvel;

	*dist = funr(p);
	
//  Light time correction
// Light time correction
	do{
       time = tdb - tau0;

	   getsb(time, sb);
	   getqb(time, npl, qb);

	   vectsub(qb, sb, q);
	   vectsub(qb, er, p);

	   tau = funr(p) + 2.0*muc2 * log((funr(e) + funr(p) + funr(q))/(funr(e) - funr(p) + funr(q)));
	   tau = tau/cvel;

	   dtau = tau - tau0;
		
	   if(fabs(dtau) > 1.0e-10) tau0 = tau;
       else  break;
      }while(1);
	

	make_unit(p);
	make_unit(q);
	make_unit(e);

//  Light deflection correction
	for( i = 0; i< 3; i++)
	p1[i] = p[i] + 2.0*muc2*(dproduct(p,q)*e[i] - dproduct(e,p)*q[i])/(1.0 + dproduct(q,e));
	
//  Aberration correction
	ev[0] = ev[0]/cvel;
	ev[1] = ev[1]/cvel;
	ev[2] = ev[2]/cvel;

	beta = sqrt(1.0 - funr(ev)*funr(ev));

	for(i=0;i<3; i++) p2[i] = (beta*p1[i] + ev[i]*(1.0 + dproduct(ev, p1)/(1.0 + beta)))/(1.0 + dproduct(p1,ev));

	precess(tdt, p2, p1);
	nutate(tdt, p1, p3);

	rtos(p3, ra, dec);
}
//-----------------------------------------------------------------------------
void asunpos(double tdt, double *ra, double *dec, double *dist)
//-----------------------------------------------------------------------------
//input : Julian date in TDT scale
//output : ra, dec, dist : Geocentric apparent RA, DEC and Distance of the sun
//-----------------------------------------------------------------------------
{
 const int npl=11;
 const double cvel=173.1446;
 const double muc2=9.87e-9;

 double er[3], ev[3], sb[3], p[3];
 double p1[3], p2[3], p3[3];
 double tau0, tau, dtau, beta, time;

 double tdb;

 int i;

	tdb = tdt;

	geterv(tdb, er, ev);
	getsb(tdb, sb);
	vectsub(sb, er, p);
	tau0 = funr(p)/cvel;
	
	*dist = funr(p);
		
	//Light time correction...
	do
	{
	time = tdb - tau0;

	getsb(time, sb);
	
	vectsub(sb, er, p);

	tau = funr(p) / cvel;
	dtau = tau - tau0;

	if(fabs(dtau) > 1.0e-11)
		tau0 = tau;
		
	else
		break;
	} while(1);


	make_unit(p);
	
	//Light deflection correction
	for(i=0; i<3; i++)
		p1[i] = p[i];

	//Aberration correction
	ev[0] = ev[0]/cvel;
	ev[1] = ev[1]/cvel;
	ev[2] = ev[2]/cvel;

	beta = sqrt(1.0 - funr(ev)*funr(ev));
	
	for(i = 0 ; i<3 ; i++)
	{
		p2[i] = (beta*p1[i] + ev[i]*(1.0 + dproduct(ev,p1)/(1.0 + beta)))/(1.0 + dproduct(p1, ev));
	}

	
	precess(tdt, p2, p1);
	nutate(tdt, p1, p3);

	rtos(p3, ra, dec);

}
//-----------------------------------------------------------------------------
void asunlb(double tdt, double *lamda, double *beta, double *dist)
//-----------------------------------------------------------------------------
//input : tdt -> JD in TDT scale
//output : ra, dec, dist -> Geocentric Apparent Lamda, Beta and Distance of the Sun
//-----------------------------------------------------------------------------
{
	double ra, dec;
	double dpsi, deps, eps;
	double p[3], p1[3];

	asunpos(tdt, &ra, &dec, dist);
	p[0] = cos(dec)*cos(ra);
	p[1] = cos(dec)*sin(ra);
	p[2] = sin(dec);

	nutconst(tdt, &dpsi, &deps, &eps);
	rotx(eps, p, p1);
	rtos(p1, lamda, beta);
}
//-----------------------------------------------------------------------------
double slong(double tdt)
//-----------------------------------------------------------------------------
//To calculate Sun's longitude(radian)
//input : JD in TDT scale
//-----------------------------------------------------------------------------
{
 double lamda, beta, dist;
 double result;

	asunlb(tdt, &lamda, &beta, &dist);
	result = lamda;
	return result;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void aplalb(double tdt, int npl, double *lamda, double *beta, double *dist)
//-----------------------------------------------------------------------------
//input : tdt -> JD in TDT scale
//		  npl -> planet no.
//
//output : ra,dec, dist : Geocentric Apparent Lamda, Beta, Distance of the planet
//-----------------------------------------------------------------------------
{
 double ra, dec;
 double dpsi, deps, eps;
 double p[3], p1[3];

	aplapos(tdt, npl, &ra, &dec, dist);
	p[0] = cos(dec)*cos(ra);
	p[1] = cos(dec)*sin(ra);
	p[2] = sin(dec);

	nutconst(tdt, &dpsi, &deps, &eps);
	rotx(eps, p, p1);
	rtos(p1, lamda, beta);

}
//-----------------------------------------------------------------------------
double mlong(double tdt)
//-----------------------------------------------------------------------------
//To calculate Moon's longitude(radian)
//input : JD in TDT scale
//-----------------------------------------------------------------------------
{
	double lamda, beta, dist;
	double result;
	const int npl = 10;
	aplalb(tdt, npl, &lamda, &beta, &dist);

	result = lamda;
	return result;
}
//-----------------------------------------------------------------------------
double mslong(double tdt)
//-----------------------------------------------------------------------------
//To calculate (Moon's longitude - Sun's longitude) in radian
//input : JD in TDT scale
//-----------------------------------------------------------------------------
{
	double twopi, result;
	int ii;
	static int first;
	first = 0;

	if(first == 0)
	{
		twopi = acos(-1.0) * 2.0;
		first = 1;
	}

	result = mlong(tdt) - slong(tdt);
	ii =  (int)(result / twopi);
	result = result - ii*twopi;

	if(result < 0.0)
		result = result + twopi;
	return result;
}
//-----------------------------------------------------------------------------
