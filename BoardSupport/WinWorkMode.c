#include "WM.h"
#include "GUI.h"
#include "BUTTON.h"
#include "layout.h"
#include "HSD_BUTTON.h"
#include "dlg.h"
//#include "Config.h"
#include "T90.h"
#include "maintask.h"

/** @brief 单拖模式、双拖模式按钮 */
static WM_HWIN buttons[2];

/** @brief 模式选择窗口的句柄 */
WM_HWIN workModeWin;

/**@brief 模式选择界面按钮的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myButtonCallback(WM_MESSAGE* pMsg){

   switch(pMsg->MsgId){
      case WM_SET_FOCUS:
           if(pMsg->Data.v){
              BUTTON_SetBkColor(pMsg->hWin, BUTTON_CI_UNPRESSED, GUI_BLUE);
           }
           else{
              BUTTON_SetBkColor(pMsg->hWin, BUTTON_CI_UNPRESSED, GUI_GRAY);
           }
           BUTTON_Callback(pMsg);
           break;
      
      case WM_KEY:
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){
							case GUI_KEY_PWM_INC:       
									 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
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
											WM_BringToTop(singleShipWin);
											WM_SetFocus(singleShipWin);
											t90_set.sys.workmode = SINGLE_MODE;
										}
										if(WM_HasFocus(buttons[1]))
										{
											WM_BringToTop(doubleShipWin);
											WM_SetFocus(doubleShipWin);
											t90_set.sys.workmode = DOUBLE_MODE;
										}
										
										T90_Store();

										WM_SendMessageNoPara(systemSetDlg, WM_INIT_DIALOG);
                    break;
            }
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
         buttons[0]  = BUTTON_CreateEx(SCREEN_WIDTH/4 - LAYOUT_WORKMODE_BUTTON_WIDTH/2,
                                       SCREEN_HEIGHT/2 - LAYOUT_WORKMODE_BUTTON_HEIGHT/2, 
                                       LAYOUT_WORKMODE_BUTTON_WIDTH, 
                                       LAYOUT_WORKMODE_BUTTON_HEIGHT, 
                                       pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
         WM_SetCallback(buttons[0], &myButtonCallback);                                       
         BUTTON_SetText(buttons[0], "single");
         BUTTON_SetBkColor(buttons[0], BUTTON_CI_UNPRESSED,GUI_GRAY);
         BUTTON_SetBkColor(buttons[0], BUTTON_CI_PRESSED, GUI_LIGHTBLUE);
         BUTTON_SetTextColor(buttons[0], BUTTON_CI_UNPRESSED, GUI_LIGHTGRAY);
         
         buttons[1]  = BUTTON_CreateEx(SCREEN_WIDTH -SCREEN_WIDTH/4 - LAYOUT_WORKMODE_BUTTON_WIDTH/2,
                                       SCREEN_HEIGHT/2 - LAYOUT_WORKMODE_BUTTON_HEIGHT/2, 
                                       LAYOUT_WORKMODE_BUTTON_WIDTH, 
                                       LAYOUT_WORKMODE_BUTTON_HEIGHT, 
                                       pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON1);    
         WM_SetCallback(buttons[1], &myButtonCallback);                                       
         BUTTON_SetText(buttons[1], "double");
         BUTTON_SetBkColor(buttons[1], BUTTON_CI_UNPRESSED,GUI_GRAY);
         BUTTON_SetBkColor(buttons[1], BUTTON_CI_PRESSED, GUI_LIGHTBLUE);
         BUTTON_SetTextColor(buttons[1], BUTTON_CI_UNPRESSED, GUI_LIGHTGRAY);
				 
				 WM_DefaultProc(pMsg);
         break;
         
    case WM_PAINT:
         GUI_SetBkColor(GUI_LIGHTGRAY);
         GUI_Clear();
         break;
		
		case WM_SET_FOCUS:
				 if(pMsg->Data.v)
				 {
						WM_SetFocus(buttons[0]);
				 }

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

