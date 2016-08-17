#include "WM.h"
#include "GUI.h"
#include <BUTTON.h>
#include "layout.h"
#include "HSD_Toast.h"
#include <lpc_types.h>
#include "T90.h"
#include "t90font.h"
#include "TEXT.h"
#define BTN_WIDTH   80
#define BTN_HEIGHT  40

#define BTN_MARGIN  10
#define TEXT_MARGIN  5


extern const ConfirmWinColor confirmWinColors[2];
extern Bool toast_flg;
GUI_RECT pRect = {0,0,400,200};

typedef struct {

   const GUI_FONT GUI_UNI_PTR* pFont;
   const char*                 text;
   uint16_t                    liveTime;
   uint8_t                     btnFlags;
   uint8_t                     isAlive;
   WM_HWIN                     hWin;
   WM_HWIN                     hToast;
   GUI_RECT                    winRect;
}Toast;


static Toast myToast;
static WM_HWIN hBtns[2];
static WM_HTIMER myToastTimer;

static void myButtonCallback(WM_MESSAGE* pMsg)
{
   switch(pMsg->MsgId){
      case WM_KEY:
           if( ((WM_KEY_INFO*)pMsg->Data.p)->Key == GUI_KEY_ENTER)    {
              toast_flg = FALSE;
              WM_DeleteWindow(WM_GetParent(pMsg->hWin));
              
              WM_SetFocus(myToast.hWin);
              myToast.btnFlags  = 0;
              myToast.hWin   = 0;
              hBtns[0]  = 0;
              myToast.hToast  = 0;
           }
           break;
      
      default:
           BUTTON_Callback(pMsg);
           break;
   }
}


static void myWindowCallback(WM_MESSAGE* pMsg)
{
   GUI_RECT clientRect;
   int xSize;
   int ySize;
   switch(pMsg->MsgId){
      case WM_CREATE:
                     
           if(myToast.btnFlags & TOAST_OK){
              hBtns[0]  = BUTTON_CreateEx(myToast.winRect.x1 -BTN_WIDTH -BTN_MARGIN,
                                          BTN_MARGIN,
                                          BTN_WIDTH,       BTN_HEIGHT,
                                          pMsg->hWin, WM_CF_SHOW,  0,   GUI_ID_BUTTON0);

              BUTTON_SetFont(hBtns[0],&GUI_Font_T90_30);
              BUTTON_SetFocusColor(hBtns[0],confirmWinColors[t90_set.sys.nightmode].btFocusBkColor);
              WM_SetCallback(hBtns[0], &myButtonCallback);
              WM_SetFocus(hBtns[0]);
              WM_ShowWindow (hBtns[0]);
              BUTTON_SetText(hBtns[0], "确定");

	    
	
              BUTTON_SetBkColor(hBtns[0],BUTTON_BI_UNPRESSED,confirmWinColors[t90_set.sys.nightmode].btFocusBkColor);
              BUTTON_SetTextColor(hBtns[0],BUTTON_BI_UNPRESSED,confirmWinColors[t90_set.sys.nightmode].btFocusTextColor);
              WM_ShowWindow(hBtns[0]);
           }
           
           else if(myToast.btnFlags == 0){
              myToastTimer  = WM_CreateTimer(pMsg->hWin, 0, myToast.liveTime, 0);
           }
            
           break;
           
      case WM_TIMER:
           WM_DeleteWindow(pMsg->hWin);

           if(myToast.btnFlags != 0){
              WM_SetFocus(myToast.hWin);
              myToast.btnFlags  = 0;
           }          
           myToastTimer  = 0;
           myToast.hToast  = 0;
           break;
           
      case WM_PAINT: 
           xSize = WM_GetWindowSizeX(pMsg->hWin);
           ySize = WM_GetWindowSizeY(pMsg->hWin);
           GUI_DrawGradientRoundedV(0, 0, xSize-1, ySize-1,10, confirmWinColors[t90_set.sys.nightmode].bkTopColor,confirmWinColors[t90_set.sys.nightmode].bkBottomColor);
           GUI_SetFont(&GUI_Font_T90_30);
           GUI_SetColor(confirmWinColors[t90_set.sys.nightmode].textColor);
           GUI_SetTextMode(GUI_TEXTMODE_TRANS);
           GUI_DispStringInRect(myToast.text,&pRect,GUI_TA_HCENTER | GUI_TA_VCENTER);

           
           break;
//      case WM_SET_FOCUS:
//           if(!pMsg->Data.v)
//              WM_DeleteWindow(pMsg->hWin);
//           break;
    default:
           WM_DefaultProc(pMsg);
           break;
   }
}


void ToastCreate(const char* text, const GUI_FONT GUI_UNI_PTR* pFont ,uint8_t btnFlags, uint16_t liveTime)
{
    uint16_t winWidth  = 0;
    uint16_t winHeight = 0;
    
    myToast.text  = text;
    myToast.pFont  = pFont;
    myToast.btnFlags  = btnFlags;
    myToast.liveTime  = liveTime;

    
    if(myToast.hToast)
       return ;
       

    GUI_SetFont(pFont);   
//    if(btnFlags){
//       myToast.hWin = WM_GetFocussedWindow(); 
//       if(btnFlags & TOAST_OK){
//          winHeight = BTN_HEIGHT +BTN_MARGIN *2;
//       }
//       if(btnFlags & TOAST_CANCEL){
//          winHeight = BTN_HEIGHT*2 +BTN_MARGIN *3;
//       }
//       winWidth  = GUI_GetStringDistX(text) + TEXT_MARGIN + BTN_WIDTH +BTN_MARGIN *2;
//    }
//    else{
//       winHeight  = GUI_GetFontDistY() + TEXT_MARGIN *2;
//       winWidth   = GUI_GetStringDistX(text) +TEXT_MARGIN*2;
//    }
    
    myToast.winRect.x0  = SCREEN_HCENTER - winWidth/2;
    myToast.winRect.y0  = SCREEN_VCENTER - winHeight/2  ;
    myToast.winRect.x1  = winWidth;
    myToast.winRect.y1  = winHeight;
    
    myToast.hToast  = WM_CreateWindow(SCREEN_HCENTER - 200,
                              SCREEN_VCENTER - 100,
                              400,
                              200,
                              WM_CF_SHOW,
                              &myWindowCallback, 
                              0);
    WM_SetHasTrans(myToast.hToast);
    WM_SetStayOnTop(myToast.hToast,1);
    
}


