
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


void CHECK_MS_Speed()
{
	MS_isSpeeding = 0;   //clear
	MS_isMax_SOG = 0;
	MS_isMin_SOG = 0;
	
	if(mothership.SOG > t90_set.alarm.danger_sog)
	{
		MS_isSpeeding = 1;
	}
	if(mothership.SOG > t90_set.alarm.max_sog)
	{
		MS_isMax_SOG = 1;
	}
	else if(mothership.SOG < t90_set.alarm.min_sog)
	{
		MS_isMin_SOG = 1;
	}
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
	}
	
	detect();	
	CHECK_DelHighSpeed();
//	SNAP_Refresh();
	CHECK_HasAlarm();
	CHECK_MS_Speed();
INFO("highspeed num:%d",validCnt);
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









