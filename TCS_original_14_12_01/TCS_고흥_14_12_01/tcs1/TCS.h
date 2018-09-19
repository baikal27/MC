/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2015. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CCDOFF                           1       /* callback function: CCD_PANEL_CB */
#define  CCDOFF_SAVE                      2       /* callback function: CCD_APPLY */
#define  CCDOFF_ADJUST                    3       /* callback function: CCD_APPLY */
#define  CCDOFF_FLENGTH                   4
#define  CCDOFF_BINNING                   5
#define  CCDOFF_Y1                        6
#define  CCDOFF_X1                        7
#define  CCDOFF_YW                        8
#define  CCDOFF_XW                        9
#define  CCDOFF_YC                        10
#define  CCDOFF_XC                        11
#define  CCDOFF_DIRW                      12
#define  CCDOFF_DIRN                      13
#define  CCDOFF_TEXTMSG_3                 14
#define  CCDOFF_TEXTMSG_4                 15
#define  CCDOFF_DECORATION                16
#define  CCDOFF_DECORATION_4              17
#define  CCDOFF_TEXTMSG_5                 18
#define  CCDOFF_DECORATION_6              19
#define  CCDOFF_DECORATION_7              20
#define  CCDOFF_TEXTMSG_6                 21
#define  CCDOFF_TEXTMSG                   22

#define  PARK                             2
#define  PARK_CLOSE                       2       /* callback function: CB_PARKING */
#define  PARK_START                       3       /* callback function: CB_PARKING */
#define  PARK_DER_LED                     4
#define  PARK_DEC_LED                     5
#define  PARK_DIR_DER                     6
#define  PARK_RA_LED                      7
#define  PARK_DIR_DEC                     8
#define  PARK_DIR_RA                      9

#define  TEL                              3       /* callback function: PanelCB */
#define  TEL_AZI                          2
#define  TEL_ALT                          3
#define  TEL_DATE                         4
#define  TEL_KST                          5
#define  TEL_LAST                         6
#define  TEL_RA_INPUT                     7
#define  TEL_HA_SPEED                     8
#define  TEL_HA_ERROR                     9
#define  TEL_OFFRA                        10
#define  TEL_OFFDEC                       11
#define  TEL_DEC_SPEED                    12
#define  TEL_DOME_ERROR                   13
#define  TEL_DEC_ERROR                    14
#define  TEL_NAME_INPUT                   15
#define  TEL_DEC_INPUT                    16
#define  TEL_DEC_HOME                     17
#define  TEL_DEC_CCW                      18
#define  TEL_DEC_CW                       19
#define  TEL_RA_HOME                      20
#define  TEL_RA_CCW                       21
#define  TEL_RA_CW                        22
#define  TEL_HA                           23
#define  TEL_DEC                          24
#define  TEL_GOTO                         25      /* callback function: APPLY */
#define  TEL_PARKING                      26      /* callback function: APPLY */
#define  TEL_INITTCS                      27      /* callback function: APPLY */
#define  TEL_INIT                         28      /* callback function: APPLY */
#define  TEL_GUIDE                        29      /* callback function: APPLY */
#define  TEL_SAVEOFF                      30      /* callback function: APPLY */
#define  TEL_GOOFF                        31      /* callback function: APPLY */
#define  TEL_STOP                         32      /* callback function: APPLY */
#define  TEL_DOME_ONOFF                   33      /* callback function: APPLY */
#define  TEL_MESSAGE                      34      /* callback function: TEXTCB */
#define  TEL_MAIN_TIMER                   35      /* callback function: CB_TIMER */
#define  TEL_DECORATION_6                 36
#define  TEL_DECORATION_7                 37
#define  TEL_DECORATION_8                 38
#define  TEL_DECORATION_9                 39
#define  TEL_TEXTMSG_21                   40
#define  TEL_DECORATION_10                41
#define  TEL_TEXTMSG_23                   42
#define  TEL_AGTIMER                      43      /* callback function: AGTIMER_CB */
#define  TEL_TEXTMSG_24                   44
#define  TEL_CALCSUN                      45      /* callback function: APPLY */
#define  TEL_TEXTMSG_25                   46
#define  TEL_TEXTMSG_17                   47
#define  TEL_TEXTMSG_26                   48
#define  TEL_DOME_TEXT                    49
#define  TEL_TEXTMSG_12                   50
#define  TEL_TEXTMSG_19                   51
#define  TEL_TEXTMSG_13                   52
#define  TEL_TEXTMSG_11                   53
#define  TEL_TEXTMSG                      54
#define  TEL_TEXTMSG_14                   55
#define  TEL_TEXTMSG_28                   56
#define  TEL_TEXTMSG_2                    57
#define  TEL_TEXTMSG_18                   58
#define  TEL_TEXTMSG_7                    59
#define  TEL_DECORATION_11                60
#define  TEL_DECORATION_12                61
#define  TEL_TEXTMSG_6                    62

#define  UTIL                             4       /* callback function: SYSTEM_PANEL_CB */
#define  UTIL_QUIT                        2       /* callback function: SYSTEM_APPLY */
#define  UTIL_ENCTEST                     3       /* callback function: SYSTEM_APPLY */
#define  UTIL_TEST                        4       /* callback function: SYSTEM_APPLY */
#define  UTIL_CCD                         5       /* callback function: SYSTEM_APPLY */
#define  UTIL_M2_ADJUST                   6       /* callback function: SYSTEM_APPLY */
#define  UTIL_GOTO                        7       /* callback function: SYSTEM_APPLY */
#define  UTIL_ALTMIN                      8       /* callback function: SYSTEM_APPLY */
#define  UTIL_DUT                         9       /* callback function: SYSTEM_APPLY */
#define  UTIL_AZI                         10
#define  UTIL_ALT                         11
#define  UTIL_AUTOGUIDE                   12      /* callback function: SYSTEM_APPLY */
#define  UTIL_LIMIT                       13      /* callback function: SYSTEM_APPLY */
#define  UTIL_TEXTMSG                     14
#define  UTIL_TEXTMSG_2                   15
#define  UTIL_DECORATION_3                16
#define  UTIL_DECORATION_4                17
#define  UTIL_DECORATION_5                18
#define  UTIL_DECORATION_6                19


     /* Menu Bars, Menus, and Menu Items: */

#define  MBFILE                           1
#define  MBFILE_File                      2
#define  MBFILE_File_TCS                  3       /* callback function: MBCB */
#define  MBFILE_File_READINPUTS           4       /* callback function: MBCB */
#define  MBFILE_File_CATALOGUE            5       /* callback function: MBCB */
#define  MBFILE_File_SCAN                 6       /* callback function: MBCB */
#define  MBFILE_File_THESKY               7
#define  MBFILE_File_THESKY_SUBMENU       8
#define  MBFILE_File_THESKY_CONNECT       9       /* callback function: MBCB */
#define  MBFILE_File_THESKY_DISCONNECT    10      /* callback function: MBCB */
#define  MBFILE_File_PRELOAD              11
#define  MBFILE_File_PRELOAD_SUBMENU      12
#define  MBFILE_File_PRELOAD_CONNECT      13      /* callback function: MBCB */
#define  MBFILE_File_PRELOAD_DISCONNECT   14      /* callback function: MBCB */
#define  MBFILE_File_SAVEAXP              15      /* callback function: MBCB */
#define  MBFILE_File_READAXP              16      /* callback function: MBCB */
#define  MBFILE_File_DOME                 17      /* callback function: MBCB */
#define  MBFILE_File_NETWORK              18      /* callback function: MBCB */
#define  MBFILE_File_ECTEST               19      /* callback function: MBCB */
#define  MBFILE_File_UTIL                 20      /* callback function: MBCB */


     /* Callback Prototypes: */ 

int  CVICALLBACK AGTIMER_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_PARKING(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CCD_APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CCD_PANEL_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK MBCB(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK PanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SYSTEM_APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SYSTEM_PANEL_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TEXTCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
