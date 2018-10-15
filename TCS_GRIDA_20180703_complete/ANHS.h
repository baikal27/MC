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

// ���̽���� ����
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

// ��� ����
#ifndef __ANT_FUNC_MODULE__
    #define __ANT_FUNC_MODULE__
    typedef enum _ANT_FUNC_MODULE
    {
        ANT_NMC_2V04    = 0xB1,        // CAMC-QI, 2 Axis
        ANT_NMC_4V04    = 0xB2,        // CAMC-QI, 4 Axis
        ANT_NIO_DC16    = 0xB3,        // Digital 16 �� Configurable
        ANT_NIO_AI4     = 0xB4,        // AI 4Ch, 16 bit
        ANT_NIO_DI32    = 0xB5,        // Digital IN  32��    (reserved)
        ANT_NIO_DO32    = 0xB6,        // Digital OUT 32��    (reserved)
        ANT_NIO_DB32    = 0xB7,        // Digital IN  16�� / OUT 16��    (reserved)
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
    } ANT_LOG_LEVEL;    // �α� ����
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
        N_HOME_ERR_AMP_FAULT                 = 0x13,        // ������ �˶� �߻� ����
        N_HOME_ERR_NEG_LIMIT                 = 0x14,        // (-)���� ������ (+)����Ʈ ���� ���� ����
        N_HOME_ERR_POS_LIMIT                 = 0x15,        // (+)���� ������ (-)����Ʈ ���� ���� ����
        N_HOME_ERR_NOT_DETECT                = 0x16,        // ������ ��ȣ �������� �� �� ��� ����
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
        N_OneHighLowHigh                       = 0x0,             // 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
        N_OneHighHighLow                       = 0x1,             // 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
        N_OneLowLowHigh                        = 0x2,             // 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
        N_OneLowHighLow                        = 0x3,             // 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
        N_TwoCcwCwHigh                         = 0x4,             // 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High     
        N_TwoCcwCwLow                          = 0x5,             // 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low     
        N_TwoCwCcwHigh                         = 0x6,             // 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
        N_TwoCwCcwLow                          = 0x7,             // 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low
        N_TwoPhase                             = 0x8,             // 2��(90' ������),  PULSE lead DIR(CW: ������), PULSE lag DIR(CCW:������)
        N_TwoPhaseReverse                      = 0x9              // 2��(90' ������),  PULSE lead DIR(CCW: ������), PULSE lag DIR(CW:������)
    } ANT_MOTION_PULSE_OUTPUT;
#endif

#ifndef ANT_MOTION_EXTERNAL_COUNTER_INPUT_DEF
    #define ANT_MOTION_EXTERNAL_COUNTER_INPUT_DEF
    /* Mode2 Data   External Counter Input */
    typedef enum _ANT_MOTION__EXTERNAL_COUNTER_INPUT
    {
        N_ObverseUpDownMode                     = 0x0,            // ������ Up/Down
        N_ObverseSqr1Mode                       = 0x1,            // ������ 1ü��
        N_ObverseSqr2Mode                       = 0x2,            // ������ 2ü��
        N_ObverseSqr4Mode                       = 0x3,            // ������ 4ü��
        N_ReverseUpDownMode                     = 0x4,            // ������ Up/Down
        N_ReverseSqr1Mode                       = 0x5,            // ������ 1ü��
        N_ReverseSqr2Mode                       = 0x6,            // ������ 2ü��
        N_ReverseSqr4Mode                       = 0x7             // ������ 4ü��
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
        N_DIR_CCW                        = 0x0,            // �ݽð����
        N_DIR_CW                         = 0x1             // �ð����
    } ANT_MOTION_MOVE_DIR;
#endif

#ifndef ANT_MOTION_RADIUS_DISTANCE_DEF
    #define ANT_MOTION_RADIUS_DISTANCE_DEF
    typedef enum _ANT_MOTION_RADIUS_DISTANCE
    {
        N_SHORT_DISTANCE                 = 0x0,            // ª�� �Ÿ��� ��ȣ �̵� 
        N_LONG_DISTANCE                  = 0x1             // �� �Ÿ��� ��ȣ �̵� 
    } ANT_MOTION_RADIUS_DISTANCE;
#endif

#ifndef ANT_MOTION_INTERPOLATION_ANIS_DEF
    #define ANT_MOTION_INTERPOLATION_ANIS_DEF
    typedef enum _ANT_MOTION_INTERPOLATION_ANIS
    {
        N_INTERPOLATION_ANIS2            = 0x0,            // 2���� �������� ����� ��
        N_INTERPOLATION_ANIS3            = 0x1,            // 3���� �������� ����� ��
        N_INTERPOLATION_ANIS4            = 0x2             // 4���� �������� ����� ��
    } ANT_MOTION_INTERPOLATION_ANIS;
#endif

#ifndef ANT_MOTION_CONTISTART_NODE_DEF
    #define ANT_MOTION_CONTISTART_NODE_DEF
    typedef enum _ANT_MOTION_CONTISTART_NODE
    {
        N_CONTI_NODE_VELOCITY             = 0x0,            // �ӵ� ���� ���� ���
        N_CONTI_NODE_MANUAL               = 0x1,            // ��� ������ ���� ���
        N_CONTI_NODE_AUTO                 = 0x2             // �ڵ� ������ ���� ���
    } ANT_MOTION_CONTISTART_NODE;
#endif

#ifndef ANT_MOTION_HOME_DETECT_SIGNAL_DEF
    #define ANT_MOTION_HOME_DETECT_SIGNAL_DEF
    typedef enum _ANT_MOTION_HOME_DETECT_SIGNAL
    {
        N_PosEndLimit                          = 0x0,            // +Elm(End limit) +���� ����Ʈ ���� ��ȣ
        N_NegEndLimit                          = 0x1,            // -Elm(End limit) -���� ����Ʈ ���� ��ȣ
        N_PosSloLimit                          = 0x2,            // +Slm(Slow Down limit) ��ȣ - ������� ����
        N_NegSloLimit                          = 0x3,            // -Slm(Slow Down limit) ��ȣ - ������� ����
        N_HomeSensor                           = 0x4,            // IN0(ORG)  ���� ���� ��ȣ
        N_EncodZPhase                          = 0x5,            // IN1(Z��)  Encoder Z�� ��ȣ
        N_UniInput02                           = 0x6,            // IN2(����) ���� �Է� 2�� ��ȣ
        N_UniInput03                           = 0x7,            // IN3(����) ���� �Է� 3�� ��ȣ
    } ANT_MOTION_HOME_DETECT_SIGNAL;
#endif


#ifndef ANT_MOTION_MPG_INPUT_METHOD_DEF
    #define ANT_MOTION_MPG_INPUT_METHOD_DEF
    typedef enum _ANT_MOTION_MPG_INPUT_METHOD
    {
        N_MPG_DIFF_ONE_PHASE                   = 0x0,            // MPG �Է� ��� One Phase
        N_MPG_DIFF_TWO_PHASE_1X                = 0x1,            // MPG �Է� ��� TwoPhase1
        N_MPG_DIFF_TWO_PHASE_2X                = 0x2,            // MPG �Է� ��� TwoPhase2
        N_MPG_DIFF_TWO_PHASE_4X                = 0x3,            // MPG �Է� ��� TwoPhase4
        N_MPG_LEVEL_ONE_PHASE                  = 0x4,            // MPG �Է� ��� Level One Phase
        N_MPG_LEVEL_TWO_PHASE_1X               = 0x5,            // MPG �Է� ��� Level Two Phase1
        N_MPG_LEVEL_TWO_PHASE_2X               = 0x6,            // MPG �Է� ��� Level Two Phase2
        N_MPG_LEVEL_TWO_PHASE_4X               = 0x7,            // MPG �Է� ��� Level Two Phase4
    } ANT_MOTION_MPG_INPUT_METHOD;
#endif

#ifndef ANT_MOTION_SENSOR_INPUT_METHOD_DEF
    #define ANT_MOTION_SENSOR_INPUT_METHOD_DEF
    typedef enum _ANT_MOTION_SENSOR_INPUT_METHOD
    {
        N_SENSOR_METHOD1                = 0x0,             // �Ϲ� ����
        N_SENSOR_METHOD2                = 0x1,             // ���� ��ȣ ���� ���� ���� ����. ��ȣ ���� �� �Ϲ� ����
        N_SENSOR_METHOD3                = 0x2              // ���� ����
    } ANT_MOTION_SENSOR_INPUT_METHOD;
#endif

#ifndef ANT_MOTION_HOME_CRC_SELECT_DEF
    #define ANT_MOTION_HOME_CRC_SELECT_DEF
    typedef enum _ANT_MOTION_HOME_CRC_SELECT
    {
        N_CRC_SELECT1                  = 0x0,              // ��ġŬ���� ������, �ܿ��޽� Ŭ���� ��� ����
        N_CRC_SELECT2                  = 0x1,              // ��ġŬ���� �����, �ܿ��޽� Ŭ���� ��� ����
        N_CRC_SELECT3                  = 0x2,              // ��ġŬ���� ������, �ܿ��޽� Ŭ���� �����
        N_CRC_SELECT4                  = 0x3               // ��ġŬ���� �����, �ܿ��޽� Ŭ���� �����
    } ANT_MOTION_HOME_CRC_SELECT;
#endif

#ifndef ANT_MOTION_IPDETECT_DESTINATION_SIGNAL_DEF
    #define ANT_MOTION_IPDETECT_DESTINATION_SIGNAL_DEF
    /* Detect Destination Signal            */
    typedef enum _ANT_MOTION_IPDETECT_DESTINATION_SIGNAL
    {
        N_PElmNegativeEdge                     = 0x0,            // +Elm(End limit) �ϰ� edge
        N_NElmNegativeEdge                     = 0x1,            // -Elm(End limit) �ϰ� edge
        N_PSlmNegativeEdge                     = 0x2,            // +Slm(Slowdown limit) �ϰ� edge
        N_NSlmNegativeEdge                     = 0x3,            // -Slm(Slowdown limit) �ϰ� edge
        N_In0DownEdge                          = 0x4,            // IN0(ORG) �ϰ� edge
        N_In1DownEdge                          = 0x5,            // IN1(Z��) �ϰ� edge
        N_In2DownEdge                          = 0x6,            // IN2(����) �ϰ� edge
        N_In3DownEdge                          = 0x7,            // IN3(����) �ϰ� edge
        N_PElmPositiveEdge                     = 0x8,            // +Elm(End limit) ��� edge
        N_NElmPositiveEdge                     = 0x9,            // -Elm(End limit) ��� edge
        N_PSlmPositiveEdge                     = 0xa,            // +Slm(Slowdown limit) ��� edge
        N_NSlmPositiveEdge                     = 0xb,            // -Slm(Slowdown limit) ��� edge
        N_In0UpEdge                            = 0xc,            // IN0(ORG) ��� edge
        N_In1UpEdge                            = 0xd,            // IN1(Z��) ��� edge
        N_In2UpEdge                            = 0xe,            // IN2(����) ��� edge
        N_In3UpEdge                            = 0xf             // IN3(����) ��� edge
    } ANT_MOTION_IPDETECT_DESTINATION_SIGNAL;
#endif

#ifndef ANT_MOTION_IPEND_STATUS_DEF
    #define ANT_MOTION_IPEND_STATUS_DEF
    /* IP End status : 0x0000�̸� ��������    */
    typedef    enum _ANT_MOTION_IPEND_STATUS
    {
        N_IPEND_STATUS_SLM                          = 0x0001,           // Bit 0, limit �������� ��ȣ �Է¿� ���� ����
        N_IPEND_STATUS_ELM                          = 0x0002,           // Bit 1, limit ������ ��ȣ �Է¿� ���� ����
        N_IPEND_STATUS_SSTOP_SIGNAL                 = 0x0004,           // Bit 2, ���� ���� ��ȣ �Է¿� ���� ����
        N_IPEND_STATUS_ESTOP_SIGANL                 = 0x0008,           // Bit 3, ������ ��ȣ �Է¿� ���� ����
        N_IPEND_STATUS_SSTOP_COMMAND                = 0x0010,           // Bit 4, ���� ���� ��ɿ� ���� ����
        N_IPEND_STATUS_ESTOP_COMMAND                = 0x0020,           // Bit 5, ������ ���� ��ɿ� ���� ����
        N_IPEND_STATUS_ALARM_SIGNAL                 = 0x0040,           // Bit 6, Alarm ��ȣ �Է¿� ���� ����
        N_IPEND_STATUS_DATA_ERROR                   = 0x0080,           // Bit 7, ������ ���� ������ ���� ����
        N_IPEND_STATUS_DEVIATION_ERROR              = 0x0100,           // Bit 8, Ż�� ������ ���� ����
        N_IPEND_STATUS_ORIGIN_DETECT                = 0x0200,           // Bit 9, ���� ���⿡ ���� ����
        N_IPEND_STATUS_SIGNAL_DETECT                = 0x0400,           // Bit 10, ��ȣ ���⿡ ���� ����(Signal search-1/2 drive ����)
        N_IPEND_STATUS_PRESET_PULSE_DRIVE           = 0x0800,           // Bit 11, Preset pulse drive ����
        N_IPEND_STATUS_SENSOR_PULSE_DRIVE           = 0x1000,           // Bit 12, Sensor pulse drive ����
        N_IPEND_STATUS_LIMIT                        = 0x2000,           // Bit 13, Limit ���������� ���� ����
        N_IPEND_STATUS_SOFTLIMIT                    = 0x4000,           // Bit 14, Soft limit�� ���� ����
        N_IPEND_STATUS_INTERPOLATION_DRIVE          = 0x8000            // Bit 15, ���� ����̺꿡 ���� ����.
    } ANT_MOTION_IPEND_STATUS;
#endif

#ifndef ANT_MOTION_IPDRIVE_STATUS_DEF
    #define ANT_MOTION_IPDRIVE_STATUS_DEF
    /* IP Drive status                        */
    typedef    enum _ANT_MOTION_IPDRIVE_STATUS
    {
        N_IPDRIVE_STATUS_BUSY                        = 0x00001,         // Bit 0, BUSY(����̺� ���� ��)
        N_IPDRIVE_STATUS_DOWN                        = 0x00002,         // Bit 1, DOWN(���� ��)
        N_IPDRIVE_STATUS_CONST                       = 0x00004,         // Bit 2, CONST(��� ��)
        N_IPDRIVE_STATUS_UP                          = 0x00008,         // Bit 3, UP(���� ��)
        N_IPDRIVE_STATUS_ICL                         = 0x00010,         // Bit 4, ICL(���� ��ġ ī���� < ���� ��ġ ī���� �񱳰�)
        N_IPDRIVE_STATUS_ICG                         = 0x00020,         // Bit 5, ICG(���� ��ġ ī���� > ���� ��ġ ī���� �񱳰�)
        N_IPDRIVE_STATUS_ECL                         = 0x00040,         // Bit 6, ECL(�ܺ� ��ġ ī���� < �ܺ� ��ġ ī���� �񱳰�)
        N_IPDRIVE_STATUS_ECG                         = 0x00080,         // Bit 7, ECG(�ܺ� ��ġ ī���� > �ܺ� ��ġ ī���� �񱳰�)
        N_IPDRIVE_STATUS_DRIVE_DIRECTION             = 0x00100,         // Bit 8, ����̺� ���� ��ȣ(0=CW/1=CCW)
        N_IPDRIVE_STATUS_COMMAND_BUSY                = 0x00200,         // Bit 9, ��ɾ� ������
        N_IPDRIVE_STATUS_PRESET_DRIVING              = 0x00400,         // Bit 10, Preset pulse drive ��
        N_IPDRIVE_STATUS_CONTINUOUS_DRIVING          = 0x00800,         // Bit 11, Continuouse speed drive ��
        N_IPDRIVE_STATUS_SIGNAL_SEARCH_DRIVING       = 0x01000,         // Bit 12, Signal search-1/2 drive ��
        N_IPDRIVE_STATUS_ORG_SEARCH_DRIVING          = 0x02000,         // Bit 13, ���� ���� drive ��
        N_IPDRIVE_STATUS_MPG_DRIVING                 = 0x04000,         // Bit 14, MPG drive ��
        N_IPDRIVE_STATUS_SENSOR_DRIVING              = 0x08000,         // Bit 15, Sensor positioning drive ��
        N_IPDRIVE_STATUS_L_C_INTERPOLATION           = 0x10000,         // Bit 16, ����/��ȣ ���� ��
        N_IPDRIVE_STATUS_PATTERN_INTERPOLATION       = 0x20000,         // Bit 17, ��Ʈ ���� ���� ��
        N_IPDRIVE_STATUS_INTERRUPT_BANK1             = 0x40000,         // Bit 18, ���ͷ�Ʈ bank1���� �߻�
        N_IPDRIVE_STATUS_INTERRUPT_BANK2             = 0x80000          // Bit 19, ���ͷ�Ʈ bank2���� �߻�
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

        N_IPMECHANICAL_PELM_LEVEL                   = 0x0001,           // Bit 0, +Limit ������ ��ȣ�� ��Ƽ�� ��
        N_IPMECHANICAL_NELM_LEVEL                   = 0x0002,           // Bit 1, -Limit ������ ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_PSLM_LEVEL                   = 0x0004,           // Bit 2, +limit �������� ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_NSLM_LEVEL                   = 0x0008,           // Bit 3, -limit �������� ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_ALARM_LEVEL                  = 0x0010,           // Bit 4, Alarm ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_INP_LEVEL                    = 0x0020,           // Bit 5, Inposition ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_ENC_DOWN_LEVEL               = 0x0040,           // Bit 6, ���ڴ� DOWN(B��) ��ȣ �Է� Level
        N_IPMECHANICAL_ENC_UP_LEVEL                 = 0x0080,           // Bit 7, ���ڴ� UP(A��) ��ȣ �Է� Level
        N_IPMECHANICAL_EXMP_LEVEL                   = 0x0100,           // Bit 8, EXMP ��ȣ �Է� Level
        N_IPMECHANICAL_EXPP_LEVEL                   = 0x0200,           // Bit 9, EXPP ��ȣ �Է� Level
        N_IPMECHANICAL_MARK_LEVEL                   = 0x0400,           // Bit 10, MARK# ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_SSTOP_LEVEL                  = 0x0800,           // Bit 11, SSTOP ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_ESTOP_LEVEL                  = 0x1000,           // Bit 12, ESTOP ��ȣ ��Ƽ�� ��
        N_IPMECHANICAL_SYNC_LEVEL                   = 0x2000,           // Bit 13, SYNC ��ȣ �Է� Level
        N_IPMECHANICAL_MODE8_16_LEVEL               = 0x4000            // Bit 14, MODE8_16 ��ȣ �Է� Level

    } ANT_MOTION_IPMECHANICAL_SIGNAL;
#endif

#ifndef ANT_MOTION_QIDETECT_DESTINATION_SIGNAL_DEF
    #define ANT_MOTION_QIDETECT_DESTINATION_SIGNAL_DEF
    /* Detect Destination Signal            */
    typedef enum _ANT_MOTION_QIDETECT_DESTINATION_SIGNAL
    {
        N_Signal_PosEndLimit                        = 0x0,              // +Elm(End limit) +���� ����Ʈ ���� ��ȣ
        N_Signal_NegEndLimit                        = 0x1,              // -Elm(End limit) -���� ����Ʈ ���� ��ȣ
        N_Signal_PosSloLimit                        = 0x2,              // +Slm(Slow Down limit) ��ȣ - ������� ����
        N_Signal_NegSloLimit                        = 0x3,              // -Slm(Slow Down limit) ��ȣ - ������� ����
        N_Signal_HomeSensor                         = 0x4,              // IN0(ORG)  ���� ���� ��ȣ
        N_Signal_EncodZPhase                        = 0x5,              // IN1(Z��)  Encoder Z�� ��ȣ
        N_Signal_UniInput02                         = 0x6,              // IN2(����) ���� �Է� 2�� ��ȣ
        N_Signal_UniInput03                         = 0x7               // IN3(����) ���� �Է� 3�� ��ȣ
    } ANT_MOTION_QIDETECT_DESTINATION_SIGNAL;
#endif

#ifndef ANT_MOTION_QIMECHANICAL_SIGNAL_DEF
    #define ANT_MOTION_QIMECHANICAL_SIGNAL_DEF
    /* QI Drive status                        */
    typedef    enum _ANT_MOTION_QIMECHANICAL_SIGNAL
    {

        N_QIMECHANICAL_PELM_LEVEL                   = 0x00001,          // Bit 0, +Limit ������ ��ȣ ���� ����
        N_QIMECHANICAL_NELM_LEVEL                   = 0x00002,          // Bit 1, -Limit ������ ��ȣ ���� ����
        N_QIMECHANICAL_PSLM_LEVEL                   = 0x00004,          // Bit 2, +limit �������� ���� ����.
        N_QIMECHANICAL_NSLM_LEVEL                   = 0x00008,          // Bit 3, -limit �������� ���� ����
        N_QIMECHANICAL_ALARM_LEVEL                  = 0x00010,          // Bit 4, Alarm ��ȣ ���� ����
        N_QIMECHANICAL_INP_LEVEL                    = 0x00020,          // Bit 5, Inposition ��ȣ ���� ����
        N_QIMECHANICAL_ESTOP_LEVEL                  = 0x00040,          // Bit 6, ��� ���� ��ȣ(ESTOP) ���� ����.
        N_QIMECHANICAL_ORG_LEVEL                    = 0x00080,          // Bit 7, ���� ��ȣ ���� ����
        N_QIMECHANICAL_ZPHASE_LEVEL                 = 0x00100,          // Bit 8, Z �� �Է� ��ȣ ���� ����
        N_QIMECHANICAL_ECUP_LEVEL                   = 0x00200,          // Bit 9, ECUP �͹̳� ��ȣ ����.
        N_QIMECHANICAL_ECDN_LEVEL                   = 0x00400,          // Bit 10, ECDN �͹̳� ��ȣ ����.
        N_QIMECHANICAL_EXPP_LEVEL                   = 0x00800,          // Bit 11, EXPP �͹̳� ��ȣ ����
        N_QIMECHANICAL_EXMP_LEVEL                   = 0x01000,          // Bit 12, EXMP �͹̳� ��ȣ ����
        N_QIMECHANICAL_SQSTR1_LEVEL                 = 0x02000,          // Bit 13, SQSTR1 �͹̳� ��ȣ ����
        N_QIMECHANICAL_SQSTR2_LEVEL                 = 0x04000,          // Bit 14, SQSTR2 �͹̳� ��ȣ ����
        N_QIMECHANICAL_SQSTP1_LEVEL                 = 0x08000,          // Bit 15, SQSTP1 �͹̳� ��ȣ ����
        N_QIMECHANICAL_SQSTP2_LEVEL                 = 0x10000,          // Bit 16, SQSTP2 �͹̳� ��ȣ ����
        N_QIMECHANICAL_MODE_LEVEL                   = 0x20000           // Bit 17, MODE �͹̳� ��ȣ ����.
    } ANT_MOTION_QIMECHANICAL_SIGNAL;
#endif

#ifndef ANT_MOTION_QIEND_STATUS_DEF
    #define ANT_MOTION_QIEND_STATUS_DEF
    /* QI End status : 0x0000�̸� ��������    */
    typedef    enum _ANT_MOTION_QIEND_STATUS
    {
        N_QIEND_STATUS_0                            = 0x00000001,       // Bit 0, ������ ����Ʈ ��ȣ(PELM)�� ���� ����
        N_QIEND_STATUS_1                            = 0x00000002,       // Bit 1, ������ ����Ʈ ��ȣ(NELM)�� ���� ����
        N_QIEND_STATUS_2                            = 0x00000004,       // Bit 2, ������ �ΰ� ����Ʈ ��ȣ(PSLM)�� ���� ���� ����
        N_QIEND_STATUS_3                            = 0x00000008,       // Bit 3, ������ �ΰ� ����Ʈ ��ȣ(NSLM)�� ���� ���� ����
        N_QIEND_STATUS_4                            = 0x00000010,       // Bit 4, ������ ����Ʈ ����Ʈ ������ ��ɿ� ���� ���� ����
        N_QIEND_STATUS_5                            = 0x00000020,       // Bit 5, ������ ����Ʈ ����Ʈ ������ ��ɿ� ���� ���� ����
        N_QIEND_STATUS_6                            = 0x00000040,       // Bit 6, ������ ����Ʈ ����Ʈ �������� ��ɿ� ���� ���� ����
        N_QIEND_STATUS_7                            = 0x00000080,       // Bit 7, ������ ����Ʈ ����Ʈ �������� ��ɿ� ���� ���� ����
        N_QIEND_STATUS_8                            = 0x00000100,       // Bit 8, ���� �˶� ��ɿ� ���� ���� ����.
        N_QIEND_STATUS_9                            = 0x00000200,       // Bit 9, ��� ���� ��ȣ �Է¿� ���� ���� ����.
        N_QIEND_STATUS_10                           = 0x00000400,       // Bit 10, �� ���� ��ɿ� ���� ���� ����.
        N_QIEND_STATUS_11                           = 0x00000800,       // Bit 11, ���� ���� ��ɿ� ���� ���� ����.
        N_QIEND_STATUS_12                           = 0x00001000,       // Bit 12, ���� ������ ��ɿ� ���� ���� ����
        N_QIEND_STATUS_13                           = 0x00002000,       // Bit 13, ���� ���� ��� #1(SQSTP1)�� ���� ���� ����.
        N_QIEND_STATUS_14                           = 0x00004000,       // Bit 14, ���� ���� ��� #2(SQSTP2)�� ���� ���� ����.
        N_QIEND_STATUS_15                           = 0x00008000,       // Bit 15, ���ڴ� �Է�(ECUP,ECDN) ���� �߻�
        N_QIEND_STATUS_16                           = 0x00010000,       // Bit 16, MPG �Է�(EXPP,EXMP) ���� �߻�
        N_QIEND_STATUS_17                           = 0x00020000,       // Bit 17, ���� �˻� ���� ����.
        N_QIEND_STATUS_18                           = 0x00040000,       // Bit 18, ��ȣ �˻� ���� ����.
        N_QIEND_STATUS_19                           = 0x00080000,       // Bit 19, ���� ������ �̻����� ���� ����.
        N_QIEND_STATUS_20                           = 0x00100000,       // Bit 20, ������ ���� �����߻�.
        N_QIEND_STATUS_21                           = 0x00200000,       // Bit 21, MPG ��� ��� �޽� ���� �����÷ο� �߻�
        N_QIEND_STATUS_22                           = 0x00400000,       // Bit 22, DON'CARE
        N_QIEND_STATUS_23                           = 0x00800000,       // Bit 23, DON'CARE
        N_QIEND_STATUS_24                           = 0x01000000,       // Bit 24, DON'CARE
        N_QIEND_STATUS_25                           = 0x02000000,       // Bit 25, DON'CARE
        N_QIEND_STATUS_26                           = 0x04000000,       // Bit 26, DON'CARE
        N_QIEND_STATUS_27                           = 0x08000000,       // Bit 27, DON'CARE
        N_QIEND_STATUS_28                           = 0x10000000,       // Bit 28, ����/������ ���� ����̺� ����
        N_QIEND_STATUS_29                           = 0x20000000,       // Bit 29, �ܿ� �޽� ���� ��ȣ ��� ��.
        N_QIEND_STATUS_30                           = 0x40000000,       // Bit 30, ������ ���� ���� ���� ����
        N_QIEND_STATUS_31                           = 0x80000000        // Bit 31, ���� ����̺� ����Ÿ ���� ����.
    } ANT_MOTION_QIEND_STATUS;
#endif

#ifndef ANT_MOTION_QIDRIVE_STATUS_DEF
    #define ANT_MOTION_QIDRIVE_STATUS_DEF
    /* QI Drive status                        */
    typedef    enum _ANT_MOTION_QIDRIVE_STATUS
    {
        N_QIDRIVE_STATUS_0                          = 0x0000001,        // Bit 0, BUSY(����̺� ���� ��)
        N_QIDRIVE_STATUS_1                          = 0x0000002,        // Bit 1, DOWN(���� ��)
        N_QIDRIVE_STATUS_2                          = 0x0000004,        // Bit 2, CONST(��� ��)
        N_QIDRIVE_STATUS_3                          = 0x0000008,        // Bit 3, UP(���� ��)
        N_QIDRIVE_STATUS_4                          = 0x0000010,        // Bit 4, ���� ����̺� ���� ��
        N_QIDRIVE_STATUS_5                          = 0x0000020,        // Bit 5, ���� �Ÿ� ����̺� ���� ��
        N_QIDRIVE_STATUS_6                          = 0x0000040,        // Bit 6, MPG ����̺� ���� ��
        N_QIDRIVE_STATUS_7                          = 0x0000080,        // Bit 7, �����˻� ����̺� ������ 
//���ǻ��� : (���� ���̺귯�� ���� ������ HOME�Լ��� ������� ���۵ȴ�.)
        N_QIDRIVE_STATUS_8                          = 0x0000100,        // Bit 8, ��ȣ �˻� ����̺� ���� ��
//���ǻ��� : (���� ���̺귯�� ���� ������ HOME�Լ��� ���� ���۵ȴ�. Ȩ�Լ��� ��ȣ��ġ�Լ��� ����������Ͽ� ������ �Ǿ����� Ȩ �˻��� BIT: 1 �ȴ�.)
        N_QIDRIVE_STATUS_9                          = 0x0000200,        // Bit 9, ���� ����̺� ���� ��
        N_QIDRIVE_STATUS_10                         = 0x0000400,        // Bit 10, Slave ����̺� ������
        N_QIDRIVE_STATUS_11                         = 0x0000800,        // Bit 11, ���� ���� ����̺� ����(���� ����̺꿡���� ǥ�� ���� �ٸ�)
        N_QIDRIVE_STATUS_12                         = 0x0001000,        // Bit 12, �޽� ����� ������ġ �Ϸ� ��ȣ �����.
        N_QIDRIVE_STATUS_13                         = 0x0002000,        // Bit 13, ���� ���� ����̺� ������.
        N_QIDRIVE_STATUS_14                         = 0x0004000,        // Bit 14, ��ȣ ���� ����̺� ������.
        N_QIDRIVE_STATUS_15                         = 0x0008000,        // Bit 15, �޽� ��� ��.
        N_QIDRIVE_STATUS_16                         = 0x0010000,        // Bit 16, ���� ���� ������ ����(ó��)(0-7)
        N_QIDRIVE_STATUS_17                         = 0x0020000,        // Bit 17, ���� ���� ������ ����(�߰�)(0-7)
        N_QIDRIVE_STATUS_18                         = 0x0040000,        // Bit 18, ���� ���� ������ ����(��)(0-7)
        N_QIDRIVE_STATUS_19                         = 0x0100000,        // Bit 19, ���� ���� Queue ��� ����.
        N_QIDRIVE_STATUS_20                         = 0x0200000,        // Bit 20, ���� ���� Queue ���� �H
        N_QIDRIVE_STATUS_21                         = 0x0400000,        // Bit 21, ���� ���� ����̺��� �ӵ� ���(ó��)
        N_QIDRIVE_STATUS_22                         = 0x0800000,        // Bit 22, ���� ���� ����̺��� �ӵ� ���(��)
        N_QIDRIVE_STATUS_23                         = 0x1000000,        // Bit 23, MPG ���� #1 Full
        N_QIDRIVE_STATUS_24                         = 0x2000000,        // Bit 24, MPG ���� #2 Full
        N_QIDRIVE_STATUS_25                         = 0x4000000,        // Bit 25, MPG ���� #3 Full
        N_QIDRIVE_STATUS_26                         = 0x8000000         // Bit 26, MPG ���� ������ OverFlow

    } ANT_MOTION_QIDRIVE_STATUS;
#endif

#ifndef ANT_MOTION_QIINTERRUPT_BANK1_DEF
    #define ANT_MOTION_QIINTERRUPT_BANK1_DEF
    /* QI Interrupt MASK1 setting            */
    typedef    enum _ANT_MOTION_QIINTERRUPT_BANK1
    {
        N_QIINTBANK1_DISABLE                        = 0x00000000,       // INTERRUT DISABLED.
        N_QIINTBANK1_0                              = 0x00000001,       // Bit 0,  ���ͷ�Ʈ �߻� ��� ������ ���� �����.
        N_QIINTBANK1_1                              = 0x00000002,       // Bit 1,  ���� �����
        N_QIINTBANK1_2                              = 0x00000004,       // Bit 2,  ���� ���۽�.
        N_QIINTBANK1_3                              = 0x00000008,       // Bit 3,  ī���� #1 < �񱳱� #1 �̺�Ʈ �߻�
        N_QIINTBANK1_4                              = 0x00000010,       // Bit 4,  ī���� #1 = �񱳱� #1 �̺�Ʈ �߻�
        N_QIINTBANK1_5                              = 0x00000020,       // Bit 5,  ī���� #1 > �񱳱� #1 �̺�Ʈ �߻�
        N_QIINTBANK1_6                              = 0x00000040,       // Bit 6,  ī���� #2 < �񱳱� #2 �̺�Ʈ �߻�
        N_QIINTBANK1_7                              = 0x00000080,       // Bit 7,  ī���� #2 = �񱳱� #2 �̺�Ʈ �߻�
        N_QIINTBANK1_8                              = 0x00000100,       // Bit 8,  ī���� #2 > �񱳱� #2 �̺�Ʈ �߻�
        N_QIINTBANK1_9                              = 0x00000200,       // Bit 9,  ī���� #3 < �񱳱� #3 �̺�Ʈ �߻�
        N_QIINTBANK1_10                             = 0x00000400,       // Bit 10, ī���� #3 = �񱳱� #3 �̺�Ʈ �߻�
        N_QIINTBANK1_11                             = 0x00000800,       // Bit 11, ī���� #3 > �񱳱� #3 �̺�Ʈ �߻�
        N_QIINTBANK1_12                             = 0x00001000,       // Bit 12, ī���� #4 < �񱳱� #4 �̺�Ʈ �߻�
        N_QIINTBANK1_13                             = 0x00002000,       // Bit 13, ī���� #4 = �񱳱� #4 �̺�Ʈ �߻�
        N_QIINTBANK1_14                             = 0x00004000,       // Bit 14, ī���� #4 < �񱳱� #4 �̺�Ʈ �߻�
        N_QIINTBANK1_15                             = 0x00008000,       // Bit 15, ī���� #5 < �񱳱� #5 �̺�Ʈ �߻�
        N_QIINTBANK1_16                             = 0x00010000,       // Bit 16, ī���� #5 = �񱳱� #5 �̺�Ʈ �߻�
        N_QIINTBANK1_17                             = 0x00020000,       // Bit 17, ī���� #5 > �񱳱� #5 �̺�Ʈ �߻�
        N_QIINTBANK1_18                             = 0x00040000,       // Bit 18, Ÿ�̸� #1 �̺�Ʈ �߻�.
        N_QIINTBANK1_19                             = 0x00080000,       // Bit 19, Ÿ�̸� #2 �̺�Ʈ �߻�.
        N_QIINTBANK1_20                             = 0x00100000,       // Bit 20, ���� ���� ���� Queue �����.
        N_QIINTBANK1_21                             = 0x00200000,       // Bit 21, ���� ���� ���� Queue ����H
        N_QIINTBANK1_22                             = 0x00400000,       // Bit 22, Ʈ���� �߻��Ÿ� �ֱ�/������ġ Queue �����.
        N_QIINTBANK1_23                             = 0x00800000,       // Bit 23, Ʈ���� �߻��Ÿ� �ֱ�/������ġ Queue ����H
        N_QIINTBANK1_24                             = 0x01000000,       // Bit 24, Ʈ���� ��ȣ �߻� �̺�Ʈ
        N_QIINTBANK1_25                             = 0x02000000,       // Bit 25, ��ũ��Ʈ #1 ��ɾ� ���� ���� Queue �����.
        N_QIINTBANK1_26                             = 0x04000000,       // Bit 26, ��ũ��Ʈ #2 ��ɾ� ���� ���� Queue �����.
        N_QIINTBANK1_27                             = 0x08000000,       // Bit 27, ��ũ��Ʈ #3 ��ɾ� ���� ���� �������� ����Ǿ� �ʱ�ȭ ��.
        N_QIINTBANK1_28                             = 0x10000000,       // Bit 28, ��ũ��Ʈ #4 ��ɾ� ���� ���� �������� ����Ǿ� �ʱ�ȭ ��.
        N_QIINTBANK1_29                             = 0x20000000,       // Bit 29, ���� �˶���ȣ �ΰ���.
        N_QIINTBANK1_30                             = 0x40000000,       // Bit 30, |CNT1| - |CNT2| >= |CNT4| �̺�Ʈ �߻�.
        N_QIINTBANK1_31                             = 0x80000000        // Bit 31, ���ͷ�Ʈ �߻� ��ɾ�|INTGEN| ����.
    } ANT_MOTION_QIINTERRUPT_BANK1;
#endif

#ifndef ANT_MOTION_QIINTERRUPT_BANK2_DEF
    #define ANT_MOTION_QIINTERRUPT_BANK2_DEF
	/* QI Interrupt MASK2 setting            */
    typedef    enum _ANT_MOTION_QIINTERRUPT_BANK2
    {
        N_QIINTBANK2_DISABLE                        = 0x00000000,       // INTERRUT DISABLED.
        N_QIINTBANK2_0                              = 0x00000001,       // Bit 0,  ��ũ��Ʈ #1 �б� ��� ��� Queue �� ����H.
        N_QIINTBANK2_1                              = 0x00000002,       // Bit 1,  ��ũ��Ʈ #2 �б� ��� ��� Queue �� ����H.
        N_QIINTBANK2_2                              = 0x00000004,       // Bit 2,  ��ũ��Ʈ #3 �б� ��� ��� �������Ͱ� ���ο� �����ͷ� ���ŵ�.
        N_QIINTBANK2_3                              = 0x00000008,       // Bit 3,  ��ũ��Ʈ #4 �б� ��� ��� �������Ͱ� ���ο� �����ͷ� ���ŵ�.
        N_QIINTBANK2_4                              = 0x00000010,       // Bit 4,  ��ũ��Ʈ #1 �� ���� ��ɾ� �� ���� �� ���ͷ�Ʈ �߻����� ������ ��ɾ� �����.
        N_QIINTBANK2_5                              = 0x00000020,       // Bit 5,  ��ũ��Ʈ #2 �� ���� ��ɾ� �� ���� �� ���ͷ�Ʈ �߻����� ������ ��ɾ� �����.
        N_QIINTBANK2_6                              = 0x00000040,       // Bit 6,  ��ũ��Ʈ #3 �� ���� ��ɾ� ���� �� ���ͷ�Ʈ �߻����� ������ ��ɾ� �����.
        N_QIINTBANK2_7                              = 0x00000080,       // Bit 7,  ��ũ��Ʈ #4 �� ���� ��ɾ� ���� �� ���ͷ�Ʈ �߻����� ������ ��ɾ� �����.
        N_QIINTBANK2_8                              = 0x00000100,       // Bit 8,  ���� ����
        N_QIINTBANK2_9                              = 0x00000200,       // Bit 9,  ���� ��ġ ���� �Ϸ�(Inposition)����� ����� ����,���� ���� �߻�.
        N_QIINTBANK2_10                             = 0x00000400,       // Bit 10, �̺�Ʈ ī���ͷ� ���� �� ����� �̺�Ʈ ���� #1 ���� �߻�.
        N_QIINTBANK2_11                             = 0x00000800,       // Bit 11, �̺�Ʈ ī���ͷ� ���� �� ����� �̺�Ʈ ���� #2 ���� �߻�.
        N_QIINTBANK2_12                             = 0x00001000,       // Bit 12, SQSTR1 ��ȣ �ΰ� ��.
        N_QIINTBANK2_13                             = 0x00002000,       // Bit 13, SQSTR2 ��ȣ �ΰ� ��.
        N_QIINTBANK2_14                             = 0x00004000,       // Bit 14, UIO0 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_15                             = 0x00008000,       // Bit 15, UIO1 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_16                             = 0x00010000,       // Bit 16, UIO2 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_17                             = 0x00020000,       // Bit 17, UIO3 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_18                             = 0x00040000,       // Bit 18, UIO4 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_19                             = 0x00080000,       // Bit 19, UIO5 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_20                             = 0x00100000,       // Bit 20, UIO6 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_21                             = 0x00200000,       // Bit 21, UIO7 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_22                             = 0x00400000,       // Bit 22, UIO8 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_23                             = 0x00800000,       // Bit 23, UIO9 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_24                             = 0x01000000,       // Bit 24, UIO10 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_25                             = 0x02000000,       // Bit 25, UIO11 �͹̳� ��ȣ�� '1'�� ����.
        N_QIINTBANK2_26                             = 0x04000000,       // Bit 26, ���� ���� ����(LMT, ESTOP, STOP, ESTOP, CMD, ALARM) �߻�.
        N_QIINTBANK2_27                             = 0x08000000,       // Bit 27, ���� �� ������ ���� ���� �߻�.
        N_QIINTBANK2_28                             = 0x10000000,       // Bit 28, Don't Care
        N_QIINTBANK2_29                             = 0x20000000,       // Bit 29, ����Ʈ ��ȣ(PELM, NELM)��ȣ�� �Է� ��.
        N_QIINTBANK2_30                             = 0x40000000,       // Bit 30, �ΰ� ����Ʈ ��ȣ(PSLM, NSLM)��ȣ�� �Է� ��.
        N_QIINTBANK2_31                             = 0x80000000        // Bit 31, ��� ���� ��ȣ(ESTOP)��ȣ�� �Էµ�.

    } ANT_MOTION_QIINTERRUPT_BANK2;
#endif

typedef void (__stdcall *ANT_EVENT_PROC)(long lActiveNo, DWORD uFlag);
typedef void (__stdcall *ANT_INTERRUPT_PROC)(long lActiveNo, DWORD uFlag);
typedef void (__stdcall *ANT_NETMON_PROC)(long lActiveNo, DWORD uFlag);

#endif    //__ANT_AXHS_H__
