#include "HSD_DIMENSION.h"
#include "Config.h"
#include "stdio.h"
#include "str.h"


typedef struct{
   WIDGET widget;
   GUI_COLOR bkColor[2];
   GUI_COLOR arrowLineColor[2];
   GUI_COLOR valColor[2];
   GUI_COLOR unitColor[2];
   I16 flags;
   const GUI_FONT GUI_UNI_PTR * valFont;

   WM_HMEM valText;
   WM_HMEM unitText;
} HSD_DIMENSION_Obj;



static void _Paint(HSD_DIMENSION_Obj* pObj, HSD_DIMENSION_Handle hObj){
   GUI_RECT r;
   GUI_COLOR bkColor;
   GUI_COLOR valColor;
   GUI_COLOR unitColor;
   GUI_COLOR arrowLineColor;
   
   I16 pixelWidthVal;
   I16 pixelWidthUnit;
   I16 pixelHeight;
   GUI_POINT arrow[3];
   const char *sVal  = NULL;
   const char *sUnit = NULL;
   
   
   GUI_GetClientRect(&r);
   
   if(pObj->widget.State & WIDGET_STATE_FOCUS){
      bkColor   = pObj->bkColor[1];
      valColor  = pObj->valColor[1];
      unitColor = pObj->unitColor[1];
      arrowLineColor  = pObj->arrowLineColor[1];
   }
   else{
      bkColor   = pObj->bkColor[0];
      valColor  = pObj->valColor[0];
      unitColor = pObj->unitColor[0];
      arrowLineColor  = pObj->arrowLineColor[0];
   }
//   GUI_SetColor(bkColor);
//   
//   GUI_FillRectEx(&r);
   
   GUI_SetColor(arrowLineColor);
   GUI_SetLineStyle(GUI_LS_SOLID);
   GUI_SetPenSize(1);  
//INFO("pObj->flags:%d", pObj->flags);  
 
   if(pObj->flags == HSD_DIMENSION_CF_HORIZONTAL){    
//INFO("horizontal");   
      GUI_DrawHLine( (r.y0+r.y1) /2, r.x0+7, r.x1-7);
      arrow[0].x  = 0;
      arrow[0].y  = 0;
      arrow[1].x  = 6;
      arrow[1].y  = -3 ;
      arrow[2].x  = 6;
      arrow[2].y  = 3;
      GUI_FillPolygon(arrow, 3,  r.x0 , (r.y0+r.y1) /2);
      arrow[0].x  = 0;
      arrow[0].y  = 0;
      arrow[1].x  = -6;
      arrow[1].y  = 3;
      arrow[2].x  =-6;
      arrow[2].y  = -3;
      GUI_FillPolygon(arrow, 3,  r.x1, (r.y0+r.y1) /2);

      GUI_SetTextMode(GUI_TM_TRANS);
            
      if(pObj->valText){
         sVal  = (const char*) GUI_ALLOC_h2p(pObj->valText);
         GUI_SetFont(pObj->valFont);
         
         pixelWidthVal  = GUI_GetStringDistX(sVal);
         pixelHeight = GUI_GetFontDistY();
       
         if(pObj->unitText){
            sUnit  = (const char*) GUI_ALLOC_h2p(pObj->unitText);                 
            pixelWidthUnit  = GUI_GetStringDistX(sUnit);        
         }
         GUI_SetColor(bkColor); 
         
         GUI_FillRect( (r.x1-r.x0)/2 - (pixelWidthVal+pixelWidthUnit)/2, 
                       (r.y1-r.y0)/2 - pixelHeight /2, 
                       (r.x1-r.x0)/2 + pixelWidthVal/2 +pixelWidthUnit/2,
                       (r.y1-r.y0)/2 + pixelHeight/2);
         GUI_SetColor(valColor);
         if(pObj->widget.State & WIDGET_STATE_FOCUS){
            GUI_FillRect( (r.x1-r.x0)/2 - (pixelWidthVal+pixelWidthUnit)/2, 
                          (r.y1-r.y0)/2 - pixelHeight/2, 
                          (r.x1-r.x0)/2 + pixelWidthVal/2  - pixelWidthUnit /2,
                          (r.y1-r.y0)/2 + pixelHeight/2);
            GUI_SetColor(GUI_WHITE);
         }
         GUI_DispStringAt(sVal, (r.x1-r.x0) /2 -(pixelWidthVal+pixelWidthUnit)/2, (r.y1-r.y0)/2 -pixelHeight/2); 
         GUI_SetColor(valColor);
         GUI_DispString(sUnit);
      }        
   }
   else{
      GUI_DrawVLine( (r.x0+r.x1) /2, r.y0+7, r.y1-7);
      arrow[0].x  = 0;
      arrow[0].y  = 0;
      arrow[1].x  = 3;
      arrow[1].y  = 6;
      arrow[2].x  = -3;
      arrow[2].y  = 6;
      GUI_FillPolygon(arrow, 3, (r.x0+r.x1) /2, r.y0);
      
      arrow[0].x  = 0;
      arrow[0].y  = 0;
      arrow[1].x  = -3;
      arrow[1].y  = -6;
      arrow[2].x  = 3;
      arrow[2].y  = -6;
      GUI_FillPolygon(arrow, 3, (r.x0+r.x1) /2, r.y1);
      
      GUI_SetTextMode(GUI_TM_TRANS);
            
      if(pObj->valText){
         sVal  = (const char*) GUI_ALLOC_h2p(pObj->valText);
         GUI_SetFont(pObj->valFont);
         
         pixelWidthVal  = GUI_GetStringDistX(sVal);
         pixelHeight = GUI_GetFontDistY();
       
         if(pObj->unitText){
            sUnit  = (const char*) GUI_ALLOC_h2p(pObj->unitText);                 
            pixelWidthUnit  = GUI_GetStringDistX(sUnit);        
         }
         GUI_SetColor(bkColor); 
         
         GUI_FillRect( (r.x1-r.x0)/2 - (pixelWidthVal)/2, 
                       (r.y1-r.y0)/2 - pixelHeight/2, 
                       (r.x1-r.x0)/2 + pixelWidthVal/2,
                       (r.y1-r.y0)/2 +pixelHeight/2);
         GUI_SetColor(valColor);
         if(pObj->widget.State & WIDGET_STATE_FOCUS){
            GUI_FillRect( (r.x1-r.x0)/2 - (pixelWidthVal)/2, 
                          (r.y1-r.y0)/2 - pixelHeight/2, 
                          (r.x1-r.x0)/2 + pixelWidthVal/2 ,
                          (r.y1-r.y0)/2 + pixelHeight/2);
            GUI_SetColor(GUI_WHITE);
         }
         GUI_DispStringAt(sVal, (r.x1-r.x0) /2 -(pixelWidthVal)/2, (r.y1-r.y0)/2 -pixelHeight/2); 
         GUI_SetColor(valColor);
         GUI_DispString(sUnit);
      }        
   }   
  

}







static void _OnKey(HSD_DIMENSION_Handle hObj,  WM_MESSAGE* pMsg){
   if( ((WM_KEY_INFO*)pMsg->Data.p)->PressedCnt > 0){
      switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){
         case GUI_KEY_UP:
       
              break;
         default:
              
              break;
      }
   }
}



void HSD_DIMENSION_Callback(WM_MESSAGE* pMsg){
   HSD_DIMENSION_Handle hObj  = pMsg->hWin;
   HSD_DIMENSION_Obj* pObj  = HSD_DIMENSION_H2P(hObj);
   
   if(WIDGET_HandleActive(hObj, pMsg) == 0){
      return ;
   }
   
   switch(pMsg->MsgId){
      case WM_PAINT:
           _Paint(pObj, hObj);
           return ;
//      case WM_KEY:
//           _OnKey(hObj, pMsg);
//           return ;
   }
   WM_DefaultProc(pMsg);
}


HSD_DIMENSION_Handle  HSD_DIMENSION_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                                             int WinFlags, int ExFlags, int Id)
{
   HSD_DIMENSION_Handle hObj;
   WM_LOCK();
   
   hObj  = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WinFlags, HSD_DIMENSION_Callback,
                                 sizeof(HSD_DIMENSION_Obj) - sizeof(WM_Obj));
                                 
   if(hObj){
      HSD_DIMENSION_Obj * pObj  = HSD_DIMENSION_H2P(hObj);
      WIDGET__Init(&pObj->widget, Id, WIDGET_STATE_FOCUSSABLE);
      pObj->bkColor[0]  = GUI_LIGHTBLUE;
      pObj->bkColor[1]  = GUI_WHITE;
      pObj->arrowLineColor[0]  = GUI_GRAY;
      pObj->arrowLineColor[1]  = GUI_BLACK;
      pObj->valColor[0]  = GUI_BLACK;
      pObj->valColor[1]  = GUI_LIGHTMAGENTA;
      pObj->valFont  = &GUI_Font16_ASCII;
      pObj->flags  = ExFlags;
//INFO("flag:%d", pObj->flags);      
   }
   
   WM_UNLOCK();
  
   return hObj;
}
  


HSD_DIMENSION_Handle   HSD_DIMENSION_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,
                                                    int x0, int y0, WM_CALLBACK* cb)  
{
   HSD_DIMENSION_Handle  hThis;
   
   GUI_USE_PARA(cb);
   
   hThis  = HSD_DIMENSION_CreateEx(pCreateInfo->x0+x0,  pCreateInfo->y0+y0,
                                   pCreateInfo->xSize,   pCreateInfo->ySize,
                                   hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
   return hThis;                                   
}                                             
                                             


void HSD_DIMENSION_SetArrowLineColor(HSD_DIMENSION_Handle hObj, I16 flag,GUI_COLOR color){
   if(hObj && flag <= HSD_DIMENSION_CI_FOCUS){
      HSD_DIMENSION_Obj* pObj;
      WM_LOCK();
      pObj  = HSD_DIMENSION_H2P(hObj);     
      
      if(pObj->arrowLineColor[flag-1] != color){
         pObj->arrowLineColor[flag-1]  = color;
         WM_InvalidateWindow(hObj);
      }
      WM_UNLOCK();
   }
}



void HSD_DIMENSION_SetBkColor(HSD_DIMENSION_Handle hObj, I16 flag, GUI_COLOR color){

   if(hObj && flag <= HSD_DIMENSION_CI_FOCUS){
  
      HSD_DIMENSION_Obj* pObj;
      
      WM_LOCK();
      pObj  = HSD_DIMENSION_H2P(hObj);
      
      if(pObj->bkColor[flag-1] != color){
         pObj->bkColor[flag-1]  = color;        
         WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}



void HSD_DIMENSION_SetValColor(HSD_DIMENSION_Handle hObj, I16 flag, GUI_COLOR color)
{
   if(hObj && flag <= HSD_DIMENSION_CI_FOCUS){
      HSD_DIMENSION_Obj* pObj;
      
      WM_LOCK();
      
      pObj  = HSD_DIMENSION_H2P(hObj);
      if(pObj->valColor[flag-1] != color){
         pObj->valColor[flag-1]  = color;
         WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}


void HSD_DIMENSION_SetValText(HSD_DIMENSION_Handle hObj, const char *s)
{
   if(hObj){
      HSD_DIMENSION_Obj* pObj;
      WM_LOCK();
      pObj  = HSD_DIMENSION_H2P(hObj);
      
      if( GUI__SetText(&pObj->valText, s)){
         WM_InvalidateWindow(hObj);
      }
      WM_UNLOCK();
   }
}


void HSD_DIMENSION_SetValFont(HSD_DIMENSION_Handle hObj, const GUI_FONT GUI_UNI_PTR* pFont)
{
   if(hObj){
      HSD_DIMENSION_Obj * pObj;
      
      WM_LOCK();
      
      pObj  = HSD_DIMENSION_H2P(hObj);
      
      if(pObj->valFont != pFont){
         pObj->valFont  = pFont;
      }
      
      WM_UNLOCK();
   }
}




void HSD_DIMENSION_SetUnitText(HSD_DIMENSION_Handle hObj, const char * s)
{
   if(hObj){
      HSD_DIMENSION_Obj* pObj;
      
      WM_LOCK();
      
      pObj  = HSD_DIMENSION_H2P(hObj);
      if( GUI__SetText(&pObj->unitText, s)){
          WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}















