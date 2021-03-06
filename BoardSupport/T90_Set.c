
#include "Config.h"
#include "Setting.h"
#include "string.h"
#include "lpc177x_8x_eeprom.h"
#include "pwm.h"
#include "sound.h"
#include "Check.h"
#include "T90.h"


unsigned long SYS_Date;
unsigned long SYS_Time;
                                     

void printT90_Set(T90_Set * p)
{
   printf("\r\n");
	
	 printf("Distance1       %d\r\n", p->dst.dst1);
	 printf("Distance2       %d\r\n", p->dst.dst2);
	 printf("Distance3       %d\r\n", p->dst.dst3);

	 printf("Invd_dst        %d\r\n", p->alarm.invd_dst);
	 printf("Danger_sog      %d\r\n", p->alarm.danger_sog);
	 printf("Min_sog         %d\r\n", p->alarm.min_sog);
	 printf("Max_sog         %d\r\n", p->alarm.max_sog);
	 printf("Nation          %X\r\n", p->alarm.nation);
	
	 printf("Workmode        %s-%d\r\n", p->sys.workmode?"double":"single", p->sys.workmode);
	 printf("Launch          %s-%d\r\n", p->sys.launch?"on":"off", p->sys.launch);
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
   
   
   printf("\r\nLoading...\r\n"); 
   
   EEPROM_Read(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &t90_set, MODE_8_BIT, sizeof(t90_set));
   /// Check if system config is right or not.
   printT90_Set(&t90_set);
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
   printT90_Set(&t90_set);               
}


void T90_Init()
{        
   if(T90_Load())
   {
INFO("System load successfully!");   
   }
   else
   {
INFO("Error happened when system load.System will be configed with default value");   
   }

   PWM_SET(t90_set.sys.bright); 
   SND_Init();
}


void T90_Reset(void)
{

  t90_set.dst.dst1 = DEFAULT_DST1;
	t90_set.dst.dst2 = DEFAULT_DST2;
	t90_set.dst.dst3 = DEFAULT_DST3;
	
	t90_set.alarm.invd_dst   = DEFAULT_INVD_DST;
	t90_set.alarm.danger_sog = DEFAULT_DANGER_SOG_X10;
	t90_set.alarm.min_sog    = DEFAULT_MIN_SOG_X10;
	t90_set.alarm.max_sog    = DEFAULT_MAX_SOG_X10;
	t90_set.alarm.nation     = DEFAULT_NATION;
	
	t90_set.sys.workmode  = SINGLE_MODE;
	t90_set.sys.launch    = OFF;
  t90_set.sys.nightmode = DAY;
  t90_set.sys.bright    = DEFAULT_BRIGHT;
  t90_set.sys.volum     = DEFAULT_VOLUM;
  t90_set.sys.unit      = NM;

  T90_Store();

  printT90_Set(&t90_set);
}


