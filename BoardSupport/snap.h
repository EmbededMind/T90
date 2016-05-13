#ifndef _SNAP_H
#define _SNAP_H

#include "Config.h"
#include "boat_struct.h"
#include "lpc_types.h"
#include "T90.h"


extern BERTH *  pSnapLink;

long SNAP_getSnapObjMMSI(void);
void SNAP_reset(void);
void SNAP_getPara(int *x, int *y);
void SNAP_searchNearestObj(int Dir_x, int Dir_y);

void SNAP_Refresh(void);

#endif


