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
#define  PANEL_AZI_HOME                   6       /* control type: LED, callback function: (none) */
#define  PANEL_ALT_CCW                    7       /* control type: LED, callback function: (none) */
#define  PANEL_ALT_VELOCITY               8       /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_VELOCITY               9       /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_MODE                   10      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_MODE                   11      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_DIR                    12      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_DIR                    13      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_COUNTER                14      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_COUNTER                15      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT_ENCODER                16      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI_ENCODER                17      /* control type: numeric, callback function: (none) */
#define  PANEL_AZI                        18      /* control type: textMsg, callback function: (none) */
#define  PANEL_TCSINIT                    19      /* control type: command, callback function: APPLY */
#define  PANEL_PARK                       20      /* control type: command, callback function: APPLY */
#define  PANEL_STOP                       21      /* control type: command, callback function: APPLY */
#define  PANEL_INIT                       22      /* control type: command, callback function: APPLY */
#define  PANEL_TEXTBOX_2                  23      /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX                    24      /* control type: textBox, callback function: (none) */
#define  PANEL_OFF_CLEAR                  25      /* control type: command, callback function: CB_TIMER */
#define  PANEL_GOOFF                      26      /* control type: command, callback function: CB_TIMER */
#define  PANEL_GOTO                       27      /* control type: command, callback function: APPLY */
#define  PANEL_DEC_INPUT                  28      /* control type: string, callback function: (none) */
#define  PANEL_RA_INPUT                   29      /* control type: string, callback function: (none) */
#define  PANEL_JDATE                      30      /* control type: numeric, callback function: (none) */
#define  PANEL_CONST_EAST                 31      /* control type: command, callback function: APPLY */
#define  PANEL_CONST_WEST                 32      /* control type: command, callback function: APPLY */
#define  PANEL_CONST_DOWN                 33      /* control type: command, callback function: APPLY */
#define  PANEL_CONST_UP                   34      /* control type: command, callback function: APPLY */
#define  PANEL_TIMER                      35      /* control type: timer, callback function: CB_TIMER */
#define  PANEL_OFF_DEC                    36      /* control type: numeric, callback function: (none) */
#define  PANEL_OFF_ALT                    37      /* control type: numeric, callback function: (none) */
#define  PANEL_OFF_AZI                    38      /* control type: numeric, callback function: (none) */
#define  PANEL_OFF_RA                     39      /* control type: numeric, callback function: (none) */
#define  PANEL_ALT                        40      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel_Proc(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
