#ifndef _LAYOUT_DIST_SET_MENU_H
#define _LAYOUT_DIST_SET_MENU_H

#include "layout_main_menu.h"

/***距离设置菜单窗口的起始位置和大小***/
#define DST_SET_MENU_X      (MAIN_MENU_X + MAIN_MENU_WIDTH )  
#define DST_SET_MENU_Y      MAIN_MENU_Y
#define DST_SET_MENU_WIDTH  200
#define DST_SET_MENU_HEIGHT MAIN_MENU_HEIGHT

/***距离设置菜单所包含控件的大小***/
#define DST_SET_MENU_ITEM_WIDTH   (DST_SET_MENU_WIDTH-DST_SET_MENU_ITEM_MARGIN*2)
#define DST_SET_MENU_ITEM_HEIGHT  MAIN_MENU_ITEM_HEIGHT
#define DST_SET_MENU_ITEM_MARGIN  MAIN_MENU_ITEM_MARGIN     //边缘属性

#endif


