//-----------------------------------------------------------------------------
#include <userint.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>

#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>
#include <ansi_c.h> 
#include <utility.h>     

#include "AXL.h"
#include "AXM.h"
#include "AXD.h"
#include "type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "cvi_util.h"
#include "gv_tel.h"
extern void show_message(char message[]);
//-----------------------------------------------------------------------------
DWORD read_counter   (char caxis, long *lcounts);
DWORD read_encoder   (char caxis, long *lcounts);
DWORD read_cw_limit  (char caxis, int  *state);
DWORD read_ccw_limit (char caxis, int  *state);
DWORD read_home_limit(char caxis, int  *state);
DWORD read_axis_all  (char caxis, double *pcounts, double *ecounts, int *cw,int *home,int *ccw);

DWORD read_dir       (char caxis, int *pbDir);
DWORD read_gate      (char caxis, DWORD *gate);
DWORD read_slew_mode (char caxis, DWORD *slew_mode);
DWORD read_speed     (char caxis, double *dpVelocity); 
DWORD read_couenc    (char caxis, long *counter, long *encoder);

DWORD set_counter    (char caxis, long lParameter);
DWORD set_encoder    (char caxis, long lParameter);
DWORD slew_stop      (char caxis, long dPos);
DWORD motor_sstop    (char caxis);
DWORD motor_stop     (char caxis);

DWORD go_const       (char caxis, double speed);

DWORD read_cpuinput_bit   (long ibit, DWORD *bit_value);
DWORD write_cpuoutput_bit (long ibit, DWORD  bit_value);
//DWORD set_cpuouput_bitmode(long bit, DWORD uLevel); 

DWORD write_m2qouput_bit(long axis, long bit, DWORD  level);
DWORD read_m2qouput_bit (long axis, long bit, DWORD *level);
//===================================================================
int set_home_search_para(void);
DWORD set_home_para(long laxis, home_para home);

//  hand paddle
int read_hp_status (hp_status *hp);

DWORD preload_on (void);
DWORD preload_off(void);
//-----------------------------------------------------------------------------
static DWORD code;

//-----------------------------------------------------------------------------
DWORD read_counter(char caxis, long *lcounts)
//-----------------------------------------------------------------------------
{
 DWORD code;
 double dpulse;
 long laxis;
 double counts;

    laxis=caxis-'A'+NODENO*2;
     code=AxmStatusGetCmdPos(laxis,&counts);
     *lcounts=counts;
     return code;
}	
//-----------------------------------------------------------------------------
DWORD read_encoder(char caxis, long  *lcounts)
//-----------------------------------------------------------------------------
{
 double dpulse;
 long laxis;
 double counts;
    
    laxis=caxis-'A'+NODENO*2;
	code = AxmStatusGetActPos(laxis,&counts);
	*lcounts = counts;
	return code;
}	
//-----------------------------------------------------------------------------
DWORD read_couenc (char caxis, long *counter, long *encoder)
//-----------------------------------------------------------------------------
{
 DWORD code;
 long laxis;
 double dc,de;
 
    laxis=caxis-'A'+NODENO*2;
    AxmStatusGetActPos(laxis,&de);
    code= AxmStatusGetCmdPos(laxis,&dc);
    *counter = dc;
    *encoder = de;
    return code;    
}	
//-----------------------------------------------------------------------------
DWORD set_encoder (char caxis, long lcounts)
//-----------------------------------------------------------------------------
{
 long laxis;
 double counts;
 counts = lcounts;
 
    laxis=caxis-'A'+NODENO*2;
 return AxmStatusSetActPos(laxis,counts);
}
//-----------------------------------------------------------------------------
DWORD set_counter(char caxis, long lcounts)
//-----------------------------------------------------------------------------
{
 long laxis;
 double counts;
 counts = lcounts;
 
    laxis=caxis-'A'+NODENO*2;
 return AxmStatusSetCmdPos(laxis, counts);
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
DWORD  read_cw_limit  (char caxis, int *state)
//-----------------------------------------------------------------------------
{
 DWORD cwstatus,ccwstatus;
 long laxis;
 
    laxis=caxis-'A'+NODENO*2;
    code=AxmSignalReadLimit (laxis,&cwstatus,&ccwstatus);
    *state = cwstatus;
	return code;
}	
//-----------------------------------------------------------------------------
DWORD  read_ccw_limit  (char caxis, int *state)
//-----------------------------------------------------------------------------
{
 DWORD cwstatus,ccwstatus;
 long laxis;
 
    laxis=caxis-'A'+NODENO*2;
    code=AxmSignalReadLimit (laxis,&cwstatus,&ccwstatus);
    *state = ccwstatus;
	return code;
}	
//-----------------------------------------------------------------------------
DWORD  read_home_limit(char caxis, int *state)
//-----------------------------------------------------------------------------
{
 DWORD status;
 long laxis;

    laxis=caxis-'A'+NODENO*2;
    code=AxmHomeReadSignal(laxis,&status);
    *state = status;
	return code;
}
//-----------------------------------------------------------------------------
DWORD  read_axis_all  (char caxis, double *pcounts, double *ecounts, int *cw,int *home,int *ccw)
//-----------------------------------------------------------------------------
{
 DWORD cwstatus,ccwstatus;
 DWORD status;
 long laxis;

    laxis=caxis-'A'+NODENO*2;
    code=AxmSignalReadLimit (laxis,&cwstatus,&ccwstatus);
    *cw  = cwstatus;
    *ccw = ccwstatus;
    
    code=AxmHomeReadSignal(laxis,&status);
    *home = status;
    
	code = AxmStatusGetActPos(laxis,ecounts);
    code = AxmStatusGetCmdPos(laxis,pcounts);
    
	return code;
}
//-----------------------------------------------------------------------------
DWORD read_dir(char caxis, int *dir)
//-----------------------------------------------------------------------------
{
 DWORD status;
 long laxis;
 
    laxis=caxis-'A'+NODENO*2;
    code = AxmStatusReadMotion(laxis,&status);    
    *dir = status >> 11 & 0x01;
    
    if(*dir==0) *dir = 1;
    else        *dir = 0;
    
    return code;
}
//-----------------------------------------------------------------------------
DWORD read_gate(char caxis, DWORD *gate)
//-----------------------------------------------------------------------------
{
 long laxis;
 
    laxis=caxis-'A'+NODENO*2;
    return AxmStatusReadInMotion (laxis,gate);    
}
//-----------------------------------------------------------------------------
DWORD read_slew_mode(char caxis, DWORD *mode)
//-----------------------------------------------------------------------------
{
 int shift,ibit;
 DWORD status;
 long laxis;

    laxis=caxis-'A'+NODENO*2;
    code=AxmStatusReadMotion(laxis,&status); 
    *mode = 0;

//   deceleration
     ibit = status >> 1 & 0x01;
     if(ibit==1) *mode = 3;

//   const     
     ibit = status >> 2 & 0x01;
     if(ibit==1) *mode = 2;
     
//   acceleration
     ibit = status >> 3 & 0x01;
     if(ibit==1) *mode = 1;

     return code;
}
//-----------------------------------------------------------------------------
DWORD read_speed(char caxis, double *dpVelocity) 
//-----------------------------------------------------------------------------
{
 long laxis;

    laxis=caxis-'A'+NODENO*2;
    return AxmStatusReadVel(laxis,dpVelocity);
}
//-----------------------------------------------------------------------------
DWORD motor_estop  (char caxis)
//-----------------------------------------------------------------------------
{
 long laxis;
    laxis=caxis-'A'+NODENO*2;
    return AxmMoveEStop(laxis);
}    
//-----------------------------------------------------------------------------
DWORD motor_stop  (char caxis)
//-----------------------------------------------------------------------------
{
 long laxis;
    laxis=caxis-'A'+NODENO*2;
    return AxmMoveSStop(laxis);
}    
//-----------------------------------------------------------------------------
DWORD go_const(char caxis, double velocity)
//-----------------------------------------------------------------------------
{
 DWORD mode;
 long laxis,iaxis;
 int dir,state;
 
    laxis=caxis-'A'+NODENO*2;
    iaxis=caxis-'A';
 
//  AxmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel);

//      state=AxmOverrideSetMaxVel(laxis,VPROFILE[iaxis].fmax*1.1);
//      state=AxmOverrideSetMaxVel(laxis,fabs(velocity)*1.1);
//	  if(state!=0) MessagePopup("error","Go const");
//    AxmOverrideSetMaxVel(laxis,fabs(velocity)*1.1);

    read_gate(caxis,&mode);     
    if(mode==0) return AxmMoveVel(laxis,velocity,VPROFILE[iaxis].acce,VPROFILE[iaxis].acce);
	
	else if(mode==3) return 0;
    
    read_dir(caxis,&dir);
    
    if( (dir==1 && velocity>0) || (dir==0 && velocity<0) )
//      if( (dir==1 && velocity>0))
      {
       code = AxmOverrideVel(laxis,velocity);  // the same direction
//     if(code!=0)  MessagePopup(" ","error Axmoverride");
       return code;
      }
    else 
      {
//     MessagePopup(" ","direction is changed");
       return motor_stop(caxis);
      }
}
//-----------------------------------------------------------------------------
DWORD slew_stop(char caxis, long lpos)
//-----------------------------------------------------------------------------
{
 long laxis,iaxis;
 double dPos;
 
    iaxis=caxis-'A';
    laxis=caxis-'A'+NODENO*2;
    dPos=lpos;
	
 return AxmMoveStartPos(laxis, dPos, VPROFILE[iaxis].fmax,VPROFILE[iaxis].acce,VPROFILE[iaxis].acce);
}
//-----------------------------------------------------------------------------
DWORD write_cpuoutput_bit(long  ibit, DWORD bit_value)
//-----------------------------------------------------------------------------------------------
{
    return  AxdoWriteOutportBit(NODENO, ibit, bit_value);
}
//-----------------------------------------------------------------------------------------------
DWORD read_cpuinput_bit(long ibit, DWORD *bit_value)
//-----------------------------------------------------------------------------------------------
{
    return AxdiReadInportBit(NODENO, ibit, bit_value);  
} 
//-----------------------------------------------------------------------------------------------
DWORD set_cpuouput_bitmode(long bit, DWORD uLevel)  
//-----------------------------------------------------------------------------------------------
{
    return AxdoLevelSetOutport(bit, uLevel);  
}    
//-----------------------------------------------------------------------------------------------
DWORD write_m2qouput_bit(long axis, long bit, DWORD level)  
//-----------------------------------------------------------------------------------------------
{
    return AxmSignalWriteOutputBit(axis, bit, level);
}    
//-----------------------------------------------------------------------------------------------
DWORD read_m2qouput_bit(long axis, long bit, DWORD *level)  
//-----------------------------------------------------------------------------------------------
{
    return AxmSignalReadInputBit(axis, bit, level);
}
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
int set_home_search_para()
//-----------------------------------------------------------------------------------------------
{
 char buff[300];
 double v1,v2,v3,v4,a1,a2;
 int i;
// 원점검색 방법을 설정하는 함수
//long laxis =  0; //원점 검색이 이루어질 축 번호
//long nHmDir  = -1; //원점검색 1단계의 이동방향, (-)방향을 입력
//DWORD uHomeSignal = HomeSensor; //신호검색구동에서의 신호들을 사용
//DWORD uZphas = 0;               // Z상 검출 유무, 1이면 검출 하고, 0이면 안함
//double dHomeClrTime =      0.0; // 원점 검색 Enc 값 Set하기 위한 대기시간. mili sec
//double dHomeOffset  =      0.0; // 원점검색후 Offset값만큼 추가적으로 이동
    
    for(i=0;i<2;i++)
       {	
        HOMEP[i].dir   = 1; 
        HOMEP[i].sw    = HomeSensor; 
        HOMEP[i].z     = 0;  
        HOMEP[i].ctime = 0;  
        HOMEP[i].offset= 0;  

// 원점검색시 사용될 속도를 설정
        HOMEP[i].v1 = SLEW_FREQ[i];  //1단계에서의 속도
        HOMEP[i].a1 = VPROFILE [i].acce;  //1단계에서의 가속도
 /*
 HOMEP[0].v2 = 200.0;  //2단계에서의 속도
 HOMEP[0].v3 = 200.0;  //3단계에서의 속도
 HOMEP[0].v4 = 200.0;  // index검색및 정밀하게 검색하기위한 속도 (Offset값 이동시 적용)
 HOMEP[0].a2 =  40.0;  //2단계에서의 가속도
 */
 
        code=set_home_para(i,HOMEP[i]);
        sprintf(buff,"code=%d\n",code);
        show_message(buff);
       }	

return 0;
}
//-----------------------------------------------------------------------------------------------
DWORD set_home_para(long laxis, home_para home)
//-----------------------------------------------------------------------------------------------
{
//  해당 축의 원점검색을 수행하기 위해서는 반드시 원점 검색관련 파라메타들이 설정되어 있어야 됩니다. 
//  만약 MotionPara설정 파일을 이용해 초기화가 정상적으로 수행됐다면 별도의 설정은 필요하지 않다. 
//  원점검색 방법 설정에는 검색 진행방향, 원점으로 사용할 신호, 원점센서 Active Level, 엔코더 Z상 검출 여부 등을 설정 한다.
//  주의사항 : 레벨을 잘못 설정시 -방향으로 설정해도  +방향으로 동작할수 있으며, 홈을 찾는데 있어 문제가 될수있다.
//  (자세한 내용은 AxmMotSaveParaAll 설명 부분 참조)
//  홈레벨은 AxmSignalSetHomeLevel 사용한다.
//  HClrTim : HomeClear Time : 원점 검색 Encoder 값 Set하기 위한 대기시간 
//  HmDir(홈 방향): DIR_CCW (0) -방향 , DIR_CW(1) +방향
//  HOffset - 원점검출후 이동거리.
//  uZphas: 1차 원점검색 완료 후 엔코더 Z상 검출 유무 설정  0: 사용안함 , 1: +방향, 2: -방향 
//  HmSig:   PosEndLimit(0) -> +Limit
//           NegEndLimit(1) -> -Limit
//           HomeSensor (4) -> 원점센서(범용 입력 0)

//  DWORD   __stdcall AxmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
//  DWORD   __stdcall AxmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond);

	
	laxis += NODENO*2;
//	AxmHomeSetResult(laxis,HOME_SUCCESS); 
	AxmHomeSetResult(laxis,1); 
    code=AxmHomeSetMethod(laxis, home.dir, home.sw, home.z, home.ctime, home.offset);
    if(code!=0) 
    {
     MessagePopup("","error in home set method");
     return code;
    }
    code =AxmHomeSetVel   (laxis, home.v1, home.v2, home.v3, home.v4, home.a1, home.a2); 
    if(code!=0) MessagePopup("","error in home set vel");
    return code;
}    
//-----------------------------------------------------------------------------------------------
DWORD goto_home(long laxis, long dir)
//-----------------------------------------------------------------------------------------------
{
//	preload_off();
    HOMEP[laxis].dir = dir;
    set_home_para(laxis,HOMEP[laxis]);
    return AxmHomeSetStart(laxis);
}
//-----------------------------------------------------------------------------------------------
//    DWORD    __stdcall AxmHomeGetResult(long lAxisNo, DWORD *upHomeResult);

//-----------------------------------------------------------------------------------------------
int read_hp_status (hp_status *hp)
//-----------------------------------------------------------------------------------------------
{
 int state;
 
    hp->ew = 0;
    read_cpuinput_bit(0,&state);
    if(state==1) hp->ew = -1;
    read_cpuinput_bit(1,&state);
    if(state==1) hp->ew =  1;
    
    hp->ns = 0;
    read_cpuinput_bit(2,&state);
    if(state==1) hp->ns = -1;
    read_cpuinput_bit(3,&state);
    if(state==1) hp->ns =  1;
    
    hp->speed = 1;                //Guide
    read_cpuinput_bit(4,&state);
    if(state==1) hp->speed = 2;   //Set
    read_cpuinput_bit(5,&state);
    if(state==1) hp->speed = 3;   //Slew
    
    hp->motor = 1;                //Ra.Dec
    
/*    
    if     (NEW.ew==-1) {SetCtrlVal(panel,GUIDE_E,1); SetCtrlVal(panel,GUIDE_W,0);}
    else if(NEW.ew== 1) {SetCtrlVal(panel,GUIDE_E,0); SetCtrlVal(panel,GUIDE_W,1);}
    else if(NEW.ew== 0) {SetCtrlVal(panel,GUIDE_E,0); SetCtrlVal(panel,GUIDE_W,0);}

    if     (NEW.ns==-1) {SetCtrlVal(panel,GUIDE_S,1); SetCtrlVal(panel,GUIDE_N,0);}
    else if(NEW.ns== 1) {SetCtrlVal(panel,GUIDE_S,0); SetCtrlVal(panel,GUIDE_N,1);}
    else if(NEW.ns== 0) {SetCtrlVal(panel,GUIDE_S,0); SetCtrlVal(panel,GUIDE_N,0);}

//  motor selct switch
    if     (NEW.motor==1) {SetCtrlVal(panel,GUIDE_RING_MOTOR,0);}   // RA/DEC
    else if(NEW.motor==3) {SetCtrlVal(panel,GUIDE_RING_MOTOR,1);}   // FOCUS
                          
//  speed switch
    if     (NEW.speed==1){SetCtrlVal(panel,GUIDE_RING_SPEED  ,1);}  // GUIDE
    else if(NEW.speed==2){SetCtrlVal(panel,GUIDE_RING_SPEED  ,2);}  // SET
    else if(NEW.speed==3){SetCtrlVal(panel,GUIDE_RING_SPEED  ,3);}  // SLEW
*/
return 0;
}
//-----------------------------------------------------------------------------------------------
DWORD set_preload_on(char caxis)
//-----------------------------------------------------------------------------
{
	if(USEPRELOAD==0) return 0;
    if(caxis=='A') return write_cpuoutput_bit(8,1);
    else           return write_cpuoutput_bit(9,1);
}			  
//-----------------------------------------------------------------------------
DWORD set_preload_off(char caxis)
//-----------------------------------------------------------------------------
{
	if(USEPRELOAD==0) return 0;
    if(caxis=='A') return write_cpuoutput_bit(8,0);
    else           return write_cpuoutput_bit(9,0);
}			  
//-----------------------------------------------------------------------------
DWORD preload_on()
//-----------------------------------------------------------------------------
{
	if(USEPRELOAD==0) return 0;
    write_cpuoutput_bit(8,1);   
    return write_cpuoutput_bit(9,1);
}			  
//-----------------------------------------------------------------------------
DWORD preload_off()
//-----------------------------------------------------------------------------
{
	if(USEPRELOAD==0) return 0;
    write_cpuoutput_bit(8,0);
    return write_cpuoutput_bit(9,0);
}			  
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
DWORD get_motion_mode(long lAxisNo, DWORD *upStatus) {return AxmStatusReadInMotion(lAxisNo, upStatus);}
DWORD read_motfile(char *fname) {return AxmMotLoadParaAll(fname);}
DWORD save_motfile(char *fname) {return AxmMotSaveParaAll(fname);}
DWORD set_axis_absolute(long lAxisNo) {return AxmMotSetAbsRelMode (lAxisNo, 0);}
DWORD set_axis_relative(long lAxisNo) {return AxmMotSetAbsRelMode (lAxisNo, 1);}
//-----------------------------------------------------------------------------------------------
