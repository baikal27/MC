#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "type_def.h"
#include "catalog.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "gv_tel.h"
#include "cvi_util.h"

//#include "gv_tel.h"
extern d_m_s   SITE_LONGI,SITE_LATI;
extern double  SITE_HEIGHT;
extern double  DUT1_SEC,DUT1_DAY;

//-----------------------------------------------------------------------------
extern int SLEW_AND_TRACK(h_m_s RA_2000, d_m_s DEC_2000);
extern int STOP_TELESCOPE(void);
//-----------------------------------------------------------------------------
static int  cat_handle;
static char buff[200], star_name[15];
static char UIR_FILE_NAME[20]="catalog.uir";
static int  IS_FOUT3_OPEN,NO_OF_SCAN_STAR,STAR_NO,STAR_INDEX;
static int year,month,day,hour,minute,second,hsecond;
static double rhour,jd_ut1;
static int starno;
static double last,ap_ha,ap_dec,alt,azi,vmag;
static h_m_s ra;
static d_m_s dec;
static FILE *fuser;
static char cname[4],gname[4],stype[20];
static int NO_OF_CAT_STARS;
static int CATALOG_MODE;
//--------------------------------------------------------------------------------
int load_cat_panel(int panel_handle);
static int display_planet(void);
static int display_messier(void);
static int display_usercat(void);
//--------------------------------------------------------------------------------

static int CAT_PANEL = 0;
//--------------------------------------------------------------------------------
int load_cat_panel(int panel_handle)
//--------------------------------------------------------------------------------
{
    if(CAT_PANEL!=0) return -1;
    
    CATALOG_MODE = 1;
    cat_handle = LoadPanel(panel_handle,UIR_FILE_NAME,CAT);
    
    set_control_dim(cat_handle,CAT_GLOBULAR);
    set_control_dim(cat_handle,CAT_NEBULAR );
    set_control_dim(cat_handle,CAT_GALAXY  );
    
    DisplayPanel(cat_handle);

    CAT_PANEL = 1;
    
    return cat_handle;
}
//--------------------------------------------------------------------------------
int CVICALLBACK SELECT_CAT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//--------------------------------------------------------------------------------
{
 int nstar, ii, value;
 char buff[150];
 int nbody;

	switch (event) {
		case EVENT_COMMIT:
		
        jd_ut1 = get_jdutc_clock() + DUT1_DAY;

        if(control==CAT_USER) display_usercat();

/*           {
            SetCtrlVal(cat_handle,CAT_TEXTMSG,"사용자가 정한 별 목록");
			ClearListCtrl (cat_handle,CAT_LISTBOX);
            ii = 0;
            sprintf(buff," No.  Name     R.A.      H.A.   Dec.   Alt.    Az.   mag  sp");
            InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,0);
			if( (fuser = fopen("user_catalog.dat","r"))==NULL )
              {MessagePopup(""," CANNOT OPEN USER_CATALOG.DAT");
               return -1;
              }

            while(fscanf(fuser, "%d %s %s %lf %lf %lf %s \n",&starno,gname,cname,&ra.rad, &dec.rad,&vmag,stype) != EOF)
                 {
                  get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,&last,&ap_ha,&ap_dec,&alt,&azi);
                  rad_to_hms(&ra);rad_to_dms(&dec);
                  alt    = rad_to_deg(alt);
                  azi    = rad_to_deg(azi);
                  ap_ha  = rad_to_deg(ap_ha) /15.0;
                  ap_dec = rad_to_deg(ap_dec);
                  ii++;
                  sprintf(buff,"%2d %3s %3s %2d %2d %4.1lf %6.2lf %6.2lf %6.2lf %7.2lf %5.1lf %s",
                          ii,gname,cname,ra.h,ra.m,ra.s,ap_ha,ap_dec,alt,azi,vmag,stype);
                  value = ii;
                  InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,value);                      
                 }
            fclose(fuser);
            NO_OF_CAT_STARS = ii;
            return ii;
           }
*/           
        else if(control==CAT_PLANET)
          {display_planet();
           SetCtrlVal(cat_handle,CAT_TEXTMSG,"지금 볼 수 있는 태양계 천체 ");
          }
          
        else if(control==CAT_MESSIER)
           display_messier();
        else if(control==CAT_GLOBULAR)
           SetCtrlVal(cat_handle,CAT_TEXTMSG,"지금 볼 수 있는 구상성단");
        else if(control==CAT_NEBULAR)
           SetCtrlVal(cat_handle,CAT_TEXTMSG,"지금 볼 수 있는 성운들");
        else if(control==CAT_GALAXY)
           SetCtrlVal(cat_handle,CAT_TEXTMSG,"지금 볼 수 있는 외부은하");           
            
		break;
	}
	return 0;
}
//--------------------------------------------------------------------------------
int CVICALLBACK CAT_APPLY(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//--------------------------------------------------------------------------------
{
 char sign;
 
    switch (event) {
      case EVENT_COMMIT:
	  
      if(control==CAT_MOVE)
        {
         GetCtrlVal(panel,CAT_LISTBOX,&STAR_INDEX);
//       if(STAR_INDEX==0) break;
         GetLabelFromIndex (panel,CAT_LISTBOX,STAR_INDEX,buff);
         sscanf (buff,"%d %s %d %d %lf %c%d %d %lf", &STAR_NO,star_name,&ra.h,&ra.m,&ra.s, &sign,&dec.d,&dec.m,&dec.s);
         hms_to_rad(&ra);
         dms_to_rad(&dec);
         if(sign=='-') {dec.rad = -dec.rad; rad_to_dms(&dec);}
                 
//       set coordinate in the input window of main panel
         strcpy(TARGET_NAME,star_name);
//       display_input_window(ra,dec,star_name);
         
		 
//       slew and track the selected star             
         SLEW_AND_TRACK(ra,dec);
		 
		 
		 //sleep(600);
         //SLEW_AND_TRACK(ra,dec);//by limit, tel. stop and re-track by Junghwan Han
		 
		 
		 
        }

	  
      else if(control==CAT_STOP)
	  {
		  STOP_TELESCOPE();
          Beep(); 
           if(ConfirmPopup("limit을 넘었습니다","다시 시작 할까요 ?")==1)
             {
              break;
              return -1;
             }
          }//alt범위 10-85 by Junghwan
		  
		  
	  }
			   
      else if(control==CAT_QUIT)
        {CATALOG_MODE = 0;
         copy_panel_to_clipboard(panel);		
         DiscardPanel(cat_handle);
         CAT_PANEL = 0;
        }
			   
      break;
     }

    return 0;
}
//--------------------------------------------------------------------------------
static int display_planet(void)
//--------------------------------------------------------------------------------
{           
 int ii,nbody;
 double distance,rpos[3];
 char sign;

 char planet_name[13][10]
  ={//"Mercury  ",// 0
    "수성     ",// 0
    "금성     ",// 1
    "지구     ",// 2
    "화성     ",// 3
    "목성     ",// 4
    "토성     ",// 5
    "천왕성   ",// 6
    "해왕성   ",// 7
    "명왕성   ",// 8
    "달       ",// 9
    "태양     ",//10
    "11       ",//11
    "12       " //12
   };
    
/* 
 char planet_name[13][10];
    sprintf(planet_name[ 1],"Mercury  ");
    sprintf(planet_name[ 2],"Venus    ");
    sprintf(planet_name[ 4],"Mars     ");
    sprintf(planet_name[ 5],"Jupiter  ");
    sprintf(planet_name[ 6],"Saturn   ");
    sprintf(planet_name[ 7],"Uranus   ");
    sprintf(planet_name[ 8],"Naptune  ");
    sprintf(planet_name[ 9],"Pluto    ");
    sprintf(planet_name[10],"Moon     ");
    sprintf(planet_name[11],"Sun      ");
*/    

    sprintf(buff,"  No.  Name         R.A. (2000)    Dec.       H.A.     Alt     Azi    ");
    SetCtrlVal(cat_handle,CAT_HEAD,buff);    

    SetCtrlVal(cat_handle,CAT_TEXTMSG,"행성과 달은 지금 어디에...");
    ClearListCtrl (cat_handle,CAT_LISTBOX);
    ii = 0;
//    sprintf(buff,"     Name         R.A. (2000)    Dec.       H.A.     Alt     Azi");
//    InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,ii);                      
    
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    
    for(nbody=1;nbody<12;nbody++)
       {
        if(nbody!=3)
          {
           gcsbpos(jd_ut1,nbody,0,0,0,rpos);           
           ra.rad  =rpos[0];
           dec.rad =rpos[1];
           distance=rpos[2];

//         convert to topocentric position           
           gctotc(&ra.rad,&dec.rad,&distance,last,SITE_LATI.rad,SITE_HEIGHT);
           get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,&last,&ap_ha,&ap_dec,&alt,&azi);
           
           if(dec.rad<0.0)
             {sign = '-'; dec.rad = -dec.rad;}
           else
              sign = '+';           
              
           rad_to_hms(&ra);rad_to_dms(&dec);
              
           alt = rad_to_deg(alt);
           azi = rad_to_deg(azi);
           ap_ha  = rad_to_deg(ap_ha) /15.0;
           ap_dec = rad_to_deg(ap_dec);
           if(alt>0.0)
             {
              sprintf(buff,"%2d %10s %2d %2d %4.1lf    %c%2d%3d%5.1lf %8.2lf %7.1lf %7.1lf",
                      ii+1,planet_name[nbody-1],ra.h,ra.m,ra.s, sign,dec.d,dec.m,dec.s, ap_ha,alt,azi);
              InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,ii);
              ii++;
             }
          }
       }
    return 0;
}
//--------------------------------------------------------------------------------
int calc_sun(void)
//--------------------------------------------------------------------------------
{           
 int ii,nbody;
 double distance,rpos[3];
 char sign;

 char planet_name[13][10]
  ={//"Mercury  ",// 0
    "수성     ",// 0
    "금성     ",// 1
    "지구     ",// 2
    "화성     ",// 3
    "목성     ",// 4
    "토성     ",// 5
    "천왕성   ",// 6
    "해왕성   ",// 7
    "명왕성   ",// 8
    "달       ",// 9
    "태양     ",//10
    "11       ",//11
    "12       " //12
   };
    
/* 
 char planet_name[13][10];
    sprintf(planet_name[ 1],"Mercury  ");
    sprintf(planet_name[ 2],"Venus    ");
    sprintf(planet_name[ 4],"Mars     ");
    sprintf(planet_name[ 5],"Jupiter  ");
    sprintf(planet_name[ 6],"Saturn   ");
    sprintf(planet_name[ 7],"Uranus   ");
    sprintf(planet_name[ 8],"Naptune  ");
    sprintf(planet_name[ 9],"Pluto    ");
    sprintf(planet_name[10],"Moon     ");
    sprintf(planet_name[11],"Sun      ");
*/    
           nbody=11;			  
           gcsbpos(jd_ut1,nbody,0,0,0,rpos);           
           ra.rad  =rpos[0];
           dec.rad =rpos[1];
           distance=rpos[2];

//         convert to topocentric position           
           gctotc(&ra.rad,&dec.rad,&distance,last,SITE_LATI.rad,SITE_HEIGHT);
           get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,&last,&ap_ha,&ap_dec,&alt,&azi);
           
           if(dec.rad<0.0)
             {sign = '-'; dec.rad = -dec.rad;}
           else
              sign = '+';           
              
		   TARGET_RA_2000.rad = ra.rad;
		   TARGET_DEC_2000.rad = dec.rad;
           rad_to_hms(&TARGET_RA_2000);rad_to_dms(&TARGET_DEC_2000);
              
    return 0;
}
//--------------------------------------------------------------------------------
static int display_messier(void)
//--------------------------------------------------------------------------------
{           
 int ii,m_no,k;
 double ram,size,mag;
 FILE *fin;
 char ibuff[200],name[20], type[3], sign;
 
    if( (fin=fopen("messier.cat","r")) == NULL )
      {MessagePopup("","Messier.cat를 열수 없습니다.");
       return -1;
      }
      
    SetCtrlVal(cat_handle,CAT_TEXTMSG,"지금 볼 수 있는 Messier 천체 ");

    sprintf(buff,"   No.   Name     R.A.(2000) Dec.     H.A.  Alt    Azi   Size  Mag    ");
    SetCtrlVal(cat_handle,CAT_HEAD,buff);    
      
    ClearListCtrl (cat_handle,CAT_LISTBOX);
    
//    sprintf(buff,"       Name     R.A.(2000) Dec.     H.A.  Alt    Azi   Size  Mag");
//    InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,ii);                      
      
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    
    ii = 0;
    while( fgets(ibuff,200,fin) != NULL)
         {
          sscanf(&ibuff[17],"%d %lf %c%d %d",&ra.h, &ram, &sign, &dec.d, &dec.m);
          ra.rad = (ra.h + ram/60.0) * 15.0; ra.rad = deg_to_rad(ra.rad);
          rad_to_hms(&ra);
          
          dec.s = 0.0; dms_to_rad(&dec);
          if(sign=='-') {dec.rad = -dec.rad; rad_to_dms(&dec);}
          
          get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,&last,&ap_ha,&ap_dec,&alt,&azi);
          
          alt = rad_to_deg(alt);
          azi = rad_to_deg(azi);
          ap_ha  = rad_to_deg(ap_ha) /15.0;
          ap_dec = rad_to_deg(ap_dec);
          
          if(alt>10.0)
            {
             sscanf(&ibuff[14],"%s",type);
             sscanf(ibuff,"%d",&m_no);
             sprintf(name,"M_%3d_%2s",m_no,type);
             for(k=0;k<8;k++) if(name[k]==' ') name[k]='0';
             
             sscanf(&ibuff[41],"%lf %lf",&size,&mag);
             
             if(sign=='-') {dec.rad = -dec.rad; rad_to_dms(&dec);}
             
             sprintf(buff,"%2d %10s %2d %2d %2.0lf %c%2d%3d%3.0lf %6.1lf %6.2lf %6.1lf %5.1lf %4.1lf",
                     ii+1,name,ra.h,ra.m,ra.s, sign,dec.d,dec.m,dec.s, ap_ha,alt,azi,
                     size, mag);
                     
             InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,ii);
             ii++;
             }
         }
         
    fclose(fin);
    return 0;
}
//--------------------------------------------------------------------------------
static int display_usercat(void)
//--------------------------------------------------------------------------------
{           
 int ii,starno;
 FILE *fin;
 char ibuff[200],name[20],sign,mag[10];
 
    if( (fin=fopen("user.cat","r")) == NULL )
      {MessagePopup("","USER.CAT 파일을 열 수 없습니다.");
       return -1;
      }
      
    SetCtrlVal(cat_handle,CAT_TEXTMSG,"     USER catalogue ");
    sprintf(mag,"***");

    sprintf(buff,"   No.    Name     R.A.(2000) Dec.    H.A.  Alt     Azi  Mag          ");
    SetCtrlVal(cat_handle,CAT_HEAD,buff);    
      
    ClearListCtrl (cat_handle,CAT_LISTBOX);
    ii = 0;
//  sprintf(buff,"        Name     R.A.(2000) Dec.    H.A.  Alt     Azi  Mag");
//  InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,ii);                      
      
    jd_ut1 = get_jdutc_clock() + DUT1_DAY;
    
    while( fgets(ibuff,200,fin) != NULL)
         {
         sscanf(ibuff,"%d %s %d %d %lf %c%d %d %lf %s",
                 &starno, name, &ra.h, &ra.m, &ra.s, &sign, &dec.d, &dec.m, &dec.s, mag);
                 
          if(starno==0) break;                 
          
                 
          hms_to_rad(&ra);
          
          dec.s = 0.0; dms_to_rad(&dec);
          if(sign=='-') {dec.rad = -dec.rad; rad_to_dms(&dec);}
          
          dms_to_rad(&dec);
          
          get_aposi(ra.rad,dec.rad,jd_ut1,SITE_LONGI.rad,SITE_LATI.rad,1,&last,&ap_ha,&ap_dec,&alt,&azi);
          
          alt    = rad_to_deg(alt);
          azi    = rad_to_deg(azi);
          ap_ha  = rad_to_deg(ap_ha) /15.0;
          ap_dec = rad_to_deg(ap_dec);
          
//          if(alt>10.0)
//          {
             if(sign=='-') {dec.rad = -dec.rad; rad_to_dms(&dec);}
             
             sprintf(buff,"%2d %10s %2d %2d %2.0lf %c%2d%3d%3.0lf %6.1lf %6.2lf %6.1lf  %s",
                     starno,name,ra.h,ra.m,ra.s, sign,dec.d,dec.m,dec.s, ap_ha,alt,azi,
                     mag);
             
             InsertListItem(cat_handle, CAT_LISTBOX,-1,buff,ii);
             ii++;
//           }

         }
         
    fclose(fin);
    return 0;
}
//--------------------------------------------------------------------------------
