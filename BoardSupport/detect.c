
#include "boat_struct.h"
#include "T90.h"
#include "math.h"
#include "detect.h"
#include "transform.h"

#define SQUARE(X) ((X)*(X))
#define MAXNUM(a,b) (a>b? a:b)
#define MINNUM(a,b) (a<b? a:b)
/**
 *
 *
 */
static unsigned int pointInPolygon = 0;

static PloPoint *pmin, *pmax;

//int getAllPointYMax()
//{
//   int i,max=-999;
//   for(i = 0; i < STUB_NUM_MAX_D; i++)
//   {
//      if(stubs[i].isValid)
//      {
//         if(stubs[i].basePoint.y > max)
//         {
//            max = stubs[i].basePoint.y;
//         }
//         if(stubs[i].tang1.point.y > max)
//         {
//            max = stubs[i].tang1.point.y;
//         }
//         if(stubs[i].tang2.point.y > max)
//         {
//            max = stubs[i].tang2.point.y;
//         }
//      }
//   }
//   return max;
//}

//int getAllPointYMin()
//{
//   int i, min = 999;
//   for(i = 0; i < STUB_NUM_MAX_D; i++)
//   {
//      if(stubs[i].isValid)
//      {
//         if(stubs[i].basePoint.y < min)
//         {
//            min = stubs[i].basePoint.y;
//         }
//         if(stubs[i].tang1.point.y < min)
//         {
//            min = stubs[i].tang1.point.y;
//         }
//         if(stubs[i].tang2.point.y < min)
//         {
//            min = stubs[i].tang2.point.y;
//         }      
//      }         
//   }
//   return min;
//}

void adjustPlo()
{
   int i;
   int tmp;
   PloPoint *index, *ptmp;
   
   if(pmin)
   {
      index = pmin->next;
      do
      {
         ptmp = index;
         index = index->next;
         free(ptmp);
      }while(index != pmin);
   
   }
   pmin = malloc(sizeof(PloPoint));
   pmax = malloc(sizeof(PloPoint));
   pmin->next = pmax;
   pmax->next = pmin;
   pmax->point.x = -9999;
   pmin->point.x = 9999;
   
   for(i = 0; i < STUB_NUM_MAX_D; i++)
   {
      
      if(stubs[i].isValid)
      {
         if(stubs[i].tang1.point.x > pmax->point.x)
         {
            pmax->point.x = stubs[i].tang1.point.x;
            pmax->point.y = stubs[i].tang1.point.y;            
         }
         if(stubs[i].tang1.point.x < pmin->point.x)
         {
            pmin->point.x = stubs[i].tang1.point.x;
            pmin->point.y = stubs[i].tang1.point.y;
         }
         if(stubs[i].tang2.point.x > pmax->point.x)
         {
            pmax->point.x = stubs[i].tang2.point.x;
            pmax->point.y = stubs[i].tang2.point.y; 
         }
         if(stubs[i].tang2.point.x < pmin->point.x)
         {
            pmin->point.x = stubs[i].tang2.point.x;
            pmin->point.y = stubs[i].tang2.point.y;
         }
      }       
   }
//   printf("max x = %d\n",pmax->point.x);
//   printf("max y = %d\n",pmax->point.y);
//   printf("min x = %d\n",pmin->point.x);
//   printf("min y = %d\n",pmin->point.y);
   for(i = 0; i < STUB_NUM_MAX_D; i++)
   {
      
      if(stubs[i].isValid )
      {
         if(Mem_isEqual(&stubs[i].tang1.point,&pmax->point,sizeof(Point)))
            goto jmptang1;
         if(Mem_isEqual(&stubs[i].tang1.point,&pmin->point,sizeof(Point)))
            goto jmptang1;
         if (((pmin->point.x - pmax->point.x)*stubs[i].tang1.point.y + (pmax->point.y - pmin->point.y)*stubs[i].tang1.point.y
				+ pmin->point.y*pmax->point.x - pmax->point.y*pmin->point.x)*(pmin->point.x - pmax->point.x) > 0)
         {
            index = pmax;
            do
            {
               if (stubs[i].tang1.point.x <= index->point.x && stubs[i].tang1.point.x >= index->next->point.x)
					{
						ptmp = malloc(sizeof(PloPoint));
						ptmp->next = index->next;
						index->next = ptmp;
						ptmp->point.x = stubs[i].tang1.point.x;
						ptmp->point.y = stubs[i].tang1.point.y;
						break;
					}
               index = index->next;
            }while(index != pmin);
         }
         else
         {
            index = pmin;
            do
            {
               if (stubs[i].tang1.point.x >= index->point.x && stubs[i].tang1.point.x <= index->next->point.x)
					{
						ptmp = malloc(sizeof(PloPoint));
						ptmp->next = index->next;
						index->next = ptmp;
						ptmp->point.x = stubs[i].tang1.point.x;
						ptmp->point.y = stubs[i].tang1.point.y;
						break;
					}
               index = index->next;
            }while(index != pmax);
         }
        
      }
jmptang1:     
      if(stubs[i].isValid )
      {
         if(Mem_isEqual(&stubs[i].tang2.point,&pmax->point,sizeof(Point)))
            goto jmptang2;
         if(Mem_isEqual(&stubs[i].tang2.point,&pmin->point,sizeof(Point)))
            goto jmptang2;
         if (((pmin->point.x - pmax->point.x)*stubs[i].tang2.point.y + (pmax->point.y - pmin->point.y)*stubs[i].tang2.point.y
				+ pmin->point.y*pmax->point.x - pmax->point.y*pmin->point.x)*(pmin->point.x - pmax->point.x) > 0)
         {
            index = pmax;
            do
            {
               if (stubs[i].tang2.point.x <= index->point.x && stubs[i].tang2.point.x >= index->next->point.x)
					{
						ptmp = malloc(sizeof(PloPoint));
						ptmp->next = index->next;
						index->next = ptmp;
						ptmp->point.x = stubs[i].tang2.point.x;
						ptmp->point.y = stubs[i].tang2.point.y;
						break;
					}
               index = index->next;
            }while(index != pmin);
         }
         else
         {
            index = pmin;
            do
            {
               if (stubs[i].tang2.point.x >= index->point.x && stubs[i].tang2.point.x <= index->next->point.x)
					{
						ptmp = malloc(sizeof(PloPoint));
						ptmp->next = index->next;
						index->next = ptmp;
						ptmp->point.x = stubs[i].tang2.point.x;
						ptmp->point.y = stubs[i].tang2.point.y;
						break;
					}
               index = index->next;
            }while(index != pmax);
         }
        
      }
jmptang2:
      NULL;
   }
   
//   index = pmin;
//   do
//   {
//      printf("x = %d\n",index->point.x);
//      printf("y = %d\n",index->point.y);
//      index = index->next;
//   }while(index != pmin);      
}
int addLeft(BERTH *pBerth, int x1, int y1, int x2, int y2)
{
   
   if(x1 + (pBerth->y_to_cross-y1)*(x2-x1)/(y2-y1) <= pBerth->x_to_cross)
   {
      return 1;
   }
   return 0;
}

int isCrossPointInLeft(BERTH *pBerth, Point pointa, Point pointb)
{
//   static int same_y = 99999; 
   if(pointa.y == pointb.y)
   {
//      if(pointa.y == same_y && same_y != 99999)
//         return 0;
//      same_y = pointa.y;      
//      if(pBerth->y_to_cross == pointa.y)
//      {                     
//         return 1;               
//      }
      return 0;
   }
   else 
   {
//      same_y = 99999;
      if (pBerth->y_to_cross > MINNUM(pointa.y,pointb.y) && pBerth->y_to_cross <= MAXNUM(pointa.y,pointb.y))
      {          
         if(pointa.y > pointb.y)
         {         
            return addLeft(pBerth, pointb.x, pointb.y, pointa.x, pointa.y);
         }
         else
         {         
            return addLeft(pBerth, pointa.x, pointa.x, pointb.x, pointb.y);
         }
      }       
   }
   return 0;
}



void detectInit()
{
   
//    int i,j;
//    int stubNumMax;
//	 if(t90_set.sys.workmode == SINGLE_MODE)
//	 {
//		  stubNumMax = STUB_NUM_MAX_S;
//	 }
//	 else
//	 {
//		  stubNumMax = STUB_NUM_MAX_D;
//	 }
//    for(i = 0; i < stubNumMax; i++)
//    {
//			
//       if(stubs[i].isValid)
//		 {
//          if(i == stubNumMax - 1)
//          {
//              j = 0;
//          }
//          else
//          {
//              for(j = i+1; j < stubNumMax; j++)
//              {
//                  if(j == stubNumMax)
//                  {
//                     j = 0;
//                     break;
//                  }
//                  if(stubs[j].isValid)
//                  {
//                     break;
//                  }
//              }
//           }
//			  if(stubs[i].tang1.point.y*stubs[j].tang2.point.x-stubs[j].tang2.point.y*stubs[i].tang1.point.x >= 0)
//			  {
//					 pointInPolygon |= 0x00000001<<i;
//			  }
//			  else
//			  {
//					 pointInPolygon &= 0xfffffffe<<i;
//			  }
//			  
//			 
//			  if(-(stubs[i].tang1.point.x-stubs[j].tang2.point.x)+stubs[i].tang1.point.y*stubs[i].tang2.point.x-stubs[i].tang2.point.y*stubs[i].tang1.point.x >= 0)
//			  {
//					pointInPolygon |= 0x00000001<<(i+16);
//			  }
//			  else
//			  {
//					pointInPolygon &= 0xfffffffe<<(i+16);
//			  }
//        }
//    }


   PloPoint *index;
   int i = 0;
   adjustPlo();
   
   index = pmin;
   do
   {
      if(index->point.y*index->next->point.x - index->next->point.y*index->point.x >= 0)
      {
          pointInPolygon |= 0x00000001<<i;
      }
      else
      {
          pointInPolygon &= 0xfffffffe<<i;
      }

      i++;
      index = index->next;
   }while(index != pmin);     
    
   
 }

Bool isCloseStub(BERTH *pBerth)
{
    
    int i = 0;    
    for(i = 0;i < STUB_NUM_MAX_D;i++)
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

   unsigned int isinpoly = 0;
   Bool newFlg,oldFlg;
//   int i, j;
//   int flg = 0;
//   int num;

//   
//   if(t90_set.sys.workmode == SINGLE_MODE)
//   {
//      num = 4;
//   }else if(t90_set.sys.workmode == DOUBLE_MODE)
//   {
//      num = 5;
//   }
//   
//   for(i = 0; i < num; i++)
//   {
//      if(stubs[i].isValid)
//      {
//         if(i == num - 1)
//         {
//           j = 0;
//         }
//         else
//         {
//           for(j = i+1; j < num; j++)
//           {
//               if(j == num)
//               {
//                  j = 0;
//                  break;
//               }
//               if(stubs[j].isValid)
//               {
//                  break;
//               }
//           }
//         }
//         flg += isCrossPointInLeft(pBerth, stubs[i].tang1.point, stubs[i].tang2.point);
//         flg += isCrossPointInLeft(pBerth, stubs[i].tang1.point, stubs[j].tang2.point);
//      }
//   }
   int flg;
   int i;
   PloPoint *index;
   index = pmin;
   do
   {
      flg += isCrossPointInLeft(pBerth, index->point, index->next->point);
      index = index->next;
   }while(index != pmin);
   
   if(flg%2)
   {
      newFlg = 1;
   }
   else
   {
      newFlg = 0;
   }
   if(!newFlg)
      return 0;
   
//    i = 0;
//    j = 0;    
//    for(i = 0;i < num;i++)
//    {       
//      if(stubs[i].isValid)
//      {
//        if(i == num - 1)
//          {
//              j = 0;
//          }
//          else
//          {
//              for(j = i+1; j < num; j++)
//              {
//                  if(j == num)
//                  {
//                     j = 0;
//                     break;
//                  }
//                  if(stubs[j].isValid)
//                  {
//                     break;
//                  }
//                  
//              }
//           }
//         if(((stubs[i].tang1.point.x-stubs[j].tang2.point.x)*pBerth->y_to_cross + (stubs[j].tang2.point.y-stubs[i].tang1.point.y)*pBerth->x_to_cross
//            +stubs[i].tang1.point.y*stubs[j].tang2.point.x-stubs[j].tang2.point.y*stubs[i].tang1.point.x) > 0)
//         {
//            isinpoly |= 0x00000001<<i;
//         }
//         else
//         {
//            isinpoly &= 0xfffffffe<<i;
//         }
//         if(((stubs[i].tang1.point.x-stubs[i].tang2.point.x)*pBerth->y_to_cross + (stubs[i].tang2.point.y-stubs[i].tang1.point.y)*pBerth->x_to_cross
//            +stubs[i].tang1.point.y*stubs[i].tang2.point.x-stubs[i].tang2.point.y*stubs[i].tang1.point.x) > 0)
//         {
//            isinpoly |= 0x00000001<<(i+16);
//         }
//         else
//         {
//            isinpoly &= 0xfffffffe<<(i+16);
//         }
//	    }
//    }
    index = pmin;
    i = 0;
    do
    {
         if(((index->point.x-index->next->point.x)*pBerth->y_to_cross + (index->next->point.y-index->point.y)*pBerth->x_to_cross
            +index->point.y*index->next->point.x-index->next->point.y*index->point.x) > 0)
         {
            isinpoly |= 0x00000001<<i;
         }
         else
         {
            isinpoly &= 0xfffffffe<<i;
         }
         i++;
         index = index->next;
    } while(index != pmin);
    
    if(isinpoly == pointInPolygon)
    {    
        oldFlg = TRUE;
    }
    else
    {
       oldFlg = FALSE;
    }
    if(newFlg == TRUE && oldFlg == TRUE)
    {
       return 1;
    }
    return 0;
   


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
     detectInit();
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





