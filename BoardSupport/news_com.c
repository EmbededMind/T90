#include "news_com.h"
#include "T90.h"

void newsComInit()
{
   int i = 0;
   uint16_t newscom[10][18] = {0};
   NewsCom *newsCom,*pnext,*p;
   
   newsCom = malloc(sizeof(NewsCom));
   p = newsCom;
   for(i = 0; i < 17; i++)
   {
      p->newsState = nonews;
//      p->newsCom = newscom[i];
      p->next = malloc(sizeof(NewsCom));
      pnext = p->next;
      p = pnext;      
   }
   p = NULL;
}

