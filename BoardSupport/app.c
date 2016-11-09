#include <ucos_ii.h>
#include "stdio.h"
#include "uart.h"
#include "app.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_timer.h"
#include "Config.h"
#include "DMA.h"
#include "uart.h"
#include "SPI2.h"
#include "SPI1.h"
#include "GUI.h"
#include "dlg.h"
#include "snap.h"
#include "comm.h"
#include "play_speech.h"

/*-------------------- Macro defines ---------------------*/
/* 定义任务优先级 */
#define UI_Task_PRIO             12
#define Insert_Task_PRIO         8
#define Refresh_Task_PRIO        9
#define Play_Task_PRIO           11
#define Comm_Task_PRIO           10


/* 定义任务堆栈大小 */
#define USER_TASK_STACK_SIZE  2048
#define TOUCH_TASK_STACK_SIZE 256
#define KEY_TASK_STACK_SIZE 128

#define PLAY_TAST_STACK_SIZE 128
#define COMM_TASK_STACK_SIZE 128

/*------------------- static ----------------------------*/
/* 定义任务堆栈 */
static	OS_STK	UI_Task_Stack[USER_TASK_STACK_SIZE];


static	OS_STK	Insert_Task_Stack[TOUCH_TASK_STACK_SIZE];

static	OS_STK	Refresh_Task_Stack[KEY_TASK_STACK_SIZE];


static   OS_STK Play_Task_Stack[PLAY_TAST_STACK_SIZE];

static   OS_STK Comm_Task_Stack[COMM_TASK_STACK_SIZE];


static uint8_t isNeedstubRefresh = 0;
OS_EVENT *  CommMBox;
OS_EVENT *  updataMBox;

//static  OS_STK_DATA UI_Task_Stack_Use;
//static  OS_STK_DATA Insert_Task_Stack_Use;
//static  OS_STK_DATA Refresh_Task_Stack_Use;


/*----------------- external function -------------------*/
void mntSetting_init(void);

/*----------------- Global   variables --------------------*/
///Insert , Refresh互斥信号量
      
///                                  _   _
/// 标记是否有新的插入事件(不要想歪  \ @ / )
///                                   ''' 


///
//unsigned char  isDstSetChanged  = 0;
//unsigned char  isDstSetNeedUpdate  = 0;
uint8_t ipcMsg  = 0;

PortStatus portStatus[3];

unsigned char isChecked  = 0;

//Bool monitorState  = FALSE;

int ReleasedDectSwitch  = 0;

OS_EVENT * Refresher;
OS_EVENT * Updater;

///--消息队列的定义部分---
OS_EVENT *QSem;//定义消息队列指针
void *MsgQeueTb[MSG_QUEUE_TABNUM];//定义消息指针数组，队列长度为10
void *ComQeueTab[5];
OS_MEM   *PartitionPt;//定义内存分区指针
// #pragma arm section rwdata = "SD_RAM2", zidata = "SD_RAM2"
uint8_t  Partition[MSG_QUEUE_TABNUM][100];
// #pragma arm section rwdata
// uint8_t  Partition[20][300]__attribute__((at(0xA1FF0000)));
uint8_t myErr;
uint8_t myErr_2;
int list_endIndex  = -1;

///* ADDRESS: 0xAC000000  SIZE: 0x400000  */
#pragma arm section rwdata = "SD_RAM1", zidata = "SD_RAM1"
BERTH Berthes[BOAT_NUM_MAX];
SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
_boat_m24A boat_list_24A[BOAT_NUM_MAX];
_boat_m24A *boat_list_p24A[BOAT_NUM_MAX];


#pragma arm section rwdata


BERTH Berthes[BOAT_NUM_MAX]__attribute__((at(0xA1D00000)));
SIMP_BERTH SimpBerthes[BOAT_NUM_MAX]__attribute__((at(0xA1E00000)));

_boat_m24A boat_list_24A[BOAT_NUM_MAX]__attribute__((at(0xA1F00000)));;
_boat_m24A *boat_list_p24A[BOAT_NUM_MAX]__attribute__((at(0xA1F80000)));


_boat_m24B boat_list_24B[BOAT_NUM_MAX];
_boat_m24B *boat_lisp_p24B[BOAT_NUM_MAX];

/*----------------- External functions -----------------------*/
extern int insert_18(struct message_18 * p_msg);
extern int insert_24A(struct message_24_partA * p_msg);
extern int insert_24B(type_of_ship * p_msg);
extern void updateTimeStamp(void);
extern void deleteBoat(long mmsi);
void setCategory(long mmsi, uint8_t category);
//extern void getMntWrapPara(long* pLg, long* pLt, map_scale* pScale);

/*----------------- External variables -----------------------*/
boat mothership;
mapping center;

extern volatile int xlCnt;

//extern long MapPara_lg;
//extern long MapPara_lt;
//extern map_scale MapPara_scale;


struct message_18 msg_18;

int N_boat = 0;
/*----------------- local   function  --------------------*/


void SysTick_Init(void);



void UI_Task(void *p_arg)/*描述(Description):	任务UI_Task*/
{      
		 MainTask();
}



void Insert_Task(void *p_arg)  //等待接收采集到的数据
{ 
   int tmp  = 0;
   uint8_t *s; 
   INT8U err;
   message_18 text_out;
   message_24_partA text_out_24A;
   type_of_ship text_out_type_of_ship; 
  // USER_Init();
   while(1)
   {
//printf("Insert while begin\n");      
      s = OSQPend(QSem,0,&err);    
      tmp  = translate_(s,&text_out,&text_out_24A,&text_out_type_of_ship); 
      OSMutexPend(Refresher, 0, &myErr);        
      switch(tmp)
      {
         case 18:       
            insert_18(&text_out);
         break;
         case 240:         
            insert_24A(&text_out_24A);
         break;
         case 241:            
            insert_24B(&text_out_type_of_ship);       
         break;
         default:
         break;
      }
    OSMutexPost(Refresher);
//printf("Insert while end\n");      
    OSTimeDly(20); 

   }
}


void Refresh_Task(void *p_arg)//任务Refresh_Task
{
   while(1)
   {
//printf("Refresh while begin\n");
      OSMutexPend(Refresher, 0, &myErr);           
      updateTimeStamp();    
      check();
      OSMutexPost(Refresher);
      
      isChecked  = 1;
       
      if(isNeedstubRefresh)
      {
         Stub_setValidity(1, portStatus[0].port == 1?1:0);
         Stub_setValidity(2, portStatus[1].port == 1?1:0);
         Stub_setValidity(3, portStatus[2].port == 1?1:0);
         StubRefresh();
      }   
//printf("Refresh while end\n");
      OSTimeDlyHMSM(0,0,5,0);

   }
}


   
void _Play_Task(void* p_arg)
{
   play_speech();
}
 

void Comm_Task(void * p_arg) 
{
   INT8U err;
   uint8_t* pFrame;
   
   uint8_t  pulseNoAckCnt  = 0;
   uint8_t  dataNoAckCnt   = 0;
   
   long SOG, COG;
   while(1)
   {
//printf("Comm Task while begin\n");
      if(Comm_isflags())
      {
         pFrame  = Comm_fetchNextFrame();
         if(pFrame[1] == 0x31){

LOL:
            Comm_sendFrame(pFrame);
            pFrame  = (uint8_t*)OSMboxPend(CommMBox, 200, &err);
            if(err == OS_ERR_NONE && (pFrame[1] == 0x31)){
               dataNoAckCnt  = 0;
               ipcMsg  |= 0x20;
            }
            else{
               dataNoAckCnt++;
               if(dataNoAckCnt > 3){
                  dataNoAckCnt  = 0;
                  ipcMsg  |= 0x10;
               }
               else{
                  goto LOL;
               }
            }
            
         }
      }
      else
      {       
         Comm_sendPulse();
         pFrame  = (uint8_t*)OSMboxPend(CommMBox, 200, &err);
         
         if(err == OS_ERR_NONE){
            /** 判断ACK类型，取得三个端口的状态 */
            if(pFrame[1] == 0x51){   /// 判断是否是来自于T81的消息
               if(pFrame[2] == 1)
               {
                  long shiftReg = 0;
                  char recPort = 0;
                  pulseNoAckCnt  = 0;

                  /// Port 1 changed!
                  shiftReg  = pFrame[3];
                  shiftReg  = shiftReg<<8|pFrame[4];
                  shiftReg  = shiftReg<<8|pFrame[5];
                  shiftReg  = shiftReg<<8|pFrame[6];
                  recPort  = (pFrame[15]>>6)&0x03;
                
                  if(shiftReg != portStatus[0].MMSI)
                  {
                     portStatus[0].MMSI  = shiftReg;
                  }
                               
                  if(recPort != portStatus[0].port ){
                     isNeedstubRefresh = 1;
                     portStatus[0].port  = recPort;
                     
                     if(recPort == 1)
                     {
                        StubRefresh();                     
                        if(t90_set.sys.workmode == SINGLE_MODE || t90_set.sys.motherpos == DEFAULT_LEFT)
                        {
                           Comm_addFrame(1,stubs[1].basePoint.x*MILLINM_TO_M,abs(stubs[1].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        }
                        else
                        {
                           Comm_addFrame(1,(stubs[1].basePoint.x - stubs[4].basePoint.x)*MILLINM_TO_M,abs(stubs[1].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                     ipcMsg  |= 0x01;               
                  }

      //printf("stubs[1].isValid %d\n",stubs[1].isValid);
                  /// Port 2 changed!
                  shiftReg  = pFrame[7];  
                  shiftReg  = shiftReg<<8|pFrame[8]; 
                  shiftReg  = shiftReg<<8|pFrame[9];
                  shiftReg  = shiftReg<<8|pFrame[10];
                  recPort  = (pFrame[15]>>4)&0x03;
                  
                  if(shiftReg != portStatus[1].MMSI)
                  {
                     portStatus[1].MMSI = shiftReg;
                  }
                  
                  if(recPort != portStatus[1].port ){
                     isNeedstubRefresh = 1;
                     portStatus[1].port  = recPort;
                     if(recPort == 1)
                     {
                        StubRefresh();                  
                        if(t90_set.sys.workmode == SINGLE_MODE || t90_set.sys.motherpos == DEFAULT_LEFT)
                        {
                           Comm_addFrame(2,stubs[2].basePoint.x*MILLINM_TO_M,abs(stubs[2].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        }
                        else
                        {
                           Comm_addFrame(2,(stubs[2].basePoint.x - stubs[4].basePoint.x)*MILLINM_TO_M,abs(stubs[2].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                     ipcMsg  |= 0x02; 
                  }

                  /// Port 3 changed!
                  shiftReg  = pFrame[11];  
                  shiftReg  = shiftReg<<8|pFrame[12]; 
                  shiftReg  = shiftReg<<8|pFrame[13];
                  shiftReg  = shiftReg<<8|pFrame[14];
                  recPort  = (pFrame[15]>>2)&0x03;
                  
                  if(shiftReg != portStatus[2].MMSI)
                  {
                     portStatus[2].MMSI = shiftReg;
                  }
                  
                  if(recPort != portStatus[2].port ){
                     isNeedstubRefresh = 1;
                     portStatus[2].port  = recPort;
                     if(recPort == 1)
                     {
                        StubRefresh();
                        if(t90_set.sys.workmode == SINGLE_MODE || t90_set.sys.motherpos == DEFAULT_LEFT)
                        {
                           Comm_addFrame(3,stubs[3].basePoint.x*MILLINM_TO_M,abs(stubs[3].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        }
                        else
                        {
                           Comm_addFrame(3,(stubs[3].basePoint.x - stubs[4].basePoint.x)*MILLINM_TO_M,abs(stubs[3].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                     ipcMsg  |= 0x04; 
                  }

                  SOG = pFrame[16];
                  SOG = SOG<<8 | pFrame[17];
                  mothership.SOG = SOG;

                  COG = pFrame[18];
                  COG = COG<<8 | pFrame[19];
                  mothership.COG = COG;
              
               }
               else if(pFrame[2] == 2)
               {
                  long shiftReg;
                  long tmplo = 1,tmpla = 1;  
                
                  if(pFrame[3] == 'E')
                  {
                     MS_EWNS |= 0x10;
                  }
                  else if(pFrame[3] == 'W')
                  {
                     MS_EWNS &= 0x01;
                  }                   
                 
                  shiftReg = pFrame[4];
                  shiftReg = (shiftReg << 8)|pFrame[5];
                  shiftReg = (shiftReg << 8)|pFrame[6];
                  shiftReg = (shiftReg << 8)|pFrame[7];
                  if(shiftReg != 0)
                     mothership.longitude = shiftReg/10;
                  tmplo = shiftReg;
                  
                  if(pFrame[8] == 'N')
                  {
                     MS_EWNS |= 0x01;
                  }
                  else if(pFrame[8] == 'S')
                  {
                     MS_EWNS &= 0x10;
                  }   
                  
                  shiftReg = pFrame[9];
                  shiftReg = (shiftReg << 8)|pFrame[10];
                  shiftReg = (shiftReg << 8)|pFrame[11];
                  shiftReg = (shiftReg << 8)|pFrame[12];
                  if(shiftReg != 0)
                     mothership.latitude = shiftReg/10;
                  tmpla = shiftReg;
                  
                  if(tmpla || tmplo)
                  {                   
                     shiftReg = pFrame[13];
                     shiftReg = (shiftReg << 8)|pFrame[14];
                     shiftReg = (shiftReg << 8)|pFrame[15];
                     shiftReg = (shiftReg << 8)|pFrame[16];
                     SYS_Time = shiftReg;
                     
                     shiftReg = pFrame[17];
                     shiftReg = (shiftReg << 8)|pFrame[18];
                     shiftReg = (shiftReg << 8)|pFrame[19];
                     shiftReg = (shiftReg << 8)|pFrame[20];
                     SYS_Date = shiftReg;
                  }
               }                
            }
         }
         else{
            if(pulseNoAckCnt <= 3){         
               pulseNoAckCnt++;
               if(pulseNoAckCnt == 4){
                  memset(&portStatus,0,sizeof(portStatus[1])*3);
                  Stub_setValidity(1, 0);
                  Stub_setValidity(2, 0);
                  Stub_setValidity(3, 0);
                  ipcMsg  |= 0x40;
                  /// Ack err
               }
            }
         }
      }
      
//printf("Comm Task while end\n");
      OSTimeDlyHMSM(0, 0, 1, 0);

   }
}
 

void App_TaskStart(void)//初始化UCOS，初始化SysTick节拍，并创建三个任务
{
   INT8U err;

   mothership.latitude = MOTHERSHIP_LA;
   mothership.longitude = MOTHERSHIP_LG;
   mothership.true_heading  = 0;


   T90_Init();
   GPDMA_Init();

   //  UART2_DMA_Init();


   SPI2_Int();
   SPI2_DMA_Init();

   SPI1_Int();

   ISD_Init();


   OSInit();  
   SysTick_Init();/* 初始化SysTick定时器 */
   Refresher  = OSMutexCreate(6,&myErr);
   Updater    = OSMutexCreate(6,&myErr_2);
   QSem = OSQCreate(&MsgQeueTb[0],MSG_QUEUE_TABNUM); //创建消息队列，10条消息

   //  ComQSem  = OSQCreate(&ComQeueTab[0], 5);
   CommMBox = OSMboxCreate(0);
   updataMBox = OSMboxCreate(0);

   PartitionPt=OSMemCreate(Partition,MSG_QUEUE_TABNUM,100,&err);

   OSTaskCreateExt(     UI_Task, 
                       (void *)0,
                       (OS_STK *)&UI_Task_Stack[USER_TASK_STACK_SIZE-1],  
                       UI_Task_PRIO, UI_Task_PRIO, 
                       (OS_STK *)&UI_Task_Stack[0], 
                       USER_TASK_STACK_SIZE,
                       (void*)0, 
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* 创建任务 UI_Task */
                       
   OSTaskCreateExt(     Insert_Task,
                       (void *)0,
                       (OS_STK *)&Insert_Task_Stack[TOUCH_TASK_STACK_SIZE-1],
                       Insert_Task_PRIO,
                       Insert_Task_PRIO,
                       (OS_STK *)&Insert_Task_Stack[0],
                       TOUCH_TASK_STACK_SIZE,
                       (void*)0,
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* 创建任务 Insert_Task */
                       
   OSTaskCreateExt(     Refresh_Task,   
                       (void *)0,
                       ( OS_STK *)&Refresh_Task_Stack[KEY_TASK_STACK_SIZE-1],    
                       Refresh_Task_PRIO,  
                       Refresh_Task_PRIO  ,
                       (OS_STK *)&Refresh_Task_Stack[0],  
                       KEY_TASK_STACK_SIZE,
                       (void*)0,  
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);/* 创建任务 Refresh_Task */
                       
   OSTaskCreateExt(     _Play_Task,
                       (void*)0,
                       (OS_STK*)&Play_Task_Stack[PLAY_TAST_STACK_SIZE-1],
                       Play_Task_PRIO,
                       Play_Task_PRIO,
                       (OS_STK*)&Play_Task_Stack[0],
                       PLAY_TAST_STACK_SIZE,
                       (void*)0,
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );

   OSTaskCreateExt(     Comm_Task,
                       (void*)0,
                       (OS_STK*)&Comm_Task_Stack[COMM_TASK_STACK_SIZE-1],
                       Comm_Task_PRIO,
                       Comm_Task_PRIO,
                       (OS_STK*)&Comm_Task_Stack[0],
                       COMM_TASK_STACK_SIZE,
                       (void*)0,
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );
   OSStart();
}

//		switch(translate_(s,&text_out,&text_out_24A,&text_out_type_of_ship))
int translate_(unsigned char *text,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship)
{
  int i=0,comma=0;
  int tmp  = 0;

  long shiftReg  = 0;
  
  if((text[0]!='!')&&(text[0]!='$'))
     return 0;
  if((text[1]=='A')&&(text[2]=='I')&&(text[3]=='V')&&(text[4]=='D')&&(text[5]=='M'))
  { 
     for(i=6; i<20; i++)
     {
        if(text[i] == ',')
        {
           comma++;
           if(comma == 5)
              break;
        }
     }    
     if(i<20)
     {     
          i++;     
          tmp  = change_table(text[i]);         
          switch(tmp)
          {
            case 19: 
                 (*text_out)  = translate_m18(text,i);
                 return 18;
            case 18:
                 (*text_out)=translate_m18(text,i);
                 return 18;           
            case 24:            
                if(change_table(text[i+6])&12)
                {
                   *text_out_type_of_ship = translate_m24B(text,i);
                   return 241;
                }
                else
                {               
                   *text_out_24A = translate_m24A(text,i); 
                   return 240;
                }                            
           default:          
                return tmp;          
         }
      }
      else
      {
         return 0;
      }
   }
   return 0;
}

/************************************* End *************************************/






