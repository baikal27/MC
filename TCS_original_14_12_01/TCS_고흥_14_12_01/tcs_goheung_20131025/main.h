/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 1999. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PARK                            1
#define  PARK_CLOSE                      2       /* callback function: CB_PARKING */
#define  PARK_START                      3       /* callback function: CB_PARKING */
#define  PARK_DER_LED                    4
#define  PARK_DEC_LED                    5
#define  PARK_DIR_DER                    6
#define  PARK_RA_LED                     7
#define  PARK_DIR_DEC                    8
#define  PARK_DIR_RA                     9

#define  SYSTEM                          2
#define  SYSTEM_CLOSE                    2       /* callback function: SYSTEM_APPLY */
#define  SYSTEM_DUT                      3       /* callback function: SYSTEM_APPLY */
#define  SYSTEM_TTEST                    4       /* callback function: SYSTEM_APPLY */
#define  SYSTEM_STRING                   5
#define  SYSTEM_TIMER                    6       /* callback function: TIMER_CB */

#define  TEL                             3
#define  TEL_AZI                         2
#define  TEL_ALT                         3
#define  TEL_DATE                        4
#define  TEL_KST                         5
#define  TEL_LAST                        6
#define  TEL_RA_INPUT_2                  7
#define  TEL_RA_INPUT                    8
#define  TEL_HA_SPEED                    9
#define  TEL_HA_ERROR                    10
#define  TEL_DEC_SPEED                   11
#define  TEL_DER_ERROR                   12
#define  TEL_DOME_ERROR                  13
#define  TEL_DEC_ERROR                   14
#define  TEL_DEC_INPUT_3                 15
#define  TEL_DEC_INPUT_4                 16
#define  TEL_NAME_INPUT                  17
#define  TEL_DEC_INPUT_2                 18
#define  TEL_DEC_INPUT                   19
#define  TEL_DEC_HOME                    20
#define  TEL_DEC_CCW                     21
#define  TEL_DEC_CW                      22
#define  TEL_RA_HOME                     23
#define  TEL_RA_CCW                      24
#define  TEL_RA_CW                       25
#define  TEL_HA                          26
#define  TEL_DEC                         27
#define  TEL_PARKING                     28      /* callback function: APPLY */
#define  TEL_GUIDE                       29      /* callback function: APPLY */
#define  TEL_HP                          30      /* callback function: APPLY */
#define  TEL_QUIT                        31      /* callback function: QUIT_PROGRAM */
#define  TEL_INIT                        32      /* callback function: APPLY */
#define  TEL_MOVE_TO_2                   33      /* callback function: APPLY */
#define  TEL_MOVE_TO_3                   34      /* callback function: APPLY */
#define  TEL_MOVE_TO                     35      /* callback function: APPLY */
#define  TEL_DOME                        36      /* callback function: APPLY */
#define  TEL_STOP                        37      /* callback function: APPLY */
#define  TEL_SYSTEM_2                    38      /* callback function: APPLY */
#define  TEL_SYSTEM                      39      /* callback function: APPLY */
#define  TEL_SCAN                        40      /* callback function: APPLY */
#define  TEL_CATALOGUE                   41      /* callback function: APPLY */
#define  TEL_NETWORK                     42      /* callback function: APPLY */
#define  TEL_DER_HOME                    43
#define  TEL_DER_ONOFF                   44      /* callback function: APPLY */
#define  TEL_DOME_ONOFF                  45      /* callback function: APPLY */
#define  TEL_DECORATION                  46
#define  TEL_DECORATION_2                47
#define  TEL_MAIN_TIMER                  48      /* callback function: CB_TIMER */
#define  TEL_TEXTMSG_12                  49
#define  TEL_TEXTMSG_13                  50
#define  TEL_TEXTMSG                     51
#define  TEL_TEXTMSG_2                   52
#define  TEL_TEXTMSG_18                  53
#define  TEL_TEXTMSG_14                  54
#define  TEL_TEXTMSG_23                  55
#define  TEL_TEXTMSG_21                  56
#define  TEL_DECORATION_6                57
#define  TEL_TEXTMSG_26                  58
#define  TEL_TEXTMSG_17                  59
#define  TEL_TEXTMSG_7                   60
#define  TEL_TEXTMSG_29                  61
#define  TEL_TEXTMSG_30                  62
#define  TEL_TEXTMSG_31                  63
#define  TEL_TEXTMSG_32                  64
#define  TEL_TEXTMSG_19                  65
#define  TEL_TEXTMSG_33                  66
#define  TEL_TEXTMSG_34                  67
#define  TEL_TEXTMSG_36                  68
#define  TEL_TEXTMSG_35                  69


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_PARKING(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUIT_PROGRAM(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SYSTEM_APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
