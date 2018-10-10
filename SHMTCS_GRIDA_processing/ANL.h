/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** ANL.H
**
** COPYRIGHT (c) AJINEXTEK Co., LTD
**
*****************************************************************************
*****************************************************************************
**
** Description
** -----------
** Ajinextek Library Header File
** For Network Ethernet/IP product only
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

#ifndef __ANT_AXL_H__
#define __ANT_AXL_H__

//#include "ANHS.h"
//#include "ANA.h"
//#include "AND.h"
//#include "ANM.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

//========== �Է� ���� ���� ���� ����. =======================================================================
// lNodeNum   : CPU ����� ID ���� ���͸� ����ġ�� ���� �ǹ� �մϴ�.(0x00 �̻�,  0xF7 ����)
//==============================================================================================================

//========== ���̺귯�� �ʱ�ȭ =================================================================================
    // ���̺귯�� �ʱ�ȭ
    DWORD    __stdcall AnlOpen();
    
	// ��忡 ������ ��� �ʱ�ȭ
    // ����ڰ� ���ϴ� �Ķ��Ÿ�� Flash�� ����Ǹ� �����ʱ�ȭ���� �װ����� �ʱ�ȭ �ǰ� ������� Axm�� �Ķ��Ÿ Default������ �ʱ�ȭ�ȴ�.
    DWORD    __stdcall AnlInit(long lNodeNum);
    // ���̺귯�� ����� ����
    BOOL    __stdcall AnlClose();
    // ���̺귯���� �ʱ�ȭ �Ǿ� �ִ� �� Ȯ��
    BOOL    __stdcall AnlIsOpened();

//========== ���̺귯�� �� ���̽� ���� ���� =================================================================================

    // ��ϵ� ��� ���� Ȯ��
    DWORD    __stdcall AnlGetNodeCount(long *lpNodeCount);
    // ��� ����(��� ID, ��� ����) Ȯ��
	// lNodeNum : ����ȣ.
	// dwpModuleId : ��� ID �ݵ�� 17���迭�� �����ؾߵ�.
    DWORD   __stdcall AnlGetNodeInfo(long lNodeNum, DWORD *dwpModuleId);
    
	// ��ϵ� ��� ������ �����Ѵ�.
    // upNodeCount : ��尹��
	// upArrayNodeID : �迭 ��� ID �ݵ�� 250���迭�� �����ؾߵ�.
    DWORD   __stdcall AnlGetNetInfo(DWORD *upNodeCount, DWORD *upArrayNodeID);

    // ���̺귯�� ���� Ȯ��
    DWORD    __stdcall AnlGetLibVersion(char *szVersion);

//========== �α� ���� =================================================================================================

    // EzSpy�� ����� �޽��� ���� ����
    // uLevel : 0 - 3 ����
    // LEVEL_NONE(0)     : ��� �޽����� ������� �ʴ´�.
    // LEVEL_ERROR(1)    : ������ �߻��� �޽����� ����Ѵ�.
    // LEVEL_RUNSTOP(2)  : ��ǿ��� Run / Stop ���� �޽����� ����Ѵ�.
    // LEVEL_FUNCTION(3) : ��� �޽����� ����Ѵ�.    
    DWORD    __stdcall AnlSetLogLevel(DWORD uLevel);
    // EzSpy�� ����� �޽��� ���� Ȯ��
    DWORD    __stdcall AnlGetLogLevel(DWORD *upLevel);

//========= ��Ʈ��ũ ���� ���� ����� �Լ� =============================================================================
    
	// �ش� ����� ���� ��Ʈ��ũ ���¸� Ȯ�� �Ѵ�.
	// dwpFlag :  '0' - connected ����, 
	//            '1' - disconnected ����
	DWORD    __stdcall AnlNetStatusRead(long lNodeNum, DWORD *dwpFlag);
    // �ش� ����� Frimware ������ Ȯ���Ѵ�.
    DWORD    __stdcall AnlGetNodeFirmVersion(long lNodeNum, char *szVersion);
    // �ش� ��忡 �ִ� ������ �Ķ���͸� �����Ѵ�.
    DWORD    __stdcall AnlSaveParamToFlash(long lNodeNum);
    // �ش� ��忡 �ִ� ������ �������� �ε��Ѵ�.
    DWORD    __stdcall AnlLoadParamFromFlash(long lNodeNum);
    // �ش� ��忡 �ִ� ������ �������� ���� ������ �ʱ�ȭ �Ѵ�.
	// Axm�� �Ķ��Ÿ Default������ �ʱ�ȭ�ȴ�.
    DWORD    __stdcall AnlResetParamFlash(long lNodeNum);
	
	DWORD    __stdcall AnlChangePassword(long lNodeNum, DWORD dwCurrentPassword, DWORD dwNewPassword);
	DWORD    __stdcall AnlChangeVendorId(long lNodeNum, DWORD dwPassword, DWORD dwVendorId);
	DWORD    __stdcall AnlGetSecurityKey(long lNodeNum, DWORD dwVendorId,  DWORD* dwpSecurityKey0, DWORD* dwpSecurityKey1);
	DWORD    __stdcall AnlSetSecurityKey(long lNodeNum, DWORD dwVendorId,  DWORD dwSecurityKey0, DWORD dwSecurityKey1);
	
	// Background process check function
	// *dwpProcessState --> 255(Background process not execute)
	// *dwpProcessState --> 0(Background process idle)
	// *dwpProcessState --> 2(Background process run)
	DWORD	 __stdcall AnlProcessStateCheck(DWORD* dwpProcessState);

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__ANT_AXL_H__
