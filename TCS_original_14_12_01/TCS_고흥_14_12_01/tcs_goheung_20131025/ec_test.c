//------------------------------------------------------------------------
#include <analysis.h>
#include <ansi_c.h>
#include <utility.h>
#include <userint.h>
#include "ec_test.h"
#include "type_def.h"
#include "st4s_lib.h" 
#include "cvi_util.h"
//#include "cpabtsub.h"
#include "c:\lib\cal_lib\astrometry.h"

extern double MOUNT_HA[20],MOUNT_DEC[20],ENC_TO_STEP[2];   // mount.inp
//------------------------------------------------------------------------
static char uir_file[20] = "ec_test.uir";
static int encoder_test_handle;
static char EC_AXIS = 'A';
static FILE *EC_FILE;
static int NO_OF_SAMPLES;
//------------------------------------------------------------------------
void begin_encoder_test(void);
static int analysis_ec_test(char fname[]);
static int plot_xy(int plot_handle, int control, double x[], double y[], int ndata, int type);
static int plot_encoder(void);
//------------------------------------------------------------------------
void begin_encoder_test()
//------------------------------------------------------------------------
{
    encoder_test_handle = LoadPanel (0,uir_file,EC_TEST);
    SetCtrlVal(encoder_test_handle,EC_TEST_SAMPLE_TIME,1.0);
    set_timer_interval(encoder_test_handle,EC_TEST_TIMER,1.0);
    DisplayPanel(encoder_test_handle);
}
//------------------------------------------------------------------------
int CVICALLBACK CB_EC_TEST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 int status,iaxis;
 double interval;
 int year,month,day, hour,minute,second;
 char fname[300];
 
	switch (event) {
		case EVENT_COMMIT:
             if(control==EC_TEST_START)
               {
                GetCtrlVal(panel,EC_TEST_RING,&status);
                EC_AXIS = 'A' + status;
                EC_FILE = fopen("ec_test.dat","w");

                fprintf(EC_FILE,"%c axis\n",EC_AXIS);
                
                GetSystemDate(&month,&day,&year);
                GetSystemTime(&hour,&minute,&second);                

                fprintf(EC_FILE,"%4d/%2d/%2d  %2d시%2d분%2d초\n",year,month,day, hour,minute,second);
                
                NO_OF_SAMPLES = 0;
                turn_on_timer(panel,EC_TEST_TIMER);
               }
             else if(control==EC_TEST_STOP)
               {
                turn_off_timer(panel,EC_TEST_TIMER);
                iaxis = EC_AXIS - 'A';
//              SlewDeceleration(EC_AXIS,RAMP[iaxis].f_base);
//              motor_stop(EC_AXIS);
                fclose(EC_FILE);
                analysis_ec_test("ec_test.dat");  
               }
             else if(control==EC_TEST_CLOSE)
               {
                DiscardPanel(encoder_test_handle);
                close_stdio();
               }
             else if(control==EC_TEST_SAMPLE_TIME)
               {
                GetCtrlVal(panel,control,&interval);
                set_timer_interval(panel,EC_TEST_TIMER,interval);
               }
               
             else if(control==EC_TEST_ANALYSIS)
               {
                if(file_select("*.dat",fname)!=0) analysis_ec_test(fname);
               }
             
             else if(control==EC_TEST_PLOTE12) plot_encoder();

			break;
	}
	return 0;							
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_EC_TIMER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 long counter,encoder,c2,e2;
 char buff[200];
	switch (event) {
		case EVENT_TIMER_TICK:
		
//          read_encoder('A', &counter);
//          read_encoder('B', &encoder);
			
			if(EC_AXIS=='C')
			  {
               read_couenc('A', &counter, &encoder);
               read_couenc('B', &c2     , &e2     );
               
               NO_OF_SAMPLES++;
               sprintf (buff,"%4d %10ld %10ld %10ld %10ld\n",NO_OF_SAMPLES,counter,encoder,c2,e2);
               SetCtrlVal(panel,EC_TEST_TEXT,buff);
               fprintf(EC_FILE,"%s",buff);
              }
			else
			  {
               read_couenc(EC_AXIS, &counter, &encoder);
               NO_OF_SAMPLES++;
               sprintf (buff,"%4d %10ld %10ld\n",NO_OF_SAMPLES,counter,encoder);

//             read_couenc('A', &counter, &encoder);
//             read_couenc('B', &c2     , &e2     );
//             NO_OF_SAMPLES++;
//             sprintf (buff,"%4d %10ld %10ld\n",NO_OF_SAMPLES,c2     ,encoder);

               SetCtrlVal(panel,EC_TEST_TEXT,buff);
//             fprintf(EC_FILE,"%4d %10ld %10ld\n",NO_OF_SAMPLES,counter,encoder);
               fprintf(EC_FILE,"%s",buff);
              }
            
			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK PRINT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-----------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_COMMIT:
//           PrintPanel(EC_TEST,"",1,VAL_FULL_PANEL,1);
             copy_panel_to_clipboard(panel);
			break;
		}
	return 0;
}
//-----------------------------------------------------------------------------
static int analysis_ec_test(char fname[])
// --------------------------------------------------------------------------
{
 double l1,l2;
// double counter[1000],encoder[1000],cond[1000][2],res[1000];
 double *counter,*encoder,*cond,*res;
 double Q[4][4],aty[4], para[4],use,use_sec;
 int ndata,i,npara;
 char buff[300],comment[300];
// char fname[300];
 double xmax,xmin,ymax,ymin,delta;
 int ixmax,ixmin,iymax,iymin;
 long d_e,d_p;
 int status;
 

    GetCtrlVal(encoder_test_handle,EC_TEST_RING,&status);
    EC_AXIS = 'A' + status;

//  if(file_select("*.dat",fname)==0) return -1;
 
    if( (EC_FILE = fopen(fname,"r"))==NULL )
      {MessagePopup(""," cannot find EC_TEST.DAT");
       exit(-1);
      }
		     
	fgets(buff,200,EC_FILE);
	sscanf(buff,"%d",&ndata);
    
    if((counter=(double *)malloc(ndata*8)) == NULL)    return -2; 
    if((encoder=(double *)malloc(ndata*8)) == NULL)    return -2; 
    if((res    =(double *)malloc(ndata*8)) == NULL)    return -2; 
    if((cond   =(double *)malloc(ndata*8*4)) == NULL)  return -2; 
    
    for(i=0;i<ndata;i++)
        {
         fgets(buff,200,EC_FILE);
         sscanf(buff,"%lf %lf %lf",&l1,&counter[i],&encoder[i]);

         cond[4*i  ] = 1.0;
         cond[4*i+1] = encoder[i];
         cond[4*i+2] = encoder[i]*encoder[i];
         cond[4*i+3] = encoder[i]*encoder[i]*encoder[i];
         }
    fclose(EC_FILE);
    
	d_e = encoder[ndata-1]-encoder[0];
	d_p = counter[ndata-1]-counter[0];
	
	GetCtrlVal(encoder_test_handle,EC_TEST_NPARA,&npara);
	
    status=clsq(cond,counter,*Q,aty,ndata,ndata,4,npara,para,res, &use);
    if(status!=0) return -1;

    if(EC_AXIS=='A') use_sec = use/MOUNT_HA[1];
    else             use_sec = use/MOUNT_DEC[1];
	
	for(i=0;i<npara;i++)
	   {
	    sprintf(buff,"PARA %d %lf\n",i,para[i]);
	    SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);
	   }
	
	    sprintf(buff,"USE=  %lf\n",use);
	    SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);
	
//	sprintf(buff,"x axis : counter, y axis : encoder\n");
//	SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);

	sprintf(buff,"# of data points : %d  Data range  Pulse: %ld Encoder: %ld\n",ndata,d_p,d_e);
	SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);
    
    sprintf(buff,"ENCODER/STEP =%10.6lf   오차:%8.1lf  pulse  %8.1lf arcsec",
            para[0],use,use_sec);
//    sprintf(buff,"STEP/ENCODER =%10.6lf   오차:%8.1lf  pulse  %8.1lf arcsec",
//            1.0/para[0],use,use_sec);

	SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);

//  plot counter vs. encoder
    MaxMin1D(counter,ndata,&xmax,&ixmax,&xmin,&ixmin);
    delta = (xmax - xmin)*0.05; xmax += delta; xmin -= delta;
    MaxMin1D(encoder,ndata,&ymax,&ixmax,&ymin,&ixmin);
    delta = (ymax - ymin)*0.05; ymax += delta; ymin -= delta;
    SetAxisRange(encoder_test_handle,EC_TEST_GRAPH,VAL_MANUAL,xmin,xmax,VAL_MANUAL,ymin,ymax);
    plot_xy(encoder_test_handle,EC_TEST_GRAPH,counter,encoder,ndata,0);
    
//  plot residual
//  change unit to degree and arcsec
//    for(i=0;i<ndata;i++)
//       {
//        if(EC_AXIS=='A')
//          {counter[i] = encoder[i]/MOUNT_HA[1]/3600.0;
//           res[i]     = res[i]    /MOUNT_HA[1];
//          }
//        else
//          {counter[i] = encoder[i]/MOUNT_DEC[1]/3600.0;
//         res[i]     = res[i]    /MOUNT_DEC[1];
//          }
//       }
       
    MaxMin1D(counter,ndata,&xmax,&ixmax,&xmin,&ixmin);
    delta = (xmax - xmin)*0.05; xmax += delta; xmin -= delta;
    MaxMin1D(res,ndata,&ymax,&ixmax,&ymin,&ixmin);
    delta = (ymax - ymin)*0.05; ymax += delta; ymin -= delta;
    SetAxisRange(encoder_test_handle,EC_TEST_GRAPH_2,VAL_MANUAL,xmin,xmax,VAL_MANUAL,ymin,ymax);
    
    plot_xy(encoder_test_handle,EC_TEST_GRAPH_2,counter,res,ndata,0);

//  write output
    EC_FILE = fopen("EC_TEST.OUT","w");
    for(i=0;i<ndata;i++)
    fprintf(EC_FILE,"%4d %10.1lf %10.1lf %10.1lf\n",i,counter[i],encoder[i],res[i]);
    fprintf(EC_FILE,"ENCODER/STEP =%10.6lf  U.S.E.: %8.1lf %8.1lf arcsec\n",
            para[0],use,use_sec);
    fprintf(EC_FILE,"STEP/ENCODER =%10.6lf",1.0/para[0]);
    fclose(EC_FILE);   
    
    free(encoder);
    free(counter);
    free(res);
    free(cond);

	return 0;
}
//-----------------------------------------------------------------------------
static int plot_xy(int plot_handle, int control, double x[], double y[], int ndata, int type)
//-----------------------------------------------------------------------------
{
 int plot_1, color;

     if(type==0) color = VAL_RED; else color = VAL_YELLOW;
/*     
     plot_1 = PlotXY(plot_handle,control,x,y,ndata,VAL_DOUBLE,VAL_DOUBLE,
                       VAL_SCATTER,VAL_SOLID_CIRCLE, VAL_SOLID, 1,color);
     DeleteGraphPlot(plot_handle,control,plot_1,VAL_NO_DRAW);
*/     
     plot_1 = PlotXY(plot_handle,control,x,y,ndata,VAL_DOUBLE,VAL_DOUBLE,
                       VAL_THIN_LINE,VAL_SOLID_CIRCLE, VAL_SOLID, 1,color);
     DeleteGraphPlot(plot_handle,control,plot_1,VAL_NO_DRAW);

    return plot_1;       
}
//-----------------------------------------------------------------------------
static int plot_encoder()
// --------------------------------------------------------------------------
{
 double l1,l2;
// double c1,c2,t[1000],encoder1[1000],encoder2[1000];
 double *c1,*c2,*t,*e1,*e2;
 int ndata,i;
 char buff[300],comment[50],fname[300];
 int status;
 
    GetCtrlVal(encoder_test_handle,EC_TEST_RING2,&status);
    EC_AXIS = 'A' + status;

    if(file_select("*.dat",fname)==0) return -1;
 
    if( (EC_FILE = fopen(fname,"r"))==NULL )
      {MessagePopup(""," cannot find EC_TEST.DAT");
       exit(-1);
      }

//  find ndata and allocate memory
	fgets(buff,200,EC_FILE);
	fgets(buff,200,EC_FILE);
	ndata = 0;
    while(fgets(buff,100,EC_FILE)!= NULL) ndata++;
    
    if((c1=(double *)malloc(ndata*8)) == NULL)  return -2; 
    if((c2=(double *)malloc(ndata*8)) == NULL)  return -2; 
    if((e1=(double *)malloc(ndata*8)) == NULL)  return -2; 
    if((e2=(double *)malloc(ndata*8)) == NULL)  return -2; 
    if((t =(double *)malloc(ndata*8)) == NULL)  return -2; 

//  read the data
    rewind(EC_FILE);
      
	fgets(buff,90,EC_FILE);
	sscanf(buff,"%c",&EC_AXIS);
		     
	fgets(comment,40,EC_FILE);
	sprintf(buff,"실험 일시 : %s",comment);
	ResetTextBox(encoder_test_handle,EC_TEST_TEXT,buff);
	
	//  plot    
    GetCtrlVal(encoder_test_handle,EC_TEST_RING2,&status);
	
    sprintf(buff,"status : %d %d",status, ndata);
//	ResetTextBox(encoder_test_handle,EC_TEST_TEXT,buff);
	SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);

    for(i=0;i<ndata;i++)
         {
          fgets(buff,90,EC_FILE);  
          sscanf(buff,"%lf %lf %lf %lf %lf",&t[i],&c1[i],&e1[i],&c2[i],&e2[i]);
//        ResetTextBox(encoder_test_handle,EC_TEST_TEXT,buff);   
          SetCtrlVal(encoder_test_handle,EC_TEST_TEXT,buff);
         }
    fclose(EC_FILE);

    if(status==0)
      {
       plot_xy(encoder_test_handle,EC_TEST_GRAPH  ,t,e1,ndata,0);
       plot_xy(encoder_test_handle,EC_TEST_GRAPH_2,t,e2,ndata,0);
      }
    else if(status==1)
      {
       plot_xy(encoder_test_handle,EC_TEST_GRAPH  ,c1,e1,ndata,0);
       plot_xy(encoder_test_handle,EC_TEST_GRAPH_2,c2,e2,ndata,0);
      }
    else if(status==2)
      {
       plot_xy(encoder_test_handle,EC_TEST_GRAPH  ,t,c1,ndata,0);
       plot_xy(encoder_test_handle,EC_TEST_GRAPH_2,t,c2,ndata,0);
      }
    else if(status==3)
      {
       plot_xy(encoder_test_handle,EC_TEST_GRAPH  ,t,c1,ndata,0);
       plot_xy(encoder_test_handle,EC_TEST_GRAPH_2,t,e1,ndata,0);
      }
    else if(status==4)
      {
       plot_xy(encoder_test_handle,EC_TEST_GRAPH  ,t,c2,ndata,0);
       plot_xy(encoder_test_handle,EC_TEST_GRAPH_2,t,e2,ndata,0);
      }

    free(t);
    free(c1); free(e1);
    free(c2); free(e2);
    
    return 0;
}    
// --------------------------------------------------------------------------
int CVICALLBACK ECTEXTCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
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
// --------------------------------------------------------------------------
