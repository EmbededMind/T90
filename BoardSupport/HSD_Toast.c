#include "WM.h"
#include "GUI.h"
#include <BUTTON.h>
#include "layout.h"
#include "HSD_Toast.h"
#include <lpc_types.h>

#define BTN_WIDTH   80
#define BTN_HEIGHT  40

#define BTN_MARGIN  10
#define TEXT_MARGIN  5



typedef struct {

   const GUI_FONT GUI_UNI_PTR* pFont;
   const char*                 text;
   uint16_t                    liveTime;
   uint8_t                     btnFlags;
   uint8_t                     isAlive;
   WM_HWIN                     hWin;
   GUI_RECT                    winRect;
}Toast;


static Toast myToast;
static WM_HWIN hBtns[2];
static WM_HTIMER myToastTimer;
static WM_HWIN hToast ;

static void myButtonCallback(WM_MESSAGE* pMsg)
{
   switch(pMsg->MsgId){
      case WM_KEY:
           if( ((WM_KEY_INFO*)pMsg->Data.p)->Key == GUI_KEY_ENTER)    {
              WM_DeleteWindow(WM_GetParent(pMsg->hWin));
              WM_SetFocus(myToast.hWin);
              hBtns[0]  = 0;
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
   
   switch(pMsg->MsgId){
      case WM_CREATE:     
           if(myToast.btnFlags & TOAST_OK){
              hBtns[0]  = BUTTON_CreateEx(myToast.winRect.x1 -BTN_WIDTH -BTN_MARGIN,
                                          BTN_MARGIN,
                                          BTN_WIDTH,       BTN_HEIGHT,
                                          pMsg->hWin, WM_CF_SHOW,  0,   GUI_ID_BUTTON0);

              BUTTON_SetFont(hBtns[0],&GUI_Font16_1);
              BUTTON_SetBkColor(hBtns[0],BUTTON_CI_UNPRESSED,GUI_GRAY);
              BUTTON_SetTextColor(hBtns[0],BUTTON_CI_UNPRESSED,GUI_RED);
              BUTTON_SetText(hBtns[0],"OK");
              WM_SetCallback(hBtns[0], &myButtonCallback);
              WM_SetFocus(hBtns[0]);
           }
           
           else if(myToast.btnFlags == 0){
              myToastTimer  = WM_CreateTimer(pMsg->hWin, 0, myToast.liveTime, 0);
           }
           break;
           
      case WM_TIMER:
           WM_DeleteWindow(pMsg->hWin);
//           WM_SetFocus(myToast.hWin);
           myToastTimer  = 0;
           break;
           
      case WM_PAINT:
           GUI_SetBkColor(GUI_WHITE);
           GUI_Clear();
           WM_GetClientRect(&clientRect);
           GUI_SetColor(GUI_BLACK);
           GUI_SetFont(myToast.pFont);
           GUI_DispStringAt(myToast.text, clientRect.x0+TEXT_MARGIN, (clientRect.y0+clientRect.y1) /2 -GUI_GetFontDistY()/2);
           break;
           
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
   if(btnFlags)
       myToast.hWin = WM_GetFocussedWindow();    
    GUI_SetFont(pFont);   
    if(btnFlags){
       if(btnFlags & TOAST_OK){
          winHeight = BTN_HEIGHT +BTN_MARGIN *2;
       }
       if(btnFlags & TOAST_CANCEL){
          winHeight = BTN_HEIGHT*2 +BTN_MARGIN *3;
       }
       winWidth  = GUI_GetStringDistX(text) + TEXT_MARGIN + BTN_WIDTH +BTN_MARGIN *2;
    }
    else{
       winHeight  = GUI_GetFontDistY() + TEXT_MARGIN *2;
       winWidth   = GUI_GetStringDistX(text) +TEXT_MARGIN*2;
    }
    
    myToast.winRect.x0  = SCREEN_HCENTER - winWidth/2;
    myToast.winRect.y0  = SCREEN_VCENTER - winHeight/2  ;
    myToast.winRect.x1  = winWidth;
    myToast.winRect.y1  = winHeight;
    
    hToast  = WM_CreateWindow(SCREEN_HCENTER - winWidth/2,
                              SCREEN_VCENTER - winHeight/2,
                              winWidth,
                              winHeight,
                              WM_CF_SHOW,
                              &myWindowCallback, 
                              0);
    WM_SetStayOnTop(hToast,1);
}


