#ifndef _WINFLEET_H
#define _WINFLEET_H

#include "WM.h"
#include "GUI.h"
#include "MainTask.h"
#include "EDIT.h"
#include "BUTTON.h"
#include "LISTVIEW.h"
#include "dlg.h"
#include "T90.h"
#include "T90font.h"
#include "layout_main_menu.h"
#include "T90Color.h"

/*********************************************************************
*
*              Handles
*
**********************************************************************
*/
/* 窗口句柄 */
WM_HWIN FleetWin;

/*********************************************************************
*
*             Windows or Widget layout
*
**********************************************************************
*/
#define FLEET_X        (MAIN_MENU_X+MAIN_MENU_WIDTH)
#define FLEET_Y        MAIN_MENU_Y
#define FLEET_WIDTH    (SCREEN_WIDTH-FLEET_X)
#define FLEET_HEIGHT   MAIN_MENU_HEIGHT

#endif
