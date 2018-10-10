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
#define  MAINPNL_ONLINE                  6
#define  MAINPNL_CLEAR                   7       /* callback function: ClearScreenCB */
#define  MAINPNL_CLIENT_NAME             8
#define  MAINPNL_CLIENT_IP               9
#define  MAINPNL_SERVER_NAME             10
#define  MAINPNL_SERVER_IP               11
#define  MAINPNL_DECORATION              12
#define  MAINPNL_TEXTMSG                 13
#define  MAINPNL_DECORATION_2            14


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ClearScreenCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TransmitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
