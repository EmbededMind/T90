#ifndef _STUB_H
#define _STUB_H

#include "layout.h"
#include "lpc_types.h"


//#define STUB_NUM_SINGLE  4
#define STUB_NUM_MAX  5

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
	
typedef enum StubType
{
	motherStub,
	aidedStub,
	safetySignStub
} StubType;

typedef struct Stub
{
	Point basePoint;
	Tangency tang1;
	Tangency tang2;
   Point crossPoint;
	Bool isValid;
	StubType type;
} Stub;

typedef struct StubNode
{
	Stub *pStub;
	struct StubNode *pNext;
} StubNode;

extern Stub stubs[STUB_NUM_MAX];


extern StubNode *pStubHead;



extern StubNode *pStubHead;

//void Stub_setParam(int which, int distX, int distY,Bool isValid);
void Stub_setParam(int which, int distX, int distY);
void Stub_setValidity(int which, Bool validity);
void StubRefresh(void);
Point GetRelativePoint(Point, Point);
int GetDistance(Point, Point);

int STUB_GetMostValue(char type);

int FetchMidStub(void);











#endif

