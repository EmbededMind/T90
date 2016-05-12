#include "stub.h"
#include "boat_struct.h"
#include "T90.h"
#include "math.h"
#include "detect.h"


void llToxy(BERTH *pBerth)
{
    
    pBerth->x_to_cross = (int)(cos(mothership.COG*3.14/1800)*(pBerth->Boat.longitude - mothership.longitude)
																				 -sin(mothership.COG*3.14/1800)*(pBerth->Boat.latitude  - mothership.latitude));
		pBerth->y_to_cross = (int)(sin(mothership.COG*3.14/1800)*(pBerth->Boat.longitude - mothership.longitude)
																				 +cos(mothership.COG*3.14/1800)*(pBerth->Boat.latitude  - mothership.latitude));    
}





