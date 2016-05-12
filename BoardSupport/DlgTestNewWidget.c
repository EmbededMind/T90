#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "HSD_DIMENSION.h"
#include "HSD_STICKFIGURE.h"
#include "Config.h"

#define ID_WIDGET_0           (GUI_ID_USER + 0x00)
#define ID_WIDGET_1           (GUI_ID_USER + 0x01)
#define ID_WIDGET_2           (GUI_ID_USER + 0x02)



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = {
   {WINDOW_CreateIndirect, "clientWin",  ID_WIDGET_0, 0, 0, 800, 400,  0,   0,  0},
  
   {HSD_STICKFIGURE_CreateIndirect, "icon",  ID_WIDGET_1, 200, 200, 20, 20, 0, 0, 0},
   {HSD_STICKFIGURE_CreateIndirect, "icon",  ID_WIDGET_2, 400, 200, 16, 20, 0, 0, 0}
};


static void myDialogCallback(WM_MESSAGE* pMsg)
{
   WM_HWIN handle;
   
   switch(pMsg->MsgId){
   case WM_INIT_DIALOG:
        handle  = WM_GetDialogItem(pMsg->hWin, ID_WIDGET_1);     
        HSD_STICKFIGURE_SetPenColor(handle, HSD_STICKFIGURE_CI_UNFOCUS, GUI_BLACK);
        HSD_STICKFIGURE_SetPenColor(handle, HSD_STICKFIGURE_CI_FOCUS, GUI_LIGHTBLUE);
        
        handle  = WM_GetDialogItem(pMsg->hWin, ID_WIDGET_2);
        HSD_STICKFIGURE_SetPenColor(handle, HSD_STICKFIGURE_CI_UNFOCUS, GUI_BLACK);
        HSD_STICKFIGURE_SetPenColor(handle, HSD_STICKFIGURE_CI_FOCUS, GUI_LIGHTBLUE);
        break;
   default:
        WM_DefaultProc(pMsg);
        break;
   }
}


WM_HWIN DLG_testCustomedWidgetCreate(void)
{
   WM_HWIN handle;
   
   handle  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &myDialogCallback, WM_HBKWIN, 0, 0);
   return handle;
}

//#define ID_WINDOW      (GUI_ID_USER + 0x00)

//#define ID_DM_1          (GUI_ID_USER + 0x10)
//#define ID_DM_2          (GUI_ID_USER + 0x11)

//static void myDimensionCallback(WM_MESSAGE* pMsg);



//static const GUI_WIDGET_CREATE_INFO _aDialogCrate[]  = {
//      {WINDOW_CreateIndirect,  "clientWin",  ID_WINDOW, 0, 0, 800, 480,  0, 0, 0 },
//      
//      {HSD_DIMENSION_CreateIndirect, "dm",   ID_DM_1,     200, 200, 100, 20, HSD_DIMENSION_CF_HORIZONTAL,  0, 0},
//      {HSD_DIMENSION_CreateIndirect, "dm",   ID_DM_2,     400, 200, 80, 100, HSD_DIMENSION_CF_VERTICAL,  0, 0}
//};



//static void myDialogCallback(WM_MESSAGE* pMsg){

//   WM_HWIN handle;
//   switch(pMsg->MsgId){
//      case WM_INIT_DIALOG:     
//           handle  = WM_GetDialogItem(pMsg->hWin, ID_WINDOW);  
//           WINDOW_SetBkColor(pMsg->hWin, GUI_GRAY);
//           
//           handle  = WM_GetDialogItem(pMsg->hWin, ID_DM_1);
//           HSD_DIMENSION_SetBkColor(handle, HSD_DIMENSION_CI_UNFOCUS, GUI_GRAY);
//           HSD_DIMENSION_SetBkColor(handle, HSD_DIMENSION_CI_FOCUS,   GUI_GRAY);
//           
//           HSD_DIMENSION_SetArrowLineColor(handle, HSD_DIMENSION_CI_UNFOCUS, GUI_LIGHTGRAY);
//           HSD_DIMENSION_SetArrowLineColor(handle, HSD_DIMENSION_CI_FOCUS,   GUI_BLACK);
//           
//           HSD_DIMENSION_SetValColor(handle, HSD_DIMENSION_CI_UNFOCUS, GUI_BLUE);
//           HSD_DIMENSION_SetValColor(handle, HSD_DIMENSION_CI_FOCUS, GUI_MAGENTA);

//           HSD_DIMENSION_SetValText(handle, "135");
//           HSD_DIMENSION_SetUnitText(handle, "mm");
//           WM_SetCallback(handle, &myDimensionCallback);
//           
//           handle  = WM_GetDialogItem(pMsg->hWin, ID_DM_2);
//           HSD_DIMENSION_SetBkColor(handle, HSD_DIMENSION_CI_UNFOCUS, GUI_GRAY);
//           HSD_DIMENSION_SetBkColor(handle, HSD_DIMENSION_CI_FOCUS,   GUI_GRAY);
//           
//           HSD_DIMENSION_SetArrowLineColor(handle, HSD_DIMENSION_CI_UNFOCUS, GUI_LIGHTGRAY);
//           HSD_DIMENSION_SetArrowLineColor(handle, HSD_DIMENSION_CI_FOCUS,   GUI_BLACK);
//           
//           HSD_DIMENSION_SetValColor(handle, HSD_DIMENSION_CI_UNFOCUS, GUI_BLUE);
//           HSD_DIMENSION_SetValColor(handle, HSD_DIMENSION_CI_FOCUS, GUI_MAGENTA);

//           HSD_DIMENSION_SetValText(handle, "135");
//           HSD_DIMENSION_SetUnitText(handle, "mm");  
//           WM_SetCallback(handle, &myDimensionCallback);           
//           break;
//           
//      default:
//           WM_DefaultProc(pMsg);
//           break;
//   }
//}


//WM_HWIN DLG_testDimensinCreate(void){
//  WM_HWIN handle;
//  
//  handle  = GUI_CreateDialogBox(_aDialogCrate, GUI_COUNTOF(_aDialogCrate), &myDialogCallback, WM_HBKWIN, 0, 0);
//  return handle;
//}


//static void myDimensionCallback(WM_MESSAGE* pMsg){
//   
//   switch(pMsg->MsgId){
//      case WM_KEY:  
//           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){
//              case GUI_KEY_LEFT:
//                   WM_SetFocusOnPrevChild(WM_GetParent(pMsg->hWin));
//                   break;
//              case GUI_KEY_RIGHT:            
//                   WM_SetFocusOnNextChild(WM_GetParent(pMsg->hWin));
//                   break;
//           }
//           break;
//     default:
//           HSD_DIMENSION_Callback(pMsg);
//           break;
//   }
//}




