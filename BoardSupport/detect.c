#include "stub.h"
#include "boat_struct.h"
#include "T90.h"
#include "math.h"
#include "detect.h"
#include "transform.h"

#define SQUARE(X) ((X)*(X))
/**
 *
 *
 */
static unsigned int pointInPolygon = 0;
static unsigned int stubNumMax;





void detectInit()
{
   
    int i,j;

	 if(t90_set.sys.workmode == SINGLE_MODE)
	 {
		  stubNumMax = STUB_NUM_MAX_S;
	 }
	 else
	 {
		  stubNumMax = STUB_NUM_MAX_D;
	 }
    for(i = 0; i < stubNumMax;i++)
    {
			
       if(stubs[i].isValid)
		 {
			  j = i+1;
			  do{
				 if(stubs[j].isValid == 1)
				 {
					 break;
				 }
				 else
				 {
					 j++;
				 }
			  }while(j == stubNumMax);
			  if(j==stubNumMax)
					 j = 0;
			  if(stubs[i].tang1.point.y*stubs[j].tang2.point.x-stubs[j].tang2.point.y*stubs[i].tang1.point.x >= 0)
			  {
					 pointInPolygon |= 0x00000001<<i;
			  }
			  else
			  {
					 pointInPolygon &= 0xfffffffe<<i;
			  }
			  
			 
			  if(-(stubs[i].tang1.point.x-stubs[j].tang2.point.x)+stubs[i].tang1.point.y*stubs[i].tang2.point.x-stubs[i].tang2.point.y*stubs[i].tang1.point.x > 0)
			  {
					pointInPolygon |= 0x00000001<<(i+16);
			  }
			  else
			  {
					pointInPolygon &= 0xfffffffe<<(i+16);
			  }
        }
    }	 
    
 }

Bool isCloseStub(BERTH *pBerth)
{
    int i = 0;    
    for(i = 0;i < stubNumMax;i++)
    { 
	    if(stubs[i].isValid)	 
			  if(SQUARE(pBerth->x_to_cross - stubs[i].basePoint.x) + SQUARE(pBerth->y_to_cross - stubs[i].basePoint.y) < SQUARE(t90_set .alarm.invd_dst )){
					return TRUE;		 
			  }
    }
    return FALSE;
}

Bool isInPolygon(BERTH *pBerth)
{
    int i = 0;
    int j;    
    unsigned int isinpoly = 0;
	
    for(i = 0;i < stubNumMax;i++)
    {       
//        j = (i+1 == stubNumMax)? 0: i+1;
      if(stubs[i].isValid)
      {
         j = i+1;
         do{
          if(stubs[j].isValid == 1)
          {
             break;
          }
          else
          {
             j++;
          }
         }while(j == stubNumMax);
         if(j==stubNumMax)
             j = 0;
         if(((stubs[i].tang1.point.x-stubs[j].tang2.point.x)*pBerth->y_to_cross + (stubs[j].tang2.point.y-stubs[i].tang1.point.y)*pBerth->x_to_cross
            +stubs[i].tang1.point.y*stubs[j].tang2.point.x-stubs[j].tang2.point.y*stubs[i].tang1.point.x) > 0)
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
//	else if(pBerth->isInvader == 0)
//	{
//		pBerth->mntState = MNTState_None;
//	}
}


void detect()
{
    int i = 0;
     detectInit ();
	  for(i = 0; i < N_boat; i++)          //clear
	  {
		    SimpBerthes[i].pBerth->isInvader = 0;
	  }
	  
   if(stubs[1].isValid || stubs[2].isValid || stubs[3].isValid)
     for(i = 0;i < N_boat;i++)
     {
         if(SimpBerthes[i].Dist < (t90_set.alarm.invd_dst+500)*7/4  &&  (SimpBerthes[i].pBerth->Boat.category&TYPE_SAFETY) == 0){
             llToxy(SimpBerthes[i].pBerth);
             isInvader(SimpBerthes[i].pBerth);
         }
         
     }

}



