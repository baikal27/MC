/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** AND.H
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

#ifndef __ANT_AND_H__
#define __ANT_AND_H__

//#include "ANHS.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

//========== 보드 및 모듈 정보
    // DIO 모듈이 있는지 확인
    DWORD    __stdcall AndInfoIsDIOModule(DWORD *upStatus);
    // DIO 입출력 모듈의 개수 확인
    DWORD    __stdcall AndInfoGetModuleCount(long *lpModuleCount);
    // 지정한 모듈의 입력 접점 개수 확인
    DWORD    __stdcall AndInfoGetInputCount(long lModuleNo, long *lpCount);
    // 지정한 모듈의 출력 접점 개수 확인
    DWORD    __stdcall AndInfoGetOutputCount(long lModuleNo, long *lpCount);
    // 지정한 모듈 번호로 노드 ID 번호, 모듈 위치, 모듈 ID 확인
    DWORD    __stdcall AndInfoGetModule(long lModuleNo, long *lpNodeNum, long *lpModulePos, DWORD *upModuleID);

//========== 입출력 레벨 설정 확인
	//==입력 레벨 설정 인
    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelSetInportBit(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelSetInportByte(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelSetInportWord(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelSetInportDword(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelGetInportBit(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelGetInportByte(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelGetInportWord(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelGetInportDword(long lModuleNo, long lOffset, DWORD *upLevel);

    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelSetInport(long lOffset, DWORD uLevel);

    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiLevelGetInport(long lOffset, DWORD *upLevel);

//==출력 레벨 설정 확인
    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelSetOutportBit(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelSetOutportByte(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelSetOutportWord(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelSetOutportDword(long lModuleNo, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelGetOutportBit(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelGetOutportByte(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelGetOutportWord(long lModuleNo, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelGetOutportDword(long lModuleNo, long lOffset, DWORD *upLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelSetOutport(long lOffset, DWORD uLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoLevelGetOutport(long lOffset, DWORD *upLevel);

//========== 입출력 포트 쓰기 읽기
//==출력 포트 쓰기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoWriteOutport(long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoWriteOutportBit(long lModuleNo, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoWriteOutportByte(long lModuleNo, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoWriteOutportWord(long lModuleNo, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 출력
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoWriteOutportDword(long lModuleNo, long lOffset, DWORD uValue);

//==출력 포트 읽기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoReadOutport(long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoReadOutportBit(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoReadOutportByte(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoReadOutportWord(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoReadOutportDword(long lModuleNo, long lOffset, DWORD *upValue);

//==입력 포트 읽기
    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiReadInport(long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiReadInportBit(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiReadInportByte(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiReadInportWord(long lModuleNo, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiReadInportDword(long lModuleNo, long lOffset, DWORD *upValue);

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
    DWORD    __stdcall AndiNLevelSetInportBit(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0xFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelSetInportByte(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelSetInportWord(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelSetInportDword(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelGetInportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelGetInportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelGetInportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelSetInport(long lNodeNum, long lOffset, DWORD uLevel);
    
	// 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *uLevel        : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiNLevelGetInport(long lNodeNum, long lOffset, DWORD *upLevel);

//==입력 포트 읽기
    // 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	//===============================================================================================//
	// lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiNReadInport(long lNodeNum, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	//===============================================================================================//
	// lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndiNReadInportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
	//===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNReadInportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
	// lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndiNReadInportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

//==출력 레벨 설정 확인
    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelSetOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelSetOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelSetOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
	// uLevel         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelSetOutportDword(long lNodeNum, long lModulePos, long lOffset, DWORD uLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
	// *upLevel       : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelGetOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //==============================================================================================//
    DWORD    __stdcall AndoNLevelGetOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelGetOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // uLevel         : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelSetOutport(long lNodeNum, long lOffset, DWORD uLevel);

    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upLevel        : LOW(0)
    //                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNLevelGetOutport(long lNodeNum, long lOffset, DWORD *upLevel);

//========== 입출력 포트 쓰기 읽기 
//==출력 포트 쓰기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
	// uValue         : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNWriteOutport(long lNodeNum, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
	// uValue         : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNWriteOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNWriteOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNWriteOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 출력
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0x0FFFFFFFF('1'로 설정 된 비트는 HIGH, '0'으로 설정 된 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNWriteOutportDword(long lNodeNum, long lModulePos, long lOffset, DWORD uValue);

//==출력 포트 읽기
    // 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lOffset        : 입력 접점에 대한 Offset 위치
	// *upValue       : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNReadOutport(long lNodeNum, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
	// *upValue       : LOW(0)
	//                : HIGH(1)
    //===============================================================================================//
    DWORD    __stdcall AndoNReadOutportBit(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNReadOutportByte(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNReadOutportWord(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

    // 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
    //===============================================================================================//
    // lNodeNum       : 노드 번호
    // lModulePos     : 모듈 위치(사용자가 로터리 스위치로 정한 절대 위치)
    // lOffset        : 입력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0x0FFFFFFFF('1'로 읽힌 비트는 HIGH, '0'으로 읽힌 비트는 LOW)
    //===============================================================================================//
    DWORD    __stdcall AndoNReadOutportDword(long lNodeNum, long lModulePos, long lOffset, DWORD *upValue);

//==출력 포트의 네트웍 예외 처리    
    // 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 방법 설정.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // dwNetErrorAct  : 접점의 상태(0 - 1)
	//                  '0' - 현재 접점 상태 유지
    //                  '1' - AndoSetNetWorkErrorByteValue 함수에 의하여 Setting된 값으로 동작
    //===============================================================================================//
    DWORD    __stdcall AndoNetWorkErrorSetAction(long lModuleNo, DWORD dwNetErrorAct);

    // 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 방법 확인.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // *dwNetErrorAct : 접점의 상태(0 - 1)
    //                  '0' - 현재 접점 상태 유지
    //                  '1' - AndoSetNetWorkErrorByteValue 함수에 의하여 Setting된 값으로 동작하도록 설정
    //===============================================================================================//
    DWORD    __stdcall AndoNetWorkErrorGetAction(long lModuleNo, DWORD* dwpNetErrorAct);
	
	// 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 상태.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // uValue         : 0x00 ~ 0xFF('1'네트웍 에러시 출력 High, '0'네트웍 에러시 출력 Low) 
    //===============================================================================================//
	DWORD    __stdcall AndoNetWorkErrorSetByteValue(long lModuleNo, long lOffset, DWORD uValue);

	// 지정한 출력 접점 모듈이 연결된 노드의 통신이 끊어 졌을때 현재 출력 접점 상태에 대한 예외처리 상태 확인.
    //===============================================================================================//
    // lModuleNo      : 모듈 번호
    // lOffset        : 출력 접점에 대한 Offset 위치
    // *upValue       : 0x00 ~ 0xFF('1'네트웍 에러시 출력 High, '0'네트웍 에러시 출력 Low) 
    //===============================================================================================//
	DWORD    __stdcall AndoNetWorkErrorGetByteValue(long lModuleNo, long lOffset, DWORD* upValue);
    
#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__ANT_AND_H__
