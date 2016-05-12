#include "SPI2.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "DMA.h"



void SPI2_Int(void)
{
	 SSP_CFG_Type SSP_ConfigStruct;
  PINSEL_ConfigPin(1, 0, 4);//p1.0：SSP0_SCK
  PINSEL_ConfigPin(1, 8, 0);//p1.8: CS //SSP0_SSEL
  PINSEL_ConfigPin(1, 4, 4);//p1.4: SSP0_MISO
  PINSEL_ConfigPin(1, 1, 4);//p0.1: SSP0_MOSI
  GPIO_SetDir(1, (1<<8), 1); 	/* 设置触摸片选管脚p1.8为输出 */
  GPIO_SetValue(1, (1<<8));

  SSP_ConfigStruct.ClockRate =1000000; 	/* 配置SPI参数最高20M */
  SSP_ConfigStruct.CPHA = SSP_CPHA_SECOND;
  SSP_ConfigStruct.CPOL = SSP_CPOL_LO; 
	SSP_ConfigStruct.Databit = SSP_DATABIT_8;
	SSP_ConfigStruct.Mode = SSP_MASTER_MODE;
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_SPI;
	SSP_Init(LPC_SSP2, &SSP_ConfigStruct);//初始化SSP2

	LPC_SSP2->CR1 |= SSP_CR1_SSP_EN;//
	//SSP_Cmd(LPC_SSP2, ENABLE);	/* 使能SPI 外设 1 */
}


uint16_t SPI2_ReadData(void)
{
   uint16_t  result   = 0;
   
   SPI2_CS_HIGH();
   SPI2_CS_LOW();
   
   result  = SPI2_SendByte(0x40);
   result  = result << 8;
   
   result  |= SPI2_SendByte(0x00);
   
   SPI2_CS_HIGH();
   
   return result;
}



uint8_t  SPI2_SendByte(uint8_t dat)
{
   while( (LPC_SSP2->SR & (SSP_SR_TNF|SSP_SR_BSY))  !=  SSP_SR_TNF);
   
   LPC_SSP2->DR  = dat;
   while( (LPC_SSP2->SR & (SSP_SR_BSY|SSP_SR_RNE))  !=  SSP_SR_RNE);
   return (LPC_SSP2->DR);
}



void SPI2_SendData(uint8_t cmd, uint8_t* pData,uint8_t limits)
{
   SPI2_CS_HIGH();
   SPI2_CS_LOW();
   
   SPI2_SendByte(cmd);
   
   if(limits > 0  &&  limits < 3){
      int i  = 0;
      for(i=0; i<limits; i++){
         SPI2_SendByte(pData[i]);
      }
   }
   SPI2_CS_HIGH();
}
