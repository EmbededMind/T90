#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_gpdma.h"
#include "uart.h"
#include "timer.h"
#include "dma.h"
#include "GUI.h"
#include "dlg.h"
#include "MainTask.h" 
#include <ucos_ii.h>
#include "sound.h"
#include "T90.h"
#include "stub.h"
#include "comm.h"

#define NUM_BYTE     7
#define DST_X_BYTE   5 
#define DST_Y_BYTE   2
#define CMD_BYTE     1
#define VERSION1     1
#define VERSION2     0
#define VERSION3     5

extern unsigned char isDstSetChanged;            

/// If new plug event detected, var 'plugEvent' contain msg about that.
  

extern uint8_t SND[4][6];
extern int isKeyTrigged;

extern OS_EVENT * CommMBox;
extern OS_EVENT * updataMBox;


volatile Bool Doubleclick  = FALSE;
volatile Bool isReleasedDet  = FALSE;

uint8_t recBuf[31]  = {0};
uint8_t updata_r[18]  = {0};
uint8_t updata_s[18]  = {0};

uint8_t* pRecBuf  = recBuf;
uint8_t* pUpData  = updata_r;
void xl_UART_Config(unsigned char port)
{
   UART_CFG_Type      UARTConfigStruct;
   UART_FIFO_CFG_Type UARTFIFOConfigStruct;
   
   UART_ConfigStructInit(&UARTConfigStruct);
   UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
   
   switch(port)
   {
      case 0:
           PINSEL_ConfigPin(0,2,1);
           PINSEL_ConfigPin(0,3,1);
           UARTConfigStruct.Baud_rate  = 115200;
           
           UART_Init( (UART_ID_Type)port, &UARTConfigStruct );
           UART_FIFOConfig( (UART_ID_Type)port, &UARTFIFOConfigStruct );
           UART_TxCmd( (UART_ID_Type)port, ENABLE );      
           
           UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
           UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);           
           
           NVIC_SetPriority(UART0_IRQn, ((0x02<<3)|0x02));
	          NVIC_EnableIRQ(UART0_IRQn);          

           break;
      case 1:
				       PINSEL_ConfigPin(3,16,3);
			        PINSEL_ConfigPin(3,17,3);
			        UARTConfigStruct.Baud_rate = 115200;
			
			        UART_Init( (UART_ID_Type)port, &UARTConfigStruct );
           UART_FIFOConfig( (UART_ID_Type)port, &UARTFIFOConfigStruct );
           UART_TxCmd( (UART_ID_Type)port, ENABLE );
                     
           UART_IntConfig(UART_1, UART_INTCFG_RBR, ENABLE);
           UART_IntConfig(UART_1, UART_INTCFG_RLS, ENABLE); 
           
           NVIC_SetPriority(UART1_IRQn, ((0x02<<2)|0x02));
	         	NVIC_EnableIRQ(UART1_IRQn);
           break;     
      case 2:      
           PINSEL_ConfigPin(0,10,1);
           PINSEL_ConfigPin(0,11,1);

           /**  */
           PINSEL_ConfigPin(1, 19, 6); 

           UARTConfigStruct.Baud_rate = 9600;
           
           UART_Init( (UART_ID_Type)port, &UARTConfigStruct );
           
           LPC_UART2->RS485CTRL  = 0x31;
           LPC_UART2->RS485DLY   = 0x10;
           
           UART_FIFOConfig( (UART_ID_Type)port, &UARTFIFOConfigStruct );
           UART_TxCmd( (UART_ID_Type)port, ENABLE );
                     
           UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
           UART_IntConfig(UART_2, UART_INTCFG_RLS, ENABLE); 
           
           NVIC_SetPriority(UART2_IRQn, ((0x02<<3)|0x03));
	          NVIC_EnableIRQ(UART2_IRQn);      


           break;
           
     default:
           break;
   }
   

}


void USER_Init(void)
{	
 xl_UART_Config(2);
 xl_UART_Config(0);
 xl_UART_Config(1);
}


void UART0_IRQHandler(void)
{
   uint8_t tmpc;
   uint32_t  tmp, tmp1;
   tmp = ((LPC_UART0->IIR) & UART_IIR_BITMASK) & UART_IIR_INTID_MASK;
   if (tmp == UART_IIR_INTID_RLS)	// Receive Line Status
   {
     tmp1 = UART_GetLineStatus(UART_0);// Check line status
     tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);// Mask out the Receive Ready and Transmit Holding empty status
   }
   if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))	// Receive Data Available or Character time-out
   {	
       UART_Receive(UART_0, &tmpc, 1, NONE_BLOCKING);
      if(tmpc >= GUI_KEY_MENU  &&  tmpc <= GUI_KEY_PGDOWN)   
      {        
               isKeyTrigged  = 1;
               switch(tmpc)
               {                
                 case GUI_KEY_TRACE_ENABLE:
                   break;
                 case GUI_KEY_TRACE_DISABLE:
                   break;
                 default:
                    GUI_StoreKeyMsg(tmpc, 1);
                   break;
               }
//printf("%d\n",tmpc);               
      }   
      else if(tmpc >= 0x80  &&  tmpc <(0x80+21))
      {
         GUI_StoreKeyMsg(GUI_KEY_RELEASE, 1);
      }
      else
      {   
      
      }
   }
}

void UART1_IRQHandler(void)
{
//   uint16_t crcVal = 0;
   uint8_t tmpc = 0;
   uint32_t tmp, tmp1;
   tmp = ((LPC_UART1->IIR) & UART_IIR_BITMASK) & UART_IIR_INTID_MASK;
   if (tmp == UART_IIR_INTID_RLS)	// Receive Line Status
   {
      tmp1 = UART_GetLineStatus(UART_1);// Check line status
      tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);// Mask out the Receive Ready and Transmit Holding empty status
   }
   if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))	// Receive Data Available or Character time-out
   {	
      UART_Receive(UART_1, pUpData++, 1, NONE_BLOCKING);
      if(updata_r[0] == 0x24)
      {
         if(pUpData - updata_r >= 18)
         {
            if(updata_r[1] == 0x5A)
            {
//               crcVal = updata_r[16];
//               crcVal = (crcVal << 8) | updata_r[17];
//               if(crcVal == Comm_getCRC(updata_r, 16))
//               {
                  memset(updata_s,0,sizeof(uint8_t)*18);
                  updata_s[0] = 0x24;
                  updata_s[1] = 0x5A;
                  updata_s[2] = VERSION1;
                  updata_s[3] = VERSION2;
                  updata_s[4] = VERSION3;
//                  crcVal = Comm_getCRC(updata_s, 16);
//                  updata_s[16] = crcVal >> 8;
//                  updata_s[17] = crcVal & 0xff;
                  UART_Send(UART_1, updata_s, 18, BLOCKING);
//               }
                  pUpData = updata_r;
            }
            else if(updata_r[1] == 0x5B)
            {
//               crcVal = updata_r[16];
//               crcVal = (crcVal << 8) | updata_r[17];
//               if(crcVal == Comm_getCRC(updata_r, 16))
//               {
                  OSMboxPost(updataMBox, (void*)updata_r);
//               }
               pUpData = updata_r;
               memset(updata_r, 0, sizeof(uint8_t)*18);
            }
            else if(updata_r[1] == 0x5c)
            {
               memset(updata_s,0,sizeof(uint8_t)*18);
               updata_s[0] = 0x24;
               updata_s[1] = 0x5C;
               UART_Send(UART_1, updata_s, 18, BLOCKING);
               pUpData = updata_r;
            }
         }
      }
      else
      {
         pUpData = updata_r;   
      }
               
   }
}
void UART2_IRQHandler(void)
{
   uint16_t crcVal  = 0;
   uint32_t  tmp, tmp1;
   tmp = ((LPC_UART2->IIR) & UART_IIR_BITMASK) & UART_IIR_INTID_MASK;
   if (tmp == UART_IIR_INTID_RLS)	// Receive Line Status
   {
     tmp1 = UART_GetLineStatus(UART_2);// Check line status
     tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);// Mask out the Receive Ready and Transmit Holding empty status
   }
   else if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))	// Receive Data Available or Character time-out
   {	 
       UART_Receive(UART_2, pRecBuf++, 1, NONE_BLOCKING);
       if(recBuf[0] == 0x24)
       {
          if(pRecBuf-recBuf >=18){
             if(recBuf[1] != 0x51)
             {
                crcVal  = recBuf[16] ;
                crcVal  = (crcVal << 8) | recBuf[17];
              
                if(crcVal == Comm_getCRC(recBuf, 16)){
                   OSMboxPost(CommMBox, (void*)recBuf);
                }
                pRecBuf  = recBuf;
             }
             else if(recBuf[1] == 0x51)
             {
                if(pRecBuf - recBuf >= 31)
                {
                   crcVal  = recBuf[29];
                   crcVal  = (crcVal << 8) | recBuf[30];
                   
                   if(crcVal == Comm_getCRC(recBuf, 29))
                   {
                      OSMboxPost(CommMBox, (void*)recBuf);
                   }
                   pRecBuf  = recBuf;
                }
             }
          }
       }
       else
       {
          pRecBuf = recBuf;
       }
   }
   else if(tmp == UART_IIR_INTID_THRE){

   }
}


PUTCHAR_PROTOTYPE//重定向C printf函数到Uart0
{
	UART_Send((UART_ID_Type)UART_0, (uint8_t*) &ch, 1, BLOCKING);  /* 发送一个字符到UART */
	while(UART_CheckBusy((UART_ID_Type)UART_0) == SET)
      ;  /* 等于发送完成，THR必须为空*/
  return ch;
}

/************************************* End *************************************/
