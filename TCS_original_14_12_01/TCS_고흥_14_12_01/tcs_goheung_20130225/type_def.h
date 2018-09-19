#ifndef _type_def_h
#define _type_def_h

typedef unsigned char byte;
typedef unsigned char Byte;
typedef unsigned char BYTE;
typedef unsigned int  word;
typedef unsigned int  Word;

//typedef struct H_M_S
//        {int sign; int h; int m; double s; double rh; double rad;} h_m_s;
//typedef struct D_M_S{
//	int sign; int d; int m; double s; double rd; double rad;} d_m_s;
	
#define MAX_NSTAIR      150

typedef struct RAMP_DATA
        {
         double	fmin;   // base clk
         double	fmax;   // max clk
         double	time;	// rise time
         double acce;
         
    	 double	f_base;                // base clk
         double	f_max;                 // max clk
         double	f_clk;                 // ref clk
         double	rise_t;	               // rise time
         unsigned int nstairs;         // number of the stairs
         double	     freq[MAX_NSTAIR]; // real frequency
         unsigned int div[MAX_NSTAIR]; // divisor
         long         sum[MAX_NSTAIR]; // sum of the pulses
         unsigned int np [MAX_NSTAIR]; // number of the pulses
        } ramp_data;


typedef struct AXIS_DATA
	  {long int     cou;  // counter
	   long int     enc;  // endcoder
	   double       spe;  // speed
       unsigned int dev;  // devision factor
	   byte         dir;  // direction
	   byte         lsw;  // limit sw
	   byte         hsw;  // home  sw
	   byte         wsw;  // warn  sw
	   byte         psw;  // power sw
	   byte         gat;  // Motor on/off status
	  }axis_data;
	  
typedef  struct switchstatus {int cw,ccw,home,gate;} switch_status;

typedef struct HOME_PARA
	  {long     dir;  
	   DWORD    sw;   
	   DWORD    z;
	   double   offset;
	   double   ctime;
	   double   v1,v2,v3,v4,a1,a2;
	  } home_para;
	  
//  DWORD    __stdcall AxmHomeSetVel(long lAxisNo, 
//double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
	  

//---------------------------------------------------------------------------
// hand paddle
//---------------------------------------------------------------------------
typedef  struct HHHH {int ew; int ns; int speed; int mode; int motor;} hp_status;
//---------------------------------------------------------------------------
//  ew_dir     : -1 =>E , 1 => W, 0 => None
//  ns_dir     : -1 =>S , 1 => N, 0 => None
//  speed_mode :  1 => guide, 2 => set, 3 => slew
//  mode       :  0 => computer. 1 => hand paddle
//  motor      :  1 => ALT/AZ, 2 => Derotator, 3 => focus
//---------------------------------------------------------------------------

#endif
