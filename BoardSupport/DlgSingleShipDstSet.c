#include "GUI.h"
#include "WM.h"
#include "HSD_STICKFIGURE.h"
#include "HSD_DIMENSION.h"
#include "DIALOG.h"

#include "layout_dist_set_area.h"
#include "Config.h"


#define ID_WINDOW           (GUI_ID_USER + 0x00)

#define ID_STICKFIGURE_0    (GUI_ID_USER + 0x10)
#define ID_STICKFIGURE_1    (GUI_ID_USER + 0x11)
#define ID_STICKFIGURE_2    (GUI_ID_USER + 0x12)

#define ID_DIMENSION_0      (GUI_ID_USER + 0x20)
#define ID_DIMENSION_1      (GUI_ID_USER + 0x21)
#define ID_DIMENSION_2      (GUI_ID_USER + 0x22)
#define ID_DIMENSION_3      (GUI_ID_USER + 0x23)
#define ID_DIMENSION_4      (GUI_ID_USER + 0x24)

WM_HWIN singleShipDstSetDlg;

HSD_STICKFIGURE_Handle hStickFigures[3];
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = {
   {WINDOW_CreateIndirect, "clientWin", ID_WINDOW, 0, 0, DST_SET_AREA_WIDTH-1, DST_SET_AREA_HEIGHT, 0, 0, 0},
   
   {HSD_STICKFIGURE_CreateIndirect, "sf0",  ID_STICKFIGURE_0, 117,251, 16, 20, 0, 0, 0},
   {HSD_STICKFIGURE_CreateIndirect, "sf1",  ID_STICKFIGURE_1, 317,251, 16, 20, 0, 0, 0},
   {HSD_STICKFIGURE_CreateIndirect, "sf2",  ID_STICKFIGURE_2, 217,301, 16, 20, 0, 0, 0}
};

static const GUI_RECT drawArea  = {50, 50, DST_SET_AREA_WIDTH-50, DST_SET_AREA_HEIGHT-50};

static const GUI_RECT tipStrArea = {50, DST_SET_AREA_HEIGHT-50 +2, DST_SET_AREA_WIDTH-50, DST_SET_AREA_HEIGHT-50 +32};


HSD_DIMENSION_Handle hDimensions[5];


static void myStickFigureCallback(WM_MESSAGE* pMsg)
{
   int  id  = WM_GetId(pMsg->hWin) - ID_STICKFIGURE_0;
   switch( pMsg->MsgId ){
   case WM_SET_FOCUS:
        if(pMsg->Data.v){
      
        }
        else{
        }
        HSD_STICKFIGURE_Callback(pMsg);
        break;
   
   case WM_KEY:
   
        switch( ((WM_KEY_INFO*)(pMsg->Data.p))->Key){
        case GUI_KEY_RIGHT:
             HSD_STICKFIGURE_Callback(pMsg);
             break;
        case GUI_KEY_LEFT:
             HSD_STICKFIGURE_Callback(pMsg);
             break;
        }
        break;
        
   default:
        HSD_STICKFIGURE_Callback(pMsg);
        break;
   }
}


static void myDialogCallback(WM_MESSAGE* pMsg)
{
   switch(pMsg->MsgId){
   case WM_INIT_DIALOG:
        WINDOW_SetBkColor(pMsg->hWin, GUI_LIGHTGRAY);
        
        hStickFigures[0]  = WM_GetDialogItem(pMsg->hWin, ID_STICKFIGURE_0);
        HSD_STICKFIGURE_SetPenColor(hStickFigures[0], HSD_STICKFIGURE_CI_UNFOCUS, GUI_BLACK);
        HSD_STICKFIGURE_SetPenColor(hStickFigures[0], HSD_STICKFIGURE_CI_FOCUS,  GUI_LIGHTBLUE);
        
        hStickFigures[1]  = WM_GetDialogItem(pMsg->hWin, ID_STICKFIGURE_1);
        HSD_STICKFIGURE_SetPenColor(hStickFigures[1], HSD_STICKFIGURE_CI_UNFOCUS, GUI_BLACK);
        HSD_STICKFIGURE_SetPenColor(hStickFigures[1], HSD_STICKFIGURE_CI_FOCUS,  GUI_LIGHTBLUE);        
        
        hStickFigures[2]  = WM_GetDialogItem(pMsg->hWin, ID_STICKFIGURE_2);
        HSD_STICKFIGURE_SetPenColor(hStickFigures[2], HSD_STICKFIGURE_CI_UNFOCUS, GUI_BLACK);
        HSD_STICKFIGURE_SetPenColor(hStickFigures[2], HSD_STICKFIGURE_CI_FOCUS,  GUI_LIGHTBLUE); 


//        hDimensions[0]  = HSD_DIMENSION_CreateEx(80, 112, 90, 140,pMsg->hWin, HSD_DIMENSION_CF_VERTICAL, 0, ID_DIMENSION_0);        
//        HSD_DIMENSION_SetBkColor(hDimensions[0], HSD_DIMENSION_CI_UNFOCUS, GUI_LIGHTGRAY);
//        HSD_DIMENSION_SetValFont(hDimensions[0], &GUI_Font20_ASCII);
//        HSD_DIMENSION_SetValText(hDimensions[0],"100");
//        HSD_DIMENSION_SetUnitText(hDimensions[0], "m");
//        WM_SetHasTrans(hDimensions[0]);
        
        hDimensions[0]  = HSD_DIMENSION_CreateEx(80, 112, 140, 90,pMsg->hWin, HSD_DIMENSION_CF_VERTICAL, 0, ID_DIMENSION_0);        
        HSD_DIMENSION_SetBkColor(hDimensions[0], HSD_DIMENSION_CI_UNFOCUS, GUI_LIGHTGRAY);
        HSD_DIMENSION_SetValFont(hDimensions[0], &GUI_Font20_ASCII);
        HSD_DIMENSION_SetValText(hDimensions[0],"100");
        HSD_DIMENSION_SetUnitText(hDimensions[0], "m");
        WM_SetHasTrans(hDimensions[0]);
        
//        hDimensions[2]  = HSD_DIMENSION_CreateEx(280,112, 90, 140, pMsg->hWin, HSD_DIMENSION_CF_VERTICAL, 0, ID_DIMENSION_2);
//        HSD_DIMENSION_SetBkColor(hDimensions[2], HSD_DIMENSION_CI_UNFOCUS, GUI_LIGHTGRAY);        
//        HSD_DIMENSION_SetValFont(hDimensions[2], &GUI_Font20_ASCII);
//        HSD_DIMENSION_SetValText(hDimensions[2], "100");
//        HSD_DIMENSION_SetUnitText(hDimensions[2], "mm");
//        WM_SetHasTrans(hDimensions[2]);
//        
//        hDimensions[4]  = HSD_DIMENSION_CreateEx(180,112, 90, 190, pMsg->hWin, HSD_DIMENSION_CF_VERTICAL, 0, ID_DIMENSION_2);
//        HSD_DIMENSION_SetBkColor(hDimensions[4], HSD_DIMENSION_CI_UNFOCUS, GUI_LIGHTGRAY);        
//        HSD_DIMENSION_SetValFont(hDimensions[4], &GUI_Font20_ASCII);
//        HSD_DIMENSION_SetValText(hDimensions[4], "500");
//        HSD_DIMENSION_SetUnitText(hDimensions[4], "mm");
//        WM_SetHasTrans(hDimensions[4]);        
        break;
        
    case WM_PAINT:
        GUI_SetColor(GUI_RED);
        GUI_SetLineStyle(GUI_LS_SOLID);
        GUI_DrawRectEx(&drawArea);
        
        GUI_DrawRectEx(&tipStrArea);
        
        GUI_SetDrawMode(GUI_DM_NORMAL);
        GUI_SetColor(GUI_BLACK);
        
        {
           int orgX  = (drawArea.x0+drawArea.x1) /2;
           int orgY  = drawArea.y0 +1;
           
           GUI_DrawLine(orgX, orgY, orgX+16, orgY+16);
           
           GUI_DrawVLine(orgX+16, orgY+17, orgY+60);
           GUI_DrawHLine(orgY+60, orgX-15, orgX+15);
           GUI_DrawVLine(orgX-16, orgY+17, orgY+60);
           
           GUI_DrawLine(orgX-16, orgY +16, orgX , orgY);
           
           GUI_SetColor(GUI_BLACK);
           GUI_SetLineStyle(GUI_LS_DOT);
           
           GUI_DrawLine(drawArea.x0, orgY+60, orgX-17, orgY+60);
           GUI_DrawLine(orgX+17, orgY+60, drawArea.x1, orgY+60);
           
           GUI_DrawLine(orgX-16, orgY+60, orgX-100, orgY+200);
           GUI_DrawLine(orgX+16, orgY+60, orgX+100, orgY+200);
           
//INFO("x:%d, y:%d",orgX-100, orgY+200);       
//INFO("x:%d, y:%d",orgX+100, orgY+200);
        }
        
        break;
        
    default:
        WM_DefaultProc(pMsg);
        break;
   }
}


WM_HWIN DLG_SingleShipDstSetAreaCreate(void)
{
   WM_HWIN handle;

   handle  = GUI_CreateDialogBox(_aDialogCreate,  GUI_COUNTOF(_aDialogCreate), &myDialogCallback, WM_HBKWIN, DST_SET_AREA_X, DST_SET_AREA_Y);
   singleShipDstSetDlg  = handle;
   return handle;
}


