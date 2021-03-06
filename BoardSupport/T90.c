#include "GUI.h"
#include "sound.h"
#include "Config.h"
//#include "Setting.h"
#include "string.h"
#include "lpc177x_8x_eeprom.h"
#include "pwm.h"
#include "Check.h"
#include "T90.h"
#include "stub.h"


//所有关于距离的变量，单位都是(1/1000nm)
//  1 nm = 1.85 km = 37/20 km

int sound  = ON;
int AISOnLine = OFF;
int Silence = OFF;

unsigned long SYS_Date;
unsigned long SYS_Time;


extern PortStatus portStatus[3] ;

T90_Set t90_set = {

   {
      DEFAULT_DST1,
      DEFAULT_DST2,
      DEFAULT_DST3,
      DEFAULT_DST4,
      DEFAULT_DST5,},
   {
      DEFAULT_DOUDST1,
      DEFAULT_DOUDST4,
      DEFAULT_DOUDST1,     
      DEFAULT_DOUDST3,
      DEFAULT_DOUDST2,
      
   },
	{
		DEFAULT_INVD_DST, 	
		DEFAULT_DANGER_SOG_X10,   
		DEFAULT_MIN_SOG_X10,     
		DEFAULT_MAX_SOG_X10,      
		DEFAULT_NATION,
  DEFAULT_HSPEED_X10,  
  DEFAULT_ALARMOFF },	
	
	{
		NONE_MODE,
		NIGHT,
		DEFAULT_VOLUM,
		DEFAULT_BRIGHT,
		NM,
  0,
  0,
  DEFAULT_LEFT,
  DEFAULT_SOG_TIME,
  DEFAULT_COG_TIME,   

  },
   {
      0,
      0,
   },
   {
      {0},
      0,
   },

};
T90_Set reset_t90_set = {

   {
      DEFAULT_DST1,
      DEFAULT_DST2,
      DEFAULT_DST3,
      DEFAULT_DST4,
      DEFAULT_DST5,},
   {
      DEFAULT_DOUDST1,
      DEFAULT_DOUDST4,
      DEFAULT_DOUDST1,     
      DEFAULT_DOUDST3,
      DEFAULT_DOUDST2,
      
   },
	{
		DEFAULT_INVD_DST, 	
		DEFAULT_DANGER_SOG_X10,   
		DEFAULT_MIN_SOG_X10,     
		DEFAULT_MAX_SOG_X10,      
		DEFAULT_NATION,
  DEFAULT_HSPEED_X10,  
  DEFAULT_ALARMOFF },	
	
	{
		NONE_MODE,
		NIGHT,
		DEFAULT_VOLUM,
		DEFAULT_BRIGHT,
		NM,
  0,
  0,
  DEFAULT_LEFT,
  DEFAULT_SOG_TIME,
  DEFAULT_COG_TIME,   

  },
   {
      0,
      0,
   },
   {
      {0},
      0,
   },

};

int MS_isSpeeding = 0;      //MS:mothership 
int MS_isMin_SOG = 0;
int MS_isMax_SOG = 0;
char MS_EWNS = 0x11;         //母船的经纬度方位

int screen_hcenter;
int screen_vcenter;



void printT90_Set(T90_Set * p)
{
   printf("\r\n");
	


   printf("Invd_dst        %d\r\n", p->alarm.invd_dst);
   printf("Danger_sog      %d\r\n", p->alarm.danger_sog);
   printf("Min_sog         %d\r\n", p->alarm.min_sog);
   printf("Max_sog         %d\r\n", p->alarm.max_sog);
   printf("Nation          %X\r\n", p->alarm.nation);
   
   printf("Workmode        %s-%d\r\n", p->sys.workmode?"double":"single", p->sys.workmode);
   printf("Nightmode       %s-%d\r\n", p->sys.nightmode?"Night":"Day",p->sys.nightmode);
   printf("Snd -- volum    %d\r\n",    p->sys.volum);
   printf("Brightness      %d\r\n",    p->sys.bright);
   printf("Unit            %s-%d\r\n", p->sys.unit?"nm":"km",p->sys.unit);
	
	printf("\r\n");
}


static Bool checkT90_Set()
{
   Bool flag  = TRUE;

   if(t90_set.sys.nightmode != DAY  &&  t90_set.sys.nightmode != NIGHT) 
   {
      flag  = FALSE;
      printf("nightmode load error! load %d as nightmode\r\n",t90_set.sys.nightmode);
      t90_set.sys.nightmode  = DAY;
   }
   if(t90_set.sys.bright < 1  ||  t90_set.sys.bright > 6)                     
   {
      flag  = FALSE;   
      printf("bright  load error! load %d as bright\r\n",t90_set.sys.bright);
      t90_set.sys.bright  = DEFAULT_BRIGHT;
   }
   if( t90_set.sys.volum > 6)            
   {
      flag  = FALSE;   
      printf("volum  load error! load %d as volum\r\n",t90_set.sys.volum);
      t90_set.sys.volum  = DEFAULT_VOLUM;
   }

   if(t90_set.sys.unit != KM  &&  t90_set.sys.unit != NM)     
   {
      flag  = FALSE;   
      printf("unit load error! load %d as unit\r\n",t90_set.sys.unit);
      t90_set.sys.unit  = NM;
   }
   return flag;
}



Bool T90_Load()
{  
 
   Bool flag  = TRUE;
   
   
//   printf("\r\nLoading...\r\n"); 
   
   EEPROM_Read(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &t90_set, MODE_8_BIT, sizeof(t90_set));
   /// Check if system config is right or not.
//   printT90_Set(&t90_set);
   if(!checkT90_Set())
   {
//      T90_Store();
		    T90_Reset();
//printf("after fix:\r\n");   
//      printT90_Set(&t90_set);
      flag  = FALSE;
   }
   return flag;
}


void T90_Store()
{
   EEPROM_Write(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &t90_set, MODE_8_BIT, sizeof(t90_set));        
//   printT90_Set(&t90_set);               
}


void T90_Init()
{        
   if(T90_Load())
   {
//INFO("System load successfully!");   
   }
   else
   {
INFO("Error happened when system load.System will be configed with default value");   
   }

   PWM_SET(t90_set.sys.bright); 
//   SND_Init();
//	 StubRefresh();
}


void T90_Reset(void)
{


	
//   t90_set.alarm.invd_dst   = DEFAULT_INVD_DST;
//   t90_set.alarm.danger_sog = DEFAULT_DANGER_SOG_X10;
//   t90_set.alarm.min_sog    = DEFAULT_MIN_SOG_X10;
//   t90_set.alarm.max_sog    = DEFAULT_MAX_SOG_X10;
//   t90_set.alarm.nation     = DEFAULT_NATION;
//   
//   t90_set.sys.workmode  = NONE_MODE;
//   t90_set.sys.launch    = OFF;
//   t90_set.sys.nightmode = NIGHT;
//   t90_set.sys.bright    = DEFAULT_BRIGHT;
//   t90_set.sys.volum     = DEFAULT_VOLUM;
//   t90_set.sys.unit      = NM;
//   t90_set.sys.update    = OFF;
   memcpy(&t90_set,&reset_t90_set,sizeof(t90_set));
   T90_Store();


//  printT90_Set(&t90_set);
}

void T90_setAlarmON_OFF(int value,int which)
{
   if(value)
   {
      t90_set.alarm.on_off |= 0x01<<which;
   }
   else
   {
      t90_set.alarm.on_off &= ~(0x01<<which);
   }
}

