/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2001. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  SERVER                          1
#define  SERVER_R_TEXT                   2
#define  SERVER_SEND                     3       /* callback function: NET_APPLY */
#define  SERVER_QUIT                     4       /* callback function: NET_APPLY */
#define  SERVER_DISCONNECT               5       /* callback function: NET_APPLY */
#define  SERVER_S_TEXT                   6       /* callback function: APPLY */
#define  SERVER_LED                      7
#define  SERVER_MODE                     8       /* callback function: NET_APPLY */
#define  SERVER_TOGGLEBUTTON             9       /* callback function: NET_APPLY */
#define  SERVER_DECORATION               10


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NET_APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
