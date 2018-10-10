/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** AXHS.H
**
** COPYRIGHT (c) AJINEXTEK Co., LTD
**
*****************************************************************************
*****************************************************************************
**
** Description
** -----------
** Resource Define Header File
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
#ifndef __ANT_ANHS_H__
#define __ANT_ANHS_H__

#define MAX_NODE_COUNT        250
#define MAX_AXIS_PER_NODE     20

#ifndef __DATA_TYPES__
    #define __DATA_TYPES__
    typedef unsigned long int                    DWORD;
    typedef unsigned short int                   WORD;
    typedef unsigned char                        BYTE;
    typedef int                                  BOOL;
    typedef void                                *HANDLE;
#endif

#pragma pack(1)
typedef struct tagNetInfo{
    unsigned char ucNodeCount;
    unsigned char ulNodeID[MAX_NODE_COUNT];        // Node device ID(set by ID switch)
}ANT_EipNetInfo;
#pragma pack()

// 베이스노드 정의
#ifndef __ANT_BASE_NODE__
    #define __ANT_BASE_NODE__
    typedef enum _ANT_BASE_NODE
    {
        ANT_UNKNOWN   = 0x00,            // Unknown Baseboard
        ANT_EIP       = 0x20,            // EtherNet/IP interface Node
        ANT_ECAT      = 0x21,            // EtherCat interface Node
        ANT_CAN       = 0x22,            // CAN interface Node
        ANT_DNET      = 0x23,            // DEVICENET interface Node
        ANT_PBUS      = 0x24             // ProfiBus interface Node
    } ANT_BASE_NODE;
#endif

// 모듈 정의
#ifndef __ANT_FUNC_MODULE__
    #define __ANT_FUNC_MODULE__
    typedef enum _ANT_FUNC_MODULE
    {
        ANT_NMC_2V04    = 0xB1,        // CAMC-QI, 2 Axis
        ANT_NMC_4V04    = 0xB2,        // CAMC-QI, 4 Axis
        ANT_NIO_DC16    = 0xB3,        // Digital 16 점 Configurable
        ANT_NIO_AI4     = 0xB4,        // AI 4Ch, 16 bit
        ANT_NIO_DI32    = 0xB5,        // Digital IN  32점    (reserved)
        ANT_NIO_DO32    = 0xB6,        // Digital OUT 32점    (reserved)
        ANT_NIO_DB32    = 0xB7,        // Digital IN  16점 / OUT 16점    (reserved)
        ANT_NIO_AO4     = 0xB8,        // AO 4Ch, 16 bit    (reserved)
        ANT_FN_EMPTY    = 0xFE,        // Empty module area.
        ANT_FN_UNKNOWN  = 0xFF         // Unkown module.
    } ANT_FUNC_MODULE;
#endif

#ifndef __ANT_FUNC_RESULT__
    #define __ANT_FUNC_RESULT__
    typedef enum _ANT_FUNC_RESULT
    {
        ANT_RT_SUCCESS                                = 0000,
        ANT_RT_OPEN_ERROR                             = 1001,
        ANT_RT_OPEN_ALREADY                           = 1002,
        ANT_RT_NOT_OPEN                               = 1053,
        ANT_RT_NOT_SUPPORT_VERSION                    = 1054,
        ANT_RT_INVALID_NODE_NO                        = 1101,
        ANT_RT_INVALID_MODULE_POS                     = 1102,
        ANT_RT_INVALID_LEVEL                          = 1103,
		ANT_RT_FLASH_BUSY                             = 1150,
        ANT_RT_ERROR_VERSION_READ                     = 1151,
        ANT_RT_ERROR_NETWORK                          = 1152,
        ANT_RT_ERROR_HW_ACCESS                        = 1153,
        ANT_RT_ERROR_NETWORK_CHEKSUM                  = 1154,

        ANT_RT_1ST_BELOW_MIN_VALUE                    = 1160,
        ANT_RT_1ST_ABOVE_MAN_VALUE                    = 1161,
        ANT_RT_2ND_BELOW_MIN_VALUE                    = 1170,
        ANT_RT_2ND_ABOVE_MAN_VALUE                    = 1171,
        ANT_RT_3RD_BELOW_MIN_VALUE                    = 1180,
        ANT_RT_3RD_ABOVE_MAN_VALUE                    = 1181,
        ANT_RT_4TH_BELOW_MIN_VALUE                    = 1190,
        ANT_RT_4TH_ABOVE_MAN_VALUE                    = 1191,
        ANT_RT_5TH_BELOW_MIN_VALUE                    = 1200,
        ANT_RT_5TH_ABOVE_MAN_VALUE                    = 1201,
        ANT_RT_6TH_BELOW_MIN_VALUE                    = 1210,
        ANT_RT_6TH_ABOVE_MAN_VALUE                    = 1211,
        ANT_RT_7TH_BELOW_MIN_VALUE                    = 1220,
        ANT_RT_7TH_ABOVE_MAN_VALUE                    = 1221,
        ANT_RT_8TH_BELOW_MIN_VALUE                    = 1230,
        ANT_RT_8TH_ABOVE_MAN_VALUE                    = 1231,
        ANT_RT_9TH_BELOW_MIN_VALUE                    = 1240,
        ANT_RT_9TH_ABOVE_MAN_VALUE                    = 1241,
        ANT_RT_10TH_BELOW_MIN_VALUE                   = 1250,
        ANT_RT_10TH_ABOVE_MAN_VALUE                   = 1251,

        ANT_RT_AIO_OPEN_ERROR                         = 2001,
        ANT_RT_AIO_NOT_MODULE                         = 2051,
        ANT_RT_AIO_NOT_EVENT                          = 2052,
        ANT_RT_AIO_INVALID_MODULE_NO                  = 2101,
        ANT_RT_AIO_INVALID_CHANNEL_NO                 = 2102,
        ANT_RT_AIO_INVALID_USE                        = 2106,
        ANT_RT_AIO_INVALID_TRIGGER_MODE               = 2107,

        ANT_RT_DIO_OPEN_ERROR                         = 3001,
        ANT_RT_DIO_NOT_MODULE                         = 3051,
        ANT_RT_DIO_NOT_INTERRUPT                      = 3052,
        ANT_RT_DIO_INVALID_MODULE_NO                  = 3101,
        ANT_RT_DIO_INVALID_OFFSET_NO                  = 3102,
        ANT_RT_DIO_INVALID_LEVEL                      = 3103,
        ANT_RT_DIO_INVALID_MODE                       = 3104,
        ANT_RT_DIO_INVALID_VALUE                      = 3105,
        ANT_RT_DIO_INVALID_USE                        = 3106,

        ANT_RT_MOTION_OPEN_ERROR                      = 4001,
        ANT_RT_MOTION_NOT_MODULE                      = 4051,
        ANT_RT_MOTION_NOT_INTERRUPT                   = 4052,
        ANT_RT_MOTION_NOT_INITIAL_ANIS_NO             = 4053,
        ANT_RT_MOTION_NOT_IN_CONT_INTERPOL            = 4054,
        ANT_RT_MOTION_NOT_PARA_READ                   = 4055,
        ANT_RT_MOTION_INVALID_ANIS_NO                 = 4101,
        ANT_RT_MOTION_INVALID_METHOD                  = 4102,
        ANT_RT_MOTION_INVALID_USE                     = 4103,
        ANT_RT_MOTION_INVALID_LEVEL                   = 4104,
        ANT_RT_MOTION_INVALID_BIT_NO                  = 4105,
        ANT_RT_MOTION_INVALID_STOP_MODE               = 4106,
        ANT_RT_MOTION_INVALID_TRIGGER_MODE            = 4107,
        ANT_RT_MOTION_INVALID_TRIGGER_LEVEL           = 4108,
        ANT_RT_MOTION_INVALID_SELECTION               = 4109,
        ANT_RT_MOTION_INVALID_TIME                    = 4110,
        ANT_RT_MOTION_INVALID_FILE_LOAD               = 4111,
        ANT_RT_MOTION_INVALID_FILE_SAVE               = 4112,
        ANT_RT_MOTION_INVALID_VELOCITY                = 4113,
        ANT_RT_MOTION_INVALID_ACCELTIME               = 4114,
        ANT_RT_MOTION_INVALID_PULSE_VALUE             = 4115,
        ANT_RT_MOTION_INVALID_NODE_NUMBER             = 4116,
        ANT_RT_MOTION_INVALID_TARGET                  = 4117,
        ANT_RT_MOTION_ERROR_IN_NONMOTION              = 4151,
        ANT_RT_MOTION_ERROR_IN_MOTION                 = 4152,
        ANT_RT_MOTION_ERROR                           = 4153,
       
        ANT_RT_MOTION_ERROR_GANTRY_ENABLE             = 4154,
        ANT_RT_MOTION_ERROR_GANTRY_ANIS               = 4155,
        ANT_RT_MOTION_ERROR_MASTER_SERVOON            = 4156,
        ANT_RT_MOTION_ERROR_SLAVE_SERVOON             = 4157,
        ANT_RT_MOTION_INVALID_POSITION                = 4158,

        ANT_RT_ERROR_NOT_SAME_MODULE                  = 4159,
        ANT_RT_ERROR_NOT_SAME_PRODUCT                 = 4161,
        ANT_RT_NOT_CAPTURED                           = 4162,
        ANT_RT_ERROR_NOT_SAME_IC                      = 4163,
        ANT_RT_ERROR_NOT_GEARMODE                     = 4164,
        ANT_ERROR_CONTI_INVALID_ANIS_NO               = 4165,
        ANT_ERROR_CONTI_INVALID_MAP_NO                = 4166,
        ANT_ERROR_CONTI_EMPTY_MAP_NO                  = 4167,
        ANT_RT_MOTION_ERROR_CACULATION                = 4168,
        ANT_RT_ERROR_NOT_SAME_NODE                    = 4169,

        ANT_ERROR_HELICAL_INVALID_ANIS_NO             = 4170,
        ANT_ERROR_HELICAL_INVALID_MAP_NO              = 4171,
        ANT_ERROR_HELICAL_EMPTY_MAP_NO                = 4172,

        ANT_ERROR_SPLINE_INVALID_ANIS_NO              = 4180,
        ANT_ERROR_SPLINE_INVALID_MAP_NO               = 4181,
        ANT_ERROR_SPLINE_EMPTY_MAP_NO                 = 4182,
        ANT_ERROR_SPLINE_NUM_ERROR                    = 4183,
        ANT_RT_MOTION_INTERPOL_VALUE                  = 4184,
        ANT_RT_ERROR_NOT_CONTIBEGIN                   = 4185,
        ANT_RT_ERROR_NOT_CONTIEND                     = 4186,

        ANT_RT_MOTION_HOME_SEARCHING                  = 4201,
        ANT_RT_MOTION_HOME_ERROR_SEARCHING            = 4202,
        ANT_RT_MOTION_HOME_ERROR_START                = 4203,
        ANT_RT_MOTION_HOME_ERROR_GANTRY               = 4204,
        ANT_RT_MOTION_POSITION_OUTOFBOUND             = 4251,
        ANT_RT_MOTION_PROFILE_INVALID                 = 4252,
        ANT_RT_MOTION_VELOCITY_OUTOFBOUND             = 4253,
        ANT_RT_MOTION_MOVE_UNIT_IS_ZERO               = 4254,
        ANT_RT_MOTION_SETTING_ERROR                   = 4255,
        ANT_RT_MOTION_IN_CONT_INTERPOL                = 4256,
        ANT_RT_MOTION_DISABLE_TRIGGER                 = 4257,
        ANT_RT_MOTION_INVALID_CONT_INDEX              = 4258,
        ANT_RT_MOTION_CONT_QUEUE_FULL                 = 4259,

        ANT_RT_INIT_DOSE_NOT_EXIST_LAN_CARD           = 4301,
        ANT_RT_INIT_DOES_NOT_RESPONSE_SLAVE           = 4302,
        ANT_RT_INIT_INVALID_HOST_IP_ADDRESS           = 4303,
        ANT_RT_INIT_INVALID_HOST_COUNT                = 4304,
        ANT_RT_INIT_EIPSTART_FAIL                     = 4305,
        ANT_RT_INIT_ALREADY_INITIALIZED               = 4306,
        ANT_RT_INIT_INVALID_NET_TYPE                  = 4307,
        ANT_RT_INIT_PRODUCTID                         = 4308,
        ANT_RT_INIT_NOT_ENOUGH_MEMORY                 = 4309,
        ANT_RT_INIT_DOES_NOT_RESPONSE_SLAVE_AT_RING   = 4310,
        ANT_RT_INIT_NETWORK_ERROR                     = 4311,
        ANT_RT_INIT_BACKGROUND_NOT_STARTED            = 4312,
        ANT_RT_INIT_BACKGROUND_START_FAIL             = 4313
    } ANT_FUNC_RESULT;
#endif
/*
#ifndef PASCAL
    #define PASCAL                                __stdcall
#endif    // PASCAL

#ifndef EXPORT
    #define EXPORT
#endif    // EXPORT
*/
#ifndef __AFX_H__
    #define HWND                                HANDLE
#endif

#ifndef FALSE
#define FALSE          0
#define TRUE           1
#endif

#ifndef WM_USER
    #define WM_USER                                0x0400
#endif

#ifndef WM_ANL_INTERRUPT
    #define WM_ANL_INTERRUPT                    (WM_USER + 1001)
#endif

#ifndef WM_ANL_NETSTAT
    #define WM_ANL_NETSTAT                        (WM_USER + 1002)
#endif

#ifndef __ANT_LOG_LEVEL__
    #define __ANT_LOG_LEVEL__
    typedef enum _ANT_LOG_LEVEL
    {
        N_LEVEL_NONE,
        N_LEVEL_ERROR,
        N_LEVEL_RUNSTOP,
        N_LEVEL_FUNCTION
    } ANT_LOG_LEVEL;    // 로그 레벨
#endif

#ifndef __ANT_EXISTENCE__
    #define __ANT_EXISTENCE__
    typedef enum _ANT_EXISTENCE
    {
        N_STATUS_NOTEXIST,
        N_STATUS_EXIST
    } ANT_EXISTENCE;
#endif
 
#ifndef __ANT_USE__
    #define __ANT_USE__
    typedef enum _ANT_USE
    {
        N_DISABLE,
        N_ENABLE
    } ANT_USE;
#endif

#ifndef __ANT_AIO_TRIGGER_MODE__
    #define __ANT_AIO_TRIGGER_MODE__
    typedef enum _ANT_AIO_TRIGGER_MODE
    {
        N_DISABLE_MODE                   = 0,
        N_NORMAL_MODE                    = 1,
        N_TIMER_MODE, 
        N_EXTERNAL_MODE
    } ANT_AIO_TRIGGER_MODE;
#endif

#ifndef __ANT_AIO_FULL_MODE__
    #define __ANT_AIO_FULL_MODE__
    typedef enum _ANT_AIO_FULL_MODE
    {
        N_NEW_DATA_KEEP,
        N_CURR_DATA_KEEP
    } ANT_AIO_FULL_MODE;
#endif

#ifndef __ANT_AIO_EVENT_MASK__
    #define __ANT_AIO_EVENT_MASK__
    typedef enum _ANT_AIO_EVENT_MASK 
    {
        N_DATA_EMPTY                      = 0x01,
        N_DATA_MANY                       = 0x02,
        N_DATA_SMALL                      = 0x04,
        N_DATA_FULL                       = 0x08
    } ANT_AIO_EVENT_MASK;
#endif

#ifndef __ANT_AIO_INTERRUPT_MASK__
    #define __ANT_AIO_INTERRUPT_MASK__
    typedef enum _ANT_AIO_INTERRUPT_MASK 
    {
        N_ADC_DONE                         = 0x00,
        N_SCAN_END                         = 0x01,
        N_FIFO_HALF_FULL                   = 0x02,
        N_NO_SIGNAL                        = 0x03
    } ANT_AIO_INTERRUPT_MASK;
#endif
#ifndef __ANT_AIO_QUEUE_EVENT__
    #define __ANT_AIO_QUEUE_EVENT__       
    typedef enum  _AIO_EVENT_MODE  
    { 
        N_AIO_EVENT_DATA_RESET             = 0x00, 
        N_AIO_EVENT_DATA_UPPER, 
        N_AIO_EVENT_DATA_LOWER, 
        N_AIO_EVENT_DATA_FULL, 
        N_AIO_EVENT_DATA_EMPTY 
    } AIO_EVENT_MODE;
#endif
    
#ifndef __ANT_DIO_EDGE__
    #define __ANT_DIO_EDGE__
    typedef enum _ANT_DIO_EDGE
    {
        N_DOWN_EDGE,
        N_UP_EDGE
    } ANT_DIO_EDGE;
#endif

#ifndef __ANT_DIO_STATE__
    #define __ANT_DIO_STATE__
    typedef enum _ANT_DIO_STATE
    {
        N_OFF_STATE,
        N_ON_STATE
    } ANT_DIO_STATE;
#endif

#ifndef ANT_MOTION_STOPMODE_DEF
    #define ANT_MOTION_STOPMODE_DEF
    typedef enum _ANT_MOTION_STOPMODE
    {
        N_EMERGENCY_STOP,
        N_SLOWDOWN_STOP
    } ANT_MOTION_STOPMODE;
#endif

#ifndef ANT_MOTION_EDGE_DEF
    #define ANT_MOTION_EDGE_DEF
    typedef enum _ANT_MOTION_EDGE
    {
        N_SIGNAL_DOWN_EDGE,
        N_SIGNAL_UP_EDGE
    } ANT_MOTION_EDGE;
#endif

#ifndef ANT_MOTION_SELECTION_DEF
    #define ANT_MOTION_SELECTION_DEF
    typedef enum _ANT_MOTION_SELECTION
    {
        N_COMMAND,
        N_ACTUAL
    } ANT_MOTION_SELECTION;
#endif

#ifndef ANT_MOTION_TRIGGER_MODE_DEF
    #define ANT_MOTION_TRIGGER_MODE_DEF
    typedef enum _ANT_MOTION_TRIGGER_MODE
    {
        N_PERIOD_MODE,
        N_ABS_POS_MODE
    } ANT_MOTION_TRIGGER_MODE;
#endif

#ifndef ANT_MOTION_LEVEL_MODE_DEF
    #define ANT_MOTION_LEVEL_MODE_DEF
    typedef enum _ANT_MOTION_LEVEL_MODE
    {
        N_LOW,
        N_HIGH,
        N_UNUSED,
        N_USED
    } ANT_MOTION_LEVEL_MODE;
#endif

#ifndef ANT_MOTION_ABSREL_MODE_DEF
    #define ANT_MOTION_ABSREL_MODE_DEF
    typedef enum _ANT_MOTION_ABSREL_MODE
    {
        N_POS_ABS_MODE,
        N_POS_REL_MODE
    } ANT_MOTION_ABSREL_MODE;
#endif

#ifndef ANT_MOTION_PROFILE_MODE_DEF
    #define ANT_MOTION_PROFILE_MODE_DEF
    typedef enum _ANT_MOTION_PROFILE_MODE
    {
        N_SYM_TRAPEZOIDE_MODE,
        N_ASYM_TRAPEZOIDE_MODE,
        N_QUASI_S_CURVE_MODE,
        N_SYM_S_CURVE_MODE,
        N_ASYM_S_CURVE_MODE
    } ANT_MOTION_PROFILE_MODE;
#endif

#ifndef ANT_MOTION_SELECTION_DEF
    #define ANT_MOTION_SELECTION_DEF
    typedef enum _ANT_MOTION_SIGNAL_LEVEL
    {
        N_INACTIVE,
        N_ACTIVE
    } ANT_MOTION_SIGNAL_LEVEL;
#endif

#ifndef ANT_MOTION_HOME_RESULT_DEF
    #define ANT_MOTION_HOME_RESULT_DEF
    typedef enum ANT_MOTION_HOME_RESULT
    {
        N_HOME_SUCCESS                       = 0x01,
        N_HOME_SEARCHING                     = 0x02,
        N_HOME_ERR_GNT_RANGE                 = 0x10,
        N_HOME_ERR_USER_BREAK                = 0x11,
        N_HOME_ERR_VELOCITY                  = 0x12,    
        N_HOME_ERR_AMP_FAULT                 = 0x13,        // 서보팩 알람 발생 에러
        N_HOME_ERR_NEG_LIMIT                 = 0x14,        // (-)방향 구동중 (+)리미트 센서 감지 에러
        N_HOME_ERR_POS_LIMIT                 = 0x15,        // (+)방향 구동중 (-)리미트 센서 감지 에러
        N_HOME_ERR_NOT_DETECT                = 0x16,        // 지정한 신호 검출하지 못 할 경우 에러
        N_HOME_ERR_UNKNOWN                   = 0xFF,
    } ANT_MOTION_HOME_RESULT;
#endif

#ifndef ANT_MOTION_UNIV_INPUT_DEF
    #define ANT_MOTION_UNIV_INPUT_DEF
    typedef enum _ANT_MOTION_UNIV_INPUT
    {
        N_UIO_INP0,
        N_UIO_INP1,
        N_UIO_INP2,
        N_UIO_INP3,
        N_UIO_INP4
    } ANT_MOTION_UNIV_INPUT;
#endif

#ifndef ANT_MOTION_UNIV_OUTPUT_DEF
    #define ANT_MOTION_UNIV_OUTPUT_DEF
    typedef enum _ANT_MOTION_UNIV_OUTPUT
    {
        N_UIO_OUT0,
        N_UIO_OUT1,
        N_UIO_OUT2,
        N_UIO_OUT3,
        N_UIO_OUT4
    } ANT_MOTION_UNIV_OUTPUT;
#endif


#ifndef ANT_MOTION_DETECT_DOWN_START_POINT_DEF
    #define ANT_MOTION_DETECT_DOWN_START_POINT_DEF
    typedef enum _ANT_MOTION_DETECT_DOWN_START_POINT
    {
        N_AutoDetect,
        N_RestPulse
    } ANT_MOTION_DETECT_DOWN_START_POINT;
#endif

#ifndef ANT_MOTION_PULSE_OUTPUT_DEF
    #define ANT_MOTION_PULSE_OUTPUT_DEF
    /* Pulse Output Method                    */
    typedef enum _ANT_MOTION__PULSE_OUTPUT 
    {
        N_OneHighLowHigh                       = 0x0,             // 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
        N_OneHighHighLow                       = 0x1,             // 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
        N_OneLowLowHigh                        = 0x2,             // 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
        N_OneLowHighLow                        = 0x3,             // 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
        N_TwoCcwCwHigh                         = 0x4,             // 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High     
        N_TwoCcwCwLow                          = 0x5,             // 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low     
        N_TwoCwCcwHigh                         = 0x6,             // 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
        N_TwoCwCcwLow                          = 0x7,             // 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
        N_TwoPhase                             = 0x8,             // 2상(90' 위상차),  PULSE lead DIR(CW: 정방향), PULSE lag DIR(CCW:역방향)
        N_TwoPhaseReverse                      = 0x9              // 2상(90' 위상차),  PULSE lead DIR(CCW: 정방향), PULSE lag DIR(CW:역방향)
    } ANT_MOTION_PULSE_OUTPUT;
#endif

#ifndef ANT_MOTION_EXTERNAL_COUNTER_INPUT_DEF
    #define ANT_MOTION_EXTERNAL_COUNTER_INPUT_DEF
    /* Mode2 Data   External Counter Input */
    typedef enum _ANT_MOTION__EXTERNAL_COUNTER_INPUT
    {
        N_ObverseUpDownMode                     = 0x0,            // 정방향 Up/Down
        N_ObverseSqr1Mode                       = 0x1,            // 정방향 1체배
        N_ObverseSqr2Mode                       = 0x2,            // 정방향 2체배
        N_ObverseSqr4Mode                       = 0x3,            // 정방향 4체배
        N_ReverseUpDownMode                     = 0x4,            // 역방향 Up/Down
        N_ReverseSqr1Mode                       = 0x5,            // 역방향 1체배
        N_ReverseSqr2Mode                       = 0x6,            // 역방향 2체배
        N_ReverseSqr4Mode                       = 0x7             // 역방향 4체배
    } ANT_MOTION_EXTERNAL_COUNTER_INPUT;
#endif

#ifndef ANT_MOTION_ACC_UNIT_DEF
    #define ANT_MOTION_ACC_UNIT_DEF
    typedef enum _ANT_MOTION_ACC_UNIT
    {
        N_UNIT_SEC2                      = 0x0,           // unit/sec2
        N_SEC                            = 0x1            // sec
    } ANT_MOTION_ACC_UNIT;
#endif

#ifndef ANT_MOTION_MOVE_DIR_DEF
    #define ANT_MOTION_MOVE_DIR_DEF
    typedef enum _ANT_MOTION_MOVE_DIR
    {
        N_DIR_CCW                        = 0x0,            // 반시계방향
        N_DIR_CW                         = 0x1             // 시계방향
    } ANT_MOTION_MOVE_DIR;
#endif

#ifndef ANT_MOTION_RADIUS_DISTANCE_DEF
    #define ANT_MOTION_RADIUS_DISTANCE_DEF
    typedef enum _ANT_MOTION_RADIUS_DISTANCE
    {
        N_SHORT_DISTANCE                 = 0x0,            // 짧은 거리의 원호 이동 
        N_LONG_DISTANCE                  = 0x1             // 긴 거리의 원호 이동 
    } ANT_MOTION_RADIUS_DISTANCE;
#endif

#ifndef ANT_MOTION_INTERPOLATION_ANIS_DEF
    #define ANT_MOTION_INTERPOLATION_ANIS_DEF
    typedef enum _ANT_MOTION_INTERPOLATION_ANIS
    {
        N_INTERPOLATION_ANIS2            = 0x0,            // 2축을 보간으로 사용할 때
        N_INTERPOLATION_ANIS3            = 0x1,            // 3축을 보간으로 사용할 때
        N_INTERPOLATION_ANIS4            = 0x2             // 4축을 보간으로 사용할 때
    } ANT_MOTION_INTERPOLATION_ANIS;
#endif

#ifndef ANT_MOTION_CONTISTART_NODE_DEF
    #define ANT_MOTION_CONTISTART_NODE_DEF
    typedef enum _ANT_MOTION_CONTISTART_NODE
    {
        N_CONTI_NODE_VELOCITY             = 0x0,            // 속도 지정 보간 모드
        N_CONTI_NODE_MANUAL               = 0x1,            // 노드 가감속 보간 모드
        N_CONTI_NODE_AUTO                 = 0x2             // 자동 가감속 보간 모드
    } ANT_MOTION_CONTISTART_NODE;
#endif

#ifndef ANT_MOTION_HOME_DETECT_SIGNAL_DEF
    #define ANT_MOTION_HOME_DETECT_SIGNAL_DEF
    typedef enum _ANT_MOTION_HOME_DETECT_SIGNAL
    {
        N_PosEndLimit                          = 0x0,            // +Elm(End limit) +방향 리미트 센서 신호
        N_NegEndLimit                          = 0x1,            // -Elm(End limit) -방향 리미트 센서 신호
        N_PosSloLimit                          = 0x2,            // +Slm(Slow Down limit) 신호 - 사용하지 않음
        N_NegSloLimit                          = 0x3,            // -Slm(Slow Down limit) 신호 - 사용하지 않음
        N_HomeSensor                           = 0x4,            // IN0(ORG)  원점 센서 신호
        N_EncodZPhase                          = 0x5,            // IN1(Z상)  Encoder Z상 신호
        N_UniInput02                           = 0x6,            // IN2(범용) 범용 입력 2번 신호
        N_UniInput03                           = 0x7,            // IN3(범용) 범용 입력 3번 신호
    } ANT_MOTION_HOME_DETECT_SIGNAL;
#endif


#ifndef ANT_MOTION_MPG_INPUT_METHOD_DEF
    #define ANT_MOTION_MPG_INPUT_METHOD_DEF
    typedef enum _ANT_MOTION_MPG_INPUT_METHOD
    {
        N_MPG_DIFF_ONE_PHASE                   = 0x0,            // MPG 입력 방식 One Phase
        N_MPG_DIFF_TWO_PHASE_1X                = 0x1,            // MPG 입력 방식 TwoPhase1
        N_MPG_DIFF_TWO_PHASE_2X                = 0x2,            // MPG 입력 방식 TwoPhase2
        N_MPG_DIFF_TWO_PHASE_4X                = 0x3,            // MPG 입력 방식 TwoPhase4
        N_MPG_LEVEL_ONE_PHASE                  = 0x4,            // MPG 입력 방식 Level One Phase
        N_MPG_LEVEL_TWO_PHASE_1X               = 0x5,            // MPG 입력 방식 Level Two Phase1
        N_MPG_LEVEL_TWO_PHASE_2X               = 0x6,            // MPG 입력 방식 Level Two Phase2
        N_MPG_LEVEL_TWO_PHASE_4X               = 0x7,            // MPG 입력 방식 Level Two Phase4
    } ANT_MOTION_MPG_INPUT_METHOD;
#endif

#ifndef ANT_MOTION_SENSOR_INPUT_METHOD_DEF
    #define ANT_MOTION_SENSOR_INPUT_METHOD_DEF
    typedef enum _ANT_MOTION_SENSOR_INPUT_METHOD
    {
        N_SENSOR_METHOD1                = 0x0,             // 일반 구동
        N_SENSOR_METHOD2                = 0x1,             // 센서 신호 검출 전은 저속 구동. 신호 검출 후 일반 구동
        N_SENSOR_METHOD3                = 0x2              // 저속 구동
    } ANT_MOTION_SENSOR_INPUT_METHOD;
#endif

#ifndef ANT_MOTION_HOME_CRC_SELECT_DEF
    #define ANT_MOTION_HOME_CRC_SELECT_DEF
    typedef enum _ANT_MOTION_HOME_CRC_SELECT
    {
        N_CRC_SELECT1                  = 0x0,              // 위치클리어 사용않함, 잔여펄스 클리어 사용 안함
        N_CRC_SELECT2                  = 0x1,              // 위치클리어 사용함, 잔여펄스 클리어 사용 안함
        N_CRC_SELECT3                  = 0x2,              // 위치클리어 사용안함, 잔여펄스 클리어 사용함
        N_CRC_SELECT4                  = 0x3               // 위치클리어 사용함, 잔여펄스 클리어 사용함
    } ANT_MOTION_HOME_CRC_SELECT;
#endif

#ifndef ANT_MOTION_IPDETECT_DESTINATION_SIGNAL_DEF
    #define ANT_MOTION_IPDETECT_DESTINATION_SIGNAL_DEF
    /* Detect Destination Signal            */
    typedef enum _ANT_MOTION_IPDETECT_DESTINATION_SIGNAL
    {
        N_PElmNegativeEdge                     = 0x0,            // +Elm(End limit) 하강 edge
        N_NElmNegativeEdge                     = 0x1,            // -Elm(End limit) 하강 edge
        N_PSlmNegativeEdge                     = 0x2,            // +Slm(Slowdown limit) 하강 edge
        N_NSlmNegativeEdge                     = 0x3,            // -Slm(Slowdown limit) 하강 edge
        N_In0DownEdge                          = 0x4,            // IN0(ORG) 하강 edge
        N_In1DownEdge                          = 0x5,            // IN1(Z상) 하강 edge
        N_In2DownEdge                          = 0x6,            // IN2(범용) 하강 edge
        N_In3DownEdge                          = 0x7,            // IN3(범용) 하강 edge
        N_PElmPositiveEdge                     = 0x8,            // +Elm(End limit) 상승 edge
        N_NElmPositiveEdge                     = 0x9,            // -Elm(End limit) 상승 edge
        N_PSlmPositiveEdge                     = 0xa,            // +Slm(Slowdown limit) 상승 edge
        N_NSlmPositiveEdge                     = 0xb,            // -Slm(Slowdown limit) 상승 edge
        N_In0UpEdge                            = 0xc,            // IN0(ORG) 상승 edge
        N_In1UpEdge                            = 0xd,            // IN1(Z상) 상승 edge
        N_In2UpEdge                            = 0xe,            // IN2(범용) 상승 edge
        N_In3UpEdge                            = 0xf             // IN3(범용) 상승 edge
    } ANT_MOTION_IPDETECT_DESTINATION_SIGNAL;
#endif

#ifndef ANT_MOTION_IPEND_STATUS_DEF
    #define ANT_MOTION_IPEND_STATUS_DEF
    /* IP End status : 0x0000이면 정상종료    */
    typedef    enum _ANT_MOTION_IPEND_STATUS
    {
        N_IPEND_STATUS_SLM                          = 0x0001,           // Bit 0, limit 감속정지 신호 입력에 의한 종료
        N_IPEND_STATUS_ELM                          = 0x0002,           // Bit 1, limit 급정지 신호 입력에 의한 종료
        N_IPEND_STATUS_SSTOP_SIGNAL                 = 0x0004,           // Bit 2, 감속 정지 신호 입력에 의한 종료
        N_IPEND_STATUS_ESTOP_SIGANL                 = 0x0008,           // Bit 3, 급정지 신호 입력에 의한 종료
        N_IPEND_STATUS_SSTOP_COMMAND                = 0x0010,           // Bit 4, 감속 정지 명령에 의한 종료
        N_IPEND_STATUS_ESTOP_COMMAND                = 0x0020,           // Bit 5, 급정지 정지 명령에 의한 종료
        N_IPEND_STATUS_ALARM_SIGNAL                 = 0x0040,           // Bit 6, Alarm 신호 입력에 희한 종료
        N_IPEND_STATUS_DATA_ERROR                   = 0x0080,           // Bit 7, 데이터 설정 에러에 의한 종료
        N_IPEND_STATUS_DEVIATION_ERROR              = 0x0100,           // Bit 8, 탈조 에러에 의한 종료
        N_IPEND_STATUS_ORIGIN_DETECT                = 0x0200,           // Bit 9, 원점 검출에 의한 종료
        N_IPEND_STATUS_SIGNAL_DETECT                = 0x0400,           // Bit 10, 신호 검출에 의한 종료(Signal search-1/2 drive 종료)
        N_IPEND_STATUS_PRESET_PULSE_DRIVE           = 0x0800,           // Bit 11, Preset pulse drive 종료
        N_IPEND_STATUS_SENSOR_PULSE_DRIVE           = 0x1000,           // Bit 12, Sensor pulse drive 종료
        N_IPEND_STATUS_LIMIT                        = 0x2000,           // Bit 13, Limit 완전정지에 의한 종료
        N_IPEND_STATUS_SOFTLIMIT                    = 0x4000,           // Bit 14, Soft limit에 의한 종료
        N_IPEND_STATUS_INTERPOLATION_DRIVE          = 0x8000            // Bit 15, 보간 드라이브에 의한 종료.
    } ANT_MOTION_IPEND_STATUS;
#endif

#ifndef ANT_MOTION_IPDRIVE_STATUS_DEF
    #define ANT_MOTION_IPDRIVE_STATUS_DEF
    /* IP Drive status                        */
    typedef    enum _ANT_MOTION_IPDRIVE_STATUS
    {
        N_IPDRIVE_STATUS_BUSY                        = 0x00001,         // Bit 0, BUSY(드라이브 구동 중)
        N_IPDRIVE_STATUS_DOWN                        = 0x00002,         // Bit 1, DOWN(감속 중)
        N_IPDRIVE_STATUS_CONST                       = 0x00004,         // Bit 2, CONST(등속 중)
        N_IPDRIVE_STATUS_UP                          = 0x00008,         // Bit 3, UP(가속 중)
        N_IPDRIVE_STATUS_ICL                         = 0x00010,         // Bit 4, ICL(내부 위치 카운터 < 내부 위치 카운터 비교값)
        N_IPDRIVE_STATUS_ICG                         = 0x00020,         // Bit 5, ICG(내부 위치 카운터 > 내부 위치 카운터 비교값)
        N_IPDRIVE_STATUS_ECL                         = 0x00040,         // Bit 6, ECL(외부 위치 카운터 < 외부 위치 카운터 비교값)
        N_IPDRIVE_STATUS_ECG                         = 0x00080,         // Bit 7, ECG(외부 위치 카운터 > 외부 위치 카운터 비교값)
        N_IPDRIVE_STATUS_DRIVE_DIRECTION             = 0x00100,         // Bit 8, 드라이브 방향 신호(0=CW/1=CCW)
        N_IPDRIVE_STATUS_COMMAND_BUSY                = 0x00200,         // Bit 9, 명령어 수행중
        N_IPDRIVE_STATUS_PRESET_DRIVING              = 0x00400,         // Bit 10, Preset pulse drive 중
        N_IPDRIVE_STATUS_CONTINUOUS_DRIVING          = 0x00800,         // Bit 11, Continuouse speed drive 중
        N_IPDRIVE_STATUS_SIGNAL_SEARCH_DRIVING       = 0x01000,         // Bit 12, Signal search-1/2 drive 중
        N_IPDRIVE_STATUS_ORG_SEARCH_DRIVING          = 0x02000,         // Bit 13, 원점 검출 drive 중
        N_IPDRIVE_STATUS_MPG_DRIVING                 = 0x04000,         // Bit 14, MPG drive 중
        N_IPDRIVE_STATUS_SENSOR_DRIVING              = 0x08000,         // Bit 15, Sensor positioning drive 중
        N_IPDRIVE_STATUS_L_C_INTERPOLATION           = 0x10000,         // Bit 16, 직선/원호 보간 중
        N_IPDRIVE_STATUS_PATTERN_INTERPOLATION       = 0x20000,         // Bit 17, 비트 패턴 보간 중
        N_IPDRIVE_STATUS_INTERRUPT_BANK1             = 0x40000,         // Bit 18, 인터럽트 bank1에서 발생
        N_IPDRIVE_STATUS_INTERRUPT_BANK2             = 0x80000          // Bit 19, 인터럽트 bank2에서 발생
    } ANT_MOTION_IPDRIVE_STATUS;
#endif

#ifndef ANT_MOTION_IPINTERRUPT_BANK1_DEF
    #define ANT_MOTION_IPINTERRUPT_BANK1_DEF
    /* IP Interrupt MASK1 setting            */
    typedef    enum _ANT_MOTION_IPINTERRUPT_BANK1
    {
        N_IPINTBANK1_DONTUSE                        = 0x00000000,       // INTERRUT DISABLED.
        N_IPINTBANK1_DRIVE_END                      = 0x00000001,       // Bit 0, Drive end(default value : 1).
        N_IPINTBANK1_ICG                            = 0x00000002,       // Bit 1, INCNT is greater than INCNTCMP.
        N_IPINTBANK1_ICE                            = 0x00000004,       // Bit 2, INCNT is equal with INCNTCMP.
        N_IPINTBANK1_ICL                            = 0x00000008,       // Bit 3, INCNT is less than INCNTCMP.
        N_IPINTBANK1_ECG                            = 0x00000010,       // Bit 4, EXCNT is greater than EXCNTCMP.
        N_IPINTBANK1_ECE                            = 0x00000020,       // Bit 5, EXCNT is equal with EXCNTCMP.
        N_IPINTBANK1_ECL                            = 0x00000040,       // Bit 6, EXCNT is less than EXCNTCMP.
        N_IPINTBANK1_SCRQEMPTY                      = 0x00000080,       // Bit 7, Script control queue is empty.
        N_IPINTBANK1_CAPRQEMPTY                     = 0x00000100,       // Bit 8, Caption result data queue is empty.
        N_IPINTBANK1_SCRREG1EXE                     = 0x00000200,       // Bit 9, Script control register-1 command is executed.
        N_IPINTBANK1_SCRREG2EXE                     = 0x00000400,       // Bit 10, Script control register-2 command is executed.
        N_IPINTBANK1_SCRREG3EXE                     = 0x00000800,       // Bit 11, Script control register-3 command is executed.
        N_IPINTBANK1_CAPREG1EXE                     = 0x00001000,       // Bit 12, Caption control register-1 command is executed.
        N_IPINTBANK1_CAPREG2EXE                     = 0x00002000,       // Bit 13, Caption control register-2 command is executed.
        N_IPINTBANK1_CAPREG3EXE                     = 0x00004000,       // Bit 14, Caption control register-3 command is executed.
        N_IPINTBANK1_INTGGENCMD                     = 0x00008000,       // Bit 15, Interrupt generation command is executed(0xFF)
        N_IPINTBANK1_DOWN                           = 0x00010000,       // Bit 16, At starting point for deceleration drive.
        N_IPINTBANK1_CONT                           = 0x00020000,       // Bit 17, At starting point for constant speed drive.
        N_IPINTBANK1_UP                             = 0x00040000,       // Bit 18, At starting point for acceleration drive.
        N_IPINTBANK1_SIGNALDETECTED                 = 0x00080000,       // Bit 19, Signal assigned in MODE1 is detected.
        N_IPINTBANK1_SP23E                          = 0x00100000,       // Bit 20, Current speed is equal with rate change point RCP23.
        N_IPINTBANK1_SP12E                          = 0x00200000,       // Bit 21, Current speed is equal with rate change point RCP12.
        N_IPINTBANK1_SPE                            = 0x00400000,       // Bit 22, Current speed is equal with speed comparison data(SPDCMP).
        N_IPINTBANK1_INCEICM                        = 0x00800000,       // Bit 23, INTCNT(1'st counter) is equal with ICM(1'st count minus limit data)
        N_IPINTBANK1_SCRQEXE                        = 0x01000000,       // Bit 24, Script queue command is executed When SCRCONQ's 30 bit is '1'.
        N_IPINTBANK1_CAPQEXE                        = 0x02000000,       // Bit 25, Caption queue command is executed When CAPCONQ's 30 bit is '1'.
        N_IPINTBANK1_SLM                            = 0x04000000,       // Bit 26, NSLM/PSLM input signal is activated.
        N_IPINTBANK1_ELM                            = 0x08000000,       // Bit 27, NELM/PELM input signal is activated.
        N_IPINTBANK1_USERDEFINE1                    = 0x10000000,       // Bit 28, Selectable interrupt source 0(refer "0xFE" command).
        N_IPINTBANK1_USERDEFINE2                    = 0x20000000,       // Bit 29, Selectable interrupt source 1(refer "0xFE" command).
        N_IPINTBANK1_USERDEFINE3                    = 0x40000000,       // Bit 30, Selectable interrupt source 2(refer "0xFE" command).
        N_IPINTBANK1_USERDEFINE4                    = 0x80000000        // Bit 31, Selectable interrupt source 3(refer "0xFE" command).
    } ANT_MOTION_IPINTERRUPT_BANK1;
#endif

#ifndef ANT_MOTION_IPINTERRUPT_BANK2_DEF
    #define ANT_MOTION_IPINTERRUPT_BANK2_DEF
	/* IP Interrupt MASK2 setting            */
    typedef    enum _ANT_MOTION_IPINTERRUPT_BANK2
    {
        N_IPINTBANK2_DONTUSE                        = 0x00000000,       // INTERRUT DISABLED.
        N_IPINTBANK2_L_C_INP_Q_EMPTY                = 0x00000001,       // Bit 0, Linear/Circular interpolation parameter queue is empty.
        N_IPINTBANK2_P_INP_Q_EMPTY                  = 0x00000002,       // Bit 1, Bit pattern interpolation queue is empty.
        N_IPINTBANK2_ALARM_ERROR                    = 0x00000004,       // Bit 2, Alarm input signal is activated.
        N_IPINTBANK2_INPOSITION                     = 0x00000008,       // Bit 3, Inposition input signal is activated.
        N_IPINTBANK2_MARK_SIGNAL_HIGH               = 0x00000010,       // Bit 4, Mark input signal is activated.
        N_IPINTBANK2_SSTOP_SIGNAL                   = 0x00000020,       // Bit 5, SSTOP input signal is activated.
        N_IPINTBANK2_ESTOP_SIGNAL                   = 0x00000040,       // Bit 6, ESTOP input signal is activated.
        N_IPINTBANK2_SYNC_ACTIVATED                 = 0x00000080,       // Bit 7, SYNC input signal is activated.
        N_IPINTBANK2_TRIGGER_ENABLE                 = 0x00000100,       // Bit 8, Trigger output is activated.
        N_IPINTBANK2_EXCNTCLR                       = 0x00000200,       // Bit 9, External(2'nd) counter is cleard by EXCNTCLR setting.
        N_IPINTBANK2_FSTCOMPARE_RESULT_BIT0         = 0x00000400,       // Bit 10, ALU1's compare result bit 0 is activated.
        N_IPINTBANK2_FSTCOMPARE_RESULT_BIT1         = 0x00000800,       // Bit 11, ALU1's compare result bit 1 is activated.
        N_IPINTBANK2_FSTCOMPARE_RESULT_BIT2         = 0x00001000,       // Bit 12, ALU1's compare result bit 2 is activated.
        N_IPINTBANK2_FSTCOMPARE_RESULT_BIT3         = 0x00002000,       // Bit 13, ALU1's compare result bit 3 is activated.
        N_IPINTBANK2_FSTCOMPARE_RESULT_BIT4         = 0x00004000,       // Bit 14, ALU1's compare result bit 4 is activated.
        N_IPINTBANK2_SNDCOMPARE_RESULT_BIT0         = 0x00008000,       // Bit 15, ALU2's compare result bit 0 is activated.
        N_IPINTBANK2_SNDCOMPARE_RESULT_BIT1         = 0x00010000,       // Bit 16, ALU2's compare result bit 1 is activated.
        N_IPINTBANK2_SNDCOMPARE_RESULT_BIT2         = 0x00020000,       // Bit 17, ALU2's compare result bit 2 is activated.
        N_IPINTBANK2_SNDCOMPARE_RESULT_BIT3         = 0x00040000,       // Bit 18, ALU2's compare result bit 3 is activated.
        N_IPINTBANK2_SNDCOMPARE_RESULT_BIT4         = 0x00080000,       // Bit 19, ALU2's compare result bit 4 is activated.
        N_IPINTBANK2_L_C_INP_Q_LESS_4               = 0x00100000,       // Bit 20, Linear/Circular interpolation parameter queue is less than 4.
        N_IPINTBANK2_P_INP_Q_LESS_4                 = 0x00200000,       // Bit 21, Pattern interpolation parameter queue is less than 4.
        N_IPINTBANK2_XSYNC_ACTIVATED                = 0x00400000,       // Bit 22, X axis sync input signal is activated.
        N_IPINTBANK2_YSYNC_ACTIVATED                = 0x00800000,       // Bit 23, Y axis sync input siangl is activated.
        N_IPINTBANK2_P_INP_END_BY_END_PATTERN       = 0x01000000        // Bit 24, Bit pattern interpolation is terminated by end pattern.
    //  IPINTBANK2_                               = 0x02000000,       // Bit 25, Don't care.
    //  IPINTBANK2_                               = 0x04000000,       // Bit 26, Don't care.
    //  IPINTBANK2_                               = 0x08000000,       // Bit 27, Don't care.
    //  IPINTBANK2_                               = 0x10000000,       // Bit 28, Don't care.
    //  IPINTBANK2_                               = 0x20000000,       // Bit 29, Don't care.
    //  IPINTBANK2_                               = 0x40000000,       // Bit 30, Don't care.
    //  IPINTBANK2_                               = 0x80000000        // Bit 31, Don't care.
    } ANT_MOTION_IPINTERRUPT_BANK2;
#endif


#ifndef ANT_MOTION_IPMECHANICAL_SIGNAL_DEF
    #define ANT_MOTION_IPMECHANICAL_SIGNAL_DEF
    /* IP Drive status                        */
    typedef    enum _ANT_MOTION_IPMECHANICAL_SIGNAL
    {

        N_IPMECHANICAL_PELM_LEVEL                   = 0x0001,           // Bit 0, +Limit 급정지 신호가 액티브 됨
        N_IPMECHANICAL_NELM_LEVEL                   = 0x0002,           // Bit 1, -Limit 급정지 신호 액티브 됨
        N_IPMECHANICAL_PSLM_LEVEL                   = 0x0004,           // Bit 2, +limit 감속정지 신호 액티브 됨
        N_IPMECHANICAL_NSLM_LEVEL                   = 0x0008,           // Bit 3, -limit 감속정지 신호 액티브 됨
        N_IPMECHANICAL_ALARM_LEVEL                  = 0x0010,           // Bit 4, Alarm 신호 액티브 됨
        N_IPMECHANICAL_INP_LEVEL                    = 0x0020,           // Bit 5, Inposition 신호 액티브 됨
        N_IPMECHANICAL_ENC_DOWN_LEVEL               = 0x0040,           // Bit 6, 엔코더 DOWN(B상) 신호 입력 Level
        N_IPMECHANICAL_ENC_UP_LEVEL                 = 0x0080,           // Bit 7, 엔코더 UP(A상) 신호 입력 Level
        N_IPMECHANICAL_EXMP_LEVEL                   = 0x0100,           // Bit 8, EXMP 신호 입력 Level
        N_IPMECHANICAL_EXPP_LEVEL                   = 0x0200,           // Bit 9, EXPP 신호 입력 Level
        N_IPMECHANICAL_MARK_LEVEL                   = 0x0400,           // Bit 10, MARK# 신호 액티브 됨
        N_IPMECHANICAL_SSTOP_LEVEL                  = 0x0800,           // Bit 11, SSTOP 신호 액티브 됨
        N_IPMECHANICAL_ESTOP_LEVEL                  = 0x1000,           // Bit 12, ESTOP 신호 액티브 됨
        N_IPMECHANICAL_SYNC_LEVEL                   = 0x2000,           // Bit 13, SYNC 신호 입력 Level
        N_IPMECHANICAL_MODE8_16_LEVEL               = 0x4000            // Bit 14, MODE8_16 신호 입력 Level

    } ANT_MOTION_IPMECHANICAL_SIGNAL;
#endif

#ifndef ANT_MOTION_QIDETECT_DESTINATION_SIGNAL_DEF
    #define ANT_MOTION_QIDETECT_DESTINATION_SIGNAL_DEF
    /* Detect Destination Signal            */
    typedef enum _ANT_MOTION_QIDETECT_DESTINATION_SIGNAL
    {
        N_Signal_PosEndLimit                        = 0x0,              // +Elm(End limit) +방향 리미트 센서 신호
        N_Signal_NegEndLimit                        = 0x1,              // -Elm(End limit) -방향 리미트 센서 신호
        N_Signal_PosSloLimit                        = 0x2,              // +Slm(Slow Down limit) 신호 - 사용하지 않음
        N_Signal_NegSloLimit                        = 0x3,              // -Slm(Slow Down limit) 신호 - 사용하지 않음
        N_Signal_HomeSensor                         = 0x4,              // IN0(ORG)  원점 센서 신호
        N_Signal_EncodZPhase                        = 0x5,              // IN1(Z상)  Encoder Z상 신호
        N_Signal_UniInput02                         = 0x6,              // IN2(범용) 범용 입력 2번 신호
        N_Signal_UniInput03                         = 0x7               // IN3(범용) 범용 입력 3번 신호
    } ANT_MOTION_QIDETECT_DESTINATION_SIGNAL;
#endif

#ifndef ANT_MOTION_QIMECHANICAL_SIGNAL_DEF
    #define ANT_MOTION_QIMECHANICAL_SIGNAL_DEF
    /* QI Drive status                        */
    typedef    enum _ANT_MOTION_QIMECHANICAL_SIGNAL
    {

        N_QIMECHANICAL_PELM_LEVEL                   = 0x00001,          // Bit 0, +Limit 급정지 신호 현재 상태
        N_QIMECHANICAL_NELM_LEVEL                   = 0x00002,          // Bit 1, -Limit 급정지 신호 현재 상태
        N_QIMECHANICAL_PSLM_LEVEL                   = 0x00004,          // Bit 2, +limit 감속정지 현재 상태.
        N_QIMECHANICAL_NSLM_LEVEL                   = 0x00008,          // Bit 3, -limit 감속정지 현재 상태
        N_QIMECHANICAL_ALARM_LEVEL                  = 0x00010,          // Bit 4, Alarm 신호 현재 상태
        N_QIMECHANICAL_INP_LEVEL                    = 0x00020,          // Bit 5, Inposition 신호 현재 상태
        N_QIMECHANICAL_ESTOP_LEVEL                  = 0x00040,          // Bit 6, 비상 정지 신호(ESTOP) 현재 상태.
        N_QIMECHANICAL_ORG_LEVEL                    = 0x00080,          // Bit 7, 원점 신호 헌재 상태
        N_QIMECHANICAL_ZPHASE_LEVEL                 = 0x00100,          // Bit 8, Z 상 입력 신호 현재 상태
        N_QIMECHANICAL_ECUP_LEVEL                   = 0x00200,          // Bit 9, ECUP 터미널 신호 상태.
        N_QIMECHANICAL_ECDN_LEVEL                   = 0x00400,          // Bit 10, ECDN 터미널 신호 상태.
        N_QIMECHANICAL_EXPP_LEVEL                   = 0x00800,          // Bit 11, EXPP 터미널 신호 상태
        N_QIMECHANICAL_EXMP_LEVEL                   = 0x01000,          // Bit 12, EXMP 터미널 신호 상태
        N_QIMECHANICAL_SQSTR1_LEVEL                 = 0x02000,          // Bit 13, SQSTR1 터미널 신호 상태
        N_QIMECHANICAL_SQSTR2_LEVEL                 = 0x04000,          // Bit 14, SQSTR2 터미널 신호 상태
        N_QIMECHANICAL_SQSTP1_LEVEL                 = 0x08000,          // Bit 15, SQSTP1 터미널 신호 상태
        N_QIMECHANICAL_SQSTP2_LEVEL                 = 0x10000,          // Bit 16, SQSTP2 터미널 신호 상태
        N_QIMECHANICAL_MODE_LEVEL                   = 0x20000           // Bit 17, MODE 터미널 신호 상태.
    } ANT_MOTION_QIMECHANICAL_SIGNAL;
#endif

#ifndef ANT_MOTION_QIEND_STATUS_DEF
    #define ANT_MOTION_QIEND_STATUS_DEF
    /* QI End status : 0x0000이면 정상종료    */
    typedef    enum _ANT_MOTION_QIEND_STATUS
    {
        N_QIEND_STATUS_0                            = 0x00000001,       // Bit 0, 정방향 리미트 신호(PELM)에 의한 종료
        N_QIEND_STATUS_1                            = 0x00000002,       // Bit 1, 역방향 리미트 신호(NELM)에 의한 종료
        N_QIEND_STATUS_2                            = 0x00000004,       // Bit 2, 정방향 부가 리미트 신호(PSLM)에 의한 구동 종료
        N_QIEND_STATUS_3                            = 0x00000008,       // Bit 3, 역방향 부가 리미트 신호(NSLM)에 의한 구동 종료
        N_QIEND_STATUS_4                            = 0x00000010,       // Bit 4, 정방향 소프트 리미트 급정지 기능에 의한 구동 종료
        N_QIEND_STATUS_5                            = 0x00000020,       // Bit 5, 역방향 소프트 리미트 급정지 기능에 의한 구동 종료
        N_QIEND_STATUS_6                            = 0x00000040,       // Bit 6, 정방향 소프트 리미트 감속정지 기능에 의한 구동 종료
        N_QIEND_STATUS_7                            = 0x00000080,       // Bit 7, 역방향 소프트 리미트 감속정지 기능에 의한 구동 종료
        N_QIEND_STATUS_8                            = 0x00000100,       // Bit 8, 서보 알람 기능에 의한 구동 종료.
        N_QIEND_STATUS_9                            = 0x00000200,       // Bit 9, 비상 정지 신호 입력에 의한 구동 종료.
        N_QIEND_STATUS_10                           = 0x00000400,       // Bit 10, 급 정지 명령에 의한 구동 종료.
        N_QIEND_STATUS_11                           = 0x00000800,       // Bit 11, 감속 정지 명령에 의한 구동 종료.
        N_QIEND_STATUS_12                           = 0x00001000,       // Bit 12, 전축 급정지 명령에 의한 구동 종료
        N_QIEND_STATUS_13                           = 0x00002000,       // Bit 13, 동기 정지 기능 #1(SQSTP1)에 의한 구동 종료.
        N_QIEND_STATUS_14                           = 0x00004000,       // Bit 14, 동기 정지 기능 #2(SQSTP2)에 의한 구동 종료.
        N_QIEND_STATUS_15                           = 0x00008000,       // Bit 15, 인코더 입력(ECUP,ECDN) 오류 발생
        N_QIEND_STATUS_16                           = 0x00010000,       // Bit 16, MPG 입력(EXPP,EXMP) 오류 발생
        N_QIEND_STATUS_17                           = 0x00020000,       // Bit 17, 원점 검색 성공 종료.
        N_QIEND_STATUS_18                           = 0x00040000,       // Bit 18, 신호 검색 성공 종료.
        N_QIEND_STATUS_19                           = 0x00080000,       // Bit 19, 보간 데이터 이상으로 구동 종료.
        N_QIEND_STATUS_20                           = 0x00100000,       // Bit 20, 비정상 구동 정지발생.
        N_QIEND_STATUS_21                           = 0x00200000,       // Bit 21, MPG 기능 블록 펄스 버퍼 오버플로우 발생
        N_QIEND_STATUS_22                           = 0x00400000,       // Bit 22, DON'CARE
        N_QIEND_STATUS_23                           = 0x00800000,       // Bit 23, DON'CARE
        N_QIEND_STATUS_24                           = 0x01000000,       // Bit 24, DON'CARE
        N_QIEND_STATUS_25                           = 0x02000000,       // Bit 25, DON'CARE
        N_QIEND_STATUS_26                           = 0x04000000,       // Bit 26, DON'CARE
        N_QIEND_STATUS_27                           = 0x08000000,       // Bit 27, DON'CARE
        N_QIEND_STATUS_28                           = 0x10000000,       // Bit 28, 현재/마지막 구동 드라이브 방향
        N_QIEND_STATUS_29                           = 0x20000000,       // Bit 29, 잔여 펄스 제거 신호 출력 중.
        N_QIEND_STATUS_30                           = 0x40000000,       // Bit 30, 비정상 구동 정지 원인 상태
        N_QIEND_STATUS_31                           = 0x80000000        // Bit 31, 보간 드라이브 데이타 오류 상태.
    } ANT_MOTION_QIEND_STATUS;
#endif

#ifndef ANT_MOTION_QIDRIVE_STATUS_DEF
    #define ANT_MOTION_QIDRIVE_STATUS_DEF
    /* QI Drive status                        */
    typedef    enum _ANT_MOTION_QIDRIVE_STATUS
    {
        N_QIDRIVE_STATUS_0                          = 0x0000001,        // Bit 0, BUSY(드라이브 구동 중)
        N_QIDRIVE_STATUS_1                          = 0x0000002,        // Bit 1, DOWN(감속 중)
        N_QIDRIVE_STATUS_2                          = 0x0000004,        // Bit 2, CONST(등속 중)
        N_QIDRIVE_STATUS_3                          = 0x0000008,        // Bit 3, UP(가속 중)
        N_QIDRIVE_STATUS_4                          = 0x0000010,        // Bit 4, 연속 드라이브 구동 중
        N_QIDRIVE_STATUS_5                          = 0x0000020,        // Bit 5, 지정 거리 드라이브 구동 중
        N_QIDRIVE_STATUS_6                          = 0x0000040,        // Bit 6, MPG 드라이브 구동 중
        N_QIDRIVE_STATUS_7                          = 0x0000080,        // Bit 7, 원점검색 드라이브 구동중 
//주의사항 : (현재 라이브러리 에서 구현된 HOME함수와 상관없이 동작된다.)
        N_QIDRIVE_STATUS_8                          = 0x0000100,        // Bit 8, 신호 검색 드라이브 구동 중
//주의사항 : (현재 라이브러리 에서 구현된 HOME함수와 같이 동작된다. 홈함수가 신호서치함수를 여러번사용하여 구현이 되었으며 홈 검색시 BIT: 1 된다.)
        N_QIDRIVE_STATUS_9                          = 0x0000200,        // Bit 9, 보간 드라이브 구동 중
        N_QIDRIVE_STATUS_10                         = 0x0000400,        // Bit 10, Slave 드라이브 구동중
        N_QIDRIVE_STATUS_11                         = 0x0000800,        // Bit 11, 현재 구동 드라이브 방향(보간 드라이브에서는 표시 정보 다름)
        N_QIDRIVE_STATUS_12                         = 0x0001000,        // Bit 12, 펄스 출력후 서보위치 완료 신호 대기중.
        N_QIDRIVE_STATUS_13                         = 0x0002000,        // Bit 13, 직선 보간 드라이브 구동중.
        N_QIDRIVE_STATUS_14                         = 0x0004000,        // Bit 14, 원호 보간 드라이브 구동중.
        N_QIDRIVE_STATUS_15                         = 0x0008000,        // Bit 15, 펄스 출력 중.
        N_QIDRIVE_STATUS_16                         = 0x0010000,        // Bit 16, 구동 예약 데이터 개수(처음)(0-7)
        N_QIDRIVE_STATUS_17                         = 0x0020000,        // Bit 17, 구동 예약 데이터 개수(중간)(0-7)
        N_QIDRIVE_STATUS_18                         = 0x0040000,        // Bit 18, 구동 예약 데이터 갯수(끝)(0-7)
        N_QIDRIVE_STATUS_19                         = 0x0100000,        // Bit 19, 구동 예약 Queue 비어 있음.
        N_QIDRIVE_STATUS_20                         = 0x0200000,        // Bit 20, 구동 예약 Queue 가득 찲
        N_QIDRIVE_STATUS_21                         = 0x0400000,        // Bit 21, 현재 구동 드라이브의 속도 모드(처음)
        N_QIDRIVE_STATUS_22                         = 0x0800000,        // Bit 22, 현재 구동 드라이브의 속도 모드(끝)
        N_QIDRIVE_STATUS_23                         = 0x1000000,        // Bit 23, MPG 버퍼 #1 Full
        N_QIDRIVE_STATUS_24                         = 0x2000000,        // Bit 24, MPG 버퍼 #2 Full
        N_QIDRIVE_STATUS_25                         = 0x4000000,        // Bit 25, MPG 버퍼 #3 Full
        N_QIDRIVE_STATUS_26                         = 0x8000000         // Bit 26, MPG 버퍼 데이터 OverFlow

    } ANT_MOTION_QIDRIVE_STATUS;
#endif

#ifndef ANT_MOTION_QIINTERRUPT_BANK1_DEF
    #define ANT_MOTION_QIINTERRUPT_BANK1_DEF
    /* QI Interrupt MASK1 setting            */
    typedef    enum _ANT_MOTION_QIINTERRUPT_BANK1
    {
        N_QIINTBANK1_DISABLE                        = 0x00000000,       // INTERRUT DISABLED.
        N_QIINTBANK1_0                              = 0x00000001,       // Bit 0,  인터럽트 발생 사용 설정된 구동 종료시.
        N_QIINTBANK1_1                              = 0x00000002,       // Bit 1,  구동 종료시
        N_QIINTBANK1_2                              = 0x00000004,       // Bit 2,  구동 시작시.
        N_QIINTBANK1_3                              = 0x00000008,       // Bit 3,  카운터 #1 < 비교기 #1 이벤트 발생
        N_QIINTBANK1_4                              = 0x00000010,       // Bit 4,  카운터 #1 = 비교기 #1 이벤트 발생
        N_QIINTBANK1_5                              = 0x00000020,       // Bit 5,  카운터 #1 > 비교기 #1 이벤트 발생
        N_QIINTBANK1_6                              = 0x00000040,       // Bit 6,  카운터 #2 < 비교기 #2 이벤트 발생
        N_QIINTBANK1_7                              = 0x00000080,       // Bit 7,  카운터 #2 = 비교기 #2 이벤트 발생
        N_QIINTBANK1_8                              = 0x00000100,       // Bit 8,  카운터 #2 > 비교기 #2 이벤트 발생
        N_QIINTBANK1_9                              = 0x00000200,       // Bit 9,  카운터 #3 < 비교기 #3 이벤트 발생
        N_QIINTBANK1_10                             = 0x00000400,       // Bit 10, 카운터 #3 = 비교기 #3 이벤트 발생
        N_QIINTBANK1_11                             = 0x00000800,       // Bit 11, 카운터 #3 > 비교기 #3 이벤트 발생
        N_QIINTBANK1_12                             = 0x00001000,       // Bit 12, 카운터 #4 < 비교기 #4 이벤트 발생
        N_QIINTBANK1_13                             = 0x00002000,       // Bit 13, 카운터 #4 = 비교기 #4 이벤트 발생
        N_QIINTBANK1_14                             = 0x00004000,       // Bit 14, 카운터 #4 < 비교기 #4 이벤트 발생
        N_QIINTBANK1_15                             = 0x00008000,       // Bit 15, 카운터 #5 < 비교기 #5 이벤트 발생
        N_QIINTBANK1_16                             = 0x00010000,       // Bit 16, 카운터 #5 = 비교기 #5 이벤트 발생
        N_QIINTBANK1_17                             = 0x00020000,       // Bit 17, 카운터 #5 > 비교기 #5 이벤트 발생
        N_QIINTBANK1_18                             = 0x00040000,       // Bit 18, 타이머 #1 이벤트 발생.
        N_QIINTBANK1_19                             = 0x00080000,       // Bit 19, 타이머 #2 이벤트 발생.
        N_QIINTBANK1_20                             = 0x00100000,       // Bit 20, 구동 예약 설정 Queue 비워짐.
        N_QIINTBANK1_21                             = 0x00200000,       // Bit 21, 구동 예약 설정 Queue 가득찲
        N_QIINTBANK1_22                             = 0x00400000,       // Bit 22, 트리거 발생거리 주기/절대위치 Queue 비워짐.
        N_QIINTBANK1_23                             = 0x00800000,       // Bit 23, 트리거 발생거리 주기/절대위치 Queue 가득찲
        N_QIINTBANK1_24                             = 0x01000000,       // Bit 24, 트리거 신호 발생 이벤트
        N_QIINTBANK1_25                             = 0x02000000,       // Bit 25, 스크립트 #1 명령어 예약 설정 Queue 비워짐.
        N_QIINTBANK1_26                             = 0x04000000,       // Bit 26, 스크립트 #2 명령어 예약 설정 Queue 비워짐.
        N_QIINTBANK1_27                             = 0x08000000,       // Bit 27, 스크립트 #3 명령어 예약 설정 레지스터 실행되어 초기화 됨.
        N_QIINTBANK1_28                             = 0x10000000,       // Bit 28, 스크립트 #4 명령어 예약 설정 레지스터 실행되어 초기화 됨.
        N_QIINTBANK1_29                             = 0x20000000,       // Bit 29, 서보 알람신호 인가됨.
        N_QIINTBANK1_30                             = 0x40000000,       // Bit 30, |CNT1| - |CNT2| >= |CNT4| 이벤트 발생.
        N_QIINTBANK1_31                             = 0x80000000        // Bit 31, 인터럽트 발생 명령어|INTGEN| 실행.
    } ANT_MOTION_QIINTERRUPT_BANK1;
#endif

#ifndef ANT_MOTION_QIINTERRUPT_BANK2_DEF
    #define ANT_MOTION_QIINTERRUPT_BANK2_DEF
	/* QI Interrupt MASK2 setting            */
    typedef    enum _ANT_MOTION_QIINTERRUPT_BANK2
    {
        N_QIINTBANK2_DISABLE                        = 0x00000000,       // INTERRUT DISABLED.
        N_QIINTBANK2_0                              = 0x00000001,       // Bit 0,  스크립트 #1 읽기 명령 결과 Queue 가 가득찲.
        N_QIINTBANK2_1                              = 0x00000002,       // Bit 1,  스크립트 #2 읽기 명령 결과 Queue 가 가득찲.
        N_QIINTBANK2_2                              = 0x00000004,       // Bit 2,  스크립트 #3 읽기 명령 결과 레지스터가 새로운 데이터로 갱신됨.
        N_QIINTBANK2_3                              = 0x00000008,       // Bit 3,  스크립트 #4 읽기 명령 결과 레지스터가 새로운 데이터로 갱신됨.
        N_QIINTBANK2_4                              = 0x00000010,       // Bit 4,  스크립트 #1 의 예약 명령어 중 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
        N_QIINTBANK2_5                              = 0x00000020,       // Bit 5,  스크립트 #2 의 예약 명령어 중 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
        N_QIINTBANK2_6                              = 0x00000040,       // Bit 6,  스크립트 #3 의 예약 명령어 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
        N_QIINTBANK2_7                              = 0x00000080,       // Bit 7,  스크립트 #4 의 예약 명령어 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
        N_QIINTBANK2_8                              = 0x00000100,       // Bit 8,  구동 시작
        N_QIINTBANK2_9                              = 0x00000200,       // Bit 9,  서보 위치 결정 완료(Inposition)기능을 사용한 구동,종료 조건 발생.
        N_QIINTBANK2_10                             = 0x00000400,       // Bit 10, 이벤트 카운터로 동작 시 사용할 이벤트 선택 #1 조건 발생.
        N_QIINTBANK2_11                             = 0x00000800,       // Bit 11, 이벤트 카운터로 동작 시 사용할 이벤트 선택 #2 조건 발생.
        N_QIINTBANK2_12                             = 0x00001000,       // Bit 12, SQSTR1 신호 인가 됨.
        N_QIINTBANK2_13                             = 0x00002000,       // Bit 13, SQSTR2 신호 인가 됨.
        N_QIINTBANK2_14                             = 0x00004000,       // Bit 14, UIO0 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_15                             = 0x00008000,       // Bit 15, UIO1 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_16                             = 0x00010000,       // Bit 16, UIO2 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_17                             = 0x00020000,       // Bit 17, UIO3 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_18                             = 0x00040000,       // Bit 18, UIO4 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_19                             = 0x00080000,       // Bit 19, UIO5 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_20                             = 0x00100000,       // Bit 20, UIO6 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_21                             = 0x00200000,       // Bit 21, UIO7 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_22                             = 0x00400000,       // Bit 22, UIO8 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_23                             = 0x00800000,       // Bit 23, UIO9 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_24                             = 0x01000000,       // Bit 24, UIO10 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_25                             = 0x02000000,       // Bit 25, UIO11 터미널 신호가 '1'로 변함.
        N_QIINTBANK2_26                             = 0x04000000,       // Bit 26, 오류 정지 조건(LMT, ESTOP, STOP, ESTOP, CMD, ALARM) 발생.
        N_QIINTBANK2_27                             = 0x08000000,       // Bit 27, 보간 중 데이터 설정 오류 발생.
        N_QIINTBANK2_28                             = 0x10000000,       // Bit 28, Don't Care
        N_QIINTBANK2_29                             = 0x20000000,       // Bit 29, 리미트 신호(PELM, NELM)신호가 입력 됨.
        N_QIINTBANK2_30                             = 0x40000000,       // Bit 30, 부가 리미트 신호(PSLM, NSLM)신호가 입력 됨.
        N_QIINTBANK2_31                             = 0x80000000        // Bit 31, 비상 정지 신호(ESTOP)신호가 입력됨.

    } ANT_MOTION_QIINTERRUPT_BANK2;
#endif

typedef void (__stdcall *ANT_EVENT_PROC)(long lActiveNo, DWORD uFlag);
typedef void (__stdcall *ANT_INTERRUPT_PROC)(long lActiveNo, DWORD uFlag);
typedef void (__stdcall *ANT_NETMON_PROC)(long lActiveNo, DWORD uFlag);

#endif    //__ANT_AXHS_H__
