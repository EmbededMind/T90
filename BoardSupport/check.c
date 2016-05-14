
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

/*----------------- Macro      defines --------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))

/*----------------- external variables ------------------------*/
extern BERTH Berthes[BOAT_NUM_MAX];
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
extern int N_boat;

extern FunctionalState isMntEnable;

/// Defined in app.c.If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;


static int hasAlarm = 0;

static void CHECK_HasAlarm(void);
static void CHECK_MaskAllBerth(void);


void CHECK_isInvader(void)
{
	int i;
	int outDst1, outDst2, outDst3;  
	outDst1 = t90_set.dst.dst1*20/37+t90_set.alarm.invd_dst/sin(atan2(t90_set.dst.dst2,t90_set.dst.dst1)); 
	outDst2 = t90_set.dst.dst2*20/37+t90_set.alarm.invd_dst/sin(atan2(t90_set.dst.dst1,t90_set.dst.dst2));
	outDst3 = (t90_set.dst.dst3- t90_set.dst.dst1)*20/37+t90_set.alarm.invd_dst/sin(atan2(t90_set.dst.dst2,(t90_set.dst.dst3-t90_set.dst.dst1))); 

	for(i = 0; i < N_boat; i++)          //clear
	{
		SimpBerthes[i].pBerth->isInvader = 0;
	}
	
	i = -1;
	while((SimpBerthes[++i].Dist < 1.2*(t90_set.alarm.invd_dst + t90_set.dst.dst3*20/37)) && i < N_boat)
	{	
		SimpBerthes[i].pBerth->x_to_cross = (int)(cos(mothership.COG*3.14/1800)*(SimpBerthes[i].longitude - mothership.longitude)
																				 -sin(mothership.COG*3.14/1800)*(SimpBerthes[i].latitude  - mothership.latitude));
		SimpBerthes[i].pBerth->y_to_cross = (int)(sin(mothership.COG*3.14/1800)*(SimpBerthes[i].longitude - mothership.longitude)
																				 +cos(mothership.COG*3.14/1800)*(SimpBerthes[i].latitude  - mothership.latitude) + t90_set.dst.dst1*20/37);
    //判断是否在外围菱形内
    if(((SimpBerthes[i].pBerth->x_to_cross*outDst1/outDst2+outDst1)>SimpBerthes[i].pBerth->y_to_cross)
    && ((-SimpBerthes[i].pBerth->x_to_cross*outDst1/outDst2+outDst1)>SimpBerthes[i].pBerth->y_to_cross)
    && ((SimpBerthes[i].pBerth->x_to_cross*outDst3/outDst2-outDst3)<SimpBerthes[i].pBerth->y_to_cross)
    && ((-SimpBerthes[i].pBerth->x_to_cross*outDst3/outDst2-outDst3)<SimpBerthes[i].pBerth->y_to_cross)   )
    {
			//判断y值是否在上部圆弧切点之上  (定义域)
			if(SimpBerthes[i].pBerth->y_to_cross > (t90_set.dst.dst1*20/37+t90_set.alarm.invd_dst*sin(atan2(t90_set.dst.dst2,t90_set.dst.dst1))))
			{			
				//判断点是否在圆弧之上         (值域)
				if((SimpBerthes[i].pBerth->x_to_cross*SimpBerthes[i].pBerth->x_to_cross + (SimpBerthes[i].pBerth->y_to_cross - t90_set.dst.dst1*20/37)*(SimpBerthes[i].pBerth->y_to_cross - t90_set.dst.dst1*20/37)) > t90_set.alarm.invd_dst*t90_set.alarm.invd_dst)
				{
					SimpBerthes[i].pBerth->isInvader = 0;
					SimpBerthes[i].pBerth->mntState = MNTState_None;
					continue;
				}
			}
			//y值是否在下部圆弧切点之下      (定义域)
			if( SimpBerthes[i].pBerth->y_to_cross < -(t90_set.dst.dst3*20/37-t90_set.dst.dst1*20/37+t90_set.alarm.invd_dst*sin(atan2(t90_set.dst.dst2,t90_set.dst.dst3-t90_set.dst.dst1))) )
			{
				//点是否在圆弧之下             (值域)
				if((SimpBerthes[i].pBerth->x_to_cross*SimpBerthes[i].pBerth->x_to_cross + (SimpBerthes[i].pBerth->y_to_cross - t90_set.dst.dst3*20/37+t90_set.dst.dst1*20/37)*(SimpBerthes[i].pBerth->y_to_cross - t90_set.dst.dst3*20/37+t90_set.dst.dst1*20/37)) > t90_set.alarm.invd_dst*t90_set.alarm.invd_dst)
				{
					SimpBerthes[i].pBerth->isInvader = 0;
					SimpBerthes[i].pBerth->mntState = MNTState_None;
					continue;
				}			 
			}
			//y值是否在左右两圆弧上下切点之间 (定义域)
			if((SimpBerthes[i].pBerth->y_to_cross>-t90_set.alarm.invd_dst*20/37*sin(atan2(t90_set.dst.dst2,t90_set.dst.dst1))
			&&SimpBerthes[i].pBerth->y_to_cross<t90_set.alarm.invd_dst*20/37*sin(atan2(t90_set.dst.dst2,t90_set.dst.dst3-t90_set.dst.dst1))))
			{
				//点是否在两边圆弧之外          (值域)
				if((((int)sqrt(fabs(t90_set.alarm.invd_dst*t90_set.alarm.invd_dst-SimpBerthes[i].pBerth->y_to_cross*SimpBerthes[i].pBerth->y_to_cross))+t90_set.dst.dst2) < SimpBerthes[i].pBerth->x_to_cross)
				||(((int)-sqrt(fabs(t90_set.alarm.invd_dst*t90_set.alarm.invd_dst-SimpBerthes[i].pBerth->y_to_cross*SimpBerthes[i].pBerth->y_to_cross))-t90_set.dst.dst2) > SimpBerthes[i].pBerth->x_to_cross)         )
				{
					SimpBerthes[i].pBerth->isInvader = 0;
					SimpBerthes[i].pBerth->mntState = MNTState_None;
					continue;
				}							 
			}
			/********************************************************************/
			if(SimpBerthes[i].Dist < 500)
			{
				SimpBerthes[i].pBerth->isInvader = 0;
				SimpBerthes[i].pBerth->mntState = MNTState_None;
				continue;
			}
			/*******************************************************************/
      SimpBerthes[i].pBerth->isInvader = 1;
			if(SimpBerthes[i].pBerth->mntState == MNTState_None)
			{
				SimpBerthes[i].pBerth->mntState = MNTState_Triggered;
			}
		}
		else
		{
			SimpBerthes[i].pBerth->isInvader = 0;		
			SimpBerthes[i].pBerth->mntState = MNTState_None;
		}   
  }
}



void CHECK_MS_Speed()
{
	MS_isSpeeding = 0;   //clear
	MS_isMax_SOG = 0;
	MS_isMin_SOG = 0;
	
	if(mothership.SOG > t90_set.alarm.danger_sog)
	{		
		if(MS_isSpeeding != MNTState_Masked)
		{
			MS_isSpeeding = MNTState_Triggered;
		}
	}
	else
	{
		MS_isSpeeding = 0;
	}
	if(mothership.SOG > t90_set.alarm.max_sog)
	{
		if(MS_isMax_SOG != MNTState_Masked)
		{
			MS_isMax_SOG = MNTState_Triggered;
		}
	}
	else if(mothership.SOG < t90_set.alarm.min_sog)
	{
		if(MS_isMin_SOG != MNTState_Masked)
		{
			MS_isMin_SOG = MNTState_Triggered;
		}
	}
	else
	{
		MS_isMax_SOG = MS_isMin_SOG = 0;
	}
}

static void CHECK_MS_Speed_masked()
{
	if(MS_isSpeeding ==MNTState_Triggered)
		MS_isSpeeding = MNTState_Masked;
	if(MS_isMax_SOG ==MNTState_Triggered)
		MS_isMax_SOG = MNTState_Masked;
	if(MS_isMin_SOG ==MNTState_Triggered)
		MS_isMin_SOG = MNTState_Masked;
}

void CHECK_DelHighSpeed()
{
    BULY_BERTH *pBerth;
    pBerth = pBulyHeader;
    
    while(pBerth)
    {
        if((pBerth->pBoatLink->Boat.category & 0xf0) == 0 && pBerth->pBoatLink->Boat.SOG < HIGH_SPEED)
        {
            BULY_delete(pBerth->pBoatLink);
        }
        pBerth = pBerth->pNext;
    }    
    
}

void check()
{
	if(isKeyTrigged)
	{
		isKeyTrigged = 0;
		CHECK_MaskAllBerth();
		CHECK_MS_Speed_masked();
	}
	
	detect();	
	CHECK_DelHighSpeed();
//	SNAP_Refresh();
	CHECK_HasAlarm();
	CHECK_MS_Speed();
//INFO("highspeed num:%d",validCnt);
	BULY_dump();
}

int CHECK_GetAlarmState(void)
{
	return hasAlarm;
}

static void CHECK_MaskAllBerth(void)
{
	int i;
	for(i = 0; i < N_boat; i++)
	{
		if(SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
		{
			SimpBerthes[i].pBerth->mntState = MNTState_Masked;
		}
	}
}

static void CHECK_HasAlarm(void)
{
	int i;
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
}


BERTH* SIMP_BERTH_fetchNextPlayBerth()    //invader alarm
{
	int i;
	for(i = 0; i < N_boat; i++)
	{
		if(SimpBerthes[i].pBerth->isInvader && SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
		{
			return SimpBerthes[i].pBerth;
		}
	}
	return NULL;
}









