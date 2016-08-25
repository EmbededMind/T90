#include "T90.h"
#include "bully.h"
#include "string.h"
#include "snap.h"



#define MAP_SIZE


typedef struct  _NATION_MAP_ITEM NATION_MAP_ITEM;
struct _NATION_MAP_ITEM
{
   short orgCode;
   unsigned char expCnt;
   unsigned char nation;
};


const NATION_MAP_ITEM NationMap[NATION_NUM] = {
                                        {416, 0, NATION_CTB},
                                        {431, 1, NATION_JPN},                                        
                                        {440, 1, NATION_KOR},
                                        {445, 0, NATION_PRK},
                                        {525, 0, NATION_INA},
                                        {574, 0, NATION_VIE}                                        
                                      };


BULY_BERTH* pBulyHeader  = NULL;
BULY_BERTH* pPlayBerth  = NULL;
int  validCnt  = 0;

BULY_BERTH BULY_Berth[BULY_NUM_MAX];




static BULY_BERTH* BULY_alloc(void)
{
   int i  = 0;
   for(i=0; i<BULY_NUM_MAX; i++)
   {
      if(BULY_Berth[i].pBoatLink == NULL)
      {
         return (&BULY_Berth[i]);
      }
   }
   return NULL;
}



Bool BULY_add(BERTH* pBoatLink)
{
   BULY_BERTH * pBuf;
   BULY_BERTH * pIterator;
   
   //   pBuf  = BULY_alloc();

/// Modified by SealedGhost at 5/12/2016   
   int i  = 0;
   for(i=0; i<BULY_NUM_MAX; i++){
      if(BULY_Berth[i].pBoatLink == pBoatLink){
         pBuf  = &BULY_Berth[i];
         pBuf->pBoatLink  = pBoatLink;
         if(pBuf->pBoatLink->mntState != MNTState_Triggered){
            validCnt++;
            pBuf->pBoatLink->mntState  = MNTState_Triggered;
         }
PRINT("readd bully");         
        return TRUE;
      }
   }
   
   if(i>=BULY_NUM_MAX)
      pBuf  = BULY_alloc();
/// Modified at 5/12/2016 end.

   if(pBuf)
   {
      pBuf->pBoatLink  = pBoatLink;
      pBuf->pBoatLink->mntState  = MNTState_Triggered;
      if(pBulyHeader)
      {
         pIterator  = pBulyHeader;
         while(pIterator->pNext)
         {
            pIterator  = pIterator->pNext;
         }
         pIterator->pNext  = pBuf;
         validCnt++;	 
         return TRUE;
      }
      else 
      {
         pBulyHeader  = pBuf;
         validCnt++;		 				
         return TRUE;
      }
   }
   else
   {
      return FALSE; 
   }
}




void BULY_delete(BERTH* addr)
{
   BULY_BERTH* pBC;
   BULY_BERTH* pCursor;
//printf("BULY_delete begin\n"); 
	if(pBulyHeader)
   {
      pBC  = pBulyHeader;  
      if(pBulyHeader->pBoatLink == addr)
      {         
         pBulyHeader  = pBulyHeader->pNext;
         validCnt--;
         pBC->pBoatLink = NULL;
         pBC->pNext = NULL;
         
//         memset(pBC, NULL, sizeof(BULY_BERTH));
         return;
      }
      else
      {
         pCursor  = pBulyHeader->pNext;
         while(pCursor)
         {
            if(pCursor->pBoatLink == addr)
            {
               pBC->pNext  = pCursor->pNext;
               validCnt--;
               pCursor->pBoatLink = NULL;
               pCursor->pNext = NULL;               
               return ;
            }

               pBC  = pCursor;
               pCursor  = pCursor->pNext;

         }
      }
   }
//printf("BULY_delete end\n");
}


//__INLINE BULY_BERTH* BULY_fetchPlayBerth(void)
//{
//   return pPlayBerth;
//}

int BULY_getValidNumber(void)
{
   return validCnt;
}


void BULY_maskAllBerth(void )
{
   BULY_BERTH* pBuly  = pBulyHeader;
   while(pBuly)
   {
      if(pBuly->pBoatLink->mntState == MNTState_Triggered)
      {
         pBuly->pBoatLink->mntState  = MNTState_Masked;
      }
      pBuly  = pBuly->pNext;
   }
}


BULY_BERTH* BULY_fetchNextPlayBerth(void)
{
   BULY_BERTH* pIterator;
   
   if(pPlayBerth == NULL)
   {
      pIterator  = pBulyHeader;
      while(pIterator)
      {
         if(pIterator->pBoatLink->mntState == MNTState_Triggered )
         {
            pPlayBerth  = pIterator;
					       SNAP_SetSnapLink(pPlayBerth->pBoatLink);
            return pPlayBerth;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      pPlayBerth  = NULL;
      return NULL;
   }
   else
   {
      pIterator  = pPlayBerth->pNext;
      
      while(pIterator)
      {
         if(pIterator->pBoatLink->mntState == MNTState_Triggered)
         {
            pPlayBerth  = pIterator;
					       SNAP_SetSnapLink(pPlayBerth->pBoatLink);
            return pPlayBerth;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      pIterator = pBulyHeader;
      while(pIterator != pPlayBerth->pNext)
      {
         if(pIterator->pBoatLink->mntState == MNTState_Triggered)
             {
               pPlayBerth  = pIterator;
               SNAP_SetSnapLink(pPlayBerth->pBoatLink);
               return pPlayBerth;
             }
         pIterator = pIterator->pNext;
      }
      pPlayBerth  = NULL;
      return NULL;
   }
}



unsigned char BULY_parseNation(long id)
{
   int i  = 0;
   int tmp  = id /1000000; 

   if(tmp < NationMap[0].orgCode  ||  tmp >= NationMap[NATION_NUM-1].orgCode+NationMap[NATION_NUM-1].expCnt)
   {
      return 0;
   }

   for(; i<NATION_NUM; i++){
      if( tmp >= NationMap[i].orgCode  &&  tmp <= (NationMap[i].orgCode+NationMap[i].expCnt) ) 
      {
         if( (0x01<<i) &t90_set.alarm.nation){
            return NationMap[i].nation;
         }
         else{
            return 0;
         }
      }
   }
   return 0;
}


void BULY_dump(void)
{
   BULY_BERTH* pIterator  = pBulyHeader;
   while(pIterator)
   {
//      printf("mmsi:%09ld--sog:%d--dist:%d--category:0x%x\n", pIterator->pBoatLink->Boat.user_id, pIterator->pBoatLink->Boat.SOG, pIterator->pBoatLink->Boat.dist, pIterator->pBoatLink->Boat.category);
     printf("mmsi:%09ld - mntstate:%d\r\n", pIterator->pBoatLink->Boat.user_id, pIterator->pBoatLink->mntState); 
		 pIterator  = pIterator->pNext;
   }
}


void BULY_print(BULY_BERTH* pBulyBerth)
{
   printf("mmsi:%09ld--sog:%d--dist:%d\n", pBulyBerth->pBoatLink->Boat.user_id, pBulyBerth->pBoatLink->Boat.SOG, pBulyBerth->pBoatLink->Boat.dist );
}

