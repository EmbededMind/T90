#ifndef _STUB_H
#define _STUB_H

#include "layout.h"

#define STUB_NUM         4

#define PI               3.1416
#define RAD_TO_ANGLE     180/PI
#define ANGLE_TO_RAD     PI/180

#define M_TO_MILLINM     20/37
#define MILLINM_TO_M     37/20



//#define SHIP_FAMILY_PIXEL_HEIGHT   

typedef struct Point
{
	int x;
	int y;
} Point;

typedef struct Tangency
{
	Point point;
	int angle;
} Tangency;
	
typedef struct Stub
{
	Point basePoint;
	Tangency tang1;
	Tangency tang2;
} Stub;

extern Stub stubs[STUB_NUM];


void StubRefresh(void);
Point GetRelativePoint(Point, Point);
int GetDistance(Point, Point);

Point STUB_GetYMin(void);

int FetchMidStub();











#endif

