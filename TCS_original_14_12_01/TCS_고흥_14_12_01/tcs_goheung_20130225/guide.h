/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2010. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FOCUSPANEL                       1
#define  FOCUSPANEL_OUT                   2       /* callback function: CB_FOCUS */
#define  FOCUSPANEL_IN                    3       /* callback function: CB_FOCUS */
#define  FOCUSPANEL_QUIT                  4       /* callback function: CB_FOCUS */
#define  FOCUSPANEL_SPEED                 5       /* callback function: CB_FOCUS */
#define  FOCUSPANEL_DECORATION            6

#define  GUIDE                            2
#define  GUIDE_S                          2       /* callback function: CB_GUIDE */
#define  GUIDE_N                          3       /* callback function: CB_GUIDE */
#define  GUIDE_E                          4       /* callback function: CB_GUIDE */
#define  GUIDE_W                          5       /* callback function: CB_GUIDE */
#define  GUIDE_SPEED_RA                   6       /* callback function: SET_SPEED */
#define  GUIDE_SPEED_DEC                  7       /* callback function: SET_SPEED */
#define  GUIDE_GRAPH                      8
#define  GUIDE_OFFSET                     9       /* callback function: GUIDE_CBF */
#define  GUIDE_STOP                       10      /* callback function: GUIDE_CBF */
#define  GUIDE_QUIT                       11      /* callback function: GUIDE_CBF */
#define  GUIDE_TRACK_ON_OFF               12      /* callback function: GUIDE_CBF */
#define  GUIDE_RING_SPEED                 13      /* callback function: CB_GUIDE */
#define  GUIDE_RING_MOTOR                 14      /* callback function: CB_GUIDE */
#define  GUIDE_RING                       15      /* callback function: GUIDE_CBF */
#define  GUIDE_DECORATION_5               16
#define  GUIDE_DECORATION_6               17
#define  GUIDE_DECORATION_7               18
#define  GUIDE_TIMER                      19      /* callback function: GUIDE_TIMER_CBF */
#define  GUIDE_TEXTMSG_7                  20


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CB_FOCUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_GUIDE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GUIDE_CBF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GUIDE_TIMER_CBF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SET_SPEED(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
