#include "HSD_STICKFIGURE.h"

#include "Config.h"

typedef struct{
   WIDGET widget;
//   GUI_COLOR bkColor[1];
   GUI_COLOR penColor[2];
//   void (*pfnWriter)(void) ;
} HSD_STICKFIGURE_Obj;

static void _Paint(HSD_STICKFIGURE_Obj* pObj, HSD_STICKFIGURE_Handle hObj){
   GUI_RECT r;
//   GUI_COLOR bkColor;
   GUI_COLOR penColor;
   
   GUI_GetClientRect(&r);
   
   if(pObj->widget.State & WIDGET_STATE_FOCUS){
//     bkColor  = pObj->bkColor[1];
     penColor = pObj->penColor[1];
   
   }
   else{
//     bkColor  = pObj->bkColor[0];
     penColor = pObj->penColor[0];
   }
   
//   LCD_SetColor(bkColor);
//   GUI_FillRectEx(&r);
//INFO("x0:%d  y0:%d  x1:%d  y1:%d", r.x0, r.y0, r.x1, r.y1);   
   GUI_SetColor(penColor);
//INFO("%ld",penColor);
   GUI_SetLineStyle(GUI_LS_SOLID);
//   GUI_SetColor(GUI_WHITE);
   GUI_DrawLine(r.x0,r.y1, (r.x0+r.x1)/2, r.y0);
   GUI_DrawLine(r.x1,r.y1, (r.x0+r.x1)/2, r.y0);
   GUI_DrawHLine(r.y1, r.x0, r.x1);
   
//   if(pObj->pfnWriter){
//      (*pObj->pfnWriter)();
//   }
}



static void _OnKey(HSD_STICKFIGURE_Handle hObj, WM_MESSAGE* pMsg)
{
   const WM_KEY_INFO* pKeyInfo;
  
   pKeyInfo  = (const WM_KEY_INFO*)(pMsg->Data.p);
   if(pKeyInfo->PressedCnt > 0){   
      switch(pKeyInfo->Key){
      case GUI_KEY_UP:
      case GUI_KEY_LEFT:  
           WM_SetFocusOnPrevChild(WM_GetParent(pMsg->hWin));
           break;
      case GUI_KEY_DOWN:
      case GUI_KEY_RIGHT:    
           WM_SetFocusOnNextChild(WM_GetParent(pMsg->hWin));          
           break;
      default:
           return;
      }
   }
}

void HSD_STICKFIGURE_Callback(WM_MESSAGE* pMsg)
{
   HSD_STICKFIGURE_Handle hObj  = pMsg->hWin;
   HSD_STICKFIGURE_Obj*   pObj  = HSD_STICKFIGURE_H2P(hObj);
   
   if(WIDGET_HandleActive(hObj, pMsg) == 0){
      return ;
   }

   switch(pMsg->MsgId){
   case WM_PAINT:
        _Paint(pObj, hObj);
        return;
   case WM_KEY:
        _OnKey(hObj, pMsg);
        return ; 
   default:
        WM_DefaultProc(pMsg);
        return;;
   }
   
  
}




HSD_STICKFIGURE_Handle HSD_STICKFIGURE_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                                                int WinFlags, int ExFlags, int Id)
{
   HSD_STICKFIGURE_Handle hObj;
   WM_LOCK();
   
   hObj  = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WinFlags, HSD_STICKFIGURE_Callback, sizeof(HSD_STICKFIGURE_Obj) - sizeof(WM_Obj));
   
   if(hObj){
      HSD_STICKFIGURE_Obj* pObj  = HSD_STICKFIGURE_H2P(hObj);
      WIDGET__Init(&pObj->widget, Id, WIDGET_STATE_FOCUSSABLE);
      pObj->penColor[0]  = GUI_GRAY;
      pObj->penColor[1]  = GUI_BLACK;
      WM_SetCallback(hObj, &HSD_STICKFIGURE_Callback);
      WM_SetHasTrans(hObj);
   }
   else{
    
   }
   
   WM_UNLOCK();
   
   return hObj;
} 



HSD_STICKFIGURE_Handle HSD_STICKFIGURE_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,
                                                      int x0, int y0, WM_CALLBACK* cb)                                               
{
   HSD_STICKFIGURE_Handle hThis;
   
   GUI_USE_PARA(cb);
   
   hThis  = HSD_STICKFIGURE_CreateEx(pCreateInfo->x0+x0, pCreateInfo->y0+y0,
                                     pCreateInfo->xSize, pCreateInfo->ySize,
                                     hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
                                     
   return hThis;                                     
}                                                      



void HSD_STICKFIGURE_SetPenColor(HSD_STICKFIGURE_Handle hObj,  I16 flag, GUI_COLOR color)
{
   if(hObj){
      HSD_STICKFIGURE_Obj* pObj;
      WM_LOCK();
      
      pObj  = HSD_STICKFIGURE_H2P(hObj);
      if(pObj->penColor[flag] != color){
         pObj->penColor[flag]  = color;
         WM_InvalidateWindow(hObj);
      }
      
      WM_UNLOCK();
   }
}














