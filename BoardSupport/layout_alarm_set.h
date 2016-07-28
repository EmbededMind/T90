#ifndef _LAYOUT_ALARM_SET_H
#define _LAYOUT_ALARM_SET_H

#include "layout_sub_menu.h"


#define ALARM_SET_X       (SUB_MENU_X + SUB_MENU_WIDTH)
#define ALARM_SET_Y       SUB_MENU_Y
#define ALARM_SET_WIDTH   (SCREEN_WIDTH - ALARM_SET_X)
#define ALARM_SET_HEIGHT  SUB_MENU_HEIGHT

#define ALARM_RADIUS      40

#define SLIDER_ORG_X      ((ALARM_SET_WIDTH-SLIDER_WIDTH)/2+40)
#define SLIDER_ORG_Y      55
#define SLIDER_WIDTH      120
#define SLIDER_HEIGHT     30
#define SLIDER_MARGIN     17

#endif
