#include <formatio.h>
#include <analysis.h>
//-------------------------------------------------------------------------------
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
//#include "type_def.h"
#include "cpabtsub.h"
#include "mathsub.h"

#include "cvi_util.h"
//#include "read_cat.h"

extern void display_message(char buff[]);
extern int main_handle, util_handle, perplot_handle;

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
static double  PI314           = 3.14159265358979323846;
static double  twopi           = 3.14159265358979323846*2.0;
static double  FACT_DEG_TO_RAD = 3.14159265358979323846/180.0;
static double  FACT_RAD_TO_DEG = 180.0/3.14159265358979323846;

typedef struct H_REC
        {int hpno,hdno;
         double ra; double dec; double par;double epar;
         double ux; double uy;
         double vm; double bv;
         double bmv;
         char q;    char sp[14];
        } hip_rec_type;
        
hip_rec_type HIP_DATA;

//-------------------------------------------------------------------------------
int open_hip_cat(void);
int close_hip_cat(void);
int scan_hip(void);
int read_hip_onestar(int hdno_inp, char sbuff[],int OUTFORMAT);

int open_oes(void);
int read_oesb8(FILE *inf);
int get_hip_no(int hd_no);
int get_hd_no(int hip_no);
int hr_to_hipno(int hr_no);
int read_ep(int star_no);

double cal_mass(double vmag,double par_mas);
static void cal_phase(double epoch[],double per, int ndata, double phase[]);

static FILE *hip_cat, *oes_cat;
static char HIP_PATH[100]="c:\\lib\\catalogue\\hipparcos\\";
static char buff[300];
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
int open_hip_cat(void)
//-------------------------------------------------------------------------------
{
 sprintf(buff,"%ship_main_sorted.cat",HIP_PATH);
 
    if( (hip_cat=fopen(buff,"r"))==NULL)
      {
       MessagePopup("cannot open",buff);
       return -1;
      }
      
    return 0;
}      
//-------------------------------------------------------------------------------
int close_hip_cat(void)
//-------------------------------------------------------------------------------
{
    fclose(hip_cat);
    return 0;
}      
//-------------------------------------------------------------------------------
int read_hip_onestar(int starno_inp, char ibuff[], int OUTFORMAT)
//-------------------------------------------------------------------------------
{
 char tbuff[500];
 h_m_s ra;
 d_m_s dec;
 double vmag,bmv,par,uksi,ueta;
 double ura,udec;
 int i,ii,hipno,hdno,starno;
 char spect[13], dsflag;
 
      SetWaitCursor(1);
      
      for(;;)
         {
          if(fgets(tbuff,480,hip_cat)==NULL)
           {
            rewind(hip_cat);
            SetWaitCursor(0);
            printf("end of file\n");
            return -1;
           }
           
          sscanf(&tbuff[  2]," %d|",&hipno);
          sscanf(&tbuff[390],"%6d|",&hdno );
          
          if(hipno>starno_inp) {rewind(hip_cat);SetWaitCursor(0); return -1;}

          if(hipno==starno_inp)
           {
            sscanf(&tbuff[ 51],"%lf|%lf|",&ra.rh,&dec.rd);
            sscanf(&tbuff[ 79],"%lf|",&par);
            sscanf(&tbuff[ 87],"%lf|",&uksi);
            sscanf(&tbuff[ 96],"%lf|",&ueta);
            sscanf(&tbuff[ 41],"%lf|",&vmag);
            sscanf(&tbuff[245],"%lf|",&bmv);
            sscanf(&tbuff[119],"%lf|",&HIP_DATA.epar);
            
            sscanf(&tbuff[435],"%12s|",&spect);
            sprintf(HIP_DATA.sp,"%s",spect);
            
            dsflag = tbuff[346];     
           
            dec.rd += ueta*(2000-1991.25)/1000/3600;
            dec.rad = deg_to_rad(dec.rd);
            rad_to_dms(&dec);
            udec = ueta/1000;

            ura = uksi/cos(dec.rad);
            ra.rh += ura*(2000-1991.25)/1000/3600;  // unit degree
            ra.rad = deg_to_rad(ra.rh);
            rad_to_hms(&ra);
            ura = ura/1000/15.0; // sec/year
            
            HIP_DATA.ra  = ra.rad;
            HIP_DATA.dec = dec.rad;
            HIP_DATA.par = par;
            HIP_DATA.ux  = ura;
            HIP_DATA.uy  = udec;
            HIP_DATA.vm  = vmag;
            HIP_DATA.bmv = bmv;
            
            HIP_DATA.hpno  = hipno;
            HIP_DATA.hdno  = hdno;

            HIP_DATA.q     = dsflag;

            if(OUTFORMAT==0)       // hip format
            sprintf(ibuff,"%6d %6d %c%7.2lf%6.2lf%6.2lf %2d%3d%10.6lf%8.2lf  %3d%3d%9.5lf%8.2lf %s %6.2lf\n",
            hipno,hdno,dsflag,par,vmag,bmv,ra.h,ra.m,ra.s,uksi,dec.d,dec.m,dec.s,ueta,spect,HIP_DATA.epar);

            else if(OUTFORMAT==1)  // F14 format
            sprintf(ibuff,"%6d %c %6.2lf%6.2lf %2d%3d%10.6lf%11.7lf %3d%3d%9.5lf%10.6lf%8.5lf %s %6.2lf\n",
            hipno,dsflag,vmag,bmv,ra.h,ra.m,ra.s,ura,dec.d,dec.m,dec.s,udec,par/1000.0,spect,HIP_DATA.epar);
            
//          display_message(ibuff);
            
            SetWaitCursor(0);
            return 0;
           }
         }
}
//-------------------------------------------------------------------------------
int open_oes()
//-------------------------------------------------------------------------------
{
    if( (oes_cat=fopen("c:\\astrometry\\catalogue\\oesb8.cat","r"))==NULL)
      {
       MessagePopup("cannot open","OESB8.CAT");
       return -1;
      }
      
    return 0;
}
//-------------------------------------------------------------------------------
int get_hip_no(int hd_no)
//-------------------------------------------------------------------------------
{
 FILE *tbl;
 int nnn,hip_no;
 
	if( (tbl=fopen("HD_HIP.tbl","r")) == NULL)
	  {
	   MessagePopup("","cannot open HD_FIP.TBL");
	   return -1;
	  }
	     
	while(fgets(buff,150,tbl)!=NULL)
	     {
          sscanf(buff,"%d|%d",&nnn,&hip_no);
          if(nnn==hd_no) {fclose(tbl); return hip_no;}
         }
	
	fclose(tbl);
 
//  MessagePopup("","cannot find HD_NO");
    return -2;
}
//-------------------------------------------------------------------------------
int hr_to_hipno(int hr_no)
//-------------------------------------------------------------------------------
{
 FILE *tbl;
 int nnn,hip_no;
 
	if( (tbl=fopen("HR_HIP.tbl","r")) == NULL)
	  {
	   MessagePopup("","cannot open HD_FIP.TBL");
	   return -1;
	  }
	     
	while(fgets(buff,150,tbl)!=NULL)
	     {
          sscanf(buff,"%d|%d",&nnn,&hip_no);
          if(nnn==hr_no) {fclose(tbl); return hip_no;}
         }
	
	fclose(tbl);
 
//  MessagePopup("","cannot find HD_NO");
    return -2;
}
//-------------------------------------------------------------------------------
int get_hd_no(int hip_no)
//-------------------------------------------------------------------------------
{
 FILE *tbl;
 int nnn,hd_no;
 
	if( (tbl=fopen("HD_HIP.tbl","r")) == NULL)
	  {
	   MessagePopup("","cannot open HD_FIP.TBL");
	   return -1;
	  }
	     
	while(fgets(buff,150,tbl)!=NULL)
	     {
          sscanf(buff,"%d|%d",&hd_no,&nnn);
          if(nnn==hip_no) {fclose(tbl); return hd_no;}
         }
	
	fclose(tbl);
 
    MessagePopup("","cannot find HD_NO");
    return -2;
}
//-------------------------------------------------------------------------------


/*
Byte-by-byte Description of file: catalog.dat
--------------------------------------------------------------------------------
   Bytes Format Units   Label     Explanations
--------------------------------------------------------------------------------
   1-  2  I2    h       RAh       RA 1900 (hours)
   3-  5  I3    dmin    RAdm      RA 1900 (deci-minutes)
       6  A1    ---     DE-       DEC1900 (sign)
   7-  8  I2    deg     DEd       DEC1900 (degrees)
   9- 10  I2    arcmin  DEm       DEC1900 (arcminutes)
      11  A1    ---     ---       [|] Separation character
  12- 24  A13   ---     Name      First name. Just a number means HD number (1)
      25  A1    ---     ---       [|] Separation character
  26- 42  A17   ---     Alias     Other Name (1)
      43  A1    ---     Eclipse   [*] The '*' indicates an eclipsing binary
      44  A1    ---     ---       [|] Separation character
  45- 49  F5.2  mag     maxMag    ? Magnitude at Maximum
      50  A1    ---     n_maxMag  [VBRP] Type of maxMag magnitude (2)
      51  A1    ---     f_maxMag  [*]  Remark or Note flag
      52  A1    ---     ---       [|] Separation character
  53- 57  F5.2  mag     minMag    ? Magnitude at Minimum
      58  A1    ---     n_minMag  [VBRP] Type of minMag magnitude (2)
      59  A1    ---     f_minMag  [*]  Remark or Note flag
      60  A1    ---     ---       [|] Separation character
  61- 76  A16   ---     Sp1       Spectral type of Primary
      77  A1    ---     ---       [|] Separation character
  78- 86  A9    ---     Sp2       Spectral type of Secondary (if known)
      87  A1    ---     ---       [|] Separation character
  88- 98  F11.5 ---     Period    ? Period, generally in days (see x_Period)
      99  A1    ---     x_Period  [y] 'y' if Period expressed in years
     100  A1    ---     f_Period  [*] Remark or Note flag
     101  A1    ---     ---       [|] Separation character
 102-112  F11.4 ---     T         ? Periastron date, generally in JD-2400000
                                    (see x_T indicator)
     113  A1    ---     x_T       [y] 'y' if periastron T expressed as year.
     114  A1    ---     f_T       [*] Remark or Note flag
     115  A1    ---     ---       [|] Separation character
 116-119  F4.2  ---     e1        [0/1[? Eccentricity of Primary
     120  A1    ---     f_e1      [*]    Remark or Note flag
     121  A1    ---     ---       [|] Separation character
 122-125  F4.2  ---     e2        [0/1[? Eccentricity of Secondary, if different
     126  A1    ---     ---       [|] Separation character
 127-131  F5.1  deg     omega1    [0/360[? Angle of periastron of Primary
     132  A1    ---     u_omega1  Uncertainty flag (:)
     133  A1    ---     f_omega1  [*] Remark or Note flag
     134  A1    ---     ---       [|] Separation character
 135-139  F5.1  deg     omega2    [0/360[? Angle of periastron of Secondary
     140  A1    ---     u_omega2  Uncertainty flag (:)
     141  A1    ---     f_omega2  [*] Remark or Note flag
     142  A1    ---     ---       [|] Separation character
 143-148  F6.1  km/s    K(1)      ? Orbital semi-amplitude of Primary
     149  A1    ---     u_K(1)    Uncertainty flag (:)
     150  A1    ---     f_K(1)    [*]    Remark or Note flag
     151  A1    ---     ---       [|] Separation character
 152-157  F6.1  km/s    K(2)      ? Orbital semi-amplitude of Secondary
     158  A1    ---     u_K(2)    Uncertainty flag (:)
     159  A1    ---     f_K(2)    Remark or Note flag
     160  A1    ---     ---       [|] Separation character
 161-166  F6.1  km/s    V(1)      ? Systemic Velocity of Primary
     167  A1    ---     u_V(1)    Uncertainty flag (:)
     168  A1    ---     f_V(1)    [*]    Remark or Note flag
     169  A1    ---     ---       [|] Separation character
 170-175  F6.1  km/s    V(2)      ? Systemic Velocity of Secondary
     176  A1    ---     u_V(2)    Uncertainty flag (:)
     177  A1    ---     f_V(2)    Remark or Note flag
     178  A1    ---     ---       [|] Separation character
     179  A1    ---     f_Quality [*]   Remark or Note flag
     180  A1    ---     Quality   [abcdei] Quality letter (3)
     181  A1    ---     ---       [|] Separation character
 182-251  A70   ---     Ref       Reference (makes use of TeX conventions)
--------------------------------------------------------------------------------
*/

double cal_mass(double vmag,double par_mas)
{
 double modulus,amag,bmag,mass,BC;
 
    modulus = 5 + 5 *0.4342944759 * log(par_mas/1000.0);
    amag = modulus + vmag;
    
//  IF Amag < -6! THEN PRINT " W A R N I N G, bolometric correction not valid."
    if     (amag>= 7.5) BC = -2.43 + 0.5071 * amag - 0.00846* amag * amag;
    else if(amag>= 1.1) BC =  0.54 - 0.341  * amag + 0.0518 * amag * amag;
    else if(amag>=-1.1) BC =  0.76 - 0.636  * amag + 0.2797 * amag * amag;
    else                BC =  1.03 - 0.775  * amag - 0.0659 * amag * amag;

    bmag = amag - BC;

	if(bmag >= 7.66) mass = pow(10.0, (5.8 - bmag)/6.0);
	else             mass = pow(10.0, (4.8 - bmag)/9.5);
	
	return mass;
}


/*
'                             Detection Index
'                           7/5/92   GG   9/7/99
'
'   Finds mass from absolute magnitude, Zombeck, pg 72 and the Detection Index,
'   Gatewood 1976 Icarus, 27,1.   Masses now match Wulf D. Heintz, pg 60.
'
40  PRINT " ----------------------------------------------------------------"
    PRINT
    INPUT " Apparent visual magnitude and Parallax"; Vmag, p
    IF Vmag = 0 GOTO 500
    PRINT Vmag, p
'
    PRINT
    Modulus = 5 + 5 * .4342944759# * LOG(p)
'
'       Ln (10) = 2.302585124969482;   1 / Ln (10) = 0.4342944758723105
'       Log  (X) = 0.4342944758723105 * Ln (X)                      
'       Dx Ln (X) = 1 / X;             Dx Log (X) = 0.4342944758723105 / X
'
    Amag = Modulus + Vmag
    IF Amag < -6! THEN PRINT " W A R N I N G, bolometric correction not valid."
    IF Amag >= 1.1 GOTO 120
    
	BC = 1.03 - .775 * Amag - .0659 * Amag * Amag
	IF Amag > -1.1 THEN BC = .76 - .636 * Amag + .2797 * Amag * Amag
	GOTO 200
'
120 BC = .54 - .341 * Amag + .0518 * Amag * Amag
	IF Amag > 7.5 THEN BC = -2.43 + .5071 * Amag - .00846 * Amag * Amag
'
200 Bmag = Amag - BC
    PRINT USING " Absolute and Bolometric magnitudes =###.### and ##.##"; Amag; Bmag
	mass = 10 ^ ((4.8 - Bmag) / 9.5)
	IF Bmag >= 7.66 THEN mass = 10 ^ ((5.8 - Bmag) / 6)
'
	PRINT USING "                   Approximate mass =###.###"; mass
	DI = p / (mass ^ (2 / 3))
    PRINT USING "                    Detection Index = ##.###"; DI
    PRINT
    GOTO 40
500 PRINT
    END
*/
//-------------------------------------------------------------------------------
int read_ep(int starno)
//-------------------------------------------------------------------------------
{
 FILE *inf;
 int hipno,ntotal,nacc,i,ii;
 char flag;
 double vminusi;
 double meanmag,sigmamag,error1,error2;
 double period,refepoch;
 char vartype,flag1,flag2;
 double bjd,magi,error;
 int    iflag;
 double epoch_arr[1000],mag_arr[1000],phase[1000];
 int plot_handle,plot_handle2;
 
    sprintf(buff,"%sHip_ep.dat",HIP_PATH);
 
    if( (inf=fopen(buff,"r"))==NULL)
      {
       MessagePopup("cannot open",buff);
       return -1;
      }
    
 do{
    if(fgets(buff,150,inf)==NULL) {fclose(inf); return -1;}
    
    sscanf (buff,"%i|%c|%lf|%i|%i"  ,&hipno,&flag,&vminusi,&ntotal,&nacc);
//    sprintf(buff,"%6i|%1c|%6.3lf|%3i|%3i\n",hipno,flag,vminusi,ntotal,nacc);
//    display_message(buff);  
    
    if(starno!=hipno) for(i=0;i<ntotal+2;i++) fgets(buff,150,inf);
    
    else
    {
//   sprintf(buff,"%6i|%1c|%6.3lf|%3i|%3i\n",hipno,flag,vminusi,ntotal,nacc);
     sprintf(buff,"%6i %1c %6.3lf %3i %3i\n",hipno,flag,vminusi,ntotal,nacc);
     display_message(buff);  
    
     fgets(buff,150,inf);
     sscanf (buff,"%lf|%lf|%lf|%lf"  ,&meanmag,&sigmamag,&error1,&error2);
     sprintf(buff,"%7.4lf|%6.4lf|%5.2lf|%5.2lf\n", meanmag, sigmamag, error1, error2);
     display_message(buff);  
    
     fgets(buff,150,inf);
     if(buff[3]=='.')
     sscanf (buff,"%lf|%lf|%c|%c|%c"  ,&period,&refepoch,&vartype,&flag1,&flag2);
     else {period = refepoch = 0; vartype = buff[22]; flag1 = buff[24]; flag2 = buff[26];}
//   sprintf(buff,"%11.7lf|%9.4lf|%1c|%1c|%1c\n", period, refepoch, vartype, flag1, flag2);
     sprintf(buff,"%11.7lf %9.4lf %1c %1c %1c\n", period, refepoch, vartype, flag1, flag2);
     display_message(buff);  
    
     ii = 0;
     for(i=0;i<ntotal;i++)
        {
         fgets(buff,150,inf);
         sscanf (buff,"%lf|%lf|%lf|%d",&bjd,&magi,&error,&iflag);
         if(iflag==0)
           {
            epoch_arr[ii] = bjd;
            mag_arr  [ii] = magi;
            ii++;
            sprintf(buff,"%3d %10.5lf %8.4lf %8.3lf\n", ii,bjd,magi,error);
            display_message(buff);
           }
//       display_message(buff);
        }
    }
       
  } while(starno!=hipno);
 
    sprintf(buff,"measure points %d\n",ii);
    display_message(buff);
//    plot_handle =  LoadPanel(0,"read_cat.uir",PLOT);
//    DisplayPanel(plot_handle);
//    auto_plot_xy_scatter(plot_handle,PLOT_GRAPH,epoch_arr,mag_arr,ii);
    
//    GetCtrlVal(main_handle,MAIN_PEREP,&period);
//    cal_phase(epoch_arr,period,ii,phase);
//    plot_handle2 =  LoadPanel(0,"read_cat.uir",PLOT2);
//    DisplayPanel(plot_handle2);
//    auto_plot_xy_scatter(plot_handle2,PLOT2_GRAPH,phase,mag_arr,ii);
    
    fclose(inf);
    return 0;
}      
//-------------------------------------------------------------------------------
static void cal_phase(double epoch[],double per, int ndata, double phase[])
//-----------------------------------------------------------------------------
{
 int i;
 double dt, t_0;

    t_0 = epoch[0];
    for(i=0;i<ndata;i++)
       {
        dt = (epoch[i] - t_0)/per;
        phase[i] = dt - floor(dt);
        if(phase[i]<0.0) phase[i] += 1.0;
       }
}
//-----------------------------------------------------------------------------
