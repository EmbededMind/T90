#ifndef _LAYOUT_SUB_MENU_H
#define _LAYOUT_SUB_MENU_H

#include "layout_main_menu.h"

/***第二级菜单窗口的起始位置和大小***/
#define SUB_MENU_X       (MAIN_MENU_X + MAIN_MENU_WIDTH )  
#define SUB_MENU_Y       MAIN_MENU_Y
#define SUB_MENU_WIDTH   250
#define SUB_MENU_HEIGHT  MAIN_MENU_HEIGHT

/***第二级菜单所包含控件的大小***/
#define SUB_MENU_ITEM_WIDTH   SUB_MENU_WIDTH
#define SUB_MENU_ITEM_HEIGHT  MAIN_MENU_ITEM_HEIGHT
#define SUB_MENU_ITEM_MARGIN  MAIN_MENU_ITEM_MARGIN     //边缘属性


#endif
