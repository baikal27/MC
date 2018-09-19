/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2013. All Rights Reserved.          */
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
#define  PANEL_CLEARC                     2       /* callback function: BASEBUTTON */
#define  PANEL_CONNECT                    3       /* callback function: BASEBUTTON */
#define  PANEL_cmdInit                    4       /* callback function: BASEBUTTON */
#define  PANEL_INIT                       5       /* callback function: BASEBUTTON */
#define  PANEL_PARK                       6       /* callback function: BASEBUTTON */
#define  PANEL_STOP                       7       /* callback function: BASEBUTTON */
#define  PANEL_MOVEACCE                   8       /* callback function: BASEBUTTON */
#define  PANEL_MOVESLEW                   9       /* callback function: BASEBUTTON */
#define  PANEL_UI9                        10
#define  PANEL_UI8                        11
#define  PANEL_UI7                        12
#define  PANEL_UI6                        13
#define  PANEL_UI5                        14
#define  PANEL_UI4                        15
#define  PANEL_UI3                        16
#define  PANEL_UI2                        17
#define  PANEL_UI1                        18
#define  PANEL_UI0                        19
#define  PANEL_CCWX                       20
#define  PANEL_HOMEX                      21
#define  PANEL_CWX                        22
#define  PANEL_CCWB                       23
#define  PANEL_HOMEB                      24
#define  PANEL_CWB                        25
#define  PANEL_CCWA                       26
#define  PANEL_HOMEA                      27
#define  PANEL_CWA                        28
#define  PANEL_COUNTER_7                  29
#define  PANEL_COUNTER_6                  30
#define  PANEL_COUNTER_2                  31
#define  PANEL_COUNTER_3                  32
#define  PANEL_COUNTER_4                  33
#define  PANEL_MODEX                      34
#define  PANEL_TEXTBOX                    35      /* callback function: TEXTAPPLY */
#define  PANEL_COUNTX                     36      /* callback function: BASEBUTTON */
#define  PANEL_VELOCITYA                  37
#define  PANEL_ENCODERA                   38      /* callback function: BASEBUTTON */
#define  PANEL_MODEB                      39
#define  PANEL_DIRX                       40
#define  PANEL_MODEA                      41
#define  PANEL_COUNTB                     42      /* callback function: BASEBUTTON */
#define  PANEL_COUNTA                     43      /* callback function: BASEBUTTON */
#define  PANEL_VELOCITYX                  44
#define  PANEL_ENCODERX                   45      /* callback function: BASEBUTTON */
#define  PANEL_VELO                       46
#define  PANEL_STEPS                      47      /* callback function: BASEBUTTON */
#define  PANEL_DIRB                       48
#define  PANEL_DIRA                       49
#define  PANEL_TIMERONOFF                 50      /* callback function: BASEBUTTON */
#define  PANEL_TIMER                      51      /* callback function: TIMERCB */
#define  PANEL_VELOCITYB                  52
#define  PANEL_ENCODERB                   53      /* callback function: BASEBUTTON */
#define  PANEL_RING2                      54
#define  PANEL_RING                       55
#define  PANEL_DECORATION                 56
#define  PANEL_DECORATION_2               57
#define  PANEL_DECORATION_3               58
#define  PANEL_U9                         59      /* callback function: BASEBUTTON */
#define  PANEL_U8                         60      /* callback function: BASEBUTTON */
#define  PANEL_NT1                        61
#define  PANEL_DECORATION_4               62
#define  PANEL_NODENO                     63      /* callback function: BASEBUTTON */
#define  PANEL_COUNTER_8                  64


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

#define  MBFILE                           2
#define  MBFILE_File                      2
#define  MBFILE_File_UTIL                 3       /* callback function: MBCB */
#define  MBFILE_File_READINPUTS           4       /* callback function: MBCB */
#define  MBFILE_File_SCAN                 5       /* callback function: MBCB */
#define  MBFILE_File_CATALOGUE            6       /* callback function: MBCB */
#define  MBFILE_File_TCS                  7       /* callback function: MBCB */
#define  MBFILE_File_DOME                 8       /* callback function: MBCB */
#define  MBFILE_File_NETWORK              9       /* callback function: MBCB */
#define  MBFILE_File_ECTEST               10      /* callback function: MBCB */
#define  MBFILE_File_THESKY               11
#define  MBFILE_File_THESKY_SUBMENU       12
#define  MBFILE_File_THESKY_CONNECT       13      /* callback function: MBCB */
#define  MBFILE_File_THESKY_DISCONNECT    14      /* callback function: MBCB */


     /* Callback Prototypes: */ 

int  CVICALLBACK BASEBUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK MBCB(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK menuBar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Panel_proc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TEXTAPPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMERCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
