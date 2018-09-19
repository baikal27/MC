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

#define  CAT                              1
#define  CAT_LISTBOX                      2
#define  CAT_NEBULAR                      3       /* callback function: SELECT_CAT */
#define  CAT_GALAXY                       4       /* callback function: SELECT_CAT */
#define  CAT_GLOBULAR                     5       /* callback function: SELECT_CAT */
#define  CAT_MESSIER                      6       /* callback function: SELECT_CAT */
#define  CAT_PLANET                       7       /* callback function: SELECT_CAT */
#define  CAT_USER                         8       /* callback function: SELECT_CAT */
#define  CAT_QUIT                         9       /* callback function: CAT_APPLY */
#define  CAT_STOP                         10      /* callback function: CAT_APPLY */
#define  CAT_MOVE                         11      /* callback function: CAT_APPLY */
#define  CAT_TEXTMSG                      12
#define  CAT_TEXTMSG_2                    13
#define  CAT_HEAD                         14
#define  CAT_DECORATION                   15
#define  CAT_DECORATION_2                 16


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CAT_APPLY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SELECT_CAT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
