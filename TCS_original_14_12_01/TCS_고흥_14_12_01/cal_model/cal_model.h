/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2012. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CMM                             1
#define  CMM_GRAPH_DEC                   2
#define  CMM_GRAPH_RA                    3
#define  CMM_TEXTBOX                     4       /* callback function: CBTB */
#define  CMM_CLOSE                       5       /* callback function: CB_CAL */
#define  CMM_READINIT                    6       /* callback function: CB_CAL */
#define  CMM_START                       7       /* callback function: CB_CAL */
#define  CMM_DEC                         8
#define  CMM_RA                          9
#define  CMM_DECS                        10
#define  CMM_RAS                         11
#define  CMM_TEXTMSG                     12
#define  CMM_DECORATION                  13


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CBTB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_CAL(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
