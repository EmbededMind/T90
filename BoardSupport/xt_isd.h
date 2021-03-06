#ifndef _XT_ISD_H
#define _XT_ISD_H
#include "stdint.h"
#include "lpc177x_8x_gpio.h"


void ISD_Init(void);
void ISD_SetVolumn(uint8_t vol);
void ISD_Wait_PWRUp(void);
void ISD_PWRDn(void);
void ISD_Play(uint8_t addr);
Bool ISD_IsBusy(void);
void ISD_SetVolumnZero();
#endif

