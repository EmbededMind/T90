#ifndef _WINMMSISET_H
#define _WINMMSISET_H

#include "WM.h"
#include "GUI.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "maintask.h"
#include "dlg.h"
#include "T90.h"
#include "t90font.h"
#include "TEXT.h"
#include "stub.h"

/*********************************************************************
*
*              Handles
*
**********************************************************************
*/
/* MMSI设置窗口的句柄 */
WM_HWIN MMSISetWin;

/* 输入法窗口的句柄 */
WM_HWIN SoftInputWin;


/*********************************************************************
*
*             Windows or Widget layout
*
**********************************************************************
*/

/************************************************************
*
*      inputWindow
*/

/* FrameWin */
#define INPUTWIN_X         20     //x0
#define INPUTWIN_Y         150    //y0
#define INPUTWIN_WIDTH     370    //xsize
#define INPUTWIN_HEIGHT    105     //ysize

/* numberKeys */
#define INPUT_BT_WIDTH      50     //xsize
#define INPUT_BT_HEIGHT     40     //ysize
#define INPUT_BT_OFFS       10     //hspace

/* y0 */
#define INPUT_BT_LIN1_Y      10      //first line
#define INPUT_BT_LIN2_Y      55     //second line

/* x0 */
#define INPUT_BT0_X       10
#define INPUT_BT1_X       INPUT_BT0_X+INPUT_BT_WIDTH+INPUT_BT_OFFS
#define INPUT_BT2_X							INPUT_BT1_X+INPUT_BT_WIDTH+INPUT_BT_OFFS
#define INPUT_BT3_X							INPUT_BT2_X+INPUT_BT_WIDTH+INPUT_BT_OFFS
#define INPUT_BT4_X							INPUT_BT3_X+INPUT_BT_WIDTH+INPUT_BT_OFFS
#define INPUT_BT5_X							INPUT_BT4_X+INPUT_BT_WIDTH+INPUT_BT_OFFS

/************************************************************
*
*      MMSISetWindows
*/

/* FrameWin */
#define MMSISET_X        150      //x0
#define MMSISET_Y        80       //y0
#define MMSISET_WIDTH    500      //xsize
#define MMSISET_HEIGHT   300      //ysize

/* EditWin */
#define MMSISET_ET_X      20      //x0
#define MMSISET_ET_Y      100     //y0
#define MMSISET_ET_WIDTH  370     //xsize
#define MMSISET_ET_HEIGHT 45      //ysize

//确定键
#define MMSISET_BTOK_X       MMSISET_ET_X+MMSISET_ET_WIDTH+10      //x0
#define MMSISET_BTOK_Y       MMSISET_ET_Y                          //y0
#define MMSISET_BTOK_WIDTH   85                                    //xsize
#define MMSISET_BTOK_HEIGHT  MMSISET_ET_HEIGHT                     //ysize


/*********************************************************************
*
*             Dialog item IDs
*
**********************************************************************
*/

/**  @brief input button Ids**/
#define ID_BUTTON_MMSIOK   GUI_ID_USER+1
#define ID_BUTTON_0        GUI_ID_USER+2
#define ID_BUTTON_1        GUI_ID_USER+3
#define ID_BUTTON_2        GUI_ID_USER+4
#define ID_BUTTON_3        GUI_ID_USER+5
#define ID_BUTTON_4        GUI_ID_USER+6
#define ID_BUTTON_DEL      GUI_ID_USER+7
#define ID_BUTTON_5        GUI_ID_USER+8
#define ID_BUTTON_6        GUI_ID_USER+9
#define ID_BUTTON_7        GUI_ID_USER+10
#define ID_BUTTON_8        GUI_ID_USER+11
#define ID_BUTTON_9        GUI_ID_USER+12
#define ID_BUTTON_EMPTY    GUI_ID_USER+13

/*********************************************************************
*
*            Skin
*
**********************************************************************
*/
/* button skin */
extern const BUTTON_SKINFLEX_PROPS btSkin[2][2];


/*********************************************************************
*
*             other define
*
**********************************************************************
*/
/*  MMSI max length */
#define MMSI_LENGTH 9

																													
#endif
	
																													