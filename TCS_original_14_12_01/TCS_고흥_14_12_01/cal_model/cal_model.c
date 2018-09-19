#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "type_def.h"
#include "cal_model.h"
#include "c:\lib\cal_lib\cvi_util.h"  
#include "c:\lib\cal_lib\mathsub.h"
//-----------------------------------------------------------------------------
static int cal_handle;
d_m_s   SITE_LONGI,SITE_LATI;
double  SITE_HEIGHT;

int cal_mount_model_radec(int np_ra, int np_dec, int encoder_mode);
int cal_mount_model_altazi(int np_ra, int np_dec, int encoder_mode);
int read_site(void);
double PARARA[10],PARADE[10];
double ERROR1[10],ERROR2[10];
int    IADJRA[10],IADJDE[10];

int read_para0(void);

int display_message(char buff[]);
int cal_mount_model_new(void);
int cal_mount_model_china(void);

//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
//------------------------------------------------------------------------
{
 int i;
 
    if(InitCVIRTE (0, argv, 0) == 0) return -1;	/* out of memory */
    
    for(i=0;i<10;i++) IADJRA[i]=IADJDE[i]=1;
    for(i=0;i<10;i++) PARARA[i]=PARADE[i]=0;

    read_site();
//  read_para0();
    
    cal_handle = LoadPanel(0,"cal_model.uir",CMM);
    DisplayPanel(cal_handle);
    
    GetCtrlVal(cal_handle,CMM_RAS ,&IADJRA[1]);    
    GetCtrlVal(cal_handle,CMM_DECS,&IADJDE[1]);    

    RunUserInterface();
//    DiscardPanel(cal_handle);
    return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK CB_CAL (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//--------------------------------------------------------------------------------
{
 int i,npara_ra,npara_dec,ndata;
 FILE *scan_out;
 char buff[200];
 double ha_array[50],res_ra[50],dec_array[50],res_de[50];
 int plot_1,plot_2;
 
	switch (event)
		{
		case EVENT_COMMIT:
		     if(control==CMM_START)
			   {
			    GetCtrlVal(cal_handle,CMM_RA ,&npara_ra );
			    GetCtrlVal(cal_handle,CMM_DEC,&npara_dec);
			    
                for(i=0;i<npara_ra ;i++)  IADJRA[i]=1;
                for(i=npara_ra ;i<10;i++){IADJRA[i]=0;PARARA[i]=0;}
                for(i=0;i<npara_dec;i++)  IADJDE[i]=1;
                for(i=npara_dec;i<10;i++){IADJDE[i]=0;PARADE[i]=0;}
			    
                GetCtrlVal(cal_handle,CMM_RAS ,&IADJRA[1]);    
                GetCtrlVal(cal_handle,CMM_DECS,&IADJDE[1]);    
			    
//                if(TELESCOPE_TYPE==_RA_DEC)
//                 cal_mount_model_radec (npara_ra,npara_dec,ENCODER_MODE);
//                 cal_mount_model_radec (npara_ra,npara_dec,1);
//                else
//                   cal_mount_model_altazi(npara_ra,npara_dec,ENCODER_MODE);

//               cal_mount_model_china();
                 cal_mount_model_new();
/*                 
                
                if((scan_out = fopen("calmount.out","r"))==NULL)
                  {puts(" CANNOT OPEN skyscan.out \n"); return -2;}
                 
                ResetTextBox(cal_handle,CMM_TEXTBOX,"  ");
                while( fgets(buff,150,scan_out) != NULL)
                SetCtrlVal(cal_handle,CMM_TEXTBOX,buff);
                fclose(scan_out);

//              plot residual                
                if((scan_out = fopen("residual.out","r"))==NULL)
                  {puts(" CANNOT OPEN residual.out \n"); return -2;}
                fscanf(scan_out,"%d\n",&ndata);
                for(i=0;i<ndata;i++)
                fscanf(scan_out,"%lf %lf %lf %lf\n",&ha_array[i],&res_ra[i],&dec_array[i],&res_de[i]);
                fclose(scan_out);
                
//                plot_1 = auto_plot_xy_scatter(graph_handle,CAL_2_GRAPH_RA ,ha_array ,res_ra, ndata);
//                plot_2 = auto_plot_xy_scatter(graph_handle,CAL_2_GRAPH_DEC,dec_array,res_de, ndata);
                plot_1 = auto_plot_xy_scatter(cal_handle,CMM_GRAPH_RA ,ha_array ,res_ra, ndata);
                plot_2 = auto_plot_xy_scatter(cal_handle,CMM_GRAPH_DEC,dec_array,res_de, ndata);
*/                
               }
		
	     else if(control==CMM_READINIT) read_para0();
	     else if(control==CMM_CLOSE)
	       {
	        DiscardPanel(cal_handle);
            QuitUserInterface(0);
           }
           
			break;
		}
	return 0;
}
//--------------------------------------------------------------------------------
int CVICALLBACK CBTB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_COMMIT:

			break;
		case EVENT_RIGHT_CLICK:
		     ResetTextBox(panel,control,"");
		    break;
		}
	return 0;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------------
int cal_mount_model_radec(int np_ra, int np_dec, int encoder_mode)
//----------------------------------------------------------------------------------
{
 double ap_ha, ap_dec, alt, az, error_ha, error_dec, ha,dec;
 double cond_ra[100][10],cond_de[100][10], y_ra[100],y_de[100];
 double res_ra[100], para_ra[10], q_ra[10][10],atyra[10], use_ra;
 double res_de[100], para_de[10], q_de[10][10],atyde[10], use_de;
 double res1[100],res2[100],para1[10],para2[10],use1,use2;
 double ha_array[100],dec_array[100];
 
 double error1,error2;
 double enc_to_step_1,enc_to_step_2;
 long   step_ha, step_dec,enco_1,enco_2;
 
 FILE *fin,*fout;
 int ii,i,iii,starno[100];
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
             if(encoder_mode==0) y_ra[nd-1]    = step_ha;
             else				 y_ra[nd-1]    = enco_1;
             
            iii=0;
//          zero point            
            if(IADJRA[0]==1){cond_ra[nd-1][iii] = 1.0; iii++;}
            y_ra[nd-1]-= PARARA[0];
            
//          slope: pulse/arcsec
            if(IADJRA[1]==1){cond_ra[nd-1][iii] = ap_ha*3600.0; iii++;}
            y_ra[nd-1]-= PARARA[1]*ap_ha*3600.0;
            
//          polar axis            
            if(IADJRA[2]==1){cond_ra[nd-1][iii] = tan(dec)*sin(ha); iii++;}
            y_ra[nd-1]-=PARARA[2]*tan(dec)*sin(ha);
            if(IADJRA[3]==1){cond_ra[nd-1][iii] = tan(dec)*cos(ha); iii++;}
            y_ra[nd-1]-= PARARA[3]*tan(dec)*cos(ha);
            
//          eccentricity            
            if(IADJRA[4]==1){cond_ra[nd-1][iii] = cos(ha); iii++;}      
            y_ra[nd-1]-= PARARA[4]*cos(ha);
            if(IADJRA[5]==1){cond_ra[nd-1][iii] = sin(ha); iii++;}  
            y_ra[nd-1]-= PARARA[5]*sin(ha);
            
//          non-perpendicularity between R.A. & Dec. axis            
            if(IADJRA[6]==1){cond_ra[nd-1][iii] = 1.0/cos(dec);iii++;}
            y_ra[nd-1]-= PARARA[6]  /cos(dec);
            
//          non-perpendicularity between optical and mechnical axis 
            if(IADJRA[7]==1){cond_ra[nd-1][iii] = sin(dec); iii++;}
            y_ra[nd-1]-= PARARA[7]*sin(dec);

//          cond_ra[nd-1][8])= sin(ha)*cos(SITE_LATI.rad);   // tube flexure, degenerate

            if(IADJRA[8]==1){cond_ra[nd-1][iii] = cos(ha*3.0); iii++;}
            y_ra[nd-1]-= PARARA[8]*cos(ha*3.0);
            
            if(IADJRA[9]==1){cond_ra[nd-1][iii] = sin(ha*3.0); iii++;}
            y_ra[nd-1]-= PARARA[9]*sin(ha*3.0);
            
            
            np_ra=iii;
                 
            if(encoder_mode==0) y_de[nd-1] = step_dec;
            else				y_de[nd-1] = enco_2;

            iii=0;
            if(IADJDE[0]==1){cond_de[nd-1][iii] = 1.0; iii++;}
            y_de[nd-1]-= PARADE[0];
//          slope
            if(IADJDE[1]==1){cond_de[nd-1][iii] = ap_dec*3600.0; iii++;}  //convert degree to arcsec
            y_de[nd-1]-= PARADE[1]*ap_dec*3600.0;
//          polar axis
            if(IADJDE[2]==1){cond_de[nd-1][iii] = cos(ha); iii++;}
            y_de[nd-1]-= PARADE[2]*cos(ha);
            if(IADJDE[3]==1){cond_de[nd-1][iii] =-sin(ha); iii++;}        
            y_de[nd-1]-= PARADE[3]*(-sin(ha));
//          eccentricity
            if(IADJDE[4]==1){cond_de[nd-1][iii] = cos(dec); iii++;}
            y_de[nd-1]-= PARADE[4]*cos(dec);
            if(IADJDE[5]==1){cond_de[nd-1][iii] = sin(dec); iii++;}
            y_de[nd-1]-= PARADE[5]*sin(dec);
//          tube flexure            
            if(IADJDE[6]==1){cond_de[nd-1][iii] = sin(SITE_LATI.rad)*cos(dec) - cos(SITE_LATI.rad)*sin(dec)*cos(ha); iii++;}
            y_de[nd-1]-= PARADE[6]*sin(SITE_LATI.rad)*cos(dec) - cos(SITE_LATI.rad)*sin(dec)*cos(ha);
            np_dec=iii;
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
    
	iii=0;
    for(i=0;i<10;i++)
        if(IADJRA[i]==1) {PARARA[i]+=para_ra[iii];ERROR1[i]=use_ra*sqrt(q_ra[iii][iii]);iii++;}
        else              ERROR1[i]=0;
        
	iii=0;
    for(i=0;i<10;i++)
        if(IADJDE[i]==1) {PARADE[i]+=para_de[iii];ERROR2[i]=use_de*sqrt(q_de[iii][iii]);iii++;}
        else              ERROR2[i]=0;
        
    for(i=0;i<10;i++) {para_ra[i]=PARARA[i]; para_de[i]=PARADE[i];}
    
    i=0;               ERROR1[i]/=PARARA[1]; ERROR2[i]/=PARADE[1];
    for(i=2;i<10;i++) {ERROR1[i]/=PARARA[1]; ERROR2[i]/=PARADE[1];}

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
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf\n", i,para_ra[i]/para_ra[1],ERROR1[i],para_de[i]/para_de[1],ERROR2[i]);
       
    i=1;
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf\n", i,para_ra[i],ERROR1[i], para_de[i],ERROR2[i]);
       
    for(i=2;i<10;i++)
        fprintf(fout,"%3d %15.3lf/ %15.3lf  %15.3lf/ %15.3lf\n", i,para_ra[i]/para_ra[1],ERROR1[i],para_de[i]/para_de[1],ERROR2[i]);
       
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
 double azi_array[100],alt_array[100];
 
 double error1,error2;
 double enc_to_step_1,enc_to_step_2;
 long   step_ha, step_dec,enco_1,enco_2;
 
 FILE *fin,*fout, *fout_new;
 int ii,i, starno[100];
 int ndmax = 100, npmax = 10;
 int nd;
 char ans, buffer[200];
 
//  Initialize the parameter and Least Squares matrix
    for(i=0;i<10;i++)
       {para_ra[i]=0.0; para_de[i]=0.0;
        q_ra[i][i]=0.0; q_de[i][i]=0.0;
       }
       
//---------------------  Calculate enc_to_step coefficients -----------------
    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
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
    
    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
    fgets(buffer,200,fin);
    nd = 0;
    while( fscanf(fin,"%d %lf %lf %lf %lf %ld %ld %ld %ld %lf %lf\n",
           &ii, &ap_ha, &ap_dec, &alt, &azi, &step_ha, &step_dec, &enco_1,&enco_2,&error_ha, &error_dec)
              != EOF)
         {
          fprintf(fout_new,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
                  ii, ap_ha, ap_dec, alt, azi, step_ha, step_dec, enco_1,enco_2,error_ha, error_dec);
          if(ii>=0)
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
      
    clsq(*cond_ra,y_ra,*q_ra,atyra, ndmax,nd,npmax,np_ra ,para_ra,res_ra, &use_ra);
    clsq(*cond_de,y_de,*q_de,atyde, ndmax,nd,npmax,np_dec,para_de,res_de, &use_de);

//------------------- Print the mount model ---------------------------------
    if((fin = fopen("skyscan.out","r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}
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
int read_site()
//----------------------------------------------------------------------------
{
 FILE *fin;
 char ssss[300];
 
//  read site parameter
    if((fin = fopen("site.inp","r"))==NULL )
      {MessagePopup(""," CANNOT OPEN SITE.INP"); exit(0);}

    fgets(ssss,100,fin);
    sscanf(ssss,"%d %d %lf", &SITE_LONGI.d, &SITE_LONGI.m, &SITE_LONGI.s);
    fgets(ssss,100,fin);
    sscanf(ssss,"%d %d %lf", &SITE_LATI.d,  &SITE_LATI.m,  &SITE_LATI.s);
    fgets(ssss,100,fin);
    sscanf(ssss,"%lf",  &SITE_HEIGHT);
    fclose(fin);
    dms_to_rad(&SITE_LONGI); dms_to_rad(&SITE_LATI);
    
    return 0;
}
//----------------------------------------------------------------------------
int read_para0()
//----------------------------------------------------------------------------
{
 FILE *fin;
 char ssss[300];
 int ii,i;
 
//  read site parameter
    if(file_select("*.inp", ssss)!=1) return -1;
    if((fin = fopen(ssss,"r"))==NULL )
      {MessagePopup(""," CANNOT OPEN SITE.INP"); exit(0);}

    for(i=0;i<10;i++)
       {
        fgets(ssss,100,fin);
        sscanf(ssss,"%d %lf %lf", &ii,&PARARA[i],&PARADE[i]);
       }
    fclose(fin);
    
    return 0;
}
//----------------------------------------------------------------------------
/*
//               RA                 Dec
//zero point     1                  1
//slope          ha                 dec
//polar S/N      tan(dec)*sin(ha)   cos(ha)
//polar E/W      tan(dec)*cos(ha)  -sin(ha)
//NP RA/DEC axis tan(dec)
//collimation    sec(dec)

          if(ii>0)
            {
             nd++;
             starno[nd-1] = ii;
             ha_array [nd-1] = ap_ha/15.0;
             dec_array[nd-1] = ap_dec    ;
             
             ha  = deg_to_rad(ap_ha);
             dec = deg_to_rad(ap_dec);
                 
                          
             cond[nd-1][0] = 1.0;
             cond[nd-1][1] = 0.0;
             cond[nd-1][2] = tan(altra)*sin(azira)
             cond[nd-1][3] = tan(altra)*cos(azira)
             cond[nd-1][4] = tan(altra)
             cond[nd-1][5] = sec(altra)
             yy[nd-1]=dazi;
             
             
             cond[nd-1][0] = 0.0;
             cond[nd-1][1] = 1.0;
             cond[nd-1][2] = cos(azira)
             cond[nd-1][3] =-sin(azira)
             cond[nd-1][4] = 0.0
             cond[nd-1][5] = 0.0
             yy[nd]=dalt;
            }
             

*/        
//----------------------------------------------------------------------------
// function to read pointing parameters
int read_pointing_para(char file_name[], double para[])
//----------------------------------------------------------------------------
{

 FILE *fin;
 char ssss[300];
 int i;
 
//  open the file to contial pointing model
//  The noumber of pointing parameters is confined to six right now.
//  It may be expaded later.

    if((fin = fopen(ssss,"r"))==NULL) return -1; // file open error
    for(i=0;i<6;i++)
       {
        fgets(ssss,100,fin);
        sscanf(ssss,"%lf",&para[i]);
       }
    fclose(fin);
    
    return 0;
}
//----------------------------------------------------------------------------
// function to calculate delta altitude and azimuth for given altitude, azimuth, and point model parameters
int calc_dalt_dazi(double para[], double altdeg, double azideg, double *daltdeg, double *dazideg)
//----------------------------------------------------------------------------
{
// input 
// para[]: array to contain poijt model parameters
// altdeg: altitude, uint in degree
// azideg: azimuth , uint in degree

// output
// daltdeg: delta altitude in degree
// dazideg: delta azimuth  in degree
//

double altrad,azirad,pi314;

//  convert degree to radian    
    pi314  = acos(-1.0);
    altrad = altdeg*pi314/180.0;
    azirad = (azideg-180.0)*pi314/180.0;
    
//  calculate delta azimuth in degree    
    *dazideg = para[0]*
              +para[2]*tan(altrad)*sin(azirad)
              +para[3]*tan(altrad)*cos(azirad)
              +para[4]*tan(altrad)
              +para[5]/cos(altrad);
         
             
//  calculate delta altitude in degree    
    *dazideg = para[1]
              +para[2]*cos(azirad)
              -para[3]*sin(azirad)
              +para[4];
return 0;
}
//----------------------------------------------------------------------------
int cal_mount_model_new()
//----------------------------------------------------------------------------
{
 FILE *fin,*fout;
 int ii,i;
 char ans,buffer[400],fname[300];
 
 double alt,azi,dalt,dazi,altra,azira;

 int nd,npoint;
 int ndmax=100, npmax=10, npara=5;
 double *yy,*res,*aty,use;
 double *cond,*qmat,*para;
 double *azi_array,*alt_array,*azi_res,*alt_res;
// double cond[100][10],qmat[10][10],para[10];
// double azi_array[100],alt_array[100],azi_res[100],alt_res[100];
 
 double ap_ha,ap_dec;
 long step_ha, step_dec, enco_1, enco_2;
 double use1,use2;
 
       
//---------------------  Calculate enc_to_step coefficients ------------------
    if(file_select("*.inp", fname)!=1) return -1;
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}

    fgets(buffer,200,fin);
    sscanf(buffer,"%d",&npoint);
    
    res = malloc(8*npoint*2);
    yy  = malloc(8*npoint*2);
    
    aty = malloc(8*npmax );
    para= malloc(8*npmax );
    cond= malloc(8*npmax*npoint*2);
    qmat= malloc(8*npmax*npmax );
    
    azi_array = malloc(8*npoint);
    alt_array = malloc(8*npoint);
    azi_res   = malloc(8*npoint);
    alt_res   = malloc(8*npoint);

    npara=6;
    nd = 0;
    for(i=0;i<npoint;i++)
       {
        fgets(buffer,200,fin);
        sscanf(buffer,"%d %lf %lf %lf %lf %ld %ld %ld %ld",
        &ii, &ap_ha, &ap_dec, &alt, &azi, &step_ha, &step_dec,&enco_1,&enco_2);
        
        azi_array[nd/2]=azi;
        alt_array[nd/2]=alt;
         
        azira = deg_to_rad(azi);
        altra = deg_to_rad(alt);
/*        
        cond[nd][0] = 1.0;       // azi zeropoint
        cond[nd][1] = 0.0;		 // alt zeropoint
        cond[nd][2] = enco_1;    // azi scale
        cond[nd][3] = 0;		 // alt scale
        cond[nd][4] = tan(altra)*sin(azira);
        cond[nd][5] = tan(altra)*cos(azira);
        cond[nd][6] = tan(altra);
        cond[nd][7] = 1.0/cos(altra);
        yy  [nd]=azi*3600;
             
        cond[nd+1][0] = 0.0;
        cond[nd+1][1] = 1.0;
        cond[nd+1][2] = 0;
        cond[nd+1][3] = enco_2;
        cond[nd+1][4] = cos(azira);
        cond[nd+1][5] =-sin(azira);  //polar axis
        cond[nd+1][6] = 0.0;
        cond[nd+1][7] = 0.0;
*/        
        cond[nd*npara  ] = 1.0;       // azi zeropoint
        cond[nd*npara+1] = 0.0;		 // alt zeropoint
        cond[nd*npara+2] = enco_1;    // azi scale
        cond[nd*npara+3] = 0;		 // alt scale
        cond[nd*npara+4] = tan(altra)*sin(azira);
        cond[nd*npara+5] = tan(altra)*cos(azira);
//      cond[nd*npara+6] = tan(altra);
//      cond[nd*npara+7] = 1.0/cos(altra);
        yy  [nd]=azi*3600;
             
        cond[(nd+1)*npara  ] = 0.0;
        cond[(nd+1)*npara+1] = 1.0;
        cond[(nd+1)*npara+2] = 0;
        cond[(nd+1)*npara+3] = enco_2;
        cond[(nd+1)*npara+4] = cos(azira);
        cond[(nd+1)*npara+5] =-sin(azira);  //polar axis
//      cond[(nd+1)*npara+6] = 0.0;
//      cond[(nd+1)*npara+7] = 0.0;
        
        yy  [nd+1]=alt*3600;
        if(ii>=0) nd+=2;
       }
    fclose(fin);
    
    npoint=nd/2;
    
    clsq(cond,yy,qmat,aty,nd,nd,npara,npara,para,res,&use);
    
    fout = fopen("cal_mount.out","w");
    use1=use2=0;
    for(i=0;i<npoint;i++)
       {
        ii=i*2;
        sprintf(buffer,"%2d %10.3lf %6.1lf  %10.3lf %6.1lf\n",i,yy[ii]/3600.0,res[ii],yy[ii+1]/3600.0,res[ii+1]);
        use1+=res[ii  ]*res[ii  ];
        use2+=res[ii+1]*res[ii+1];
        
        display_message(buffer);
        fprintf(fout,"%s",buffer);
        
        azi_res[i]=res[ii  ];
        alt_res[i]=res[ii+1];
       }
    use1= sqrt(use1/npoint);   
    use2= sqrt(use2/npoint);   
    
    sprintf(buffer,"r.m.s residual azi:%10.3lf alt:%10.3lf\n",use1,use2);
    display_message(buffer);
    fprintf(fout,"%s",buffer);
       
    sprintf(buffer,"\n -----  Result of Mount Model calaulation -----\n");
    display_message(buffer); fprintf(fout,"%s",buffer);
    
//    for(i=0;i<npara;i++)
//       {
//        sprintf(buffer,"%2d %10.3lf +/- %10.3lf\n",i+1,para[i],use*sqrt(qmat[i][i]));
//        display_message(buffer);
//        fprintf(fout,"%s",buffer);
//       }

       i=0;
       sprintf(buffer,"%2d %10.3lf +/- %10.3lf  azimuth zero point\n",i+1,para[i],use*sqrt(qmat[i*npara+i]));
       display_message(buffer); fprintf(fout,"%s",buffer);
          
       i=1;
       sprintf(buffer,"%2d %10.3lf +/- %10.3lf  altitutde zero point\n",i+1,para[i],use*sqrt(qmat[i*npara+i]));
       display_message(buffer); fprintf(fout,"%s",buffer);
          
       i=2;
       sprintf(buffer,"%2d %10.3lf +/- %10.3lf  azimuth scale  (arcsec/counts)\n",i+1,para[i],use*sqrt(qmat[i*npara+i]));
       display_message(buffer); fprintf(fout,"%s",buffer);
          
       i=3;
       sprintf(buffer,"%2d %10.3lf +/- %10.3lf  altitude scale (arcsec/counts)\n",i+1,para[i],use*sqrt(qmat[i*npara+i]));
       display_message(buffer); fprintf(fout,"%s",buffer);
          
       i=4;
       sprintf(buffer,"%2d %10.3lf +/- %10.3lf  vertical axis error E_W (arcsec)\n",i+1,para[i],use*sqrt(qmat[i*npara+i]));
       display_message(buffer); fprintf(fout,"%s",buffer);
          
       i=5;
       sprintf(buffer,"%2d %10.3lf +/- %10.3lf  vertical axix error N_S (arcsec)\n",i+1,para[i],use*sqrt(qmat[i*npara+i]));
       display_message(buffer); fprintf(fout,"%s",buffer);
       
//     AZI = a_0 + a_1 * counts + c_1 * tan(altra)*sin(azira)+ c_2*tan(altra)*cos(azira);
//     ALT = b_0 + b_1 * counts + c_1 * cos(azira)           + c_2*(-sin(azira))
//     a_0 = para[0]
//     a_1 = para[2]
//     c_1 = para[4]
//     c_2 = para[5]

//     b_0 = para[1]
//     b_1 = para[3]
//     c_1 = para[4]
//     c_2 = para[5]

       sprintf(buffer,"--------- New Mount Model ------------\n");
       display_message(buffer); fprintf(fout,"%s",buffer);
       sprintf(buffer,"%2d %12.5lf %12.5lf  zero point       \n",0, -para[0]/para[2],-para[1]/para[3]);
       display_message(buffer); fprintf(fout,"%s",buffer);
       sprintf(buffer,"%2d %12.5lf %12.5lf  scale            \n",1,  1.0    /para[2], 1.0    /para[3]);
       display_message(buffer); fprintf(fout,"%s",buffer);
       sprintf(buffer,"%2d %12.5lf %12.5lf  vertical axis E-W\n",2, -para[4]/para[2],-para[4]/para[3]);
       display_message(buffer); fprintf(fout,"%s",buffer);
       sprintf(buffer,"%2d %12.5lf %12.5lf  vertical axis N-S\n",3, -para[5]/para[2],-para[5]/para[3]);
       display_message(buffer); fprintf(fout,"%s",buffer);
       
       sprintf(buffer," --- Output file is cal_model.out ----\n");
       display_message(buffer); fprintf(fout,"%s",buffer);

       SetCtrlAttribute (cal_handle,CMM_GRAPH_RA ,ATTR_LABEL_TEXT,"AZI residual (deg, arcsec)");
       SetCtrlAttribute (cal_handle,CMM_GRAPH_DEC,ATTR_LABEL_TEXT,"ALT residual (deg, arcsec)");
       auto_plot_xy_scatter(cal_handle,CMM_GRAPH_RA ,azi_array,azi_res, npoint);
       auto_plot_xy_scatter(cal_handle,CMM_GRAPH_DEC,alt_array,alt_res, npoint);
          
    fclose(fout);
    
    free(res);
    free(yy);
    free(aty);
    free(para);
    free(cond);
    free(qmat);
    free(azi_array);
    free(alt_array);
    free(azi_res  );
    free(alt_res  );
    
return 0;
}    
//----------------------------------------------------------------------------
int display_message(char buff[])
//----------------------------------------------------------------------------
{
 return SetCtrlVal(cal_handle,CMM_TEXTBOX,buff);
} 
//----------------------------------------------------------------------------
// Delta A = C0 +C3*tanE + C4*secE + C5*cosA*tanE + c6*sinA*tanE
// Delta E = C1 +C2*cosE +           C5*sinA      + c6*cosA
int cal_mount_model_china()
//----------------------------------------------------------------------------
{
 FILE *fin,*fout;
 int ii,i;
 char ans,buffer[400],fname[300];
 
 double alt,azi,del_alt,del_azi,altra,azira;

 int nd,npoint;
 int ndmax=200, npmax=10, npara=5;
 double yy[200],res[200],aty[10],use;
 double cond[200][10],qmat[10][10],para[10];
 
 double ap_ha,ap_dec;
 long step_ha, step_dec, enco_1, enco_2;
 
       
//---------------------  Calculate enc_to_step coefficients ------------------
    if(file_select("*.dat", fname)!=1) return -1;
    if((fin = fopen(fname,"r"))==NULL ) {MessagePopup(""," CANNOT OPEN SKYSCAN.OUT"); return 0;}

    fgets(buffer,200,fin);
    sscanf(buffer,"%d",&npoint);
    nd = 0;
    for(i=0;i<npoint;i++)
       {
        fgets(buffer,200,fin);
        sscanf(buffer,"%lf %lf %lf %lf",&del_azi,&del_alt,&azi,&alt);
        
//        azi -=180.0;
         
        azira = deg_to_rad(azi);
        altra = deg_to_rad(alt);
        
        cond[nd][0] = 1.0;				      //C0
        cond[nd][1] = 0.0;					  //C1
        cond[nd][2] = tan(altra)*sin(azira);  //C6
        cond[nd][3] = tan(altra)*cos(azira);  //C5
        cond[nd][4] = tan(altra);			  //C3
        cond[nd][5] = 1.0/cos(altra);		  //C4
        cond[nd][6] = 0;					  //C2
//        cond[nd][7] = 0;
        yy  [nd]=del_azi;
             
             						   
        cond[nd+1][0] = 0.0;		   //C0
        cond[nd+1][1] = 1.0;		   //C1
        cond[nd+1][2] = cos(azira);	   //C2
        cond[nd+1][3] =-sin(azira);	   //C3
        cond[nd+1][4] = 0.0;
        cond[nd+1][5] = 0.0;
        cond[nd+1][6] = cos(altra);  // C2
//        cond[nd+1][6] = 0;
//        cond[nd+1][7] = enco_2;
        yy  [nd+1]=del_alt;
        
        if(ii>0) nd+=2;
       }
    fclose(fin);
    npoint=nd/2;
    
    npara=7;
    clsq(*cond,yy,*qmat,aty,ndmax,nd,npmax,npara,para,res, &use);
    
    fout = fopen("cal_mount.out","w");
    for(i=0;i<npoint;i++)
       {
        ii=i*2;
        sprintf(buffer,"%2d %10.3lf %6.1lf  %10.3lf %6.1lf\n",i,yy[ii],res[ii],yy[ii+1],res[ii+1]);
        display_message(buffer);
        fprintf(fout,"%s",buffer);
       }
       
    sprintf(buffer,"\n -----  parameter  -----\n");
    display_message(buffer);
    for(i=0;i<npara;i++)
       {
        sprintf(buffer,"%2d %10.3lf +/- %10.3lf\n",i+1,para[i],use*sqrt(qmat[i][i]));
        display_message(buffer);
        fprintf(fout,"%s",buffer);
       }

    fclose(fout);
    
return 0;
}    
//----------------------------------------------------------------------------

