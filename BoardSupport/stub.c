
//     此函数为关于母船和安全标的函数
//     Stub结构体里包括桩点和桩点两侧的切点

#include "stub.h"
#include "math.h"
#include "T90.h"
#include "detect.h"
#include "transform.h"


Stub stubs[STUB_NUM_MAX];
StubNode *pStubHead = NULL;


static void PrintStubInfo(void);
static void FillStubInfo(void);
static void FillStubNodes(void);



void Stub_setParam(int which, int distX, int distY)
{
   if(t90_set.sys.workmode == SINGLE_MODE){
      switch(which){
         case 1:
              t90_set.dst.dst2 = distX;
              t90_set.dst.dst1 = distY;
              break;
         case 2:

              t90_set.dst.dst3 = distY;
              break;
         case 3:
              t90_set.dst.dst4 = distX;
              t90_set.dst.dst5 = distY;

              break;
         default:
              INFO("stub number is out of range!");
              break;
      }
   }
   else if(t90_set.sys.workmode  == DOUBLE_MODE){
   
   }
}




void StubRefresh()   //根据设置的距离计算桩点的坐标
{

	if(t90_set.sys.workmode == SINGLE_MODE)
	{
		stubs[0].basePoint.x = 0;
		stubs[0].basePoint.y = 0;

		stubs[0].isValid = TRUE;

		stubs[0].isValid = 1;

		stubs[0].type = motherStub;
		
		stubs[1].basePoint.x = -t90_set.dst.dst2*M_TO_MILLINM;
		stubs[1].basePoint.y = -t90_set.dst.dst1*M_TO_MILLINM;

		stubs[1].isValid = 1;

		stubs[1].type = safetySignStub;
		
		stubs[2].basePoint.x = 0;
		stubs[2].basePoint.y = -t90_set.dst.dst3*M_TO_MILLINM;


		stubs[2].isValid = 1;

		stubs[2].type = safetySignStub;
		
		stubs[3].basePoint.x =  t90_set.dst.dst4*M_TO_MILLINM;
		stubs[3].basePoint.y = -t90_set.dst.dst5*M_TO_MILLINM;

		stubs[3].type = safetySignStub;
		
		stubs[4].isValid = FALSE;
  

		stubs[3].isValid = 1;
		stubs[3].type = safetySignStub;
		
		stubs[4].isValid = 0;
  
  
  stubs[1].isValid  = 0;
  stubs[2].isValid  = 0;
  stubs[3].isValid  = 0;

	}
	else if(t90_set.sys.workmode == DOUBLE_MODE)
	{
		stubs[0].basePoint.x = 0;
		stubs[0].basePoint.y = 0;

		stubs[0].isValid = TRUE;

		stubs[0].isValid = 1;

		stubs[0].type = motherStub;
		
		stubs[1].basePoint.x = 50*M_TO_MILLINM;
		stubs[1].basePoint.y = -400*M_TO_MILLINM;



		stubs[1].isValid = 1;

		stubs[1].type = safetySignStub;
		
		stubs[2].basePoint.x = 100*M_TO_MILLINM;
		stubs[2].basePoint.y = -500*M_TO_MILLINM;

		stubs[2].isValid = 1;

		stubs[2].type = safetySignStub;
		
		stubs[3].basePoint.x =  150*M_TO_MILLINM;
		stubs[3].basePoint.y = -400*M_TO_MILLINM;



		stubs[3].isValid = 1;

		stubs[3].type = safetySignStub;
		
		stubs[4].basePoint.x = 200*M_TO_MILLINM;
		stubs[4].basePoint.y = 0;

		stubs[4].isValid = TRUE;

		stubs[4].isValid = 1;

		stubs[4].type = aidedStub;
	}
	FillStubNodes();
	FillStubInfo();
	detectInit();
//	PrintStubInfo();
}



void Stub_setValidity(int which, Bool validity)
{
   if(which > 0  &&  which < 4){
      stubs[which].isValid  = validity;
      if(validity == FALSE){
         switch(which){
            case 1: t90_set.dst.dst1 = DEFAULT_DST1; t90_set.dst.dst2 = DEFAULT_DST2; break;
            case 2: t90_set.dst.dst3 = DEFAULT_DST3;  break;
            case 3: t90_set.dst.dst4 = DEFAULT_DST4; t90_set.dst.dst5 = DEFAULT_DST5; break;
         }
      }
   }
}


static void FillStubNodes(void)
{
	int i;
	StubNode *pTemp, *pIndex = NULL;
	pIndex = pStubHead;
	if(pIndex)         //删除链表中所有节点
	{
		pIndex = pIndex->pNext;
		if(pIndex == pStubHead)
			free(pIndex);
		else
		{
			do
			{
				pTemp = pIndex;
				pIndex = pIndex->pNext;
				free(pTemp);
			}
			while(pIndex != pStubHead);
			free(pIndex);
		}
	}                 //删除完成
	for(i = 0; i < STUB_NUM_MAX; i++)
	{
		if(stubs[i].isValid)
		{
			pTemp = (StubNode *)malloc(sizeof(StubNode));
			if(pStubHead == NULL)
			{
				pStubHead = pTemp;
				pIndex = pStubHead;
				pIndex->pStub = &stubs[i];
			}
			else
			{
				pIndex->pNext = pTemp;
				pIndex = pTemp;
				pIndex->pStub = &stubs[i];
			}
		}
	}
	if(pIndex)
		pIndex->pNext = pStubHead;
}

Point GetRelativePoint(Point point1, Point point2)   //两点的相对坐标
{
	Point point;
	point.x = point2.x - point1.x;
	point.y = point2.y - point1.y;
	return point;
}

int GetDistance(Point point1, Point point2)   //两点间的距离
{
	Point point = GetRelativePoint(point1, point2);
	return (int)(sqrt(point.x*point.x + point.y*point.y)+0.5);
}

static void PrintStubInfo()
{
	int i = 0;	
	StubNode *pIndex = pStubHead;
	printf("\r\n");
	printf("StubInfo:\r\n");
	if(pIndex)
	{
		do
		{
			printf("stub[%d].basePoint:(%d, %d)\r\n", i, pIndex->pStub->basePoint.x, pIndex->pStub->basePoint.y);
			printf("stub[%d].tang1.point:(%d, %d)\r\n", i, pIndex->pStub->tang1.point.x, pIndex->pStub->tang1.point.y);
			printf("stub[%d].tang1.angle:%d\r\n", i, pIndex->pStub->tang1.angle);
			printf("stub[%d].tang2.point:(%d, %d)\r\n", i, pIndex->pStub->tang2.point.x, pIndex->pStub->tang2.point.y);
			printf("stub[%d].tang2.angle:%d\r\n", i, pIndex->pStub->tang2.angle);
			i++;
			pIndex = pIndex->pNext;
		}
		while(pIndex != pStubHead);
	}
	printf("\r\n");	
}

static void FillStubInfo(void)    //根据桩点坐标计算桩点两侧切点的信息
{
	int dist;
	Point point;
	StubNode *pIndex = pStubHead;


	if(pIndex)
	{
		do
		{
			point = GetRelativePoint(pIndex->pStub->basePoint, pIndex->pNext->pStub->basePoint);

			dist = GetDistance(pIndex->pStub->basePoint, pIndex->pNext->pStub->basePoint);

			pIndex->pStub->tang1.point.x =  t90_set.alarm.invd_dst*point.y/dist + pIndex->pStub->basePoint.x;
			pIndex->pStub->tang1.point.y = -t90_set.alarm.invd_dst*point.x/dist + pIndex->pStub->basePoint.y;
			pIndex->pStub->tang1.angle = atan2(pIndex->pStub->tang1.point.y - pIndex->pStub->basePoint.y, 
																				 pIndex->pStub->tang1.point.x - pIndex->pStub->basePoint.x)*RAD_TO_ANGLE+1;
			
			pIndex->pNext->pStub->tang2.point.x = pIndex->pStub->tang1.point.x + point.x;
			pIndex->pNext->pStub->tang2.point.y = pIndex->pStub->tang1.point.y + point.y;
			pIndex->pNext->pStub->tang2.angle = atan2(pIndex->pNext->pStub->tang2.point.y - pIndex->pNext->pStub->basePoint.y, 
																								pIndex->pNext->pStub->tang2.point.x - pIndex->pNext->pStub->basePoint.x)*RAD_TO_ANGLE-1;
			pIndex = pIndex->pNext;
		}
		while(pIndex != pStubHead);
	}
}

//int STUB_GetMostValue(char type)
//{	
//   StubNode *pIndex = pStubHead;
//   
//   switch(type)
//   {
//      case X_MAX:
//      {
//         int xMax = -99999;
//         
//         if(pIndex)
//         {
//            do
//            {
//               if(pIndex->pStub->basePoint.x > xMax)
//                  xMax = pIndex->pStub->basePoint.x;
//               pIndex = pIndex->pNext;
//            }
//            while(pIndex != pStubHead);
//         }
//         return xMax;
//      }

//      case X_MIN:
//      {
//         int xMin = 99999;
//         
//         if(pIndex)
//         {
//            do
//            {
//               if(pIndex->pStub->basePoint.x < xMin)
//                  xMin = pIndex->pStub->basePoint.x;
//               pIndex = pIndex->pNext;
//            }
//            while(pIndex != pStubHead);
//         }
//         return xMin;
//      }

//      
//      case Y_MAX:
//      {
//         int yMax = -99999;
//         
//         if(pIndex)
//         {
//          do
//          {
//             if(pIndex->pStub->basePoint.y > yMax)
//                yMax = pIndex->pStub->basePoint.y;
//             pIndex = pIndex->pNext;
//            }
//            while(pIndex != pStubHead);
//         }
//         return yMax;
//      }

//      
//      case Y_MIN:
//      {
//         int yMin = 99999;
//         
//         if(pIndex)
//         {
//            do
//            {
//               if(pIndex->pStub->basePoint.y < yMin)
//                  yMin = pIndex->pStub->basePoint.y;
//               pIndex = pIndex->pNext;
//            }
//            while(pIndex != pStubHead);
//         }
//         return yMin;
//      }

//   }
//}



int STUB_GetMostValue(char type)
{	
	StubNode *pIndex = pStubHead;
	switch(type)
	{
		case X_MAX:
		{
			int xMax = -99999;
			if(pIndex)
			{
				do
				{
					if(pIndex->pStub->basePoint.x > xMax)
						xMax = pIndex->pStub->basePoint.x;
					pIndex = pIndex->pNext;
				}
				while(pIndex != pStubHead);
			}
			return xMax;
		}
		break;
		case X_MIN:
		{
			int xMin = 99999;
			if(pIndex)
			{
				do
				{
					if(pIndex->pStub->basePoint.x < xMin)
						xMin = pIndex->pStub->basePoint.x;
					pIndex = pIndex->pNext;
				}
				while(pIndex != pStubHead);
			}
			return xMin;
		}
		break;
		case Y_MAX:
		{
			int yMax = -99999;
			if(pIndex)
			{
				do
				{
					if(pIndex->pStub->basePoint.y > yMax)
						yMax = pIndex->pStub->basePoint.y;
					pIndex = pIndex->pNext;
				}
				while(pIndex != pStubHead);
			}
			return yMax;
		}
		break;
		case Y_MIN:
		{
			int yMin = 99999;
			if(pIndex)
			{
				do
				{
					if(pIndex->pStub->basePoint.y < yMin)
						yMin = pIndex->pStub->basePoint.y;
					pIndex = pIndex->pNext;
				}
				while(pIndex != pStubHead);
			}
			return yMin;
		}
		break;
	}

}
		
int FetchMidStub()
{
    int i = 0;
    int maxpoint_y = -99999;
    for(i = 1; i < STUB_NUM; i++)
    {
        if(stubs[i].basePoint.y > maxpoint_y)
            maxpoint_y  = stubs [i].basePoint.y;        
    }
    
    return maxpoint_y/2;
}



















