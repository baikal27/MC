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
 
//  ���̺귯���� �ʱ�ȭ �Ѵ�.
    Code = AxlOpen();

	if(Code == AXT_RT_SUCCESS)
	   {  
		show_message("���̺귯�� �ʱ�ȭ �Ǿ����ϴ�.\n");            
		detect_module();
	   }   
	else
	   {	
        show_message("���̺귯�� �ʱ�ȭ ���� ���޽��ϴ�.\n"); 	  
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
// ��Ǹ���� �ִ��� �˻�
 		
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
      
    sprintf(buffer,"��Ǹ���� ���� �մϴ�.\n");
    show_message(buffer);
			
//  �ý��ۿ� ������ �� ������ Ȯ��
//	long lpAxisCount;
	AxmInfoGetAxisCount(&lpAxisCount);
	sprintf(buffer,"�ý��ۿ� ������ �� ���� : %d \n",lpAxisCount);    
	show_message(buffer);
			
//  0�� ���� ����ȣ, �����ġ, ��� ID�� Ȯ���Ѵ�
//	long lNodeNo, lModulePos;
//	long lAxisNo = 0;
//	DWORD dwModuleID;
	lAxisNo = 0;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d�� ���� ����ȣ : %x, �����ġ : %x, ��� ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
			 
	lAxisNo = 1;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d�� ���� ����ȣ : %x, �����ġ : %x, ��� ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
			 
	lAxisNo = 2;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d�� ���� ����ȣ : %x, �����ġ : %x, ��� ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
			 
	lAxisNo = 3;
	AxmInfoGetAxis(lAxisNo, &lpNodeNo, &lModulePos, &dwModuleID);
	sprintf(buffer,"%d�� ���� ����ȣ : %x, �����ġ : %x, ��� ID : %x\n",lAxisNo,lpNodeNo,lModulePos,dwModuleID); 
	show_message(buffer);
		
//  0�� ����� ?�� ��⿡�� ���� �� ��ȣ�� Ȯ���Ѵ�.
//	long lFirstAxisNo;
	lModulePos = 1;
	AxmInfoGetFirstAxisNo(lNodeNum, lModulePos, &lFirstAxisNo);
	sprintf(buffer,"%d�� ����� %d��° ����� ���� ���ȣ : %d\n",lNodeNum,lModulePos,lFirstAxisNo);
	show_message(buffer);

	lModulePos = 2;
	AxmInfoGetFirstAxisNo(lNodeNum, lModulePos, &lFirstAxisNo);
	sprintf(buffer,"%d�� ����� %d��° ����� ���� ���ȣ : %d\n",lNodeNum,lModulePos,lFirstAxisNo);
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
	
// DIO ����� ����� ���� Ȯ��
 return 0;		  
}
//-------------------------------------------------------------------------------
char CloseDevice()
//-------------------------------------------------------------------------------
{

// ���� ���̺귯���� ��밡������ (�ʱ�ȭ�� �Ǿ�����)�� Ȯ�� �Ѵ�.
	if(AxlIsOpened())
	 {									
//    ���̺귯���� �ݴ´�.
      AxlClose();
      MessagePopup("info","\n ���̺귯�� �� Motion��� ���� �Ͽ����ϴ�.\n");
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
// ��� ����� ���� �ϴ��� Ȯ��
 
    iaxis = laxis;
    laxis = NODENO*2+laxis;
	
    mode = AxmInfoIsMotionModule (&dwStatus);
	if(mode!=AXT_RT_SUCCESS) return -1;
	if (dwStatus!= STATUS_EXIST) return -2;
//  MessagePopup("info", "Motion ����� ���� �մϴ�.\n");
//  show_message("Set Axis parameter\n"); 

//  �޽� ��¸�� ���� : 2�޽� ���
//    OneHighLowHigh          = 0x0   // 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
//    OneHighHighLow          = 0x1   // 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
//    OneLowLowHigh           = 0x2   // 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
//    OneLowHighLow           = 0x3   // 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
//    TwoCcwCwHigh            = 0x4   // 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High     
//    TwoCcwCwLow             = 0x5   // 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low     
//    TwoCwCcwHigh            = 0x6   // 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
//    TwoCwCcwLow             = 0x7   // 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low
//    TwoPhase                = 0x8   // 2��(90' ������),  PULSE lead DIR(CW: ������), PULSE lag DIR(CCW:������)
//    TwoPhaseReverse         = 0x9   // 2��(90' ������),  PULSE lead DIR(CCW: ������), PULSE lag DIR(CW:������)
   	AxmMotSetPulseOutMethod(laxis,OneHighLowHigh);

//  Home signal	level
//	AxmHomeSetSignalLevel (laxis,HIGH);
	AxmHomeSetSignalLevel (laxis,SWSTATUS[iaxis].home);

//  Inposition ��� ���� : ������� ����
	AxmSignalSetInpos(laxis,UNUSED);
//  Alarm ��� ���� : ������� ����
	AxmSignalSetServoAlarm(laxis, UNUSED);  
//  Servo On ��ȣ�� ������ HIGH�� �Ѵ�.
	AxmSignalSetServoOnLevel(laxis, HIGH);
//  Servo ����̹��� ��ȣ�� ON �Ѵ�.
//	AxmSignalServoOn(laxis, ENABLE);
	
//  set relative
//  set_axis_relative(laxis);
    AxmMotSetAbsRelMode (laxis,1);
//  1�޽��� �̵��Ÿ�  
   	AxmMotSetMoveUnitPerPulse(laxis, 1, 1);

//  ������ ���ζ����� ��Ī ��ٸ��÷� �Ѵ�.
	AxmMotSetProfileMode(laxis,SYM_TRAPEZOIDE_MODE);
//ProfileMode : SYM_TRAPEZOIDE_MODE    '0' - ��Ī Trapezode
//              ASYM_TRAPEZOIDE_MODE   '1' - ���Ī Trapezode
//              QUASI_S_CURVE_MODE     '2' - ��������
//              SYM_S_CURVE_MODE       '3' - ��Ī S Curve
//              ASYM_S_CURVE_MODE      '4' - ���Ī S Curve

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
	
//  ���ڴ� �Է� ����� 4ü��� ���� �Ѵ�.
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
