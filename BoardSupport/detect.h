#ifndef _DETECT_H
#define _DETECT_H
#include "boat_struct.h"
#include "stub.h"

void detect(void);
void detectInit(void);

typedef struct PloPoint
{
   Point point;
   struct PloPoint *next;
}PloPoint;

#endif







