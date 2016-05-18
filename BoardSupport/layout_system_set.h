#ifndef _LAYOUT_SYSTEM_SET_H
#define _LAYOUT_SYSTEM_SET_H

#include "layout_main_menu.h"

#define ID_WINDOW_0         (GUI_ID_USER + 0x00)

#define ID_SLIDER_0         (GUI_ID_USER + 0x20)
#define ID_SLIDER_1         (GUI_ID_USER + 0x21)
#define ID_SLIDER_2         (GUI_ID_USER + 0x22)
#define ID_SLIDER_3         (GUI_ID_USER + 0x23)
#define ID_SLIDER_4         (GUI_ID_USER + 0x24)
#define ID_SLIDER_5         (GUI_ID_USER + 0x25)
#define ID_SLIDER_6         (GUI_ID_USER + 0x26)
#define ID_SLIDER_7         (GUI_ID_USER + 0x27)


#define SYSTEM_SET_X        (MAIN_MENU_X+MAIN_MENU_WIDTH)
#define SYSTEM_SET_Y        MAIN_MENU_Y

#define SYSTEM_SET_WIDTH    (SCREEN_WIDTH-SYSTEM_SET_X)
#define SYSTEM_SET_HEIGHT   MAIN_MENU_HEIGHT

#define SYSTEM_SET_ITEM_X       250
#define SYSTEM_SET_ITEM_Y       55
#define SYSTEM_SET_ITEM_WIDTH   120
#define SYSTEM_SET_ITEM_HEIGHT  30
#define SYSTEM_SET_ITEM_MARGIN  12
#define SYSTEM_SET_LEFT_CHOICE       170
#define SYSTEM_SET_RIGHT_CHOICE      230
#endif

