#ifndef _LAYOUT_SINGLE_SHIP_H
#define _LAYOUT_SINGLE_SHIP_H

#include "layout.h"


#define BBS1_RADIUS        16
#define BBS1_MARGIN        15

#define BBS1_ABOVE_X       (SCREEN_HCENTER+BBS1_MARGIN*1)
#define BBS1_ABOVE_Y       (BBS1_MARGIN * 3)

#define BBS1_BELOW_X       BBS1_ABOVE_X
#define BBS1_BELOW_Y       (SCREEN_VCENTER + BBS1_MARGIN)

#define BBS1_WIDTH         (810-BBS1_ABOVE_X)
#define BBS1_HEIGHT        (SCREEN_VCENTER - BBS1_MARGIN * 4)



#define BBS2_RADIUS        16
#define BBS2_MARGIN        15

#define BBS2_ABOVE_X       BBS1_ABOVE_X
#define BBS2_ABOVE_Y       (BBS2_MARGIN * 3)

#define BBS2_ABOVE_WIDTH   BBS1_WIDTH
#define BBS2_ABOVE_HEIGHT  (SCREEN_HEIGHT/4)

#define BBS2_BELOW_X       BBS1_ABOVE_X
#define BBS2_BELOW_Y       (BBS2_ABOVE_HEIGHT+BBS2_ABOVE_Y+BBS2_MARGIN*2)

#define BBS2_BELOW_WIDTH   BBS1_WIDTH
#define BBS2_BELOW_HEIGHT  (SCREEN_HEIGHT-BBS2_BELOW_Y-BBS2_MARGIN*3)


#endif

