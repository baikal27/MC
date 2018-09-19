//----------------------------------------------------------------------------
#include <analysis.h>
#include <formatio.h>
#include <utility.h>
#include <userint.h>
#include <ansi_c.h>
#include <rs232.h>
#include "type_def.h"
#include "cvi_util.h"
//#include "cpabtsub.h"
#include "c:\lib\cal_lib\cpabtsub.h"
#include "st4s_lib.h"
#include "st4stest.h"
//----------------------------------------------------------------------------
extern void begin_encoder_test(void);
//----------------------------------------------------------------------------
static int  panel_handle, adc_handle, pwm_handle, clock_handle;
int  download_handle, ramp_handle;
int  clock_test_handle, encoder_test_handle, slew_test_handle, hp_test_handle;
long gCount=0;
int  RS232Error;
static char file_name[MAX_PATHNAME_LEN];

static long   COUNTS[4];
static long   ENCODER[4];
static word   DIVISOR[4];
static double SPEED[4];
static char   DAXIS = 'D';

extern int DEVICE_ID;
static int COM_PORT;

char uir_file[20]= "st4stest.uir";

static FILE *adc_out;
static int ICYCLE;
static char BUFFER[100];
//----------------------------------------------------------------------------
int  show_user_led(void);  
int  execute_one(void);
int  display_tcs_status(void);
int  get_uio_string(char str[], int channel);
int  adc_test(void);
int  pwm_test(void);
int  adc_analysis(void);
int  io_test(void);

static int display_ioport0(void);
static int clock_test_analysis(void);
int set_rtc_int(int i);
static void display_message(char buff[]);
int  set_alluo(char caxis,int state);

//----------------------------------------------------------------------------
int load_st4stest(int comport)
//----------------------------------------------------------------------------
{
 int e_code, result;
 char inp_buffer[20],out_buffer[20],head[5];
 long step,encoder;
 long i,loops;
 word divisor;
 FILE *fin;
 int value;
 double t0,t1;
 
    panel_handle = LoadPanel (0,uir_file,MAIN);
    DisplayPanel (panel_handle);
    
//  timer on    
    SetCtrlAttribute(panel_handle,MAIN_TIMER,ATTR_ENABLED,1);
    SetCtrlVal      (panel_handle,MAIN_LOOP_ON           ,1);
    
//  init_rtc();
    SetCtrlAttribute (panel_handle,MAIN_GATE_A,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute (panel_handle,MAIN_GATE_A,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    SetCtrlAttribute (panel_handle,MAIN_GATE_B,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute (panel_handle,MAIN_GATE_B,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    SetCtrlAttribute (panel_handle,MAIN_GATE_C,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute (panel_handle,MAIN_GATE_C,ATTR_OFF_COLOR, VAL_PANEL_GRAY);
    SetCtrlAttribute (panel_handle,MAIN_GATE_D,ATTR_ON_COLOR , VAL_RED);
    SetCtrlAttribute (panel_handle,MAIN_GATE_D,ATTR_OFF_COLOR, VAL_PANEL_GRAY);

    display_tcs_status();
    
    return 0;
}
//-------------------------------------------------------------------------------
int CVICALLBACK CB_LOOP_TIMER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
    switch(event)
          {
           case EVENT_TIMER_TICK:
                display_tcs_status();
           break;
          }
    return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK SET_COUNTER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 double dcount;
 long count_a;
 char caxis;
 
    if     (control==MAIN_COUNT_A) caxis = 'A';
    else if(control==MAIN_COUNT_B) caxis = 'B';
    else if(control==MAIN_COUNT_C) caxis = 'C';
    else if(control==MAIN_COUNT_D) caxis = 'D';
    else return -1;
 
	switch (event) {
		case EVENT_COMMIT:
		    GetCtrlVal(panel       ,control,&dcount);
		    count_a = (long) dcount;
		    set_counter(caxis,count_a);
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
//----------------------------------------------------------------------------
int CVICALLBACK SET_ENCODER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 char caxis;
 long count; double dcount;
 
    if     (control==MAIN_ENCODER_A) caxis = 'A';
    else if(control==MAIN_ENCODER_B) caxis = 'B';
    else if(control==MAIN_ENCODER_C) caxis = 'C';
    else if(control==MAIN_ENCODER_D) caxis = 'D';
    
  	switch(event)
  	      {
           case EVENT_COMMIT:
		        GetCtrlVal(panel_handle,control,&dcount);
                count = (long) dcount;
                set_encoder(caxis,count);
	       break;
           case EVENT_RIGHT_CLICK:
           break;
          }
    return 0;
}
//----------------------------------------------------------------------------
int CVICALLBACK SET_DIVISOR (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 char caxis;
 int iaxis;
 
    if     (control==MAIN_SPEED_A) caxis = 'A';
    else if(control==MAIN_SPEED_B) caxis = 'B';
    else if(control==MAIN_SPEED_C) caxis = 'C';
    else if(control==MAIN_SPEED_D) caxis = 'D';
    
    iaxis = caxis - 'A';

	switch(event)
	      {
           case EVENT_COMMIT:
           
                GetCtrlVal(panel_handle,control,&SPEED[iaxis]);
                if(caxis=='A' || caxis=='B')
                  {
                   DIVISOR[iaxis] = F_1/SPEED[iaxis] + 0.5;
                   SPEED  [iaxis] = F_1/DIVISOR[iaxis];
                  }
                else
                  {
                   DIVISOR[iaxis] = F_2/SPEED[iaxis] + 0.5;
                   SPEED  [iaxis] = F_2/DIVISOR[iaxis];
                  }
                  
                set_divisor(caxis,DIVISOR[iaxis]);
                
           break;
           case EVENT_RIGHT_CLICK:
           break;
	}
	return 0;
}
//----------------------------------------------------------------------------
int CVICALLBACK MOTOR_ON (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 char caxis;
 int status;
 
    if     (control==MAIN_GATE_A) caxis = 'A';
    else if(control==MAIN_GATE_B) caxis = 'B';
    else if(control==MAIN_GATE_C) caxis = 'C';
    else if(control==MAIN_GATE_D) caxis = 'D';

	switch (event) {
		case EVENT_COMMIT:
		     GetCtrlVal(panel_handle,control,&status);
		     if(status==1) motor_go  (caxis);
		     else          motor_stop(caxis);
		     break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
//----------------------------------------------------------------------------
int CVICALLBACK CB_DIR (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 char caxis; int status;
 
    if     (control==MAIN_DIR_A) caxis = 'A';
    else if(control==MAIN_DIR_B) caxis = 'B';
    else if(control==MAIN_DIR_C) caxis = 'C';
    else if(control==MAIN_DIR_D) caxis = 'D';
    
	switch (event) {
		case EVENT_COMMIT:
		     GetCtrlVal(panel_handle,control,&status);
		     if(status==1) set_dir(caxis,1);
		     else          set_dir(caxis,0);
		     break;
		case EVENT_RIGHT_CLICK:
			break;
		}

	return 0;
}
//----------------------------------------------------------------------------
int display_tcs_status(void)
//----------------------------------------------------------------------------
{
 char out_buffer[100],inp_buffer[100];
 int i,e_code, itemp;
 char buff[30],str[9],caxis;
 int dir[4],gate[4],cw[4],ccw[4],home[4];
 int ivalue;
 long timer2;
 
 // display DATE and TIME
    SetCtrlVal(panel_handle,MAIN_DATE,DateStr());
    SetCtrlVal(panel_handle,MAIN_TIME,TimeStr());
 
    for(i=0;i<NO_OF_AXIS;i++)
       {
        caxis = 'A'+i;

        read_encoder(caxis, &ENCODER[i]);
//        Delay(0.01);
        read_gate   (caxis,&gate[i]);
//        Delay(0.01);
        read_dir    (caxis,&dir [i]);
//        Delay(0.01);
        read_cw_limit  (caxis,&cw  [i]);
//        Delay(0.01);
        read_ccw_limit (caxis,&ccw [i]);
//        Delay(0.01);
        read_home_limit(caxis,&home[i]);
//        Delay(0.01);
        read_counter(caxis, &COUNTS [i]);
//        Delay(0.01);
         
        read_divisor(caxis, &DIVISOR[i]);
        if(caxis=='A' || caxis == 'C') SPEED[i] = F_1/DIVISOR[i];
        else                           SPEED[i] = F_2/DIVISOR[i];


        if(i==0)
          {SetCtrlVal(panel_handle,MAIN_COUNT_A,  (double) COUNTS[i]);
           SetCtrlVal(panel_handle,MAIN_ENCODER_A,(double)ENCODER[i]);
           SetCtrlVal(panel_handle,MAIN_SPEED_A,            SPEED[i]);
           SetCtrlVal(panel_handle,MAIN_GATE_A, gate[i]);
           SetCtrlVal(panel_handle,MAIN_DIR_A , dir [i]);
           
           SetCtrlVal(panel_handle,MAIN_CW_A  ,cw[i]  );
           SetCtrlVal(panel_handle,MAIN_CCW_A ,ccw[i] );
           SetCtrlVal(panel_handle,MAIN_HOME_A,home[i]);
          }
           
        else if(i==1)
          {SetCtrlVal(panel_handle,MAIN_COUNT_B,  (double) COUNTS[i]);
           SetCtrlVal(panel_handle,MAIN_ENCODER_B,(double)ENCODER[i]);
           SetCtrlVal(panel_handle,MAIN_SPEED_B,            SPEED[i]);
           SetCtrlVal(panel_handle,MAIN_GATE_B,   (int) gate[i]);
           SetCtrlVal(panel_handle,MAIN_DIR_B,    (int) dir [i]);
           
           SetCtrlVal(panel_handle,MAIN_CW_B  ,cw[i]  );
           SetCtrlVal(panel_handle,MAIN_CCW_B ,ccw[i] );
           SetCtrlVal(panel_handle,MAIN_HOME_B,home[i]);
          }
           
        else if(i==2)
          {SetCtrlVal(panel_handle,MAIN_COUNT_C,  (double) COUNTS[i]);
           SetCtrlVal(panel_handle,MAIN_ENCODER_C,(double)ENCODER[i]);
           SetCtrlVal(panel_handle,MAIN_SPEED_C,            SPEED[i]);
           SetCtrlVal(panel_handle,MAIN_GATE_C,   (int) gate[i]);
           SetCtrlVal(panel_handle,MAIN_DIR_C,    (int) dir [i]);
           
           SetCtrlVal(panel_handle,MAIN_CW_C  ,cw[i]  );
           SetCtrlVal(panel_handle,MAIN_CCW_C ,ccw[i] );
           SetCtrlVal(panel_handle,MAIN_HOME_C,home[i]);
          }

        else if(i==3)
          {SetCtrlVal(panel_handle,MAIN_COUNT_D,  (double) COUNTS[i]);
           SetCtrlVal(panel_handle,MAIN_ENCODER_D,(double)ENCODER[i]);
           SetCtrlVal(panel_handle,MAIN_SPEED_D,            SPEED[i]);
           SetCtrlVal(panel_handle,MAIN_GATE_D,   (int) gate[i]);
           SetCtrlVal(panel_handle,MAIN_DIR_D,    (int) dir [i]);
           
           SetCtrlVal(panel_handle,MAIN_CW_D  ,cw[i]  );
           SetCtrlVal(panel_handle,MAIN_CCW_D ,ccw[i] );
           SetCtrlVal(panel_handle,MAIN_HOME_D,home[i]);
          }
       }

//  display slew mode
    GetCtrlVal(panel_handle,MAIN_SLEW_AXIS,&i);
    caxis = i + 'A';
    read_slew_mode (caxis,&i);
    sprintf(buff,"%d",i);
    ResetTextBox(panel_handle,MAIN_SLEW_MODE,buff);
        
//  display user input0
    show_user_led();

//  display user ioport0
    display_ioport0();

//  display Timer2
    read_timer2(&timer2);
    itemp = timer2;
    sprintf(buff,"%ld",itemp);
    SetCtrlVal(panel_handle,MAIN_TIMER2,buff);

    return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK SLEWTEST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 double interval;
 
	switch (event) {
		case EVENT_COMMIT:
		     if     (control==SLEW_START) turn_on_timer (panel,SLEW_TIMER);
		     else if(control==SLEW_STOP ) turn_off_timer(panel,SLEW_TIMER);
		     else if(control==SLEW_CLOSE)
		       {DiscardPanel(slew_test_handle);
                close_stdio();
               }
             else if(control==SLEW_SAMPLE_TIME)
               {
                GetCtrlVal(panel,control,&interval);
                set_timer_interval(panel,SLEW_TIMER,interval);
               }

			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_SLEW_TIMER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 int state;
 int i;
 char caxis;
 
	switch (event) {
		case EVENT_TIMER_TICK:
		    for(i=0;i<NO_OF_AXIS;i++)
		       {
		        caxis = 'A' + i;
//            read_gate('A',&state); if(state==0) SlewStop_P('A',100000);
//            read_gate('B',&state); if(state==0) SlewStop_P('B',100000); 
              read_gate(caxis,&state); if(state==0) SlewStop_P(caxis,100000); 
               }
            display_tcs_status();
            break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_SLEW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 char caxis; long step; double dstep;
 int iaxis;
 
	switch (event) {
		case EVENT_COMMIT:
		     GetCtrlVal(panel_handle,MAIN_SLEW_AXIS,&iaxis);
             caxis = iaxis + 'A';
//           printf("iaxis %2d caxis %c\n",iaxis,caxis);
             
             if(control== MAIN_SLEW)		
		       {GetCtrlVal(panel_handle,MAIN_SLEW_COUNT,&dstep);
		        step = (long) dstep;
		        SlewStop_P(caxis,step);
		       }
		     
		    else if(control==MAIN_SLEW_ACC) SlewAcceleration(caxis,RAMP[iaxis].f_max );
		    else if(control==MAIN_SLEW_DEC) SlewDeceleration(caxis,RAMP[iaxis].f_base);
		     
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
//----------------------------------------------------------------------------
int CVICALLBACK STAPPLY (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//----------------------------------------------------------------------------
{
 int status,i;
 double interval;
 char caxis;
 
	switch (event) {
		case EVENT_COMMIT:
               
             if(control==MAIN_INIT_TCS    )
               {
                SetWaitCursor(1);
                init_tcs(COM_PORT);
                SetWaitCursor(0);
                Beep();
//              MessagePopup("","TCS 초기화 완료 ........");
                close_stdio();
                display_tcs_status();
               }
               
		     else if(control==MAIN_EC_TEST) begin_encoder_test();
               
		     else if(control==MAIN_SLEW_TEST)
			   {
                slew_test_handle = LoadPanel (0,uir_file,SLEW);
                DisplayPanel(slew_test_handle);
               }
               
		     else if(control==MAIN_HP_TEST    )
			   {
                hp_test_handle = LoadPanel (0,uir_file,HP);
                DisplayPanel(hp_test_handle);
               }
               
		     else if(control==MAIN_LOOP_ON)
			   {
                GetCtrlVal(panel_handle,control,&status);
                if(status==1)
                   SetCtrlAttribute(panel_handle,MAIN_TIMER,ATTR_ENABLED,1);
                else
                   SetCtrlAttribute(panel_handle,MAIN_TIMER,ATTR_ENABLED,0);
               }
               
		     else if(control==MAIN_QUIT) DiscardPanel(panel_handle);
               
		     else if(control==MAIN_TIMER_INTERVAL)
			   {
                GetCtrlVal(panel_handle,MAIN_TIMER_INTERVAL,&interval);
                ResetTimer(panel_handle,MAIN_TIMER);
                status = SetCtrlAttribute(panel_handle,MAIN_TIMER,ATTR_INTERVAL,interval);
               }
               
		     else if(control==MAIN_ADC) adc_test();

		     else if(control==MAIN_RESET_TIMER2) set_timer2(0);
		     
		     else if(control==MAIN_PWM) pwm_test();

		     else if(control==MAIN_CLOCK)
 		       {clock_handle = LoadPanel (0,uir_file,CLOCK);
                DisplayPanel(clock_handle);
               }
               
		     else if(control==MAIN_DEVICE)
		       {
		        GetCtrlVal(panel_handle,MAIN_DEVICE,&i);
		        DEVICE_ID = i + 'A';
		       }
  
		     else if(control==MAIN_TEMP)
		       {
		        GetCtrlVal(panel_handle,MAIN_DEVICE,&i);
		        DEVICE_ID = i + 'A';
		       }
		       
		     else if(control==MAIN_IOTEST)	io_test();
		       
             break;
               
		case EVENT_RIGHT_CLICK:
			 break;
	}
	return 0;
}
//--------------------------------------------------------------------------
int CVICALLBACK CB_HP_TIMER(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 hp_status hp;
 
	switch (event) {
		case EVENT_TIMER_TICK:
//             read_hp_status(&hp);
/*---------------------------------------------------------------------------*/
//  ew_dir     : -1 =>E , 1 => W, 0 => None
//  ns_dir     : -1 =>S , 1 => N, 0 => None
//  speed_mode :  1 => guide, 2 => set, 3 => slew
//  mode       :  0 => computer. 1 => hand paddle
//  motor      :  1 => ALT/AZ, 2 => Derotator, 3 => focus
/*---------------------------------------------------------------------------*/
             if     (hp.ew==-1) SetCtrlVal(panel,HP_LED_E,1);
             else if(hp.ew== 1) SetCtrlVal(panel,HP_LED_W,1);
             else if(hp.ew== 0){SetCtrlVal(panel,HP_LED_E,0);
                                SetCtrlVal(panel,HP_LED_W,0);}
             if     (hp.ns==-1) SetCtrlVal(panel,HP_LED_S,1);
             else if(hp.ns== 1) SetCtrlVal(panel,HP_LED_N,1);
             else if(hp.ns== 0){SetCtrlVal(panel,HP_LED_S,0);
                                SetCtrlVal(panel,HP_LED_N,0);}

             if     (hp.speed== 1)
               {SetCtrlVal(panel,HP_LED_GUIDE,1);
                SetCtrlVal(panel,HP_LED_SET  ,0);
                SetCtrlVal(panel,HP_LED_SLEW ,0);
               }
             else if(hp.speed== 2)
               {SetCtrlVal(panel,HP_LED_GUIDE,0);
                SetCtrlVal(panel,HP_LED_SET  ,1);
                SetCtrlVal(panel,HP_LED_SLEW ,0);
               }
             else if(hp.speed== 3)
               {SetCtrlVal(panel,HP_LED_GUIDE,0);
                SetCtrlVal(panel,HP_LED_SET  ,0);
                SetCtrlVal(panel,HP_LED_SLEW ,1);
               }

             if     (hp.mode== 0) SetCtrlVal(panel,HP_MODE,0); // manual
             else                 SetCtrlVal(panel,HP_MODE,1); // auto
//           printf("mode %2d\r",hp.mode);


             if     (hp.motor== 1)
               {SetCtrlVal(panel,HP_LED_ALT  ,1); // ALT/AZ
                SetCtrlVal(panel,HP_LED_DER  ,0); // Derotator               
                SetCtrlVal(panel,HP_LED_FOCUS,0); // Focus
               }
             else if(hp.motor== 2)
               {SetCtrlVal(panel,HP_LED_ALT  ,0); // ALT/AZ
                SetCtrlVal(panel,HP_LED_DER  ,1); // Derotator               
                SetCtrlVal(panel,HP_LED_FOCUS,0); // Focus
               }
             else if(hp.motor== 3)
               {SetCtrlVal(panel,HP_LED_ALT  ,0); // ALT/AZ
                SetCtrlVal(panel,HP_LED_DER  ,0); // Derotator               
                SetCtrlVal(panel,HP_LED_FOCUS,1); // Focus
               }
                               
//          do_guide_hp(0);                               
			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_HP(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
	switch (event) {
		case EVENT_COMMIT:
             if(control==HP_CLOSE) DiscardPanel(hp_test_handle);
			break;
	}
	return 0;
}
// --------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int adc_test()
// --------------------------------------------------------------------------
{
    adc_handle = LoadPanel(0,uir_file,ADC);
    DisplayPanel(adc_handle);      
//  turn_on_timer(adc_handle,ADC_TIMER);
    SetCtrlVal(adc_handle,ADC_ON_OFF,0);
    return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_ADC (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 int state;
 double time;
 int year,month,day;
 
 
	switch (event) {
		case EVENT_COMMIT:
		     if(control==ADC_ON_OFF)
		       {
		        GetCtrlVal(adc_handle,control,&state);
		        if(state==1)
		          {turn_on_timer (adc_handle, ADC_TIMER);
		           adc_out = fopen("adc.out","a");
                   GetSystemDate(&month,&day,&year);		           
		           fprintf(adc_out,"\n %4d %2d %2d\n",year,month,day);
		          }
		        else
		          {turn_off_timer(adc_handle, ADC_TIMER);
		           fclose(adc_out);
		          }
               }
               
		     else if(control==ADC_TIME)
		       {GetCtrlVal(adc_handle,ADC_TIME,&time);
		        set_timer_interval(adc_handle,ADC_TIME,time);
		       }
		     
		     else if(control==ADC_DATA)  adc_analysis();

		     else if(control==ADC_CLOSE) DiscardPanel(adc_handle);
		       
			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_ADC_TIMER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 int i,nsample,bit,ichan;
 char channel;
 word adc_value;
 char buff[20];
 double volt[100],volt_mean; 
 int hour,minute,second;
 double data[3];
 
	switch (event) {
		case EVENT_TIMER_TICK:
		        GetCtrlVal(adc_handle,ADC_BIT,&bit);
		        GetCtrlVal(adc_handle,ADC_CHANNEL,&ichan);
		        channel= ichan + 'A';

		        volt_mean = 0.0;
		        nsample = 30;
		        for(i=0;i<nsample;i++)
		           {
                    read_adc(channel,bit,&adc_value);
                    if(bit==8) volt[i] = 5.0 * adc_value / 0xff;
                    else       volt[i] = 0.004782*adc_value  - 0.003;
                    
                    volt_mean += volt[i];
                   }
                volt_mean /= nsample;
                
                sprintf(buff,"%4d %6.3lf volt",adc_value, volt_mean);
                SetCtrlVal(adc_handle,ADC_VALUE,buff);

//              report output to file
                GetSystemTime (&hour,&minute,&second);
                fprintf(adc_out,"%s %3d %2d %2d\n",buff,hour,minute,second);

                for(i=0;i<3;i++) data[i] = volt_mean;
                
//              PlotStripChart (adc_handle, ADC_CHART, volt,nsample, 0, 0, VAL_DOUBLE);
                PlotStripChart (adc_handle, ADC_CHART, data,3, 0, 0, VAL_DOUBLE);
                
			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int adc_analysis()
// --------------------------------------------------------------------------
{
 double l1,l2;
 double adc_value[1000],volt[1000],res[1000],slope,offset,use;
 int ndata,i;
 char buff[100];
 FILE *in_file;
 
 
    if( (in_file = fopen("ADC_TEST.DAT","r"))==NULL)
      {MessagePopup("","ADC_TES.DAT does not exist");
       return -1;
      }
		     
	ndata = 0;
	fgets(buff,90,in_file);
	
	fgets (buff,90,in_file);
    sscanf(buff,"%d",&ndata);
	
    for(i=0;i<ndata;i++)
       {
	    fgets (buff,90,in_file);
        sscanf(buff,"%lf %lf",&adc_value[i],&volt[i]);
       }
             
    fclose(in_file);
            
    linear_fit(volt,adc_value,ndata,&slope,&offset,res,&use);
    printf (" volt   =  adc_value * %10.6lf %10.3lf  use = %10.3lf\n",slope,offset,use);
    
    XYGraphPopup ("X: ADC  Y:volt",adc_value,volt,ndata,VAL_DOUBLE,VAL_DOUBLE);
    XYGraphPopup ("Residual      ",adc_value,res ,ndata,VAL_DOUBLE,VAL_DOUBLE);
/*
    EC_FILE = fopen("EC_TEST.OUT","w");
    for(i=0;i<ndata;i++)
       fprintf(EC_FILE,"%4d %10.1lf %10.1lf %10.1lf\n",i,counter[i],encoder[i],res[i]);
    fprintf(EC_FILE,"slope %10.5lf  use %10.1lf\n",para[0],use);   
    fclose(EC_FILE);   
*/    

	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK UserOut (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{ 
 int status, iaxis;
 char caxis;
 
	switch (event)
		{
 		 case EVENT_COMMIT:
		
		  GetCtrlVal(panel,control,&status);
		  GetCtrlVal(panel,MAIN_UAXIS,&iaxis);
		  caxis = iaxis + 'A';
		  
		  if     (control==MAIN_UO_0) set_userout_bit(caxis,0,status);
		  else if(control==MAIN_UO_1) set_userout_bit(caxis,1,status);
		  else if(control==MAIN_UO_2) set_userout_bit(caxis,2,status);   
		  else if(control==MAIN_UO_3) set_userout_bit(caxis,3,status);   
		  else if(control==MAIN_UO_4) set_userout_bit(caxis,4,status);   
		  else if(control==MAIN_UO_5) set_userout_bit(caxis,5,status);   
		  else if(control==MAIN_UO_6) set_userout_bit(caxis,6,status); 
		  else if(control==MAIN_UO_7) set_userout_bit(caxis,7,status);   
		  else if(control==MAIN_ALLH) {set_alluo(caxis,1);
		                               SetCtrlVal(panel_handle,MAIN_ALLH,1);
		                               SetCtrlVal(panel_handle,MAIN_ALLL,0);
		                              }
		  else if(control==MAIN_ALLL) {set_alluo(caxis,0);
		                               SetCtrlVal(panel_handle,MAIN_ALLL,1);
		                               SetCtrlVal(panel_handle,MAIN_ALLH,0);
		                              }
		  
		  else if(control==MAIN_I1_0) set_ioport1_bit(0,status);
		  else if(control==MAIN_I1_1) set_ioport1_bit(1,status);
		  else if(control==MAIN_I1_2) set_ioport1_bit(2,status);
		  else if(control==MAIN_I1_3) set_ioport1_bit(3,status);
		  else if(control==MAIN_I1_4) set_ioport1_bit(4,status);
		  else if(control==MAIN_I1_5) set_ioport1_bit(5,status);
		  else if(control==MAIN_I1_6) set_ioport1_bit(6,status);
		  else if(control==MAIN_I1_7) set_ioport1_bit(7,status);
		  
		break;
		}
	return 0;
}
// --------------------------------------------------------------------------
int show_user_led() 
// --------------------------------------------------------------------------
{
 int status, iaxis;
 char caxis;
 
    GetCtrlVal(panel_handle,MAIN_UAXIS,&iaxis);
    caxis = iaxis + 'A';
    
    read_userinput_bit(caxis,0,&status); SetCtrlVal(panel_handle,MAIN_UI_0,status);
    read_userinput_bit(caxis,1,&status); SetCtrlVal(panel_handle,MAIN_UI_1,status);
    read_userinput_bit(caxis,2,&status); SetCtrlVal(panel_handle,MAIN_UI_2,status);
    read_userinput_bit(caxis,3,&status); SetCtrlVal(panel_handle,MAIN_UI_3,status);
    read_userinput_bit(caxis,4,&status); SetCtrlVal(panel_handle,MAIN_UI_4,status);
    read_userinput_bit(caxis,5,&status); SetCtrlVal(panel_handle,MAIN_UI_5,status);
    read_userinput_bit(caxis,6,&status); SetCtrlVal(panel_handle,MAIN_UI_6,status);
    read_userinput_bit(caxis,7,&status); SetCtrlVal(panel_handle,MAIN_UI_7,status);
      
	return 0;
}
// --------------------------------------------------------------------------
static int display_ioport0() 
// --------------------------------------------------------------------------
{
 int status;
    
    read_ioport0_bit(0,&status); SetCtrlVal(panel_handle,MAIN_I0_0,status);
    read_ioport0_bit(1,&status); SetCtrlVal(panel_handle,MAIN_I0_1,status);
    read_ioport0_bit(2,&status); SetCtrlVal(panel_handle,MAIN_I0_2,status);
    read_ioport0_bit(3,&status); SetCtrlVal(panel_handle,MAIN_I0_3,status);
    read_ioport0_bit(4,&status); SetCtrlVal(panel_handle,MAIN_I0_4,status);
    read_ioport0_bit(5,&status); SetCtrlVal(panel_handle,MAIN_I0_5,status);
    read_ioport0_bit(6,&status); SetCtrlVal(panel_handle,MAIN_I0_6,status);
    read_ioport0_bit(7,&status); SetCtrlVal(panel_handle,MAIN_I0_7,status);
      
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK PWM_APPLY (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 unsigned char value;
 int status;
 
	switch (event) {
		case EVENT_COMMIT:
		
		     if     (control==PWMP_INIT) {init_pwm('A'); init_pwm('B'); init_pwm('C');}
		     else if(control==PWMP_PWM0) {GetCtrlVal(pwm_handle,PWMP_PWM0,&value); set_pwm('A',value);}
		     else if(control==PWMP_PWM1) {GetCtrlVal(pwm_handle,PWMP_PWM1,&value); set_pwm('B',value);}
		     else if(control==PWMP_PWM2) {GetCtrlVal(pwm_handle,PWMP_PWM2,&value); set_pwm('C',value);}
		     else if(control==PWMP_ONOFF)
		       {GetCtrlVal(panel,control,&status);
		        if(status==1) set_ioport1_bit(7,0);
		        else          set_ioport1_bit(7,1);
		       }
		     else if(control==PWMP_CLOSE) DiscardPanel(pwm_handle);

			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int pwm_test()
// --------------------------------------------------------------------------
{
    if( set_pwm('B',50) !=0 ) MessagePopup("","error in set_pwm");
    pwm_handle = LoadPanel(0,uir_file,PWMP);
    set_on_color(pwm_handle,PWMP_ONOFF);
    DisplayPanel(pwm_handle);      
    return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK TEST_TIMER_CB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 static int hour,minute,second;
 static double t0_com,t0_tcs,t0_timer;
 static double dt_com,dt_tcs,dt_timer;
 static long counts;
 int Month,Day,Year;
 
	switch (event)
		{
		case EVENT_TIMER_TICK:
		     if(ICYCLE == 0)
		       {
		        t0_timer = Timer();
		        
		        read_counter('A',&counts);
		        t0_tcs = counts/1000.0;
		        
                GetSystemDate (&Month,&Day,&Year);
                GetSystemTime (&hour,&minute,&second);
                t0_com = hour*3600.0 + minute*60.0 + second;
                
                fprintf(adc_out,"TEST BEGIN : %d %d %d  %dh %dm %ds\n",
                        Year,Month,Day, hour,minute,second);
               }
             else
               {
                dt_timer = Timer() - t0_timer;
                
		        read_counter('A',&counts);
		        dt_tcs = counts/1000.0 - t0_tcs;
		        
                GetSystemTime (&hour,&minute,&second);
                dt_com = hour*3600.0 + minute*60.0 + second - t0_com;
                if(dt_com<0) dt_com += 86400;
                
                printf("%4d %10.4lf %10.4lf %10.4lf\n",ICYCLE, dt_com, dt_timer, dt_tcs);
                fprintf(adc_out,"%4d %10.3lf %10.3lf %10.3lf\n",ICYCLE, dt_com, dt_timer, dt_tcs);
               }
               
              ICYCLE++;
                
			break;
		}
	return 0;
}
// --------------------------------------------------------------------------
static int clock_test_analysis()
// --------------------------------------------------------------------------
{
 double l1,l2;
 double com_t[1000], timer_t[1000],tcs_t[1000],res[1000],slope,offset,use;
 int ndata,i;
 char buff[100];
 FILE *in_file;
 
 
    if( (in_file = fopen("CLOCK_TEST.DAT","r"))==NULL)
      {MessagePopup("","ADC_TES.DAT does not exist");
       return -1;
      }
		     
	ndata = 0;
	fgets(buff,90,in_file);
	
    while( fgets (buff,90,in_file) != NULL )
         {
          ndata++;
          sscanf(buff,"%d %lf %lf %lf",&i,&com_t[ndata-1],&timer_t[ndata-1],&tcs_t[ndata-1]);
          if(com_t[ndata-1] < 0) com_t[ndata-1] += 86400;
       }
             
    fclose(in_file);
            
    linear_fit(tcs_t,timer_t,ndata,&slope,&offset,res,&use);
//  linear_fit(tcs_t,com_t  ,ndata,&slope,&offset,res,&use);
    printf (" TCS_TIME =  COM_TIME * %10.6lf + %10.3lf  use = %10.3lf\n",slope,offset,use);
    
    XYGraphPopup ("X: TIMER  Y:TCS",timer_t,tcs_t,ndata,VAL_DOUBLE,VAL_DOUBLE);
    XYGraphPopup ("Residual       ",timer_t,res  ,ndata,VAL_DOUBLE,VAL_DOUBLE);

	return 0;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int CVICALLBACK CLOCK_CB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
// --------------------------------------------------------------------------
{
 //long encoder,delta;
 int i,status;
 int year,month,date,hour,minute,second;
 char string[30];
 int day = 7;
 
	switch (event) {
		case EVENT_COMMIT:

		     if(control==CLOCK_ONOFF)
		       {
		        GetCtrlVal(clock_handle,control,&i);
		        if(i==1) turn_on_timer (panel,CLOCK_TIMER);
		        else     turn_off_timer(panel,CLOCK_TIMER);
		       }
		       
            else if(control==CLOCK_SET || control==CLOCK_STRING)
                 {
                  GetCtrlVal(panel,CLOCK_STRING,string);
                  sscanf(string,"%d %d %d %d %d %d",&year,&month,&date,&hour,&minute,&second);
                  set_rtc_date_time(year,month,date,hour,minute,second,day);
                 }
		       
		        DiscardPanel(clock_handle);
		        close_stdio();
			break;
	}
	return 0;
}
// --------------------------------------------------------------------------
int CVICALLBACK CB_RTCTIMER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//-------------------------------------------------------------------------------
{
 int year,month,date,hour,minute,second,day;
 char buffer[40];
 
	switch (event)
		{
         case EVENT_TIMER_TICK:
              read_rtc_date_time(&year,&month,&date,&hour,&minute,&second, &day);
//              sprintf(buffer," %4d %2d %2d  %2dh %2dm %2ds  %2d",
//                      year,month,date,hour,minute,second,day);

              sprintf(buffer," %4d %2d %2d  %2dh %2dm %2ds",
                      year,month,date,hour,minute,second);

              ResetTextBox(panel,CLOCK_STRING,buffer);
              
              break;
		}
	return 0;
}
//-------------------------------------------------------------------------------
int set_rtc_int(int i)
//-------------------------------------------------------------------------------
{
 char string[10];

    if(i==0) string[0] = '0';
    else     string[0] = '1';
    
    string[1] = NULL;

    return send_command("TI",'A',string);
}
//-------------------------------------------------------------------------------
static void display_message(char buff[])
//------------------------------------------------------------------------
{
    SetCtrlVal(panel_handle,MAIN_MESSAGE,"\n");
    SetCtrlVal(panel_handle,MAIN_MESSAGE,buff);
}
//------------------------------------------------------------------------
int CVICALLBACK RAMPSEND (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
//------------------------------------------------------------------------
{
 double speed;
 
	switch (event)
		{
		case EVENT_COMMIT:
		
		   if(control==CHANRAMP_APPLYA)
		     {
              SetWaitCursor(1);
		      GetCtrlVal(ramp_handle,CHANRAMP_PEAKA,&RAMP[0].f_max );
		      GetCtrlVal(ramp_handle,CHANRAMP_DTA  ,&RAMP[0].rise_t);
              calc_ramp('A'); send_table(COM_PORT,'A');
              SetWaitCursor(0);
             }
             
		   else if(control==CHANRAMP_APPLYB)
		     {
              SetWaitCursor(1);
		      GetCtrlVal(ramp_handle,CHANRAMP_PEAKA,&RAMP[1].f_max );
		      GetCtrlVal(ramp_handle,CHANRAMP_DTA  ,&RAMP[1].rise_t);
              calc_ramp('B'); send_table(COM_PORT,'B');
              SetWaitCursor(0);
             }
             

			break;
		}
	return 0;
}
//------------------------------------------------------------------------
int CVICALLBACK CLOSE_CHANRAMP (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
//------------------------------------------------------------------------
{
	switch (event)
		{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
		     DiscardPanel(ramp_handle);

			break;
		}
	return 0;
}
//------------------------------------------------------------------------
void display_ramp_panel()
//------------------------------------------------------------------------
{
    ramp_handle = LoadPanel (0,uir_file,CHANRAMP);
    DisplayPanel(ramp_handle);
    SetCtrlVal(ramp_handle,CHANRAMP_PEAKA,RAMP[0].f_max);
    SetCtrlVal(ramp_handle,CHANRAMP_PEAKB,RAMP[1].f_max);
    SetCtrlVal(ramp_handle,CHANRAMP_DTA  ,RAMP[0].rise_t);
    SetCtrlVal(ramp_handle,CHANRAMP_DTB  ,RAMP[1].rise_t);
}
//------------------------------------------------------------------------
int set_alluo(char caxis,int state)
//------------------------------------------------------------------------
{
    if(state==1)  
      {set_userout(caxis,255);}
    else
      {set_userout(caxis,  0);}
      
    SetCtrlVal(panel_handle,MAIN_UO_0,state);
    SetCtrlVal(panel_handle,MAIN_UO_1,state);
    SetCtrlVal(panel_handle,MAIN_UO_2,state);
    SetCtrlVal(panel_handle,MAIN_UO_3,state);
    SetCtrlVal(panel_handle,MAIN_UO_4,state);
    SetCtrlVal(panel_handle,MAIN_UO_5,state);
    SetCtrlVal(panel_handle,MAIN_UO_6,state);
    SetCtrlVal(panel_handle,MAIN_UO_7,state);
    
    return 0;
}    
//------------------------------------------------------------------------
int io_test()
//------------------------------------------------------------------------
{
 unsigned char out_val,inp_val;
 int i;

    for(i=0;i<256;i++)
       {
        out_val = i;
        set_userout   ('A',out_val);
        read_userinput('A',&inp_val);
        
        sprintf(BUFFER,"output %4d input %4d\n",out_val,inp_val);
        display_message(BUFFER);
        if(inp_val!=out_val) 
          {
           MessagePopup("error in io_test",BUFFER);
           break;
          }
       }
       
    return 0;
}
//------------------------------------------------------------------------
