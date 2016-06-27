#include "HSD_DIMENSION_EX.h"
#include "Config.h"
#include "stdio.h"
#include "str.h"
#include "math.h"

typedef struct
{
   WIDGET widget;
   GUI_COLOR bkColor[2];
   GUI_COLOR arrowLineColor[2];
   GUI_COLOR valColor[2];
   GUI_COLOR unitColor[2];
   GUI_POINT arrows[6];
   const GUI_FONT GUI_UNI_PTR* pFont;
   WM_HMEM   valText;
   WM_HMEM   unitText;
} HSD_DIMENSION_EX_Obj;


static void _Paint(HSD_DIMENSION_EX_Obj* pObj, HSD_DIMENSION_EX_Handle hObj)
{
   GUI_RECT r;
   GUI_COLOR bkColor;
   GUI_COLOR valColor;
   GUI_COLOR unitColor;
   GUI_COLOR arrowLineColor;
   
   I16 valTextWidth;
   I16 unitTextWidth;
   I16 textHeight;
   
   const char* sVal  = NULL;
   const char* sUnit = NULL;
   
   GUI_GetClientRect(&r);
   
   
   if(pObj->widget.State & WIDGET_STATE_FOCUS){
      bkColor  = pObj->bkColor[1];
      valColor = pObj->valColor[1];
      unitColor= pObj->unitColor[1];
      arrowLineColor = pObj->arrowLineColor[1];
   }
   else{
      bkColor  = pObj->bkColor[0];
      valColor = pObj->valColor[0];
      unitColor= pObj->unitColor[0];
      arrowLineColor = pObj->arrowLineColor[0];
   }
   
//   GUI_SetColor(bkColor);
////   GUI_FillRectEx(&r);
//   GUI_DrawRectEx(&r);
   
   GUI_SetColor(arrowLineColor);
   GUI_SetLineStyle(GUI_LS_SOLID);
   GUI_SetPenSize(1);

   GUI_FillPolygon(pObj->arrows, 3, 0, 0);
   
   GUI_FillPolygon(&(pObj->arrows[3]), 3, 0, 0);
   
   GUI_SetColor(arrowLineColor);
   GUI_DrawLine(pObj->arrows[0].x, pObj->arrows[0].y, pObj->arrows[3].x, pObj->arrows[3].y);
   
   GUI_SetTextMode(GUI_TM_TRANS);
   if(pObj->valText){
      sVal  = (const char*) GUI_ALLOC_h2p(pObj->valText);    
      GUI_SetFont(pObj->pFont);
      
      valTextWidth  = GUI_GetStringDistX(sVal);
      textHeight    = GUI_GetFontDistY();    
      
      if(pObj->unitText){
         sUnit  = (const char*) GUI_ALLOC_h2p(pObj->unitText);
         unitTextWidth  = GUI_GetStringDistX(sUnit);       
      }
      
      GUI_SetColor(bkColor);
      
      r.x0  = (pObj->arrows[0].x + pObj->arrows[3].x)/2 - (valTextWidth+unitTextWidth)/2;
      r.x1  = (pObj->arrows[0].x + pObj->arrows[3].x)/2 + (valTextWidth+unitTextWidth)/2;
      r.y0  = (pObj->arrows[0].y + pObj->arrows[3].y)/2 - textHeight/2;
      r.y1  = (pObj->arrows[0].y + pObj->arrows[3].y)/2 + textHeight/2;
      GUI_FillRectEx(&r);

      
      GUI_SetColor(valColor);
      if(pObj->widget.State & WIDGET_STATE_FOCUS){
         GUI_FillRect(r.x0, r.y0, r.x0 + valTextWidth, r.y0 + textHeight);

         
         GUI_SetColor(GUI_WHITE);
      }
      GUI_DispStringAt(sVal, r.x0, r.y0);
      GUI_SetColor(valColor);
      GUI_DispString(sUnit);
   }
}



void HSD_DIMENSION_EX_Callback(WM_MESSAGE* pMsg)
{
   HSD_DIMENSION_EX_Handle hObj  = pMsg->hWin;
   HSD_DIMENSION_EX_Obj*   pObj  = HSD_DIMENSION_EX_H2P(hObj);
   
   if(WIDGET_HandleActive(hObj, pMsg) == 0){
      return ;
   }
   
   switch(pMsg->MsgId){
      case WM_PAINT:
           _Paint(pObj, hObj);
           return;
   }
   WM_DefaultProc(pMsg);
}



HSD_DIMENSION_EX_Handle HSD_DIMENSION_EX_CreateEx(int x0, int y0, int xSize, int ySize,WM_HWIN hParent,
                                                  int WinFlags, int ExFlags, int Id)
{ 
   HSD_DIMENSION_EX_Handle hObj;
   WM_LOCK();
   
   hObj  = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent,WinFlags, HSD_DIMENSION_EX_Callback,
                                  sizeof(HSD_DIMENSION_EX_Obj) - sizeof(WM_Obj));
                                  
   if(hObj)                                  {
      HSD_DIMENSION_EX_Obj* pObj  = HSD_DIMENSION_EX_H2P(hObj);
      WIDGET__Init(&pObj->widget, Id, WIDGET_STATE_FOCUSSABLE);
      pObj->bkColor[0]  = GUI_LIGHTBLUE;
      pObj->bkColor[1]  = GUI_WHITE;
      pObj->arrowLineColor[0]  = GUI_GRAY;
      pObj->arrowLineColor[1]  = GUI_BLACK;
      pObj->valColor[0]  = GUI_BLACK;
      pObj->valColor[1]  = GUI_LIGHTMAGENTA;
      pObj->unitColor[0]  = GUI_BLACK;
      pObj->unitColor[1]  = GUI_LIGHTMAGENTA;
      pObj->pFont  = &GUI_Font24B_1;
      
      pObj->arrows[0].x  = 10;
      pObj->arrows[0].y  = 10;
      pObj->arrows[1].x  = 14;
      pObj->arrows[1].y  = 10;
      pObj->arrows[2].x  = 10;
      pObj->arrows[2].y  = 14;
      
      pObj->arrows[3].x  = 40;
      pObj->arrows[3].y  = 40;
      pObj->arrows[4].x  = 36;
      pObj->arrows[4].y  = 40;
      pObj->arrows[5].x  = 40;
      pObj->arrows[5].y  = 36;
   }
   
   WM_UNLOCK();
   return hObj;
}



HSD_DIMENSION_EX_Handle  HSD_DIMENSION_EX_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,
                                                        int x0, int y0, WM_CALLBACK* cb)
{
   HSD_DIMENSION_EX_Handle hThis;
   
   GUI_USE_PARA(cb);
   
   hThis  = HSD_DIMENSION_EX_CreateEx(pCreateInfo->x0+x0, pCreateInfo->y0+y0,
                                      pCreateInfo->xSize, pCreateInfo->ySize,
                                      hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
   return hThis;                                        
}



void HSD_DIMENSION_EX_SetArrowLineColor(HSD_DIMENSION_EX_Handle hObj,  I16 flag, GUI_COLOR color)
{
   if(hObj && flag <= HSD_DIMENSION_EX_CI_FOCUS){
      HSD_DIMENSION_EX_Obj * pObj;
      WM_LOCK();
      pObj  = HSD_DIMENSION_EX_H2P(hObj);
      
      if(pObj->arrowLineColor[flag-1] != color){
         pObj->arrowLineColor[flag-1]  = color;
         WM_InvalidateWindow(hObj);
      }
      WM_UNLOCK();
   }
}



void HSD_DIMENSION_EX_SetBkColor(HSD_DIMENSION_EX_Handle hObj, I16 flag, GUI_COLOR color)
{
   if(hObj && flag <= HSD_DIMENSION_EX_CI_FOCUS){
      HSD_DIMENSION_EX_Obj * pObj;
      WM_LOCK();
      pObj   = HSD_DIMENSION_EX_H2P(hObj);
      
      if(pObj->bkColor[flag-1] != color){
         pObj->bkColor[flag-1]  = color;
         WM_InvalidateWindow(hObj);
      }
      WM_UNLOCK();
   }
}



void HSD_DIMENSION_EX_SetValColor(HSD_DIMENSION_EX_Handle hObj, I16 flag, GUI_COLOR color)
{
   if(hObj  &&  flag <= HSD_DIMENSION_EX_CI_FOCUS){
      HSD_DIMENSION_EX_Obj* pObj;
      WM_LOCK();
      pObj  = HSD_DIMENSION_EX_H2P(hObj);
      
      if(pObj->valColor[flag-1] != color){
         pObj->valColor[flag-1]  = color;
         WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}




void HSD_DIMENSION_EX_SetArrows(HSD_DIMENSION_EX_Handle hObj, GUI_POINT* paPoint)
{
   if(hObj){
      HSD_DIMENSION_EX_Obj* pObj;
      
      WM_LOCK();
      pObj  = HSD_DIMENSION_EX_H2P(hObj);
      
      pObj->arrows[0].x  = paPoint[0].x;
      pObj->arrows[0].y  = paPoint[0].y;
      pObj->arrows[1].x  = paPoint[1].x;
      pObj->arrows[1].y  = paPoint[1].y;
      pObj->arrows[2].x  = paPoint[2].x;
      pObj->arrows[2].y  = paPoint[2].y;
      pObj->arrows[3].x  = paPoint[3].x;
      pObj->arrows[3].y  = paPoint[3].y;
      pObj->arrows[4].x  = paPoint[4].x;
      pObj->arrows[4].y  = paPoint[4].y;
      pObj->arrows[5].x  = paPoint[5].x;
      pObj->arrows[5].y  = paPoint[5].y;
      
      WM_InvalidateWindow(hObj);
      
      WM_UNLOCK();
   }
}


void HSD_DIMENSION_EX_SetValText(HSD_DIMENSION_EX_Handle hObj, const char* s)
{
   if(hObj){
      HSD_DIMENSION_EX_Obj* pObj;
      WM_LOCK();
      
      pObj  = HSD_DIMENSION_EX_H2P(hObj);
      
      if(GUI__SetText(&pObj->valText, s)){
         WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}


void HSD_DIMENSION_EX_SetUnitText(HSD_DIMENSION_EX_Handle hObj, const char* s)
{
   if(hObj){
      HSD_DIMENSION_EX_Obj* pObj;
      
      WM_LOCK();
      
      pObj  = HSD_DIMENSION_EX_H2P(hObj);
      
      if( GUI__SetText(&pObj->unitText, s)){
         WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}



void HSD_DIMENSION_EX_SetFont(HSD_DIMENSION_EX_Handle hObj, const GUI_FONT GUI_UNI_PTR* pFont)
{
   if(hObj){
      HSD_DIMENSION_EX_Obj* pObj;
      WM_LOCK();
      pObj  = HSD_DIMENSION_EX_H2P(hObj);
      
      if(pObj->pFont != pFont){
         pObj->pFont  = pFont;
         WM_InvalidateWindow(hObj);
      }
      WM_UNLOCK();
   }
}



