/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2018. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: Panel_Proc */
#define  PANEL_ALT_CW                     2       /* control type: LED, callback function: (none) */
#define  PANEL_AZI_CW                     3       /* control type: LED, callback function: (none) */
#define  PANEL_ALT_HOME                   4       /* control type: LED, callback function: (none) */
#define  PANEL_AZI_CCW                    5       /* control type: LED, callback function: (none) */
#define  PANEL_SHM_CONNECTED              6       /* control type: LED, callback function: (none) */
#define  PANEL_AZI_HOME                   7       /* control type: LED, callback function: (none) */
#define  PANEL_ALT_CCW                    8       /* control type: LED, callback function: (none) */
#define  PANEL_ALT_VELOCITY               9       /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_VELOCITY               10      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_MODE                   11      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_MODE                   12      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_DIR                    13      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_DIR                    14      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_COUNTER                15      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_COUNTER                16      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_ENCODER                17      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_ENCODER                18      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI                        19      /* control type: textMsg, callback function: (none) */
#define  PANEL_TCSINIT                    20      /* control type: command, callback function: (none) */
#define  PANEL_PARK                       21      /* control type: command, callback function: (none) */
#define  PANEL_STOP                       22      /* control type: command, callback function: (none) */
#define  PANEL_INIT                       23      /* control type: command, callback function: (none) */
#define  PANEL_TEXTBOX_2                  24      /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX                    25      /* control type: textBox, callback function: (none) */
#define  PANEL_OFF_CLEAR                  26      /* control type: command, callback function: TRACKING_TIMER */
#define  PANEL_GOOFF                      27      /* control type: command, callback function: TRACKING_TIMER */
#define  PANEL_GOTO                       28      /* control type: command, callback function: (none) */
#define  PANEL_DEC_INPUT                  29      /* control type: string, callback function: (none) */
#define  PANEL_RA_INPUT                   30      /* control type: string, callback function: (none) */
#define  PANEL_JDATE                      31      /* control type: numeric, callback function: (none) */
#define  PANEL_CONST_EAST                 32      /* control type: command, callback function: (none) */
#define  PANEL_CONST_WEST                 33      /* control type: command, callback function: (none) */
#define  PANEL_CONST_DOWN                 34      /* control type: command, callback function: (none) */
#define  PANEL_CONST_UP                   35      /* control type: command, callback function: (none) */
#define  PANEL_OFF_DEC                    36      /* control type: numeric, callback function: (none) */
#define  PANEL_OFF_ALT                    37      /* control type: numeric, callback function: (none) */
#define  PANEL_OFF_AZI                    38      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT                        39      /* control type: textMsg, callback function: (none) */
#define  PANEL_OFF_RA                     40      /* control type: numeric, callback function: (none) */
#define  PANEL_APPLY_TIMER                41      /* control type: timer, callback function: APPLY_TIMER */
#define  PANEL_TRACKING_TIMER             42      /* control type: timer, callback function: TRACKING_TIMER */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK APPLY_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel_Proc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TRACKING_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
