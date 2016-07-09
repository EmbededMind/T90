#ifndef _NEWS_COM_H
#define _NEWS_COM_H
#include "stdint.h"

enum NewsState{nonews,willsend,sended,overtime};
static uint16_t newscom[10][18];

typedef struct NewsCom
{
	uint16_t newsCom[18];
   enum NewsState newsState;	
	struct NewsCom *next;
}NewsCom;

extern NewsCom *newsCom;


#endif