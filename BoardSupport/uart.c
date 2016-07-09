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

#define NUM_BYTE     7
#define DST_X_BYTE   5 
#define DST_Y_BYTE   2
#define CMD_BYTE     1

extern unsigned char isDstSetChanged;            

/// If new plug event detected, var 'plugEvent' contain msg about that.
extern T90_PlugEvent plugEvent;  

extern uint8_t SND[4][6];
extern int isKeyTrigged;

extern OS_EVENT * CommMBox;



volatile Bool Doubleclick  = FALSE;
volatile Bool isReleasedDet  = FALSE;

uint8_t recBuf[18]  = {0};
uint8_t* pRecBuf  = recBuf;

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


void UART2_IRQHandler(void)
{
   uint8_t tmpc;
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

       
       /// CRC
       
       OSMboxPost(CommMBox, (void*)recBuf);

   }
   else if(tmp == UART_IIR_INTID_THRE){
      printf("uart2 send\n");
   }
}


PUTCHAR_PROTOTYPE//重定向C printf函数到Uart0
{
	UART_Send((UART_ID_Type)UART_0, (uint8_t*) &ch, 1, BLOCKING);  /* 发送一个字符到UART */
	while (UART_CheckBusy((UART_ID_Type)UART_0) == SET);/* 等于发送完成，THR必须为空*/
  return ch;
}

/************************************* End *************************************/
