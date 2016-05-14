#ifndef _DETECT_H
#define _DETECT_H
#include "boat_struct.h"



void detect(void);
void detectInit(void);

void llToxy(BERTH *pBerth);
int getAngleOfShip(BERTH *pBNerth);
int SND_ParseNum(int num, uint8_t *pNum);

#endif







