//-----------------------------------------------------------------------------
#include <analysis.h>
#include <utility.h>
#include <userint.h>
#include <ansi_c.h>
//-----------------------------------------------------------------------------
int turn_on_timer (int handle, int control);
int turn_off_timer(int handle, int control);
int set_timer_interval(int handle, int control, double interval);
int set_panel_title(int handle, char title[]);
int set_control_visible(int panel_handle, int control);
int set_control_hidden (int panel_handle, int control);
int set_control_dim(int handle, int control);
int set_control_undim(int handle, int control);
int intsort(int n, int arrin[], int indx[]);

void close_stdio(void);
void open_stdio (void);

int copy_panel_to_clipboard(int panel);
int copy_active_control_to_clipboard(int panel);

int auto_plot_xy(int plot_handle, int control, double x[], double y[], int ndata, int type);
int auto_plot_xy_scatter(int plot_handle, int control, double x[], double y[], int ndata);
int auto_plot_xy_line   (int plot_handle, int control, double x[], double y[], int ndata);

int set_on_color(int panel,int control);
int file_select(char file_ext[], char file_name[]);
int dir_select (char default_dir[], char dirname[]);
int get_fbody(char fname[], char fbody[]);
int get_dir  (char fname[], char dir[]);
int del_dir(char dirfname[], char fname[]);

int sort_double(int ndata, double yinp[], int indx[]);
int cubic_spline_interpolate(int n1, double y1[], int df, double y2[]);

//-----------------------------------------------------------------------------
int turn_on_timer(int handle, int control)
//-----------------------------------------------------------------------------
{
    return SetCtrlAttribute(handle,control,ATTR_ENABLED,1);
}		     
//-----------------------------------------------------------------------------
int turn_off_timer(int handle, int control)
//-----------------------------------------------------------------------------
{
    return SetCtrlAttribute(handle,control,ATTR_ENABLED,0);
}		     
//-----------------------------------------------------------------------------
int set_timer_interval(int handle, int control, double interval)
//-----------------------------------------------------------------------------
{
    return SetCtrlAttribute(handle,control,ATTR_INTERVAL,interval);
}        
//-----------------------------------------------------------------------------
void close_stdio(void)
//-----------------------------------------------------------------------------
{
    SetStdioWindowVisibility (0);   
}
//-----------------------------------------------------------------------------
void open_stdio(void)
//-----------------------------------------------------------------------------
{
    SetStdioWindowVisibility (1);   
}
//-----------------------------------------------------------------------------
int set_panel_title(int handle, char title[])
//-----------------------------------------------------------------------------
{
    return SetPanelAttribute (handle, ATTR_TITLE, title);
}
//-----------------------------------------------------------------------------
int set_control_visible(int panel_handle, int control)
//-----------------------------------------------------------------------------
{
    return SetCtrlAttribute (panel_handle,control, ATTR_VISIBLE, 1);
}
//-----------------------------------------------------------------------------
int set_control_hidden(int panel_handle, int control)
//-----------------------------------------------------------------------------
{
    return SetCtrlAttribute (panel_handle,control, ATTR_VISIBLE, 0);
}
//-----------------------------------------------------------------------------
int copy_panel_to_clipboard(int panel)
//-----------------------------------------------------------------------------
{
 int bitmap;
    GetPanelDisplayBitmap(panel,VAL_VISIBLE_AREA,VAL_ENTIRE_OBJECT,&bitmap);
    ClipboardPutBitmap(bitmap);
    DiscardBitmap(bitmap);

    return 0;
}
//-----------------------------------------------------------------------------
int copy_active_control_to_clipboard(int panel)
//-----------------------------------------------------------------------------
{
 int ctrl, bitmap, include_label;

    ctrl = GetActiveCtrl(panel);
    if(ctrl>0)
      {include_label = ConfirmPopup("Copying Active Control to Clipboard","Include label?");
       GetCtrlDisplayBitmap(panel,ctrl,include_label,&bitmap);
       ClipboardPutBitmap(bitmap);
       DiscardBitmap(bitmap);
       return 0;
      }
    else
      return -1;
}
//-----------------------------------------------------------------------------
int set_control_dim(int handle, int control)
//-----------------------------------------------------------------------------
{
     return SetCtrlAttribute(handle,control,ATTR_DIMMED,1);
}     
//-----------------------------------------------------------------------------
int set_control_undim(int handle, int control)
//-----------------------------------------------------------------------------
{
     return SetCtrlAttribute(handle,control,ATTR_DIMMED,0);
}     
//-----------------------------------------------------------------------------

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
int auto_plot_xy(int plot_handle, int control, double x[], double y[], int ndata, int type)
//-----------------------------------------------------------------------------
{
 int plot_1;
 double xmax,xmin,ymax,ymin,zmax,zmin,del;
 int    ixmax,ixmin,iymax,iymin;
// int izmax,izmin;
 
    MaxMin1D(x,ndata,&xmax,&ixmax,&xmin,&ixmin);
    del = xmax - xmin;
    if(del<=0.0) del = 10.0;
    xmax += del*0.05; xmin -= del*0.05;

    MaxMin1D(y,ndata,&ymax,&iymax,&ymin,&iymin);
    del = ymax - ymin;
    if(del<=0.0) del = 10.0;
    ymax += del*0.05; ymin -= del*0.05;

    
    SetAxisRange(plot_handle,control,VAL_MANUAL,xmin,xmax,VAL_MANUAL,ymin,ymax);

    if(type==0) // scatter       
       plot_1 = PlotXY(plot_handle,control,x,y,ndata,VAL_DOUBLE,VAL_DOUBLE,
                       VAL_SCATTER,VAL_SOLID_CIRCLE,VAL_SOLID, 1,VAL_RED);

    else if(type==1) // thin line       
       plot_1 = PlotXY(plot_handle,control,x,y,ndata,VAL_DOUBLE,VAL_DOUBLE,
                       VAL_THIN_LINE,VAL_SOLID_CIRCLE,VAL_SOLID,1,VAL_RED);
                       
    DeleteGraphPlot(plot_handle,control,plot_1,VAL_NO_DRAW);

    return plot_1;       
}
//-----------------------------------------------------------------------------
int auto_plot_xy_scatter(int plot_handle, int control, double x[], double y[], int ndata)
//-----------------------------------------------------------------------------
{
    return auto_plot_xy(plot_handle,control,x,y,ndata,0);
}
//-----------------------------------------------------------------------------
int auto_plot_xy_line(int plot_handle, int control, double x[], double y[], int ndata)
//-----------------------------------------------------------------------------
{
    return auto_plot_xy(plot_handle,control,x,y,ndata,1);
}
//-----------------------------------------------------------------------------
int set_on_color(int panel,int control)
//-----------------------------------------------------------------------------
{
    return SetCtrlAttribute (panel,control,ATTR_ON_COLOR,VAL_RED);
}
//-----------------------------------------------------------------------------
int file_select(char file_ext[], char file_name[])
//-----------------------------------------------------------------------------
{
      return FileSelectPopup ("",file_ext,file_ext, "Name of File to open",
              VAL_OK_BUTTON, 0, 0, 1, 0,file_name);
}
//-----------------------------------------------------------------------------
int dir_select(char default_dir[], char dirname[])
//-----------------------------------------------------------------------------
{
    return DirSelectPopup (default_dir,"Select Directory", 1, 1, dirname);
}    
//-----------------------------------------------------------------------------
int del_dir(char dirfname[], char fname[])
//-----------------------------------------------------------------------------
{
 int ilen,i,ii;
 
 // sprintf(fbody,"%s",dirfname);
    
    ilen = strlen(dirfname);
    ii = -1;
    for(i=ilen-1;i>=0;i--) if(dirfname[i]=='\\') {ii=i; break;}
    ii++;
    
    sprintf(fname,"%s",&dirfname[ii]);
    
 return ii;    
}
//-----------------------------------------------------------------------------
int get_fbody(char fname[], char fbody[])
//-----------------------------------------------------------------------------
{
 int ilen,i,ii;
 
    sprintf(fbody,"%s",fname);
    
    ilen = strlen(fname);
    ii = -1;
    for(i=ilen-1;i>=0;i--) if(fname[i]=='\\') {ii=i; break;}
    ii++;
    
    sprintf(fbody,"%s",&fname[ii]);
    
    ilen = strlen(fbody);
    ii = -1;
    for(i=ilen-1;i>=0;i--) if(fbody[i]=='.') {ii=i; break;}
    
    if(ii>0) fbody[ii] = NULL;
    
 return ii;    
}
//-----------------------------------------------------------------------------
int get_dir(char fname[], char dir[])
//-----------------------------------------------------------------------------
{
 int ilen,i,ii;
 
    sprintf(dir,"%s",fname);
    
    ilen = strlen(fname);
    ii = -1;
    for(i=ilen-1;i>=0;i--) if(fname[i]=='\\') {ii=i; break;}
    
    if(ii>0) dir[ii] = NULL;
    else     dir[0 ] = NULL;
//  display_message(WDIR);    
//  if(ii=-1) ii = 0;
    
 return ii;    
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
int sort_double(int ndata, double yinp[], int indx[])
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
               indxt = indx[ir];    //INDXT = INDX(IR)
               q = yinp[indxt];     //Q = ARRIN(INDXT)
               indx[ir] = indx[1];  //INDX(IR) = INDX(1)
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
int sort_double_old(int ndata, double yinp[], int indx[])
//-----------------------------------------------------------------------------
{
 int i,l,j,ir;
 double q;
 int indxt;

    for(j=0;j<ndata;j++) indx[j] = j;

	l  = ndata/2 +1;// L = N/2 + 1
	ir = ndata;    // IR = N

        for(;;)
           {
            if(l>1)                   // IF(L.GT.1) THEN
              {l = l - 1;             //   L = L - 1
               indxt = indx[l-1];     //   INDXT = INDX(L)
               q     = yinp[indxt-1]; //   Q = ARRIN(INDXT)
              }
            else
              {
               indxt = indx[ir-1];    //INDXT = INDX(IR)
               q = yinp[indxt-1];     //Q = ARRIN(INDXT)
               indx[ir-1] = indx[0];  //INDX(IR) = INDX(1)
               ir = ir - 1;           //IR = IR - 1
               if(ir==1)              //IF(IR.EQ.1) THEN
                 {indx[0] = indxt;    //INDX(1) = INDXT
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
//                   IF(ARRIN(INDX(J)).LT.  ARRIN(INDX(J+1)) ) J = J+1
                     if( yinp[indx[j-1]-1] < yinp[indx[j]-1] ) j = j+1;
                    } // ENDIF

                  if(q < yinp[indx[j-1]-1]) //IF(Q.LT.ARRIN(INDX(J))) THEN
                    {indx[i-1] = indx[j-1]; //INDX(I) = INDX(J)
                     i = j;                 //I = J
                     j = j + j;             //J = J + J
                    }
                  else                      //ELSE
                     j = ir + 1;            //J = IR + 1
                 //ENDIF
                }
	      indx[i-1] = indxt; //INDX(I) = INDXT
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
//-----------------------------------------------------------------------------
