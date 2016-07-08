#ifndef _TRANSFORM_H
#define _TRANSFORM_H


#include "boat_struct.h"
#include "T90.h"

int rectifyNum(int num, int stepValue);

void llToxy(BERTH *pBerth);

int getAngleOfShip(BERTH *pBerth);

int SND_ParseNum(int num, uint8_t *pNum);








#endif


















