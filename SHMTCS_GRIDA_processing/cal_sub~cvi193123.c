#include <utility.h>
#include <userint.h>
#include <ansi_c.h>

#include "cal_lib\astrometry.h"
#include "type_def.h"
#include "st4s_lib.h "  
#include "cvi_util.h" 
#include "gv_tel.h"  // global variables for telescope control

extern void display_message(char buff[]);

//static int ROTATE_PAXIS = 0;
static double XP[3],YP[3],ZP[3];

static const double  PI314           = 3.14159265358979323846;
static const double  TWOPI           = 3.14159265358979323846*2.0;

//-------------------------   Prototyping of Functions   ---------------------------
#include "cal_sub.h"  // global variables for telescope control

static int adjust_hadec(double *ha, double *dec);
static int cal_mount_model_new(int np_ra, int np_dec, int encoder_mode);

//----------------------------------------------------------------------------------
int cal_mount_model_radec(int np_ra, int np_dec, int encoder_mode)
//----------------------------------------------------------------------------------
{
 double ap_ha, ap_dec, alt, az, error_ha, error_dec, ha,dec;
 double cond_ra[100][10],cond_de[100][10], y_ra[100],y_de[100];
 double res_ra[100], para_ra[10], q_ra[10][10],atyra[10], use_ra;
 double res_de[100], para_de[10], q_de[10][10],atyde[10], use_de;
 double res1[100],res2[100],para1[10],para2[10],use1,use2;
 double ha_array[100]={0.,},dec_array[100]={0.,};
 
 double error1,error2;
 double enc_to_step_1,enc_to_step_2;
 long   step_ha, step_dec,enco_1,enco_2;
 
 FILE *fin,*fout;
 int ii,i, starno[100]={0,};
 int ndmax = 100, npmax = 10;
 int nd;
 char ans,buffer[300],fname[300];
 
//  Initialize the parameter and Least Squares matrix
    for(i=0;i<10;i++)
       {para_ra[i]=0.0; para_de[i]=0.0;
        q_ra[i][i]=0.0; q_de[i][i]=0.0;
       }
       
//---------------------  Calculate enc_to_step coefficients ------------------
    if(file_select("*.out", fname)!=1) return -1;
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
//  if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}

    fgets(buffer,200,fin);
    nd = 0;
    while(fgets(buffer,200,fin)!=NULL)
         {
          sscanf(buffer,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &ap_ha, &ap_dec, &alt, &az, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec);
         
          if(ii>0) nd++;
          y_ra[nd-1]          = step_ha;
          cond_ra[nd-1][0]    = 1.0;
          cond_ra[nd-1][1]    = enco_1;
              
          y_de[nd-1]          = step_dec;
          cond_de[nd-1][0]    = 1.0;
          cond_de[nd-1][1]    = enco_2;
         }
    fclose(fin);
    
    clsq(*cond_ra,y_ra,*q_ra,atyra,ndmax,nd,npmax,2,para1,res1, &use1);
    enc_to_step_1 = para1[1];
    clsq(*cond_de,y_de,*q_de,atyde,ndmax,nd,npmax,2,para2,res2, &use2);
    enc_to_step_2 = para2[1];

//---------------------      Calculate mount model         ------------------
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    nd = 0;
    while(fgets(buffer,200,fin)!=NULL)
         {
          sscanf(buffer,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
          &ii, &ap_ha, &ap_dec, &alt, &az, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec);
         
          if(ii>0)
            {
             nd++;
             starno[nd-1] = ii;
             ha_array [nd-1] = ap_ha/15.0;
             dec_array[nd-1] = ap_dec    ;
             
             ha  = deg_to_rad(ap_ha);
             dec = deg_to_rad(ap_dec);
                 
//           R.A. axis
             if(encoder_mode==0)
                y_ra[nd-1]    = step_ha;
             else
                y_ra[nd-1]    = enco_1;
                    
             cond_ra[nd-1][0] = 1.0;
             cond_ra[nd-1][1] = ap_ha*3600.0;                 // convert degree to arcsec
             cond_ra[nd-1][2] = tan(dec)*sin(ha);
             cond_ra[nd-1][3] = tan(dec)*cos(ha);             // polar axis non-alignment
             cond_ra[nd-1][4] = cos(ha);                      // eccentricity error
             cond_ra[nd-1][5] = sin(ha);  
             cond_ra[nd-1][6] = 1.0/cos(dec);                 // non-perpendicularity between R.A. & Dec. axis
             cond_ra[nd-1][7] = sin(dec);                     // non-perpendicularity between optical and mechnical axis
             cond_ra[nd-1][8] = sin(ha)*cos(SITE_LATI.rad);   // tube flexure, degenerate
//           cond_ra[nd-1][7] = cos(ha*25.0);          
//           cond_ra[nd-1][8] = sin(ha*25.0);          
                 
             if(encoder_mode==0)
                y_de[nd-1]    = step_dec;
             else
                y_de[nd-1]    = enco_2;

             cond_de[nd-1][0] = 1.0;
             cond_de[nd-1][1] = ap_dec*3600.0;                //convert degree to arcsec
             cond_de[nd-1][2] = cos(ha);
             cond_de[nd-1][3] =-sin(ha);                      // polar axis non-alignment
             cond_de[nd-1][4] = cos(dec);
             cond_de[nd-1][5] = sin(dec);
             cond_de[nd-1][6] = sin(SITE_LATI.rad)*cos(dec)   // tube flexure
                              - cos(SITE_LATI.rad)*sin(dec)*cos(ha);
            }
         }

    fclose(fin);
        
    if(nd<np_ra || nd<np_dec) 
      {Beep();
       MessagePopup(""," Too few data points...");
       return -2;
      }
      
    clsq(*cond_ra,y_ra,*q_ra,atyra,ndmax,nd,npmax,np_ra ,para_ra,res_ra,&use_ra);
    clsq(*cond_de,y_de,*q_de,atyde,ndmax,nd,npmax,np_dec,para_de,res_de,&use_de);

//------------------- Print the mount model ----------------------------------
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    fout = fopen("calmount.out","w");

    fprintf(fout,"                    << Inut Data & Residuals >>\n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  i         HA          DEC           Alt.          dHA       dDec.\n");
    fprintf(fout,"----------------------------------------------------------------------\n");

    i = 0;
    while(fgets(buffer,200,fin)!=NULL)
         {
          sscanf(buffer,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
          &ii, &ap_ha, &ap_dec, &alt, &az, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec);
         
          if(ii>0)             
            {i++;
//           change the scale of the residual to arcsec
             res_ra[i-1] /= para_ra[1];
             res_de[i-1] /= para_de[1];

             fprintf(fout,"%3d %12.3lf %12.3lf  %12.3lf  %10.1lf %10.1lf\n",
             starno[i-1], ap_ha/15.0, ap_dec, alt, res_ra[i-1], res_de[i-1]);
            }
         }
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout," u.s.e.                                      %10.1lf %10.1lf\n", use_ra/para_ra[1], use_de/para_de[1]);
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  unit : HA (hour), Dec, Alt (degree)  dHA, dDec, use (arcsec)\n");


    fprintf(fout,"                 << Mount model Parameters >>\n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"            para_RA/          error         para_Dec./         error  \n");
    fprintf(fout,"----------------------------------------------------------------------\n");

    i=0;
        error1 = use_ra*sqrt(q_ra[i][i])/para_ra[1];
        error2 = use_de*sqrt(q_de[i][i])/para_de[1];
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf\n", i,para_ra[i]/para_ra[1],error1,
                                                                   para_de[i]/para_de[1],error2);
       
    i=1;
        error1 = use_ra*sqrt(q_ra[i][i]);
        error2 = use_de*sqrt(q_de[i][i]);
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf\n", i,para_ra[i],error1, para_de[i],error2);
       
    for(i=2;i<10;i++)
       {
        error1 = use_ra*sqrt(q_ra[i][i])/para_ra[1];
        error2 = use_de*sqrt(q_de[i][i])/para_de[1];
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf\n", i,para_ra[i]/para_ra[1],error1,
                                                                   para_de[i]/para_de[1],error2);
       }
       
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  unit: i=1 steps   i =0,2,3... arcsec\n\n");

    fprintf(fout,"             << Inclination of the current polar axis >>         \n");
    fprintf(fout," HA[2],  Dec[2] : the inclination of the polar axis toward south\n");
    fprintf(fout," HA[3],  Dec[3] : the inclination of the polar axis toward east \n");
    fprintf(fout," HA[4],  Dec[4] : disk eccentricity \n");
    fprintf(fout," HA[5],  Dec[5] : disk eccentricity \n");
    fprintf(fout," HA[6]          : ra, dec axis perpendicularity error \n");
    fprintf(fout,"         DEC[6] : tube flexure \n");
    fprintf(fout," HA[7]          : optical & mechanical axis perpendicularity error \n");
    fprintf(fout," Note :  HA[2] includes tube flexure.\n");

    fclose(fin); fclose(fout);

//  new mount
    fout = fopen("newmount.inp","w");
    for(i=0;i<10;i++)
    fprintf(fout,"%3d %15.5lf  %15.5lf\n",i,para_ra[i],para_de[i]);
    for(i=10;i<20;i++)
    fprintf(fout,"%3d %15.5lf  %15.5lf\n",i,0.0,0.0);

    fprintf(fout,"%3d ENCODER_MODE\n",encoder_mode);
    fprintf(fout,"%12.5lf %12.5lf ENC_TO_STEP R.A. & DEC.\n",enc_to_step_1, enc_to_step_2);
    fprintf(fout,"%12.5lf %12.5lf ERROR       R.A. & DEC.\n",use1         , use2         );
    fclose(fout);

//  write out residual data
    fout = fopen("residual.out","w");
    fprintf(fout,"%10d\n",nd);
    for(i=0;i<nd;i++)
       fprintf(fout,"%10.3lf %10.3lf %10.3lf %10.3lf\n",ha_array[i],res_ra[i],dec_array[i],res_de[i]);
    fclose(fout);
    
    return 0;
}
//----------------------------------------------------------------------------------
int cal_mount_model_altazi(int np_ra, int np_dec, int encoder_mode)
//----------------------------------------------------------------------------
{
 double ap_ha, ap_dec, alt, azi, error_ha, error_dec, ha,dec;
 double cond_ra[100][10],cond_de[100][10], y_ra[100],y_de[100];
 double res_ra[100], para_ra[10],q_ra[10][11],atyra[10],use_ra;
 double res_de[100], para_de[10],q_de[10][11],atyde[10],use_de;
 double res1[100],res2[100],para1[10],para2[10],use1,use2;
 double azi_array[100]={0.,},alt_array[100]={0.,};
 char fname[300];
 
 double error1,error2;
 double enc_to_step_1,enc_to_step_2;
 long   step_ha, step_dec,enco_1,enco_2;
 
 FILE *fin,*fout, *fout_new;
 int ii,i, starno[100]={0,};
 int ndmax = 100, npmax = 10;
 int nd;
 char ans, buffer[200];
 
//  Initialize the parameter and Least Squares matrix
    for(i=0;i<10;i++)
       {para_ra[i]=0.0; para_de[i]=0.0;
        q_ra[i][i]=0.0; q_de[i][i]=0.0;
       }
       
//---------------------  Calculate enc_to_step coefficients -----------------
    if(file_select("*.out", fname)!=1) return -1;
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
//    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    nd = 0;
    while( fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &ap_ha, &ap_dec, &alt, &azi, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
           != EOF)
         {
          nd++;
          y_ra[nd-1]          = step_ha;
          cond_ra[nd-1][0]    = 1.0;
          cond_ra[nd-1][1]    = enco_1;
              
          y_de[nd-1]          = step_dec;
          cond_de[nd-1][0]    = 1.0;
          cond_de[nd-1][1]    = enco_2;
         }
    fclose(fin);
    clsq(*cond_ra,y_ra,*q_ra,atyra,ndmax,nd,npmax,2,para1,res1,&use1);
    enc_to_step_1 = para1[1];
    clsq(*cond_de,y_de,*q_de,atyde,ndmax,nd,npmax,2,para2,res2,&use2);
    enc_to_step_2 = para2[1];

//---------------------      Calculate mount model         ------------------
    fout_new = fopen("newmount.inp","w");
    fprintf(fout_new,"  -------   Input data   -----------\n");
    fprintf(fout_new," star_no ap_ha        ap_dec        alt          az        step_ha    step_dec   enco_1     enco_2    error_ha  error_dec\n");
    
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    nd = 0;
    while( fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &ap_ha, &ap_dec, &alt, &azi, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
              != EOF)
         {
          fprintf(fout_new,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
                  ii, ap_ha, ap_dec, alt, azi, step_ha, step_dec, enco_1,enco_2,error_ha, error_dec);
          if(ii>0)
            {
             nd++;
             starno[nd-1] = ii;
             azi_array[nd-1] = azi;
             alt_array[nd-1] = alt;
             
             cond_ra[nd-1][0] = 1.0;
             cond_ra[nd-1][1] = azi*3600.0;                 // convert degree to arcsec
             cond_de[nd-1][0] = 1.0;                          // zero point
             cond_de[nd-1][1] = alt*3600.0;                   // scale term
             
             azi = deg_to_rad(azi);
             alt = deg_to_rad(alt);
                 
//           R.A. axis
             if(encoder_mode==0)
                y_ra[nd-1]    = step_ha;
             else
                y_ra[nd-1]    = enco_1;
                    
             cond_ra[nd-1][2] = tan(alt)*sin(azi);
             cond_ra[nd-1][3] = tan(alt)*cos(azi);          // polar axis non-alignment
             cond_ra[nd-1][4] = cos(azi);                   // eccentricity error
             cond_ra[nd-1][5] = sin(azi);  
             cond_ra[nd-1][6] = 1.0/cos(alt);               // non-perpendicularity between R.A. & Dec. axis
             cond_ra[nd-1][7] = cos(azi*25.0);          
             cond_ra[nd-1][8] = sin(azi*25.0);          
//           cond_ra[nd-1][9] = sin(ha)*cos(SITE_LATI.rad); // tube flexure, degenerate
                 
             if(encoder_mode==0)
                y_de[nd-1]    = step_dec;
             else
                y_de[nd-1]    = enco_2;

             cond_de[nd-1][2] = cos(azi);					  // polar axis non-alignment
             cond_de[nd-1][3] =-sin(azi);                     
             cond_de[nd-1][4] = cos(alt);
             cond_de[nd-1][5] = sin(alt);
             cond_de[nd-1][6] = sin(SITE_LATI.rad)*cos(alt)   // tube flexure
                              - cos(SITE_LATI.rad)*sin(alt)*cos(azi);
            }
         }

    fclose(fin);
        
    if(nd<np_ra || nd<np_dec) 
      {Beep();
       MessagePopup(""," Too few data points...");
       return -2;
      }
      
    clsq(*cond_ra,y_ra,*q_ra,atyra, ndmax,nd,npmax,np_ra ,para_ra,res_ra, &use_ra);
    clsq(*cond_de,y_de,*q_de,atyde, ndmax,nd,npmax,np_dec,para_de,res_de, &use_de);

//------------------- Print the mount model ---------------------------------
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    fout = fopen("calmount.out","w");

    fprintf(fout,"                    << Inut Data & Residuals >>\n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  i         HA          Azi           Alt.          dAzi      dAlt    \n");
    fprintf(fout,"----------------------------------------------------------------------\n");

    i = 0;
    while( fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
            &ii, &ap_ha, &ap_dec, &alt, &azi, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
          != EOF)
         {
          if(ii>0)             
            {i++;
//           change the scale of the residual to arcsec
             res_ra[i-1] /= para_ra[1];
             res_de[i-1] /= para_de[1];

             fprintf(fout,"%3d %12.3lf %12.3lf  %12.3lf  %10.1lf %10.1lf\n",
             starno[i-1], ap_ha/15.0, azi, alt, res_ra[i-1], res_de[i-1]);
            }
         }
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout," u.s.e.                                      %10.1lf %10.1lf\n", use_ra/para_ra[1], use_de/para_de[1]);
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  unit : HA (hour), Azi, Alt (degree)  dAzi, dAlt, use (arcsec)\n");


    fprintf(fout,"                 << Mount model Parameters >>\n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"           para_Azi/          error         para_Alt./         error  \n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    for(i=0;i<2;i++)
       {
        error1 = use_ra*sqrt(q_ra[i][i]); error2 = use_de*sqrt(q_de[i][i]);
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf   \n",
                i,para_ra[i],error1, para_de[i],error2);
       }
    for(i=2;i<10;i++)
       {
        error1 = use_ra*sqrt(q_ra[i][i]);
        error2 = use_de*sqrt(q_de[i][i]);
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf   \n",
                i,para_ra[i]/para_ra[1],error1/para_ra[1], para_de[i]/para_de[1],error2/para_de[1]);
       }
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  unit : i=0,1 steps   i = 2,3... arcsec\n\n");

    fprintf(fout,"             << Inclination of the current polar axis >>         \n");
    fprintf(fout," AZI[2],  ALT[2] : the inclination of the Alt. axis toward south\n");
    fprintf(fout," AZI[3],  ALT[3] : the inclination of the Alt. axis toward east \n");
    fprintf(fout," Note :  AZI[2] includes tube flexure.\n");

    fclose(fin); fclose(fout);

    fprintf(fout_new,"                 << Mount model Parameters >>\n");
    fprintf(fout_new,"----------------------------------------------------------------------\n");
    fprintf(fout_new,"           para_AZI          para_ALT. (unit: steps)\n");
    fprintf(fout_new,"----------------------------------------------------------------------\n");
    for(i=0;i<10;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,para_ra[i],para_de[i]);

    for(i=10;i<20;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,0.0,0.0);

    fprintf(fout_new,"%3d ENCODER_MODE\n",encoder_mode);
    fprintf(fout_new,"%12.5lf %12.5lf ENC_TO_STEP R.A. & DEC.\n",enc_to_step_1, enc_to_step_2);
    fprintf(fout_new,"%12.5lf %12.5lf ERROR       R.A. & DEC.\n",use1         , use2         );
    fclose(fout_new);

//  new mount
    fout_new = fopen("newmount.dat","w");
    for(i=0;i<10;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,para_ra[i],para_de[i]);
    for(i=10;i<20;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,0.0,0.0);

    fprintf(fout_new,"%3d ENCODER_MODE\n",encoder_mode);
    fprintf(fout_new,"%12.5lf %12.5lf ENC_TO_STEP AZI & ALT.\n",enc_to_step_1, enc_to_step_2);
    fprintf(fout_new,"%12.5lf %12.5lf ERROR       AZI & ALT.\n",use1         , use2         );
    fclose(fout_new);

//  write out residual data
    fout_new = fopen("residual.out","w");
    fprintf(fout_new,"%10d\n",nd);
    for(i=0;i<nd;i++)
       fprintf(fout_new,"%10.3lf %10.3lf %10.3lf %10.3lf\n",azi_array[i],res_ra[i],alt_array[i],res_de[i]);
    fclose(fout_new);
    
    return 0;
}
//----------------------------------------------------------------------------
double calc_ha_count(double ha,double dec, double azi,double alt)
//----------------------------------------------------------------------------------
{
 double temp, ha_sec;
 
//    if(ROTATE_PAXIS == 1) adjust_hadec(&ha, &dec);
    if(TELESCOPE_TYPE==_ALT_AZI) {ha=azi;dec=alt;}

    ha_sec = rad_to_deg(ha)*3600.0;  //convert radian to arcsec
    temp =  MOUNT_HA[0]
          + MOUNT_HA[1]*ha_sec
          + MOUNT_HA[2]*tan(dec)*sin(ha)
          + MOUNT_HA[3]*tan(dec)*cos(ha)             // polar axis non-alignment
          + MOUNT_HA[4]*cos(ha)                      // eccentricity error
          + MOUNT_HA[5]*sin(ha)                   
          + MOUNT_HA[6]/cos(dec)                     // non-perpendicularity between R.A. & Dec. axis
          + MOUNT_HA[7]*sin(dec)		             // non-perpendicularity between optical and mechnical axis
          + MOUNT_HA[8]*sin(ha)*cos(SITE_LATI.rad)   // tube flexure, degenerate
          ;
	sprintf(GBUFF, "MOUNT_HA[0] = %d, MOUNT_HA[1] = %lf, MOUNT_HA[2] = %lf, MOUNT_HA[3] = %lf, MOUNT_HA[4] = %lf \n", MOUNT_HA[0], MOUNT_HA[1], MOUNT_HA[2], MOUNT_HA[3], MOUNT_HA[4]);
	display_message(GBUFF);
    return temp;
}
//----------------------------------------------------------------------------------
double calc_dec_count(double ha,double dec, double azi,double alt)
//----------------------------------------------------------------------------------
{
 double temp, dec_sec;
 
//  if(ROTATE_PAXIS == 1) adjust_hadec(&ha, &dec);
    if(TELESCOPE_TYPE==_ALT_AZI) {ha=azi;dec=alt;}
 
    dec_sec = rad_to_deg(dec)*3600.0; //convert radian to arcsec

    temp = MOUNT_DEC[0]
         + MOUNT_DEC[1]*dec_sec
         + MOUNT_DEC[2]*cos(ha)
         - MOUNT_DEC[3]*sin(ha)                                 // polar axis non-alignment
         + MOUNT_DEC[4]*cos(dec)
         + MOUNT_DEC[5]*sin(dec)
         + MOUNT_DEC[6]*( sin(SITE_LATI.rad)*cos(dec)           // tube flexure
                        - cos(SITE_LATI.rad)*sin(dec)*cos(ha) )
        ;
    return temp;
}
//----------------------------------------------------------------------------------
double calc_der_count(double psi)
//----------------------------------------------------------------------------------
{
     return rad_to_deg(psi)*PSI_FACTOR;
}
//----------------------------------------------------------------------------------
int calc_hadec(double *ha, double *dec)
//----------------------------------------------------------------------------------
{
 long count1,count2;
    if(ENCODER_MODE==0)
      {read_counter('A',&count1); read_counter('B',&count2);}
    else
      {read_encoder('A',&count1); read_encoder('B',&count2);}
      
    calc_position1(count1,count2,ha,dec);
        
    return 0;
}
//----------------------------------------------------------------------------------
int calc_radec(double *ra, double *dec)
//----------------------------------------------------------------------------------
{
 long count1,count2;
 double jdutc;

    if(ENCODER_MODE==0)
      {read_counter('A',&count1); read_counter('B',&count2);}
    else
      {read_encoder('A',&count1); read_encoder('B',&count2);}

    calc_position1(count1,count2,ra,dec);
    jdutc = get_jdutc_clock();
    *ra = jdtolast(jdutc, SITE_LONGI.rad) - *ra;
        
    return 0;
}
//----------------------------------------------------------------------------------
int calc_position1(long count_ha, long count_de, double *ha, double *dec)
//----------------------------------------------------------------------------------
{
 double temp1,temp2, del1,del2;
 double dha,ddec, tol = 0.1;
 int i;
 
 del1 = 0.0; del2 = 0.0;

//  calculate the first approximation
    temp1 = (count_ha - MOUNT_HA[0]  - del1) / MOUNT_HA[1]  / 3600.0;
    temp2 = (count_de - MOUNT_DEC[0] - del2) / MOUNT_DEC[1] / 3600.0;

//  printf(" %3d HA: %15.2lf  Dec.: %15.2lf\n",i,temp1*3600.0,temp2*3600.0);
    temp1 = deg_to_rad(temp1);          // convert degree to radian;
    temp2 = deg_to_rad(temp2);          // convert degree to radian;

    i = 0;
    do
     {
      del1  =  calc_ha_count(temp1,temp2,temp1,temp2)
             - MOUNT_HA[0] -  MOUNT_HA[1] *rad_to_deg(temp1)*3600.0;
      del2  =  calc_dec_count(temp1,temp2,temp1,temp2)
             - MOUNT_DEC[0]-  MOUNT_DEC[1]*rad_to_deg(temp2)*3600.0;

      *ha  = deg_to_rad( (count_ha - MOUNT_HA[0]  - del1) / MOUNT_HA[1]  / 3600.0 );
      *dec = deg_to_rad( (count_de - MOUNT_DEC[0] - del2) / MOUNT_DEC[1] / 3600.0 );
      dha  = (*ha - temp1)*206265.0;
      ddec = (*dec- temp2)*206265.0;

      i++;
      temp1 = *ha; temp2 = *dec;

      if(i>=10) return i;
     }while( fabs(dha)>tol || fabs(ddec)>tol );
	
    if(*ha > PI314) *ha = *ha - TWOPI;

    return 0;
}
//----------------------------------------------------------------------------------
int calc_mean_tracking_freq(h_m_s ra_2000, d_m_s dec_2000)
//----------------------------------------------------------------------------------
{
 double jd_ut1;
 double count_azi_now, count_azi_next;
 double count_alt_now, count_alt_next;
 double count_psi_now, count_psi_next;
 h_m_s last,ap_ha;
 d_m_s ap_dec,alt,azi;
 double psi;
 
 
//  jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    jd_ut1 = JDUTC_0 + Timer()/3600.0/24.0 + DUT1_DAY;
/*    
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);
*/
    get_alt_azi_psi(ra_2000.rad, dec_2000.rad, jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
                    &azi.rad,&alt.rad,&psi);
              
    count_azi_now = calc_ha_count (ra_2000.rad, dec_2000.rad,azi.rad,alt.rad);
    count_alt_now = calc_dec_count(ra_2000.rad, dec_2000.rad,azi.rad,alt.rad);
    count_psi_now = calc_der_count(psi);

    jd_ut1 += SERVO_UPDATE_SEC/3600.0/24.0;
/*    
    get_aposi(ra_2000.rad, dec_2000.rad, jd_ut1, SITE_LONGI.rad, SITE_LATI.rad,1,
              &last.rad, &ap_ha.rad, &ap_dec.rad, &alt.rad,&azi.rad);
*/              
    get_alt_azi_psi(ra_2000.rad, dec_2000.rad, jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
                    &azi.rad,&alt.rad,&psi);
              
    count_azi_next = calc_ha_count (ra_2000.rad, dec_2000.rad,azi.rad,alt.rad);
    count_alt_next = calc_dec_count(ra_2000.rad, dec_2000.rad,azi.rad,alt.rad);
    count_psi_next = calc_der_count(psi);
    
    MEAN_TRACKING_FREQ[0] = (count_azi_next - count_azi_now) / SERVO_UPDATE_SEC;
    MEAN_TRACKING_FREQ[1] = (count_alt_next - count_alt_now) / SERVO_UPDATE_SEC;
//    MEAN_TRACKING_FREQ[2] = (count_psi_next - count_psi_now) / SERVO_UPDATE_SEC;
    
    if(ENCODER_MODE==1)
      {MEAN_TRACKING_FREQ[0] *= ENC_TO_STEP[0];
       MEAN_TRACKING_FREQ[1] *= ENC_TO_STEP[1];
      }
    return 0;
}
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
int read_mount_model(char file_name[])
//----------------------------------------------------------------------------------
{
 FILE *fin;
 char ssss[200];
 int i,ii;
 
    if( (fin = fopen(file_name,"r"))==NULL )
      {Beep(); MessagePopup(""," CANNOT OPEN MOUNT.INP \n");
       return -1;
      }

    for(i=0;i<20;i++)
       {
        fgets(ssss,150,fin);
        sscanf(ssss,"%d %lf %lf", &ii, &MOUNT_HA[i], &MOUNT_DEC[i]);
       }
    fgets(ssss,150,fin); sscanf(ssss,"%d",&ENCODER_MODE);
    fgets(ssss,150,fin); sscanf(ssss,"%lf %lf",&ENC_TO_STEP[0],&ENC_TO_STEP[1]);
    
    if(TELESCOPE_TYPE==_ALT_AZI)
      {fgets(ssss,150,fin); sscanf(ssss,"%lf",&PSI_FACTOR);}
    
    fclose(fin);

    OFFSET_HA_COUNT = OFFSET_DEC_COUNT = 0.0;

    return 0;
}
//----------------------------------------------------------------------------------
void calc_error_count_radec(h_m_s ra, d_m_s dec, double *error_ha, double *error_dec)
//----------------------------------------------------------------------------------
{
 double last,ap_ha,ap_dec,alt,az,jd_ut1;
 long int step_ha,step_dec;

    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1, &last,&ap_ha,&ap_dec,&alt,&az);

    if(ENCODER_MODE==0)
      {read_counter('A',&step_ha); read_counter('B',&step_dec);}
    else
      {read_encoder('A',&step_ha); read_encoder('B',&step_dec);}

    *error_ha  = calc_ha_count (ap_ha,ap_dec,alt,az) - step_ha  ;
    *error_dec = calc_dec_count(ap_ha,ap_dec,alt,az) -  step_dec;
}
//----------------------------------------------------------------------------
void calc_error_count_altazi(h_m_s ra, d_m_s dec, double *error_azi, double *error_alt)
//----------------------------------------------------------------------------
{
 double last,ap_ha,ap_dec,alt,azi,jd_ut1;
 long int step_alt,step_azi;

    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1, &last,&ap_ha,&ap_dec,&alt,&azi);

    if(ENCODER_MODE==0)
      {read_counter('A',&step_azi); read_counter('B',&step_alt);}
    else
      {read_encoder('A',&step_azi); read_encoder('B',&step_alt);}

    *error_azi = calc_ha_count (ap_ha,ap_dec,azi,alt) - step_azi;
    *error_alt = calc_dec_count(ap_ha,ap_dec,azi,alt) - step_alt;
}
//----------------------------------------------------------------------------
static int adjust_hadec(double *ha, double *dec)
//----------------------------------------------------------------------------
{
 double r[3];
// double x,y,z;
 
    stor(*ha,*dec,r);
    *dec = asin(dproduct(r,ZP));
    *ha  = atan2(dproduct(r,YP),dproduct(r,XP));

    return 0;
}
//----------------------------------------------------------------------------
double speed_to_freq(char caxis, double speed)
//----------------------------------------------------------------------------
{
 double f;
 int iaxis;
 
    if(caxis=='A') f = speed*MOUNT_HA [1];
    else           f = speed*MOUNT_DEC[1];
    
    iaxis = caxis - 'A';
    if(ENCODER_MODE==1) f *= ENC_TO_STEP[iaxis];
    
    return f;
}
//----------------------------------------------------------------------------
double freq_to_speed(char caxis, double freq)
//----------------------------------------------------------------------------
{
 double speed;
 int iaxis;
 
    iaxis = caxis - 'A';
 
    if(caxis=='A') speed = freq/MOUNT_HA [1];
    else           speed = freq/MOUNT_DEC[1];
    
    if(ENCODER_MODE==1) speed /= ENC_TO_STEP[iaxis];
    
    return speed;
}
//----------------------------------------------------------------------------
static int cal_mount_model_new(int np_ra, int np_dec, int encoder_mode)
//----------------------------------------------------------------------------
{
 double ap_ha, ap_dec, alt, az, error_ha, error_dec, ha,dec;
 double cond_ra[100][10],cond_de[100][10], y_ra[100],y_de[100];
 double res_ra[100] = {0.,}, para_ra[10] = {0.,}, q_ra[10][11] = {0., }, use_ra;
 double res_de[100] = {0., }, para_de[10] = {0., }, q_de[10][11] = {0., }, use_de = 0.0;
 double res1[100],res2[100],para1[10] = {0.,},para2[10] = {0.,},use1 = 0.0, use2 = 0.0;
 double ha_array[100] = {0., },dec_array[100] = {0., };
 
 double error1,error2;
 double enc_to_step_1 = 0.0, enc_to_step_2 = 0.0 ;
 long   step_ha, step_dec,enco_1,enco_2;
 
 FILE *fin,*fout, *fout_new;
 int ii,i, starno[100] = {0,};
 int ndmax = 100, npmax = 10;
 int nd,ndata,npara;
 char ans, buffer[200];
 
 double hhh,ddd,att,azz;
 

//  Initialize the parameter and Least Squares matrix
    for(i=0;i<10;i++)
       {para_ra[i]=0.0; para_de[i]=0.0;
        q_ra[i][i]=0.0; q_de[i][i]=0.0;
       }
       
//---------------------  Calculate enc_to_step coefficients ------------------
    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    nd = 0;
    while(
/*    
          fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &ap_ha, &ap_dec, &alt, &az, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
           != EOF)
*/           
          fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &hhh, &ddd, &att, &azz, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
           != EOF)
         {
          if(TELESCOPE_TYPE==_RA_DEC) {ap_ha = hhh; ap_dec=ddd; alt = att; az = azz;}
          else                        {ap_ha = azz; ap_dec=att; alt = ddd; az = hhh;}
           
          nd++;
          y_ra[nd-1]          = step_ha;
          cond_ra[nd-1][0]    = 1.0;
          cond_ra[nd-1][1]    = enco_1;
              
          y_de[nd-1]          = step_dec;
          cond_de[nd-1][0]    = 1.0;
          cond_de[nd-1][1]    = enco_2;
         }
    fclose(fin);
    
    if(nd<4)
      {MessagePopup("","관측한 별이 적어도 4개가 필요합니다.");
       return -1;
      }
      
    if(ENCODER_MODE==1)
      {
//       clsq(*cond_ra,y_ra,*q_ra,para1,res1, &use1,ndmax,nd,npmax,2);
       enc_to_step_1 = para1[1];
//       clsq(*cond_de,y_de,*q_de,para2,res2, &use2,ndmax,nd,npmax,2);
       enc_to_step_2 = para2[1];
      }
//---------------------      Calculate mount model         ------------------
    fout_new = fopen("newmount.inp","w");
    fprintf(fout_new,"  -------   Input data   -----------\n");
    fprintf(fout_new," star_no ap_ha        ap_dec        alt          az        step_ha    step_dec   enco_1     enco_2    error_ha  error_dec\n");
    
    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    nd = 0;
    while(
/*    
          fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &ap_ha, &ap_dec, &alt, &az, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
              != EOF)
*/

          fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &hhh, &ddd, &att, &azz, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
           != EOF)
         {
          if(TELESCOPE_TYPE==_RA_DEC) {ap_ha = hhh; ap_dec=ddd; alt = att; az = azz;}
          else                        {ap_ha = azz; ap_dec=att; alt = ddd; az = hhh;}

          fprintf(fout_new,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
                  ii, ap_ha, ap_dec, alt, az, step_ha, step_dec, enco_1,enco_2,error_ha, error_dec);
          if(ii>0)
            {
             nd++;
             starno   [nd-1] = ii;
             ha_array [nd-1] = ap_ha/15.0;
             dec_array[nd-1] = ap_dec    ;
             
             ha  = deg_to_rad(ap_ha);
             dec = deg_to_rad(ap_dec);

             ii = (nd-1)*2;
             if(encoder_mode==0) {y_ra[ii] = step_ha; y_ra[ii+1] = step_dec;}
             else                {y_ra[ii] = enco_1 ; y_ra[ii+1] = enco_2  ;}

             cond_ra[ii  ][0] = 1.0;
             cond_ra[ii  ][1] = ap_ha*3600.0;        // convert degree to arcsec
             cond_ra[ii  ][4] = tan(dec)*sin(ha);
             cond_ra[ii  ][5] = tan(dec)*cos(ha);    
             cond_ra[ii+1][2] = 1.0;                
             cond_ra[ii+1][3] = ap_dec*3600.0;  
             cond_ra[ii+1][4] = cos(ha);             
             cond_ra[ii+1][5] =-sin(ha);  
             cond_ra[ii  ][2] = 0.0;
             cond_ra[ii  ][3] = 0.0;
             cond_ra[ii+1][0] = 0.0;
             cond_ra[ii+1][1] = 0.0;
            }
         }

    fclose(fin);

    ndata = nd*2;        
    npara = 6;
//  clsq(*cond_ra,y_ra,*q_ra,para_ra,res_ra,&use_ra,ndmax,ndata,npmax,npara);

    use_ra = 0.0;
    use_de = 0.0;
    for(i=0;i<nd;i++)
       {
        ii = i*2;
        res_ra[i] = res_ra[ii  ]; use_ra += res_ra[i]*res_ra[i];
        res_de[i] = res_ra[ii+1]; use_de += res_de[i]*res_de[i];
       }
        
    use_ra = sqrt(use_ra/(nd-3.0));
    use_de = sqrt(use_de/(nd-3.0));
    
    para_de[0] = para_ra[2];
    para_de[1] = para_ra[3];
    
    para_ra[2] = para_ra[4];
    para_ra[3] = para_ra[5];
    
    para_de[2] = para_ra[4];
    para_de[3] =-para_ra[5];
    
//------------------- Print the mount model ----------------------------------
    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    fout = fopen("calmount.out","w");

    fprintf(fout,"                    << Inut Data & Residuals >>\n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  i         HA          DEC           Alt.        dHA        dDec.\n");
    fprintf(fout,"----------------------------------------------------------------------\n");

    i = 0;
    while(
/*    
          fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
            &ii, &ap_ha, &ap_dec, &alt, &az, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
          != EOF)
*/
          fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &hhh, &ddd, &att, &azz, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
           != EOF)
         {
          if(TELESCOPE_TYPE==_RA_DEC) {ap_ha = hhh; ap_dec=ddd; alt = att; az = azz;}
          else                        {ap_ha = azz; ap_dec=att; alt = ddd; az = hhh;}

          if(ii>0)             
            {i++;
//           change the scale of the residual to arcsec
             res_ra[i-1] /= para_ra[1];
             res_de[i-1] /= para_de[1];

             fprintf(fout,"%3d %12.3lf %12.3lf  %12.3lf  %10.1lf %10.1lf\n",
             starno[i-1], ap_ha/15.0, ap_dec, alt, res_ra[i-1], res_de[i-1]);
            }
         }
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout," u.s.e.                                      %10.1lf %10.1lf\n", use_ra/para_ra[1], use_de/para_de[1]);
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  unit : HA (hour), Dec, Alt (degree)  dHA, dDec, use (arcsec)\n");


    fprintf(fout,"                 << Mount model Parameters >>\n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"            para_RA/          error         para_Dec./         error  \n");
    fprintf(fout,"----------------------------------------------------------------------\n");
    for(i=0;i<2;i++)
       {
        error1 = use_ra*sqrt(q_ra[i][i]); error2 = use_de*sqrt(q_de[i][i]);
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf   \n",
                i,para_ra[i],error1, para_de[i],error2);
       }
    for(i=2;i<10;i++)
       {
        error1 = use_ra*sqrt(q_ra[i][i]);
        error2 = use_de*sqrt(q_de[i][i]);
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf   \n",
                i,para_ra[i]/para_ra[1],error1/para_ra[1], para_de[i]/para_de[1],error2/para_de[1]);
       }
    fprintf(fout,"----------------------------------------------------------------------\n");
    fprintf(fout,"  unit : i=0,1 steps   i = 2,3... arcsec\n\n");

    fprintf(fout,"             << Inclination of the current polar axis >>         \n");
    fprintf(fout," HA[2],  Dec[2] :  the inclination of the polar axis toward south\n");
    fprintf(fout," HA[3], -Dec[3] :  the inclination of the polar axis toward east \n");
    fprintf(fout," Note :  HA[2] includes tube flexure.\n");

    fclose(fin); fclose(fout);

    fprintf(fout_new,"                 << Mount model Parameters >>\n");
    fprintf(fout_new,"----------------------------------------------------------------------\n");
    fprintf(fout_new,"            para_RA          para_Dec. (unit: steps)\n");
    fprintf(fout_new,"----------------------------------------------------------------------\n");
    for(i=0;i<10;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,para_ra[i],para_de[i]);

    for(i=10;i<20;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,0.0,0.0);

    fprintf(fout_new,"%3d ENCODER_MODE\n",encoder_mode);
    fprintf(fout_new,"%12.5lf %12.5lf ENC_TO_STEP R.A. & DEC.\n",enc_to_step_1, enc_to_step_2);
    fprintf(fout_new,"%12.5lf %12.5lf ERROR       R.A. & DEC.\n",use1         , use2         );
    fclose(fout_new);

//  new mount
    fout_new = fopen("newmount.dat","w");
    for(i=0;i<10;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,para_ra[i],para_de[i]);
    for(i=10;i<20;i++)
    fprintf(fout_new,"%3d %15.5lf  %15.5lf\n",i,0.0,0.0);

    fprintf(fout_new,"%3d ENCODER_MODE\n",encoder_mode);
    fprintf(fout_new,"%12.5lf %12.5lf ENC_TO_STEP R.A. & DEC.\n",enc_to_step_1, enc_to_step_2);
    fprintf(fout_new,"%12.5lf %12.5lf ERROR       R.A. & DEC.\n",use1         , use2         );
    fclose(fout_new);

//  write out residual data
    fout_new = fopen("residual.out","w");
    fprintf(fout_new,"%10d\n",nd);
    for(i=0;i<nd;i++)
       fprintf(fout_new,"%10.3lf %10.3lf %10.3lf %10.3lf\n",ha_array[i],res_ra[i],dec_array[i],res_de[i]);
    fclose(fout_new);
    
    return 0;
}
//----------------------------------------------------------------------------
