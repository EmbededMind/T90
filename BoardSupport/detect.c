#include "stub.h"
#include "boat_struct.h"
#include "T90.h"
#include "math.h"
#include "detect.h"

#define SQUARE(X) ((X)*(X))
/**
 *
 *
 */
static unsigned int pointInPolygon = 0;

void detectInit()
{
    int i,j;
    for(i = 0; i < STUB_NUM;i++)
    {
				j = (i+1 == STUB_NUM)? 0: i+1;
        if(stubs[i].tang1.point.y*stubs[j].tang2.point.x-stubs[j].tang2.point.y*stubs[i].tang1.point.x >= 0)
        {
             pointInPolygon |= 0x00000001<<i;
        }
        else
        {
             pointInPolygon &= 0xfffffffe<<i;
        }
        
       
        if(stubs[i].tang1.point.y*stubs[i].tang2.point.x-stubs[i].tang2.point.y*stubs[i].tang1.point.x > 0)
        {
            pointInPolygon |= 0x00000001<<(i+16);
        }
        else
        {
            pointInPolygon &= 0xfffffffe<<(i+16);
        }
    }            
    
 }

Bool isCloseStub(BERTH *pBerth)
{
    int i = 0;    
    for(i = 0;i < STUB_NUM;i++)
    {
        if(SQUARE(pBerth->x_to_cross - stubs[i].basePoint.x) + SQUARE(pBerth->y_to_cross - stubs[i].basePoint.y) < SQUARE(t90_set .alarm.invd_dst )){
            return TRUE;
    
        }
    }
    return FALSE;
}

Bool isInPolygon(BERTH *pBerth)
{
    int i = 0;
    unsigned int isinpoly = 0;
    for(i = 0;i < STUB_NUM;i++)
    {
        if(((stubs[i].tang1.point.x-stubs[((i+1) == STUB_NUM)?0:(i+1)].tang2.point.x)*pBerth->y_to_cross + (stubs[((i+1) == STUB_NUM)?0:(i+1)].tang2.point.y-stubs[i].tang1.point.y)*pBerth->x_to_cross
            +stubs[i].tang1.point.y*stubs[((i+1) == STUB_NUM)?0:(i+1)].tang2.point.x-stubs[((i+1) == STUB_NUM)?0:(i+1)].tang2.point.y*stubs[i].tang1.point.x) > 0)
        {
            isinpoly |= 0x00000001<<i;
        }
        else
        {
            isinpoly &= 0xfffffffe<<i;
        }
        if(((stubs[i].tang1.point.x-stubs[i].tang2.point.x)*pBerth->y_to_cross + (stubs[i].tang2.point.y-stubs[i].tang1.point.y)*pBerth->x_to_cross
            +stubs[i].tang1.point.y*stubs[i].tang2.point.x-stubs[i].tang2.point.y*stubs[i].tang1.point.x) > 0)
        {
            isinpoly |= 0x00000001<<(i+16);
        }
        else
        {
            isinpoly &= 0xfffffffe<<(i+16);
        }
    }
    if(isinpoly == pointInPolygon)
    {    
        return TRUE;
    }
    return FALSE;
}



void isInvader(BERTH  *pBerth)
{
	pBerth->isInvader = isCloseStub(pBerth) || isInPolygon(pBerth);
	if(pBerth->isInvader && pBerth->mntState == MNTState_None)
	{
		pBerth->mntState = MNTState_Triggered;
	}
	else if(pBerth->isInvader == 0)
	{
		pBerth->mntState = MNTState_None;
	}
}


void detect()
{
    int i = 0;
  
    for(i = 0;i < N_boat;i++)
    {
        if(SimpBerthes[i].Dist < (t90_set.alarm.invd_dst+500)*5/4){
            llToxy(SimpBerthes[i].pBerth);
            isInvader(SimpBerthes[i].pBerth);
        }
        
    }

}



