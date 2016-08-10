#include "lpc177x_8x_uart.h"
#include <string.h>
#include "comm.h"

uint8_t frameTab[5][18] = {  {0x24, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00},
                             {0x24, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00},
                             {0x24, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00},
                             {0x24, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00},
                             {0x24, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}
                          };
static uint8_t  flags  = 0;                          

static uint8_t cursor  = 0;


uint16_t Comm_getCRC(uint8_t* ptr, uint8_t num)
{
   uint16_t crc  = 0xffff;
   uint16_t  gx  = 0x1021;  
   uint8_t    i;

   while(num--)   
   {
      for(i=0x01; i!=0; i<<=1){
         if( (crc&0x8000) != 0){
            crc  <<= 1;
            crc  ^= gx;
         }
         else{
            crc  <<= 1;
         }
         
         if( ((*ptr)&i) != 0){
            crc  ^= gx;
         }
      }
      
      ptr++;
   }
   
   return ~crc;
}


/**@brief 发送心跳数据
 *
 */
void Comm_sendPulse()
{
   uint16_t crcRes  = 0;
   
   static uint8_t pulseFrame[18]  = {0x24, 0x5a, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x5D, 0x2C};
   

   
   UART_Send(UART_2,  pulseFrame, 18, BLOCKING);
}


void Comm_sendFrame(uint8_t* pFrame)
{
   UART_Send(UART_2, pFrame, 18, BLOCKING);
}


/**@brief 发送一帧数据
 *
 * @input [in] whichPort  安全标端口号{0, 1, 2}  
 * @input [in] offset_x   X方向偏移
 * @input [in] offset_y   Y方向偏移
 *
 */
void Comm_addFrame( uint8_t whichPort, int offset_x, uint16_t offset_y, uint8_t sog_time, uint8_t cog_time)
{
   uint16_t crcRes  = 0;
   
   
   frameTab[cursor][2]  = offset_y >> 8;
   frameTab[cursor][3]  = offset_y & 0xff;
   
   if(offset_x >= 0)
   {
      frameTab[cursor][4] = 1;
   }
   else
   {
      frameTab[cursor][4] = 0;
      offset_x = -offset_x;
   }
      
   frameTab[cursor][5]  = offset_x >> 8;
   frameTab[cursor][6]  = offset_x & 0xff;
   
   frameTab[cursor][7]  = whichPort;
   frameTab[cursor][8]  = sog_time;
   frameTab[cursor][9]  = cog_time;
   
   crcRes  = Comm_getCRC(frameTab[cursor], 16);
   
   frameTab[cursor][16]  = crcRes  >> 8;
   frameTab[cursor][17]  = crcRes   & 0xff;
   
   flags  |= (0x01<<cursor);
   
   cursor  = (cursor+1) % 5;

}


uint8_t* Comm_fetchNextFrame(void)
{
   int  i;
   for(i=0; i<5; i++){
      if(flags & (0x01<<i)){
         flags &= (~(0x01<<i));
         return &(frameTab[i][0]);
      }
   }  
   return NULL;
}

int Comm_isflags(void)
{
   if(flags & 0x1F)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}



