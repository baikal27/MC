#ifndef __AXT_AXDEV_H__
#define __AXT_AXDEV_H__

#include "AXHD.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

	
    // 라이브러리 초기화(High resolution performance counter 기능을 사용할 수 없는 시스템에서 사용)
    DWORD    __stdcall AxlOpenTimer();

    // Board Number를 이용하여 Board Address 찾기
    DWORD    __stdcall AxlGetBoardAddress(long lBoardNo, DWORD *upBoardAddress);
    // Board Number를 이용하여 Board ID 찾기
    DWORD    __stdcall AxlGetBoardID(long lBoardNo, DWORD *upBoardID);
    // Board Number를 이용하여 Board Version 찾기
    DWORD    __stdcall AxlGetBoardVersion(long lBoardNo, DWORD *upBoardVersion);
    // Board Number와 Module Position을 이용하여 Module ID 찾기
    DWORD    __stdcall AxlGetModuleID(long lBoardNo, long lModulePos, DWORD *upModuleID);
    // Board Number와 Module Position을 이용하여 Module Version 찾기
    DWORD    __stdcall AxlGetModuleVersion(long lBoardNo, long lModulePos, DWORD *upModuleVersion);
    // Board Number와 Module Position을 이용하여 AIO Module Number 찾기
    DWORD    __stdcall AxaInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
    // Board Number와 Module Position을 이용하여 DIO Module Number 찾기
    DWORD    __stdcall AxdInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
    
//==Log
    // 지정한 모듈의 EzSpy에 로그 출력 여부 설정
    DWORD    __stdcall AxdLogSetModule(long lModuleNo, DWORD uUse);
    // 지정한 모듈의 EzSpy에 로그 출력 여부 확인
    DWORD    __stdcall AxdLogGetModule(long lModuleNo, DWORD *upUse);
    
    // 지정 축에 16bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 16bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // 지정 축에 32bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 32bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);

    /*
    // 지정 축에 QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandQi(long lAxisNo, QICOMMAND sCommand);
    // 지정 축에 8bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 8bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // 지정 축에 24bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 24bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);

    // 지정 축에 Port Data 가져오기 - IP
    DWORD    __stdcall AxmGetPortData(long lAxisNo,  WORD wOffset, DWORD *upData);
    // 지정 축에 Port Data Setting - IP
    DWORD    __stdcall AxmSetPortData(long lAxisNo, WORD wOffset, DWORD dwData);

    // 지정 축에 Port Data 가져오기 - QI
    DWORD    __stdcall AxmGetPortDataQi(long lAxisNo, WORD byOffset, DWORD *wpData);
    // 지정 축에 Port Data Setting - QI
    DWORD    __stdcall AxmSetPortDataQi(long lAxisNo, WORD byOffset, WORD wData);
        
    // 지정 축에 스크립트를 설정한다. - QI
    DWORD    __stdcall AxmSetScriptCaptionQi(long lAxisNo, long sc, DWORD event, DWORD cmd, DWORD data);
    // 지정 축에 스크립트를 반환한다. - QI
    DWORD    __stdcall AxmGetScriptCaptionQi(long lAxisNo, long sc, DWORD *event, DWORD *cmd, DWORD *data);
    */
    
    DWORD    __stdcall AxdoDoSaveToFlash(long lModuleNo);
    DWORD    __stdcall AxdoDoLoadToFlash(long lModuleNo);

    
    //======= 헬리컬 이동  ===========================================================================
    // 지정된 좌표계에 시작점, 종료점과 중심점을 지정하여 헬리컬 보간 구동하는 함수이다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 종료점과 중심점을 지정하여 헬리컬 연속보간 구동하는 함수이다. 
    // 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다)
    // dCenterPos = 중심점 X,Y  , dEndPos = 종료점 X,Y .
    
    // uCWDir   DIR_CCW(0): 반시계방향, DIR_CW(1) 시계방향
    DWORD    __stdcall AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
    
    // 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 헬리컬 보간 구동하는 함수이다. 
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 중간점, 종료점을 지정하여 헬리컬연속 보간 구동하는 함수이다. 
    // 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)
    // dMidPos = 중간점 X,Y  , dEndPos = 종료점 X,Y 
    DWORD    __stdcall AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel);
    
    // 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 헬리컬 보간 구동하는 함수이다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 헬리컬연속 보간 구동하는 함수이다. 
    // 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)
    // dRadius = 반지름, dEndPos = 종료점 X,Y  , uShortDistance = 작은원(0), 큰원(1)
    
    // uCWDir   DIR_CCW(0): 반시계방향, DIR_CW(1) 시계방향
    DWORD    __stdcall AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);
    
    // 지정된 좌표계에 시작점, 회전각도와 반지름을 지정하여 헬리컬 보간 구동하는 함수이다
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 회전각도와 반지름을 지정하여 헬리컬연속 보간 구동하는 함수이다. 
    // 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)
    // dCenterPos = 중심점 X,Y  , dAngle = 각도.
    // uCWDir   DIR_CCW(0): 반시계방향, DIR_CW(1) 시계방향
    DWORD    __stdcall AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
    
    //======== 스플라인 이동 =========================================================================== 
    
    // AxmContiBeginNode, AxmContiEndNode와 같이사용안함. 
    // 스플라인 연속 보간 구동하는 함수이다. 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다.
    // AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)    
    // lPosSize : 최소 3개 이상.
    // 2축으로 사용시 dPoZ값을 0으로 넣어주면 됨.
    // 3축으로 사용시 축맵핑을 3개및 dPosZ 값을 넣어준다.
    DWORD    __stdcall AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor);

    //====보간 함수 ============================================================================================================================================;
    // 지정된 좌표계에 연속보간 축 맵핑을 설정한다.
    // (축맵핑 번호는 0 부터 시작))
    // 주의점: 축맵핑할때는 반드시 실제 축번호가 작은 숫자부터 큰숫자를 넣는다.
    //         가상축 맵핑 함수를 사용하였을 때 가상축번호를 실제 축번호가 작은 값 부터 lpAxesNo의 낮은 인텍스에 입력하여야 한다.
    //         가상축 맵핑 함수를 사용하였을 때 가상축번호에 해당하는 실제 축번호가 다른 값이라야 한다.
    //         같은 축을 다른 Coordinate에 중복 맵핑하지 말아야 한다.
    DWORD    __stdcall AxmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
    //지정된 좌표계에 연속보간 축 맵핑을 반환한다.
    DWORD    __stdcall AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);
    
    // 지정된 좌표계에 연속보간 축 절대/상대 모드를 설정한다.
    // (주의점 : 반드시 축맵핑 하고 사용가능)
    // 지정 축의 이동 거리 계산 모드를 설정한다.
    // uAbsRelMode : POS_ABS_MODE '0' - 절대 좌표계
    //              POS_REL_MODE '1' - 상대 좌표계
    
    DWORD    __stdcall AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
    // 지정된 좌표계에 연속보간 축 절대/상대 모드를 반환한다.
    DWORD    __stdcall AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);
    
    // 지정된 좌표계에 보간 구동을 위한 내부 Queue가 비어 있는지 확인하는 함수이다.
    DWORD    __stdcall AxmContiReadFree(long lCoord, DWORD *upQueueFree);
    // 지정된 좌표계에 보간 구동을 위한 내부 Queue에 저장되어 있는 보간 구동 개수를 확인하는 함수이다.
    DWORD    __stdcall AxmContiReadIndex(long lCoord, long *lpQueueIndex);
    // 지정된 좌표계에 연속 보간 구동을 위해 저장된 내부 Queue를 모두 삭제하는 함수이다.
    DWORD    __stdcall AxmContiWriteClear(long lCoord);
    
    // 지정된 좌표계에 연속보간에서 수행할 작업들의 등록을 시작한다. 이함수를 호출한후,
    // AxmContiEndNode함수가 호출되기 전까지 수행되는 모든 모션작업은 실제 모션을 수행하는 것이 아니라 연속보간 모션으로 등록 되는 것이며,
    // AxmContiStart 함수가 호출될 때 비로소 등록된모션이 실제로 수행된다.
    DWORD    __stdcall AxmContiBeginNode(long lCoord);
    // 지정된 좌표계에서 연속보간을 수행할 작업들의 등록을 종료한다.
    DWORD    __stdcall AxmContiEndNode(long lCoord);
    
    // 연속 보간 시작 한다.
    // dwProfileset(0 : 연속 보간 사용, 1 : 프로파일 보간 사용, 2 : 자동 프로파일 보간, 3 : 속도보상 모드 사용) 
    DWORD    __stdcall AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle); 
    // 지정된 좌표계에 연속 보간 구동 중인지 확인하는 함수이다.
    DWORD    __stdcall AxmContiIsMotion(long lCoord, DWORD *upInMotion);
    // 지정된 좌표계에 연속 보간 구동 중 현재 구동중인 연속 보간 인덱스 번호를 확인하는 함수이다.
    DWORD    __stdcall AxmContiGetNodeNum(long lCoord, long *lpNodeNum);
    // 지정된 좌표계에 설정한 연속 보간 구동 총 인덱스 갯수를 확인하는 함수이다.
    DWORD    __stdcall AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum);

    // 라이브러리 초기화(랜 카드를 2개를 사용한 Ring 구조 네트웍 구성, Lancard가 2개 이상 장착되어야 한다.)
    DWORD    __stdcall AxlRingOpen();
    
    // 설정한 거리만큼 또는 위치까지 이동한다.
    // 지정 축의 절대 좌표/상대좌표로 설정된 위치까지 2단계의 속도값으로 가감속 한다.
    // 1) 설정된 가속 종료 속도까지 가속 2) 감속 시작 속도까지 자동 가감속 3) 감속 시작 속도에서 감속 시작후 정지.
    // 속도 프로파일은 AxmMotSetProfileMode 함수에서 설정한다. 
    // 펄스가 출력되는 시점에서 함수를 벗어난다.
    DWORD    __stdcall AxmMoveStartPos2(long lAxisNo, double dPos, double dAccelEndVel, double dDecelStrVel, double dAccel, double dDecel);

    // 설정한 거리만큼 또는 위치까지 이동한다.
    // 지정 축의 절대 좌표/상대좌표로 설정된 위치까지 2단계의 속도값으로 가감속 한다.
    // 1) 설정된 가속 종료 속도까지 가속 2) 감속 시작 속도까지 자동 가감속 3) 감속 시작 속도에서 감속 시작후 정지.
    // 속도 프로파일은 AxmMotSetProfileMode 함수에서 설정한다. 
    // 펄스 출력이 종료되는 시점에서 함수를 벗어난다
    DWORD    __stdcall AxmMovePos2(long lAxisNo, double dPos, double dAccelEndVel, double dDecelStrVel, double dAccel, double dDecel);
    
    // 지정 축에 설정된 특정 이벤트 발생시 미리 지정한 동작을 수행하도록 한다.
    // 지정 축에 감시 이벤트와 실행할 명령어, 사용할 데이타값을 설정한다.
    DWORD    __stdcall AxmEventSetOperation(long lAxisNo, DWORD uEvent, DWORD uCmd, DWORD uData);
    
    // 지정 축에 감시 이벤트와 실행할 명령어, 사용할 데이타값을 확인한다.
    DWORD    __stdcall AxmEventGetOperation(long lAxisNo,  DWORD *upEvent, DWORD *upCmd, DWORD *upData);

    // 지정 축에 이벤트 감시 명령어 수행으로 캡쳐된 값을 확인 한다.
    DWORD    __stdcall AxmEventGetCapture(long lAxisNo, DWORD *upCapData);

    // 지정 축에 이벤트 감시 명령어 수행 기능 사용 여부를 설정 한다.
    // uUse       : DISABLE(0), ENABLE(1)
    DWORD    __stdcall AxmEventSetUse(long lAxisNo, DWORD uUse);

    // 지정 축에 이벤트 감시 명령어 수행 기능 사용 여부를 확인 한다.
    DWORD    __stdcall AxmEventGetUse(long lAxisNo, DWORD *upUse);
    

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXDEV_H__
