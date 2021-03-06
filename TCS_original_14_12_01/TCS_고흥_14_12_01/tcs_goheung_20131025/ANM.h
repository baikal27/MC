/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** ANM.H
**
** COPYRIGHT (c) AJINEXTEK Co., LTD
**
*****************************************************************************
*****************************************************************************
**
** Description
** -----------
** Ajinextek Motion Library Header File
** 
**
*****************************************************************************
*****************************************************************************
**
** Source Change Indices
** ---------------------
** 
** (None)
**
**
*****************************************************************************
*****************************************************************************
**
** Website
** ---------------------
**
** http://www.ajinextek.com
**
*****************************************************************************
*****************************************************************************
*/

#ifndef __ANT_ANM_H__
#define __ANT_ANM_H__

//#include "ANHS.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

//========== 보드 및 모듈 확인함수(Info) - Infomation =================================================================================

    // 해당 축의 보드번호, 모듈 위치, 모듈 아이디를 반환한다.
    DWORD    __stdcall AnmInfoGetAxis(long lAxisNo, long *lpNodeNum, long *lpModulePos, DWORD *upModuleID);
    // 모션 모듈이 존재하는지 반환한다.
    DWORD    __stdcall AnmInfoIsMotionModule(DWORD *upStatus);
    // 해당 축이 유효한지 반환한다.
    DWORD    __stdcall AnmInfoIsInvalidAxisNo(long lAxisNo);
    // CAMC-QI 축 개수, 시스템에 장착된 유효한 모션 축수를 반환한다.
    DWORD    __stdcall AnmInfoGetAxisCount(long *lpAxisCount);
    // 해당 노드/모듈의 첫번째 축번호를 반환한다.
    DWORD    __stdcall AnmInfoGetFirstAxisNo(long lNodeNum, long lModulePos, long *lpAxisNo);
    
//========= 가상 축 함수 ============================================================================================    
    // 초기 상태에서 ANM 모든 함수의 축번호 설정은 0 ~ (실제 시스템에 장착된 축수 - 1) 범위에서 유효하지만
    // 이 함수를 사용하여 실제 장착된 축번호 대신 임의의 축번호로 바꿀 수 있다.
    // 이 함수는 제어 시스템의 H/W 변경사항 발생시 기존 프로그램에 할당된 축번호를 그대로 유지하고 실제 제어 축의 
    // 물리적인 위치를 변경하여 사용을 위해 만들어진 함수이다.
    // 주의사항 : 여러 개의 실제 축번호에 대하여 같은 번호로 가상 축을 중복해서 맵핑할 경우 
    //            실제 축번호가 낮은 축만 가상 축번호로 제어 할 수 있으며, 
    //            나머지 같은 가상축 번호로 맵핑된 축은 제어가 불가능한 경우가 발생 할 수 있다.
    // 가상축을 설정한다.
    DWORD    __stdcall AnmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo);
    // 설정한 가상축 번호를 반환한다.
    DWORD    __stdcall AnmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo);
    // 멀티 가상축을 설정한다.
    DWORD    __stdcall AnmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // 설정한 멀티 가상축 번호를 반환한다.
    DWORD    __stdcall AnmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // 가상축 설정을 해지한다.
    DWORD    __stdcall AnmVirtualResetAxisMap();

//======== 모션 파라메타 설정 ===========================================================================================================================================================
    // AnmMotLoadParaAll로 파일을 Load 시키지 않으면 초기 파라메타 설정시 기본 파라메타 설정. 
    // 현재 PC에 사용되는 모든축에 똑같이 적용된다. 기본파라메타는 아래와 같다. 
    // 00:ANIS_NO.             =0       01:PULSE_OUT_METHOD.    =4      02:ENC_INPUT_METHOD.    =3     03:INPOSITION.          =2
    // 04:ALARM.               =0       05:NEG_END_LIMIT.       =0      06:POS_END_LIMIT.       =0     07:MIN_VELOCITY.        =1
    // 08:MAN_VELOCITY.        =700000  09:HOME_SIGNAL.         =4      10:HOME_LEVEL.          =1     11:HOME_DIR.            =0
    // 12:ZPHASE_LEVEL.        =1       13:ZPHASE_USE.          =0      14:STOP_SIGNAL_MODE.    =0     15:STOP_SIGNAL_LEVEL.   =0
    // 16:HOME_FIRST_VELOCITY. =10000   17:HOME_SECOND_VELOCITY.=10000  18:HOME_THIRD_VELOCITY. =2000  19:HOME_LAST_VELOCITY.  =100
    // 20:HOME_FIRST_ACCEL.    =40000   21:HOME_SECOND_ACCEL.   =40000  22:HOME_END_CLEAR_TIME. =1000  23:HOME_END_OFFSET.     =0
    // 24:NEG_SOFT_LIMIT.      =0.000   25:POS_SOFT_LIMIT.      =0      26:MOVE_PULSE.          =1     27:MOVE_UNIT.           =1
    // 28:INIT_POSITION.       =1000    29:INIT_VELOCITY.       =200    30:INIT_ACCEL.          =400   31:INIT_DECEL.          =400
    // 32:INIT_ABSRELMODE.     =0       33:INIT_PROFILEMODE.    =4

    // 00=[ANIS_NO             ]: 축 (0축 부터 시작함)
    // 01=[PULSE_OUT_METHOD    ]: Pulse out method TwocwccwHigh = 6
    // 02=[ENC_INPUT_METHOD    ]: disable = 0   1체배 = 1  2체배 = 2  4체배 = 3, 결선 관련방향 교체시(-).1체배 = 11  2체배 = 12  4체배 = 13
    // 03=[INPOSITION          ], 04=[ALARM     ], 05,06 =[END_LIMIT   ]  : 0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지
    // 07=[MIN_VELOCITY        ]: 시작 속도(START VELOCITY)
    // 08=[MAN_VELOCITY        ]: 드라이버가 지령을 받아들일수 있는 지령 속도. 보통 일반 Servo는 700k
    // Ex> screw : 20mm pitch drive: 10000 pulse 모터: 400w
    // 09=[HOME_SIGNAL         ]: 4 - Home in0 , 0 :PosEndLimit , 1 : NegEndLimit // _HOME_SIGNAL참조.
    // 10=[HOME_LEVEL          ]: 0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지
    // 11=[HOME_DIR            ]: 홈 방향(HOME DIRECTION) 1:+방향, 0:-방향
    // 12=[ZPHASE_LEVEL        ]: 0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지
    // 13=[ZPHASE_USE          ]: Z상사용여부. 0: 사용안함 , 1: +방향, 2: -방향 
    // 14=[STOP_SIGNAL_MODE    ]: ESTOP, SSTOP 사용시 모드 0:감속정지, 1:급정지 
    // 15=[STOP_SIGNAL_LEVEL   ]: ESTOP, SSTOP 사용 레벨.  0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지 
    // 16=[HOME_FIRST_VELOCITY ]: 1차구동속도 
    // 17=[HOME_SECOND_VELOCITY]: 검출후속도 
    // 18=[HOME_THIRD_VELOCITY ]: 마지막 속도 
    // 19=[HOME_LAST_VELOCITY  ]: index검색및 정밀하게 검색하기위한 속도. 
    // 20=[HOME_FIRST_ACCEL    ]: 1차 가속도 , 21=[HOME_SECOND_ACCEL   ] : 2차 가속도 
    // 22=[HOME_END_CLEAR_TIME ]: 원점 검색 Enc 값 Set하기 위한 대기시간,  23=[HOME_END_OFFSET] : 원점검출후 Offset만큼 이동.
    // 24=[NEG_SOFT_LIMIT      ]: - SoftWare Limit 같게 설정하면 사용안함, 25=[POS_SOFT_LIMIT ]: + SoftWare Limit 같게 설정하면 사용안함.
    // 26=[MOVE_PULSE          ]: 드라이버의 1회전당 펄스량              , 27=[MOVE_UNIT  ]: 드라이버 1회전당 이동량 즉:스크류 Pitch
    // 28=[INIT_POSITION       ]: 에이젼트 사용시 초기위치  , 사용자가 임의로 사용가능
    // 29=[INIT_VELOCITY       ]: 에이젼트 사용시 초기속도  , 사용자가 임의로 사용가능
    // 30=[INIT_ACCEL          ]: 에이젼트 사용시 초기가속도, 사용자가 임의로 사용가능
    // 31=[INIT_DECEL          ]: 에이젼트 사용시 초기감속도, 사용자가 임의로 사용가능
    // 32=[INIT_ABSRELMODE     ]: 절대(0)/상대(1) 위치 설정
    // 33=[INIT_PROFILEMODE    ]: 프로파일모드(0 - 4) 까지 설정
    //                            '0': 대칭 Trapezode, '1': 비대칭 Trapezode, '2': 대칭 Quasi-S Curve, '3':대칭 S Curve, '4':비대칭 S Curve
    
    // AnmMotSaveParaAll로 저장 되어진 .mot파일을 불러온다. 해당 파일은 사용자가 Edit 하여 사용 가능하다.
    DWORD    __stdcall AnmMotLoadParaAll(char *szFilePath);
    // 모든축에 대한 모든 파라메타를 축별로 저장한다. .mot파일로 저장한다.
    DWORD    __stdcall AnmMotSaveParaAll(char *szFilePath);

    // 파라메타 28 - 31번까지 사용자가 프로그램내에서  이 함수를 이용해 설정 한다
    DWORD    __stdcall AnmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel);    
    // 파라메타 28 - 31번까지 사용자가 프로그램내에서  이 함수를 이용해 확인 한다.
    DWORD    __stdcall AnmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel);    

    // 지정 축의 펄스 출력 방식을 설정한다.
    //uMethod  0 :OneHighLowHigh, 1 :OneHighHighLow, 2 :OneLowLowHigh, 3 :OneLowHighLow, 4 :TwoCcwCwHigh
    //         5 :TwoCcwCwLow,    6 :TwoCwCcwHigh,   7 :TwoCwCcwLow,   8 :TwoPhase,      9 :TwoPhaseReverse
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

    DWORD    __stdcall AnmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod);
    // 지정 축의 펄스 출력 방식 설정을 반환한다,
    DWORD    __stdcall AnmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod);

    // 지정 축의 외부(Actual) 카운트의 증가 방향 설정을 포함하여 지정 축의 Encoder 입력 방식을 설정한다.
    // uMethod : 0 - 7 설정.
    // ObverseUpDownMode          = 0x0    // 정방향 Up/Down
    // ObverseSqr1Mode            = 0x1    // 정방향 1체배
    // ObverseSqr2Mode            = 0x2    // 정방향 2체배
    // ObverseSqr4Mode            = 0x3    // 정방향 4체배
    // ReverseUpDownMode          = 0x4    // 역방향 Up/Down
    // ReverseSqr1Mode            = 0x5    // 역방향 1체배
    // ReverseSqr2Mode            = 0x6    // 역방향 2체배
  // ReverseSqr4Mode              = 0x7    // 역방향 4체배
    DWORD    __stdcall AnmMotSetEncInputMethod(long lAxisNo, DWORD uMethod);
    // 지정 축의 외부(Actual) 카운트의 증가 방향 설정을 포함하여 지정 축의 Encoder 입력 방식을 반환한다.
    DWORD    __stdcall AnmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod);

    // 설정 속도 단위가 RPM(Revolution Per Minute)으로 맞추고 싶다면.
    // ex>    rpm 계산:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1이면      pulse/ sec 초당 펄스수가 되는데
    // 4500 rpm에 맞추고 싶다면     4500 / 60 초 : 75회전/ 1초
    // 모터가 1회전에 몇 펄스인지 알아야 된다. 이것은 Encoder에 Z상을 검색해보면 알수있다.
    // 1회전:1800 펄스라면 75 x 1800 = 135000 펄스가 필요하게 된다.
    // AnmMotSetMoveUnitPerPulse에 Unit = 1, Pulse = 1800 넣어 동작시킨다.
    // 주의할점 : rpm으로 제어하게 된다면 속도와 가속도 도 rpm단위로 바뀌게 된다.

    // 지정 축의 펄스 당 움직이는 거리를 설정한다.
    DWORD    __stdcall AnmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse);
    // 지정 축의 펄스 당 움직이는 거리를 반환한다.
    DWORD    __stdcall AnmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse);
    
    // 지정 축에 감속 시작 포인트 검출 방식을 설정한다.
    //uMethod : 0 -1 설정
    // AutoDetect = 0x0 : 자동 가감속.
    // RestPulse  = 0x1 : 수동 가감속."

    DWORD    __stdcall AnmMotSetDecelMode(long lAxisNo, DWORD uMethod);
    // 지정 축의 감속 시작 포인트 검출 방식을 반환한다
    DWORD    __stdcall AnmMotGetDecelMode(long lAxisNo, DWORD *upMethod);
    
    // 지정 축에 수동 감속 모드에서 잔량 펄스를 설정한다.
    // 사용방법: 만약 AnmMotSetRemainPulse를 500 펄스를 설정
    //           AnmMoveStartPos를 위치 10000을 보냈을경우에 9500펄스부터 
    //           남은 펄스 500은  AnmMotSetMinVel로 설정한 속도로 유지하면서 감속 된다.
    DWORD    __stdcall AnmMotSetRemainPulse(long lAxisNo, DWORD uData);
    // 지정 축의 수동 감속 모드에서 잔량 펄스를 반환한다.
    DWORD    __stdcall AnmMotGetRemainPulse(long lAxisNo, DWORD *upData);

    // 지정 축에 등속도 구동 함수에서의 최고 속도를 설정한다.
    DWORD    __stdcall AnmMotSetMaxVel(long lAxisNo, double dVel);
    // 지정 축의 등속도 구동 함수에서의 최고 속도를 반환한다.
    DWORD    __stdcall AnmMotGetMaxVel(long lAxisNo, double *dpVel);

    // 지정 축의 이동 거리 계산 모드를 설정한다.
    //uAbsRelMode : POS_ABS_MODE '0' - 절대 좌표계
    //              POS_REL_MODE '1' - 상대 좌표계
    DWORD    __stdcall AnmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode);
    // 지정 축의 설정된 이동 거리 계산 모드를 반환한다
    DWORD    __stdcall AnmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode);

    //지정 축의 구동 속도 프로파일 모드를 설정한다.
    //ProfileMode : SYM_TRAPEZOIDE_MODE    '0' - 대칭 Trapezode
    //              ASYM_TRAPEZOIDE_MODE   '1' - 비대칭 Trapezode
    //              QUASI_S_CURVE_MODE     '2' - 지원안함
    //              SYM_S_CURVE_MODE       '3' - 대칭 S Curve
    //              ASYM_S_CURVE_MODE      '4' - 비대칭 S Curve
    DWORD    __stdcall AnmMotSetProfileMode(long lAxisNo, DWORD uProfileMode);
    // 지정 축의 설정한 구동 속도 프로파일 모드를 반환한다.
    DWORD    __stdcall AnmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode);
    
    //지정 축의 가속도 단위를 설정한다.
    //AccelUnit : UNIT_SEC2   '0' - 가감속 단위를 unit/sec2 사용
    //            SEC         '1' - 가감속 단위를 sec 사용
    DWORD    __stdcall AnmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit);
    // 지정 축의 설정된 가속도단위를 반환한다.
    DWORD    __stdcall AnmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit);

    // 지정 축에 초기 속도를 설정한다.
    DWORD    __stdcall AnmMotSetMinVel(long lAxisNo, double dMinVel);
    // 지정 축의 초기 속도를 반환한다.
    DWORD    __stdcall AnmMotGetMinVel(long lAxisNo, double *dpMinVel);

    // 지정 축의 가속 저크값을 설정한다.[%].
    DWORD    __stdcall AnmMotSetAccelJerk(long lAxisNo, double dAccelJerk);
    // 지정 축의 설정된 가속 저크값을 반환한다.
    DWORD    __stdcall AnmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk);

    // 지정 축의 감속 저크값을 설정한다.[%].
    DWORD    __stdcall AnmMotSetDecelJerk(long lAxisNo, double dDecelJerk);
    // 지정 축의 설정된 감속 저크값을 반환한다.
    DWORD    __stdcall AnmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk);
    
//=========== 입출력 신호 관련 설정함수 ================================================================================

    // 지정 축의 Z 상 Level을 설정한다.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel);
    // 지정 축의 Z 상 Level을 반환한다.
    DWORD    __stdcall AnmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel);

    // 지정 축의 Servo-On신호의 출력 레벨을 설정한다.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel);
    // 지정 축의 Servo-On신호의 출력 레벨 설정을 반환한다.
    DWORD    __stdcall AnmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel);

    // 지정 축의 Servo-Alarm Reset 신호의 출력 레벨을 설정한다.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel);
    // 지정 축의 Servo-Alarm Reset 신호의 출력 레벨을 설정을 반환한다.
    DWORD    __stdcall AnmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel);

    //    지정 축의 Inpositon 신호 사용 여부 및 신호 입력 레벨을 설정한다
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetInpos(long lAxisNo, DWORD uUse);
    // 지정 축의 Inpositon 신호 사용 여부 및 신호 입력 레벨을 반환한다.
    DWORD    __stdcall AnmSignalGetInpos(long lAxisNo, DWORD *upUse);
    // 지정 축의 Inpositon 신호 입력 상태를 반환한다.
    DWORD    __stdcall AnmSignalReadInpos(long lAxisNo, DWORD *upStatus);

    //    지정 축의 알람 신호 입력 시 비상 정지의 사용 여부 및 신호 입력 레벨을 설정한다.
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetServoAlarm(long lAxisNo, DWORD uUse);
    // 지정 축의 알람 신호 입력 시 비상 정지의 사용 여부 및 신호 입력 레벨을 반환한다.
    DWORD    __stdcall AnmSignalGetServoAlarm(long lAxisNo, DWORD *upUse);
    // 지정 축의 알람 신호의 입력 레벨을 반환한다.
    DWORD    __stdcall AnmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus);

    // 지정 축의 end limit sensor의 사용 유무 및 신호의 입력 레벨을 설정한다. 
    // end limit sensor 신호 입력 시 감속정지 또는 급정지에 대한 설정도 가능하다.
    // uStopMode: EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uPositiveLevel, uNegativeLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel);
    // 지정 축의 end limit sensor의 사용 유무 및 신호의 입력 레벨, 신호 입력 시 정지모드를 반환한다
    DWORD    __stdcall AnmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel);
    // 지정축의 end limit sensor의 입력 상태를 반환한다.
    DWORD    __stdcall AnmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);

    // 지정 축의 Software limit의 사용 유무, 사용할 카운트, 그리고 정지 방법을 설정한다
    // uUse       : DISABLE(0), ENABLE(1)
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uSelection : COMMAND(0), ACTUAL(1)
    // 주의사항: 원점검색시 위함수를 이용하여 소프트웨어 리밋을 미리 설정해서 구동시 원점검색시 원점검색을 도중에 멈추어졌을경우에도  Enable된다. 
    
    DWORD    __stdcall AnmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos);
    // 지정 축의 Software limit의 사용 유무, 사용할 카운트, 그리고 정지 방법을 반환한다
    DWORD    __stdcall AnmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos);

    // 비상 정지 신호의 정지 방법 (급정지/감속정지) 또는 사용 유무를 설정한다.
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel);
    // 비상 정지 신호의 정지 방법 (급정지/감속정지) 또는 사용 유무를 반환한다.
    DWORD    __stdcall AnmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel);
    // 비상 정지 신호의 입력 상태를 반환한다.
    DWORD    __stdcall AnmSignalReadStop(long lAxisNo, DWORD *upStatus);

    // 지정 축의 Servo-On 신호를 출력한다.
    // uOnOff : FALSE(0), TRUE(1) ( 범용 0출력에 해당됨)
    DWORD    __stdcall AnmSignalServoOn(long lAxisNo, DWORD uOnOff);
    // 지정 축의 Servo-On 신호의 출력 상태를 반환한다.
    DWORD    __stdcall AnmSignalIsServoOn(long lAxisNo, DWORD *upOnOff);

    // 지정 축의 Servo-Alarm Reset 신호를 출력한다.
    // uOnOff : FALSE(0), TRUE(1) ( 범용 1출력에 해당됨)
    DWORD    __stdcall AnmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff);
    
    //    범용 출력값을 설정한다.
    //  uValue : Hex Value 0x00
    DWORD    __stdcall AnmSignalWriteOutput(long lAxisNo, DWORD uValue);
    // 범용 출력값을 반환한다.
    DWORD    __stdcall AnmSignalReadOutput(long lAxisNo, DWORD *upValue);

    // lBitNo : Bit Number(0 - 4)
    // uOnOff : FALSE(0), TRUE(1)
    // 범용 출력값을 비트별로 설정한다.
    DWORD    __stdcall AnmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff);
    // 범용 출력값을 비트별로 반환한다.
    DWORD    __stdcall AnmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff);

    // 범용 입력값을 Hex값으로 반환한다.
    DWORD    __stdcall AnmSignalReadInput(long lAxisNo, DWORD *upValue);
    
    // lBitNo : Bit Number(0 - 4)
    // 범용 입력값을 비트별로 반환한다.
    DWORD    __stdcall AnmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn);

//========== 모션 구동중 및 구동후에 상태 확인하는 함수============================================================

    // 지정 축의 펄스 출력 상태를 반환한다.
    // (구동상태)"
    DWORD    __stdcall AnmStatusReadInMotion(long lAxisNo, DWORD *upStatus);

    // 구동시작 이후 지정 축의 구동 펄스 카운터 값을 반환한다.
    // 주의사항: 구동중에만 카운터값을 표시하고 구동종료후에는 카운터값이 CLEAR된다.    
    //  (펄스 카운트 값)"
    DWORD    __stdcall AnmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse);
    
    // 지정 축의 DriveStatus(모션중 상태) 레지스터를 반환한다
    DWORD    __stdcall AnmStatusReadMotion(long lAxisNo, DWORD *upStatus);
    
    // 지정 축의 EndStatus(정지 상태) 레지스터를 반환한다.
    DWORD    __stdcall AnmStatusReadStop(long lAxisNo, DWORD *upStatus);
    
    // 지정 축의 Mechanical Signal Data(현재 기계적인 신호상태) 를 반환한다.
    DWORD    __stdcall AnmStatusReadMechanical(long lAxisNo, DWORD *upStatus);
    
    // 지정 축의 현재 구동 속도를 읽어온다.
    DWORD    __stdcall AnmStatusReadVel(long lAxisNo, double *dpVel);
    
    // 지정 축의 Command Pos과 Actual Pos의 차를 반환한다.
    DWORD    __stdcall AnmStatusReadPosError(long lAxisNo, double *dpError);
    
    // 최종 드라이브로 이동하는(이동한) 거리를 확인 한다
    DWORD    __stdcall AnmStatusReadDriveDistance(long lAxisNo, double *dpUnit);

    // 지정 축의 Actual 위치를 설정한다.
    DWORD    __stdcall AnmStatusSetActPos(long lAxisNo, double dPos);
    // 지정 축의 Actual 위치를 반환한다.
    DWORD    __stdcall AnmStatusGetActPos(long lAxisNo, double *dpPos);

    // 지정 축의 Command 위치를 설정한다.
    DWORD    __stdcall AnmStatusSetCmdPos(long lAxisNo, double dPos);
    // 지정 축의 Command 위치를 반환한다.
    DWORD    __stdcall AnmStatusGetCmdPos(long lAxisNo, double *dpPos);

//======== 홈관련 함수=============================================================================================================================================================================================    

    // 지정 축의 Home 센서 Level 을 설정한다.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmHomeSetSignalLevel(long lAxisNo, DWORD uLevel);
    // 지정 축의 Home 센서 Level 을 반환한다.
    DWORD    __stdcall AnmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel);
    // 현재 홈 신호 입력상태를 확인한다. 홈신호는 사용자가 임의로 AnmHomeSetMethod 함수를 이용하여 설정할수있다.
    // 일반적으로 홈신호는 범용 입력 0를 사용하고있지만 AnmHomeSetMethod 이용해서 바꾸면 + , - Limit를 사용할수도있다.
    // upStatus : OFF(0), ON(1)
    DWORD    __stdcall AnmHomeReadSignal(long lAxisNo, DWORD *upStatus);
    
    // 해당 축의 원점검색을 수행하기 위해서는 반드시 원점 검색관련 파라메타들이 설정되어 있어야 됩니다. 
    // 만약 MotionPara설정 파일을 이용해 초기화가 정상적으로 수행됐다면 별도의 설정은 필요하지 않다. 
    // 원점검색 방법 설정에는 검색 진행방향, 원점으로 사용할 신호, 원점센서 Active Level, 엔코더 Z상 검출 여부 등을 설정 한다.
    // 주의사항 : 레벨을 잘못 설정시 -방향으로 설정해도  +방향으로 동작할수 있으며, 홈을 찾는데 있어 문제가 될수있다.
    // (자세한 내용은 AnmMotSaveParaAll 설명 부분 참조)
    // 홈레벨은 AnmSignalSetHomeLevel 사용한다.
    // HClrTim : HomeClear Time : 원점 검색 Encoder 값 Set하기 위한 대기시간 
    // HmDir(홈 방향): DIR_CCW (0) -방향 , DIR_CW(1) +방향
    // HOffset - 원점검출후 이동거리.
    // uZphas: 1차 원점검색 완료 후 엔코더 Z상 검출 유무 설정  0: 사용안함 , 1: +방향, 2: -방향 
    // HmSig:   PosEndLimit(0) -> +Limit
    //          NegEndLimit(1) -> -Limit
    //          HomeSensor (4) -> 원점센서(범용 입력 0)

    DWORD   __stdcall AnmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
    // 설정되어있는 홈 관련 파라메타들을 반환한다.
    DWORD   __stdcall AnmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset);

    // 원점을 빠르고 정밀하게 검색하기 위해 여러 단계의 스탭으로 검출한다. 이때 각 스탭에 사용 될 속도를 설정한다. 
    // 이 속도들의 설정값에 따라 원점검색 시간과, 원점검색 정밀도가 결정된다. 
    // 각 스탭별 속도들을 적절히 바꿔가면서 각 축의 원점검색 속도를 설정하면 된다. 
    // (자세한 내용은 AnmMotSaveParaAll 설명 부분 참조)
    // 원점검색시 사용될 속도를 설정하는 함수
    // [dVelFirst]- 1차구동속도   [dVelSecond]-검출후속도   [dVelThird]- 마지막 속도  [dvelLast]- index검색및 정밀하게 검색하기위해. 
    // [dAccFirst]- 1차구동가속도 [dAccSecond]-검출후가속도 
    DWORD    __stdcall AnmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
    // 설정되어있는 원점검색시 사용될 속도를 반환한다.
    DWORD    __stdcall AnmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond);

    // 원점검색을 시작한다.
    // 원점검색 시작함수를 실행하면 라이브러리 내부에서 해당축의 원점검색을 수행 할 쓰레드가 자동 생성되어 원점검색을 순차적으로 수행한 후 자동 종료된다.
    // 주의사항 : 진행방향과 반대방향의 리미트 센서가 들어와도 진행방향의 센서가 ACTIVE되지않으면 동작한다.
    //          원점 검색이 시작되어 진행방향이 리밋트 센서가 들어오면 리밋트 센서가 감지되었다고 생각하고 다음단계로 진행된다.

    DWORD    __stdcall AnmHomeSetStart(long lAxisNo);
    // 원점검색 결과를 사용자가 임의로 설정한다.
    // 원점검색 함수를 이용해 성공적으로 원점검색이 수행되고나면 검색 결과가 HOME_SUCCESS로 설정됩니다.
    // 이 함수는 사용자가 원점검색을 수행하지않고 결과를 임의로 설정할 수 있다. 
    // uHomeResult 설정
    // HOME_SUCCESS               = 0x01    // 홈 완료
    // HOME_SEARCHING             = 0x02    // 홈검색중
    // HOME_ERR_GNT_RANGE         = 0x10    // 홈 검색 범위를 벗어났을경우
    // HOME_ERR_USER_BREAK        = 0x11    // 속도 유저가 임의로 정지명령을 내렸을경우
    // HOME_ERR_VELOCITY          = 0x12    // 속도 설정 잘못했을경우
    // HOME_ERR_AMP_FAULT         = 0x13    // 서보팩 알람 발생 에러
    // HOME_ERR_NEG_LIMIT         = 0x14    // (-)방향 구동중 (+)리미트 센서 감지 에러
    // HOME_ERR_POS_LIMIT         = 0x15    // (+)방향 구동중 (-)리미트 센서 감지 에러
    // HOME_ERR_NOT_DETECT        = 0x16    // 지정한 신호 검출하지 못 할 경우 에러
    // HOME_ERR_UNKNOWN           = 0xFF    
    DWORD    __stdcall AnmHomeSetResult(long lAxisNo, DWORD uHomeResult);
    // 원점검색 결과를 반환한다.
    // 원점검색 함수의 검색 결과를 확인한다. 원점검색이 시작되면 HOME_SEARCHING으로 설정되며 원점검색에 실패하면 실패원인이 설정된다. 실패 원인을 제거한 후 다시 원점검색을 진행하면 된다.
    DWORD    __stdcall AnmHomeGetResult(long lAxisNo, DWORD *upHomeResult);

    // 원점검색 진행률을 반환한다.
    // 원점검색 시작되면 진행율을 확인할 수 있다. 원점검색이 완료되면 성공여부와 관계없이 100을 반환하게 된다. 원점검색 성공여부는 GetHome Result함수를 이용해 확인할 수 있다.
    // upHomeMainStepNumber : Main Step 진행율이다. 
    // 겐트리 FALSE일 경우upHomeMainStepNumber : 0 일때면 선택한 축만 진행사항이고 홈 진행율은 upHomeStepNumber 표시한다.
    // 겐트리 TRUE일 경우 upHomeMainStepNumber : 0 일때면 마스터 홈을 진행사항이고 마스터 홈 진행율은 upHomeStepNumber 표시한다.
    // 겐트리 TRUE일 경우 upHomeMainStepNumber : 10 일때면 슬레이브 홈을 진행사항이고 마스터 홈 진행율은 upHomeStepNumber 표시한다.
    // upHomeStepNumber     : 선택한 축에대한 진행율을 표시한다. 
    // 겐트리 FALSE일 경우  : 선택한 축만 진행율을 표시한다.
    // 겐트리 TRUE일 경우 마스터축, 슬레이브축 순서로 진행율을 표시된다.
    DWORD    __stdcall AnmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber);

//========= 위치 구동함수 ===============================================================================================================
    
    // 주의사항: 위치를 설정할경우 반드시 UNIT/PULSE의 맞추어서 설정한다.
    //           위치를 UNIT/PULSE 보다 작게할 경우 최소단위가 UNIT/PULSE로 맞추어지기때문에 그위치까지 구동이 될수없다.

    // 설정 속도 단위가 RPM(Revolution Per Minute)으로 맞추고 싶다면.
    // ex>    rpm 계산:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1이면      pulse/ sec 초당 펄스수가 되는데
    // 4500 rpm에 맞추고 싶다면     4500 / 60 초 : 75회전/ 1초
    // 모터가 1회전에 몇 펄스인지 알아야 된다. 이것은 Encoder에 Z상을 검색해보면 알수있다.
    // 1회전:1800 펄스라면 75 x 1800 = 135000 펄스가 필요하게 된다.
    // AnmMotSetMoveUnitPerPulse에 Unit = 1, Pulse = 1800 넣어 동작시킨다. 

    // 설정한 거리만큼 또는 위치까지 이동한다.
    // 지정 축의 절대 좌표/ 상대좌표 로 설정된 위치까지 설정된 속도와 가속율로 구동을 한다.
    // 속도 프로파일은 AnmMotSetProfileMode 함수에서 설정한다.
    // 펄스가 출력되는 시점에서 함수를 벗어난다.
    // AnmMotSetAccelUnit(lAxisNo, 1) 일경우 dAccel -> dAccelTime , dDecel -> dDecelTime 으로 바뀐다.
    DWORD    __stdcall AnmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

    // 설정한 거리만큼 또는 위치까지 이동한다.
    // 지정 축의 절대 좌표/상대좌표로 설정된 위치까지 설정된 속도와 가속율로 구동을 한다.
    // 속도 프로파일은 AnmMotSetProfileMode 함수에서 설정한다. 
    // 펄스 출력이 종료되는 시점에서 함수를 벗어난다
    DWORD    __stdcall AnmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

    // 설정한 속도로 구동한다.
    // 지정 축에 대하여 설정된 속도와 가속율로 지속적으로 속도 모드 구동을 한다. 
    // 펄스 출력이 시작되는 시점에서 함수를 벗어난다.
    // Vel값이 양수이면 CW, 음수이면 CCW 방향으로 구동.
    DWORD    __stdcall AnmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel);

    // 지정된 다축에 대하여 설정된 속도와 가속율로 지속적으로 속도 모드 구동을 한다.
    // 펄스 출력이 시작되는 시점에서 함수를 벗어난다.
    // Vel값이 양수이면 CW, 음수이면 CCW 방향으로 구동.
    DWORD    __stdcall AnmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

    // 특정 Input 신호의 Edge를 검출하여 즉정지 또는 감속정지하는 함수.
    // lDetect Signal : edge 검출할 입력 신호 선택.
    // lDetectSignal  : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // Signal Edge    : 선택한 입력 신호의 edge 방향 선택 (rising or falling edge).
    //                  SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // 구동방향       : Vel값이 양수이면 CW, 음수이면 CCW.
    // SignalMethod   : 급정지 EMERGENCY_STOP(0), 감속정지 SLOWDOWN_STOP(1)
    // 주의사항: SignalMethod를 EMERGENCY_STOP(0)로 사용할경우 가감속이 무시되며 지정된 속도로 가속 급정지하게된다.
    //           lDetectSignal이 PosEndLimit , NegEndLimit(0,1) 을 찾을경우 신호의레벨 Active 상태를 검출하게된다.
    DWORD    __stdcall AnmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);
    
    // 지정 축에서 설정된 신호를 검출하고 그 위치를 저장하기 위해 이동하는 함수이다.
    // 원하는 신호를 골라 찾아 움직이는 함수 찾을 경우 그 위치를 저장시켜놓고 AnmGetCapturePos사용하여 그값을 읽는다.
    // Signal Edge   : 선택한 입력 신호의 edge 방향 선택 (rising or falling edge).
    //                 SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
    // SignalMethod  : 급정지 EMERGENCY_STOP(0), 감속정지 SLOWDOWN_STOP(1)
    // lDetect Signal: edge 검출할 입력 신호 선택.SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // lDetectSignal : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // lTarget       : COMMAND(0), ACTUAL(1)
    // 주의사항: SignalMethod를 EMERGENCY_STOP(0)로 사용할경우 가감속이 무시되며 지정된 속도로 가속 급정지하게된다.
    //           lDetectSignal이 PosEndLimit , NegEndLimit(0,1) 을 찾을경우 신호의레벨 Active 상태를 검출하게된다.
    
    DWORD    __stdcall AnmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod);
    
    // 'AnmMoveSignalCapture' 함수에서 저장된 위치값을 확인하는 함수이다.
    // 주의사항: 함수 실행 결과가 "ANT_RT_SUCCESS"일때 저장된 위치가 유효하며, 이 함수를 한번 실행하면 저장 위치값이 초기화된다.
    DWORD    __stdcall AnmMoveGetCapturePos(long lAxisNo, double *dpCapPotition);

    // "설정한 거리만큼 또는 위치까지 이동하는 함수.
    // 함수를 실행하면 해당 Motion 동작을 시작한 후 Motion 이 완료될때까지 기다리지 않고 바로 함수를 빠져나간다."
    DWORD    __stdcall AnmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);
    
    // 다축을 설정한 거리만큼 또는 위치까지 이동한다.
    // 지정 축들의 절대 좌표로 설정된 위치까지 설정된 속도와 가속율로 구동을 한다.
    DWORD    __stdcall AnmMoveMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);

    // 지정 축을 설정한 감속도로 감속 정지 한다.
    // dDecel : 정지 시 감속율값
    DWORD    __stdcall AnmMoveStop(long lAxisNo, double dDecel);
    // 지정 축을 급 정지 한다.
    DWORD    __stdcall AnmMoveEStop(long lAxisNo);
    // 지정 축을 감속 정지한다.
    DWORD    __stdcall AnmMoveSStop(long lAxisNo);

//========= 오버라이드 함수 ============================================================================

    // 위치 오버라이드 한다.
    // 지정 축의 구동이 종료되기 전 지정된 출력 펄스 수를 조정한다.
    // 주의사항: 오버라이드할 위치를 넣을때는 구동 시점의 위치를 기준으로한 Relative 형태의 위치값으로 넣어준다.
    //           구동시작후 같은방향의 경우 오버라이드를 계속할수있지만 반대방향으로 오버라이드할경우에는 오버라이드를 계속할수없다.
    DWORD    __stdcall AnmOverridePos(long lAxisNo, double dOverridePos);
    
    // 지정 축의 속도오버라이드 하기전에 오버라이드할 최고속도를 설정한다.
    // 주의점 : 속도오버라이드를 5번한다면 그중에 최고 속도를 설정해야된다. 
    DWORD    __stdcall AnmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel);
    
    // 속도 오버라이드 한다.
    // 지정 축의 구동 중에 속도를 가변 설정한다. (반드시 모션 중에 가변 설정한다.)
    // 주의점: AnmOverrideVel 함수를 사용하기전에. AnmOverrideMaxVel 최고로 설정할수있는 속도를 설정해놓는다.
    // EX> 속도오버라이드를 두번한다면 
    // 1. 두개중에 높은 속도를 AnmOverrideMaxVel 설정 최고 속도값 설정.
    // 2. AnmMoveStartPos 실행 지정 축의 구동 중(Move함수 모두 포함)에 속도를 첫번째 속도로 AnmOverrideVel 가변 설정한다.
    // 3. 지정 축의 구동 중(Move함수 모두 포함)에 속도를 두번째 속도로 AnmOverrideVel 가변 설정한다.
    DWORD    __stdcall AnmOverrideVel(long lAxisNo, double dOverrideVel);

    // 가속도, 속도, 감속도를  오버라이드 한다.
    // 지정 축의 구동 중에 가속도, 속도, 감속도를 가변 설정한다. (반드시 모션 중에 가변 설정한다.)
    // 주의점: AnmOverrideAccelVelDecel 함수를 사용하기전에. AnmOverrideMaxVel 최고로 설정할수있는 속도를 설정해놓는다.
    // EX> 속도오버라이드를 두번한다면 
    // 1. 두개중에 높은 속도를 AnmOverrideMaxVel 설정 최고 속도값 설정.
    // 2. AnmMoveStartPos 실행 지정 축의 구동 중(Move함수 모두 포함)에 가속도, 속도, 감속도를 첫번째 속도로 AnmOverrideAccelVelDecel 가변 설정한다.
    // 3. 지정 축의 구동 중(Move함수 모두 포함)에 가속도, 속도, 감속도를 두번째 속도로 AnmOverrideAccelVelDecel 가변 설정한다.
    DWORD    __stdcall AnmOverrideAccelVelDecel(long lAxisNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel);
    
    // 어느 시점에서 속도 오버라이드 한다.
    // 어느 위치 지점과 오버라이드할 속도를 입력시켜 그위치에서 속도오버라이드 되는 함수
    // lTarget : COMMAND(0), ACTUAL(1)
    // 주의점: AnmOverrideVelAtPos 함수를 사용하기전에. AnmOverrideMaxVel 최고로 설정할수있는 속도를 설정해놓는다.

    DWORD    __stdcall AnmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel,double dOverridePos, double dOverrideVel, long lTarget);
    
//========= 마스터, 슬레이브  기어비로 구동 함수 ===========================================================================

    // Electric Gear 모드에서 Master 축과 Slave 축과의 기어비를 설정한다.
    // dSlaveRatio : 마스터축에 대한 슬레이브의 기어비( 0 : 0% , 0.5 : 50%, 1 : 100%)
    DWORD    __stdcall AnmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio);
    // Electric Gear 모드에서 설정된 Master 축과 Slave 축과의 기어비를 반환한다.
    DWORD    __stdcall AnmLinkGetMode(long lMasterAxisNo, long *lpSlaveAxisNo, double *dpGearRatio);
    // Master 축과 Slave축간의 전자기어비를 설정 해제 한다.
    DWORD    __stdcall AnmLinkResetMode(long lMasterAxisNo);

//======== 겐트리 관련 함수===========================================================================================================================================================
    // 모션모듈은 두 축이 기구적으로 Link되어있는 겐트리 구동시스템 제어를 지원한다. 
    // 이 함수를 이용해 Master축을 겐트리 제어로 설정하면 해당 Slave축은 Master축과 동기되어 구동됩니다. 
    // 만약 겐트리 설정 이후 Slave축에 구동명령이나 정지 명령등을 내려도 모두 무시됩니다.
    // uSlHomeUse     : 슬레이축 홈사용 우뮤 ( 0 - 2)
    //             (0 : 슬레이브축 홈을 사용안하고 마스터축을 홈을 찾는다.)
    //             (1 : 마스터축 , 슬레이브축 홈을 찾는다. 슬레이브 dSlOffset 값 적용해서 보정함.)
    //             (2 : 마스터축 , 슬레이브축 홈을 찾는다. 슬레이브 dSlOffset 값 적용해서 보정안함.)
    // dSlOffset      : 슬레이브축 옵셋값
    // dSlOffsetRange : 슬레이브축 옵셋값 레인지 설정
    // 주의사항: 갠트리 ENABLE시 슬레이브축은 모션중 AnmStatusReadMotion 함수로 확인하면 True(Motion 구동 중)로 확인되야 정상동작이다. 
    //           슬레이브축에 AnmStatusReadMotion로 확인했을때 InMotion 이 False이면 Gantry Enable이 안된것이므로 알람 혹은 리밋트 센서 등을 확인한다.
    DWORD    __stdcall AnmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange);

    // Slave축의 Offset값을 알아내는방법.
    // A. 마스터, 슬레이브를 모두 서보온을 시킨다.         
    // B. AnmGantrySetEnable함수에서 uSlHomeUse = 2로 설정후 AnmHomeSetStart함수를 이용해서 홈을 찾는다. 
    // C. 홈을 찾고 나면 마스터축의 Command값을 읽어보면 마스터축과 슬레이브축의 틀어진 Offset값을 볼수있다.
    // D. Offset값을 읽어서 AnmGantrySetEnable함수의 dSlOffset인자에 넣어준다. 
    // E. dSlOffset값을 넣어줄때 마스터축에 대한 슬레이브 축 값이기때문에 부호를 반대로 -dSlOffset 넣어준다.
    // F. dSIOffsetRange 는 Slave Offset의 Range 범위를 말하는데 Range의 한계를 지정하여 한계를 벗어나면 에러를 발생시킬때 사용한다.        
    // G. AnmGantrySetEnable함수에 Offset값을 넣어줬으면  AnmGantrySetEnable함수에서 uSlHomeUse = 1로 설정후 AnmHomeSetStart함수를 이용해서 홈을 찾는다.         
    
    // 겐트리 구동에 있어 사용자가 설정한 파라메타를 반환한다.
    DWORD    __stdcall AnmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn);
    // 모션 모듈은 두 축이 기구적으로 Link되어있는 겐트리 구동시스템 제어를 해제한다.
    DWORD    __stdcall AnmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo);

//====일반 보간함수 ============================================================================================================================================;
    //지정된 좌표계에 보간 축 맵핑을 설정한다.
    //(축맵핑 번호는 0 부터 시작))
    // 주의점: 축맵핑할때는 반드시 실제 축번호가 작은 숫자부터 큰숫자를 넣는다.
    //         가상축 맵핑 함수를 사용하였을 때 가상축번호를 실제 축번호가 작은 값 부터 lpAxesNo의 낮은 인텍스에 입력하여야 한다.
    //         가상축 맵핑 함수를 사용하였을 때 가상축번호에 해당하는 실제 축번호가 다른 값이라야 한다.
    //         같은 축을 다른 Coordinate에 중복 맵핑하지 말아야 한다.
    DWORD    __stdcall AnmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
    //지정된 좌표계에 보간 축 맵핑을 반환한다.
    DWORD    __stdcall AnmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);
    
    // 지정된 좌표계에 보간 축 절대/상대 모드를 설정한다.
    // (주의점 : 반드시 축맵핑 하고 사용가능)
    // 지정 축의 이동 거리 계산 모드를 설정한다.
    //uAbsRelMode : POS_ABS_MODE '0' - 절대 좌표계
    //              POS_REL_MODE '1' - 상대 좌표계
    DWORD    __stdcall AnmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
    // 지정된 좌표계에 보간 축 절대/상대 모드를 반환한다.
    DWORD    __stdcall AnmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);

    // 지정된 좌표계에 보간 구동 중인지 확인하는 함수이다.
    DWORD    __stdcall AnmContiIsMotion(long lCoord, DWORD *upInMotion);

    // 주의사항1: AnmContiSetAxisMap함수를 이용하여 축맵핑후에 낮은순서축부터 맵핑을 하면서 사용해야된다.
    //           원호보간의 경우에는 반드시 낮은순서축부터 축배열에 넣어야 동작 가능하다.
    
    // 주의사항2: 위치를 설정할경우 반드시 마스터축과 슬레이브 축의 UNIT/PULSE의 맞추어서 설정한다.
    //           위치를 UNIT/PULSE 보다 작게 설정할 경우 최소단위가 UNIT/PULSE로 맞추어지기때문에 그위치까지 구동이 될수없다.

    // 주의사항3: 원호 보간을 할경우 반드시 한칩내에서 구동이 될수있으므로 

    // 주의사항4: 보간 구동 시작/중에 비정상 정지 조건(+- Limit신호, 서보 알람, 비상정지 등)이 발생하면 
    //            구동 방향에 상관없이 구동을 시작하지 않거나 정지 된다.

    // 직선 보간 한다.
    // 시작점과 종료점을 지정하여 다축 직선 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AnmContiBeginNode, AnmContiEndNode와 같이사용시 지정된 좌표계에 시작점과 종료점을 지정하여 직선 보간 구동하는 Queue에 저장함수가된다. 
    DWORD    __stdcall AnmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel);

    // 2축 원호보간 한다.
    // 시작점, 종료점과 중심점을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AnmContiBeginNode, AnmContiEndNode, 와 같이사용시 지정된 좌표계에 시작점, 종료점과 중심점을 지정하여 구동하는 원호 보간 Queue에 저장함수가된다.
    // lAxisNo = 두축 배열 , dCenterPos = 중심점 X,Y 배열 , dEndPos = 종료점 X,Y 배열.
    // uCWDir   DIR_CCW(0): 반시계방향, DIR_CW(1) 시계방향
    
    DWORD   __stdcall AnmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

    // 중간점, 종료점을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AnmContiBeginNode, AnmContiEndNode와 같이사용시 지정된 좌표계에 중간점, 종료점을 지정하여 구동하는 원호 보간 Queue에 저장함수가된다.
    // lAxisNo = 두축 배열 , dMidPos = 중간점 X,Y 배열 , dEndPos = 종료점 X,Y 배열, lArcCircle = 아크(0), 원(1)

    DWORD    __stdcall AnmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle);
    
    // 시작점, 종료점과 반지름을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AnmContiBeginNode, AnmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 원호 보간 구동하는 Queue에 저장함수가된다.
    // lAxisNo = 두축 배열 , dRadius = 반지름, dEndPos = 종료점 X,Y 배열 , uShortDistance = 작은원(0), 큰원(1)
    // uCWDir   DIR_CCW(0): 반시계방향, DIR_CW(1) 시계방향
    DWORD   __stdcall AnmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);

    // 시작점, 회전각도와 반지름을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AnmContiBeginNode, AnmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 회전각도와 반지름을 지정하여 원호 보간 구동하는 Queue에 저장함수가된다.
    // lAxisNo = 두축 배열 , dCenterPos = 중심점 X,Y 배열 , dAngle = 각도.
    // uCWDir   DIR_CCW(0): 반시계방향, DIR_CW(1) 시계방향
    DWORD   __stdcall AnmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir);

//====================트리거 함수 ===============================================================================================================================

    // 주의사항: 트리거 위치를 설정할경우 반드시 UNIT/PULSE의 맞추어서 설정한다.
    //           위치를 UNIT/PULSE 보다 작게할 경우 최소단위가 UNIT/PULSE로 맞추어지기때문에 그위치에 출력할수없다.

    // 지정 축에 트리거 기능의 사용 여부, 출력 레벨, 위치 비교기, 트리거 신호 지속 시간 및 트리거 출력 모드를 설정한다.
    // 트리거 기능 사용을 위해서는 먼저  AnmTriggerSetTimeLevel 를 사용하여 관련 기능 설정을 먼저 하여야 한다.
    //  dTrigTime  : 트리거 출력 시간(1usec - 최대 50msec ( 1 - 50000 까지 설정))
    //  upTriggerLevel : 트리거 출력 레벨 유무    => LOW(0), HIGH(1)
    //  uSelect        : 사용할 기준 위치            => COMMAND(0), ACTUAL(1)
    //  uInterrupt     : 인터럽트 설정                => DISABLE(0), ENABLE(1)
    
    // 지정 축에 트리거 신호 지속 시간 및 트리거 출력 레벨, 트리거 출력방법을 설정한다.
     DWORD    __stdcall AnmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt);
    // 지정 축에 트리거 신호 지속 시간 및 트리거 출력 레벨, 트리거 출력방법을 반환한다.
    DWORD    __stdcall AnmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt);
    
    // 지정 축의 트리거 출력 기능을 설정한다.
    //  uMethod : PERIOD_MODE      0x0 : 현재 위치를 기준으로 dPos를 위치 주기로 사용한 주기 트리거 방식
    //            ABS_POS_MODE     0x1 : 트리거 절대 위치에서 트리거 발생, 절대 위치 방식

    //  dPos : 주기 선택시 : 위치마다위치마다 출력하기때문에 그 위치
    //         절대 선택시 : 출력할 그 위치, 이 위치와같으면 무조건 출력이 나간다. 
    //  주의사항: AnmTriggerSetAbsPeriod의 주기모드로 설정할 경우 처음 그위치가 범위 안에 있으므로 트리거 출력이 한번 발생한다.

    DWORD    __stdcall AnmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos);
    // 지정 축에 트리거 기능의 사용 여부, 출력 레벨, 위치 비교기, 트리거 신호 지속 시간 및 트리거 출력 모드를 반환한다.
    DWORD    __stdcall AnmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos);

    //  사용자가 지정한 시작위치부터 종료위치까지 일정구간마다 트리거를 출력 한다.
    DWORD    __stdcall AnmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos);
    // 'AnmTriggerSetBlock' 함수의 설정한 값을 읽는다..
    DWORD    __stdcall AnmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos);
    // 사용자가 한 개의 트리거 펄스를 출력한다.
    DWORD    __stdcall AnmTriggerOneShot(long lAxisNo);
    // 사용자가 한 개의 트리거 펄스를 지정 시간 후에 출력한다.
    DWORD    __stdcall AnmTriggerSetTimerOneshot(long lAxisNo, long lmSec);
    // 입력한 절대위치값의 순서로 해당 위치를 지날때 트리거 신호를 출력한다.
    DWORD    __stdcall AnmTriggerOnlyAbs(long lAxisNo,long lTrigNum, double* dpTrigPos);
    // 트리거 기능 설정을 초기화 한다.
    DWORD    __stdcall AnmTriggerSetReset(long lAxisNo); 

//======== CRC( 잔여 펄스 클리어 함수)=====================================================================    
    //Level   : LOW(0), HIGH(1), UNUSED(2), USED(3) 
    //uMethod : 잔여펄스 제거 출력 신호 펄스 폭 2 - 6까지 설정가능.
    //          0: Don't care , 1: Don't care, 2: 500 uSec, 3: 1 mSec, 4: 10 mSec, 5: 50 mSec, 6: 100 mSec

    //지정 축에 CRC 신호 사용 여부 및 출력 레벨을 설정한다.
    DWORD    __stdcall AnmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod);
    // 지정 축의 CRC 신호 사용 여부 및 출력 레벨을 반환한다.
    DWORD    __stdcall AnmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod);

    //uOnOff  : CRC 신호를 Program으로 발생 여부  (FALSE(0),TRUE(1))

    // 지정 축에 CRC 신호를 강제로 발생 시킨다.
    DWORD    __stdcall AnmCrcSetOutput(long lAxisNo, DWORD uOnOff);
    // 지정 축의 CRC 신호를 강제로 발생 여부를 반환한다.
    DWORD    __stdcall AnmCrcGetOutput(long lAxisNo, DWORD *upOnOff);

    //======MPG(Manual Pulse Generation) 함수===========================================================
    // lInputMethod  : 0-3 까지 설정가능. 0:OnePhase, 2:TwoPhase2, 3:TwoPhase4
    // lDriveMode    : 0 만 설정가능 (0 :MPG 연속모드)
    // MPGPos        : 사용하지 않음.(Dont't care.)
    // MPGdenominator: MPG(수동 펄스 발생 장치 입력)구동 시 나누기 값
    // dMPGnumerator : MPG(수동 펄스 발생 장치 입력)구동 시 곱하기 값
    // dwNumerator   : 최대(1 에서    64) 까지 설정 가능
    // dwDenominator : 최대(1 에서  4096) 까지 설정 가능
    // dMPGdenominator = 4096, MPGnumerator=1 가 의미하는 것은 
    // MPG 한바퀴에 200펄스면 그대로 1:1로 1펄스씩 출력을 의미한다. 
    // 만약 dMPGdenominator = 4096, MPGnumerator=2 로 했을경우는 1:2로 2펄스씩 출력을 내보낸다는의미이다. 
    // 여기에 MPG PULSE = ((Numerator) * (Denominator)/ 4096 ) 칩내부에 출력나가는 계산식이다.

    // 지정 축에 MPG 입력방식, 드라이브 구동 모드, 이동 거리, MPG 속도 등을 설정한다.
    DWORD    __stdcall AnmMPGSetEnable(long lAxisNo, long lInputMethod, long lDriveMode, double dMPGPos, double dVel, double dAccel);
    // 지정 축에 MPG 입력방식, 드라이브 구동 모드, 이동 거리, MPG 속도 등을 반환한다.
    DWORD    __stdcall AnmMPGGetEnable(long lAxisNo, long *lpInputMethod, long *lpDriveMode, double *dpMPGPos, double *dpVel, double *dAccel);

    // 지정 축에 MPG 드라이브 구동 모드에서 한펄스당 이동할 펄스 비율을 설정한다.
    DWORD    __stdcall AnmMPGSetRatio(long lAxisNo, DWORD uMPGnumerator, DWORD uMPGdenominator);
    // 지정 축에 MPG 드라이브 구동 모드에서 한펄스당 이동할 펄스 비율을 반환한다.
    DWORD    __stdcall AnmMPGGetRatio(long lAxisNo, DWORD *upMPGnumerator, DWORD *upMPGdenominator);
    
    // 지정 축에 MPG 드라이브 설정을 해지한다.
    DWORD    __stdcall AnmMPGReset(long lAxisNo);

//========= 부가기능 함수 ============================================================================

    // 지정 축이 연결된 노드의 통신이 끊어 졌을때 현재 모션 동작 상태에 대한 예외처리 방법 설정.
    //dwNetErrorAct :    '0' - 현재 모션 계속 진행
    //                   '1' - 감속 정지
    //                   '2' - 급 정지
    DWORD    __stdcall AnmNetWorkErrorSetAction(long lAxisNo, DWORD dwNetErrorAct);
    // 지정 축이 연결된 노드의 통신이 끊어 졌을때 현재 모션 동작 상태에 대한 예외처리 방법 확인.
    DWORD    __stdcall AnmNetWorkErrorGetAction(long lAxisNo, DWORD* dwpNetErrorAct);

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__ANT_ANM_H__
