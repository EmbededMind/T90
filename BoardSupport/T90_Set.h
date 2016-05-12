#ifndef  _SYS_CONF_H
#define  _SYS_CONF_H

#include "GUI.h"
#include "uart.h"
#include "lpc177x_8x_eeprom.h"
#include "Config.h"


extern unsigned long SYS_Date;
extern unsigned long SYS_Time;

void T90_Init(void);
Bool T90_Load(void);
void T90_Store(void);
void T90_Reset(void);

#endif

