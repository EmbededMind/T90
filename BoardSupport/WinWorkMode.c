#include "WM.h"
#include "GUI.h"
#include "BUTTON.h"
#include "layout.h"
#include "HSD_BUTTON.h"
#include "dlg.h"
//#include "Config.h"
#include "T90.h"
#include "maintask.h"
#include "t90font.h"
#include "stub.h"

/** @brief 单拖模式、双拖模式按钮 */
static WM_HWIN buttons[2];

/** @brief 模式选择窗口的句柄 */
WM_HWIN workModeWin;

static const HomeColor *pColor = homeColors;

/**@brief 模式选择界面按钮的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myButtonCallback(WM_MESSAGE* pMsg){
   WM_MESSAGE myMsg;

   switch(pMsg->MsgId){
      case WM_KEY:
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){             
              case GUI_KEY_MORIGHT:
                    if(t90_set.sys.motherpos == DEFAULT_LEFT)
                    {
                        myMsg.hWin = systemSetDlg;
                        myMsg.hWinSrc = pMsg->hWin;
                        myMsg.MsgId = USER_MSG_MOTHERPOS;
                        myMsg.Data.v = !t90_set.sys.motherpos;
                        WM_SendMessage(myMsg.hWin, &myMsg);
                    }                    
                    break;
               case GUI_KEY_LEFT:
                    if(pMsg->hWin == buttons[1]){
                       WM_SetFocus(buttons[0]);
                    }
                    break;
               case GUI_KEY_RIGHT:								
                    if(pMsg->hWin == buttons[0]){
                       WM_SetFocus(buttons[1]);
                    }
						  break;
               case GUI_KEY_ENTER:
                     if(WM_HasFocus(buttons[0]))
                     {                                
                        t90_set.sys.workmode = SINGLE_MODE;
                     }
                     if(WM_HasFocus(buttons[1]))
                     {
                        t90_set.sys.workmode = DOUBLE_MODE;
                     }
                     WM_BringToTop(mainShipWin);
                     WM_SetFocus(mainShipWin);
                     T90_Store();
                     sound = ON;
                     StubRefresh();
                     WM_SendMessageNoPara(systemSetDlg, WM_INIT_DIALOG);
                    break;
               }
           BUTTON_Callback(pMsg);
           WM_Paint(workModeWin);
           break;
           
      default :
           BUTTON_Callback(pMsg);
           break;
   }
}


/** @brief 工作模式选择窗口的回调函数
 *
 *    @param [in] pMsg  消息指针
 */
static void myWindowCallback(WM_MESSAGE* pMsg){

  switch(pMsg->MsgId){

    case WM_CREATE:
		
			      pColor = &homeColors[t90_set.sys.nightmode];
		
         buttons[0]  = BUTTON_CreateEx(0,0,1,1,pMsg->hWin, WM_CF_HIDE,  0,  GUI_ID_BUTTON0);   
         WM_SetCallback(buttons[0], &myButtonCallback);                                       
		       WM_HideWindow(buttons[0]);
         
         buttons[1]  = BUTTON_CreateEx( 0,0,1,1,pMsg->hWin, WM_CF_HIDE,  0,  GUI_ID_BUTTON1);    
         WM_SetCallback(buttons[1], &myButtonCallback);                                       

   		    WM_HideWindow(buttons[0]);
				 
         break;
         
    case WM_PAINT:
         GUI_SetBkColor(pColor->bkColor);
         GUI_Clear();
         if(WM_HasFocus(buttons[0]))
         {
            GUI_SetColor(pColor->numColor);
         }
         else 
         {
            GUI_SetColor(pColor->bbsTopColor);
         }
         GUI_FillRoundedRect(BUTTON0_X,BUTTON0_Y,BUTTON0_X+BUTTON_WIDTH-1,BUTTON0_Y+BUTTON_HEIGHT-1,8);

         GUI_SetTextMode(GUI_TM_TRANS);
         GUI_SetColor(GUI_WHITE);
         GUI_SetFont(&GUI_Font_T90_30B);
         GUI_DispStringAt("单拖模式", BUTTON0_X+43,BUTTON0_Y+8);
         if(WM_HasFocus(buttons[1]))
         {
            GUI_SetColor(pColor->numColor);
         }
         else 
         {
            GUI_SetColor(pColor->bbsTopColor);
         }
         GUI_FillRoundedRect(BUTTON1_X,BUTTON1_Y,BUTTON1_X+BUTTON_WIDTH-1,BUTTON1_Y+BUTTON_HEIGHT-1,8);
         GUI_SetColor(GUI_WHITE);
         GUI_DispStringAt("双拖模式", BUTTON1_X+43,BUTTON1_Y+8);
         break;		
		case WM_SET_FOCUS:
         if(pMsg->Data.v)
         {
             WM_SetFocus(buttons[0]);
         }
         WM_Paint(workModeWin);
         break;
         
    default:
         WM_DefaultProc(pMsg);
  }
}



/** @brief 创建单双拖模式选择窗口
 *
 *    @param [out] 所创建窗口的句柄
 */
WM_HWIN WIN_WorkModeCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindow(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WM_CF_SHOW, &myWindowCallback , 0);
   return handle;
}

