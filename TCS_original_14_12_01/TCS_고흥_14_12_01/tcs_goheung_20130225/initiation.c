#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>
#include <ansi_c.h> 
#include <utility.h>    

#include "AXD.h"
#include "AXL.h"
#include "AXM.h"
#include "type_def.h"
#include "c:\lib\cal_lib\astrometry.h"
#include "gv_tel.h"
#include "st4s_lib.h"

extern void show_message(char message[]);

//extern ramp_data VPROFILE[4];
//extern switch_status SWSTATUS[4];
//-------------------------------------------------------------------------------
int  init_tcs(void); 
char CloseDevice(void);   
int  read_set_vprofile(void);
int  detect_module(void);
int  set_axis_para(long laxis);
//-------------------------------------------------------------------------------
static char buffer[200];
//-------------------------------------------------------------------------------
int init_tcs()
//-------------------------------------------------------------------------------
{
//long lCount;
DWORD Code;
 
//  라이브러리를 초기화 한다.
    Code = AxlOpen();

	if(Code == AXT_RT_SUCCESS)
	   {  
		show_message("라이브러리 초기화 되었습니다.\n");            
		detect_module();
	   }   
	else
	   {	
        show_message("라이브러리 초기화 하지 못햇습니다.\n"); 	  
        sprintf(buffer,"AxlOpen() : ERROR code  %d\n",Code);//	 0x%x\n",Code);    
        show_message(buffer);  
		
		return -1;
	   }
	
	return 0;
}	
//-------------------------------------------------------------------------------
int detect_module()
//-------------------------------------------------------------------------------
{
 DWORD dwStatus;     
 DWORD Code;
 long lpAxisCount, lpNodeNo,lpModuleCount;  
 long lNodeNo, lModulePos,lNodeNum = 0;
 long lAxisNo = 0;
 DWORD dwModuleID;
 long lFirstAxisNo;
 long lpCounts;
// 모션모듈이 있는지 검사
 		
	Code = AxmInfoIsMotionModule(&dwStatus);
    if(Code != AXT_RT_SUCCESS)
      {
	    sprintf(buffer,"AxmInfoIsMotionModule() : ERROR ( Return FALSE ) code %d\n",Code); //0x%x\n",Code); 
        show_message(buffer);
		return -1;
	   }

	if(dwStatus != STATUS_EXIST)
      {
       sprintf(buffer,"AxmInfoIsMotionModule() : ERROR ( NOT STATUS_EXIST ) code %d\n",Code);//0x%x\n",Code);       
       show_message(buffer);
	   return -2;
      }
      
    sprintf(buffer,"모션모듈이 존재 합니다.\n");
    show_message(buffer);
			
//  시스템에 장착된 축 개수를 확인
//	long lpAxisCount;
	AxmInfoGetAxisCount(&lpAxisCount);
	sprintf(buffer,"시스템에 장착된 축 개수 : %d \n",lpAxisCount);    
	show_message(buffer);
			
//  0번 축의 노드번호, 모듈위치, 모듈 ID를 확인한다
//	long lNodeNo, lModulePos;
//	long lAxisNo = 0;
//	DWORD dwModuleID;
	lAxisNo = 0;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d번 축의 노드번호 : %x, 모듈위치 : %x, 모듈 ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
			 
	lAxisNo = 1;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d번 축의 노드번호 : %x, 모듈위치 : %x, 모듈 ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
			 
	lAxisNo = 2;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d번 축의 노드번호 : %x, 모듈위치 : %x, 모듈 ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
			 
	lAxisNo = 3;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d번 축의 노드번호 : %x, 모듈위치 : %x, 모듈 ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
		
//  0번 노드의 ?번 모듈에서 시작 축 번호를 확인한다.
//	long lFirstAxisNo;
	lModulePos = 1;
	AxmInfoGetFirstAxisNo(lNodeNum, lModulePos, &lFirstAxisNo);
	sprintf(buffer,"%d번 노드의 %d번째 모듈의 시작 축번호 : %d\n",lNodeNum,lModulePos,lFirstAxisNo);
	show_message(buffer);

	lModulePos = 2;
	AxmInfoGetFirstAxisNo(lNodeNum, lModulePos, &lFirstAxisNo);
	sprintf(buffer,"%d번 노드의 %d번째 모듈의 시작 축번호 : %d\n",lNodeNum,lModulePos,lFirstAxisNo);
	show_message(buffer);
	
	Code = AxdInfoIsDIOModule(&dwStatus);
	Code = AxmInfoIsMotionModule(&dwStatus);
    if(Code != AXT_RT_SUCCESS)
      {
	    sprintf(buffer,"AxmInfoIsMotionModule() : ERROR ( Return FALSE ) code %d\n",Code); //0x%x\n",Code); 
        show_message(buffer);
		return -1;
	   }	
	
	if(dwStatus == STATUS_EXIST)
      {
       AxdInfoGetModuleCount(&lpModuleCount);
       sprintf(buffer,"DIO module %d \n",lpModuleCount);
       show_message(buffer);
	   
	   AxdInfoGetInputCount(0,&lpCounts);
       sprintf(buffer,"DIO input no %d \n",lpCounts);
       show_message(buffer);
	   
   	   AxdInfoGetOutputCount(0,&lpCounts);
       sprintf(buffer,"DIO output no %d \n",lpCounts);
       show_message(buffer);
      }
	else
      {
       sprintf(buffer,"no DIO module exists\n");
       show_message(buffer);
	  }
	
// DIO 입출력 모듈의 개수 확인
 return 0;		  
}
//-------------------------------------------------------------------------------
char CloseDevice()
//-------------------------------------------------------------------------------
{

// 통합 라이브러리가 사용가능한지 (초기화가 되었는지)를 확인 한다.
	if(AxlIsOpened())
	 {									
//    라이브러리를 닫는다.
      AxlClose();
      MessagePopup("info","\n 라이브러리 및 Motion모듈 종료 하였습니다.\n");
      return 0;
     }	
	else return    -1;
}
//-------------------------------------------------------------------------------
int set_axis()
//-------------------------------------------------------------------------------
{
    set_axis_para(0);
    set_axis_para(1);
    set_home_search_para();
 return 0;      
}    
//-------------------------------------------------------------------------------
int set_axis_para(long laxis)
//-------------------------------------------------------------------------------
{
 DWORD dwStatus, mode;
 int iaxis;
// 모션 모듈이 존재 하는지 확인
 
    iaxis = laxis;
    laxis = NODENO*2+laxis;
	
    mode = AxmInfoIsMotionModule (&dwStatus);
	if(mode!=AXT_RT_SUCCESS) return -1;
	if (dwStatus!= STATUS_EXIST) return -2;
//  MessagePopup("info", "Motion 모듈이 존재 합니다.\n");
//  show_message("Set Axis parameter\n"); 

//  펄스 출력모드 설정 : 2펄스 모드
//    OneHighLowHigh          = 0x0   // 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
//    OneHighHighLow          = 0x1   // 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
//    OneLowLowHigh           = 0x2   // 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
//    OneLowHighLow           = 0x3   // 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
//    TwoCcwCwHigh            = 0x4   // 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High     
//    TwoCcwCwLow             = 0x5   // 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low     
//    TwoCwCcwHigh            = 0x6   // 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
//    TwoCwCcwLow             = 0x7   // 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
//    TwoPhase                = 0x8   // 2상(90' 위상차),  PULSE lead DIR(CW: 정방향), PULSE lag DIR(CCW:역방향)
//    TwoPhaseReverse         = 0x9   // 2상(90' 위상차),  PULSE lead DIR(CCW: 정방향), PULSE lag DIR(CW:역방향)
   	AxmMotSetPulseOutMethod(laxis,OneHighLowHigh);

//  Home signal	level
//	AxmHomeSetSignalLevel (laxis,HIGH);
	AxmHomeSetSignalLevel (laxis,SWSTATUS[iaxis].home);

//  Inposition 사용 유무 : 사용하지 않음
	AxmSignalSetInpos(laxis,UNUSED);
//  Alarm 사용 유무 : 사용하지 않음
	AxmSignalSetServoAlarm(laxis, UNUSED);  
//  Servo On 신호의 레벨을 HIGH로 한다.
	AxmSignalSetServoOnLevel(laxis, HIGH);
//  Servo 드라이버의 신호를 ON 한다.
//	AxmSignalServoOn(laxis, ENABLE);
	
//  set relative
//  set_axis_relative(laxis);
    AxmMotSetAbsRelMode (laxis,1);
//  1펄스당 이동거리  
   	AxmMotSetMoveUnitPerPulse(laxis, 1, 1);

//  가감속 프로라일을 대칭 사다리꼴로 한다.
	AxmMotSetProfileMode(laxis,SYM_TRAPEZOIDE_MODE);
//ProfileMode : SYM_TRAPEZOIDE_MODE    '0' - 대칭 Trapezode
//              ASYM_TRAPEZOIDE_MODE   '1' - 비대칭 Trapezode
//              QUASI_S_CURVE_MODE     '2' - 지원안함
//              SYM_S_CURVE_MODE       '3' - 대칭 S Curve
//              ASYM_S_CURVE_MODE      '4' - 비대칭 S Curve

//  maximum speed
	AxmMotSetMaxVel(laxis,VPROFILE[iaxis].fmax*1.1);
	
//  Start/Stop/speed 
    AxmMotSetMinVel(laxis,VPROFILE[iaxis].fmin);            
    
//  vprifile	
    AxmOverrideSetMaxVel(laxis,VPROFILE[iaxis].fmax*1.1);
	
//  Pulse counter clear
	AxmStatusSetCmdPos(laxis,0);
	
//  Encoder counter clear
	AxmStatusSetActPos(laxis,0.0);
	
//  엔코더 입력 방식을 4체배로 설정 한다.
	AxmMotSetEncInputMethod(laxis, ObverseSqr4Mode);
    
//  home signal level    
    AxmHomeSetSignalLevel(laxis,SWSTATUS[iaxis].home);
    
//  CW, CCW level    
    AxmSignalSetLimit(laxis,EMERGENCY_STOP,SWSTATUS[iaxis].cw,SWSTATUS[iaxis].ccw);
    
//	AxmSignalSetServoAlarm(laxis, HIGH);  
//  AxdoLevelSetOutportBit(0,8,0);
		
	return 0;
}
//--------------------------------------------------------------------------
