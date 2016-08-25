
#include "Check.h"
#include <math.h>
#include "sound.h"
#include "invader.h"
#include <ucos_ii.h>
#include <string.h>
#include "bully.h"
#include "T90.h"
#include "detect.h"
#include "bully.h"
#include "snap.h"
#include "transform.h"

#define STIME 180

/*----------------- Macro      defines --------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))

/*----------------- external variables ------------------------*/
extern BERTH Berthes[BOAT_NUM_MAX];
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
extern int N_boat;
extern int Triggered_SOG;
extern FunctionalState isMntEnable;

/// Defined in app.c.If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;

static int STime = STIME;
static int hasAlarm = 0;

static void CHECK_HasAlarm(void);
static void CHECK_MaskAllBerth(void);

int Triggered_SOG = 0;

void CHECK_MS_Speed()
{
//	MS_isSpeeding = 0;   //clear
//	MS_isMax_SOG = 0;
//	MS_isMin_SOG = 0;
//printf("CHECK_MS_Speed begin\n");	
	  if( (t90_set.alarm.on_off & (0x01<<1)) && mothership.SOG > t90_set.alarm.danger_sog)
	  {		
		    if(MS_isSpeeding != MNTState_Masked)
		    {
			      MS_isSpeeding = MNTState_Triggered;
         Triggered_SOG = mothership.SOG;
		    }
	  }
	  else
	  {
		    MS_isSpeeding = 0;
	  }
   if(!(t90_set.alarm.on_off & (0x01<<2)))
      goto sog;
	  if( mothership.SOG > t90_set.alarm.max_sog)
	  {
		    if(MS_isMax_SOG != MNTState_Masked)
						{
									MS_isMax_SOG = MNTState_Triggered;
         Triggered_SOG = mothership.SOG;
						}
	  }
	  else if(mothership.SOG < t90_set.alarm.min_sog)
	  {
		    if(MS_isMin_SOG != MNTState_Masked)
		    {
			      MS_isMin_SOG = MNTState_Triggered;
         Triggered_SOG = mothership.SOG;
		    }
	  }
  	else
	  {
      sog:		MS_isMax_SOG = MS_isMin_SOG = 0;
	  }
//printf("CHECK_MS_Speed bend\n");
}

//static void CHECK_MS_Speed_masked()
//{
//printf("CHECK_MS_Speed_masked begin\n");
//	if(MS_isSpeeding ==MNTState_Triggered)
//		MS_isSpeeding = MNTState_Masked;
//	if(MS_isMax_SOG ==MNTState_Triggered)
//		MS_isMax_SOG = MNTState_Masked;
//	if(MS_isMin_SOG ==MNTState_Triggered)
//		MS_isMin_SOG = MNTState_Masked;
//printf("CHECK_MS_Speed_masked end\n");
//}

void CHECK_DelHighSpeed()
{
   BULY_BERTH *pBerth;
   BULY_BERTH *pNext;
//printf("CHECK_DelHighSpeed begin\n");
   pBerth = pBulyHeader;
   if(!(t90_set.alarm.on_off & (0x01<<3)))
   {
      while(pBerth)
      {
         if((pBerth->pBoatLink->Boat.category & 0xf0) == 0)
         {
              pNext = pBerth->pNext;
              pBerth->pBoatLink->Boat.category = 0;
              BULY_delete(pBerth->pBoatLink);
         }
         pBerth = pNext;
      }       
      return;
   }
    
   while(pBerth)
   {
      pNext = pBerth->pNext;
      if((pBerth->pBoatLink->Boat.category & 0xf0) == 0 && pBerth->pBoatLink->Boat.highspeedflag < 3)
      {
         pBerth->pBoatLink->Boat.category = 0;
         BULY_delete(pBerth->pBoatLink);
         goto delhighjmp;// pBerth已经是一个空指针不能访问了
      }
      if((pBerth->pBoatLink->Boat.category & TYPE_FAMILY) == TYPE_FAMILY)
      {
         pBerth->pBoatLink->Boat.category = 0;
         BULY_delete(pBerth->pBoatLink);
      }
delhighjmp:      pBerth = pNext;
   }    
//printf("CHECK_DelHighSpeed end\n");  
}

void CHECK_STRefresh()
{
//printf("CHECK_STRefresh begin\n");
    if(mothership.SOG < 10)
    {
	    if(STime)   STime--;     
    }
    else
    {
       STime = STIME;
    }
//printf("CHECK_STRefresh end\n");
}

int FetchSTime()
{
   return STime;
}
 

void check()
{
//printf("check begin\n");
	if(isKeyTrigged)
	{
		isKeyTrigged = 0;
		CHECK_MaskAllBerth();
//		CHECK_MS_Speed_masked();
	}
	
	detect();
	CHECK_DelHighSpeed();
	CHECK_HasAlarm();
	CHECK_MS_Speed();
  CHECK_STRefresh();
//printf("check end\n");
}

int CHECK_GetAlarmState(void)
{
	return hasAlarm;
}

static void CHECK_MaskAllBerth(void)
{
	int i;
//printf("CHECK_MaskAllBerth begin\n");
	for(i = 0; i < N_boat; i++)
	{
		if(SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
		{
			SimpBerthes[i].pBerth->mntState = MNTState_Masked;
		}
	}
//printf("CHECK_MaskAllBerth end\n");
}

static void CHECK_HasAlarm(void)
{
	int i;
//printf("CHECK_HasAlarm begin\n");
	for(i = 0; i < N_boat; i++)
	{
		if(SimpBerthes[i].pBerth->isInvader)
		{
			hasAlarm = 1;
			return;
		}
	}
	if(BULY_getValidNumber())
	{
		hasAlarm = 1;
		return;
	}
	hasAlarm = 0;
//printf("CHECK_HasAlarm end\n");
}


BERTH* SIMP_BERTH_fetchNextPlayBerth()    //invader alarm
{
    int i = 0;
    int flag = 0;
    static BERTH *pBerth = 0;
    static long int user_id;
    
    if(pBerth)
    {
        for(i = 0; i < N_boat; i++)
        {            
            if(flag)
            {
                 if(SimpBerthes[i].pBerth->isInvader && SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
                 {
//                     SNAP_SetSnapLink(SimpBerthes[i].pBerth);
                     pBerth =  SimpBerthes[i].pBerth;
                     user_id = SimpBerthes[i].pBerth->Boat.user_id;
                     return SimpBerthes[i].pBerth;
                 } 
            }
            if(!flag && pBerth == SimpBerthes[i].pBerth && user_id == SimpBerthes[i].pBerth->Boat.user_id) 
                flag = 1;
        }
        for(i = 0; i < N_boat; i++)
        {            
            if(SimpBerthes[i].pBerth->isInvader && SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
            {
//                SNAP_SetSnapLink(SimpBerthes[i].pBerth);
                pBerth =  SimpBerthes[i].pBerth;
                user_id = SimpBerthes[i].pBerth->Boat.user_id;
                return SimpBerthes[i].pBerth;
            }
        }
    }
    else
    {
        for(i = 0; i < N_boat; i++)
        {            
            if(SimpBerthes[i].pBerth->isInvader && SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
            {
//                SNAP_SetSnapLink(SimpBerthes[i].pBerth);
                pBerth =  SimpBerthes[i].pBerth;
                user_id = SimpBerthes[i].pBerth->Boat.user_id;
                return SimpBerthes[i].pBerth;
            }       
        }
    }
    return NULL;     
}


                                           






