#ifndef _COMM_H
#define _COMM_H
#include <lpc_types.h>

void Comm_sendPulse(void);
void Comm_addFrame( uint8_t whichPort, uint16_t offset_x, uint16_t offset_y );
uint8_t* Comm_fetchNextFrame(void);
void Comm_sendFrame(uint8_t* pFrame);
//void Comm_sendFrame(uint8_t whichPort, uint16_t offset_x, uint16_t offset_y);

#endif