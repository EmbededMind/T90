#include "Config.h"
#include "WM.h"
#include "BUTTON.h"
#include "MainTask.h"
#include "dlg.h"
#include "T90.h"
#include "t90font.h"
#include "layout_dst_set.h"
#include "HSD_DIMENSION_EX.h"

#define ID_EX_DIM_0      (GUI_ID_USER + 0x10)
#define ID_EX_DIM_1      (GUI_ID_USER + 0x11)
#define ID_EX_DIM_2      (GUI_ID_USER + 0x12)  
#define ID_EX_DIM_3      (GUI_ID_USER + 0x13)
#define ID_EX_DIM_4      (GUI_ID_USER + 0x14)

#define ID_SFig_0          (GUI_ID_USER + 0x20)
#define ID_SFig_1          (GUI_ID_USER + 0x21)
#define ID_SFig_2          (GUI_ID_USER + 0x22)


WM_HWIN doubleShipDstSetWin;


static DouDstSet agentDouDstSet;
static DouDstSet dummyDouDstSet;

static const GUI_RECT drawArea  = {50, 50, DST_SET_WIDTH-50, DST_SET_HEIGHT-50};
static const GUI_RECT tipStrArea = {50, DST_SET_HEIGHT-50 +2 , DST_SET_WIDTH -50, DST_SET_HEIGHT -50 +32};

static const SetWinColor* pColors  = setWinColors;

static uint8_t whichFig = 0;

static WM_HWIN hExDim[5];
static WM_HWIN hFigs[3];


static void myDimCallback(WM_MESSAGE* pMsg)
{
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key)
           {
              case GUI_KEY_UP:
                   
                   break;
              case GUI_KEY_DOWN:
                   
                   break;
           }
           break;
   }
}



/** @brief ˫�о������ô��ڵĻص�
 *
 *  @param [in] pMsg  ��Ϣָ��
 */
static void myWindowcallback(WM_MESSAGE * pMsg)
{
   GUI_POINT arrows[6];

   
   switch(pMsg->MsgId){
      case USER_MSG_FOCUS:
           if(pMsg->Data.v < 3){
              whichFig  = pMsg->Data.v;
              switch(whichFig)
              {
                 case 0:WM_SetFocus(hExDim[2]); break;
                 case 1:WM_SetFocus(hExDim[0]); break;
                 case 2:WM_SetFocus(hExDim[4]); break;
              }
              HSD_STICKFIGURE_SetPenColor(hFigs[whichFig], HSD_STICKFIGURE_CI_UNFOCUS, pColors->focusArrowLineColor);
           }
           break;
   
      case WM_CREATE:
           dummyDouDstSet.width1  = 200;
           dummyDouDstSet.width2  = 100;
           dummyDouDstSet.length1 = 500;
           dummyDouDstSet.length2 = 600;
           memcpy(&agentDouDstSet, &dummyDouDstSet, sizeof(dummyDouDstSet));
           
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
           HSD_DIMENSION_EX_SetBkColor(hExDim[0], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS,  pColors->focusBkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[0],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           arrows[0].x =  0;  arrows[1].x =  6; arrows[2].x =  6; arrows[3].x = 153; arrows[4].x = 147; arrows[5].x = 147; 
           arrows[0].y = 20;  arrows[1].y = 22; arrows[2].y = 18; arrows[3].y = 20;  arrows[4].y = 22;  arrows[5].y = 18;
           HSD_DIMENSION_EX_SetArrows(hExDim[0], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[0], &GUI_Font_T90_24);
           HSD_DIMENSION_EX_SetValText(hExDim[0], "100");
           HSD_DIMENSION_EX_SetUnitText(hExDim[0], "米");
           WM_SetHasTrans(hExDim[0]);           
         
           hExDim[1]  = HSD_DIMENSION_EX_CreateEx(160, 206,88, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_1);         
           HSD_DIMENSION_EX_SetBkColor(hExDim[1], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[1], HSD_DIMENSION_EX_CI_FOCUS,  pColors->focusBkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[1],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[1], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           arrows[0].x = 0;   arrows[1].x =  6; arrows[2].x =  6; arrows[3].x = 88; arrows[4].x = 82; arrows[5].x = 82; 
           arrows[0].y = 20;  arrows[1].y = 22; arrows[2].y = 18; arrows[3].y = 20; arrows[4].y = 22; arrows[5].y = 18;
           HSD_DIMENSION_EX_SetArrows(hExDim[1], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[1], &GUI_Font_T90_24);
           HSD_DIMENSION_EX_SetValText(hExDim[1], "1000");
           HSD_DIMENSION_EX_SetUnitText(hExDim[1], "米");
           WM_SetHasTrans(hExDim[1]);          
         
           hExDim[2]  = HSD_DIMENSION_EX_CreateEx(70, 120,68, 112, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_2);         
           HSD_DIMENSION_EX_SetBkColor(hExDim[2], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[2], HSD_DIMENSION_EX_CI_FOCUS,  pColors->focusBkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[2],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[2], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           arrows[0].x = 14;  arrows[1].x =  17; arrows[2].x = 13; arrows[3].x =  52; arrows[4].x =  48; arrows[5].x =  53; 
           arrows[0].y =  1;  arrows[1].y =   5; arrows[2].y =  7; arrows[3].y = 113; arrows[4].y = 110; arrows[5].y = 108;
           HSD_DIMENSION_EX_SetArrows(hExDim[2], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[2], &GUI_Font_T90_24);
           HSD_DIMENSION_EX_SetValText(hExDim[2], "2000");
           HSD_DIMENSION_EX_SetUnitText(hExDim[2], "米");
           WM_SetHasTrans(hExDim[2]);    

           hExDim[3]  = HSD_DIMENSION_EX_CreateEx(224, 125,120, 260, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_3);         
           HSD_DIMENSION_EX_SetBkColor(hExDim[3], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[3], HSD_DIMENSION_EX_CI_FOCUS,  pColors->focusBkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[3],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[3], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           arrows[0].x = 110;  arrows[1].x = 105; arrows[2].x =  111; arrows[3].x =  10; arrows[4].x =  9; arrows[5].x = 14; 
           arrows[0].y =   1;  arrows[1].y =   5; arrows[2].y =    7; arrows[3].y = 258; arrows[4].y = 252; arrows[5].y = 254;
           HSD_DIMENSION_EX_SetArrows(hExDim[3], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[3], &GUI_Font_T90_24);
           HSD_DIMENSION_EX_SetValText(hExDim[3], "3000");
           HSD_DIMENSION_EX_SetUnitText(hExDim[3], "米");
           WM_SetHasTrans(hExDim[3]);   

           hExDim[4]  = HSD_DIMENSION_EX_CreateEx(218, 106,75, 112, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_4);         
           HSD_DIMENSION_EX_SetBkColor(hExDim[4], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[4], HSD_DIMENSION_EX_CI_FOCUS,  pColors->focusBkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[4],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[4], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           arrows[0].x = 62;  arrows[1].x = 58; arrows[2].x =  63; arrows[3].x =  20; arrows[4].x =  19; arrows[5].x =  24; 
           arrows[0].y =  1;  arrows[1].y =  5; arrows[2].y =   7; arrows[3].y = 108; arrows[4].y = 102; arrows[5].y = 104;
           HSD_DIMENSION_EX_SetArrows(hExDim[4], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[4], &GUI_Font_T90_24);
           HSD_DIMENSION_EX_SetValText(hExDim[4], "3000");
           HSD_DIMENSION_EX_SetUnitText(hExDim[4], "米");
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



/** ˫�а�ȫ��������ô��ڴ���
 *
 *
 */
WM_HWIN  WIN_doubleShipDstSetCreate(void)
{
   WM_HWIN handle;
   
   handle  = WM_CreateWindow(DST_SET_X, DST_SET_Y, DST_SET_WIDTH, DST_SET_HEIGHT,
                             WM_CF_SHOW, myWindowcallback,  0);
}


