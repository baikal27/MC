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

#define  MAINPNL                          1       /* callback function: MainPanelCB */
#define  MAINPNL_TRANSMIT                 2       /* control type: textBox, callback function: (none) */
#define  MAINPNL_STRING                   3       /* control type: string, callback function: TransmitCB */
#define  MAINPNL_RECEIVE                  4       /* control type: textBox, callback function: (none) */
#define  MAINPNL_CONNECTED                5       /* control type: LED, callback function: (none) */
#define  MAINPNL_CLEAR                    6       /* control type: command, callback function: ClearScreenCB */
#define  MAINPNL_CLIENT_NAME              7       /* control type: string, callback function: (none) */
#define  MAINPNL_CLIENT_IP                8       /* control type: string, callback function: (none) */
#define  MAINPNL_SERVER_NAME              9       /* control type: string, callback function: (none) */
#define  MAINPNL_SERVER_IP                10      /* control type: string, callback function: (none) */
#define  MAINPNL_DECORATION               11      /* control type: deco, callback function: (none) */
#define  MAINPNL_TEXTMSG                  12      /* control type: textMsg, callback function: (none) */
#define  MAINPNL_DEC_INPUT                13      /* control type: string, callback function: (none) */
#define  MAINPNL_RA_INPUT                 14      /* control type: string, callback function: (none) */
#define  MAINPNL_TCSINIT                  15      /* control type: command, callback function: TransmitCB */
#define  MAINPNL_PARK                     16      /* control type: command, callback function: TransmitCB */
#define  MAINPNL_STOP                     17      /* control type: command, callback function: TransmitCB */
#define  MAINPNL_INIT                     18      /* control type: command, callback function: TransmitCB */
#define  MAINPNL_GOTO                     19      /* control type: command, callback function: TransmitCB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ClearScreenCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TransmitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
