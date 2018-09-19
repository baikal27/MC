/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2010. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: Panel_proc */
#define  PANEL_OPENDEVICE                 2       /* callback function: BASEBUTTON */
#define  PANEL_cmdInit                    3       /* callback function: BASEBUTTON */
#define  PANEL_strOutput                  4
#define  PANEL_STOP                       5       /* callback function: BASEBUTTON */
#define  PANEL_cmdExit                    6       /* callback function: BASEBUTTON */
#define  PANEL_MOVEACCE                   7       /* callback function: BASEBUTTON */
#define  PANEL_MOVESLEW                   8       /* callback function: BASEBUTTON */
#define  PANEL_numThreadfFunction2        9
#define  PANEL_numThreadfFunction1        10
#define  PANEL_UO0                        11
#define  PANEL_UI4                        12
#define  PANEL_UI3                        13
#define  PANEL_UI2                        14
#define  PANEL_UI1                        15
#define  PANEL_UI0                        16
#define  PANEL_CCWLIMIT                   17
#define  PANEL_HOME                       18
#define  PANEL_CWLIMIT                    19
#define  PANEL_ENCODER1                   20
#define  PANEL_COUNT1                     21
#define  PANEL_AXIS                       22
#define  PANEL_COUNTER                    23
#define  PANEL_COUNTER_2                  24
#define  PANEL_COUNTER_3                  25
#define  PANEL_COUNTER_4                  26
#define  PANEL_COUNTER_5                  27
#define  PANEL_NUMERIC_2                  28
#define  PANEL_TOGGLEBUTTON_2             29
#define  PANEL_TOGGLEBUTTON               30
#define  PANEL_TEXTBOX                    31
#define  PANEL_VELOCITY                   32
#define  PANEL_ENCODER                    33      /* callback function: BASEBUTTON */
#define  PANEL_MODE                       34
#define  PANEL_COUNT0                     35
#define  PANEL_COUNT                      36      /* callback function: BASEBUTTON */
#define  PANEL_VELO                       37
#define  PANEL_STEPS                      38      /* callback function: BASEBUTTON */
#define  PANEL_DIR                        39
#define  PANEL_TIMERONOFF                 40      /* callback function: BASEBUTTON */
#define  PANEL_TIMER                      41      /* callback function: TIMERCB */


     /* Menu Bars, Menus, and Menu Items: */

#define  Main                             1
#define  Main_File                        2
#define  Main_File_Control_test           3
#define  Main_File_SEPARATOR              4
#define  Main_File_Exit                   5
#define  Main_menuTheSky                  6
#define  Main_menuTheSky_menuConnect      7       /* callback function: menuBar */
#define  Main_menuTheSky_menuDisConnect   8       /* callback function: menuBar */
#define  Main_Help                        9
#define  Main_Help_Help_About             10


     /* Callback Prototypes: */ 

int  CVICALLBACK BASEBUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK menuBar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Panel_proc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMERCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
