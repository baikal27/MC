/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** AXD.H
**
** COPYRIGHT (c) AJINEXTEK Co., LTD
**
*****************************************************************************
*****************************************************************************
**
** Description
** -----------
** Ajinextek Digital Library Header File
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

#ifndef __AXT_AXD_H__
#define __AXT_AXD_H__

#include "AXHS.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

//========== 노드 및 모듈 정보
    // DIO 모듈이 있는지 확인
    DWORD    __stdcall AxdInfoIsDIOModule(DWORD *upStatus);
    // DIO 입출력 모듈의 개수 확인
    DWORD    __stdcall AxdInfoGetModuleCount(long *lpModuleCount);
    // 지정한 모듈의 입력 접점 개수 확인
    DWORD    __stdcall AxdInfoGetInputCount(long lModuleNo, long *lpCount);
    // 지정한 모듈의 출력 접점 개수 확인
    DWORD    __stdcall AxdInfoGetOutputCount(long lModuleNo, long *lpCount);
    // 지정한 모듈 번호로 노드 ID 번호, 모듈 위치, 모듈 ID 확인
    DWORD    __stdcall AxdInfoGetModule(long lModuleNo, long *lpNodeNum, long *lpModulePos, DWORD *upModuleID);

//========== 입출력 레벨 설정 확인
	//==입력 레벨 설정 인
    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelSetInportBit(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelSetInportByte(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelSetInportWord(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelSetInportDword(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelGetInportBit(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelGetInportByte(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelGetInportWord(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelGetInportDword(long lModuleNo, long lOffset, DWORD *upLevel);

    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelSetInport(long lOffset, DWORD uLevel);

    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiLevelGetInport(long lOffset, DWORD *upLevel);

//==출력 레벨 설정 확인
    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelSetOutportBit(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelSetOutportByte(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelSetOutportWord(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelSetOutportDword(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelGetOutportBit(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelGetOutportByte(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelGetOutportWord(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelGetOutportDword(long lModuleNo, long lOffset, DWORD *upLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelSetOutport(long lOffset, DWORD uLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoLevelGetOutport(long lOffset, DWORD *upLevel);

//========== 입출력 포트 쓰기 읽기
//==출력 포트 쓰기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoWriteOutport(long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoWriteOutportBit(long lModuleNo, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoWriteOutportByte(long lModuleNo, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoWriteOutportWord(long lModuleNo, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoWriteOutportDword(long lModuleNo, long lOffset, DWORD uValue);

//==출력 포트 읽기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoReadOutport(long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoReadOutportBit(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoReadOutportByte(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoReadOutportWord(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoReadOutportDword(long lModuleNo, long lOffset, DWORD *upValue);

//==입력 포트 읽기
    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiReadInport(long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiReadInportBit(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiReadInportByte(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiReadInportWord(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiReadInportDword(long lModuleNo, long lOffset, DWORD *upValue);

//========== 입출력 레벨 설정 확인 (지정 노드에 대한 제어)
//==입력 레벨 설정 인
    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
	// lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelSetInportBit(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0xFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelSetInportByte(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelSetInportWord(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelSetInportDword(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelGetInportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelGetInportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelGetInportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelSetInport(long lNodeNum, long lOffset, DWORD uLevel);
    
	// 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *uLevel        : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiNLevelGetInport(long lNodeNum, long lOffset, DWORD *upLevel);

//==입력 포트 읽기
    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	//===============================================================================================//
	// lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiNReadInport(long lNodeNum, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	//===============================================================================================//
	// lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdiNReadInportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
	//===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNReadInportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
	// lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdiNReadInportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

//==출력 레벨 설정 확인
    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelSetOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelSetOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelSetOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
	// uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelSetOutportDword(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
	// *upLevel       : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelGetOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //==============================================================================================//
    DWORD    __stdcall AxdoNLevelGetOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelGetOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelSetOutport(long lNodeNum, long lOffset, DWORD uLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel        : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNLevelGetOutport(long lNodeNum, long lOffset, DWORD *upLevel);

//========== 입출력 포트 쓰기 읽기 
//==출력 포트 쓰기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
	// uValue         : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNWriteOutport(long lNodeNum, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
	// uValue         : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNWriteOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNWriteOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNWriteOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNWriteOutportDword(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

//==출력 포트 읽기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
	// *upValue       : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNReadOutport(long lNodeNum, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
	// *upValue       : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AxdoNReadOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNReadOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNReadOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AxdoNReadOutportDword(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

//==출력 포트의 네트웍 예외 처리    
    // 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 방법 설정.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // dwNetErrorAct  : 접점의 상태(0 - 1)
	//                  '0' - 현재 접점 상태 유지
    //                  '1' - AxdoSetNetWorkErrorByteValue 함수에 의하여 Setting된 값으로 동작
    //===============================================================================================//
    DWORD    __stdcall AxdoNetWorkErrorSetAction(long lModuleNo, DWORD dwNetErrorAct);

    // 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 방법 확인.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // *dwNetErrorAct : 접점의 상태(0 - 1)
    //                  '0' - 현재 접점 상태 유지
    //                  '1' - AxdoSetNetWorkErrorByteValue 함수에 의하여 Setting된 값으로 동작하도록 설정
    //===============================================================================================//
    DWORD    __stdcall AxdoNetWorkErrorGetAction(long lModuleNo, DWORD* dwpNetErrorAct);
    
    // 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 상태.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0xFF('1'네트웍 에러시 출력 High, '0'네트웍 에러시 출력 Low) 
    //===============================================================================================//
	DWORD    __stdcall AxdoNetWorkErrorSetByteValue(long lModuleNo, long lOffset, DWORD uValue);

	// 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 상태 확인.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0xFF('1'네트웍 에러시 출력 High, '0'네트웍 에러시 출력 Low) 
    //===============================================================================================//
	DWORD    __stdcall AxdoNetWorkErrorGetByteValue(long lModuleNo, long lOffset, DWORD* upValue);
    
#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXD_H__
