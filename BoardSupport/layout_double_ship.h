#ifndef _LAYOUT_DOUBLE_SHIP_H

#define _LAYOUT_DOUBLE_SHIP_H

#include "layout.h"

#define _MAP_LEFT    0
#define _MAP_RIGHT   (SCREEN_WIDTH/2)
#define _MAP_TOP     0
#define _MAP_BOTTOM  SCREEN_HEIGHT

#define BBS_RADIUS        20
#define BBS_MARGIN        15
//#define BBS_MARGIN_LEFT   30
//#define BBS_MARGIN_RIGHT  BBS_RADIUS
//#define BBS_MARGIN_TOP    40
//#define BBS_MARGIN_BOTTOM 40

#define BBS_ABOVE_X       (_MAP_RIGHT+BBS_MARGIN*2)
#define BBS_ABOVE_Y       (BBS_MARGIN * 2)

#define BBS_BELOW_X       (_MAP_RIGHT+BBS_MARGIN*2)
#define BBS_BELOW_Y       (SCREEN_VCENTER + BBS_MARGIN)

#define BBS_WIDTH         (SCREEN_WIDTH-BBS_ABOVE_X-BBS_MARGIN*2)
#define BBS_HEIGHT        (SCREEN_VCENTER - BBS_MARGIN * 3)


//#define BBS_BELOW_X       (_MAP_RIGHT+BBS_MARGIN)
//#define BBS_BELOW_WIDTH   (SCREEN_WIDTH-BBS_BELOW_X)

#endif

