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

#define  PANEL                            1       /* callback function: PanelCallback */
#define  PANEL_CLIENT_LIST                2       /* control type: listBox, callback function: (none) */
#define  PANEL_SEND                       3       /* control type: command, callback function: SendCallback */
#define  PANEL_DISCONNECT                 4       /* control type: command, callback function: DisconnectCallback */
#define  PANEL_DATA                       5       /* control type: textBox, callback function: (none) */

#define  REC_PANEL                        2       /* callback function: PanelCallback */
#define  REC_PANEL_CLIENT_NAME            2       /* control type: string, callback function: (none) */
#define  REC_PANEL_DATA                   3       /* control type: textBox, callback function: (none) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DisconnectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
