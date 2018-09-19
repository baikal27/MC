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

#define  CAL_2                            1
#define  CAL_2_GRAPH_DEC                  2
#define  CAL_2_GRAPH_RA                   3

#define  CMM                              2
#define  CMM_TEXTBOX                      2
#define  CMM_PRINT                        3       /* callback function: CB_CAL */
#define  CMM_CLOSE                        4       /* callback function: CB_CAL */
#define  CMM_START                        5       /* callback function: CB_CAL */
#define  CMM_DEC                          6
#define  CMM_RA                           7
#define  CMM_TEXTMSG                      8
#define  CMM_DECORATION_3                 9

#define  SCAN                             3
#define  SCAN_DECL_MIN                    2
#define  SCAN_DECL_MAX                    3
#define  SCAN_MAGMIN                      4
#define  SCAN_MAGMAX                      5
#define  SCAN_STAR_LIST_2                 6
#define  SCAN_STAR_LIST                   7
#define  SCAN_CLOSE                       8       /* callback function: CB_SCAN */
#define  SCAN_TSA                         9       /* callback function: CB_SCAN */
#define  SCAN_MODEL                       10      /* callback function: CB_SCAN */
#define  SCAN_STOPT                       11      /* callback function: CB_SCAN */
#define  SCAN_ACCEPT                      12      /* callback function: CB_SCAN */
#define  SCAN_SELECT                      13      /* callback function: CB_SCAN */
#define  SCAN_HP                          14      /* callback function: CB_SCAN */
#define  SCAN_MOVE                        15      /* callback function: CB_SCAN */
#define  SCAN_OFFSET                      16      /* callback function: CB_SCAN */
#define  SCAN_RING                        17      /* callback function: CB_SCAN */
#define  SCAN_NNSTAR                      18
#define  SCAN_DECORATION_3                19
#define  SCAN_DECORATION_4                20
#define  SCAN_TEXTMSG_3                   21
#define  SCAN_TEXTMSG_4                   22
#define  SCAN_TEXTMSG_5                   23
#define  SCAN_TEXTMSG                     24
#define  SCAN_TEXTMSG_2                   25
#define  SCAN_TEXTMSG_7                   26
#define  SCAN_TEXTMSG_6                   27


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CB_CAL(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_SCAN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
