/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2013. All Rights Reserved.          */
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
#define  GUIDE_OUT                        4       /* callback function: FGUIDE */
#define  GUIDE_IN                         5       /* callback function: FGUIDE */
#define  GUIDE_E                          6       /* callback function: CB_GUIDE */
#define  GUIDE_W                          7       /* callback function: CB_GUIDE */
#define  GUIDE_SPEED_RA                   8       /* callback function: SET_SPEED */
#define  GUIDE_MEANF                      9       /* callback function: GUIDE_CBF */
#define  GUIDE_OFFRA                      10      /* callback function: SET_SPEED */
#define  GUIDE_OFFDEC                     11      /* callback function: SET_SPEED */
#define  GUIDE_SPEED_DEC                  12      /* callback function: SET_SPEED */
#define  GUIDE_GRAPH                      13
#define  GUIDE_SAVEPOINTING               14      /* callback function: GUIDE_CBF */
#define  GUIDE_OFFSET                     15      /* callback function: GUIDE_CBF */
#define  GUIDE_STOP                       16      /* callback function: GUIDE_CBF */
#define  GUIDE_QUIT_2                     17
#define  GUIDE_GOOFF                      18      /* callback function: SET_SPEED */
#define  GUIDE_QUIT                       19      /* callback function: GUIDE_CBF */
#define  GUIDE_TRACK_ON_OFF               20      /* callback function: GUIDE_CBF */
#define  GUIDE_RING_SPEED                 21      /* callback function: CB_GUIDE */
#define  GUIDE_RING_MOTOR                 22      /* callback function: CB_GUIDE */
#define  GUIDE_RING                       23      /* callback function: GUIDE_CBF */
#define  GUIDE_DECORATION_5               24
#define  GUIDE_DECORATION_6               25
#define  GUIDE_DECORATION_9               26
#define  GUIDE_DECORATION_8               27
#define  GUIDE_DECORATION_7               28
#define  GUIDE_OFFSET_TIMER               29      /* callback function: OFFSET_TIMER_CBF */
#define  GUIDE_TIMER                      30      /* callback function: GUIDE_TIMER_CBF */
#define  GUIDE_TEXTMSG_8                  31
#define  GUIDE_TEXTMSG_7                  32
#define  GUIDE_GDT                        33


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CB_FOCUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_GUIDE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FGUIDE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GUIDE_CBF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GUIDE_TIMER_CBF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OFFSET_TIMER_CBF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SET_SPEED(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
