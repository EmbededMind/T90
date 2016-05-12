#ifndef _SPI2_H
#define _SPI2_H


#include <LPC177x_8x.h>
#include <LPC177x_8x_gpio.h>


#define SPI2_CS_LOW()       GPIO_ClearValue(1, (1<<8))
#define SPI2_CS_HIGH()      GPIO_SetValue(1,  (1<<8))


void SPI2_Int(void);
uint8_t SPI2_SendByte(uint8_t dat);
void SPI2_SendData(uint8_t cmd, uint8_t* pData,uint8_t limits);
uint16_t SPI2_ReadData(void);


#endif
