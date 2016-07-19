#include "Config.h"
#include "WM.h"
#include "BUTTON.h"
#include "MainTask.h"
#include "dlg.h"
#include "T90.h"
#include "t90font.h"
#include "layout_dst_set.h"
#include "HSD_DIMENSION_EX.h"
#include "stub.h"
#include "lpc177x_8x_uart.h"
#include "comm.h"

#define ID_EX_DIM_0      (GUI_ID_USER + 0x10)
#define ID_EX_DIM_1      (GUI_ID_USER + 0x11)
#define ID_EX_DIM_2      (GUI_ID_USER + 0x12)  
#define ID_EX_DIM_3      (GUI_ID_USER + 0x13)
#define ID_EX_DIM_4      (GUI_ID_USER + 0x14)

#define ID_SFig_0          (GUI_ID_USER + 0x20)
#define ID_SFig_1          (GUI_ID_USER + 0x21)
#define ID_SFig_2          (GUI_ID_USER + 0x22)
#define MAXMOTOAS         3000
#define MAXMOTOSTUB       3000
#define MAXSTUBTOSTUB     3000

WM_HWIN doubleShipDstSetWin;

static const GUI_RECT drawArea  = {50, 50, DST_SET_WIDTH-50, DST_SET_HEIGHT-50};
static const GUI_RECT tipStrArea = {50, DST_SET_HEIGHT-50 +2 , DST_SET_WIDTH -50, DST_SET_HEIGHT -50 +32};

static const SetWinColor* pColors  = setWinColors;

static uint8_t whichFig = 0;
static uint8_t prevwhichFig = 0;

static WM_HWIN hExDim[5];
static WM_HWIN hFigs[3];



static DoubleDst_SetOne tempDouDstSet[3];
static DoubleDst_SetOne preDouDstSet[3];



static void myDimCallback(WM_MESSAGE* pMsg)
{
   int i = 0;   
   int id  = 0;
   WM_MESSAGE myMsg;
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key)
           {
              case GUI_KEY_UP:
                   id  = WM_GetId(pMsg->hWin) - ID_EX_DIM_0;
                   switch(id){
                      case 0:                          
                          if(tempDouDstSet[whichFig].motoas < MAXMOTOAS){
                             tempDouDstSet[whichFig].motoas += 50;
                             tempDouDstSet[whichFig].motoas -= (tempDouDstSet[whichFig].motoas %50);
                             sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motoas);
                             HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);
                              }
                          
                      break;
                      case 1:
                           if(tempDouDstSet[whichFig].stubtostub < MAXSTUBTOSTUB){
                              tempDouDstSet[whichFig].stubtostub += 50;
                              tempDouDstSet[whichFig].stubtostub -= (tempDouDstSet[whichFig].stubtostub %50);
                              sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].stubtostub);
                              HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);                         
                              }
                      break;
                      case 2:
                            if(tempDouDstSet[whichFig].motostub < MAXMOTOSTUB){
                              tempDouDstSet[whichFig].motostub  += 50;
                              tempDouDstSet[whichFig].motostub  -= (tempDouDstSet[whichFig].motostub %50);
                              sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motostub);
                              HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);                         
                            }
                      break;
                      case 3:
                             if(tempDouDstSet[whichFig].motostub < MAXMOTOSTUB){
                                tempDouDstSet[whichFig].motostub += 50;
                                tempDouDstSet[whichFig].motostub -= (tempDouDstSet [whichFig].motostub % 50);
                                sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motostub);
                                HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);
                             }
                      break;
                      case 4:
                             if(tempDouDstSet[whichFig].motostub < MAXMOTOSTUB){
                                tempDouDstSet[whichFig].motostub += 50;
                                tempDouDstSet[whichFig].motostub -= (tempDouDstSet [whichFig].motostub % 50);
                                sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motostub);
                                HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);
                             }
                     break;
                     }
                     break;
              case GUI_KEY_DOWN:
                   id  = WM_GetId(pMsg->hWin) - ID_EX_DIM_0;
                   switch(id){
                      case 0:                          
                          if(tempDouDstSet[whichFig].motoas > 50){
                              tempDouDstSet[whichFig].motoas  -= 50;
                              tempDouDstSet[whichFig].motoas  -= (tempDouDstSet[whichFig].motoas %50);
                              sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motoas);
                              HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);                         
                              }
                      break;
                      case 1:
                           if(tempDouDstSet[whichFig].stubtostub > 50){
                              tempDouDstSet[whichFig].stubtostub -= 50;
                              tempDouDstSet[whichFig].stubtostub -= (tempDouDstSet[whichFig].stubtostub %50);
                              sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].stubtostub);
                              HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);                         
                              }
                      break;
                      case 2:
                            if(tempDouDstSet[whichFig].motostub > 50){
                              tempDouDstSet[whichFig].motostub  -= 50;
                              tempDouDstSet[whichFig].motostub  -= (tempDouDstSet[whichFig].motostub %50);
                              sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motostub);
                              HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);                         
                            }
                      break;
                      case 3:
                             if(tempDouDstSet[whichFig].motostub > 0){
                                tempDouDstSet[whichFig].motostub -= 50;
                                tempDouDstSet[whichFig].motostub -= (tempDouDstSet [whichFig].motostub % 50);
                                sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motostub);
                                HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);
                             }
                      break;
                      case 4:
                             if(tempDouDstSet[whichFig].motostub > 0){
                                tempDouDstSet[whichFig].motostub -= 50;
                                tempDouDstSet[whichFig].motostub -= (tempDouDstSet [whichFig].motostub % 50);
                                sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motostub);
                                HSD_DIMENSION_EX_SetValText(hExDim[id], pStrBuf);
                             }
                   break;
                   }                   
                   break;
              
              case GUI_KEY_LEFT:
              case GUI_KEY_RIGHT:
                   id  = WM_GetId(pMsg->hWin) - ID_EX_DIM_0;
                   switch(id)
                   {
                      case 0: 
                           if(whichFig == 0){
                              WM_SetFocus(hExDim[2]);
                           }
                           else if(whichFig == 1){
                              WM_SetFocus(hExDim[3]);
                           }
                           else{
                              WM_SetFocus(hExDim[4]);
                           }
                           break;
                      case 1:
                           if(whichFig != 1){
                              WM_SetFocus(hExDim[0]);
                           }
                           break;
                      case 2:
                           WM_SetFocus(hExDim[1]);
                           break;
                      case 3:
                           WM_SetFocus(hExDim[0]);
                           break;
                      case 4:
                           WM_SetFocus(hExDim[1]);
                           break;
                   }
                   break;
              case GUI_KEY_BACKSPACE:
                   if(Mem_isEqual(&t90_set.doubledst_set, tempDouDstSet, sizeof(tempDouDstSet)))
                   {
                       WM_SetFocus(dstSetMenuDlg);
                   }
                   else
                   {
                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
                      myMsg.hWinSrc  = doubleShipDstSetWin;
                      myMsg.MsgId  = USER_MSG_CHOOSE;
                      myMsg.Data.v  = SYS_SETTING;
                      WM_SendMessage(myMsg.hWin, &myMsg);
                        
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(confirmWin);
                   }
//                   StubRefresh();
//                   WM_SetFocus(dstSetMenuDlg);
                   break;
           }
           break;     
      default:
           HSD_DIMENSION_EX_Callback(pMsg);
           break;      
   }
}

/** @brief Ë«ÍÐ¾àÀëÉèÖÃ´°¿ÚµÄ»Øµ÷
 *
 *  @param [in] pMsg  ÏûÏ¢Ö¸Õë
 */
static void myWindowcallback(WM_MESSAGE * pMsg)
{
   GUI_POINT arrows[6];
   int i;
   
   switch(pMsg->MsgId){
      
      case USER_MSG_DATA_ACK_RESULT:

         if(pMsg->Data.v)
         {

            memcpy(&t90_set.doubledst_set,tempDouDstSet,sizeof(tempDouDstSet));
             sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motoas);
             HSD_DIMENSION_EX_SetValText(hExDim[0], pStrBuf);
            if(whichFig==3)
             {              
               sprintf(pStrBuf, "%d", tempDouDstSet[prevwhichFig].stubtostub);
               HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
             }
             else
             {
               sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].stubtostub);
               HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
             }
             sprintf(pStrBuf, "%d", tempDouDstSet[0].motostub);
             HSD_DIMENSION_EX_SetValText(hExDim[2], pStrBuf);
             sprintf(pStrBuf, "%d", tempDouDstSet[1].motostub);
             HSD_DIMENSION_EX_SetValText(hExDim[3], pStrBuf);
             sprintf(pStrBuf, "%d", tempDouDstSet[2].motostub);
             HSD_DIMENSION_EX_SetValText(hExDim[4], pStrBuf);
             T90_Store();
             StubRefresh();             
         }
         else
         {

             memcpy(tempDouDstSet,&t90_set.doubledst_set,sizeof(tempDouDstSet));
                                     
         }     
         break;
      case USER_MSG_REPLY:
           if(pMsg->Data.v == REPLY_OK)
           {
              StubRefresh();
              Comm_addFrame(whichFig+1,stubs[whichFig+1].basePoint.x*MILLINM_TO_M,abs(stubs[whichFig+1].basePoint.y*MILLINM_TO_M));
              
              memcpy(preDouDstSet, &t90_set.doubledst_set, sizeof(preDouDstSet));
              sprintf(pStrBuf, "%d", preDouDstSet[whichFig].motoas);
              HSD_DIMENSION_EX_SetValText(hExDim[0], pStrBuf);
              if(whichFig==3)
              {              
                 sprintf(pStrBuf, "%d", preDouDstSet[prevwhichFig].stubtostub);
                 HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
              }
              else
              {
                 sprintf(pStrBuf, "%d", preDouDstSet[whichFig].stubtostub);
                 HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
              }
              sprintf(pStrBuf, "%d", preDouDstSet[0].motostub);
              HSD_DIMENSION_EX_SetValText(hExDim[2], pStrBuf);
              sprintf(pStrBuf, "%d", preDouDstSet[1].motostub);
              HSD_DIMENSION_EX_SetValText(hExDim[3], pStrBuf);
              sprintf(pStrBuf, "%d", preDouDstSet[2].motostub);
              HSD_DIMENSION_EX_SetValText(hExDim[4], pStrBuf);
           }
           else
           {
              memcpy(tempDouDstSet,&t90_set.doubledst_set,sizeof(tempDouDstSet));
      
              sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motoas);
              HSD_DIMENSION_EX_SetValText(hExDim[0], pStrBuf);
              if(whichFig==3)
              {              
                 sprintf(pStrBuf, "%d", tempDouDstSet[prevwhichFig].stubtostub);
                 HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
              }
              else
              {
                 sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].stubtostub);
                 HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
              }
              sprintf(pStrBuf, "%d", tempDouDstSet[0].motostub);
              HSD_DIMENSION_EX_SetValText(hExDim[2], pStrBuf);
              sprintf(pStrBuf, "%d", tempDouDstSet[1].motostub);
              HSD_DIMENSION_EX_SetValText(hExDim[3], pStrBuf);
              sprintf(pStrBuf, "%d", tempDouDstSet[2].motostub);
              HSD_DIMENSION_EX_SetValText(hExDim[4], pStrBuf);                           
           }
           
           WM_SetFocus(dstSetMenuDlg);
      break;
      case USER_MSG_DST_SET:
       
          for(i = 0; i < 3; i++)  //clear
          {
             HSD_STICKFIGURE_SetPenColor(hFigs[i], HSD_STICKFIGURE_CI_UNFOCUS, pColors->arrowLineColor);
          }
          if(pMsg->Data.v != -1)
          {
             HSD_STICKFIGURE_SetPenColor(hFigs[pMsg->Data.v], HSD_STICKFIGURE_CI_UNFOCUS, pColors->focusBkColor);
             prevwhichFig = whichFig;
             whichFig = pMsg->Data.v;
             sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].motoas);
             HSD_DIMENSION_EX_SetValText(hExDim[0], pStrBuf);
             if(whichFig != 1)
             {
                sprintf(pStrBuf, "%d", tempDouDstSet[whichFig].stubtostub);
                HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);  
             }                
          }
          break;
      case USER_MSG_SKIN:
       pColors = &(setWinColors[pMsg->Data.v]);	
       
       for(i = 0; i < 5; i++)
       {
          HSD_DIMENSION_EX_SetBkColor(hExDim[i], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
          HSD_DIMENSION_EX_SetBkColor(hExDim[i], HSD_DIMENSION_EX_CI_FOCUS,  pColors->bkColor);
          
          HSD_DIMENSION_EX_SetValColor(hExDim[i], HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
          HSD_DIMENSION_EX_SetValColor(hExDim[i], HSD_DIMENSION_EX_CI_FOCUS,   pColors->focusBkColor);
          
          HSD_DIMENSION_EX_SetArrowLineColor(hExDim[i],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
          HSD_DIMENSION_EX_SetArrowLineColor(hExDim[i], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
       }
       break;
      case USER_MSG_FOCUS:
           if(pMsg->Data.v < 3){
              whichFig  = pMsg->Data.v;
              switch(whichFig)
              {
                 case 0:WM_SetFocus(hExDim[2]); break;
                 case 1:WM_SetFocus(hExDim[0]); break;
                 case 2:WM_SetFocus(hExDim[4]); break;
              }
              
              HSD_STICKFIGURE_SetPenColor(hFigs[whichFig], HSD_STICKFIGURE_CI_UNFOCUS, pColors->focusBkColor);
           }
           break;
   
      case WM_CREATE:


           memcpy(tempDouDstSet,&t90_set.doubledst_set,sizeof(tempDouDstSet));
           
           pColors  = &setWinColors[t90_set.sys.nightmode];
           
           hFigs[0]  = HSD_STICKFIGURE_CreateEx(144, 
                                                214,
                                                16, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_SFig_0);  
           HSD_STICKFIGURE_SetPenColor(hFigs[0], HSD_STICKFIGURE_CI_UNFOCUS, pColors->arrowLineColor);                                                
           HSD_STICKFIGURE_SetPenColor(hFigs[0], HSD_STICKFIGURE_CI_FOCUS,   pColors->focusArrowLineColor);
           WM_SetHasTrans(hFigs[0]);
           
           hFigs[1]  = HSD_STICKFIGURE_CreateEx( 197,
                                                 360,
                                                 16, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_SFig_1);
           HSD_STICKFIGURE_SetPenColor(hFigs[1], HSD_STICKFIGURE_CI_UNFOCUS, pColors->arrowLineColor);                                                
           HSD_STICKFIGURE_SetPenColor(hFigs[1], HSD_STICKFIGURE_CI_FOCUS,   pColors->focusArrowLineColor);
           WM_SetHasTrans(hFigs[1]);
           
           hFigs[2]  = HSD_STICKFIGURE_CreateEx( 249,
                                                 214,
                                                 16, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_SF_2 );
           HSD_STICKFIGURE_SetPenColor(hFigs[2], HSD_STICKFIGURE_CI_UNFOCUS, pColors->arrowLineColor);                                                
           HSD_STICKFIGURE_SetPenColor(hFigs[2], HSD_STICKFIGURE_CI_FOCUS,   pColors->focusArrowLineColor);
           WM_SetHasTrans(hFigs[2]);                                     
           
           hExDim[0]  = HSD_DIMENSION_EX_CreateEx(128,60, 153, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_0);   
           WM_SetCallback(hExDim[0], &myDimCallback);           
           HSD_DIMENSION_EX_SetBkColor(hExDim[0], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS,  pColors->bkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[0],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[0], HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS,   pColors->focusBkColor);
           arrows[0].x =  0;  arrows[1].x =  6; arrows[2].x =  6; arrows[3].x = 153; arrows[4].x = 147; arrows[5].x = 147; 
           arrows[0].y = 20;  arrows[1].y = 22; arrows[2].y = 18; arrows[3].y = 20;  arrows[4].y = 22;  arrows[5].y = 18;
           HSD_DIMENSION_EX_SetArrows(hExDim[0], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[0], &GUI_Font_T90_24);
           sprintf(pStrBuf,"%d",tempDouDstSet[whichFig].motoas);
           HSD_DIMENSION_EX_SetValText(hExDim[0],pStrBuf);
           HSD_DIMENSION_EX_SetUnitText(hExDim[0], "ç±³");
           WM_SetHasTrans(hExDim[0]);           
         
           hExDim[1]  = HSD_DIMENSION_EX_CreateEx(160, 206,88, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_1);         
           WM_SetCallback(hExDim[1], &myDimCallback);  
           HSD_DIMENSION_EX_SetBkColor(hExDim[1], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[1], HSD_DIMENSION_EX_CI_FOCUS,  pColors->bkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[1],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[1], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[1], HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[1], HSD_DIMENSION_EX_CI_FOCUS,   pColors->focusBkColor);
           arrows[0].x = 0;   arrows[1].x =  6; arrows[2].x =  6; arrows[3].x = 88; arrows[4].x = 82; arrows[5].x = 82; 
           arrows[0].y = 20;  arrows[1].y = 22; arrows[2].y = 18; arrows[3].y = 20; arrows[4].y = 22; arrows[5].y = 18;
           HSD_DIMENSION_EX_SetArrows(hExDim[1], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[1], &GUI_Font_T90_24);
           sprintf(pStrBuf,"%d",tempDouDstSet[whichFig == 2?prevwhichFig:whichFig].stubtostub);
           HSD_DIMENSION_EX_SetValText(hExDim[1], pStrBuf);
           HSD_DIMENSION_EX_SetUnitText(hExDim[1], "ç±³");
           WM_SetHasTrans(hExDim[1]);          
         
           hExDim[2]  = HSD_DIMENSION_EX_CreateEx(70, 120,68, 112, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_2);         
           WM_SetCallback(hExDim[2], &myDimCallback);  
           HSD_DIMENSION_EX_SetBkColor(hExDim[2], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[2], HSD_DIMENSION_EX_CI_FOCUS,  pColors->bkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[2],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[2], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[2], HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[2], HSD_DIMENSION_EX_CI_FOCUS,   pColors->focusBkColor);
           arrows[0].x = 14;  arrows[1].x =  17; arrows[2].x = 13; arrows[3].x =  52; arrows[4].x =  48; arrows[5].x =  53; 
           arrows[0].y =  1;  arrows[1].y =   5; arrows[2].y =  7; arrows[3].y = 113; arrows[4].y = 110; arrows[5].y = 108;
           HSD_DIMENSION_EX_SetArrows(hExDim[2], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[2], &GUI_Font_T90_24);
           sprintf(pStrBuf,"%d",tempDouDstSet[0].motostub);
           HSD_DIMENSION_EX_SetValText(hExDim[2], pStrBuf);
           HSD_DIMENSION_EX_SetUnitText(hExDim[2], "ç±³");
           WM_SetHasTrans(hExDim[2]);    

           hExDim[3]  = HSD_DIMENSION_EX_CreateEx(224, 125,120, 260, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_3);         
           WM_SetCallback(hExDim[3], &myDimCallback);  
           HSD_DIMENSION_EX_SetBkColor(hExDim[3], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[3], HSD_DIMENSION_EX_CI_FOCUS,  pColors->bkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[3],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[3], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[3], HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[3], HSD_DIMENSION_EX_CI_FOCUS,   pColors->focusBkColor);
           arrows[0].x = 110;  arrows[1].x = 105; arrows[2].x =  111; arrows[3].x =  10; arrows[4].x =  9; arrows[5].x = 14; 
           arrows[0].y =   1;  arrows[1].y =   5; arrows[2].y =    7; arrows[3].y = 258; arrows[4].y = 252; arrows[5].y = 254;
           HSD_DIMENSION_EX_SetArrows(hExDim[3], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[3], &GUI_Font_T90_24);
           sprintf(pStrBuf,"%d",tempDouDstSet[1].motostub);
           HSD_DIMENSION_EX_SetValText(hExDim[3], pStrBuf);
           HSD_DIMENSION_EX_SetUnitText(hExDim[3], "ç±³");
           WM_SetHasTrans(hExDim[3]);   

           hExDim[4]  = HSD_DIMENSION_EX_CreateEx(218, 106,75, 112, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_4);         
           WM_SetCallback(hExDim[4], &myDimCallback);  
           HSD_DIMENSION_EX_SetBkColor(hExDim[4], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[4], HSD_DIMENSION_EX_CI_FOCUS,  pColors->bkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[4],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[4], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[4], HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetValColor(hExDim[4], HSD_DIMENSION_EX_CI_FOCUS,   pColors->focusBkColor);
           arrows[0].x = 62;  arrows[1].x = 58; arrows[2].x =  63; arrows[3].x =  20; arrows[4].x =  19; arrows[5].x =  24; 
           arrows[0].y =  1;  arrows[1].y =  5; arrows[2].y =   7; arrows[3].y = 108; arrows[4].y = 102; arrows[5].y = 104;
           HSD_DIMENSION_EX_SetArrows(hExDim[4], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[4], &GUI_Font_T90_24);
           sprintf(pStrBuf,"%d",tempDouDstSet[2].motostub);
           HSD_DIMENSION_EX_SetValText(hExDim[4], pStrBuf);
           HSD_DIMENSION_EX_SetUnitText(hExDim[4], "ç±³");
           WM_SetHasTrans(hExDim[4]);           
           break;
           
     case WM_PAINT:
          {
             GUI_RECT r;
             int xPos_M; 
             int yPos_M;
             int xPos_A;
             int yPos_A;
             
             WM_GetClientRectEx(pMsg->hWin,&r);              
             GUI_SetBkColor(pColors->bkColor);
             GUI_ClearRectEx(&r);
             
             GUI_SetColor(pColors->textColor);
             xPos_M  = drawArea.x0 + (drawArea.x1 - drawArea.x0)/5;
             yPos_M  = drawArea.y0;
             GUI_DrawLine(xPos_M, yPos_M, xPos_M+16, yPos_M +16);
             GUI_DrawVLine(xPos_M+16, yPos_M+17, yPos_M+60);
             GUI_DrawHLine(yPos_M+60, xPos_M-15, xPos_M+15);
             GUI_DrawVLine(xPos_M-16, yPos_M+17, yPos_M+60);
             GUI_DrawLine(xPos_M-16, yPos_M+16, xPos_M, yPos_M);
             
             xPos_A  = drawArea.x1 - (drawArea.x1 - drawArea.x0)/5;
             yPos_A  = drawArea.y0;
             GUI_DrawLine(xPos_A, yPos_A, xPos_A+16, yPos_A +16);
             GUI_DrawVLine(xPos_A+16, yPos_A+17, yPos_A+60);
             GUI_DrawHLine(yPos_A+60, xPos_A-15, xPos_A+15);
             GUI_DrawVLine(xPos_A-16, yPos_A+17, yPos_A+60);
             GUI_DrawLine(xPos_A-16, yPos_A+16, xPos_A, yPos_A);
             
             GUI_SetLineStyle(GUI_LS_DOT);
            
             GUI_DrawLine(xPos_M, yPos_M+61, (xPos_M+xPos_A)/2, drawArea.y1-50);
             
             GUI_DrawLine(xPos_A, yPos_A+61, (xPos_M+xPos_A)/2, drawArea.y1-50);
             
             GUI_DrawLine(297, 111, 336, 125);
             GUI_DrawLine(204, 372, 243, 386);
             
             GUI_DrawLine(111, 111,  72, 125);
             GUI_DrawLine(149, 222, 110, 236);
             
             GUI_DrawLine(297, 111, 269, 101);
             GUI_DrawLine(254, 222, 226, 212);
             GUI_DrawRectEx(&drawArea);
          }
          break;
           
     default:
           WM_DefaultProc(pMsg);
           break;
   }

}

/** Ë«ÍÐ°²È«±ê¾àÀëÉèÖÃ´°¿Ú´´½¨
 *
 *
 */
WM_HWIN  WIN_doubleShipDstSetCreate(void)
{
   WM_HWIN handle;
   
   handle  = WM_CreateWindow(DST_SET_X, DST_SET_Y, DST_SET_WIDTH, DST_SET_HEIGHT,
                             WM_CF_SHOW, myWindowcallback,  0);
}




 

