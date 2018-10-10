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

#define  PANEL                            1
#define  PANEL_TRANSMITLIST               2       /* control type: textBox, callback function: (none) */
#define  PANEL_TRANSMIT                   3       /* control type: string, callback function: TransmitCallback */
#define  PANEL_TCP_CONNECTED              4       /* control type: LED, callback function: (none) */
#define  PANEL_RECEIVELIST                5       /* control type: textBox, callback function: (none) */
#define  PANEL_CONNECTED                  6       /* control type: LED, callback function: (none) */
#define  PANEL_CLIENT_PORTNUM             7       /* control type: string, callback function: (none) */
#define  PANEL_CLIENT_IP                  8       /* control type: string, callback function: (none) */
#define  PANEL_ONLINE                     9       /* control type: LED, callback function: (none) */
#define  PANEL_CLEAR                      10      /* control type: command, callback function: ClearReceiveScreen */
#define  PANEL_QUIT                       11      /* control type: command, callback function: Quit */
#define  PANEL_TIMER                      12      /* control type: timer, callback function: TimerCallback */
#define  PANEL_DECORATION_2               13      /* control type: deco, callback function: (none) */
#define  PANEL_SERVER_PORTNUM             14      /* control type: string, callback function: (none) */
#define  PANEL_SERVER_IP                  15      /* control type: string, callback function: (none) */
#define  PANEL_DECORATION                 16      /* control type: deco, callback function: (none) */
#define  PANEL_DEC                        17      /* control type: string, callback function: (none) */
#define  PANEL_RA                         18      /* control type: string, callback function: (none) */
#define  PANEL_Transmit2                  19      /* control type: command, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ClearReceiveScreen(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TransmitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
