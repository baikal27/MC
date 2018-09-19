#ifndef __AXT_AXDEV_H__
#define __AXT_AXDEV_H__

#include "AXHD.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

	
    // ���̺귯�� �ʱ�ȭ(High resolution performance counter ����� ����� �� ���� �ý��ۿ��� ���)
    DWORD    __stdcall AxlOpenTimer();

    // Board Number�� �̿��Ͽ� Board Address ã��
    DWORD    __stdcall AxlGetBoardAddress(long lBoardNo, DWORD *upBoardAddress);
    // Board Number�� �̿��Ͽ� Board ID ã��
    DWORD    __stdcall AxlGetBoardID(long lBoardNo, DWORD *upBoardID);
    // Board Number�� �̿��Ͽ� Board Version ã��
    DWORD    __stdcall AxlGetBoardVersion(long lBoardNo, DWORD *upBoardVersion);
    // Board Number�� Module Position�� �̿��Ͽ� Module ID ã��
    DWORD    __stdcall AxlGetModuleID(long lBoardNo, long lModulePos, DWORD *upModuleID);
    // Board Number�� Module Position�� �̿��Ͽ� Module Version ã��
    DWORD    __stdcall AxlGetModuleVersion(long lBoardNo, long lModulePos, DWORD *upModuleVersion);
    // Board Number�� Module Position�� �̿��Ͽ� AIO Module Number ã��
    DWORD    __stdcall AxaInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
    // Board Number�� Module Position�� �̿��Ͽ� DIO Module Number ã��
    DWORD    __stdcall AxdInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
    
//==Log
    // ������ ����� EzSpy�� �α� ��� ���� ����
    DWORD    __stdcall AxdLogSetModule(long lModuleNo, DWORD uUse);
    // ������ ����� EzSpy�� �α� ��� ���� Ȯ��
    DWORD    __stdcall AxdLogGetModule(long lModuleNo, DWORD *upUse);
    
    // ���� �࿡ 16bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 16bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 32bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 32bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);

    /*
    // ���� �࿡ QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandQi(long lAxisNo, QICOMMAND sCommand);
    // ���� �࿡ 8bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 8bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 24bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 24bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);

    // ���� �࿡ Port Data �������� - IP
    DWORD    __stdcall AxmGetPortData(long lAxisNo,  WORD wOffset, DWORD *upData);
    // ���� �࿡ Port Data Setting - IP
    DWORD    __stdcall AxmSetPortData(long lAxisNo, WORD wOffset, DWORD dwData);

    // ���� �࿡ Port Data �������� - QI
    DWORD    __stdcall AxmGetPortDataQi(long lAxisNo, WORD byOffset, DWORD *wpData);
    // ���� �࿡ Port Data Setting - QI
    DWORD    __stdcall AxmSetPortDataQi(long lAxisNo, WORD byOffset, WORD wData);
        
    // ���� �࿡ ��ũ��Ʈ�� �����Ѵ�. - QI
    DWORD    __stdcall AxmSetScriptCaptionQi(long lAxisNo, long sc, DWORD event, DWORD cmd, DWORD data);
    // ���� �࿡ ��ũ��Ʈ�� ��ȯ�Ѵ�. - QI
    DWORD    __stdcall AxmGetScriptCaptionQi(long lAxisNo, long sc, DWORD *event, DWORD *cmd, DWORD *data);
    */
    
    DWORD    __stdcall AxdoDoSaveToFlash(long lModuleNo);
    DWORD    __stdcall AxdoDoLoadToFlash(long lModuleNo);

    
    //======= �︮�� �̵�  ===========================================================================
    // ������ ��ǥ�迡 ������, �������� �߽����� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, �������� �߽����� �����Ͽ� �︮�� ���Ӻ��� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�)
    // dCenterPos = �߽��� X,Y  , dEndPos = ������ X,Y .
    
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD    __stdcall AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
    
    // ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�. 
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 �߰���, �������� �����Ͽ� �︮�ÿ��� ���� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)
    // dMidPos = �߰��� X,Y  , dEndPos = ������ X,Y 
    DWORD    __stdcall AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel);
    
    // ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� �︮�ÿ��� ���� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)
    // dRadius = ������, dEndPos = ������ X,Y  , uShortDistance = ������(0), ū��(1)
    
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD    __stdcall AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);
    
    // ������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� �︮�ÿ��� ���� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)
    // dCenterPos = �߽��� X,Y  , dAngle = ����.
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD    __stdcall AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
    
    //======== ���ö��� �̵� =========================================================================== 
    
    // AxmContiBeginNode, AxmContiEndNode�� ���̻�����. 
    // ���ö��� ���� ���� �����ϴ� �Լ��̴�. ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�.
    // AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)    
    // lPosSize : �ּ� 3�� �̻�.
    // 2������ ���� dPoZ���� 0���� �־��ָ� ��.
    // 3������ ���� ������� 3���� dPosZ ���� �־��ش�.
    DWORD    __stdcall AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor);

    //====���� �Լ� ============================================================================================================================================;
    // ������ ��ǥ�迡 ���Ӻ��� �� ������ �����Ѵ�.
    // (����� ��ȣ�� 0 ���� ����))
    // ������: ������Ҷ��� �ݵ�� ���� ���ȣ�� ���� ���ں��� ū���ڸ� �ִ´�.
    //         ������ ���� �Լ��� ����Ͽ��� �� �������ȣ�� ���� ���ȣ�� ���� �� ���� lpAxesNo�� ���� ���ؽ��� �Է��Ͽ��� �Ѵ�.
    //         ������ ���� �Լ��� ����Ͽ��� �� �������ȣ�� �ش��ϴ� ���� ���ȣ�� �ٸ� ���̶�� �Ѵ�.
    //         ���� ���� �ٸ� Coordinate�� �ߺ� �������� ���ƾ� �Ѵ�.
    DWORD    __stdcall AxmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
    //������ ��ǥ�迡 ���Ӻ��� �� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);
    
    // ������ ��ǥ�迡 ���Ӻ��� �� ����/��� ��带 �����Ѵ�.
    // (������ : �ݵ�� ����� �ϰ� ��밡��)
    // ���� ���� �̵� �Ÿ� ��� ��带 �����Ѵ�.
    // uAbsRelMode : POS_ABS_MODE '0' - ���� ��ǥ��
    //              POS_REL_MODE '1' - ��� ��ǥ��
    
    DWORD    __stdcall AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
    // ������ ��ǥ�迡 ���Ӻ��� �� ����/��� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);
    
    // ������ ��ǥ�迡 ���� ������ ���� ���� Queue�� ��� �ִ��� Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiReadFree(long lCoord, DWORD *upQueueFree);
    // ������ ��ǥ�迡 ���� ������ ���� ���� Queue�� ����Ǿ� �ִ� ���� ���� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiReadIndex(long lCoord, long *lpQueueIndex);
    // ������ ��ǥ�迡 ���� ���� ������ ���� ����� ���� Queue�� ��� �����ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiWriteClear(long lCoord);
    
    // ������ ��ǥ�迡 ���Ӻ������� ������ �۾����� ����� �����Ѵ�. ���Լ��� ȣ������,
    // AxmContiEndNode�Լ��� ȣ��Ǳ� ������ ����Ǵ� ��� ����۾��� ���� ����� �����ϴ� ���� �ƴ϶� ���Ӻ��� ������� ��� �Ǵ� ���̸�,
    // AxmContiStart �Լ��� ȣ��� �� ��μ� ��ϵȸ���� ������ ����ȴ�.
    DWORD    __stdcall AxmContiBeginNode(long lCoord);
    // ������ ��ǥ�迡�� ���Ӻ����� ������ �۾����� ����� �����Ѵ�.
    DWORD    __stdcall AxmContiEndNode(long lCoord);
    
    // ���� ���� ���� �Ѵ�.
    // dwProfileset(0 : ���� ���� ���, 1 : �������� ���� ���, 2 : �ڵ� �������� ����, 3 : �ӵ����� ��� ���) 
    DWORD    __stdcall AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle); 
    // ������ ��ǥ�迡 ���� ���� ���� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiIsMotion(long lCoord, DWORD *upInMotion);
    // ������ ��ǥ�迡 ���� ���� ���� �� ���� �������� ���� ���� �ε��� ��ȣ�� Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiGetNodeNum(long lCoord, long *lpNodeNum);
    // ������ ��ǥ�迡 ������ ���� ���� ���� �� �ε��� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum);

    // ���̺귯�� �ʱ�ȭ(�� ī�带 2���� ����� Ring ���� ��Ʈ�� ����, Lancard�� 2�� �̻� �����Ǿ�� �Ѵ�.)
    DWORD    __stdcall AxlRingOpen();
    
    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/�����ǥ�� ������ ��ġ���� 2�ܰ��� �ӵ������� ������ �Ѵ�.
    // 1) ������ ���� ���� �ӵ����� ���� 2) ���� ���� �ӵ����� �ڵ� ������ 3) ���� ���� �ӵ����� ���� ������ ����.
    // �ӵ� ���������� AxmMotSetProfileMode �Լ����� �����Ѵ�. 
    // �޽��� ��µǴ� �������� �Լ��� �����.
    DWORD    __stdcall AxmMoveStartPos2(long lAxisNo, double dPos, double dAccelEndVel, double dDecelStrVel, double dAccel, double dDecel);

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/�����ǥ�� ������ ��ġ���� 2�ܰ��� �ӵ������� ������ �Ѵ�.
    // 1) ������ ���� ���� �ӵ����� ���� 2) ���� ���� �ӵ����� �ڵ� ������ 3) ���� ���� �ӵ����� ���� ������ ����.
    // �ӵ� ���������� AxmMotSetProfileMode �Լ����� �����Ѵ�. 
    // �޽� ����� ����Ǵ� �������� �Լ��� �����
    DWORD    __stdcall AxmMovePos2(long lAxisNo, double dPos, double dAccelEndVel, double dDecelStrVel, double dAccel, double dDecel);
    
    // ���� �࿡ ������ Ư�� �̺�Ʈ �߻��� �̸� ������ ������ �����ϵ��� �Ѵ�.
    // ���� �࿡ ���� �̺�Ʈ�� ������ ��ɾ�, ����� ����Ÿ���� �����Ѵ�.
    DWORD    __stdcall AxmEventSetOperation(long lAxisNo, DWORD uEvent, DWORD uCmd, DWORD uData);
    
    // ���� �࿡ ���� �̺�Ʈ�� ������ ��ɾ�, ����� ����Ÿ���� Ȯ���Ѵ�.
    DWORD    __stdcall AxmEventGetOperation(long lAxisNo,  DWORD *upEvent, DWORD *upCmd, DWORD *upData);

    // ���� �࿡ �̺�Ʈ ���� ��ɾ� �������� ĸ�ĵ� ���� Ȯ�� �Ѵ�.
    DWORD    __stdcall AxmEventGetCapture(long lAxisNo, DWORD *upCapData);

    // ���� �࿡ �̺�Ʈ ���� ��ɾ� ���� ��� ��� ���θ� ���� �Ѵ�.
    // uUse       : DISABLE(0), ENABLE(1)
    DWORD    __stdcall AxmEventSetUse(long lAxisNo, DWORD uUse);

    // ���� �࿡ �̺�Ʈ ���� ��ɾ� ���� ��� ��� ���θ� Ȯ�� �Ѵ�.
    DWORD    __stdcall AxmEventGetUse(long lAxisNo, DWORD *upUse);
    

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXDEV_H__
