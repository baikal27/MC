#include <utility.h>
#include <userint.h>
#include <ansi_c.h>
#include "type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "gv_tel.h"
#include "st4s_lib.h"
#include "tel_sub.h"
extern double MAG_MAX, MAG_MIN;

extern int hand_paddle(int track_mode); // located in guide_sub.c
extern int SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000);
extern int STOP_TRACKING(void);
extern int display_input_window(h_m_s ra_2000, d_m_s dec_2000, char star_name[]);

extern void display_message(char buff[]);


//----------------------------------------------------------------------------
int make_scan_file(double lmag, int catoption);
int get_star1     (double jd,double lmag,double ldec);
int get_star1_bsc (double jd,double lmag,double ldec);
int get_star      (double jd_ut1,double alt1,double alt2,double hangle_min,double hangle_max,int *nstar);
int find_equator_star(double lmag, double dec_min, double dec_max);
int get_star_coordiantes(int nstar, double *ra, double *dec);
int move_to_a_star(int nstar);
int simulate(void);
int show_scan_lists(int handle,int control);
double find_nearby_star(h_m_s ra_target, d_m_s dec_target,double lmag,h_m_s *ra_near, d_m_s *dec_near);

//----------------------------------------------------------------------------
FILE *fout1, *fout3;
static int year,month,day,hour,minute,second,hsecond;
static double rhour,jd_ut1;
static int starno;
static double last,ap_ha,ap_dec,alt,az,vmag;
static char cname[4],gname[4],stype[20],star_name[20];
double altmin=20.0;
static h_m_s ra;
static d_m_s dec;
static int NO_OF_CAT_STARS;

const double  TWOPI  = 3.14159265358979323846*2.0;
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
int make_scan_file(double lmag, int catoption)
//----------------------------------------------------------------------------
{
 double ldec,dec1,dec2;
 int i,nstar;
 double hangle_min,hangle_max;

    ldec = -37;
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;

    if(catoption==1)
      nstar = get_star1(jd_ut1,lmag,ldec);
    else
      nstar = get_star1_bsc(jd_ut1,lmag,ldec);

	if( (fout1 = fopen("skyscan.dat","w"))==NULL )
	  {Beep(); MessagePopup(""," CANNOT OPEN skyscan.DAT \n"); exit(0);}

    nstar = 0;
    dec1  =-90, dec2 = 20;
	for(i=6;i>=-5;i--)
	   {hangle_min = 2.0*(i-1.0);  hangle_max = 2.0*i;
	    get_star(jd_ut1,dec1,dec2,hangle_min,hangle_max,&nstar);
       }

    dec1 = 20, dec2 = 80;
    for(i=-5;i<=6;i++)
       {hangle_min = 2.0*(i-1.0);  hangle_max = 2.0*i; 
        get_star(jd_ut1,dec1,dec2,hangle_min,hangle_max, &nstar);
       }
    fclose(fout1);
    NO_OF_CAT_STARS = nstar;
    return nstar;
}
//----------------------------------------------------------------------------
int get_star1(double jd, double lmag, double ldec)
//----------------------------------------------------------------------------
//  prepare "sky.dat" from FK5 catalogue. "sky.dat" is used in get_star
//----------------------------------------------------------------------------
{
 int starno,rah,ram,decd,decm, ii;
 double ras,pra,decs,pdec,mag;
 double ra0,dec0,dt;
 h_m_s rai;
 d_m_s deci;
 FILE *fin, *fout;

    sprintf(cname,"XXX");
    sprintf(gname,"XXX");

    dt = (jd-2451545.0) / 36525.0;
    ii = 0;

    if( (fin = fopen("fk5n.dat","r"))==NULL )
      {puts(" CANNOT OPEN FK5N.DAT \n"); return 0;}

    if( (fout = fopen("sky.dat","w"))==NULL )
      {puts(" CANNOT OPEN SKY.DAT \n"); return 0;}

    while( fscanf(fin,"%d  %d %d %lf %lf %d %d %lf %lf %lf %s",&starno, &rah,&ram,&ras,&pra, &decd,&decm,&decs,&pdec,&mag,stype)
          != EOF)
         {
//        if(mag<lmag)
          if(MAG_MIN<mag && mag<MAG_MAX)
            {ra0  =  rah + ram/60.0 + ras/3600.0;
             if(decd>0) dec0 =  decd + decm/60.0 + decs/3600.0;
             else       dec0 =  decd - decm/60.0 - decs/3600.0;

             ra0  = (ra0  + dt*pra /3600.0)*15.0; // in degree
             dec0 = dec0 + dt*pdec/3600.0;        // in degree

             rai.rad = deg_to_rad(ra0);           // convert to radian
             deci.rad= deg_to_rad(dec0);          // convert to radian
             if(rai.rad<0.0) rai.rad +=  TWOPI;

             if(dec0>ldec)
               {rad_to_hms(&rai); rad_to_dms(&deci);
                ii++;
                fprintf(fout, "%5d %3s %3s %15.8lf %15.8lf %6.2lf %s\n",
                        starno,gname,cname,rai.rad,deci.rad,mag,stype);
               }
            } // end of if
         } // end of while
         
    fclose(fin); fclose(fout);
    return ii;
}
//----------------------------------------------------------------------------
int  get_star1_bsc(double jd_ut1, double lmag, double ldec)
//----------------------------------------------------------------------------
//  prepare "sky.dat" from YBSC. "sky.dat" is used in get_star
//----------------------------------------------------------------------------
{
 int nstar, hrno;
 char signd,plxd;
 double pra,pdec,plx,ub,bv,dt;
 FILE *fin,*fout;

    if( (fin = fopen("partbsc5.cat","r"))==NULL )
      {puts(" CANNOT OPEN PARTBSC5.CAT \n"); return 0;}

    if( (fout = fopen("sky.dat","w"))==NULL )
      {puts(" CANNOT OPEN SKY.DAT \n"); return 0;}

    dt = (jd_ut1-2451545.0) / 365.25;
    nstar = 0;
    while( fscanf(fin, " %d %3s %3s %d %d %lf %c%d %d %lf %lf %lf %lf%c %lf %lf %lf %s\n",
           &hrno, gname, cname,
           &ra.h,&ra.m,&ra.s, &signd,&dec.d,&dec.m,&dec.s,
           &pra,&pdec,&plx,&plxd,
           &vmag,&ub,&bv, stype) != EOF)
         {
          dms_to_rad(&dec);  hms_to_rad(&ra);
          if(signd=='-') dec.rad = -dec.rad;
          dec.rad = dec.rad + deg_to_rad(dt*pdec/3600.0);
          if(dec.rad<0.0) {dec.rad = -dec.rad; signd = '-';}
          else signd = '+';

          pra = pra/cos(dec.rad);
          ra.rad  = ra.rad + deg_to_rad(dt*pra/3600.0);
          if(ra.rad<0.0) ra.rad = ra.rad + TWOPI;

          rad_to_hms(&ra); rad_to_dms(&dec);
          if(signd=='-') dec.rad = -dec.rad;

//          get_aposi_ord(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad, SITE_LATI.rad,1,
//                  &last,&ap_ha,&ap_dec,&alt,&az);
          get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad, SITE_LATI.rad,1,
                  &last,&ap_ha,&ap_dec,&alt,&az);
          alt    = rad_to_deg(alt);
          az     = rad_to_deg(az );
          ap_ha  = rad_to_deg(ap_ha) /15.0;
          ap_dec = rad_to_deg(ap_dec);

//        if(vmag<lmag && ap_dec>ldec)
          if(MAG_MIN<vmag && vmag<MAG_MAX && ap_dec>ldec)
            {nstar = nstar + 1;
             fprintf(fout, "%5d %3s %3s %15.8lf %15.8lf %6.2lf %s\n",
                     hrno,gname,cname,ra.rad,dec.rad,vmag,stype);
            }   
         } // end of while loop
    fclose(fin); fclose(fout);
    return nstar;
}
//----------------------------------------------------------------------------
int get_star(double jd_ut1, double dec1, double dec2,double hangle_min, double hangle_max, int *nstar)
//----------------------------------------------------------------------------
//  make scan file "skyscan.dat" using "sky.dat"
//----------------------------------------------------------------------------
{
 FILE *fin;
    if( (fin = fopen("sky.dat","r"))==NULL )
      {Beep(); MessagePopup("","CANNOT OPEN SKY.DAT \n"); exit(0);}


    while(fscanf(fin, "%d %s %s %lf %lf %lf %s \n",&starno,gname,cname,&ra.rad,&dec.rad,&vmag,stype)
	      != EOF)
         {get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad, SITE_LATI.rad,1,
                    &last,&ap_ha,&ap_dec,&alt,&az);
          rad_to_hms(&ra); rad_to_dms(&dec);
          alt    = rad_to_deg(alt);
          az     = rad_to_deg(az);
          ap_ha  = rad_to_deg(ap_ha) /15.0;
          ap_dec = rad_to_deg(ap_dec);
          if(alt>altmin && ap_dec>dec1 && ap_dec<dec2 &&
             ap_ha <hangle_max && ap_ha>hangle_min)
            {*nstar = *nstar + 1;
             fprintf(fout1,"%5d %3s %3s %15.8lf %15.8lf %5.2lf %s\n",
                     *nstar,gname,cname,ra.rad,dec.rad,vmag,stype);
//           printf("%3d %4d %3s %3s %2d %2d %4.1lf %6.2lf %6.2lf %6.2lf %7.2lf %4.1lf %s\n",
//                  *nstar,starno,gname,cname,ra.h,ra.m,ra.s,ap_ha,ap_dec,alt,az,vmag,stype);
	        }
         }
    fclose(fin);
        
    return 0;
}
//----------------------------------------------------------------------------
int find_equator_star(double lmag, double dec_min, double dec_max)
//----------------------------------------------------------------------------
{
 double ldec = -37.0;
 int i,nstar,catoption;
 double dec1, dec2;
 double hangle_min,hangle_max;

    catoption = 2;
    dec1 = dec_min;
    dec2 = dec_max;
        
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;

    if(catoption==1)
       nstar = get_star1(jd_ut1,lmag,ldec);
    else
       nstar = get_star1_bsc(jd_ut1,lmag,ldec);

	fout1 = fopen("skyscan.dat","w");
	nstar = 0;
    for(i=6;i>=-5;i--)
	   {hangle_min = 2.0*(i-1.0);  hangle_max = 2.0*i;
	    get_star(jd_ut1,dec1,dec2,hangle_min,hangle_max,&nstar);
	   }
    fclose(fout1);

    NO_OF_CAT_STARS = nstar;

    return nstar;
}
//----------------------------------------------------------------------------
int get_star_coordiantes(int nstar, double *ra2000, double *dec2000)
//----------------------------------------------------------------------------
{
 int ii, flag;
 FILE *fin;

    if( (fin = fopen("skyscan.dat","r"))==NULL )
      {puts(" CANNOT OPEN SKYSCAN.DAT \n"); exit(0);}

    flag = -1;
    for(ii=1;ii<=NO_OF_CAT_STARS;ii++)
       {fscanf(fin, "%d %s %s %lf %lf %lf %s \n",&starno,gname,cname,ra2000,dec2000,&vmag,stype);
        if(starno==nstar) {flag=0; break;}
       }
    fclose(fin);
    return 0;
}
//----------------------------------------------------------------------------
int simulate()
//----------------------------------------------------------------------------
{
 double lmag;
 long step_ha,step_dec,encoder_1,encoder_2;
 double  must_count_ha,must_count_dec;
 double  error_ha, error_dec;
 int nstar;
 double dtime = 0.5;
 FILE *foutt,*fout2;
 int error_code;

 char ccc;
 int i,iii,scode;
 FILE *fin, *fout;

//  Begin Sky Scan
    if( (fin = fopen("skyscan.dat","r"))==NULL )
      {puts(" CANNOT OPEN SKY.DAT \n"); exit(0);}

    while(fscanf(fin,"%d %s %s %lf %lf %lf %s \n",&starno,gname,cname,&ra.rad,&dec.rad,&vmag,stype)
         != EOF)
        {
         sprintf(star_name,"%s %s",gname,cname);
         display_input_window(ra,dec,star_name);
         return SLEW_AND_TRACK(ra,dec);
        }
    fclose(fin); fclose(fout); fclose(foutt); fclose(fout1);fclose(fout2);
    return 0;
}
//----------------------------------------------------------------------------
int move_to_a_star(int nstar)
//----------------------------------------------------------------------------
{
 int ii, flag;
 FILE *fin;

    if( (fin = fopen("skyscan.dat","r"))==NULL )
      {puts(" CANNOT OPEN SKYSCAN.DAT \n"); exit(0);}

    flag = -1;
    for(ii=1;ii<=NO_OF_CAT_STARS;ii++)
       {fscanf(fin, "%d %s %s %lf %lf %lf %s \n",&starno,gname,cname,&ra.rad,&dec.rad,&vmag,stype);
        if(starno==nstar) {flag=0; break;}
       }
    fclose(fin);

    if(flag != 0) return flag;  // cannot find the selected stars

    sprintf(star_name,"%s %s",gname,cname);
    display_input_window(ra,dec,star_name);
    return SLEW_AND_TRACK(ra,dec);
}
//----------------------------------------------------------------------------
double find_nearby_star(h_m_s ra_target, d_m_s dec_target,double lmag,h_m_s *ra_near, d_m_s *dec_near)
//----------------------------------------------------------------------------
//  return value: angular seperation (deg) of the selected star
//----------------------------------------------------------------------------
{
 int    nstar, hrno, update, i,ndata;
 char   signd,plxd;
 double pra,pdec,plx,ub,bv,dist,dist_old,jdut1;
 FILE   *fin;
 char   ssss[300];
 h_m_s ra; d_m_s dec;
 double last,ap_ha,ap_dec,alt,az,vmag,jd_ut1;
 char   cname[4],gname[4],stype[20];

    jd_ut1 = get_jdutc_clock();
    dist_old = 5.0;
    update = 0;

    printf("Target R.A.%10.2lf, Dec. %10.2lf (deg)\n",
            rad_to_deg(ra_target.rad)/15.0,rad_to_deg(dec_target.rad));

    if((fin = fopen("partbsc5.dat","r"))==NULL )
      {puts(" CANNOT OPEN PARTBSC5.DAT \n"); return 0;}

    fscanf(fin,"%d",&ndata);
    for(i=0;i<ndata;i++)
       {
        fgets(ssss,150,fin);
        sscanf(ssss, "%d %3s %3s %d %d %lf %c%d %d %lf %lf %lf %lf%c %lf %lf %lf %s",
               &hrno, gname, cname,
               &ra.h,&ra.m,&ra.s, &signd,&dec.d,&dec.m,&dec.s,
               &pra,&pdec,&plx,&plxd,
               &vmag,&ub,&bv, stype);

        hms_to_rad(&ra);
        dms_to_rad(&dec); if(signd=='-') dec.rad = -dec.rad;
        dist = cos(dec.rad)*cos(ra.rad) * cos(dec_target.rad)*cos(ra_target.rad)
              +cos(dec.rad)*sin(ra.rad) * cos(dec_target.rad)*sin(ra_target.rad)
              +sin(dec.rad)             * sin(dec_target.rad);
        dist = 1.0 - dist;

        if(dist<dist_old && vmag<lmag)
          {dist_old = dist;
           (*ra_near).rad = ra.rad; (*dec_near).rad = dec.rad;

           get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
                     &last,&ap_ha,&ap_dec,&alt,&az);
           alt    = rad_to_deg(alt);
           az     = rad_to_deg(az );
           ap_ha  = rad_to_deg(ap_ha) /15.0;
           ap_dec = rad_to_deg(ap_dec);

           sprintf(STAR_STRING,"%5d %3s %3s %2d %2d %4.1lf %6.2lf %6.2lf %6.2lf %7.2lf %5.1lf %5s\n",
                   hrno,gname,cname,ra.h,ra.m,ra.s,ap_ha,ap_dec,alt,az,vmag,stype);
           printf("%5d %3s %3s %2d %2d %4.1lf %6.2lf %6.2lf %6.2lf %7.2lf %5.1lf %5s\r",
                   hrno,gname,cname,ra.h,ra.m,ra.s,ap_ha,ap_dec,alt,az,vmag,stype);
           update = update + 1;

//         printf("%5d,%5d %10.3lf\r",update,hrno,dist);
          }
       }
    fclose(fin);

    dist_old = 1.0 - dist_old;
    rad_to_hms(ra_near); rad_to_dms(dec_near);

    return rad_to_deg(acos(dist_old));
}
//----------------------------------------------------------------------------
int show_scan_lists(int handle,int control)
//----------------------------------------------------------------------------
{
 int nstar, ii;
 FILE *fin;
 int value;
 char buff[150];

    if( (fin = fopen("skyscan.dat","r"))==NULL )
      {puts(" CANNOT OPEN SKYSCAN.DAT \n"); exit(0);}

    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
        
    ii = 0;
//  sprintf(buff," No.  Name     R.A.     H.A.   Dec.   Alt.    Azi.   mag sp");
//  SetCtrlVal(handle,SCAN_TEXTMSG,buff);
//  InsertListItem(handle, control,-1,buff,0);
    while(fscanf(fin, "%d %s %s %lf %lf %lf %s \n",&starno,gname,cname,&ra.rad, &dec.rad,&vmag,stype)
          != EOF)
         {
          get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,&last,&ap_ha,&ap_dec,&alt,&az);
          rad_to_hms(&ra);rad_to_dms(&dec);
          alt    = rad_to_deg(alt);
          az     = rad_to_deg(az );
          ap_ha  = rad_to_deg(ap_ha) /15.0;
          ap_dec = rad_to_deg(ap_dec);
          sprintf(buff,"%2d %3s %3s %2d %2d %4.1lf %6.2lf %6.2lf %6.2lf %7.2lf %5.1lf %s",
                  ii+1,gname,cname,ra.h,ra.m,ra.s,ap_ha,ap_dec,alt,az,vmag,stype);
		  
          value = ii;
          InsertListItem(handle, control,-1,buff,value);
          ii++;
		  
     }
    fclose(fin);
    NO_OF_CAT_STARS = ii;
    return ii;
}
//----------------------------------------------------------------------------
