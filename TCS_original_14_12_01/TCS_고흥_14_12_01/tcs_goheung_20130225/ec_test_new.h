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

#define  EC_TEST                          1
#define  EC_TEST_RING2                    2
#define  EC_TEST_RING                     3
#define  EC_TEST_SAMPLE_TIME              4       /* callback function: CB_EC_TEST */
#define  EC_TEST_GRAPH_2                  5
#define  EC_TEST_GRAPH                    6
#define  EC_TEST_PLOTE12                  7       /* callback function: CB_EC_TEST */
#define  EC_TEST_ANALYSIS                 8       /* callback function: CB_EC_TEST */
#define  EC_TEST_START                    9       /* callback function: CB_EC_TEST */
#define  EC_TEST_STOP                     10      /* callback function: CB_EC_TEST */
#define  EC_TEST_PRINT                    11      /* callback function: PRINT */
#define  EC_TEST_CLOSE                    12      /* callback function: CB_EC_TEST */
#define  EC_TEST_TEXT                     13
#define  EC_TEST_TIMER                    14      /* callback function: CB_EC_TIMER */
#define  EC_TEST_DECORATION               15


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CB_EC_TEST(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_EC_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRINT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
