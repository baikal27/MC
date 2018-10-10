/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2006. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAINPNL                         1       /* callback function: MainPanelCB */
#define  MAINPNL_TRANSMIT                2
#define  MAINPNL_STRING                  3       /* callback function: TransmitCB */
#define  MAINPNL_RECEIVE                 4
#define  MAINPNL_CONNECTED               5
#define  MAINPNL_CLEAR                   6       /* callback function: ClearScreenCB */
#define  MAINPNL_CLIENT_NAME             7
#define  MAINPNL_CLIENT_IP               8
#define  MAINPNL_SERVER_NAME             9
#define  MAINPNL_SERVER_IP               10
#define  MAINPNL_DECORATION              11
#define  MAINPNL_TEXTMSG                 12


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ClearScreenCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TransmitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
