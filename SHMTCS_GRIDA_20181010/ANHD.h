#ifndef __ANT_ANHD_H__
#define __ANT_ANHD_H__

/* IP COMMAND LIST							*/
typedef enum _N_IPCOMMAND
{
     // PGM-1 Group Register
	N_IPxyRANGERead							= 0x00,				// PGM-1 RANGE READ, 16bit, 0xFFFF
	N_IPxyRANGEWrite						= 0x80,				// PGM-1 RANGE WRITE
	N_IPxySTDRead							= 0x01,				// PGM-1 START/STOP SPEED DATA READ, 16bit, 
	N_IPxySTDWrite							= 0x81,				// PGM-1 START/STOP SPEED DATA WRITE
	N_IPxyOBJRead							= 0x02,				// PGM-1 OBJECT SPEED DATA READ, 16bit, 
	N_IPxyOBJWrite							= 0x82,				// PGM-1 OBJECT SPEED DATA WRITE
	N_IPxyRATE1Read							= 0x03,				// PGM-1 RATE-1 DATA READ, 16bit, 0xFFFF
	N_IPxyRATE1Write						= 0x83,				// PGM-1 RATE-1 DATA WRITE
	N_IPxyRATE2Read							= 0x04,				// PGM-1 RATE-2 DATA READ, 16bit, 0xFFFF
	N_IPxyRATE2Write						= 0x84,				// PGM-1 RATE-2 DATA WRITE
	N_IPxyRATE3Read							= 0x05,				// PGM-1 RATE-3 DATA READ, 16bit, 0xFFFF
	N_IPxyRATE3Write						= 0x85,				// PGM-1 RATE-3 DATA WRITE
	N_IPxyRCP12Read							= 0x06,				// PGM-1 RATE CHANGE POINT 1-2 READ, 16bit, 0xFFFF
	N_IPxyRCP12Write						= 0x86,				// PGM-1 RATE CHANGE POINT 1-2 WRITE
	N_IPxyRCP23Read							= 0x07,				// PGM-1 RATE CHANGE POINT 2-3 READ, 16bit, 0xFFFF
	N_IPxyRCP23Write						= 0x87,				// PGM-1 RATE CHANGE POINT 2-3 WRITE
	N_IPxySW1Read							= 0x08,				// PGM-1 SW-1 DATA READ, 15bit, 0x7FFF
	N_IPxySW1Write							= 0x88,				// PGM-1 SW-1 DATA WRITE
	N_IPxySW2Read							= 0x09,				// PGM-1 SW-2 DATA READ, 15bit, 0x7FFF
	N_IPxySW2Write							= 0x89,				// PGM-1 SW-2 DATA WRITE
	N_IPxyPWMRead							= 0x0A,				// PGM-1 PWM 출력 설정 DATA READ(0~6), 3bit, 0x00
	N_IPxyPWMWrite							= 0x8A,				// PGM-1 PWM 출력 설정 DATA WRITE
	N_IPxyREARRead							= 0x0B,				// PGM-1 SLOW DOWN/REAR PULSE READ, 32bit, 0x00000000
	N_IPxyREARWrite							= 0x8B,				// PGM-1 SLOW DOWN/REAR PULSE WRITE
	N_IPxySPDRead							= 0x0C,				// PGM-1 현재 SPEED DATA READ, 16bit, 0x0000
	N_IPxyNoOperation_8C					= 0x8C,				// No operation
	N_IPxySPDCMPRead						= 0x0D,				// PGM-1 현재 SPEED 비교 DATA READ, 16bit, 0x0000
	N_IPxySPDCMPWrite						= 0x8D,				// PGM-1 현재 SPEED 비교 DATA WRITE
	N_IPxyDRVPULSERead						= 0x0E,				// PGM-1 DRIVE PULSE COUNTER READ, 32bit, 0x00000000
	N_IPxyNoOperation_8E					= 0x8E,				// No operation
	N_IPxyPRESETPULSERead					= 0x0F,				// PGM-1 PRESET PULSE DATA READ, 32bit, 0x00000000
	N_IPxyNoOperation_8F					= 0x8F,				// No operation

	// PGM-1 Update Group Register
	N_IPxyURANGERead						= 0x10,				// PGM-1 UP-DATE RANGE READ, 16bit, 0xFFFF
	N_IPxyURANGEWrite						= 0x90,				// PGM-1 UP-DATE RANGE WRITE
	N_IPxyUSTDRead							= 0x11,				// PGM-1 UP-DATE START/STOP SPEED DATA READ, 16bit, 
	N_IPxyUSTDWrite							= 0x91,				// PGM-1 UP-DATE START/STOP SPEED DATA WRITE
	N_IPxyUOBJRead							= 0x12,				// PGM-1 UP-DATE OBJECT SPEED DATA READ, 16bit, 
	N_IPxyUOBJWrite							= 0x92,				// PGM-1 UP-DATE OBJECT SPEED DATA WRITE
	N_IPxyURATE1Read						= 0x13,				// PGM-1 UP-DATE RATE-1 DATA READ, 16bit, 0xFFFF
	N_IPxyURATE1Write						= 0x93,				// PGM-1 UP-DATE RATE-1 DATA WRITE
	N_IPxyURATE2Read						= 0x14,				// PGM-1 UP-DATE RATE-2 DATA READ, 16bit, 0xFFFF
	N_IPxyURATE2Write						= 0x94,				// PGM-1 UP-DATE RATE-2 DATA WRITE
	N_IPxyURATE3Read						= 0x15,				// PGM-1 UP-DATE RATE-3 DATA READ, 16bit, 0xFFFF
	N_IPxyURATE3Write						= 0x95,				// PGM-1 UP-DATE RATE-3 DATA WRITE
	N_IPxyURCP12Read						= 0x16,				// PGM-1 UP-DATE RATE CHANGE POINT 1-2 READ, 16bit, 0xFFFF
	N_IPxyURCP12Write						= 0x96,				// PGM-1 UP-DATE RATE CHANGE POINT 1-2 WRITE
	N_IPxyURCP23Read						= 0x17,				// PGM-1 UP-DATE RATE CHANGE POINT 2-3 READ, 16bit, 0xFFFF
	N_IPxyURCP23Write						= 0x97,				// PGM-1 UP-DATE RATE CHANGE POINT 2-3 WRITE
	N_IPxyUSW1Read							= 0x18,				// PGM-1 UP-DATE SW-1 DATA READ, 15bit, 0x7FFF
	N_IPxyUSW1Write							= 0x98,				// PGM-1 UP-DATE SW-1 DATA WRITE
	N_IPxyUSW2Read							= 0x19,				// PGM-1 UP-DATE SW-2 DATA READ, 15bit, 0x7FFF
	N_IPxyUSW2Write							= 0x99,				// PGM-1 UP-DATE SW-2 DATA WRITE
	N_IPxyNoOperation_1A					= 0x1A,				// No operation
	N_IPxyNoOperation_9A					= 0x9A,				// No operation
	N_IPxyUREARRead							= 0x1B,				// PGM-1 UP-DATE SLOW DOWN/REAR PULSE READ, 32bit, 0x00000000
	N_IPxyUREARWrite						= 0x9B,				// PGM-1 UP-DATE SLOW DOWN/REAR PULSE WRITE
	N_IPxySPDRead_1C						= 0x1C,				// PGM-1 UP-DATA CURRENT SPEED READ(Same with 0x0C)
	N_IPxyNoOperation_9C					= 0x9C,				// No operation
	N_IPxySPDCMPRead_1D						= 0x1D,				// PGM-1 현재 SPEED 비교 DATA READ(Same with 0x0D) 
	N_IPxySPDCMPWrite_9D					= 0x9D,				// PGM-1 현재 SPEED 비교 DATA WRITE(Same with 0x8D) 
	N_IPxyACCPULSERead						= 0x1E,				// PGM-1 가속 PULSE COUNTER READ, 32bit, 0x00000000
	N_IPxyNoOperation_9E					= 0x9E,				// No operation
	N_IPxyPRESETPULSERead_1F				= 0x1F,				// PGM-1 PRESET PULSE DATA READ(Same with 0x0F)
	N_IPxyNoOperation_9F					= 0x9F,				// No operation

	// PGM-2 Group Register
	N_IPxyNoOperation_20					= 0x20,				// No operation
	N_IPxyPPRESETDRV						= 0xA0,				// +PRESET PULSE DRIVE, 32
	N_IPxyNoOperation_21					= 0x21,				// No operation
	N_IPxyPCONTDRV							= 0xA1,				// +CONTINUOUS DRIVE
	N_IPxyNoOperation_22					= 0x22,				// No operation
	N_IPxyPSCH1DRV							= 0xA2,				// +SIGNAL SEARCH-1 DRIVE
	N_IPxyNoOperation_23					= 0x23,				// No operation
	N_IPxyPSCH2DRV							= 0xA3,				// +SIGNAL SEARCH-2 DRIVE
	N_IPxyNoOperation_24					= 0x24,				// No operation
	N_IPxyPORGDRV							= 0xA4,				// +ORIGIN(원점) SEARCH DRIVE
	N_IPxyNoOperation_25					= 0x25,				// No operation
	N_IPxyMPRESETDRV						= 0xA5,				// -PRESET PULSE DRIVE, 32
	N_IPxyNoOperation_26					= 0x26,				// No operation
	N_IPxyMCONTDRV							= 0xA6,				// -CONTINUOUS DRIVE
	N_IPxyNoOperation_27					= 0x27,				// No operation
	N_IPxyMSCH1DRV							= 0xA7,				// -SIGNAL SEARCH-1 DRIVE
	N_IPxyNoOperation_28					= 0x28,				// No operation
	N_IPxyMSCH2DRV							= 0xA8,				// -SIGNAL SEARCH-2 DRIVE
	N_IPxyNoOperation_29					= 0x29,				// No operation
	N_IPxyMORGDRV							= 0xA9,				// -ORIGIN(원점) SEARCH DRIVE
	N_IPxyPULSEOVERRead						= 0x2A,				// Preset/MPG drive override pulse data read
	N_IPxyPULSEOVERWrite					= 0xAA,				// PRESET PULSE DATA OVERRIDE(ON_BUSY)
	N_IPxyNoOperation_2B					= 0x2B,				// No operation
	N_IPxySSTOPCMD							= 0xAB,				// SLOW DOWN STOP
	N_IPxyNoOperation_2C					= 0x2C,				// No operation
	N_IPxyESTOPCMD							= 0xAC,				// EMERGENCY STOP
	N_IPxyDRIVEMODERead						= 0x2D,				// 드라이브 동작 설정 DATA READ
	N_IPxyDRIVEMODEWrite					= 0xAD,				// 드라이브 동작 설정 DATA WRITE
	N_IPxyMPGCONRead						= 0x2E,				// MPG OPERATION SETTING DATA READ, 3bit, 0x00	
	N_IPxyMPGCONWrite						= 0xAE,				// MPG OPERATION SETTING DATA WRITE				
	N_IPxyPULSEMPGRead						= 0x2F,				// MPG PRESET PULSE DATA READ, 32bit, 0x00000000
	N_IPxyPULSEMPGWrite						= 0xAF,				// MPG PRESET PULSE DATA WRITE					
	
	/* Extension Group Register */
	N_IPxyNoOperation_30					= 0x30,				// No operation
	N_IPxyPSPO1DRV							= 0xB0,				// +SENSOR POSITIONING DRIVE I
	N_IPxyNoOperation_31					= 0x31,				// No operation
	N_IPxyMSPO1DRV							= 0xB1,				// -SENSOR POSITIONING DRIVE I
	N_IPxyNoOperation_32					= 0x32,				// No operation
	N_IPxyPSPO2DRV							= 0xB2,				// +SENSOR POSITIONING DRIVE II
	N_IPxyNoOperation_33					= 0x33,				// No operation
	N_IPxyMSPO2DRV							= 0xB3,				// -SENSOR POSITIONING DRIVE II
	N_IPxyNoOperation_34					= 0x34,				// No operation
	N_IPxyPSPO3DRV							= 0xB4,				// +SENSOR POSITIONING DRIVE III
	N_IPxyNoOperation_35					= 0x35,				// No operation
	N_IPxyMSPO3DRV							= 0xB5,				// -SENSOR POSITIONING DRIVE III
	N_IPxySWLMTCONRead						= 0x36,				// SOFT LIMIT 설정 READ, 3bit, 0x00
	N_IPxySWLMTCONWrite						= 0xB6,				// SOFT LIMIT 설정 WRITE
	N_IPxyMSWLMTCOMPRead					= 0x37,				// -SOFT LIMIT 비교 레지스터 설정 READ, 32bit, 0x80000000
	N_IPxyMSWLMTCOMPWrite					= 0xB7,				// -SOFT LIMIT 비교 레지스터 설정 WRITE
	N_IPxyPSWLMTCOMPRead					= 0x38,				// +SOFT LIMIT 비교 레지스터 설정 READ, 32bit, 0x7FFFFFFF
	N_IPxyPSWLMTCOMPWrite					= 0xB8,				// +SOFT LIMIT 비교 레지스터 설정 WRITE
	N_IPxyTRGCONRead						= 0x39,				// TRIGGER MODE 설정 READ, 32bit, 0x00010000
	N_IPxyTRGCONWrite						= 0xB9,				// TRIGGER MODE 설정 WRITE
	N_IPxyTRGCOMPRead						= 0x3A,				// TRIGGER 비교 데이터 설정 READ, 32bit, 0x00000000
	N_IPxyTRGCOMPWrite						= 0xBA,				// TRIGGER 비교 데이터 설정 WRITE
	N_IPxyICMRead							= 0x3B,				// INTERNAL M-DATA 설정 READ, 32bit, 0x80000000
	N_IPxyICMWrite							= 0xBB,				// INTERNAL M-DATA 설정 WRITE
	N_IPxyECMRead							= 0x3C,				// EXTERNAL M-DATA 설정 READ, 32bit, 0x80000000
	N_IPxyECMWrite							= 0xBC,				// EXTERNAL M-DATA 설정 WRITE
	N_IPxySTOPPWRead						= 0x3D,				// Stop pulse width Read
	N_IPxySTOPPWWrite						= 0xBD,				// Stop pulse width Write
	N_IPxyNoOperation_3E					= 0x3E,				// No operation
	N_IPxyNoOperation_BE					= 0xBE,				// No operation
	N_IPxyNoOperation_3F					= 0x3F,				// No operation
	N_IPxyTRGCMD							= 0xBF,				// TRIG output signal generation command
   	
	/* Interpolation Group	Registers	*/
	N_IPxCIRXCRead							= 0x40,				// Circular interpolation X axis center point read
	N_IPxCIRXCWrite							= 0xC0,				// Circular interpolation X axis center point write 
	N_IPxCIRYCRead							= 0x41,				// Circular interpolation Y axis center point read 
	N_IPxCIRYCWrite							= 0xC1,				// Circular interpolation Y axis center point write  
	N_IPxENDXRead							= 0x42,				// Interpolation X axis end point read 
	N_IPxENDXWrite							= 0xC2,				// Interpolation X axis end point write  
	N_IPxENDYRead							= 0x43,				// Interpolation Y axis end point read  
	N_IPxENDYWrite							= 0xC3,				// Interpolation Y axis end point write  
	N_IPxPTXENDRead							= 0x44,				// Pattern interpolation X Queue data read
	N_IPxPTXENDWrite						= 0xC4,				// Pattern interpolation X Queue data with queue push 
	N_IPxPTYENDRead							= 0x45,				// Pattern interpolation Y Queue data read 
	N_IPxPTYENDWrite						= 0xC5,				// Pattern interpolation Y Queue data write
	N_IPxPTQUEUERead						= 0x46,				// Pattern interpolation Queue index read
	N_IPxNoOperation_C6						= 0xC6,				// No operation
	N_IPxNoOperation_47						= 0x47,				// No operation
	N_IPxNoOperation_C7						= 0xC7,				// No operation
	N_IPxNoOperation_48						= 0x48,				// No operation
	N_IPxNoOperation_C8						= 0xC8,				// No operation
	N_IPxNoOperation_49						= 0x49,				// No operation
	N_IPxNoOperation_C9						= 0xC9,				// No operation
	N_IPxINPSTATUSRead						= 0x4A,				// Interpolation Status register read
	N_IPxNoOperation_CA						= 0xCA,				// No operation
	N_IPxINPMODE_4B							= 0x4B,				// Interpolation mode in Queue TOP contets
	N_IPxLINPDRV							= 0xCB,				// Linear interpolation with Queue push
	N_IPxINPMODE_4C							= 0x4C,				// Interpolation mode in Queue TOP contets
	N_IPxCINPDRV							= 0xCC,				// Circular interpolation with Queue push 
	N_IPxBPINPMODE							= 0x4D,				// Bit Pattern Interpolation mode in Queue TOP contets
	N_IPxBPINPDRV							= 0xCD,				// Bit pattern Drive
	N_IPxNoOperation_4E						= 0x4E,				// No Operation
	N_IPxNoOperation_CE						= 0xCE,				// No Operation 
	N_IPxNoOperation_4F						= 0x4F,				// No Operation 
	N_IPxNoOperation_CF						= 0xCF,				// No Operation 

	/* Arithemetic Group Register */
	N_IPxNoOperation_50						= 0x50,				// No Operation
	N_IPxINPCLR								= 0xD0,				// Initialize all interpolation control block
	N_IPxINPMPOINTRead						= 0x51,				// Interpolation deceleration manual point(unsigned) read
	N_IPxINPMPOINTWrite						= 0xD1,				// Interpolation deceleration manual point(unsigned) write
	N_IPxNoOperation_52						= 0x52,				// No Operation
	N_IPxINPCLRSWrite						= 0xD2,				// Initialize interpolation control block with target selection
	N_IPxNoOperation_53						= 0x53,				// No Operation
	N_IPxINPDRVWrite						= 0xD3,				// linear/circular drive start with queue data(Hold on mode), Restart on pause
	N_IPxNoOperation_54						= 0x54,				// No operation
	N_IPxNoOperation_D4						= 0xD4,				// No operation
	N_IPxNoOperation_55						= 0x55,				// No operation
	N_IPxARTSHOT							= 0xD5,				// Arithmetic block One time execution
	N_IPxARTSHOPERRead						= 0x56,				// Arithmetic block shift and operation selection Read
	N_IPxARTSHOPERWrite						= 0xD6,				// Arithmetic block shift and operation selection Write
	N_IPxARTSHRead							= 0x57,				// Arithmetic block shift amount data Read
	N_IPxARTSHWrite							= 0xD7,				// Arithmetic block shift amount data Write
	N_IPxARTSOURCERead						= 0x58,				// Arithmetic block operand configure data Read
	N_IPxARTSOURCEWrite						= 0xD8,				// Arithmetic block operand configure data Write
	N_IPxARTCRESULT1Read					= 0x59,				// Arithmetic first compare result data Read
	N_IPxNoOperation_D9						= 0xD9,				// No Operation
	N_IPxARTCRESULT2Read					= 0x5A,				// Arithmetic second compare result data Read
	N_IPxNoOperation_DA						= 0xDA,				// No Operation
	N_IPxARTARESULT1Read					= 0x5B,				// Arithmetic first algebraic result data Read
	N_IPxNoOperation_DB						= 0xDB,				// No Operation
	N_IPxARTARESULT2Read					= 0x5C,				// Arithmetic second algebraic result data Read
	N_IPxNoOperation_DC						= 0xDC,				// No operation
	N_IPxARTUSERARead						= 0x5D,				// Arithmetic block User operand A Read
	N_IPxARTUSERAWrite						= 0xDD,				// Arithmetic block User operand A Write
	N_IPxARTUSERBRead						= 0x5E,				// Arithmetic block User operand B Read
	N_IPxARTUSERBWrite						= 0xDE,				// Arithmetic block User operand B Write
	N_IPxARTUSERCRead						= 0x5F,				// Arithmetic block User operand C Read
	N_IPxARTUSERCWrite						= 0xDF,				// Arithmetic block User operand C Write

	/* Scripter Group Register			*/
	N_IPySCRCON1Read						= 0x40,				// 스크립트 동작 설정 레지스터-1 READ, 32bit, 0x00000000
	N_IPySCRCON1Write						= 0xC0,				// 스크립트 동작 설정 레지스터-1 WRITE
	N_IPySCRCON2Read						= 0x41,				// 스크립트 동작 설정 레지스터-2 READ, 32bit, 0x00000000
	N_IPySCRCON2Write						= 0xC1,				// 스크립트 동작 설정 레지스터-2 WRITE
	N_IPySCRCON3Read						= 0x42,				// 스크립트 동작 설정 레지스터-3 READ, 32bit, 0x00000000 
	N_IPySCRCON3Write						= 0xC2,				// 스크립트 동작 설정 레지스터-3 WRITE
	N_IPySCRCONQRead						= 0x43,				// 스크립트 동작 설정 레지스터-Queue READ, 32bit, 0x00000000
	N_IPySCRCONQWrite						= 0xC3,				// 스크립트 동작 설정 레지스터-Queue WRITE
	N_IPySCRDATA1Read						= 0x44,				// 스크립트 동작 데이터 레지스터-1 READ, 32bit, 0x00000000 
	N_IPySCRDATA1Write						= 0xC4,				// 스크립트 동작 데이터 레지스터-1 WRITE
	N_IPySCRDATA2Read						= 0x45,				// 스크립트 동작 데이터 레지스터-2 READ, 32bit, 0x00000000 
	N_IPySCRDATA2Write						= 0xC5,				// 스크립트 동작 데이터 레지스터-2 WRITE
	N_IPySCRDATA3Read						= 0x46,				// 스크립트 동작 데이터 레지스터-3 READ, 32bit, 0x00000000 
	N_IPySCRDATA3Write						= 0xC6,				// 스크립트 동작 데이터 레지스터-3 WRITE
	N_IPySCRDATAQRead						= 0x47,				// 스크립트 동작 데이터 레지스터-Queue READ, 32bit, 0x00000000 
	N_IPySCRDATAQWrite						= 0xC7,				// 스크립트 동작 데이터 레지스터-Queue WRITE
	N_IPyNoOperation_48						= 0x48,				// No operation
	N_IPySCRQCLR							= 0xC8,				// 스크립트 Queue clear
	N_IPySCRCQSIZERead						= 0x49,				// 스크립트 동작 설정 Queue 인덱스 READ, 4bit, 0x00
	N_IPyNoOperation_C9						= 0xC9,				// No operation
	N_IPySCRDQSIZERead						= 0x4A,				// 스크립트 동작 데이터 Queue 인덱스 READ, 4bit, 0x00
	N_IPyNoOperation_CA						= 0xCA,				// No operation
	N_IPySCRQFLAGRead						= 0x4B,				// 스크립트 Queue Full/Empty Flag READ, 4bit, 0x05
	N_IPyNoOperation_CB						= 0xCB,				// No operation
	N_IPySCRQSIZECONRead					= 0x4C,				// 스크립트 Queue size 설정(0~13) READ, 16bit, 0xD0D0
	N_IPySCRQSIZECONWrite					= 0xCC,				// 스크립트 Queue size 설정(0~13) WRITE
	N_IPySCRQSTATUSRead						= 0x4D,				// 스크립트 Queue status READ, 12bit, 0x005
	N_IPyNoOperation_CD						= 0xCD,				// No operation
	N_IPyNoOperation_4E						= 0x4E,				// No operation
	N_IPyNoOperation_CE						= 0xCE,				// No operation
	N_IPyNoOperation_4F						= 0x4F,				// No operation
	N_IPyNoOperation_CF						= 0xCF,				// No operation

	/* Caption Group Register */
	N_IPyCAPCON1Read						= 0x50,				// 갈무리 동작 설정 레지스터-1 READ, 32bit, 0x00000000
	N_IPyCAPCON1Write						= 0xD0,				// 갈무리 동작 설정 레지스터-1 WRITE
	N_IPyCAPCON2Read						= 0x51,				// 갈무리 동작 설정 레지스터-2 READ, 32bit, 0x00000000
	N_IPyCAPCON2Write						= 0xD1,				// 갈무리 동작 설정 레지스터-2 WRITE
	N_IPyCAPCON3Read						= 0x52,				// 갈무리 동작 설정 레지스터-3 READ, 32bit, 0x00000000 
	N_IPyCAPCON3Write						= 0xD2,				// 갈무리 동작 설정 레지스터-3 WRITE
	N_IPyCAPCONQRead						= 0x53,				// 갈무리 동작 설정 레지스터-Queue READ, 32bit, 0x00000000
	N_IPyCAPCONQWrite						= 0xD3,				// 갈무리 동작 설정 레지스터-Queue WRITE
	N_IPyCAPDATA1Read						= 0x54,				// 갈무리 동작 데이터 레지스터-1 READ, 32bit, 0x00000000 
	N_IPyNoOperation_D4						= 0xD4,				// No operation
	N_IPyCAPDATA2Read						= 0x55,				// 갈무리 동작 데이터 레지스터-2 READ, 32bit, 0x00000000 
	N_IPyNoOperation_D5						= 0xD5,				// No operation
	N_IPyCAPDATA3Read						= 0x56,				// 갈무리 동작 데이터 레지스터-3 READ, 32bit, 0x00000000 
	N_IPyNoOperation_D6						= 0xD6,				// No operation
	N_IPyCAPDATAQRead						= 0x57,				// 갈무리 동작 데이터 레지스터-Queue READ, 32bit, 0x00000000 
	N_IPyNoOperation_D7						= 0xD7,				// No operation
	N_IPyNoOperation_58						= 0x58,				// No operation
	N_IPyCAPQCLR							= 0xD8,				// 갈무리 Queue clear
	N_IPyCAPCQSIZERead						= 0x59,				// 갈무리 동작 설정 Queue 인덱스 READ, 4bit, 0x00
	N_IPyNoOperation_D9						= 0xD9,				// No operation
	N_IPyCAPDQSIZERead						= 0x5A,				// 갈무리 동작 데이터 Queue 인덱스 READ, 4bit, 0x00
	N_IPyNoOperation_DA						= 0xDA,				// No operation
	N_IPyCAPQFLAGRead						= 0x5B,				// 갈무리 Queue Full/Empty Flag READ, 4bit, 0x05
	N_IPyNoOperation_DB						= 0xDB,				// No operation
	N_IPyCAPQSIZECONRead					= 0x5C,				// 갈무리 Queue size 설정(0~13) READ, 16bit, 0xD0D0
	N_IPyCAPQSIZECONWrite					= 0xDC,				// 갈무리 Queue size 설정(0~13) WRITE
	N_IPyCAPQSTATUSRead						= 0x5D,				// 갈무리 Queue status READ, 12bit, 0x005
	N_IPyNoOperation_DD						= 0xDD,				// No operation
	N_IPyNoOperation_5E						= 0x5E,				// No operation
	N_IPyNoOperation_DE						= 0xDE,				// No operation
	N_IPyNoOperation_5F						= 0x5F,				// No operation
	N_IPyNoOperation_DF						= 0xDF,				// No operation

	/* BUS - 1 Group Register			*/
	N_IPxyINCNTRead							= 0x60,				// INTERNAL COUNTER DATA READ(Signed), 32bit, 0x00000000
	N_IPxyINCNTWrite						= 0xE0,				// INTERNAL COUNTER DATA WRITE(Signed)
	N_IPxyINCNTCMPRead						= 0x61,				// INTERNAL COUNTER COMPARATE DATA READ(Signed), 32bit, 0x00000000
	N_IPxyINCNTCMPWrite						= 0xE1,				// INTERNAL COUNTER COMPARATE DATA WRITE(Signed)
	N_IPxyINCNTSCALERead					= 0x62,				// INTERNAL COUNTER PRE-SCALE DATA READ, 8bit, 0x00
	N_IPxyINCNTSCALEWrite					= 0xE2,				// INTERNAL COUNTER PRE-SCALE DATA WRITE
	N_IPxyICPRead							= 0x63,				// INTERNAL COUNTER P-DATA READ, 32bit, 0x7FFFFFFF
	N_IPxyICPWrite							= 0xE3,				// INTERNAL COUNTER P-DATA WRITE
	N_IPxyEXCNTRead							= 0x64,				// EXTERNAL COUNTER DATA READ READ(Signed), 32bit, 0x00000000
	N_IPxyEXCNTWrite						= 0xE4,				// EXTERNAL COUNTER DATA READ WRITE(Signed)
	N_IPxyEXCNTCMPRead						= 0x65,				// EXTERNAL COUNTER COMPARATE DATA READ(Signed), 32bit, 0x00000000
	N_IPxyEXCNTCMPWrite						= 0xE5,				// EXTERNAL COUNTER COMPARATE DATA WRITE(Signed)
	N_IPxyEXCNTSCALERead					= 0x66,				// EXTERNAL COUNTER PRE-SCALE DATA READ, 8bit, 0x00
	N_IPxyEXCNTSCALEWrite					= 0xE6,				// EXTERNAL COUNTER PRE-SCALE DATA WRITE
	N_IPxyEXPRead							= 0x67,				// EXTERNAL COUNTER P-DATA READ, 32bit, 0x7FFFFFFF
	N_IPxyEXPWrite							= 0xE7,				// EXTERNAL COUNTER P-DATA WRITE
	N_IPxyEXSPDRead							= 0x68,				// EXTERNAL SPEED DATA READ, 32bit, 0x00000000
	N_IPxyNoOperation_E8					= 0xE8,				// No operation
	N_IPxyEXSPDCMPRead						= 0x69,				// EXTERNAL SPEED COMPARATE DATA READ, 32bit, 0x00000000
	N_IPxyEXSPDCMPWrite						= 0xE9,				// EXTERNAL SPEED COMPARATE DATA WRITE
	N_IPxyEXFILTERDRead						= 0x6A,				// 외부 센서 필터 대역폭 설정 DATA READ, 32bit, 0x00050005
	N_IPxyEXFILTERDWrite					= 0xEA,				// 외부 센서 필터 대역폭 설정 DATA WRITE
	N_IPxyOFFREGIONRead						= 0x6B,				// OFF-RANGE DATA READ, 8bit, 0x00
	N_IPxyOFFREGIONWrite					= 0xEB,				// OFF-RANGE DATA WRITE
	N_IPxyDEVIATIONRead						= 0x6C,				// DEVIATION DATA READ, 16bit, 0x0000
	N_IPxyNoOperation_EC					= 0xEC,				// No operation
	N_IPxyPGMCHRead							= 0x6D,				// PGM REGISTER CHANGE DATA READ
	N_IPxyPGMCHWrite						= 0xED,				// PGM REGISTER CHANGE DATA WRITE
	N_IPxyCOMPCONRead						= 0x6E,				// COMPARE REGISTER INPUT CHANGE DATA READ
	N_IPxyCOMPCONWrite						= 0xEE,				// COMPARE REGISTER INPUT CHANGE DATA WRITE
	N_IPxyNoOperation_6F					= 0x6F,				// No operation
	N_IPxyNoOperation_EF					= 0xEF,				// No operation

	/* BUS - 2 Group Register			*/
	N_IPxyFUNCONRead						= 0x70,				// 칩 기능 설정 DATA READ,
	N_IPxyFUNCONWrite						= 0xF0,				// 칩 기능 설정 DATA WRITE
	N_IPxyMODE1Read							= 0x71,				// MODE1 DATA READ,
	N_IPxyMODE1Write						= 0xF1,				// MODE1 DATA WRITE
	N_IPxyMODE2Read							= 0x72,				// MODE2 DATA READ,
	N_IPxyMODE2Write						= 0xF2,				// MODE2 DATA WRITE
	N_IPxyUIODATARead						= 0x73,				// UNIVERSAL IN READ,
	N_IPxyUIODATAWrite						= 0xF3,				// UNIVERSAL OUT WRITE
	N_IPxyENDSTATUSRead						= 0x74,				// END STATUS DATA READ,
	N_IPxyCLIMCLR							= 0xF4,				// Complete limit stop clear command
	N_IPxyMECHRead							= 0x75,				// MECHANICAL SIGNAL DATA READ, 13bit
	N_IPxyNoOperation_F5					= 0xF5,				// No operation
	N_IPxyDRVSTATUSRead						= 0x76,				// DRIVE STATE DATA READ, 20bit
	N_IPxyNoOperation_F6					= 0xF6,				// No operation
	N_IPxyEXCNTCLRRead						= 0x77,				// EXTERNAL COUNTER 설정 DATA READ, 9bit, 0x00
	N_IPxyEXCNTCLRWrite						= 0xF7,				// EXTERNAL COUNTER 설정 DATA WRITE
	N_IPxyNoOperation_78					= 0x78,				// No operation
	N_IPxySWRESET							= 0xF8,				// REGISTER CLEAR(INITIALIZATION), Software reset
	N_IPxyINTFLAG1Read						= 0x79,				// Interrupt Flag1 READ, 32bit, 0x00000000
	N_IPxyINTFLAG1CLRWrite					= 0xF9,				// Interrupt Flag1 Clear data write command.
	N_IPxyINTMASK1Read						= 0x7A,				// Interrupt Mask1 READ, 32bit, 0x00000001
	N_IPxyINTMASK1Write						= 0xFA,				// Interrupt Mask1 WRITE
	N_IPxyUIOMODERead						= 0x7B,				// UIO MODE DATA READ, 12bit, 0x01F
	N_IPxyUIOMODEWrite						= 0xFB,				// UIO MODE DATA WRITE
	N_IPxyINTFLAG2Read						= 0x7C,				// Interrupt Flag2 READ, 32bit, 0x00000000
	N_IPxyINTFLAG2CLRWrite					= 0xFC,				// Interrupt Flag2 Clear data write command.
	N_IPxyINTMASK2Read						= 0x7D,				// Interrupt Mask2 READ, 32bit, 0x00000001
	N_IPxyINTMASK2Write						= 0xFD,				// Interrupt Mask2 WRITE
	N_IPxyINTUSERCONRead					= 0x7E,				// User interrupt selection control.
	N_IPxyINTUSERCONWrite					= 0xFE,				// User interrupt selection control. 
	N_IPxyNoOperation_7F					= 0x7F,				// No operation
	N_IPxyINTGENCMD							= 0xFF				// Interrupt generation command.
} N_IPCOMMAND;

/* CAMC-QI COMMAND LIST							*/
typedef enum _N_QICOMMAND
{
    // Previous register and etc Registers
	N_QiPRANGERead							= 0x00,				// Previous RANGE READ
	N_QiPRANGEWrite							= 0x80,				// Previous RANGE WRITE
	N_QiPSTDRead							= 0x01,				// Previous START/STOP SPEED DATA READ
	N_QiPSTDWrite							= 0x81,				// Previous START/STOP SPEED DATA WRITE
	N_QiPOBJRead							= 0x02,				// Previous OBJECT SPEED DATA READ
	N_QiPOBJWrite							= 0x82,				// Previous OBJECT SPEED DATA WRITE
	N_QiPRATE1Read							= 0x03,				// Previous RATE-1 DATA READ
	N_QiPRATE1Write							= 0x83,				// Previous RATE-1 DATA WRITE
	N_QiPRATE2Read							= 0x04,				// Previous RATE-2 DATA READ
	N_QiPRATE2Write							= 0x84,				// Previous RATE-2 DATA WRITE
	N_QiPSW1Read							= 0x05,				// Previous SW-1 DATA READ
	N_QiPSW1Write							= 0x85,				// Previous SW-1 DATA WRITE
	N_QiPSW2Read							= 0x06,				// Previous SW-2 DATA READ
	N_QiPSW2Write							= 0x86,				// Previous SW-2 DATA WRITE
	N_QiPDCFGRead							= 0x07,				// Previous Drive configure data READ
	N_QiPDCFGWrite							= 0x87,				// Previous Drive configure data WRITE
	N_QiPREARRead							= 0x08,				// Previous SLOW DOWN/REAR PULSE READ
	N_QiPREARWrite							= 0x88,				// Previous SLOW DOWN/REAR PULSE WRITE
	N_QiPPOSRead							= 0x09,				// Previous Drive pulse amount data/Interpolation end position READ
	N_QiPPOSWrite							= 0x89,				// Previous Drive pulse amount data/Interpolation end position WRITE 
	N_QiPCENTRead							= 0x0A,				// Previous Circular Int. center/Master axis target position for multiple chip linear int. READ
	N_QiPCENTWrite							= 0x8A,				// Previous Circular Int. center/Master axis target position for multiple chip linear int. WRITE
	N_QiPISNUMRead							= 0x0B,				// Previous Interpolation step number READ
	N_QiPISNUMWrite							= 0x8B,				// Previous Interpolation step number WRITE
	N_QiNoOperation_0C						= 0x0C,				// No operation
	N_QiCLRPRE								= 0x8C,				// Clear previous driving data Queue.
	N_QiNoOperation_0D						= 0x0D,				// No operation
	N_QiPOPPRE								= 0x8D,				// Pop and shift data of previous driving data Queue.
	N_QiPPORTMARestore						= 0x0E,				// Restore data ports.
	N_QiPPORTMABackup						= 0x8E,				// Backup data ports.
	N_QiCURSPDRead							= 0x0F,				// Current SPEED DATA READ
	N_QiNoOperation_8F						= 0x8F,				// No operation

	// Working Registers
	N_QiRANGERead							= 0x10,				// RANGE READ
	N_QiRANGEWrite							= 0x90,				// RANGE WRITE
	N_QiSTDRead								= 0x11,				// START/STOP SPEED DATA READ
	N_QiSTDWrite							= 0x91,				// START/STOP SPEED DATA WRITE
	N_QiOBJRead								= 0x12,				// OBJECT SPEED DATA READ
	N_QiOBJWrite							= 0x92,				// OBJECT SPEED DATA WRITE
	N_QiRATE1Read							= 0x13,				// RATE-1 DATA READ
	N_QiRATE1Write							= 0x93,				// RATE-1 DATA WRITE
	N_QiRATE2Read							= 0x14,				// RATE-2 DATA READ
	N_QiRATE2Write							= 0x94,				// RATE-2 DATA WRITE
	N_QiSW1Read								= 0x15,				// SW-1 DATA READ
	N_QiSW1Write							= 0x95,				// SW-1 DATA WRITE
	N_QiSW2Read								= 0x16,				// SW-2 DATA READ
	N_QiSW2Write							= 0x96,				// SW-2 DATA WRITE
	N_QiDCFGRead							= 0x17,				// Drive configure data READ
	N_QiDCFGWrite							= 0x97,				// Drive configure data WRITE
	N_QiREARRead							= 0x18,				// SLOW DOWN/REAR PULSE READ
	N_QiREARWrite							= 0x98,				// SLOW DOWN/REAR PULSE WRITE
	N_QiPOSRead								= 0x19,				// Drive pulse amount data/Interpolation end position READ
	N_QiPOSWrite							= 0x99,				// Drive pulse amount data/Interpolation end position WRITE 
	N_QiCENTRead							= 0x1A,				// Circular Int. center/Master axis target position for multiple chip linear int. READ
	N_QiCENTWrite							= 0x9A,				// Circular Int. center/Master axis target position for multiple chip linear int. WRITE
	N_QiISNUMRead							= 0x1B,				// Interpolation step number READ
	N_QiISNUMWrite							= 0x9B,				// Interpolation step number WRITE
	N_QiREMAIN								= 0x1C,				// Remain pulse data after stopping preset drive function abnormally.
	N_QiNoOperation_9C						= 0x9C,				// No operation
	N_QiOBJORGRead							= 0x1F,				// Original search object speed READ
	N_QiOBJORGWrite							= 0x9F,				// Original search object speed WRITE

	// Universal in/out setting
	N_QiUIOMRead							= 0x1D,				// Universal in/out terminal mode READ
	N_QiUIOMWrite							= 0x9D,				// Universal in/out terminal mode WRITE
	N_QiUIORead								= 0x1E,				// Universal in/out terminal mode READ
	N_QiUIOWrite							= 0x9E,				// Universal in/out terminal mode WRIT

	// Drive start command
	N_QiNoOperation_20						= 0x20,				// No operation.
	N_QiSTRN								= 0xA0,				// Normal profile mode drive start.(STD => OBJ => STD)
	N_QiNoOperation_21						= 0x21,				// No operation.
	N_QiSTRO								= 0xA0,				// Start at OBJ profile mode drive start.(OBJ => STD)
	N_QiNoOperation_22						= 0x22,				// No operation.
	N_QiSTRCO								= 0xA0,				// Constant speed profile #1 drive start.(OBJ)
	N_QiNoOperation_23						= 0x23,				// No operation.
	N_QiSTRCS								= 0xA0,				// Constant speed profile #2 drive start.(STD)
	N_QiNoOperation_60						= 0x5C,				// No operation.
	N_QiASTRN								= 0xDC,				// Normal profile mode drive start with DCFG7~0 bit data in DATAPL0 port.(STD => OBJ => STD)
	N_QiNoOperation_61						= 0x5D,				// No operation.
	N_QiASTRO								= 0xDD,				// Start at OBJ profile mode drive start with DCFG7~0 bit data in DATAPL0 port.(OBJ => STD)
	N_QiNoOperation_62						= 0x5E,				// No operation.
	N_QiASTRCO								= 0xDE,				// Constant speed profile #1 drive start with DCFG7~0 bit data in DATAPL0 port.(OBJ)
	N_QiNoOperation_63						= 0x5F,				// No operation.
	N_QiASTRCS								= 0xDF,				// Constant speed profile #2 drive start with DCFG7~0 bit data in DATAPL0 port.(STD)

	// Drive control command
	N_QiNoOperation_24						= 0x24,				// No operation.
	N_QiSSTOP								= 0xA4,				// Slow Down stop.
	N_QiNoOperation_25						= 0x25,				// No operation.
	N_QiSTOP								= 0xA5,				// Immediately stop.
	N_QiNoOperation_26						= 0x26,				// No operation.
	N_QiSQRO1								= 0xA6,				// Output one shot of the start pulse form SQSTR1 terminal.
	N_QiNoOperation_27						= 0x27,				// No operation.
	N_QiSQRO2								= 0xA7,				// Output one shot of the start pulse form SQSTR2 terminal.
	N_QiNoOperation_28						= 0x28,				// No operation.
	N_QiSQRI1								= 0xA8,				// Execution sync start function same as SQSTR1 input.
	N_QiNoOperation_29						= 0x29,				// No operation.
	N_QiSQRI2								= 0xA9,				// Execution sync start function same as SQSTR2 input.
	N_QiNoOperation_2A						= 0x2A,				// No operation
	N_QiSQSTP1								= 0xAA,				// Output one shot of the stop pulse from SQSTP1 terminal.
	N_QiNoOperation_2B						= 0x2B,				// No operation.
	N_QiSQSTP2								= 0xAB,				// Output one shot of the stop pulse from SQSTP2 terminal.
	N_QiISCNTRead							= 0x2C,				// Interpolation stop counter value READ.
	N_QiNoOperation_AC						= 0xAC,				// No operation.
	N_QiISACNTRead							= 0x2D,				// Interpolation step counter READ for advanced deceleration mode . 
	N_QiNoOperation_AD						= 0xAD,				// No operation.
	N_QiNoOperation_2E						= 0x2E,				// No operation.
	N_QiESTOP								= 0xAE,				// Emergency stop all axis.
	N_QiNoOperation_2F						= 0x2F,				// No operation
	N_QiSWRESET								= 0xAF,				// Software reset(all axis).
	N_QiNoOperation_30						= 0x30,				// No operation
	N_QiDRPCNTRead							= 0xB0,				// Driven pulse amount during last driving(Interpolation step counter for path move).
	N_QiNoOperation_31						= 0x31,				// No operation
	N_QiINTGEN								= 0xB1,				// Interrupt generation command.

	// Peripheral function setting.
	N_QiNoOperation_33						= 0x32,				// No operation.
	N_QiTRGQPOP								= 0xB2,				// Pop and shift data in trigger position queue.
	N_QiTRTMCFRead							= 0x33,				// Trigger/Timer configure READ.
	N_QiTRTMCFWrite							= 0xB3,				// Trigger/Timer configure WRITE.
	N_QiSNSMTRead							= 0x34,				// Software negative limit position READ.
	N_QiSNSMTWrite							= 0xB4,				// Software negative limit position WRITE.
	N_QiSPSMTRead							= 0x35,				// Software positive limit position READ.
	N_QiSPSMTWrite							= 0xB5,				// Software positive limit position WRITE.
	N_QiTRGPWRead							= 0x36,				// Trigger pulse width READ.
	N_QiTRGPWWrite							= 0xB6,				// Trigger pulse width WRITE.
	N_QiTRGSPRead							= 0x37,				// Trigger function start position READ.
	N_QiTRGSPWrite							= 0xB7,				// Trigger function start position WRITE.
	N_QiTRGEPRead							= 0x38,				// Trigger function end position READ.
	N_QiTRGEPWrite							= 0xB8,				// Trigger function end position WRITE.
	N_QiPTRGPOSRead							= 0x39,				// Trigger position or period queue data READ.
	N_QiPTRGPOSWrite						= 0xB9,				// Push trigger position or period queue.
	N_QiNoOperation_3A						= 0x3A,				// No operation.
	N_QiCLRTRIG								= 0xBA,				// Clear trigger position or period queue.
	N_QiNoOperation_3B						= 0x3B,				// No operation.
	N_QiTRGGEN								= 0xBB,				// Generate one shot trigger pulse.
	N_QiTMRP1Read							= 0x3C,				// Timer #1 period data READ.
	N_QiTMRP1Write							= 0xBC,				// Timer #1 period data WRITE.
	N_QiTMRP2Read							= 0x3D,				// Timer #2 period data READ.
	N_QiTMRP2Write							= 0xBD,				// Timer #2 period data WRITE.
	N_QiTMR1GENstop							= 0x3E,				// Timer #1 stop.
	N_QiTMR1GENstart						= 0xBE,				// Timer #1 start.
	N_QiTMR2GENstop							= 0x3F,				// Timer #2 stop.
	N_QiTMR2GENstart						= 0xBF,				// Timer #2 start.
	N_QiERCReset							= 0x60,				// ERC signal reset.
	N_QiERCSet								= 0xE0,				// ERC signal set.

	//Script1/2/3 setting registers
	N_QiSCRCON1Read							= 0x40,				// Script1 control queue register READ.
	N_QiSCRCON1Write						= 0xC0,				// Script1 control queue register WRITE.
	N_QiSCRCMD1Read							= 0x41,				// Script1 command queue register READ.
	N_QiSCRCMD1Write						= 0xC1,				// Script1 command queue register WRITE.
	N_QiSCRDAT1Read							= 0x42,				// Script1 execution data queue register READ.
	N_QiSCRDAT1Write						= 0xC2,				// Script1 execution data queue register WRITE.
	N_QiCQ1Read								= 0x43,				// Script1 captured data queue register(top of depth 15 Queue)READ.
	N_QiNoOperation_C3						= 0xC3,				// No operation.
	N_QiSCRCFG1Read							= 0x44,				// Script1 flag control register READ.
	N_QiSCRCFG1Write						= 0xC4,				// Script1 flag control register WRITE. 
	N_QiSCRCON2Read							= 0x45,				// Script2 control queue register READ.
	N_QiSCRCON2Write						= 0xC5,				// Script2 control queue register WRITE.
	N_QiSCRCMD2Read							= 0x46,				// Script2 command queue register READ.
	N_QiSCRCMD2Write						= 0xC6,				// Script2 command queue register WRITE.
	N_QiSCRDAT2Read							= 0x47,				// Script2 execution data queue register READ.
	N_QiSCRDAT2Write						= 0xC7,				// Script2 execution data queue register WRITE.
	N_QiCQ2Read								= 0x48,				// Script2 captured data queue register(top of depth 15 Queue)READ.
	N_QiNoOperation_C8						= 0xC8,				// No operation.
	N_QiSCRCFG2Read							= 0x49,				// Script2 flag control register READ.
	N_QiSCRCFG2Write						= 0xC9,				// Script2 flag control register WRITE. 
	N_QiSCRCON3Read							= 0x4A,				// Script3 control register READ.
	N_QiSCRCON3Write						= 0xCA,				// Script3 control register WRITE.
	N_QiSCRCMD3Read							= 0x4B,				// Script3 command register READ.
	N_QiSCRCMD3Write						= 0xCB,				// Script3 command register WRITE.
	N_QiSCRDAT3Read							= 0x4C,				// Script3 execution data register READ.
	N_QiSCRDAT3Write						= 0xCC,				// Script3 execution data register WRITE.
	N_QiCQ3Read								= 0x4D,				// Script3 captured data register READ.
	N_QiNoOperation_CD						= 0xCD,				// No operation.
	N_QiNoOperation_4E						= 0x4E,				// No operation.
	N_QiNoOperation_CE						= 0xCE,				// No operation.
	N_QiNoOperation_4F						= 0x4F,				// No operation.
	N_QiNoOperation_CF						= 0xCF,				// [No operation code for script reservation command].

	//Script4 and Script status setting registers
	N_QiSCRCON4Read							= 0x50,				// Script4 control register READ.
	N_QiSCRCON4Write						= 0xD0,				// Script4 control register WRITE.
	N_QiSCRCMD4Read							= 0x51,				// Script4 command register READ.
	N_QiSCRCMD4Write						= 0xD1,				// Script4 command register WRITE.
	N_QiSCRDAT4Read							= 0x52,				// Script4 execution data register READ.
	N_QiSCRDAT4Write						= 0xD2,				// Script4 execution data register WRITE.
	N_QiCQ4Read								= 0x53,				// Script4 captured data register READ.
	N_QiNoOperation_D3						= 0xD3,				// No operation.
	N_QiSCRTGRead							= 0x54,				// Target source data setting READ.
	N_QiSCRTGWrite							= 0xD4,				// Target source data setting WRITE.
	N_QiSCRSTAT1Read						= 0x55,				// Script status #1 READ.
	N_QiNoOperation_D5						= 0xD5,				// No operation.
	N_QiSCRSTAT2Read						= 0x56,				// Script status #2 READ.
	N_QiNoOperation_D6						= 0xD6,				// No operation.
	N_QiNoOperation_57						= 0x57,				// No operation.
	N_QiINITSQWrite							= 0xD7,				// Initialize script queues with target selection.
	N_QiNoOperation_58						= 0x58,				// No operation.
	N_QiINITCQWrite							= 0xD8,				// Initialize captured data queue with target selection.
	N_QiSCRMRead							= 0x59,				// Set enable mode with target selection READ.
	N_QiSCRMWrite							= 0xD9,				// Set enable mode with target selection WRITE.
	N_QiNoOperation_5A						= 0x5A,				// No operation.
	N_QiSQ1POP								= 0xDA,				// Pop and shift data of script1 queue.
	N_QiNoOperation_5B						= 0x5B,				// No operation.
	N_QiSQ2POP								= 0xDB,				// Pop and shift data of script2 queue.

	//Counter function registers
	N_QiCNTLBRead							= 0x61,				// Counter lower bound data READ.
	N_QiCNTLBWrite							= 0xE1,				// Counter lower bound data WRITE.
	N_QiCNTUBRead							= 0x62,				// Counter upper bound data READ.
	N_QiCNTUBWrite							= 0xE2,				// Counter upper bound data WRITE.
	N_QiCNTCF1Read							= 0x63,				// Counter configure #1 READ.
	N_QiCNTCF1Write							= 0xE3,				// Counter configure #1 WRITE.
	N_QiCNTCF2Read							= 0x64,				// Counter configure #2 READ.
	N_QiCNTCF2Write							= 0xE4,				// Counter configure #2 WRITE.
	N_QiCNTCF3Read							= 0x65,				// Counter configure #3 READ.
	N_QiCNTCF3Write							= 0xE5,				// Counter configure #3 WRITE.
	N_QiCNT1Read							= 0x66,				// Counter #1 data READ.
	N_QiCNT1Write							= 0xE6,				// Counter #1 data WRITE.
	N_QiCNT2Read							= 0x67,				// Counter #2 data READ.
	N_QiCNT2Write							= 0xE7,				// Counter #2 data WRITE.
	N_QiCNT3Read							= 0x68,				// Counter #3 data READ.
	N_QiCNT3Write							= 0xE8,				// Counter #3 data WRITE.
	N_QiCNT4Read							= 0x69,				// Counter #4 data READ.
	N_QiCNT4Write							= 0xE9,				// Counter #4 data WRITE.
	N_QiCNT5Read							= 0x6A,				// Counter #5 data READ.
	N_QiCNT5Write							= 0xEA,				// Counter #5 data WRITE.
	N_QiCNTC1Read							= 0x6B,				// Counter #1 comparator's data READ.
	N_QiCNTC1Write							= 0xEB,				// Counter #1 comparator's data WRITE.
	N_QiCNTC2Read							= 0x6C,				// Counter #2 comparator's data READ.
	N_QiCNTC2Write							= 0xEC,				// Counter #2 comparator's data WRITE.
	N_QiCNTC3Read							= 0x6D,				// Counter #3 comparator's data READ.
	N_QiCNTC3Write							= 0xED,				// Counter #3 comparator's data WRITE.
	N_QiCNTC4Read							= 0x6E,				// Counter #4 comparator's data READ.
	N_QiCNTC4Write							= 0xEE,				// Counter #4 comparator's data WRITE.
	N_QiCNTC5Read							= 0x6F,				// Counter #5 comparator's data READ.
	N_QiCNTC5Write							= 0xEF,				// Counter #5 comparator's data WRITE.

	// Configure and Status registers
	N_QiUCFG1Read							= 0x70,				// Configure register #1 READ.
	N_QiUCFG1Write							= 0xF0,				// Configure register #1 WRITE.
	N_QiUCFG2Read							= 0x71,				// Configure register #2 READ.
	N_QiUCFG2Write							= 0xF1,				// Configure register #2 WRITE.
	N_QiUCFG3Read							= 0x72,				// Configure register #3 READ.
	N_QiUCFG3Write							= 0xF2,				// Configure register #3 WRITE.
	N_QiUCFG4Read							= 0x73,				// Configure register #4 READ.
	N_QiUCFG4Write							= 0xF3,				// Configure register #4 WRITE.
	N_QiNoOperation_74						= 0x74,				// No operation.
	N_QiNoOperation_F4						= 0xF4,				// No operation.
	N_QiNoOperation_75						= 0x75,				// No operation.
	N_QiNoOperation_F5						= 0xF5,				// No operation.
	N_QiNoOperation_76						= 0x76,				// No operation.
	N_QiNoOperation_F6						= 0xF6,				// No operation.
	N_QiIMASK1Read							= 0x77,				// Interrupt bank#1 mask register READ.
	N_QiIMASK1Write							= 0xF7,				// Interrupt bank#1 mask register WRITE.
	N_QiIMASK2Read							= 0x78,				// Interrupt bank#2 mask register READ.
	N_QiIMASK2Write							= 0xF8,				// Interrupt bank#2 mask register WRITE.
	N_QiSTAT1Read							= 0x79,				// Status register #1(END STATUS)READ.
	N_QiESCLR								= 0xF9,				// Status register #1(END STATUS) Clear.
	N_QiSTAT2Read							= 0x7A,				// Status register #2 READ.
	N_QiNoOperation_FA						= 0xFA,				// No operation.
	N_QiSTAT3Read							= 0x7B,				// Status register #3 READ.
	N_QiNoOperation_FB						= 0xFB,				// No operation.
	N_QiSTAT4Read							= 0x7C,				// Status register #4 READ.
	N_QiNoOperation_FC						= 0xFC,				// No operation.
	N_QiSTAT5Read							= 0x7D,				// Status register #5 READ.
	N_QiNoOperation_FD						= 0xFD,				// No operation.
	N_QiIFLAG1Read							= 0x7E,				// Interrupt bank #1 flag READ.
	N_QiIFLAG1Clear							= 0xFE,				// Interrupt bank #1 flag Clear.
	N_QiIFLAG2Read							= 0x7F,				// Interrupt bank #2 flag READ.
	N_QiIFLAG2Clear							= 0xFF,				// Interrupt bank #2 flag Clear.

} N_QICOMMAND;


//--------------------------------------------------------------------------------------------//
// CAMC-QI Script/Caption Define
//--------------------------------------------------------------------------------------------//
#define N_QI_SCR_REG1							1
#define N_QI_SCR_REG2							2
#define N_QI_SCR_REG3							3
#define N_QI_SCR_REG4							4

#define N_QI_OPERATION_ONCE_RUN					0x00000000					// bit 24 OFF
#define N_QI_OPERATION_CONTINUE_RUN				0x01000000					// bit 24 ON
#define N_QI_INPUT_DATA_FROM_SCRIPT_DATA		0x00000000					// bit 23 OFF,
#define N_QI_INPUT_DATA_FROM_TARGET_REG			0x00800000					// bit 23 ON,
//JEKIM. 070327
//#define N_QI_INTERRUPT_GEN_ENABLE				0x00000000					// bit 22 OFF,
//#define N_QI_INTERRUPT_GEN_DISABLE			0x00400000					// bit 22 ON,
#define N_QI_INTERRUPT_GEN_ENABLE				0x00400000					// bit 22 ON,
#define N_QI_INTERRUPT_GEN_DISABLE				0x00000000					// bit 22 OFF,
#define N_QI_OPERATION_EVENT_NONE				0x00000000					// bit 21=OFF, 20=OFF
//JEKIM. 070327
//#define N_QI_OPERATION_EVENT_OR				0x00100000					// bit 21=OFF, 20=ON
//#define N_QI_OPERATION_EVENT_AND				0x00200000					// bit 21=ON,  20=OF
#define N_QI_OPERATION_EVENT_AND				0x00100000					// bit 21=OFF, 20=ON
#define N_QI_OPERATION_EVENT_OR					0x00200000					// bit 21=ON,  20=OFF
#define N_QI_OPERATION_EVENT_XOR				0x00300000					// bit 21=ON,  20=ON
#define N_QI_SND_EVENT_AXIS(Axis)				((Axis % 4) << 18)			// bit 19~18 (00:X, 01:Y, 10:Z, 11:U)
#define N_QI_FST_EVENT_AXIS(Axis)				((Axis % 4) << 16)			// bit 17~16 (00:X, 01:Y, 10:Z, 11:U)
#define N_QI_OPERATION_EVENT_2(Event)			((Event & 0xFF) << 8)		// bit 15..8
#define N_QI_OPERATION_EVENT_1(Event)			(Event & 0xFF)				// bit 7..0
#define N_QI_OPERATION_COMMAND(Command, Axis)	(Command&0xFF << ((Axis % 4) * 8))		// bit 7..0 : enum _N_QISCOMMAND 참조

typedef enum _N_QIEVENT
{
	N_EVENT_QINOOP							= 0x00,				// No operation.
	N_EVENT_QIDRVEND						= 0x01,				// Drive end event(inposition function excluded).
	N_EVENT_QIDECEL							= 0X02,				// Deceleration state.
	N_EVENT_QICONST							= 0x03,				// Constant speed state.
	N_EVENT_QIACCEL							= 0X04,				// Acceleration state.
	N_EVENT_QICNT1L							= 0x05,				// Counter1 < Comparater1 state.
	N_EVENT_QICNT1E							= 0X06,				// Counter1 = Comparater1 state.
	N_EVENT_QICNT1G							= 0x07,				// Counter1 > Comparater1 state.
	N_EVENT_QICNT1LE						= 0x08,				// Counter1 ≤ Comparater1 state.
	N_EVENT_QICNT1GE						= 0x09,				// Counter1 ≥ Comparater1 state.
	N_EVENT_QICNT1EUP						= 0x0A,				// Counter1 = Comparater1 event during counting up.
	N_EVENT_QICNT1EDN						= 0x0B,				// Counter1 = Comparater1 event during counting down.
	N_EVENT_QICNT1BND						= 0x0C,				// Counter1 is same with boundary value.

	N_EVENT_QICNT2L							= 0x0D,				// Counter2 < Comparater2 state.
	N_EVENT_QICNT2E							= 0x0E,				// Counter2 = Comparater2 state.
	N_EVENT_QICNT2G							= 0x0F,				// Counter2 > Comparater2 state.
	N_EVENT_QICNT2LE						= 0x10,				// Counter2 ≤ Comparater2 state.
	N_EVENT_QICNT2GE						= 0x11,				// Counter2 ≥ Comparater2 state.
	N_EVENT_QICNT2EUP						= 0x12,				// Counter2 = Comparater2 event during counting up.
	N_EVENT_QICNT2EDN						= 0x13,				// Counter2 = Comparater2 event during counting down.
	N_EVENT_QICNT2BND						= 0x14,				// Counter2 is same with boundary value.


	N_EVENT_QICNT3L							= 0x15,				// Counter3 < Comparater3 state.
	N_EVENT_QICNT3E							= 0x16,				// Counter3 = Comparater3 state.
	N_EVENT_QICNT3G							= 0x17,				// Counter3 > Comparater3 state.
	N_EVENT_QICNT3LE						= 0x18,				// Counter3 ≤ Comparater3 state.
	N_EVENT_QICNT3GE						= 0x19,				// Counter3 ≥ Comparater3 state.
	N_EVENT_QICNT3EUP						= 0x1A,				// Counter3 = Comparater3 event during counting up.
	N_EVENT_QICNT3EDN						= 0x1B,				// Counter3 = Comparater3 event during counting down.
	N_EVENT_QICNT3BND						= 0x1C,				// Counter3 is same with boundary value.
	N_EVENT_QICNT4L							= 0x1D,				// Counter4 < Comparater4 state.
	N_EVENT_QICNT4E							= 0x1E,				// Counter4 = Comparater4 state.
	N_EVENT_QICNT4G							= 0x1F,				// Counter4 > Comparater4 state.
	N_EVENT_QICNT4LE						= 0x20,				// Counter4 ≤ Comparater4 state.
	N_EVENT_QICNT4GE						= 0x21,				// Counter4 ≥ Comparater4 state.
	N_EVENT_QICNT4EUP						= 0x22,				// Counter4 = Comparater4 event during counting up.
	N_EVENT_QICNT4EDN						= 0x23,				// Counter4 = Comparater4 event during counting down.
	N_EVENT_QICNT4BND						= 0x24,				// Counter4 is same with boundary value.
	N_EVENT_QICNT5L							= 0x25,				// Counter5 < Comparater5 state.
	N_EVENT_QICNT5E							= 0x26,				// Counter5 = Comparater5 state.
	N_EVENT_QICNT5G							= 0x27,				// Counter5 > Comparater5 state.
	N_EVENT_QICNT5LE						= 0x28,				// Counter5 ≤ Comparater5 state.
	N_EVENT_QICNT5GE						= 0x29,				// Counter5 ≥ Comparater5 state.
	N_EVENT_QICNT5EUP						= 0x2A,				// Counter5 = Comparater5 event during counting up.
	N_EVENT_QICNT5EDN						= 0x2B,				// Counter5 = Comparater5 event during counting down.
	N_EVENT_QICNT5BND						= 0x2C,				// Counter5 is same with boundary value.
	N_EVENT_QIDEVL							= 0x2D,				// DEVIATION value < Comparater4 state.
	N_EVENT_QIDEVE							= 0x2E,				// DEVIATION value = Comparater4 state.
	N_EVENT_QIDEVG							= 0x2F,				// DEVIATION value > Comparater4 state.
	N_EVENT_QIDEVLE							= 0x30,				// DEVIATION value ≤ Comparater4 state.
	N_EVENT_QIDEVGE							= 0x31,				// DEVIATION value ≥ Comparater4 state.
	N_EVENT_QIPELM							= 0x32,				// PELM input signal is activated state.
	N_EVENT_QINELM							= 0x33,				// NELM input signal is activated state.
	N_EVENT_QIPSLM							= 0x34,				// PSLM input signal is activated state.
	N_EVENT_QINSLM							= 0x35,				// NSLM input signal is activated state.
	N_EVENT_QIALARM							= 0x36,				// ALAMR input signal is activated state.
	N_EVENT_QIINPOS							= 0x37,				// INPOSITION input signal ia activated state.
	N_EVENT_QIESTOP							= 0x38,				// ESTOP input signal is activated state.
	N_EVENT_QIORG							= 0x39,				// ORG input signal is activated state.
	N_EVENT_QIZ_PHASE						= 0x3A,				// Z_PHASE input signal is activated state.
	N_EVENT_QIECUP							= 0x3B,				// ECUP input signal is high level state.
	N_EVENT_QIECDN							= 0x3C,				// ECDN input signal is high level state.
	N_EVENT_QIEXPP							= 0x3D,				// EXPP input signal is high level state.
	N_EVENT_QIEXMP							= 0x3E,				// EXMP input signal is high level state.
	N_EVENT_QISQSTR1						= 0x3F,				// SYNC Start1 input signal is activated state(activated).
	N_EVENT_QISQSTR2						= 0x40,				// SYNC Start2 input signal is activated state(activated).
	N_EVENT_QISQSTP1						= 0x41,				// SYNC STOP1 input signal is activated state(activated).
	N_EVENT_QISQSTP2						= 0x42,				// SYNC STOP2 input signal is activated state(activated).
	N_EVENT_QIALARMS						= 0x43,				// At least one alarm signal of each axis is activated state.
	N_EVENT_QIUIO0							= 0x44,				// UIO0 data is high state.
	N_EVENT_QIUIO1							= 0x45,				// UIO1 data is high state.
	N_EVENT_QIUIO2							= 0x46,				// UIO2 data is high state.
	N_EVENT_QIUIO3							= 0x47,				// UIO3 data is high state.
	N_EVENT_QIUIO4							= 0x48,				// UIO4 data is high state.
	N_EVENT_QIUIO5							= 0x49,				// UIO5 data is high state.
	N_EVENT_QIUIO6							= 0x4A,				// UIO6 data is high state.
	N_EVENT_QIUIO7							= 0x4B,				// UIO7 data is high state.
	N_EVENT_QIUIO8							= 0x4C,				// UIO8 data is high state.
	N_EVENT_QIUIO9							= 0x4D,				// UIO9 data is high state.
	N_EVENT_QIUIO10							= 0x4E,				// UIO10 data is high state.
	N_EVENT_QIUIO11							= 0x4F,				// UIO11 data is high state.
	N_EVENT_QIERC							= 0x50,				// ERC output is activated.
	N_EVENT_QITRG							= 0x51,				// TRIGGER signal is activated.
	N_EVENT_QIPREQI0						= 0x52,				// Previous queue data index 0 bit is high state.
	N_EVENT_QIPREQI1						= 0x53,				// Previous queue data index 1 bit is high state.
	N_EVENT_QIPREQI2						= 0x54,				// Previous queue data index 2 bit is high state.
	N_EVENT_QIPREQZ							= 0x55,				// Previous queue is empty state.
	N_EVENT_QIPREQF							= 0x56,				// Previous queue is full state.
	N_EVENT_QIMPGE1							= 0x57,				// MPG first stage is overflowed state.
	N_EVENT_QIMPGE2							= 0x58,				// MPG second stage is overflowed state.
	N_EVENT_QIMPGE3							= 0x59,				// MPG third stage is overflowed state.
	N_EVENT_QIMPGERR						= 0x5A,				// MPG all state is overflowed state.
	N_EVENT_QITRGCNT0						= 0x5B,				// TRIGGER queue index bit 0 is high state.
	N_EVENT_QITRGCNT1 						= 0x5C,				// TRIGGER queue index bit 1 is high state.
	N_EVENT_QITRGCNT2 						= 0x5D,				// TRIGGER queue index bit 2 is high state.
	N_EVENT_QITRGCNT3 						= 0x5E,				// TRIGGER queue index bit 3 is high state.
	N_EVENT_QITRGQEPT 						= 0x5F,				// TRIGGER queue is empty state.
	N_EVENT_QITRGQFULL 						= 0x60,				// TRIGGER queue is full state.
	N_EVENT_QIDPAUSE 						= 0x61,				// Drive paused state.
	N_EVENT_QIESTOPEXE 						= 0x62,				// Emergency stop occurred
	N_EVENT_QISSTOPEXE 						= 0x63,				// Slowdown stop occurred
	N_EVENT_QIPLMTSTOP 						= 0x64,				// Limit stop event occurred during positive driving.
	N_EVENT_QINLMTSTOP 						= 0x65,				// Limit stop event occurred during negative driving.
	N_EVENT_QIOPLMTSTOP 					= 0x66,				// Optional limit stop event occurred during positive driving.
	N_EVENT_QIONLMTSTOP 					= 0x67,				// Optional limit stop event occurred during negative driving.
	N_EVENT_QIPSWESTOP 						= 0x68,				// Software emergency limit stop event occurred.(CW)
	N_EVENT_QINSWESTOP 						= 0x69,				// Software emergency limit stop event occurred.(CCW)
	N_EVENT_QIPSWSSTOP 						= 0x6A,				// Software slowdown limit stop event occurred.(CW)
	N_EVENT_QINSWSSTOP 						= 0x6B,				// Software slowdown limit stop event occurred.(CCW)
	N_EVENT_QIALMSTOP 						= 0x6C,				// Emergency stop event occurred by alarm signal function.
	N_EVENT_QIESTOPSTOP 					= 0x6D,				// Emergency stop event occurred by estop signal function.
	N_EVENT_QIESTOPCMD 						= 0x6E,				// Emergency stop event occurred by command.
	N_EVENT_QISSTOPCMD 						= 0x6F,				// Slowdown stop event occurred by command.
	N_EVENT_QIALLSTCMD 						= 0x70,				// Emergency stop event occurred by all stop command.
	N_EVENT_QISYSTOP1 						= 0x71,				// SYNC stop1 event occurred.
	N_EVENT_QISYSTOP2 						= 0x72,				// SYNC stop2 event occurred.
	N_EVENT_QIENCODERR 						= 0x73,				// Encoder input error event occurred.
	N_EVENT_QIMPGOVERFLOW					= 0x74,				// MPG input error event occurred.
	N_EVENT_QIORGOK 						= 0x75,				// Original drive is executed successfully.
	N_EVENT_QISSCHOK 						= 0x76,				// Signal search drive is executed successfully.
	N_EVENT_QIUIO0LOW						= 0x77,				// UIO0 data is low state.
	N_EVENT_QIUIO1LOW						= 0x78,				// UIO1 data is low state.
	N_EVENT_QIUIO2LOW						= 0x79,				// UIO2 data is low state.
	N_EVENT_QIUIO3LOW						= 0x7A,				// UIO3 data is low state.
	N_EVENT_QIUIO4LOW						= 0x7B,				// UIO4 data is low state.
	N_EVENT_QIUIO5LOW						= 0x7C,				// UIO5 data is low state.
	N_EVENT_QIUIO6LOW						= 0x7D,				// UIO6 data is low state.
	N_EVENT_QIUIO7LOW						= 0x7E,				// UIO7 data is low state.
	N_EVENT_QIUIO8LOW						= 0x7F,				// UIO8 data is low state.
	N_EVENT_QIUIO9LOW						= 0x80,				// UIO9 data is low state.
	N_EVENT_QIUIO10LOW						= 0x81,				// UIO10 data is low state.
	N_EVENT_QIUIO11LOW						= 0x82,				// UIO11 data is low state.
	N_EVENT_QIUIO0RISING 					= 0x83,				// UIO0 rising edge event occurred.
	N_EVENT_QIUIO1RISING					= 0x84,				// UIO1 rising edge event occurred.
	N_EVENT_QIUIO2RISING					= 0x85,				// UIO2 rising edge event occurred.
	N_EVENT_QIUIO3RISING					= 0x86,				// UIO3 rising edge event occurred.
	N_EVENT_QIUIO4RISING					= 0x87,				// UIO4 rising edge event occurred.
	N_EVENT_QIUIO5RISING					= 0x88,				// UIO5 rising edge event occurred.
	N_EVENT_QIUIO6RISING					= 0x89,				// UIO6 rising edge event occurred.
	N_EVENT_QIUIO7RISING					= 0x8A,				// UIO7 rising edge event occurred.
	N_EVENT_QIUIO8RISING					= 0x8B,				// UIO8 rising edge event occurred.
	N_EVENT_QIUIO9RISING					= 0x8C,				// UIO9 rising edge event occurred.
	N_EVENT_QIUIO10RISING					= 0x8D,				// UIO10 rising edge event occurred.
	N_EVENT_QIUIO11RISING					= 0x8E,				// UIO11 rising edge event occurred.
	N_EVENT_QIUIO0FALLING					= 0x8F,				// UIO0 falling edge event occurred.
	N_EVENT_QIUIO1FALLING 					= 0x90,				// UIO1 falling edge event occurred.
	N_EVENT_QIUIO2FALLING 					= 0x91,				// UIO2 falling edge event occurred.
	N_EVENT_QIUIO3FALLING 					= 0x92,				// UIO3 falling edge event occurred.
	N_EVENT_QIUIO4FALLING 					= 0x93,				// UIO4 falling edge event occurred.
	N_EVENT_QIUIO5FALLING 					= 0x94,				// UIO5 falling edge event occurred.
	N_EVENT_QIUIO6FALLING 					= 0x95,				// UIO6 falling edge event occurred.
	N_EVENT_QIUIO7FALLING 					= 0x96,				// UIO7 falling edge event occurred.
	N_EVENT_QIUIO8FALLING 					= 0x97,				// UIO8 falling edge event occurred.
	N_EVENT_QIUIO9FALLING					= 0x98,				// UIO9 falling edge event occurred.
	N_EVENT_QIUIO10FALLING					= 0x99,				// UIO10 falling edge event occurred.
	N_EVENT_QIUIO11FALLING					= 0x9A,				// UIO11 falling edge event occurred.
	N_EVENT_QIDRVSTR 						= 0x9B,				// Drive started.
	N_EVENT_QIDNSTR 						= 0x9C,				// Speed down event occurred.
	N_EVENT_QICOSTR 						= 0x9D,				// Constant speed event occurred.
	N_EVENT_QIUPSTR 						= 0x9E,				// Speed up event occurred.
	N_EVENT_QICONTISTR 						= 0x9F,				// Continuous drive started.
	N_EVENT_QIPRESETSTR 					= 0xA0,				// Preset drive started.
	N_EVENT_QIMPGSTR 						= 0xA1,				// MPG drive started.
	N_EVENT_QIORGSTR 						= 0Xa2,				// Original drive started.
	N_EVENT_QISSCHSTR 						= 0xA3,				// Signal search drive started.
	N_EVENT_QIPATHSTR 						= 0xA4,				// Interpolation drive started.
	N_EVENT_QISLAVESTR 						= 0xA5,				// Slave drive started.
	N_EVENT_QICCWSTR 						= 0xA6,				// CCW direction drive started.
	N_EVENT_QIINPWAIT 						= 0xA7,				// Inposition wait event occurred.
	N_EVENT_QILINSTR 						= 0xA8,				// Linear drive stated.
	N_EVENT_QICIRSTR 						= 0xA9,				// Circular drive started.
	N_EVENT_QIDRVENDII 						= 0xAA,				// Drive stopped.(Inposition state included)
	N_EVENT_QIDNEND 						= 0xAB,				// Speed down end event occurred.
	N_EVENT_QICOEND 						= 0xAC,				// Constant speed end event occurred.
	N_EVENT_QIUPEND 						= 0xAD,				// Speed up end event occurred.
	N_EVENT_QICONTIEND 						= 0xAE,				// Continuous drive ended.
	N_EVENT_QIPRESETEND 					= 0xAF,				// Preset drive ended.
	N_EVENT_QIMPGEND 						= 0xB0,				// MPG drive ended.
	N_EVENT_QIORGEND 						= 0xB1,				// Original drive ended.
	N_EVENT_QISSCHEND 						= 0XB2,				// Signal search drive ended.
	N_EVENT_QIPATHEND 						= 0xB3,				// Interpolation drive ended.
	N_EVENT_QISLAVEEND 						= 0xB4,				// Slave drive ended.
	N_EVENT_QICCWEND 						= 0xB5,				// CCW direction drive ended.
	N_EVENT_QIINPEND 						= 0xB6,				// Escape from Inposition waiting.
	N_EVENT_QILINEND 						= 0xB7,				// Linear drive ended.
	N_EVENT_QICIREND 						= 0xB8,				// Circular drive ended.
	N_EVENT_QIBUSY 							= 0xB9,				// During driving state.
	N_EVENT_QINBUSY 						= 0xBA,				// During not driving state.
	N_EVENT_QITMR1EX 						= 0xBB,				// Timer1 expired event.
	N_EVENT_QITMR2EX 						= 0xBC,				// Timer2 expired event.
	N_EVENT_QIDRVENDIII 					= 0xBD,				// Drive(that interrupt enable bit is set to high) end N_EVENT 
	N_EVENT_QIERROR 						= 0xBE,				// Error stop occurred.
//	N_EVENT_QINOP 							= 0xBF,				// NOP.
	N_EVENT_QIALWAYS 						= 0xFF				// Always Generate.

} N_QIEVENT;

//========================================================================================================================;
#endif	//__AXT_AXHD_H__
