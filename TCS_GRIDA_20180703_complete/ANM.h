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

//========== ���� �� ��� Ȯ���Լ�(Info) - Infomation =================================================================================

    // �ش� ���� �����ȣ, ��� ��ġ, ��� ���̵� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmInfoGetAxis(long lAxisNo, long *lpNodeNum, long *lpModulePos, DWORD *upModuleID);
    // ��� ����� �����ϴ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmInfoIsMotionModule(DWORD *upStatus);
    // �ش� ���� ��ȿ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmInfoIsInvalidAxisNo(long lAxisNo);
    // CAMC-QI �� ����, �ý��ۿ� ������ ��ȿ�� ��� ����� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmInfoGetAxisCount(long *lpAxisCount);
    // �ش� ���/����� ù��° ���ȣ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmInfoGetFirstAxisNo(long lNodeNum, long lModulePos, long *lpAxisNo);
    
//========= ���� �� �Լ� ============================================================================================    
    // �ʱ� ���¿��� ANM ��� �Լ��� ���ȣ ������ 0 ~ (���� �ý��ۿ� ������ ��� - 1) �������� ��ȿ������
    // �� �Լ��� ����Ͽ� ���� ������ ���ȣ ��� ������ ���ȣ�� �ٲ� �� �ִ�.
    // �� �Լ��� ���� �ý����� H/W ������� �߻��� ���� ���α׷��� �Ҵ�� ���ȣ�� �״�� �����ϰ� ���� ���� ���� 
    // �������� ��ġ�� �����Ͽ� ����� ���� ������� �Լ��̴�.
    // ���ǻ��� : ���� ���� ���� ���ȣ�� ���Ͽ� ���� ��ȣ�� ���� ���� �ߺ��ؼ� ������ ��� 
    //            ���� ���ȣ�� ���� �ุ ���� ���ȣ�� ���� �� �� ������, 
    //            ������ ���� ������ ��ȣ�� ���ε� ���� ��� �Ұ����� ��찡 �߻� �� �� �ִ�.
    // �������� �����Ѵ�.
    DWORD    __stdcall AnmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo);
    // ������ ������ ��ȣ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo);
    // ��Ƽ �������� �����Ѵ�.
    DWORD    __stdcall AnmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // ������ ��Ƽ ������ ��ȣ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // ������ ������ �����Ѵ�.
    DWORD    __stdcall AnmVirtualResetAxisMap();

//======== ��� �Ķ��Ÿ ���� ===========================================================================================================================================================
    // AnmMotLoadParaAll�� ������ Load ��Ű�� ������ �ʱ� �Ķ��Ÿ ������ �⺻ �Ķ��Ÿ ����. 
    // ���� PC�� ���Ǵ� ����࿡ �Ȱ��� ����ȴ�. �⺻�Ķ��Ÿ�� �Ʒ��� ����. 
    // 00:ANIS_NO.             =0       01:PULSE_OUT_METHOD.    =4      02:ENC_INPUT_METHOD.    =3     03:INPOSITION.          =2
    // 04:ALARM.               =0       05:NEG_END_LIMIT.       =0      06:POS_END_LIMIT.       =0     07:MIN_VELOCITY.        =1
    // 08:MAN_VELOCITY.        =700000  09:HOME_SIGNAL.         =4      10:HOME_LEVEL.          =1     11:HOME_DIR.            =0
    // 12:ZPHASE_LEVEL.        =1       13:ZPHASE_USE.          =0      14:STOP_SIGNAL_MODE.    =0     15:STOP_SIGNAL_LEVEL.   =0
    // 16:HOME_FIRST_VELOCITY. =10000   17:HOME_SECOND_VELOCITY.=10000  18:HOME_THIRD_VELOCITY. =2000  19:HOME_LAST_VELOCITY.  =100
    // 20:HOME_FIRST_ACCEL.    =40000   21:HOME_SECOND_ACCEL.   =40000  22:HOME_END_CLEAR_TIME. =1000  23:HOME_END_OFFSET.     =0
    // 24:NEG_SOFT_LIMIT.      =0.000   25:POS_SOFT_LIMIT.      =0      26:MOVE_PULSE.          =1     27:MOVE_UNIT.           =1
    // 28:INIT_POSITION.       =1000    29:INIT_VELOCITY.       =200    30:INIT_ACCEL.          =400   31:INIT_DECEL.          =400
    // 32:INIT_ABSRELMODE.     =0       33:INIT_PROFILEMODE.    =4

    // 00=[ANIS_NO             ]: �� (0�� ���� ������)
    // 01=[PULSE_OUT_METHOD    ]: Pulse out method TwocwccwHigh = 6
    // 02=[ENC_INPUT_METHOD    ]: disable = 0   1ü�� = 1  2ü�� = 2  4ü�� = 3, �ἱ ���ù��� ��ü��(-).1ü�� = 11  2ü�� = 12  4ü�� = 13
    // 03=[INPOSITION          ], 04=[ALARM     ], 05,06 =[END_LIMIT   ]  : 0 = A���� 1= B���� 2 = ������. 3 = �������� ����
    // 07=[MIN_VELOCITY        ]: ���� �ӵ�(START VELOCITY)
    // 08=[MAN_VELOCITY        ]: ����̹��� ������ �޾Ƶ��ϼ� �ִ� ���� �ӵ�. ���� �Ϲ� Servo�� 700k
    // Ex> screw : 20mm pitch drive: 10000 pulse ����: 400w
    // 09=[HOME_SIGNAL         ]: 4 - Home in0 , 0 :PosEndLimit , 1 : NegEndLimit // _HOME_SIGNAL����.
    // 10=[HOME_LEVEL          ]: 0 = A���� 1= B���� 2 = ������. 3 = �������� ����
    // 11=[HOME_DIR            ]: Ȩ ����(HOME DIRECTION) 1:+����, 0:-����
    // 12=[ZPHASE_LEVEL        ]: 0 = A���� 1= B���� 2 = ������. 3 = �������� ����
    // 13=[ZPHASE_USE          ]: Z���뿩��. 0: ������ , 1: +����, 2: -���� 
    // 14=[STOP_SIGNAL_MODE    ]: ESTOP, SSTOP ���� ��� 0:��������, 1:������ 
    // 15=[STOP_SIGNAL_LEVEL   ]: ESTOP, SSTOP ��� ����.  0 = A���� 1= B���� 2 = ������. 3 = �������� ���� 
    // 16=[HOME_FIRST_VELOCITY ]: 1�������ӵ� 
    // 17=[HOME_SECOND_VELOCITY]: �����ļӵ� 
    // 18=[HOME_THIRD_VELOCITY ]: ������ �ӵ� 
    // 19=[HOME_LAST_VELOCITY  ]: index�˻��� �����ϰ� �˻��ϱ����� �ӵ�. 
    // 20=[HOME_FIRST_ACCEL    ]: 1�� ���ӵ� , 21=[HOME_SECOND_ACCEL   ] : 2�� ���ӵ� 
    // 22=[HOME_END_CLEAR_TIME ]: ���� �˻� Enc �� Set�ϱ� ���� ���ð�,  23=[HOME_END_OFFSET] : ���������� Offset��ŭ �̵�.
    // 24=[NEG_SOFT_LIMIT      ]: - SoftWare Limit ���� �����ϸ� ������, 25=[POS_SOFT_LIMIT ]: + SoftWare Limit ���� �����ϸ� ������.
    // 26=[MOVE_PULSE          ]: ����̹��� 1ȸ���� �޽���              , 27=[MOVE_UNIT  ]: ����̹� 1ȸ���� �̵��� ��:��ũ�� Pitch
    // 28=[INIT_POSITION       ]: ������Ʈ ���� �ʱ���ġ  , ����ڰ� ���Ƿ� ��밡��
    // 29=[INIT_VELOCITY       ]: ������Ʈ ���� �ʱ�ӵ�  , ����ڰ� ���Ƿ� ��밡��
    // 30=[INIT_ACCEL          ]: ������Ʈ ���� �ʱⰡ�ӵ�, ����ڰ� ���Ƿ� ��밡��
    // 31=[INIT_DECEL          ]: ������Ʈ ���� �ʱⰨ�ӵ�, ����ڰ� ���Ƿ� ��밡��
    // 32=[INIT_ABSRELMODE     ]: ����(0)/���(1) ��ġ ����
    // 33=[INIT_PROFILEMODE    ]: �������ϸ��(0 - 4) ���� ����
    //                            '0': ��Ī Trapezode, '1': ���Ī Trapezode, '2': ��Ī Quasi-S Curve, '3':��Ī S Curve, '4':���Ī S Curve
    
    // AnmMotSaveParaAll�� ���� �Ǿ��� .mot������ �ҷ��´�. �ش� ������ ����ڰ� Edit �Ͽ� ��� �����ϴ�.
    DWORD    __stdcall AnmMotLoadParaAll(char *szFilePath);
    // ����࿡ ���� ��� �Ķ��Ÿ�� �ະ�� �����Ѵ�. .mot���Ϸ� �����Ѵ�.
    DWORD    __stdcall AnmMotSaveParaAll(char *szFilePath);

    // �Ķ��Ÿ 28 - 31������ ����ڰ� ���α׷�������  �� �Լ��� �̿��� ���� �Ѵ�
    DWORD    __stdcall AnmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel);    
    // �Ķ��Ÿ 28 - 31������ ����ڰ� ���α׷�������  �� �Լ��� �̿��� Ȯ�� �Ѵ�.
    DWORD    __stdcall AnmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel);    

    // ���� ���� �޽� ��� ����� �����Ѵ�.
    //uMethod  0 :OneHighLowHigh, 1 :OneHighHighLow, 2 :OneLowLowHigh, 3 :OneLowHighLow, 4 :TwoCcwCwHigh
    //         5 :TwoCcwCwLow,    6 :TwoCwCcwHigh,   7 :TwoCwCcwLow,   8 :TwoPhase,      9 :TwoPhaseReverse
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

    DWORD    __stdcall AnmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod);
    // ���� ���� �޽� ��� ��� ������ ��ȯ�Ѵ�,
    DWORD    __stdcall AnmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod);

    // ���� ���� �ܺ�(Actual) ī��Ʈ�� ���� ���� ������ �����Ͽ� ���� ���� Encoder �Է� ����� �����Ѵ�.
    // uMethod : 0 - 7 ����.
    // ObverseUpDownMode          = 0x0    // ������ Up/Down
    // ObverseSqr1Mode            = 0x1    // ������ 1ü��
    // ObverseSqr2Mode            = 0x2    // ������ 2ü��
    // ObverseSqr4Mode            = 0x3    // ������ 4ü��
    // ReverseUpDownMode          = 0x4    // ������ Up/Down
    // ReverseSqr1Mode            = 0x5    // ������ 1ü��
    // ReverseSqr2Mode            = 0x6    // ������ 2ü��
  // ReverseSqr4Mode              = 0x7    // ������ 4ü��
    DWORD    __stdcall AnmMotSetEncInputMethod(long lAxisNo, DWORD uMethod);
    // ���� ���� �ܺ�(Actual) ī��Ʈ�� ���� ���� ������ �����Ͽ� ���� ���� Encoder �Է� ����� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod);

    // ���� �ӵ� ������ RPM(Revolution Per Minute)���� ���߰� �ʹٸ�.
    // ex>    rpm ���:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1�̸�      pulse/ sec �ʴ� �޽����� �Ǵµ�
    // 4500 rpm�� ���߰� �ʹٸ�     4500 / 60 �� : 75ȸ��/ 1��
    // ���Ͱ� 1ȸ���� �� �޽����� �˾ƾ� �ȴ�. �̰��� Encoder�� Z���� �˻��غ��� �˼��ִ�.
    // 1ȸ��:1800 �޽���� 75 x 1800 = 135000 �޽��� �ʿ��ϰ� �ȴ�.
    // AnmMotSetMoveUnitPerPulse�� Unit = 1, Pulse = 1800 �־� ���۽�Ų��.
    // �������� : rpm���� �����ϰ� �ȴٸ� �ӵ��� ���ӵ� �� rpm������ �ٲ�� �ȴ�.

    // ���� ���� �޽� �� �����̴� �Ÿ��� �����Ѵ�.
    DWORD    __stdcall AnmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse);
    // ���� ���� �޽� �� �����̴� �Ÿ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse);
    
    // ���� �࿡ ���� ���� ����Ʈ ���� ����� �����Ѵ�.
    //uMethod : 0 -1 ����
    // AutoDetect = 0x0 : �ڵ� ������.
    // RestPulse  = 0x1 : ���� ������."

    DWORD    __stdcall AnmMotSetDecelMode(long lAxisNo, DWORD uMethod);
    // ���� ���� ���� ���� ����Ʈ ���� ����� ��ȯ�Ѵ�
    DWORD    __stdcall AnmMotGetDecelMode(long lAxisNo, DWORD *upMethod);
    
    // ���� �࿡ ���� ���� ��忡�� �ܷ� �޽��� �����Ѵ�.
    // �����: ���� AnmMotSetRemainPulse�� 500 �޽��� ����
    //           AnmMoveStartPos�� ��ġ 10000�� ��������쿡 9500�޽����� 
    //           ���� �޽� 500��  AnmMotSetMinVel�� ������ �ӵ��� �����ϸ鼭 ���� �ȴ�.
    DWORD    __stdcall AnmMotSetRemainPulse(long lAxisNo, DWORD uData);
    // ���� ���� ���� ���� ��忡�� �ܷ� �޽��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetRemainPulse(long lAxisNo, DWORD *upData);

    // ���� �࿡ ��ӵ� ���� �Լ������� �ְ� �ӵ��� �����Ѵ�.
    DWORD    __stdcall AnmMotSetMaxVel(long lAxisNo, double dVel);
    // ���� ���� ��ӵ� ���� �Լ������� �ְ� �ӵ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetMaxVel(long lAxisNo, double *dpVel);

    // ���� ���� �̵� �Ÿ� ��� ��带 �����Ѵ�.
    //uAbsRelMode : POS_ABS_MODE '0' - ���� ��ǥ��
    //              POS_REL_MODE '1' - ��� ��ǥ��
    DWORD    __stdcall AnmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode);
    // ���� ���� ������ �̵� �Ÿ� ��� ��带 ��ȯ�Ѵ�
    DWORD    __stdcall AnmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode);

    //���� ���� ���� �ӵ� �������� ��带 �����Ѵ�.
    //ProfileMode : SYM_TRAPEZOIDE_MODE    '0' - ��Ī Trapezode
    //              ASYM_TRAPEZOIDE_MODE   '1' - ���Ī Trapezode
    //              QUASI_S_CURVE_MODE     '2' - ��������
    //              SYM_S_CURVE_MODE       '3' - ��Ī S Curve
    //              ASYM_S_CURVE_MODE      '4' - ���Ī S Curve
    DWORD    __stdcall AnmMotSetProfileMode(long lAxisNo, DWORD uProfileMode);
    // ���� ���� ������ ���� �ӵ� �������� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode);
    
    //���� ���� ���ӵ� ������ �����Ѵ�.
    //AccelUnit : UNIT_SEC2   '0' - ������ ������ unit/sec2 ���
    //            SEC         '1' - ������ ������ sec ���
    DWORD    __stdcall AnmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit);
    // ���� ���� ������ ���ӵ������� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit);

    // ���� �࿡ �ʱ� �ӵ��� �����Ѵ�.
    DWORD    __stdcall AnmMotSetMinVel(long lAxisNo, double dMinVel);
    // ���� ���� �ʱ� �ӵ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetMinVel(long lAxisNo, double *dpMinVel);

    // ���� ���� ���� ��ũ���� �����Ѵ�.[%].
    DWORD    __stdcall AnmMotSetAccelJerk(long lAxisNo, double dAccelJerk);
    // ���� ���� ������ ���� ��ũ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk);

    // ���� ���� ���� ��ũ���� �����Ѵ�.[%].
    DWORD    __stdcall AnmMotSetDecelJerk(long lAxisNo, double dDecelJerk);
    // ���� ���� ������ ���� ��ũ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk);
    
//=========== ����� ��ȣ ���� �����Լ� ================================================================================

    // ���� ���� Z �� Level�� �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Z �� Level�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel);

    // ���� ���� Servo-On��ȣ�� ��� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Servo-On��ȣ�� ��� ���� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel);

    // ���� ���� Servo-Alarm Reset ��ȣ�� ��� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Servo-Alarm Reset ��ȣ�� ��� ������ ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel);

    //    ���� ���� Inpositon ��ȣ ��� ���� �� ��ȣ �Է� ������ �����Ѵ�
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetInpos(long lAxisNo, DWORD uUse);
    // ���� ���� Inpositon ��ȣ ��� ���� �� ��ȣ �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalGetInpos(long lAxisNo, DWORD *upUse);
    // ���� ���� Inpositon ��ȣ �Է� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadInpos(long lAxisNo, DWORD *upStatus);

    //    ���� ���� �˶� ��ȣ �Է� �� ��� ������ ��� ���� �� ��ȣ �Է� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetServoAlarm(long lAxisNo, DWORD uUse);
    // ���� ���� �˶� ��ȣ �Է� �� ��� ������ ��� ���� �� ��ȣ �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalGetServoAlarm(long lAxisNo, DWORD *upUse);
    // ���� ���� �˶� ��ȣ�� �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus);

    // ���� ���� end limit sensor�� ��� ���� �� ��ȣ�� �Է� ������ �����Ѵ�. 
    // end limit sensor ��ȣ �Է� �� �������� �Ǵ� �������� ���� ������ �����ϴ�.
    // uStopMode: EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uPositiveLevel, uNegativeLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel);
    // ���� ���� end limit sensor�� ��� ���� �� ��ȣ�� �Է� ����, ��ȣ �Է� �� ������带 ��ȯ�Ѵ�
    DWORD    __stdcall AnmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel);
    // �������� end limit sensor�� �Է� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);

    // ���� ���� Software limit�� ��� ����, ����� ī��Ʈ, �׸��� ���� ����� �����Ѵ�
    // uUse       : DISABLE(0), ENABLE(1)
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uSelection : COMMAND(0), ACTUAL(1)
    // ���ǻ���: �����˻��� ���Լ��� �̿��Ͽ� ����Ʈ���� ������ �̸� �����ؼ� ������ �����˻��� �����˻��� ���߿� ���߾�������쿡��  Enable�ȴ�. 
    
    DWORD    __stdcall AnmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos);
    // ���� ���� Software limit�� ��� ����, ����� ī��Ʈ, �׸��� ���� ����� ��ȯ�Ѵ�
    DWORD    __stdcall AnmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos);

    // ��� ���� ��ȣ�� ���� ��� (������/��������) �Ǵ� ��� ������ �����Ѵ�.
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AnmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel);
    // ��� ���� ��ȣ�� ���� ��� (������/��������) �Ǵ� ��� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel);
    // ��� ���� ��ȣ�� �Է� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadStop(long lAxisNo, DWORD *upStatus);

    // ���� ���� Servo-On ��ȣ�� ����Ѵ�.
    // uOnOff : FALSE(0), TRUE(1) ( ���� 0��¿� �ش��)
    DWORD    __stdcall AnmSignalServoOn(long lAxisNo, DWORD uOnOff);
    // ���� ���� Servo-On ��ȣ�� ��� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalIsServoOn(long lAxisNo, DWORD *upOnOff);

    // ���� ���� Servo-Alarm Reset ��ȣ�� ����Ѵ�.
    // uOnOff : FALSE(0), TRUE(1) ( ���� 1��¿� �ش��)
    DWORD    __stdcall AnmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff);
    
    //    ���� ��°��� �����Ѵ�.
    //  uValue : Hex Value 0x00
    DWORD    __stdcall AnmSignalWriteOutput(long lAxisNo, DWORD uValue);
    // ���� ��°��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadOutput(long lAxisNo, DWORD *upValue);

    // lBitNo : Bit Number(0 - 4)
    // uOnOff : FALSE(0), TRUE(1)
    // ���� ��°��� ��Ʈ���� �����Ѵ�.
    DWORD    __stdcall AnmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff);
    // ���� ��°��� ��Ʈ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff);

    // ���� �Է°��� Hex������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadInput(long lAxisNo, DWORD *upValue);
    
    // lBitNo : Bit Number(0 - 4)
    // ���� �Է°��� ��Ʈ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn);

//========== ��� ������ �� �����Ŀ� ���� Ȯ���ϴ� �Լ�============================================================

    // ���� ���� �޽� ��� ���¸� ��ȯ�Ѵ�.
    // (��������)"
    DWORD    __stdcall AnmStatusReadInMotion(long lAxisNo, DWORD *upStatus);

    // �������� ���� ���� ���� ���� �޽� ī���� ���� ��ȯ�Ѵ�.
    // ���ǻ���: �����߿��� ī���Ͱ��� ǥ���ϰ� ���������Ŀ��� ī���Ͱ��� CLEAR�ȴ�.    
    //  (�޽� ī��Ʈ ��)"
    DWORD    __stdcall AnmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse);
    
    // ���� ���� DriveStatus(����� ����) �������͸� ��ȯ�Ѵ�
    DWORD    __stdcall AnmStatusReadMotion(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� EndStatus(���� ����) �������͸� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmStatusReadStop(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� Mechanical Signal Data(���� ������� ��ȣ����) �� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmStatusReadMechanical(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� ���� ���� �ӵ��� �о�´�.
    DWORD    __stdcall AnmStatusReadVel(long lAxisNo, double *dpVel);
    
    // ���� ���� Command Pos�� Actual Pos�� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmStatusReadPosError(long lAxisNo, double *dpError);
    
    // ���� ����̺�� �̵��ϴ�(�̵���) �Ÿ��� Ȯ�� �Ѵ�
    DWORD    __stdcall AnmStatusReadDriveDistance(long lAxisNo, double *dpUnit);

    // ���� ���� Actual ��ġ�� �����Ѵ�.
    DWORD    __stdcall AnmStatusSetActPos(long lAxisNo, double dPos);
    // ���� ���� Actual ��ġ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmStatusGetActPos(long lAxisNo, double *dpPos);

    // ���� ���� Command ��ġ�� �����Ѵ�.
    DWORD    __stdcall AnmStatusSetCmdPos(long lAxisNo, double dPos);
    // ���� ���� Command ��ġ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmStatusGetCmdPos(long lAxisNo, double *dpPos);

//======== Ȩ���� �Լ�=============================================================================================================================================================================================    

    // ���� ���� Home ���� Level �� �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AnmHomeSetSignalLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Home ���� Level �� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel);
    // ���� Ȩ ��ȣ �Է»��¸� Ȯ���Ѵ�. Ȩ��ȣ�� ����ڰ� ���Ƿ� AnmHomeSetMethod �Լ��� �̿��Ͽ� �����Ҽ��ִ�.
    // �Ϲ������� Ȩ��ȣ�� ���� �Է� 0�� ����ϰ������� AnmHomeSetMethod �̿��ؼ� �ٲٸ� + , - Limit�� ����Ҽ����ִ�.
    // upStatus : OFF(0), ON(1)
    DWORD    __stdcall AnmHomeReadSignal(long lAxisNo, DWORD *upStatus);
    
    // �ش� ���� �����˻��� �����ϱ� ���ؼ��� �ݵ�� ���� �˻����� �Ķ��Ÿ���� �����Ǿ� �־�� �˴ϴ�. 
    // ���� MotionPara���� ������ �̿��� �ʱ�ȭ�� ���������� ����ƴٸ� ������ ������ �ʿ����� �ʴ�. 
    // �����˻� ��� �������� �˻� �������, �������� ����� ��ȣ, �������� Active Level, ���ڴ� Z�� ���� ���� ���� ���� �Ѵ�.
    // ���ǻ��� : ������ �߸� ������ -�������� �����ص�  +�������� �����Ҽ� ������, Ȩ�� ã�µ� �־� ������ �ɼ��ִ�.
    // (�ڼ��� ������ AnmMotSaveParaAll ���� �κ� ����)
    // Ȩ������ AnmSignalSetHomeLevel ����Ѵ�.
    // HClrTim : HomeClear Time : ���� �˻� Encoder �� Set�ϱ� ���� ���ð� 
    // HmDir(Ȩ ����): DIR_CCW (0) -���� , DIR_CW(1) +����
    // HOffset - ���������� �̵��Ÿ�.
    // uZphas: 1�� �����˻� �Ϸ� �� ���ڴ� Z�� ���� ���� ����  0: ������ , 1: +����, 2: -���� 
    // HmSig:   PosEndLimit(0) -> +Limit
    //          NegEndLimit(1) -> -Limit
    //          HomeSensor (4) -> ��������(���� �Է� 0)

    DWORD   __stdcall AnmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
    // �����Ǿ��ִ� Ȩ ���� �Ķ��Ÿ���� ��ȯ�Ѵ�.
    DWORD   __stdcall AnmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset);

    // ������ ������ �����ϰ� �˻��ϱ� ���� ���� �ܰ��� �������� �����Ѵ�. �̶� �� ���ǿ� ��� �� �ӵ��� �����Ѵ�. 
    // �� �ӵ����� �������� ���� �����˻� �ð���, �����˻� ���е��� �����ȴ�. 
    // �� ���Ǻ� �ӵ����� ������ �ٲ㰡�鼭 �� ���� �����˻� �ӵ��� �����ϸ� �ȴ�. 
    // (�ڼ��� ������ AnmMotSaveParaAll ���� �κ� ����)
    // �����˻��� ���� �ӵ��� �����ϴ� �Լ�
    // [dVelFirst]- 1�������ӵ�   [dVelSecond]-�����ļӵ�   [dVelThird]- ������ �ӵ�  [dvelLast]- index�˻��� �����ϰ� �˻��ϱ�����. 
    // [dAccFirst]- 1���������ӵ� [dAccSecond]-�����İ��ӵ� 
    DWORD    __stdcall AnmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
    // �����Ǿ��ִ� �����˻��� ���� �ӵ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond);

    // �����˻��� �����Ѵ�.
    // �����˻� �����Լ��� �����ϸ� ���̺귯�� ���ο��� �ش����� �����˻��� ���� �� �����尡 �ڵ� �����Ǿ� �����˻��� ���������� ������ �� �ڵ� ����ȴ�.
    // ���ǻ��� : �������� �ݴ������ ����Ʈ ������ ���͵� ��������� ������ ACTIVE���������� �����Ѵ�.
    //          ���� �˻��� ���۵Ǿ� ��������� ����Ʈ ������ ������ ����Ʈ ������ �����Ǿ��ٰ� �����ϰ� �����ܰ�� ����ȴ�.

    DWORD    __stdcall AnmHomeSetStart(long lAxisNo);
    // �����˻� ����� ����ڰ� ���Ƿ� �����Ѵ�.
    // �����˻� �Լ��� �̿��� ���������� �����˻��� ����ǰ��� �˻� ����� HOME_SUCCESS�� �����˴ϴ�.
    // �� �Լ��� ����ڰ� �����˻��� ���������ʰ� ����� ���Ƿ� ������ �� �ִ�. 
    // uHomeResult ����
    // HOME_SUCCESS               = 0x01    // Ȩ �Ϸ�
    // HOME_SEARCHING             = 0x02    // Ȩ�˻���
    // HOME_ERR_GNT_RANGE         = 0x10    // Ȩ �˻� ������ ��������
    // HOME_ERR_USER_BREAK        = 0x11    // �ӵ� ������ ���Ƿ� ��������� ���������
    // HOME_ERR_VELOCITY          = 0x12    // �ӵ� ���� �߸��������
    // HOME_ERR_AMP_FAULT         = 0x13    // ������ �˶� �߻� ����
    // HOME_ERR_NEG_LIMIT         = 0x14    // (-)���� ������ (+)����Ʈ ���� ���� ����
    // HOME_ERR_POS_LIMIT         = 0x15    // (+)���� ������ (-)����Ʈ ���� ���� ����
    // HOME_ERR_NOT_DETECT        = 0x16    // ������ ��ȣ �������� �� �� ��� ����
    // HOME_ERR_UNKNOWN           = 0xFF    
    DWORD    __stdcall AnmHomeSetResult(long lAxisNo, DWORD uHomeResult);
    // �����˻� ����� ��ȯ�Ѵ�.
    // �����˻� �Լ��� �˻� ����� Ȯ���Ѵ�. �����˻��� ���۵Ǹ� HOME_SEARCHING���� �����Ǹ� �����˻��� �����ϸ� ���п����� �����ȴ�. ���� ������ ������ �� �ٽ� �����˻��� �����ϸ� �ȴ�.
    DWORD    __stdcall AnmHomeGetResult(long lAxisNo, DWORD *upHomeResult);

    // �����˻� ������� ��ȯ�Ѵ�.
    // �����˻� ���۵Ǹ� �������� Ȯ���� �� �ִ�. �����˻��� �Ϸ�Ǹ� �������ο� ������� 100�� ��ȯ�ϰ� �ȴ�. �����˻� �������δ� GetHome Result�Լ��� �̿��� Ȯ���� �� �ִ�.
    // upHomeMainStepNumber : Main Step �������̴�. 
    // ��Ʈ�� FALSE�� ���upHomeMainStepNumber : 0 �϶��� ������ �ุ ��������̰� Ȩ �������� upHomeStepNumber ǥ���Ѵ�.
    // ��Ʈ�� TRUE�� ��� upHomeMainStepNumber : 0 �϶��� ������ Ȩ�� ��������̰� ������ Ȩ �������� upHomeStepNumber ǥ���Ѵ�.
    // ��Ʈ�� TRUE�� ��� upHomeMainStepNumber : 10 �϶��� �����̺� Ȩ�� ��������̰� ������ Ȩ �������� upHomeStepNumber ǥ���Ѵ�.
    // upHomeStepNumber     : ������ �࿡���� �������� ǥ���Ѵ�. 
    // ��Ʈ�� FALSE�� ���  : ������ �ุ �������� ǥ���Ѵ�.
    // ��Ʈ�� TRUE�� ��� ��������, �����̺��� ������ �������� ǥ�õȴ�.
    DWORD    __stdcall AnmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber);

//========= ��ġ �����Լ� ===============================================================================================================
    
    // ���ǻ���: ��ġ�� �����Ұ�� �ݵ�� UNIT/PULSE�� ���߾ �����Ѵ�.
    //           ��ġ�� UNIT/PULSE ���� �۰��� ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� ����ġ���� ������ �ɼ�����.

    // ���� �ӵ� ������ RPM(Revolution Per Minute)���� ���߰� �ʹٸ�.
    // ex>    rpm ���:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1�̸�      pulse/ sec �ʴ� �޽����� �Ǵµ�
    // 4500 rpm�� ���߰� �ʹٸ�     4500 / 60 �� : 75ȸ��/ 1��
    // ���Ͱ� 1ȸ���� �� �޽����� �˾ƾ� �ȴ�. �̰��� Encoder�� Z���� �˻��غ��� �˼��ִ�.
    // 1ȸ��:1800 �޽���� 75 x 1800 = 135000 �޽��� �ʿ��ϰ� �ȴ�.
    // AnmMotSetMoveUnitPerPulse�� Unit = 1, Pulse = 1800 �־� ���۽�Ų��. 

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/ �����ǥ �� ������ ��ġ���� ������ �ӵ��� �������� ������ �Ѵ�.
    // �ӵ� ���������� AnmMotSetProfileMode �Լ����� �����Ѵ�.
    // �޽��� ��µǴ� �������� �Լ��� �����.
    // AnmMotSetAccelUnit(lAxisNo, 1) �ϰ�� dAccel -> dAccelTime , dDecel -> dDecelTime ���� �ٲ��.
    DWORD    __stdcall AnmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/�����ǥ�� ������ ��ġ���� ������ �ӵ��� �������� ������ �Ѵ�.
    // �ӵ� ���������� AnmMotSetProfileMode �Լ����� �����Ѵ�. 
    // �޽� ����� ����Ǵ� �������� �Լ��� �����
    DWORD    __stdcall AnmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

    // ������ �ӵ��� �����Ѵ�.
    // ���� �࿡ ���Ͽ� ������ �ӵ��� �������� ���������� �ӵ� ��� ������ �Ѵ�. 
    // �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
    // Vel���� ����̸� CW, �����̸� CCW �������� ����.
    DWORD    __stdcall AnmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel);

    // ������ ���࿡ ���Ͽ� ������ �ӵ��� �������� ���������� �ӵ� ��� ������ �Ѵ�.
    // �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
    // Vel���� ����̸� CW, �����̸� CCW �������� ����.
    DWORD    __stdcall AnmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

    // Ư�� Input ��ȣ�� Edge�� �����Ͽ� ������ �Ǵ� ���������ϴ� �Լ�.
    // lDetect Signal : edge ������ �Է� ��ȣ ����.
    // lDetectSignal  : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // Signal Edge    : ������ �Է� ��ȣ�� edge ���� ���� (rising or falling edge).
    //                  SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // ��������       : Vel���� ����̸� CW, �����̸� CCW.
    // SignalMethod   : ������ EMERGENCY_STOP(0), �������� SLOWDOWN_STOP(1)
    // ���ǻ���: SignalMethod�� EMERGENCY_STOP(0)�� ����Ұ�� �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
    //           lDetectSignal�� PosEndLimit , NegEndLimit(0,1) �� ã����� ��ȣ�Ƿ��� Active ���¸� �����ϰԵȴ�.
    DWORD    __stdcall AnmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);
    
    // ���� �࿡�� ������ ��ȣ�� �����ϰ� �� ��ġ�� �����ϱ� ���� �̵��ϴ� �Լ��̴�.
    // ���ϴ� ��ȣ�� ��� ã�� �����̴� �Լ� ã�� ��� �� ��ġ�� ������ѳ��� AnmGetCapturePos����Ͽ� �װ��� �д´�.
    // Signal Edge   : ������ �Է� ��ȣ�� edge ���� ���� (rising or falling edge).
    //                 SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // ��������      : Vel���� ����̸� CW, �����̸� CCW.
    // SignalMethod  : ������ EMERGENCY_STOP(0), �������� SLOWDOWN_STOP(1)
    // lDetect Signal: edge ������ �Է� ��ȣ ����.SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // lDetectSignal : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // lTarget       : COMMAND(0), ACTUAL(1)
    // ���ǻ���: SignalMethod�� EMERGENCY_STOP(0)�� ����Ұ�� �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
    //           lDetectSignal�� PosEndLimit , NegEndLimit(0,1) �� ã����� ��ȣ�Ƿ��� Active ���¸� �����ϰԵȴ�.
    
    DWORD    __stdcall AnmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod);
    
    // 'AnmMoveSignalCapture' �Լ����� ����� ��ġ���� Ȯ���ϴ� �Լ��̴�.
    // ���ǻ���: �Լ� ���� ����� "ANT_RT_SUCCESS"�϶� ����� ��ġ�� ��ȿ�ϸ�, �� �Լ��� �ѹ� �����ϸ� ���� ��ġ���� �ʱ�ȭ�ȴ�.
    DWORD    __stdcall AnmMoveGetCapturePos(long lAxisNo, double *dpCapPotition);

    // "������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��ϴ� �Լ�.
    // �Լ��� �����ϸ� �ش� Motion ������ ������ �� Motion �� �Ϸ�ɶ����� ��ٸ��� �ʰ� �ٷ� �Լ��� ����������."
    DWORD    __stdcall AnmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);
    
    // ������ ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ����� ���� ��ǥ�� ������ ��ġ���� ������ �ӵ��� �������� ������ �Ѵ�.
    DWORD    __stdcall AnmMoveMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);

    // ���� ���� ������ ���ӵ��� ���� ���� �Ѵ�.
    // dDecel : ���� �� ��������
    DWORD    __stdcall AnmMoveStop(long lAxisNo, double dDecel);
    // ���� ���� �� ���� �Ѵ�.
    DWORD    __stdcall AnmMoveEStop(long lAxisNo);
    // ���� ���� ���� �����Ѵ�.
    DWORD    __stdcall AnmMoveSStop(long lAxisNo);

//========= �������̵� �Լ� ============================================================================

    // ��ġ �������̵� �Ѵ�.
    // ���� ���� ������ ����Ǳ� �� ������ ��� �޽� ���� �����Ѵ�.
    // ���ǻ���: �������̵��� ��ġ�� �������� ���� ������ ��ġ�� ���������� Relative ������ ��ġ������ �־��ش�.
    //           ���������� ���������� ��� �������̵带 ����Ҽ������� �ݴ�������� �������̵��Ұ�쿡�� �������̵带 ����Ҽ�����.
    DWORD    __stdcall AnmOverridePos(long lAxisNo, double dOverridePos);
    
    // ���� ���� �ӵ��������̵� �ϱ����� �������̵��� �ְ�ӵ��� �����Ѵ�.
    // ������ : �ӵ��������̵带 5���Ѵٸ� ���߿� �ְ� �ӵ��� �����ؾߵȴ�. 
    DWORD    __stdcall AnmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel);
    
    // �ӵ� �������̵� �Ѵ�.
    // ���� ���� ���� �߿� �ӵ��� ���� �����Ѵ�. (�ݵ�� ��� �߿� ���� �����Ѵ�.)
    // ������: AnmOverrideVel �Լ��� ����ϱ�����. AnmOverrideMaxVel �ְ�� �����Ҽ��ִ� �ӵ��� �����س��´�.
    // EX> �ӵ��������̵带 �ι��Ѵٸ� 
    // 1. �ΰ��߿� ���� �ӵ��� AnmOverrideMaxVel ���� �ְ� �ӵ��� ����.
    // 2. AnmMoveStartPos ���� ���� ���� ���� ��(Move�Լ� ��� ����)�� �ӵ��� ù��° �ӵ��� AnmOverrideVel ���� �����Ѵ�.
    // 3. ���� ���� ���� ��(Move�Լ� ��� ����)�� �ӵ��� �ι�° �ӵ��� AnmOverrideVel ���� �����Ѵ�.
    DWORD    __stdcall AnmOverrideVel(long lAxisNo, double dOverrideVel);

    // ���ӵ�, �ӵ�, ���ӵ���  �������̵� �Ѵ�.
    // ���� ���� ���� �߿� ���ӵ�, �ӵ�, ���ӵ��� ���� �����Ѵ�. (�ݵ�� ��� �߿� ���� �����Ѵ�.)
    // ������: AnmOverrideAccelVelDecel �Լ��� ����ϱ�����. AnmOverrideMaxVel �ְ�� �����Ҽ��ִ� �ӵ��� �����س��´�.
    // EX> �ӵ��������̵带 �ι��Ѵٸ� 
    // 1. �ΰ��߿� ���� �ӵ��� AnmOverrideMaxVel ���� �ְ� �ӵ��� ����.
    // 2. AnmMoveStartPos ���� ���� ���� ���� ��(Move�Լ� ��� ����)�� ���ӵ�, �ӵ�, ���ӵ��� ù��° �ӵ��� AnmOverrideAccelVelDecel ���� �����Ѵ�.
    // 3. ���� ���� ���� ��(Move�Լ� ��� ����)�� ���ӵ�, �ӵ�, ���ӵ��� �ι�° �ӵ��� AnmOverrideAccelVelDecel ���� �����Ѵ�.
    DWORD    __stdcall AnmOverrideAccelVelDecel(long lAxisNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel);
    
    // ��� �������� �ӵ� �������̵� �Ѵ�.
    // ��� ��ġ ������ �������̵��� �ӵ��� �Է½��� ����ġ���� �ӵ��������̵� �Ǵ� �Լ�
    // lTarget : COMMAND(0), ACTUAL(1)
    // ������: AnmOverrideVelAtPos �Լ��� ����ϱ�����. AnmOverrideMaxVel �ְ�� �����Ҽ��ִ� �ӵ��� �����س��´�.

    DWORD    __stdcall AnmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel,double dOverridePos, double dOverrideVel, long lTarget);
    
//========= ������, �����̺�  ����� ���� �Լ� ===========================================================================

    // Electric Gear ��忡�� Master ��� Slave ����� ���� �����Ѵ�.
    // dSlaveRatio : �������࿡ ���� �����̺��� ����( 0 : 0% , 0.5 : 50%, 1 : 100%)
    DWORD    __stdcall AnmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio);
    // Electric Gear ��忡�� ������ Master ��� Slave ����� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmLinkGetMode(long lMasterAxisNo, long *lpSlaveAxisNo, double *dpGearRatio);
    // Master ��� Slave�ణ�� ���ڱ��� ���� ���� �Ѵ�.
    DWORD    __stdcall AnmLinkResetMode(long lMasterAxisNo);

//======== ��Ʈ�� ���� �Լ�===========================================================================================================================================================
    // ��Ǹ���� �� ���� �ⱸ������ Link�Ǿ��ִ� ��Ʈ�� �����ý��� ��� �����Ѵ�. 
    // �� �Լ��� �̿��� Master���� ��Ʈ�� ����� �����ϸ� �ش� Slave���� Master��� ����Ǿ� �����˴ϴ�. 
    // ���� ��Ʈ�� ���� ���� Slave�࿡ ��������̳� ���� ��ɵ��� ������ ��� ���õ˴ϴ�.
    // uSlHomeUse     : �������� Ȩ��� ��� ( 0 - 2)
    //             (0 : �����̺��� Ȩ�� �����ϰ� ���������� Ȩ�� ã�´�.)
    //             (1 : �������� , �����̺��� Ȩ�� ã�´�. �����̺� dSlOffset �� �����ؼ� ������.)
    //             (2 : �������� , �����̺��� Ȩ�� ã�´�. �����̺� dSlOffset �� �����ؼ� ��������.)
    // dSlOffset      : �����̺��� �ɼ°�
    // dSlOffsetRange : �����̺��� �ɼ°� ������ ����
    // ���ǻ���: ��Ʈ�� ENABLE�� �����̺����� ����� AnmStatusReadMotion �Լ��� Ȯ���ϸ� True(Motion ���� ��)�� Ȯ�εǾ� �������̴�. 
    //           �����̺��࿡ AnmStatusReadMotion�� Ȯ�������� InMotion �� False�̸� Gantry Enable�� �ȵȰ��̹Ƿ� �˶� Ȥ�� ����Ʈ ���� ���� Ȯ���Ѵ�.
    DWORD    __stdcall AnmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange);

    // Slave���� Offset���� �˾Ƴ��¹��.
    // A. ������, �����̺긦 ��� �������� ��Ų��.         
    // B. AnmGantrySetEnable�Լ����� uSlHomeUse = 2�� ������ AnmHomeSetStart�Լ��� �̿��ؼ� Ȩ�� ã�´�. 
    // C. Ȩ�� ã�� ���� ���������� Command���� �о�� ��������� �����̺����� Ʋ���� Offset���� �����ִ�.
    // D. Offset���� �о AnmGantrySetEnable�Լ��� dSlOffset���ڿ� �־��ش�. 
    // E. dSlOffset���� �־��ٶ� �������࿡ ���� �����̺� �� ���̱⶧���� ��ȣ�� �ݴ�� -dSlOffset �־��ش�.
    // F. dSIOffsetRange �� Slave Offset�� Range ������ ���ϴµ� Range�� �Ѱ踦 �����Ͽ� �Ѱ踦 ����� ������ �߻���ų�� ����Ѵ�.        
    // G. AnmGantrySetEnable�Լ��� Offset���� �־�������  AnmGantrySetEnable�Լ����� uSlHomeUse = 1�� ������ AnmHomeSetStart�Լ��� �̿��ؼ� Ȩ�� ã�´�.         
    
    // ��Ʈ�� ������ �־� ����ڰ� ������ �Ķ��Ÿ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn);
    // ��� ����� �� ���� �ⱸ������ Link�Ǿ��ִ� ��Ʈ�� �����ý��� ��� �����Ѵ�.
    DWORD    __stdcall AnmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo);

//====�Ϲ� �����Լ� ============================================================================================================================================;
    //������ ��ǥ�迡 ���� �� ������ �����Ѵ�.
    //(����� ��ȣ�� 0 ���� ����))
    // ������: ������Ҷ��� �ݵ�� ���� ���ȣ�� ���� ���ں��� ū���ڸ� �ִ´�.
    //         ������ ���� �Լ��� ����Ͽ��� �� �������ȣ�� ���� ���ȣ�� ���� �� ���� lpAxesNo�� ���� ���ؽ��� �Է��Ͽ��� �Ѵ�.
    //         ������ ���� �Լ��� ����Ͽ��� �� �������ȣ�� �ش��ϴ� ���� ���ȣ�� �ٸ� ���̶�� �Ѵ�.
    //         ���� ���� �ٸ� Coordinate�� �ߺ� �������� ���ƾ� �Ѵ�.
    DWORD    __stdcall AnmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
    //������ ��ǥ�迡 ���� �� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);
    
    // ������ ��ǥ�迡 ���� �� ����/��� ��带 �����Ѵ�.
    // (������ : �ݵ�� ����� �ϰ� ��밡��)
    // ���� ���� �̵� �Ÿ� ��� ��带 �����Ѵ�.
    //uAbsRelMode : POS_ABS_MODE '0' - ���� ��ǥ��
    //              POS_REL_MODE '1' - ��� ��ǥ��
    DWORD    __stdcall AnmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
    // ������ ��ǥ�迡 ���� �� ����/��� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AnmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);

    // ������ ��ǥ�迡 ���� ���� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AnmContiIsMotion(long lCoord, DWORD *upInMotion);

    // ���ǻ���1: AnmContiSetAxisMap�Լ��� �̿��Ͽ� ������Ŀ� ������������� ������ �ϸ鼭 ����ؾߵȴ�.
    //           ��ȣ������ ��쿡�� �ݵ�� ������������� ��迭�� �־�� ���� �����ϴ�.
    
    // ���ǻ���2: ��ġ�� �����Ұ�� �ݵ�� ��������� �����̺� ���� UNIT/PULSE�� ���߾ �����Ѵ�.
    //           ��ġ�� UNIT/PULSE ���� �۰� ������ ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� ����ġ���� ������ �ɼ�����.

    // ���ǻ���3: ��ȣ ������ �Ұ�� �ݵ�� ��Ĩ������ ������ �ɼ������Ƿ� 

    // ���ǻ���4: ���� ���� ����/�߿� ������ ���� ����(+- Limit��ȣ, ���� �˶�, ������� ��)�� �߻��ϸ� 
    //            ���� ���⿡ ������� ������ �������� �ʰų� ���� �ȴ�.

    // ���� ���� �Ѵ�.
    // �������� �������� �����Ͽ� ���� ���� ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AnmContiBeginNode, AnmContiEndNode�� ���̻��� ������ ��ǥ�迡 �������� �������� �����Ͽ� ���� ���� �����ϴ� Queue�� �����Լ����ȴ�. 
    DWORD    __stdcall AnmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel);

    // 2�� ��ȣ���� �Ѵ�.
    // ������, �������� �߽����� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AnmContiBeginNode, AnmContiEndNode, �� ���̻��� ������ ��ǥ�迡 ������, �������� �߽����� �����Ͽ� �����ϴ� ��ȣ ���� Queue�� �����Լ����ȴ�.
    // lAxisNo = ���� �迭 , dCenterPos = �߽��� X,Y �迭 , dEndPos = ������ X,Y �迭.
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    
    DWORD   __stdcall AnmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

    // �߰���, �������� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AnmContiBeginNode, AnmContiEndNode�� ���̻��� ������ ��ǥ�迡 �߰���, �������� �����Ͽ� �����ϴ� ��ȣ ���� Queue�� �����Լ����ȴ�.
    // lAxisNo = ���� �迭 , dMidPos = �߰��� X,Y �迭 , dEndPos = ������ X,Y �迭, lArcCircle = ��ũ(0), ��(1)

    DWORD    __stdcall AnmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle);
    
    // ������, �������� �������� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AnmContiBeginNode, AnmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� ��ȣ ���� �����ϴ� Queue�� �����Լ����ȴ�.
    // lAxisNo = ���� �迭 , dRadius = ������, dEndPos = ������ X,Y �迭 , uShortDistance = ������(0), ū��(1)
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD   __stdcall AnmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);

    // ������, ȸ�������� �������� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AnmContiBeginNode, AnmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� ��ȣ ���� �����ϴ� Queue�� �����Լ����ȴ�.
    // lAxisNo = ���� �迭 , dCenterPos = �߽��� X,Y �迭 , dAngle = ����.
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD   __stdcall AnmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir);

//====================Ʈ���� �Լ� ===============================================================================================================================

    // ���ǻ���: Ʈ���� ��ġ�� �����Ұ�� �ݵ�� UNIT/PULSE�� ���߾ �����Ѵ�.
    //           ��ġ�� UNIT/PULSE ���� �۰��� ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� ����ġ�� ����Ҽ�����.

    // ���� �࿡ Ʈ���� ����� ��� ����, ��� ����, ��ġ �񱳱�, Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ��带 �����Ѵ�.
    // Ʈ���� ��� ����� ���ؼ��� ����  AnmTriggerSetTimeLevel �� ����Ͽ� ���� ��� ������ ���� �Ͽ��� �Ѵ�.
    //  dTrigTime  : Ʈ���� ��� �ð�(1usec - �ִ� 50msec ( 1 - 50000 ���� ����))
    //  upTriggerLevel : Ʈ���� ��� ���� ����    => LOW(0), HIGH(1)
    //  uSelect        : ����� ���� ��ġ            => COMMAND(0), ACTUAL(1)
    //  uInterrupt     : ���ͷ�Ʈ ����                => DISABLE(0), ENABLE(1)
    
    // ���� �࿡ Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ����, Ʈ���� ��¹���� �����Ѵ�.
     DWORD    __stdcall AnmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt);
    // ���� �࿡ Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ����, Ʈ���� ��¹���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt);
    
    // ���� ���� Ʈ���� ��� ����� �����Ѵ�.
    //  uMethod : PERIOD_MODE      0x0 : ���� ��ġ�� �������� dPos�� ��ġ �ֱ�� ����� �ֱ� Ʈ���� ���
    //            ABS_POS_MODE     0x1 : Ʈ���� ���� ��ġ���� Ʈ���� �߻�, ���� ��ġ ���

    //  dPos : �ֱ� ���ý� : ��ġ������ġ���� ����ϱ⶧���� �� ��ġ
    //         ���� ���ý� : ����� �� ��ġ, �� ��ġ�Ͱ����� ������ ����� ������. 
    //  ���ǻ���: AnmTriggerSetAbsPeriod�� �ֱ���� ������ ��� ó�� ����ġ�� ���� �ȿ� �����Ƿ� Ʈ���� ����� �ѹ� �߻��Ѵ�.

    DWORD    __stdcall AnmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos);
    // ���� �࿡ Ʈ���� ����� ��� ����, ��� ����, ��ġ �񱳱�, Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AnmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos);

    //  ����ڰ� ������ ������ġ���� ������ġ���� ������������ Ʈ���Ÿ� ��� �Ѵ�.
    DWORD    __stdcall AnmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos);
    // 'AnmTriggerSetBlock' �Լ��� ������ ���� �д´�..
    DWORD    __stdcall AnmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos);
    // ����ڰ� �� ���� Ʈ���� �޽��� ����Ѵ�.
    DWORD    __stdcall AnmTriggerOneShot(long lAxisNo);
    // ����ڰ� �� ���� Ʈ���� �޽��� ���� �ð� �Ŀ� ����Ѵ�.
    DWORD    __stdcall AnmTriggerSetTimerOneshot(long lAxisNo, long lmSec);
    // �Է��� ������ġ���� ������ �ش� ��ġ�� ������ Ʈ���� ��ȣ�� ����Ѵ�.
    DWORD    __stdcall AnmTriggerOnlyAbs(long lAxisNo,long lTrigNum, double* dpTrigPos);
    // Ʈ���� ��� ������ �ʱ�ȭ �Ѵ�.
    DWORD    __stdcall AnmTriggerSetReset(long lAxisNo); 

//======== CRC( �ܿ� �޽� Ŭ���� �Լ�)=====================================================================    
    //Level   : LOW(0), HIGH(1), UNUSED(2), USED(3) 
    //uMethod : �ܿ��޽� ���� ��� ��ȣ �޽� �� 2 - 6���� ��������.
    //          0: Don't care , 1: Don't care, 2: 500 uSec, 3: 1 mSec, 4: 10 mSec, 5: 50 mSec, 6: 100 mSec

    //���� �࿡ CRC ��ȣ ��� ���� �� ��� ������ �����Ѵ�.
    DWORD    __stdcall AnmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod);
    // ���� ���� CRC ��ȣ ��� ���� �� ��� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod);

    //uOnOff  : CRC ��ȣ�� Program���� �߻� ����  (FALSE(0),TRUE(1))

    // ���� �࿡ CRC ��ȣ�� ������ �߻� ��Ų��.
    DWORD    __stdcall AnmCrcSetOutput(long lAxisNo, DWORD uOnOff);
    // ���� ���� CRC ��ȣ�� ������ �߻� ���θ� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmCrcGetOutput(long lAxisNo, DWORD *upOnOff);

    //======MPG(Manual Pulse Generation) �Լ�===========================================================
    // lInputMethod  : 0-3 ���� ��������. 0:OnePhase, 2:TwoPhase2, 3:TwoPhase4
    // lDriveMode    : 0 �� �������� (0 :MPG ���Ӹ��)
    // MPGPos        : ������� ����.(Dont't care.)
    // MPGdenominator: MPG(���� �޽� �߻� ��ġ �Է�)���� �� ������ ��
    // dMPGnumerator : MPG(���� �޽� �߻� ��ġ �Է�)���� �� ���ϱ� ��
    // dwNumerator   : �ִ�(1 ����    64) ���� ���� ����
    // dwDenominator : �ִ�(1 ����  4096) ���� ���� ����
    // dMPGdenominator = 4096, MPGnumerator=1 �� �ǹ��ϴ� ���� 
    // MPG �ѹ����� 200�޽��� �״�� 1:1�� 1�޽��� ����� �ǹ��Ѵ�. 
    // ���� dMPGdenominator = 4096, MPGnumerator=2 �� �������� 1:2�� 2�޽��� ����� �������ٴ��ǹ��̴�. 
    // ���⿡ MPG PULSE = ((Numerator) * (Denominator)/ 4096 ) Ĩ���ο� ��³����� �����̴�.

    // ���� �࿡ MPG �Է¹��, ����̺� ���� ���, �̵� �Ÿ�, MPG �ӵ� ���� �����Ѵ�.
    DWORD    __stdcall AnmMPGSetEnable(long lAxisNo, long lInputMethod, long lDriveMode, double dMPGPos, double dVel, double dAccel);
    // ���� �࿡ MPG �Է¹��, ����̺� ���� ���, �̵� �Ÿ�, MPG �ӵ� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMPGGetEnable(long lAxisNo, long *lpInputMethod, long *lpDriveMode, double *dpMPGPos, double *dpVel, double *dAccel);

    // ���� �࿡ MPG ����̺� ���� ��忡�� ���޽��� �̵��� �޽� ������ �����Ѵ�.
    DWORD    __stdcall AnmMPGSetRatio(long lAxisNo, DWORD uMPGnumerator, DWORD uMPGdenominator);
    // ���� �࿡ MPG ����̺� ���� ��忡�� ���޽��� �̵��� �޽� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AnmMPGGetRatio(long lAxisNo, DWORD *upMPGnumerator, DWORD *upMPGdenominator);
    
    // ���� �࿡ MPG ����̺� ������ �����Ѵ�.
    DWORD    __stdcall AnmMPGReset(long lAxisNo);

//========= �ΰ���� �Լ� ============================================================================

    // ���� ���� ����� ����� ����� ���� ������ ���� ��� ���� ���¿� ���� ����ó�� ��� ����.
    //dwNetErrorAct :    '0' - ���� ��� ��� ����
    //                   '1' - ���� ����
    //                   '2' - �� ����
    DWORD    __stdcall AnmNetWorkErrorSetAction(long lAxisNo, DWORD dwNetErrorAct);
    // ���� ���� ����� ����� ����� ���� ������ ���� ��� ���� ���¿� ���� ����ó�� ��� Ȯ��.
    DWORD    __stdcall AnmNetWorkErrorGetAction(long lAxisNo, DWORD* dwpNetErrorAct);

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__ANT_ANM_H__
