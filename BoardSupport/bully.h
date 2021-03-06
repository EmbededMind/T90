#ifndef _BULLY_H
#define _BULLY_H



#include "Config.h"
#include "lpc_types.h"
#include "boat_struct.h"

#define HIGHTIMES 2

typedef struct _BULY_BERTH BULY_BERTH;
struct _BULY_BERTH
{
   BERTH* pBoatLink;
   BULY_BERTH* pNext;
};

extern int  validCnt;
extern BULY_BERTH* pBulyHeader;

unsigned char BULY_parseNation(long id);
Bool BULY_add(BERTH* pBoatLink);
void BULY_delete(BERTH* addr);
BULY_BERTH* BULY_gov_fetchNextPlayBerth(void);
BULY_BERTH* BULY_highSpeed_fetchNextPlayBerth(void);
int BULY_getValidNumber(void);
void BULY_maskAllBerth(void );
void BULY_dump(void);


#endif

