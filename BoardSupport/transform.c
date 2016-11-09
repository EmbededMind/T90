#include "stub.h"
#include "boat_struct.h"
#include "T90.h"
#include "math.h"
#include "transform.h"
#include "sound.h"

#define  MYABS(X)    ((X)<0 ? -(X) : (X))  

#define   GETANGLE(Y,X)   (int)(atan2(MYABS(Y),MYABS(X))*180/3.1416)
    

int rectifyNum(int num, int stepValue)   //根据步进值对整形数纠正误差
{
	if(stepValue > 1)
	{
		return (num+stepValue-1 - (num+stepValue-1)%stepValue);
	}
	return num;
}

void llToxy(BERTH *pBerth)
{
   
      
   pBerth->x_to_cross = (int)(cos(mothership.COG*3.14/1800)*(pBerth->Boat.longitude - mothership.longitude)
                                                             -sin(mothership.COG*3.14/1800)*(pBerth->Boat.latitude  - mothership.latitude));
   pBerth->y_to_cross = (int)(sin(mothership.COG*3.14/1800)*(pBerth->Boat.longitude - mothership.longitude)
                                                             +cos(mothership.COG*3.14/1800)*(pBerth->Boat.latitude  - mothership.latitude));
   
   if(!(t90_set.sys.workmode == SINGLE_MODE || t90_set.sys.motherpos == DEFAULT_LEFT))
   {
      pBerth->x_to_cross += stubs[4].basePoint.x;      
   }      
}


int getAngleOfShip(BERTH *pBerth)
{
   if(pBerth->x_to_cross > 0)
   {
      if(pBerth->y_to_cross >= 0)
      {            
         return 90 - GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross);
      }
      else
      {           
         return GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross) + 90;
      }
   }
   else if(pBerth->x_to_cross < 0)
   {
      if(pBerth->y_to_cross <= 0)
      {
         return 270 - GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross);
      }
      else
      {
         return 270 + GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross);
      }
   }
   else
   {
      if(pBerth->y_to_cross >= 0)
      {
         return 0;
      }
      else 
      {
         return 180;
      }
   }
}


int SND_ParseNum(int num, uint8_t *pNum)
{
   uint8_t hand;
   uint8_t ten;
   uint8_t one;
   uint8_t oneth;
   uint8_t tenth;
   memset(pNum, 0 , sizeof(uint8_t)*8); 
   hand = num%1000000/100000;
   ten = num%100000/10000;
   one = num%10000/1000;
   oneth = num%1000/100;
   tenth = num%100/10;
 
   pNum[7] = tenth;
   pNum[6] = oneth;
   pNum[4] = one;
   pNum[2] = ten;
   pNum[0] = hand;
   //百要不要读
   if(hand)
   {
      pNum[1] = SND_ID_HAND;
   }
   //十读 十/零
   if(ten)
   {
      pNum[3] = SND_ID_TEN;
      if(ten == 1 && !hand)
      {
         pNum[2] = 0;
      }
   }
   if(hand && !ten && one)
   {
      pNum[3] = SND_ID_ZRO;
   }
   //个位读什么 几/零
   if(!hand && !ten && !one)
   {
      pNum[4] = SND_ID_ZRO;
   }
   //点要不要读
   if(oneth || tenth)
   {
      pNum[5] = SND_ID_DOT;
   }
   else
   {
      return 0;
   }
   //十分位读 几/零
   if(!oneth && tenth)
   {
      pNum[6] = SND_ID_ZRO;
   }

//    memset(pNum, 0 , sizeof(uint8_t)*5);
//    pNum[0] = num/100000;
//    if(pNum[0])//百位不为零
//    {
//        pNum[1] = SND_ID_HAND;
//        pNum[2] = num%100000/10000;
//        if(pNum[2])//十位不为零
//        {
//            pNum[3] = SND_ID_TEN;
//            pNum[4] = num%10000/1000;
//        }
//        else//十位为零
//        {
//            pNum[2] = num%10000/1000;
//            if(pNum[2])//个位不为零
//            {
//                pNum[2] = SND_ID_ZRO;//点几  不够了  所以pNum[4]=0
//                pNum[3] =num%10000/1000;
//                pNum[4] = 0;
//                return NULL;
//            }
//            else
//            {
//                
//                pNum[2] = num%1000/100;
//                if(pNum[2])
//                {
//                    pNum[2] = SND_ID_DOT;
//                    pNum[3] = num%1000/100;
//                    pNum[4] = num%100/10;
//                }
//                else
//                {
//                    pNum[4] = num%100/10;
//                    if(pNum[4])
//                    {
//                        pNum[2] = SND_ID_DOT;   
//                        pNum[3] = SND_ID_ZRO;
//                    }
//									 
//                }
//            }
//        }
//    }
//    else
//    {
//        pNum[0] = num/10000;
//        if(pNum[0])
//        {
//            if(pNum[0]==1)
//            pNum[0] = 0;
//            pNum[1] = SND_ID_TEN;
//            pNum[2] = num%10000/1000;
//            if(pNum[2])
//            {
//                pNum[3] = num%1000/100;
//                if(pNum[3])
//                {
//                    pNum[3] =  SND_ID_DOT;
//                    pNum[4] = num%1000/100;
//                }
//                else
//                {
//                    pNum[4] = 0;                      //点零几  不够了
//                }
//                        
//            }                                                                 ////////////////////////
//            else
//            {
//                pNum[2] = num%1000/100;
//                if(pNum[2])
//                {
//                    pNum[2] = SND_ID_DOT;
//                    pNum[3] = num%1000/100;
//                    pNum[4] = num%100/10;
//                   
//                }
//                else
//                {
//                    pNum[2] = num%100/10;
//                    if(pNum[2])
//                    {
//                        pNum[2] = SND_ID_DOT;
//                        pNum[3] = SND_ID_ZRO;
//                        pNum[4] = num%100/10;
//                    }
//                    else
//                    {
//                        pNum[2] = 0;            //点零零几 不够了
//                    }
//                }
//            
//            }
//        
//        }
//        else            
//        {
//            pNum[0] = num/1000;
//            if(pNum[0])
//            {

//                pNum[1] = num%1000/100;
//                if(pNum[1])
//                {
//                    pNum[1] = SND_ID_DOT;
//                    pNum[2] = num%1000/100;
//                    pNum[3] = num%100/10;
//                    if(pNum[3])
//                    {
//                        pNum[4] = num%10;                       
//                    }
//                    else
//                    {
//                        pNum[3] = num%10;
//                        if(pNum[3])
//                        {
//                            pNum[3] = SND_ID_ZRO;
//                            pNum[4] = num%10;
//                            
//                        }                    
//                    }
//                }
//                else
//                {
//                    pNum[1] = num%100/10;
//                    if(pNum[1])
//                    {
//                        pNum[1] = SND_ID_DOT;
//                        pNum[2] = SND_ID_ZRO;
//                        pNum[3] = num%100/10;
//                        pNum[4] = num%10;
//                    }
//                    else
//                    {
//                        pNum[1] = num%10;
//                        if(pNum[1])
//                        {    
//                            
//                            pNum[4] = num%10;
//                            if(pNum[4])
//                            {                          
//                                pNum[1] = SND_ID_DOT;
//                                pNum[2] = SND_ID_ZRO;
//                                pNum[3] = SND_ID_ZRO;
//                                pNum[4] = num%10;
//                            }
//                            else
//                            {
//                                pNum[1] = 0;
//                                pNum[2] = 0;
//                                pNum[3] = 0;
//                                pNum[4] = 0;
//                            }
//                        }
//                    }
//                }
//            }
//            else
//            {
//                pNum[0] = num/100;
//                if(pNum[0])
//                {
//                    pNum[0] = SND_ID_ZRO;
//                    pNum[1] = SND_ID_DOT;
//                    pNum[2] = num/100;
//                    pNum[3] = num%100/10;
//                    if(pNum[3])
//                    {
//                        pNum[4] = num%10;
//                    }
//                    else
//                    {
//                        pNum[3] = num%10;
//                        if(pNum[3])
//                        {
//                            pNum[3] = SND_ID_ZRO;
//                            pNum[4] =  num%10;
//                        }
//                        else
//                        {
//                            pNum[3] = 0;
//                        }
//                    }
//                }
//                else
//                {
//                    pNum[0] = num/10;
//                    if(pNum[0])
//                    {
//                        pNum[0] =  SND_ID_ZRO;
//                        pNum[1] = SND_ID_DOT;
//                        pNum[2] = SND_ID_ZRO;
//                        pNum[3] = num/10;
//                        pNum[4] = num%10;
//                    }
//                    else
//                    {  
//                        pNum[0] = num%10;
//                        if(pNum[0])
//                        {
//                            pNum[0] = SND_ID_ZRO;
//                            pNum[1] = SND_ID_DOT;
//                            pNum[2] = SND_ID_ZRO;
//                            pNum[3] = SND_ID_ZRO;
//                            pNum[4] = num%10; 
//                        }
//                        else
//                        {
//                            pNum[0] = SND_ID_ZRO;
//                        }
//                    }
//                }
//              
//            }
//        }
//    
//    }
//		return NULL;
}





