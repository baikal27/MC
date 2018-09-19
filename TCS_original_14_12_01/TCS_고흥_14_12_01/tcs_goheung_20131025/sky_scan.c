#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "scan_sub.h"
#include "sky_scan.h"
#include "cvi_util.h"  
#include "tel_sub.h"
#include "cal_sub.h"
#include "gv_tel.h"

extern DWORD read_counter(char caxis,long *counts);
extern DWORD read_encoder(char caxis,long *counts);
extern int do_two_star_align(double *delta_dec, double zz[]);
extern void display_message(char buff[]);

//extern int load_guide_panel(h_m_s ra_2000, d_m_s dec_2000);
//extern int adjust_offset(h_m_s ra_2000, d_m_s dec_2000);
//extern int stop_guide(void);
//-----------------------------------------------------------------------------
extern int SLEW_AND_TRACK(h_m_s ra_2000, d_m_s dec_2000);
extern int STOP_TELESCOPE(void);
extern int display_input_window(h_m_s ra_2000, d_m_s dec_2000, char star_name[]);
//-----------------------------------------------------------------------------
static h_m_s RA_2000; static d_m_s DEC_2000;
static int scan_handle, cal_handle,graph_handle;
static char UIR_FILE_NAME[20] = "sky_scan.uir";
static int IS_FOUT3_OPEN,NO_OF_SCAN_STAR,STAR_NO,STAR_INDEX;
static double jd_ut1;
static FILE *fout3;
static int OFFSET_ON_OFF;

double MAG_MAX, MAG_MIN;

int load_scan_panel(int panel_handle);
int write_sky_scan_out(int star_no);
int show_scan_lists(int handle,int control);
int cal_mount_model_altazi_new(void);
//-----------------------------------------------------------------------------


static int SCAN_PANEL = 0, GRAPH_PANEL = 0;
//--------------------------------------------------------------------------------
int load_scan_panel(int panel_handle)
//--------------------------------------------------------------------------------
{
 char buff[256];
    if(SCAN_PANEL != 0) return -1;
    
    scan_handle = LoadPanel(0,UIR_FILE_NAME,SCAN);
	DisplayPanel(scan_handle);
    sprintf(buff,"   No.  Name     R.A.     H.A.   Dec.   Alt.    Azi.   mag sp           ");
    SetCtrlVal(scan_handle,SCAN_TEXTMSG,buff);
    set_control_dim(scan_handle,SCAN_TSA);
	
    SCAN_PANEL = 1;
	
    IS_FOUT3_OPEN = 0;
    OFFSET_ON_OFF = 1;
	return scan_handle;
}
/*---------------------------------------------------------------------------*/
int write_sky_scan_out(int star_no)
/*---------------------------------------------------------------------------*/
{
 double   error_ha, error_dec;
 long int step_ha, step_dec;
 long     encoder_1,encoder_2;
 double   must_count_ha,must_count_dec;
 double   ap_ha,last,ap_dec,alt,azi;
 char buff[200];
 int i_value = 0;

    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
        
//  Calculate error count and print it into the file
    get_aposi(RA_2000.rad,DEC_2000.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
              &last,&ap_ha,&ap_dec,&alt,&azi);
              
    must_count_ha = calc_ha_count (ap_ha,ap_dec,azi,alt);
    must_count_dec= calc_dec_count(ap_ha,ap_dec,azi,alt);
      
    read_counter('A',&step_ha)  ; read_counter('B',&step_dec);
    read_encoder('A',&encoder_1); read_encoder('B',&encoder_2);

    if(ENCODER_MODE==0)
      {
       error_ha  = must_count_ha  - step_ha + 0.5;
       error_dec = must_count_dec - step_dec+ 0.5;
      }
    else
      {
       error_ha  = must_count_ha  - encoder_1+ 0.5;
       error_dec = must_count_dec - encoder_2+ 0.5;
      }

    ap_ha  = rad_to_deg(ap_ha) ;
    ap_dec = rad_to_deg(ap_dec);
    alt    = rad_to_deg(alt);
    azi    = rad_to_deg(azi);

    fprintf(fout3,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
            star_no, ap_ha, ap_dec, alt, azi, step_ha, step_dec,encoder_1,encoder_2,
            error_ha/MOUNT_HA[1], error_dec/MOUNT_DEC[1]);

    sprintf(buff,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
            star_no, ap_ha, ap_dec, alt, azi, step_ha, step_dec,encoder_1,encoder_2,
            error_ha/MOUNT_HA[1], error_dec/MOUNT_DEC[1]);

//  InsertListItem (scan_handle,SCAN_STAR_LIST_2, -1, buff);
    i_value++;
	InsertListItem (scan_handle,SCAN_STAR_LIST_2, -1, buff,i_value);
    
    return 0;
}
//----------------------------------------------------------------------------
int write_sky_scan_out_new(int star_no)
/*---------------------------------------------------------------------------*/
{
 double   error_ha, error_dec;
 long int step_ha, step_dec;
 long     encoder_1,encoder_2;
 double   must_count_ha,must_count_dec;
 double   ap_ha,last,ap_dec,alt,azi;
 char buff[200];
 int i_value = 0;
 FILE *fout;
 
    fout = fopen("Save_Pointing.dat","a");

    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
        
//  Calculate error count and print it into the file
    get_aposi(TARGET_RA_2000.rad,TARGET_DEC_2000.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,
              &last,&ap_ha,&ap_dec,&alt,&azi);
              
    must_count_ha = calc_ha_count (ap_ha,ap_dec,azi,alt);
    must_count_dec= calc_dec_count(ap_ha,ap_dec,azi,alt);
      
    read_counter('A',&step_ha)  ; read_counter('B',&step_dec);
    read_encoder('A',&encoder_1); read_encoder('B',&encoder_2);

    if(ENCODER_MODE==0)
      {
       error_ha  = must_count_ha  - step_ha + 0.5;
       error_dec = must_count_dec - step_dec+ 0.5;
      }
    else
      {
       error_ha  = must_count_ha  - encoder_1+ 0.5;
       error_dec = must_count_dec - encoder_2+ 0.5;
      }
	
//    OFFSET_HA_COUNT  = step_ha  - calc_ha_count (ap_ha.rad,ap_dec.rad,azi.rad,alt.rad);
//    OFFSET_DEC_COUNT = step_dec - calc_dec_count(ap_ha.rad,ap_dec.rad,azi.rad,alt.rad);

      OFFSET_HA_COUNT  = -error_ha ;
      OFFSET_DEC_COUNT = -error_dec;
	
    ap_ha  = rad_to_deg(ap_ha) ;
    ap_dec = rad_to_deg(ap_dec);
    alt    = rad_to_deg(alt);
    azi    = rad_to_deg(azi);

    fprintf(fout,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
            star_no, ap_ha, ap_dec, alt, azi, step_ha, step_dec,encoder_1,encoder_2,
            error_ha/MOUNT_HA[1], error_dec/MOUNT_DEC[1]);

    sprintf(buff,"%3d %12.5lf %12.5lf %12.5lf %12.5lf %10ld %10ld %10ld %10ld %8.1lf %8.1lf\n",
            star_no, ap_ha, ap_dec, alt, azi, step_ha, step_dec,encoder_1,encoder_2,
            error_ha/MOUNT_HA[1], error_dec/MOUNT_DEC[1]);
	
	display_message(buff);

    fclose(fout);
    
    return 0;
}
//----------------------------------------------------------------------------
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
                   cal_mount_model_altazi_new();
/*			    
			   GetCtrlVal(cal_handle,CMM_RA ,&npara_ra );
			    GetCtrlVal(cal_handle,CMM_DEC,&npara_dec);
			    
                if(TELESCOPE_TYPE==_RA_DEC)
                   cal_mount_model_radec (npara_ra,npara_dec,ENCODER_MODE);
                else
                   cal_mount_model_altazi(npara_ra,npara_dec,ENCODER_MODE);
                
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
                
                plot_1 = auto_plot_xy_scatter(graph_handle,CAL_2_GRAPH_RA ,ha_array ,res_ra, ndata);
                plot_2 = auto_plot_xy_scatter(graph_handle,CAL_2_GRAPH_DEC,dec_array,res_de, ndata);
*/				
               }
		
	     else if(control==CMM_CLOSE)
	       {
	        if(ConfirmPopup("","새마운트 모델을 사용할까요?")==1) read_mount_model("newmount.inp");
	        DiscardPanel(cal_handle);
	        DiscardPanel(graph_handle);
            GRAPH_PANEL = 0;  // two panels to calculate the mount model are closed
	        
//          open skyscan.out file for appending                
            if((fout3 = fopen("skyscan.out","a"))==NULL )
              {puts(" CANNOT OPEN skyscan.out \n"); return -1;}
            else
              IS_FOUT3_OPEN = 1;
           }

			break;
		}
	return 0;
}
//--------------------------------------------------------------------------------
int CVICALLBACK CB_SCAN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//--------------------------------------------------------------------------------
{
 double lmag, dec_max, dec_min;
 int val, state;
 char buff[200], name[20];
 double delta_dec,ZP[3];
 
	switch (event) {
		case EVENT_COMMIT:
            
		     if(control==SCAN_SELECT)
		       {
                ClearListCtrl (panel,SCAN_STAR_LIST);
   		        GetCtrlVal(panel,SCAN_MAGMAX,&MAG_MAX);
   		        GetCtrlVal(panel,SCAN_MAGMIN,&MAG_MIN);
   		        
   		        if(MAG_MIN>MAG_MAX) {lmag=MAG_MIN; MAG_MIN=MAG_MAX; MAG_MAX=lmag;}
   		        else                {lmag=MAG_MAX;}
                GetCtrlVal(panel,SCAN_RING,&val );
                SetWaitCursor(1);
		         if(val==0)        // all sky
		           {make_scan_file (lmag,2);}
		         else if(val==1)   // equator
		           {
                    GetCtrlVal(panel,SCAN_DECL_MAX,&dec_max);
                    GetCtrlVal(panel,SCAN_DECL_MIN,&dec_min);
      		        if(dec_min>dec_max) {lmag=dec_min; dec_min=dec_max; dec_max=lmag;}
                    find_equator_star(lmag,dec_min,dec_max);
                   }
		           
		        show_scan_lists(scan_handle,SCAN_STAR_LIST);

                if( (fout3 = fopen("skyscan.out","w"))==NULL )
                  {puts(" CANNOT OPEN SKYSCAN.OUT\n"); exit(0);}
                else IS_FOUT3_OPEN = 1;
                fprintf(fout3,"SCAN START: at %s   %s\n",DateStr(),TimeStr());
                  
                NO_OF_SCAN_STAR = 0;
                SetWaitCursor(0);
               }
               
		     else if(control==SCAN_MOVE)
		       {
                GetCtrlVal        (panel, SCAN_STAR_LIST,&STAR_INDEX);
//              if(STAR_INDEX==0) break;
                GetLabelFromIndex (panel, SCAN_STAR_LIST,STAR_INDEX,buff);
                sscanf (buff,"%d",&STAR_NO);
                
//              set coordinate in the input window of main panel             
                strncpy(TARGET_NAME, buff,10);
                TARGET_NAME[10] = NULL;
                display_input_window(RA_2000,DEC_2000,name);

//              slew and track the selected star             
                get_star_coordiantes(STAR_NO,&RA_2000.rad,&DEC_2000.rad);
                rad_to_hms(&RA_2000); rad_to_dms(&DEC_2000);
                
//                if(GUIDE_MODE != GUIDE_MODE_OFF) stop_guide();
//                stop_guide();
                SLEW_AND_TRACK(RA_2000, DEC_2000);
               }
               
//		     else if(control==SCAN_STOP)
//		       {
//		        if(ConfirmPopup("","마운트 모델에 이용할까요?  ")==1)
//		          {
//                   NO_OF_SCAN_STAR++;
//                   write_sky_scan_out(STAR_NO);
//                   if(OFFSET_ON_OFF==1) adjust_offset(RA_2000, DEC_2000);
//                  }
//                  
//                STOP_TELESCOPE();
//               }
               
		     else if(control==SCAN_ACCEPT)
		       {
                NO_OF_SCAN_STAR++;
                SetCtrlVal(panel,SCAN_NNSTAR,NO_OF_SCAN_STAR);
                write_sky_scan_out(STAR_NO);
//                if(OFFSET_ON_OFF==1) adjust_offset(RA_2000, DEC_2000);
//                stop_guide();
//              STOP_TELESCOPE();
               }
               
		     else if(control==SCAN_STOPT) STOP_TELESCOPE();
               
		     else if(control==SCAN_MODEL)
		       {
		        if(GRAPH_PANEL!=0) break;
		        
		        if(IS_FOUT3_OPEN != 0) {fclose(fout3); IS_FOUT3_OPEN = 0;}
                
                graph_handle = LoadPanel(0,"sky_scan.uir",CAL_2);
                DisplayPanel(graph_handle);
                
                cal_handle = LoadPanel(0,UIR_FILE_NAME,CMM);
                DisplayPanel(cal_handle);
                GRAPH_PANEL = 1;
               }
               
		     else if(control==SCAN_CLOSE)
		       {
                copy_panel_to_clipboard(panel);
			    DiscardPanel(panel);
                SCAN_PANEL = 0;
			    
                if(IS_FOUT3_OPEN == 1) fclose(fout3);
               }
               
		     else if(control==SCAN_HP)
		       {
		        GUIDE_MODE = GUIDE_MODE_HP;
//	            load_guide_panel(RA_2000,DEC_2000);
	           }
	           
		     else if(control==SCAN_OFFSET)
		        GetCtrlVal(panel,control,&OFFSET_ON_OFF);
		        
		     else if(control==SCAN_RING)
		       {
		        GetCtrlVal(panel,control,&state);
		        if(state==0)
		          {SetCtrlVal(panel,SCAN_DECL_MAX, 90.0);
                   SetCtrlVal(panel,SCAN_DECL_MIN,-37.0);
                  }
                else
		          {SetCtrlVal(panel,SCAN_DECL_MAX, 10.0);
                   SetCtrlVal(panel,SCAN_DECL_MIN,-10.0);
                  }
               }

/*		        
		     else if(control==SCAN_TSA)
		       {
                fclose(fout3);
                IS_FOUT3_OPEN == 0;
                
                do_two_star_align(&delta_dec,ZP);
                
//              open skyscan.out file for appending                
                if((fout3 = fopen("skyscan.out","a"))==NULL )
                  {puts(" CANNOT OPEN skyscan.out \n"); return -1;}
                else
                  IS_FOUT3_OPEN = 1;
               }
*/               
		        
               
			break;
	}
	return 0;
}
//--------------------------------------------------------------------------------
int cal_mount_model_altazi_new()
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

       SetCtrlAttribute (cal_handle,CAL_2_GRAPH_RA ,ATTR_LABEL_TEXT,"AZI residual (deg, arcsec)");
       SetCtrlAttribute (cal_handle,CAL_2_GRAPH_DEC,ATTR_LABEL_TEXT,"ALT residual (deg, arcsec)");
       auto_plot_xy_scatter(cal_handle,CAL_2_GRAPH_RA ,azi_array,azi_res, npoint);
       auto_plot_xy_scatter(cal_handle,CAL_2_GRAPH_DEC,alt_array,alt_res, npoint);
          
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
int displaymessage(char buff[])
//----------------------------------------------------------------------------
{
 return SetCtrlVal(cal_handle,CMM_TEXTBOX,buff);
} 
//----------------------------------------------------------------------------
