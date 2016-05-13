#include "snap.h"
#include "bully.h"


extern boat mothership;
extern int N_boat;
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];

BERTH *  pSnapLink  = (BERTH*)(&mothership);
//char   snapType   = 0;


static long checkMMSI  = 0;

long SNAP_getSnapObjMMSI(void)
{
   return checkMMSI;
}


void SNAP_reset(void)
{
   pSnapLink  = (BERTH*)(&mothership);
	 pSnapLink->x_to_cross = 0;
	 pSnapLink->y_to_cross = 0;
//   snapType   = 0;
//   checkMMSI  = 0;
}

void SNAP_Refresh(void)
{
	if(pSnapLink == NULL)
	{
		SNAP_reset();
		return;
	}

	if(!BULY_getValidNumber() && pSnapLink->Boat.user_id == 0)
	{
		SNAP_reset();
	}
}

void SNAP_searchNearestObj(int Dir_x, int Dir_y)
{
	long minDiff = 99999;
	long ref_x = pSnapLink->x_to_cross;
	long ref_y = pSnapLink->y_to_cross;
	
	BERTH * potentialSnapLink = NULL;
	BULY_BERTH * pBuly = NULL;
	
	long refMMSI = pSnapLink->Boat.user_id;
	
	int i;
		 if(Dir_x == -1)
     {
        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].pBerth->x_to_cross < ref_x  &&  ref_x - SimpBerthes[i].pBerth->x_to_cross < minDiff)
                 {
                    minDiff  = ref_x - SimpBerthes[i].pBerth->x_to_cross;
                    potentialSnapLink  = SimpBerthes[i].pBerth;                  
                 }
              }
           }
        }
        
					 if(BULY_getValidNumber() > 0)
					 {
							pBuly  = pBulyHeader;
							while(pBuly){
								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
											 if(    pBuly->pBoatLink->x_to_cross  
													 && pBuly->pBoatLink->x_to_cross < ref_x  
													 && ref_x -pBuly->pBoatLink->x_to_cross < minDiff )
											 {
													minDiff  = ref_x -pBuly->pBoatLink->x_to_cross;
													potentialSnapLink  = pBuly->pBoatLink;
											 }
								 }
								 
								 pBuly  = pBuly->pNext;
							}
					 }
     
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
        }    
        return ;
     }
		 
     else if(Dir_x == 1)  
     {
        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].pBerth->x_to_cross > ref_x  &&  SimpBerthes[i].pBerth->x_to_cross - ref_x < minDiff)
                 {
                    minDiff  = SimpBerthes[i].pBerth->x_to_cross - ref_x;
                    potentialSnapLink  = SimpBerthes[i].pBerth;             
                 }
              }
           }
        }

					 if(BULY_getValidNumber() > 0)
					 {
							pBuly  = pBulyHeader;
							while(pBuly){
								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
											 if(    pBuly->pBoatLink->x_to_cross  
													 && pBuly->pBoatLink->x_to_cross > ref_x  
													 && pBuly->pBoatLink->x_to_cross -ref_x < minDiff )
											 {
													minDiff  = pBuly->pBoatLink->x_to_cross -ref_x;
													potentialSnapLink  = pBuly->pBoatLink;
											 }
								 }
								 
								 pBuly  = pBuly->pNext;
							}
					 }

        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;      
        }      
        return ;
     }
		 
     else if(Dir_y == -1)
     {

        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].pBerth->y_to_cross < ref_y  &&  ref_y - SimpBerthes[i].pBerth->y_to_cross < minDiff)
                 {
                    minDiff  = ref_y - SimpBerthes[i].pBerth->y_to_cross;
                    potentialSnapLink  = SimpBerthes[i].pBerth;
                 }
              }
           }
        }

					 if(BULY_getValidNumber() > 0)
					 {
							pBuly  = pBulyHeader;
							while(pBuly){
								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
											 if(    pBuly->pBoatLink->y_to_cross  
													 && pBuly->pBoatLink->y_to_cross < ref_y 
													 && ref_y -pBuly->pBoatLink->y_to_cross < minDiff )
											 {
													minDiff  = ref_y -pBuly->pBoatLink->y_to_cross;
													potentialSnapLink  = pBuly->pBoatLink;
											 }
								 }
								 
								 pBuly  = pBuly->pNext;
							}
					 }
         
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;        
        }
        
        return ;
     } 
		 
     else if(Dir_y == 1)   
     {

        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].pBerth->y_to_cross > ref_y  &&  SimpBerthes[i].pBerth->y_to_cross - ref_y < minDiff)
                 {
                    minDiff  = SimpBerthes[i].pBerth->y_to_cross - ref_y;
                    potentialSnapLink  = SimpBerthes[i].pBerth;
                 }
              }
           }
        }

					 if(BULY_getValidNumber() > 0)
					 {
							pBuly  = pBulyHeader;
							while(pBuly){
								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
											 if(    pBuly->pBoatLink->y_to_cross  
													 && pBuly->pBoatLink->y_to_cross > ref_y 
													 && pBuly->pBoatLink->y_to_cross -ref_y< minDiff )
											 {
													minDiff  = pBuly->pBoatLink->y_to_cross -ref_y;
													potentialSnapLink  = pBuly->pBoatLink;
											 }
								 }
								 
								 pBuly  = pBuly->pNext;
							}
					 }
        
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;         
        }
        
        return ;
     }     
}

//void SNAP_getPara(int *x, int *y)
//{
//   switch(snapType)
//   {
//      case 1:
//           if( ((BERTH*)pSnapLink)->Boat.user_id == checkMMSI)
//           {
//              *x = ((BERTH*)pSnapLink)->x_to_cross;
//              *y = ((BERTH*)pSnapLink)->y_to_cross;
//           }
//           else
//           {
//              *x  = 0;
//              *y  = 0;
//           }
//           break;
//     default:
//           *x  = 0;
//           *y  = 0;
//           break;
//   }
//}


//void SNAP_searchNearestObj(int Dir_x, int Dir_y)
//{
//     long  minDiff  = 99999;
//     long  ref_x    = 0;
//     long  ref_y    = 0;
//     long  refMMSI  = 0;
//     
//     void* potentialSnapLink = NULL;
//     char  potentialSnapType = 0;
//     
//     BULY_BERTH* pBuly  = NULL;  
//	
//     int   i;
//     
//     switch(snapType)
//     {
//        /// Previous link is berth
//        case 1:
//             refMMSI  = pSnapLink->Boat.user_id;
////             if(refMMSI == checkMMSI)
////             {
//                ref_x  = ((BERTH*)pSnapLink)->x_to_cross;
//                ref_y  = ((BERTH*)pSnapLink)->y_to_cross;
////             }
////             else
////             {
////                ref_x  = 0;
////                ref_y  = 0;
////                refMMSI = 0;
////             }
//             break;
//             
//       default:
//             ref_x  = 0;
//             ref_y  = 0;
//             refMMSI  = 0;
//             checkMMSI = 0;
////printf("ref snap:mothership\n\r");             
//             break;       
//     }
//     
//     
//     if(Dir_x == -1)
//     {
//        for(i=0; i<N_boat; i++)
//        {
//           if(SimpBerthes[i].pBerth->isInvader)
//           {
//              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
//              {
//                 if(SimpBerthes[i].pBerth->x_to_cross < ref_x  &&  ref_x - SimpBerthes[i].pBerth->x_to_cross < minDiff)
//                 {
//                    minDiff  = ref_x - SimpBerthes[i].pBerth->x_to_cross;
//                    potentialSnapLink  = SimpBerthes[i].pBerth;
//                    potentialSnapType  = 1;                   
//                 }
//              }
//           }
//        }
//        
////				if(monitorState == ON)
////				{
//					 if(BULY_getValidNumber() > 0)
//					 {
//							pBuly  = pBulyHeader;
//							while(pBuly){
//								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
//		//                if(pBuly->pBoatLink->mntState == MNTState_Triggered){
//											 if(    pBuly->pBoatLink->x_to_cross  
//													 && pBuly->pBoatLink->x_to_cross < ref_x  
//													 && ref_x -pBuly->pBoatLink->x_to_cross < minDiff )
//											 {
//													minDiff  = ref_x -pBuly->pBoatLink->x_to_cross;
//													potentialSnapLink  = pBuly->pBoatLink;
//													potentialSnapType  = 1;
//											 }
//		//                }
//								 }
//								 
//								 pBuly  = pBuly->pNext;
//							}
//					 }
////				 }
//     
//        if(potentialSnapLink)
//        {
//           pSnapLink  = potentialSnapLink;
//           snapType  = potentialSnapType;
//           
//           switch(snapType)
//           {
//              case 1:
//                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
//                   break;

//             default:
//                   checkMMSI  = 0;
//                   break;
//           }
//        }    
//        return ;
//     }
//		 
//     else if(Dir_x == 1)  
//     {
//        for(i=0; i<N_boat; i++)
//        {
//           if(SimpBerthes[i].pBerth->isInvader)
//           {
//              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
//              {
//                 if(SimpBerthes[i].pBerth->x_to_cross > ref_x  &&  SimpBerthes[i].pBerth->x_to_cross - ref_x < minDiff)
//                 {
//                    minDiff  = SimpBerthes[i].pBerth->x_to_cross - ref_x;
//                    potentialSnapLink  = SimpBerthes[i].pBerth;
//                    potentialSnapType  = 1;               
//                 }
//              }
//           }
//        }

////				if(monitorState == ON)
////				{
//					 if(BULY_getValidNumber() > 0)
//					 {
//							pBuly  = pBulyHeader;
//							while(pBuly){
//								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
//		//                if(pBuly->pBoatLink->mntState == MNTState_Triggered){
//											 if(    pBuly->pBoatLink->x_to_cross  
//													 && pBuly->pBoatLink->x_to_cross > ref_x  
//													 && pBuly->pBoatLink->x_to_cross -ref_x < minDiff )
//											 {
//													minDiff  = pBuly->pBoatLink->x_to_cross -ref_x;
//													potentialSnapLink  = pBuly->pBoatLink;
//													potentialSnapType  = 1;
//											 }
//		//                }
//								 }
//								 
//								 pBuly  = pBuly->pNext;
//							}
//					 }
////				 }
//        
//        if(potentialSnapLink)
//        {
//           pSnapLink  = potentialSnapLink;
//           snapType  = potentialSnapType;
//           switch(snapType)
//           {
//              case 1:
//                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
//                   break;

//             default:
//                   checkMMSI  = 0;
//                   break;
//           }           
//        }      
//        return ;
//     }
//		 
//     else if(Dir_y == -1)
//     {

//        for(i=0; i<N_boat; i++)
//        {
//           if(SimpBerthes[i].pBerth->isInvader)
//           {
//              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
//              {
//                 if(SimpBerthes[i].pBerth->y_to_cross < ref_y  &&  ref_y - SimpBerthes[i].pBerth->y_to_cross < minDiff)
//                 {
//                    minDiff  = ref_y - SimpBerthes[i].pBerth->y_to_cross;
//                    potentialSnapLink  = SimpBerthes[i].pBerth;
//                    potentialSnapType  = 1;
//                 }
//              }
//           }
//        }

////				if(monitorState == ON)
////				{
//					 if(BULY_getValidNumber() > 0)
//					 {
//							pBuly  = pBulyHeader;
//							while(pBuly){
//								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
//		//                if(pBuly->pBoatLink->mntState == MNTState_Triggered){
//											 if(    pBuly->pBoatLink->y_to_cross  
//													 && pBuly->pBoatLink->y_to_cross < ref_y 
//													 && ref_y -pBuly->pBoatLink->y_to_cross < minDiff )
//											 {
//													minDiff  = ref_y -pBuly->pBoatLink->y_to_cross;
//													potentialSnapLink  = pBuly->pBoatLink;
//													potentialSnapType  = 1;
//											 }
//		//                }
//								 }
//								 
//								 pBuly  = pBuly->pNext;
//							}
//					 }
////				 }
//         
//        if(potentialSnapLink)
//        {
//           pSnapLink  = potentialSnapLink;
//           snapType  = potentialSnapType;
//           switch(snapType)
//           {
//              case 1:
//                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
//                   break;

//             default:
//                   checkMMSI  = 0;
//                   break;
//           }           
//        }
//        
//        return ;
//     } 
//		 
//     else if(Dir_y == 1)   
//     {

//        for(i=0; i<N_boat; i++)
//        {
//           if(SimpBerthes[i].pBerth->isInvader)
//           {
//              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
//              {
//                 if(SimpBerthes[i].pBerth->y_to_cross > ref_y  &&  SimpBerthes[i].pBerth->y_to_cross - ref_y < minDiff)
//                 {
//                    minDiff  = SimpBerthes[i].pBerth->y_to_cross - ref_y;
//                    potentialSnapLink  = SimpBerthes[i].pBerth;
//                    potentialSnapType  = 1;
//                 }
//              }
//           }
//        }

////				if(monitorState == ON)
////				{
//					 if(BULY_getValidNumber() > 0)
//					 {
//							pBuly  = pBulyHeader;
//							while(pBuly){
//								 if(pBuly->pBoatLink->Boat.user_id != refMMSI){
//		//                if(pBuly->pBoatLink->mntState == MNTState_Triggered){
//											 if(    pBuly->pBoatLink->y_to_cross  
//													 && pBuly->pBoatLink->y_to_cross > ref_y 
//													 && pBuly->pBoatLink->y_to_cross -ref_y< minDiff )
//											 {
//													minDiff  = pBuly->pBoatLink->y_to_cross -ref_y;
//													potentialSnapLink  = pBuly->pBoatLink;
//													potentialSnapType  = 1;
//											 }
//		//                }
//								 }
//								 
//								 pBuly  = pBuly->pNext;
//							}
//					 }
////				 }
//        
//        if(potentialSnapLink)
//        {
//           pSnapLink  = potentialSnapLink;
//           snapType  = potentialSnapType;
//           switch(snapType)
//           {
//              case 1:
//                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
//                   break;

//             default:
//                   checkMMSI  = 0;
//                   break;
//           }           
//        }
//        
//        return ;
//     }     
//}
