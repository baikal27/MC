/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** AXL.H
**
** COPYRIGHT (c) AJINEXTEK Co., LTD
**
*****************************************************************************
*****************************************************************************
**
** Description
** -----------
** Ajinextek Library Header File
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

#ifndef __AXT_AXL_H__
#define __AXT_AXL_H__

#include "AXHS.h"

//#include "AXA.h"
//#include "AXD.h"
//#include "AXM.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

//========== �Է� ���� ���� ���� ����. =======================================================================
// lNodeNum   : CPU ����� ID ���� ���͸� ����ġ�� ���� �ǹ� �մϴ�.(0x00 �̻�,  0xF7 ����)
//==============================================================================================================

//========== ���̺귯�� �ʱ�ȭ =================================================================================
    // ���̺귯�� �ʱ�ȭ
    DWORD    __stdcall AxlOpen(void);
    
	// ��忡 ������ ��� �ʱ�ȭ
    // ����ڰ� ���ϴ� �Ķ��Ÿ�� Flash�� ����Ǹ� �����ʱ�ȭ���� �װ����� �ʱ�ȭ �ǰ� ������� Axm�� �Ķ��Ÿ Default������ �ʱ�ȭ�ȴ�.
    DWORD    __stdcall AxlInit(long lNodeNum);

	// ���̺귯�� ����� ����
    DWORD    __stdcall AxlClose(void);
 
    // ���̺귯���� �ʱ�ȭ �Ǿ� �ִ� �� Ȯ��
    DWORD    __stdcall AxlIsOpened(void);

//========== ���̺귯�� �� ���̽� ���� ���� =================================================================================

    // ��ϵ� ��� ���� Ȯ��
    DWORD    __stdcall AxlGetNodeCount(long *lpNodeCount);
    // ��� ����(��� ID, ��� ����) Ȯ��
	// lNodeNum : ����ȣ.
	// dwpModuleId : ��� ID �ݵ�� 17���迭�� �����ؾߵ�.
    DWORD   __stdcall AxlGetNodeInfo(long lNodeNum, DWORD *dwpModuleId);
    
	// ��ϵ� ��� ������ �����Ѵ�.
    // upNodeCount : ��尹��
	// upArrayNodeID : �迭 ��� ID �ݵ�� 250���迭�� �����ؾߵ�.
    DWORD   __stdcall AxlGetNetInfo(DWORD *upNodeCount, DWORD *upArrayNodeID);

    // ���̺귯�� ���� Ȯ��
    DWORD    __stdcall AxlGetLibVersion(char *szVersion);

//========== �α� ���� =================================================================================================

    // EzSpy�� ����� �޽��� ���� ����
    // uLevel : 0 - 3 ����
    // LEVEL_NONE(0)     : ��� �޽����� ������� �ʴ´�.
    // LEVEL_ERROR(1)    : ������ �߻��� �޽����� ����Ѵ�.
    // LEVEL_RUNSTOP(2)  : ��ǿ��� Run / Stop ���� �޽����� ����Ѵ�.
    // LEVEL_FUNCTION(3) : ��� �޽����� ����Ѵ�.    
    DWORD    __stdcall AxlSetLogLevel(DWORD uLevel);
    // EzSpy�� ����� �޽��� ���� Ȯ��
    DWORD    __stdcall AxlGetLogLevel(DWORD *upLevel);

//========= ��Ʈ��ũ ���� ���� ����� �Լ� =============================================================================
    
	// �ش� ����� ���� ��Ʈ��ũ ���¸� Ȯ�� �Ѵ�.
	// dwpFlag :  '0' - connected ����, 
	//            '1' - disconnected ����
	DWORD    __stdcall AxlNetStatusRead(long lNodeNum, DWORD *dwpFlag);
    // �ش� ����� Frimware ������ Ȯ���Ѵ�.
    DWORD    __stdcall AxlGetNodeFirmVersion(long lNodeNum, char *szVersion);
    // �ش� ��忡 �ִ� ������ �Ķ���͸� �����Ѵ�.
    DWORD    __stdcall AxlSaveParamToFlash(long lNodeNum);
    // �ش� ��忡 �ִ� ������ �������� �ε��Ѵ�.
    DWORD    __stdcall AxlLoadParamFromFlash(long lNodeNum);
    // �ش� ��忡 �ִ� ������ �������� ���� ������ �ʱ�ȭ �Ѵ�.
	// Axm�� �Ķ��Ÿ Default������ �ʱ�ȭ�ȴ�.
    DWORD    __stdcall AxlResetParamFlash(long lNodeNum);
    
	DWORD    __stdcall AxlChangePassword(long lNodeNum, DWORD dwCurrentPassword, DWORD dwNewPassword);
	DWORD    __stdcall AxlChangeVendorId(long lNodeNum, DWORD dwPassword, DWORD dwVendorId);
	DWORD    __stdcall AxlGetSecurityKey(long lNodeNum, DWORD dwVendorId,  DWORD* dwpSecurityKey0, DWORD* dwpSecurityKey1);
	DWORD    __stdcall AxlSetSecurityKey(long lNodeNum, DWORD dwVendorId,  DWORD dwSecurityKey0, DWORD dwSecurityKey1);
	
	// Background process check function
	// *dwpProcessState --> 255(Background process not execute)
	// *dwpProcessState --> 0(Background process idle)
	// *dwpProcessState --> 2(Background process run)
	DWORD	 __stdcall AxlProcessStateCheck(DWORD* dwpProcessState);    

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXL_H__
