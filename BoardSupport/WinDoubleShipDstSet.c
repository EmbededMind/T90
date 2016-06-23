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


WM_HWIN doubleShipDstSetWin;


static DouDstSet agentDouDstSet;
static DouDstSet dummyDouDstSet;

static const GUI_RECT drawArea  = {50, 50, DST_SET_WIDTH-50, DST_SET_HEIGHT-50};
static const GUI_RECT tipStrArea = {50, DST_SET_HEIGHT-50 +2 , DST_SET_WIDTH -50, DST_SET_HEIGHT -50 +32};

static const SetWinColor* pColors  = setWinColors;

static WM_HWIN hExDim[4];

/** @brief Ë«ÍÐ¾àÀëÉèÖÃ´°¿ÚµÄ»Øµ÷
 *
 *  @param [in] pMsg  ÏûÏ¢Ö¸Õë
 */
static void myWindowcallback(WM_MESSAGE * pMsg)
{
   GUI_POINT arrows[6];
   switch(pMsg->MsgId){
      case WM_CREATE:
           dummyDouDstSet.width1  = 200;
           dummyDouDstSet.width2  = 100;
           dummyDouDstSet.length1 = 500;
           dummyDouDstSet.length2 = 600;
           memcpy(&agentDouDstSet, &dummyDouDstSet, sizeof(dummyDouDstSet));
           
           pColors  = &setWinColors[t90_set.sys.nightmode];
           
           hExDim[0]  = HSD_DIMENSION_EX_CreateEx(100, 100, 80, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_0);
//           hExDim[1]  = HSD_DIMENSION_EX_CreateEx(100, 200, 80, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_1);
//           hExDim[2]  = HSD_DIMENSION_EX_CreateEx(50,  200, 80, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_2);
//           hExDim[3]  = HSD_DIMENSION_EX_CreateEx(200, 200, 80, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_EX_DIM_3);
           
           HSD_DIMENSION_EX_SetBkColor(hExDim[0], HSD_DIMENSION_EX_CI_UNFOCUS,pColors->bkColor);
           HSD_DIMENSION_EX_SetBkColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS,  pColors->focusBkColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[0],HSD_DIMENSION_EX_CI_UNFOCUS, pColors->arrowLineColor);
           HSD_DIMENSION_EX_SetArrowLineColor(hExDim[0], HSD_DIMENSION_EX_CI_FOCUS, pColors->focusArrowLineColor);
           arrows[0].x = 10;  arrows[1].x = 16; arrows[2].x = 12; arrows[3].x = 40; arrows[4].x = 34; arrows[5].x = 38; 
           arrows[0].y = 10;  arrows[1].y = 12; arrows[2].y = 16; arrows[3].y = 40; arrows[4].y = 38; arrows[5].y = 34;
           HSD_DIMENSION_EX_SetArrows(hExDim[0], arrows); 
           HSD_DIMENSION_EX_SetFont(hExDim[0], &GUI_Font_T90_24);
           HSD_DIMENSION_EX_SetValText(hExDim[0], "100");
           HSD_DIMENSION_EX_SetUnitText(hExDim[0], "ç±³");
           WM_SetHasTrans(hExDim[0]);           
         
           break;
           
     case WM_PAINT:
          {
             GUI_RECT r;
             int xPos_M; 
             int yPos_M;
             int xPos_A;
             int yPos_A;
             
             WM_GetClientRectEx(pMsg->hWin,&r);              
             GUI_SetBkColor(GUI_GRAY);
             GUI_ClearRectEx(&r);
             
             GUI_SetColor(GUI_RED);
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


