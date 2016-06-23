#ifndef _SNAP_H
#define _SNAP_H

#include "Config.h"
#include "boat_struct.h"
#include "lpc_types.h"
#include "T90.h"
#include "stub.h"

#define UP      0x01
#define DOWN    0x02
#define LEFT    0x03
#define RIGHT   0x04


extern BERTH *  pSnapLink;

long SNAP_getSnapObjMMSI(void);
void SNAP_reset(void);
void SNAP_getPara(int *x, int *y);
void SNAP_searchNearestObj(int Dir_x, int Dir_y);

void SNAP_Refresh(void);
void SNAP_SetSnapLink(BERTH *pBerth);
void SNAP_SearchNearestStub(char keyType, Stub *pOldCursor);

#endif


