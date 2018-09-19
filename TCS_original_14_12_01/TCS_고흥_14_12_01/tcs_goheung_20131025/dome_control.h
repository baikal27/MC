/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2009. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAIN                            1
#define  MAIN_GOAL                       2       /* callback function: DOME_CBF */
#define  MAIN_ERROR                      3
#define  MAIN_AZI                        4
#define  MAIN_TIMERINTERVAL              5       /* callback function: DOME_CBF */
#define  MAIN_ENCODER                    6       /* callback function: DOME_CBF */
#define  MAIN_Dome_CW_2                  7
#define  MAIN_COMMANDBUTTON_20           8
#define  MAIN_COMMANDBUTTON_16           9
#define  MAIN_COMMANDBUTTON_15           10
#define  MAIN_COMMANDBUTTON_14           11
#define  MAIN_LAMP_OFF                   12      /* callback function: DOME_CBF */
#define  MAIN_LAMP_ON                    13      /* callback function: DOME_CBF */
#define  MAIN_DS_STOP                    14      /* callback function: DOME_CBF */
#define  MAIN_DS_CLOSE                   15      /* callback function: DOME_CBF */
#define  MAIN_DS_OPEN                    16      /* callback function: DOME_CBF */
#define  MAIN_DOME_STOP                  17      /* callback function: DOME_CBF */
#define  MAIN_DOME_CCW                   18      /* callback function: DOME_CBF */
#define  MAIN_DOME_CW                    19      /* callback function: DOME_CBF */
#define  MAIN_US_STOP                    20      /* callback function: DOME_CBF */
#define  MAIN_US_CLOSE                   21      /* callback function: DOME_CBF */
#define  MAIN_US_OPEN                    22      /* callback function: DOME_CBF */
#define  MAIN_PARKING                    23      /* callback function: DOME_CBF */
#define  MAIN_LED_DS_CLOSE               24
#define  MAIN_LED_LAMP_OFF               25
#define  MAIN_LED_LAMP_ON                26
#define  MAIN_LED_DS_OPEN                27
#define  MAIN_DOME_ONOFF                 28      /* callback function: DOME_CBF */
#define  MAIN_LED_US_CLOSE               29
#define  MAIN_LED_US_OPEN                30
#define  MAIN_DOME_POSITION              31
#define  MAIN_COMMANDBUTTON_24           32
#define  MAIN_COMMANDBUTTON_23           33
#define  MAIN_COMMANDBUTTON_22           34
#define  MAIN_COMMANDBUTTON_21           35
#define  MAIN_LED_CCW                    36
#define  MAIN_LED_HOME                   37
#define  MAIN_LED_STOP                   38
#define  MAIN_LED_CW                     39
#define  MAIN_LED                        40
#define  MAIN_QUIT                       41      /* callback function: DOME_CBF */
#define  MAIN_OFFSET                     42      /* callback function: DOME_CBF */
#define  MAIN_GO                         43      /* callback function: DOME_CBF */
#define  MAIN_LOOPSYNC                   44      /* callback function: DOME_CBF */
#define  MAIN_LOOP                       45      /* callback function: DOME_CBF */
#define  MAIN_DECORATION_5               46
#define  MAIN_DECORATION_7               47
#define  MAIN_DECORATION_8               48
#define  MAIN_DECORATION_9               49
#define  MAIN_DECORATION_10              50
#define  MAIN_PARKING_TIMER              51      /* callback function: PARKING_TIMER_CALLBACK */
#define  MAIN_DECORATION                 52
#define  MAIN_DECORATION_11              53
#define  MAIN_DECORATION_12              54
#define  MAIN_TIMER                      55      /* callback function: DOME_TIMER_CALLBACK */

#define  PARKING                         2
#define  PARKING_STOP                    2       /* callback function: PARKING_CALLBACK */
#define  PARKING_QUIT                    3       /* callback function: PARKING_CALLBACK */
#define  PARKING_BEGIN                   4       /* callback function: PARKING_CALLBACK */
#define  PARKING_LED                     5
#define  PARKING_RING                    6
#define  PARKING_TEXTMSG                 7


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK DOME_CBF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DOME_TIMER_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PARKING_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PARKING_TIMER_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
