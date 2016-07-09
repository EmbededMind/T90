#include <ucos_ii.h>
#include "stdio.h"
#include "uart.h"
#include "app.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_timer.h"
#include "Config.h"
//#include "Setting.h"
#include "DMA.h"
#include "Check.h"
#include "uart.h"
#include "SPI2.h"
#include "SPI1.h"
#include "GUI.h"
#include "dlg.h"
#include "sound.h"
#include "bully.h"
#include "xt_isd.h"
#include "T90.h"
#include "snap.h"
#include "transform.h"
#include "stub.h"
#include "comm.h"

//#ifndef test_test
//	#define test_test
//#endif

/*-------------------- Macro defines ---------------------*/
/* ¶¨ÒåÈÎÎñÓÅÏÈ¼¶ */
#define UI_Task_PRIO             12
#define Insert_Task_PRIO         8
#define Refresh_Task_PRIO        9
#define Play_Task_PRIO           11
#define Comm_Task_PRIO           10


/* ¶¨ÒåÈÎÎñ¶ÑÕ»´óÐ¡ */
#define USER_TASK_STACK_SIZE 2048
#define TOUCH_TASK_STACK_SIZE 256
#define KEY_TASK_STACK_SIZE 128

#define PLAY_TAST_STACK_SIZE 128
#define COMM_TASK_STACK_SIZE 128

/*------------------- static ----------------------------*/
/* ¶¨ÒåÈÎÎñ¶ÑÕ» */
static	OS_STK	UI_Task_Stack[USER_TASK_STACK_SIZE];


static	OS_STK	Insert_Task_Stack[TOUCH_TASK_STACK_SIZE];

static	OS_STK	Refresh_Task_Stack[KEY_TASK_STACK_SIZE];


static OS_STK Play_Task_Stack[PLAY_TAST_STACK_SIZE];

static OS_STK Comm_Task_Stack[COMM_TASK_STACK_SIZE];


OS_EVENT *  CommMBox;
//static  OS_STK_DATA UI_Task_Stack_Use;
//static  OS_STK_DATA Insert_Task_Stack_Use;
//static  OS_STK_DATA Refresh_Task_Stack_Use;


#define MUSIC_ADD(x)  if(x==0) \
                         musics[musicCursor]  = SND_ID_ZRO; \
                      else \
                         musics[musicCursor]  = x; \
                      musicCursor++
                      
#define MUSIC_RESET   musicCursor  = 0

#define MUSIC_ADD_5NUMS if(aNums[0]) \
                        { \
							  MUSIC_ADD(aNums[0]); \
						} \
                        if(aNums[1]) \
						{ \
                            MUSIC_ADD(aNums[1]); \
						} \
                        if(aNums[2]) \
						{ \
                            MUSIC_ADD(aNums[2]); \
						} \
                        if(aNums[3]) \
						{ \
                            MUSIC_ADD(aNums[3]); \
						} \
                        if(aNums[4]) \
						{ \
                            MUSIC_ADD(aNums[4]); \
						} 													

/*----------------- external function -------------------*/
void mntSetting_init(void);

/*----------------- Global   variables --------------------*/
///Insert , Refresh»¥³âÐÅºÅÁ¿
int isKeyTrigged  = 0;
///                                  _   _
/// ±ê¼ÇÊÇ·ñÓÐÐÂµÄ²åÈëÊÂ¼þ(²»ÒªÏëÍá  \ @ / )
///                                   ''' 
T90_PlugEvent plugEvent;

///
unsigned char  isDstSetChanged  = 0;
unsigned char  isDstSetNeedUpdate  = 0;

long gPlugBoats[3];

unsigned char isChecked  = 0;

//Bool monitorState  = FALSE;

int ReleasedDectSwitch  = 0;

OS_EVENT * Refresher;
OS_EVENT * Updater;

///--ÏûÏ¢¶ÓÁÐµÄ¶¨Òå²¿·Ö---
OS_EVENT *QSem;//¶¨ÒåÏûÏ¢¶ÓÁÐÖ¸Õë
OS_EVENT *ComQSem;
void *MsgQeueTb[MSG_QUEUE_TABNUM];//¶¨ÒåÏûÏ¢Ö¸ÕëÊý×é£¬¶ÓÁÐ³¤¶ÈÎª10
void *ComQeueTab[5];
OS_MEM   *PartitionPt;//¶¨ÒåÄÚ´æ·ÖÇøÖ¸Õë
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

void detectPlugEvent(void);

void sendPulse();

void UI_Task(void *p_arg)/*ÃèÊö(Description):	ÈÎÎñUI_Task*/
{      
		MainTask();
}



void Insert_Task(void *p_arg)  //µÈ´ý½ÓÊÕ²É¼¯µ½µÄÊý¾Ý
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
    OSTimeDly(20); 

   }
}


void Refresh_Task(void *p_arg)//ÈÎÎñRefresh_Task
{
   while(1)
   {
//      int i = 1;
      OSMutexPend(Refresher, 0, &myErr);     
      detectPlugEvent();      
      updateTimeStamp();    
      check();
      OSMutexPost(Refresher);

      
//      OSMboxPost(MSBOX,&i);
      isChecked  = 1;
      sendPulse();
      
      OSTimeDlyHMSM(0,0,5,0);
   }
}


   
void _Play_Task(void* p_arg)
{
   uint8_t  musics[30];
   uint8_t  musicCursor  = 0; 
   uint8_t  Nums[3];
   uint8_t  aNums[5];	
   int angle;   
   uint8_t playList  = 1;  
   BULY_BERTH* thisBulyBerth  = NULL;
   BERTH * thisinvdBerth = NULL; 
   
   ISD_Wait_PWRUp();    
   ISD_SetVolumn(t90_set.sys.volum);
   MUSIC_RESET;
   ISD_Play(SND_ID_WLCM);
   ISD_PWRDn();      
   while(1)
   {
       
        if(FetchSTime() == 0)
        {
              MUSIC_ADD(SND_ID_STOF);
        }
         else
         {
								if(monitorState == ON) // Ý à ˜
		            {  
                   thisBulyBerth  = BULY_fetchNextPlayBerth();
                   if(thisBulyBerth){                             
                   if((thisBulyBerth->pBoatLink->Boat.category & 0xf0) > 0){    //ÓæÕþ´¬
                       switch(thisBulyBerth->pBoatLink->Boat.category & 0xf0){
                              case NATION_CTB:
                                   MUSIC_ADD(SND_ID_CTB);
                                   break;
                              case NATION_JPN:
                                   MUSIC_ADD(SND_ID_JPN);
                                   break;
                              case NATION_KOR:
                                   MUSIC_ADD(SND_ID_KOR);                  
                                   break;
                              case NATION_PRK:
                                   MUSIC_ADD(SND_ID_PRK);
                                   break;
                              case NATION_INA:
                                   MUSIC_ADD(SND_ID_INA);
                                   break;
                              case NATION_VIE:
                                   MUSIC_ADD(SND_ID_VIE);
                                   break;
                       }                                  
                   angle = getAngleOfShip(thisBulyBerth->pBoatLink);
                   if(angle>=0 && angle<360)
                        {
                             SND_ParseNum(angle*1000,aNums);
                             MUSIC_ADD(SND_ID_ANG);                                   
                             MUSIC_ADD_5NUMS;
                             MUSIC_ADD(SND_ID_DEG);
                          }                                                    
                          if(thisBulyBerth->pBoatLink->Boat.dist < 99999){
                               SND_ParseNum((t90_set.sys.unit == NM)? thisBulyBerth->pBoatLink->Boat.dist/100*100 : thisBulyBerth->pBoatLink->Boat.dist * 37/20/100*100, aNums);
                               MUSIC_ADD(SND_ID_DST);      
                               MUSIC_ADD_5NUMS;                                   
                               if(t90_set.sys.unit == NM)
                               {   
                                   MUSIC_ADD(SND_ID_NM);
                               }
                               else
                               {
                                   MUSIC_ADD(SND_ID_KM);
                               }
                               MUSIC_ADD(SND_ID_SIS);                                  
                               SND_ParseNum(thisBulyBerth->pBoatLink->Boat.SOG *100, aNums);
                               MUSIC_ADD_5NUMS;
                               MUSIC_ADD(SND_ID_KT);
                          }
                     }
                     else{                                      // ¸ßËÙ´¬
                          MUSIC_ADD(SND_ID_HSB);
                          angle = getAngleOfShip(thisBulyBerth->pBoatLink);
                          if(angle>=0 && angle<360)
                          {
                               SND_ParseNum(angle*1000,aNums);
                               MUSIC_ADD(SND_ID_ANG);                              
                               MUSIC_ADD_5NUMS;
                               MUSIC_ADD(SND_ID_DEG);
                           }                                                 
                           MUSIC_ADD(SND_ID_DST);     
                           SND_ParseNum((t90_set.sys.unit == NM)? thisBulyBerth->pBoatLink->Boat.dist/100*100 : thisBulyBerth->pBoatLink->Boat.dist * 37/20/100*100, aNums);
                           MUSIC_ADD_5NUMS;
                           if(t90_set.sys.unit == NM)
                           {   
                                MUSIC_ADD(SND_ID_NM);
                           }
                           else
                           {
                                MUSIC_ADD(SND_ID_KM);
                           }
                           }
                        }
                //            playList  = 2;
                //        }
                            
                        else //if(playList == 2)
                           {
                               if(playList == 1)
                               {    
                                    thisinvdBerth = SIMP_BERTH_fetchNextPlayBerth();
                                    if(thisinvdBerth)
                                    {
                                         MUSIC_ADD(SND_ID_INVD);
                                         if(thisinvdBerth->y_to_cross > FetchMidStub())
                                         {
                                              MUSIC_ADD(SND_ID_MS);
                                         }
                                         else
                                         {
                                              MUSIC_ADD(SND_ID_NET);
                                         }                                    
                                         angle = getAngleOfShip(thisinvdBerth);                                           
                                         if(angle>=0 && angle<360)
                                         {
                                             SND_ParseNum(angle*1000,aNums);
                                             MUSIC_ADD(SND_ID_ANG);                                        
                                             MUSIC_ADD_5NUMS;
                                             MUSIC_ADD(SND_ID_DEG);
                                         }                                         
                                        MUSIC_ADD(SND_ID_DST);
                                        SND_ParseNum((t90_set.sys.unit == NM)? thisinvdBerth->Boat.dist/100*100 : thisinvdBerth->Boat.dist * 37/20/100*100, aNums);																	 
                                        MUSIC_ADD_5NUMS;
                                        if(t90_set.sys.unit == NM)
                                        {   
                                             MUSIC_ADD(SND_ID_NM);
                                        }
                                        else
                                        {
                                             MUSIC_ADD(SND_ID_KM);
                                        }
                                    }
                                    playList  = 2;
                                    OSTimeDlyHMSM(0, 0, 2, 0);
                                }
                                else if(playList == 2)
                                {
                                    if(MS_isSpeeding == MNTState_Triggered)
                                    {                                     
                                        SND_ParseNum(t90_set.alarm.danger_sog*100,aNums);
                                        MUSIC_ADD(SND_ID_MHS);
                                        MUSIC_ADD_5NUMS;
                                        MUSIC_ADD(SND_ID_KT);                 
                                    }
                                    else
                                    {
                                        if(MS_isMax_SOG == MNTState_Triggered)
                                        {
                                            SND_ParseNum(mothership.SOG*100,aNums);
                                            MUSIC_ADD(SND_ID_SN);                                   
                                            MUSIC_ADD_5NUMS;
                                            MUSIC_ADD(SND_ID_KT);
                                            MUSIC_ADD(SND_ID_HIGH);
                                            MUSIC_ADD(SND_ID_SNOR);
                                        }
                                        else if(MS_isMin_SOG == MNTState_Triggered)
                                        {
                                            SND_ParseNum(mothership.SOG*100,aNums);
                                            MUSIC_ADD(SND_ID_SN);                                  
                                            MUSIC_ADD_5NUMS;
                                            MUSIC_ADD(SND_ID_KT);
                                            MUSIC_ADD(SND_ID_LOW);
                                            MUSIC_ADD(SND_ID_SNOR);                                     
                                        }
                                   }
                                   playList = 1;
                               }
                          }
                       }
					 }
                      if(musicCursor){
                      int i  = 0;
                          
                      ISD_Wait_PWRUp();             
                      for(i=0; i< musicCursor; i++){
                          int timeOutCnt  = 0;

                          ISD_Play(musics[i]);
                          while(ISD_IsBusy()){
                              if(timeOutCnt > 15){
                                  timeOutCnt  = 0;
                                  break;
                          }
                          timeOutCnt ++;
                          OSTimeDlyHMSM(0, 0, 0, 200);
                          }
                      }
                      ISD_PWRDn();            
                      MUSIC_RESET;                // ÏÂ±êÖÃ0         
                 } /// End. execute play 
             /// End . if(monitorState == FALSE)    
            OSTimeDlyHMSM(0, 0, 3, 0);
     } /// 'End'. while(1).In fact this will not happen
  
}
 

void Comm_Task(void * p_arg) 
{
   INT8U err;
   uint8_t* pFrame;
   
   uint8_t  pulseNoAckCnt  = 0;
   uint8_t  dataNoAckCnt   = 0;
   
   while(1)
   {
      
      Comm_sendPulse();
      pFrame  = (uint8_t*)OSMboxPend(CommMBox, 1000, &err);
      
      if(err == OS_ERR_NONE){
         /** ÅÐ¶ÏACKÀàÐÍ£¬È¡µÃÈý¸ö¶Ë¿ÚµÄ×´Ì¬ */
         if(pFrame[1] == 0x5a){
            pulseNoAckCnt  = 0;
            printf("pulse ack ok\n");
         }
      }
      else{
         pulseNoAckCnt++;
         if(pulseNoAckCnt > 3){
            pulseNoAckCnt  = 0;
            printf("pulse ack timeout more than 3 times\n");
            /// Ack err
         }
      }
      
      pFrame  = Comm_fetchNextFrame();
      if(pFrame){
         Comm_sendFrame(pFrame);
         pFrame  = (uint8_t*)OSMboxPend(CommMBox, 1000, &err);{
            if(err == OS_ERR_NONE){
               dataNoAckCnt  = 0;
               printf("data ack ok\n");
            }
            else{
               dataNoAckCnt++;
               if(dataNoAckCnt > 3){
                  dataNoAckCnt  = 0;
                  printf("data ack timeout more than 3 times\n");
               }
            }
         }
      }
      OSTimeDlyHMSM(0, 0, 1, 0);
   }
}
 

void App_TaskStart(void)//³õÊ¼»¯UCOS£¬³õÊ¼»¯SysTick½ÚÅÄ£¬²¢´´½¨Èý¸öÈÎÎñ
{
  INT8U err;

  mothership.latitude = MOTHERSHIP_LA;
  mothership.longitude = MOTHERSHIP_LG;
  mothership.true_heading  = 0;
  

  T90_Init();
printf("init motoas=%d\n",t90_set.motoas);
  GPDMA_Init();
  
//  UART2_DMA_Init();

  
  SPI2_Int();
  SPI2_DMA_Init();  
	
  SPI1_Int();

  ISD_Init();
  

  
  OSInit();  
  SysTick_Init();/* ³õÊ¼»¯SysTick¶¨Ê±Æ÷ */
  Refresher  = OSMutexCreate(6,&myErr);
  Updater    = OSMutexCreate(6,&myErr_2);
  QSem = OSQCreate(&MsgQeueTb[0],MSG_QUEUE_TABNUM); //´´½¨ÏûÏ¢¶ÓÁÐ£¬10ÌõÏûÏ¢
  ComQSem  = OSQCreate(&ComQeueTab[0], 5);
  CommMBox = OSMboxCreate(0);
  
  PartitionPt=OSMemCreate(Partition,MSG_QUEUE_TABNUM,100,&err);
  
  OSTaskCreateExt(     UI_Task, 
                       (void *)0,
                       (OS_STK *)&UI_Task_Stack[USER_TASK_STACK_SIZE-1],  
                       UI_Task_PRIO, UI_Task_PRIO, 
                       (OS_STK *)&UI_Task_Stack[0], 
                       USER_TASK_STACK_SIZE,
                       (void*)0, 
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* ´´½¨ÈÎÎñ UI_Task */
                       
  OSTaskCreateExt(     Insert_Task,
                       (void *)0,
                       (OS_STK *)&Insert_Task_Stack[TOUCH_TASK_STACK_SIZE-1],
                       Insert_Task_PRIO,
                       Insert_Task_PRIO,
                       (OS_STK *)&Insert_Task_Stack[0],
                       TOUCH_TASK_STACK_SIZE,
                       (void*)0,
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* ´´½¨ÈÎÎñ Insert_Task */
                       
  OSTaskCreateExt(     Refresh_Task,   
                       (void *)0,
                       ( OS_STK *)&Refresh_Task_Stack[KEY_TASK_STACK_SIZE-1],    
                       Refresh_Task_PRIO,  
                       Refresh_Task_PRIO  ,
                       (OS_STK *)&Refresh_Task_Stack[0],  
                       KEY_TASK_STACK_SIZE,
                       (void*)0,  
                       OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);/* ´´½¨ÈÎÎñ Refresh_Task */
                       
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

	else if((text[4]=='M')&&(text[5]=='C')) //GPS GPRMC
	{ 
    shiftReg   = text[6];
    shiftReg   = (shiftReg << 8) | text[7];
    shiftReg   = (shiftReg << 8) | text[8];
    shiftReg   = (shiftReg << 8) | text[9];
    if(shiftReg )
       mothership.latitude  = shiftReg / 10;
      
    shiftReg   = text[10];
    shiftReg   = (shiftReg << 8) | text[11];
    shiftReg   = (shiftReg << 8) | text[12];
    shiftReg   = (shiftReg << 8) | text[13];
    if(shiftReg)
       mothership.longitude  = shiftReg / 10;
    
    shiftReg   = text[14];
    shiftReg   = (shiftReg << 8) | text[15];
    mothership.SOG = shiftReg;
   

    shiftReg   = text[16];
    shiftReg   = (shiftReg << 8) | text[17];
    mothership.COG = shiftReg /10;


    shiftReg   = text[18];
    shiftReg   = (shiftReg << 8) | text[19];
    shiftReg   = (shiftReg << 8) | text[20];
    shiftReg   = (shiftReg << 8) | text[21];
    SYS_Date   = shiftReg;


    shiftReg   = text[22];
    shiftReg   = (shiftReg << 8) | text[23];
    shiftReg   = (shiftReg << 8) | text[24];
    shiftReg   = (shiftReg << 8) | text[25];
    SYS_Time   = shiftReg;
	}

  return 0;
}

/************************************* End *************************************/



void detectPlugEvent()
{
      if(isDstSetChanged == 0){
         return ;
      }
      isDstSetChanged   = 0;
      isDstSetNeedUpdate++;
//   if(plugEvent.eventType == PGEvent_Data){
//      if(plugEvent.whichPort & 0x01){
         
         if(plugEvent.status & 0x01){
            Stub_setParam(1, plugEvent.dist_1, plugEvent.dist_2);
            Stub_setValidity(1, TRUE);
            if(gPlugBoats[0] != plugEvent.mmsi[0] && gPlugBoats[1] != plugEvent.mmsi[0] && gPlugBoats[2] != plugEvent.mmsi[0]){
               gPlugBoats[0]  = plugEvent.mmsi[0];
               setCategory(gPlugBoats[0], TYPE_SAFETY);
            }
         }
         else{
            Stub_setValidity(1, FALSE);
            if(gPlugBoats[0] > 0){
               deleteBoat(gPlugBoats[0]);
            }           
         }
//      }
//      if(plugEvent.whichPort & 0x02){
         if(plugEvent.status & 0x04){
            Stub_setParam(2, 0, plugEvent.dist_4); 
            Stub_setValidity(2, TRUE);
            if(gPlugBoats[0] != plugEvent.mmsi[1] && gPlugBoats[1] != plugEvent.mmsi[1] && gPlugBoats[2] != plugEvent.mmsi[1]){
               gPlugBoats[1]  = plugEvent.mmsi[1];
               setCategory(gPlugBoats[1], TYPE_SAFETY);
            }
         }
        else{
           Stub_setValidity(2, FALSE);
           if(gPlugBoats[1] > 0){
              deleteBoat(gPlugBoats[1]);
           }
        }        
//      }
//      if(plugEvent.whichPort & 0x04){
         if(plugEvent.status & 0x10){
            Stub_setParam(3, plugEvent.dist_5, plugEvent.dist_6);
            Stub_setValidity(3, TRUE); 
            if(gPlugBoats[0] != plugEvent.mmsi[2] && gPlugBoats[1] != plugEvent.mmsi[2] && gPlugBoats[2] != plugEvent.mmsi[2]){
               gPlugBoats[2]  = plugEvent.mmsi[2];
               setCategory(gPlugBoats[2], TYPE_SAFETY);
            }            
         }
         else{
            Stub_setValidity(3, FALSE);
            if(gPlugBoats[2] > 0){            
               deleteBoat(gPlugBoats[2]);
            }
         }

      plugEvent.whichPort  = 0;    
      StubRefresh();  
}


uint16_t msg_crc(uint8_t *ptr,uint8_t num)
{
   uint16_t crc=0xffff;
   uint8_t i;
   uint16_t gx=0x1021;
   
   while(num--)
   {
    for(i=0x01;i!=0;i<<=1)
    {
     if((crc&0x8000)!=0)
     {
      crc<<=1;
      crc^=gx;
     }
     else
     {
      crc<<=1;
     }
     if(((*ptr)&i)!=0) 
     {
      crc^=gx;
     }
    }
    ptr++;
   }
   return ~crc;
}


void sendPulse()
{
   uint8_t buf[18]  ={0};
   uint16_t i  = 0;
   
   buf[0]  = 0x24;
   buf[1]  = 0x5a;
   
   i  = msg_crc(buf, 16);
   
   buf[16]  = i >> 8;
   buf[17]  = i & 0xff;
   
   UART_Send(UART_2, buf, 18, BLOCKING);
}




