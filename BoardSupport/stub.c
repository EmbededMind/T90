#include "stub.h"
#include "math.h"
#include "T90.h"
#include "detect.h"


Stub stubs[STUB_NUM];


static void PrintStubInfo(void);
static void FillStubInfo(void);


void StubRefresh()
{
	stubs[0].basePoint.x = 0;
	stubs[0].basePoint.y = 0;
	stubs[1].basePoint.x = -t90_set.dst.dst2*M_TO_MILLINM;
	stubs[1].basePoint.y = -t90_set.dst.dst1*M_TO_MILLINM;
	stubs[2].basePoint.x = 0;
	stubs[2].basePoint.y = -t90_set.dst.dst3*M_TO_MILLINM;
	stubs[3].basePoint.x =  t90_set.dst.dst4*M_TO_MILLINM;
	stubs[3].basePoint.y = -t90_set.dst.dst5*M_TO_MILLINM;
	FillStubInfo();
	detectInit();
//	PrintStubInfo();
}

Point GetRelativePoint(Point point1, Point point2)
{
	Point point;
	point.x = point2.x - point1.x;
	point.y = point2.y - point1.y;
	return point;
}

int GetDistance(Point point1, Point point2)
{
	Point point = GetRelativePoint(point1, point2);
	return (int)sqrt(point.x*point.x + point.y*point.y);
}

static void PrintStubInfo()
{
	int i;
	printf("\r\n");
	printf("StubInfo:\r\n");
	for(i = 0; i < STUB_NUM; i++)
	{
		printf("stub[%d].basePoint:(%d, %d)\r\n", i, stubs[i].basePoint.x, stubs[i].basePoint.y);
		printf("stub[%d].tang1.point:(%d, %d)\r\n", i, stubs[i].tang1.point.x, stubs[i].tang1.point.y);
		printf("stub[%d].tang1.angle:%d\r\n", i, stubs[i].tang1.angle);
		printf("stub[%d].tang2.point:(%d, %d)\r\n", i, stubs[i].tang2.point.x, stubs[i].tang2.point.y);
		printf("stub[%d].tang2.angle:%d\r\n", i, stubs[i].tang2.angle);
	}	
	printf("\r\n");	
}

static void FillStubInfo(void)
{
	int i;
	int dist;
	Point point;
	for(i = 0; i < STUB_NUM; i++)
	{
		int j = (i+1 == STUB_NUM)? 0: i+1;
		
		point = GetRelativePoint(stubs[i].basePoint, stubs[j].basePoint);

		dist = GetDistance(stubs[i].basePoint, stubs[j].basePoint);

		stubs[i].tang1.point.x =  t90_set.alarm.invd_dst*point.y/dist + stubs[i].basePoint.x;
		stubs[i].tang1.point.y = -t90_set.alarm.invd_dst*point.x/dist + stubs[i].basePoint.y;
		stubs[i].tang1.angle = atan2(stubs[i].tang1.point.y - stubs[i].basePoint.y, 
																 stubs[i].tang1.point.x - stubs[i].basePoint.x)*RAD_TO_ANGLE;
		
		stubs[j].tang2.point.x = stubs[i].tang1.point.x + point.x;
		stubs[j].tang2.point.y = stubs[i].tang1.point.y + point.y;
		stubs[j].tang2.angle = atan2(stubs[j].tang2.point.y - stubs[j].basePoint.y, 
																 stubs[j].tang2.point.x - stubs[j].basePoint.x)*RAD_TO_ANGLE;
	}
}

Point STUB_GetYMin(void)
{
	int i;
	Point point;
	for(i = 0; i < STUB_NUM; i++)
	{
		if(stubs[i].basePoint.y < point.y)
		{
			point.y = stubs[i].basePoint.y;
			point.x = stubs[i].basePoint.x;
		}
	}
	return point;
}
		




















