
//     此函数为关于母船和安全标的函数
//     Stub结构体里包括桩点和桩点两侧的切点

#include "stub.h"
#include "math.h"
#include "T90.h"
#include "detect.h"
#include "transform.h"


Stub stubs[STUB_NUM_MAX_D];
StubNode *pStubHead = NULL;


static void PrintStubInfo(void);
static void FillStubInfo(void);
static void FillStubNodes(void);

void StubRefresh()   //根据设置的距离计算桩点的坐标
{
	if(t90_set.sys.workmode == SINGLE_MODE)
	{
		stubs[0].basePoint.x = 0;
		stubs[0].basePoint.y = 0;

		stubs[0].isValid = TRUE;

		stubs[0].isValid = 1;

		stubs[0].type = motherStub;
		stubs[0].StubNum = 0;
      
		stubs[1].basePoint.x = -t90_set.singledst_set.dst2*M_TO_MILLINM;
		stubs[1].basePoint.y = -t90_set.singledst_set.dst1*M_TO_MILLINM;


		stubs[1].type = safetySignStub;
		stubs[1].StubNum = 1;
		stubs[2].basePoint.x = 0;
		stubs[2].basePoint.y = -t90_set.singledst_set.dst3*M_TO_MILLINM;



		stubs[2].type = safetySignStub;
		stubs[2].StubNum = 2;
		stubs[3].basePoint.x =  t90_set.singledst_set.dst4*M_TO_MILLINM;
		stubs[3].basePoint.y = -t90_set.singledst_set.dst5*M_TO_MILLINM;

		stubs[3].type = safetySignStub;
		stubs[3].StubNum = 3;
		stubs[4].isValid = FALSE;
  


		stubs[3].type = safetySignStub;
		
		stubs[4].isValid = 0;
      stubs[4].StubNum = 4;
  


	}
	else if(t90_set.sys.workmode == DOUBLE_MODE)
	{      

		stubs[0].basePoint.x = 0;
		stubs[0].basePoint.y = 0;
		stubs[0].isValid = 1;
		stubs[0].type = motherStub;
		stubs[0].StubNum = 0;
      
		stubs[1].basePoint.x = -(t90_set.doubledst_set.net_port - t90_set.doubledst_set.mo_to_as) * M_TO_MILLINM / 2;
		stubs[1].basePoint.y = -sqrt(-stubs[1].basePoint.x * stubs[1].basePoint.x + 
                         t90_set.doubledst_set.safety1_to_mo * t90_set.doubledst_set.safety1_to_mo*M_TO_MILLINM*M_TO_MILLINM);

		stubs[1].type = safetySignStub;
		stubs[1].StubNum = 1;
		stubs[2].basePoint.x = t90_set.doubledst_set.mo_to_as * M_TO_MILLINM/ 2;
		stubs[2].basePoint.y = -sqrt(t90_set.doubledst_set.safety2_to_mo * t90_set.doubledst_set.safety2_to_mo*M_TO_MILLINM*M_TO_MILLINM 
                             - stubs[2].basePoint.x * stubs[2].basePoint.x);

		stubs[2].type = safetySignStub;
		stubs[2].StubNum = 2;
		stubs[3].basePoint.x = (t90_set.doubledst_set.net_port + t90_set.doubledst_set.mo_to_as)*M_TO_MILLINM/2;
		stubs[3].basePoint.y = -sqrt(t90_set.doubledst_set.safety3_to_mo * t90_set.doubledst_set.safety3_to_mo 
                         - (t90_set.doubledst_set.net_port - t90_set.doubledst_set.mo_to_as) * 
                           (t90_set.doubledst_set.net_port - t90_set.doubledst_set.mo_to_as) / 4)*M_TO_MILLINM;

		stubs[3].type = safetySignStub;
		stubs[3].StubNum = 3;
  stubs[4].basePoint.x = t90_set.doubledst_set.mo_to_as*M_TO_MILLINM;
		stubs[4].basePoint.y = 0;
		stubs[4].basePoint.y = 0;

		stubs[4].isValid = TRUE;
		stubs[4].type = aidedStub;
      stubs[4].StubNum = 4;
      

	}
	FillStubNodes();
	FillStubInfo();	
}



void Stub_setValidity(int which, Bool validity)
{
   
   if(which > 0  &&  which < 4){
      stubs[which].isValid  = validity;
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
	for(i = 0; i < STUB_NUM_MAX_D; i++)
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
   pIndex = pStubHead;
   if(pIndex)
   {
      do{
         pIndex->pNext->pPrev = pIndex;
         pIndex = pIndex->pNext;
      }while(pIndex != pStubHead);
   }
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

//static void PrintStubInfo()
//{
//	int i = 0;	
//	StubNode *pIndex = pStubHead;
//	printf("\r\n");
//	printf("StubInfo:\r\n");
//	if(pIndex)
//	{
//		do
//		{
//			printf("stub[%d].basePoint:(%d, %d)\r\n", i, pIndex->pStub->basePoint.x, pIndex->pStub->basePoint.y);
//			printf("stub[%d].tang1.point:(%d, %d)\r\n", i, pIndex->pStub->tang1.point.x, pIndex->pStub->tang1.point.y);
//			printf("stub[%d].tang1.angle:%d\r\n", i, pIndex->pStub->tang1.angle);
//			printf("stub[%d].tang2.point:(%d, %d)\r\n", i, pIndex->pStub->tang2.point.x, pIndex->pStub->tang2.point.y);
//			printf("stub[%d].tang2.angle:%d\r\n", i, pIndex->pStub->tang2.angle);
//			i++;
//			pIndex = pIndex->pNext;
//		}
//		while(pIndex != pStubHead);
//	}
//	printf("\r\n");	
//}

static void FillStubInfo(void)    //根据桩点坐标计算桩点两侧切点的信息
{
	int dist;
   Point pointa,pointb,pointc,pointd;
   int numValid = 0;
	Point point;
	StubNode *pIndex = pStubHead;
   int A1, A2, B1, B2, C1, C2;
   A1 = A2 = B1 = B2 = C1 = C2 = 0;
   
   
	if(pIndex)
	{
		do
		{
   numValid++;
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

         pIndex = pStubHead;
         do
         {
            pointa.x = pIndex->pPrev->pStub->tang1.point.x;
            pointa.y = pIndex->pPrev->pStub->tang1.point.y;
            pointb.x = pIndex->pStub->tang2.point.x;
            pointb.y = pIndex->pStub->tang2.point.y;
            pointc.x = pIndex->pStub->tang1.point.x;
            pointc.y = pIndex->pStub->tang1.point.y;
            pointd.x = pIndex->pNext->pStub->tang2.point.x;
            pointd.y = pIndex->pNext->pStub->tang2.point.y;
            
           	A1 = pointb.y - pointa.y;
            B1 = pointa.x - pointb.x;
            C1 = pointa.y * pointb.x - pointa.x * pointb.y;
            A2 = pointd.y - pointc.y;
            B2 = pointc.x - pointd.x;
            C2 = pointc.y + pointd.x - pointc.x * pointd.y;
            if(B1*A2 - B2*A1)
            {
               pIndex->pStub->crossPoint.x = (C1 * B2 - C2 * B1) / (B1 * A2 - B2 * A1);
               pIndex->pStub->crossPoint.y = (C2 * A1 - C1 * A2) / (B1 * A2 - B2 * A1);           
            }
         }while(pIndex != pStubHead);
        
	}

}





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
    for(i = 1; i < STUB_NUM_MAX_D; i++)
    {
        if(stubs[i].isValid && stubs[i].basePoint.y > maxpoint_y)
            maxpoint_y  = stubs [i].basePoint.y;        
    }
    
    return maxpoint_y/2;
}

int IsLineCrossTwoPoint(int A, int B, int C, Point point1, Point point2)
{
	if ((point1.x * A + point1.y * B + C) * (point2.x * A + point2.y * B + C) < 0)
	{
		return 1;
	}
	return 0;
}

int IsLineSegmCross(Point pointa, Point pointb, Point pointc, Point pointd)
{
	int flga, flgb;
	int A1, B1, C1, A2, B2, C2;

	A1 = pointb.y - pointa.y;
	B1 = pointa.x - pointb.x;
	C1 = pointa.y * pointb.x - pointa.x * pointb.y;
	A2 = pointd.y - pointc.y;
	B2 = pointc.x - pointd.x;
	C2 = pointc.y + pointd.x - pointc.x * pointd.y;

	if (IsLineCrossTwoPoint(A1, B1, C1, pointc, pointd) && IsLineCrossTwoPoint(A2, B2, C2, pointa, pointb))
	{
		return 1;
	}
	return 0;


}

















