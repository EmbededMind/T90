#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "Config.h"
#include "dlg.h"
#include "HSD_BUTTON.h"
#include "maintask.h"
#include "T90.h"
#include "Check.h"
#include "t90font.h"

#include "layout_main_menu.h"

#define ID_WINDOW            (GUI_ID_USER + 0x00)

WM_HWIN mainMenuDlg;

static WM_HWIN buttons[3];


static const MenuColor *pColors = mainMenuColors;


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = {
   {WINDOW_CreateIndirect,   "clientWin", ID_WINDOW,      0, 0,                                               MAIN_MENU_WIDTH,    MAIN_MENU_HEIGHT,           0, 0, 0},
   
   {HSD_BUTTON_CreateIndirect,  "距离设置", GUI_ID_BUTTON0, MAIN_MENU_ITEM_MARGIN, MAIN_MENU_ITEM_HEIGHT+MAIN_MENU_ITEM_MARGIN*2,   MAIN_MENU_ITEM_WIDTH, MAIN_MENU_ITEM_HEIGHT,    0, 0, 0},
   {HSD_BUTTON_CreateIndirect,  "almSet", GUI_ID_BUTTON1, MAIN_MENU_ITEM_MARGIN, MAIN_MENU_ITEM_HEIGHT*2+MAIN_MENU_ITEM_MARGIN*3, MAIN_MENU_ITEM_WIDTH, MAIN_MENU_ITEM_HEIGHT,    0, 0, 0},
   {HSD_BUTTON_CreateIndirect,  "sysSet", GUI_ID_BUTTON2, MAIN_MENU_ITEM_MARGIN, MAIN_MENU_ITEM_HEIGHT*3+MAIN_MENU_ITEM_MARGIN*4, MAIN_MENU_ITEM_WIDTH, MAIN_MENU_ITEM_HEIGHT,    0, 0, 0}
};



static void myButtonCallback(WM_MESSAGE* pMsg){
   int id  = 0;
	 int i;
	 static int flag_prevfocus = 0;
   switch(pMsg->MsgId){
      case WM_SET_FOCUS:
           if(pMsg->Data.v)
					 {
							flag_prevfocus = 0;
						  for(i = 0; i < 3; i++)                      //all buttons reset to unfocussed state,
						  {
								HSD_BUTTON_SetBkColor(buttons[i], pColors->btBkColor);
							  HSD_BUTTON_SetTextColor(buttons[i], pColors->btTextColor);
						  }
							
							HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->btFocusBkColor);  //then, set the focussed button's backcolor.
              id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
              switch(id){
              case 0:
                   WM_BringToTop(dstSetMenuDlg);
									 WM_BringToTop(singleShipDstSetWin);
                   break;
							case 1:
									 WM_BringToTop(alarmSetMenuDlg);
									 WM_BringToTop(invdAlarmSetWin);
									 break;
							case 2:
									 WM_BringToTop(systemSetDlg);
									 break;
              }
           }
					 else
					 {				 
						 if(flag_prevfocus)																						//if previous focussed
						 {
								HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->btPrevFocusBkColor);   
							  HSD_BUTTON_SetTextColor(pMsg->hWin, pColors->btFocusTextColor);
						 }
						 else																													//if unfocussed
						 {
							 HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->btBkColor);
							 HSD_BUTTON_SetTextColor(pMsg->hWin, pColors->btTextColor);
						 }
					 }
           HSD_BUTTON_Callback(pMsg);
           break;
      case WM_KEY:          
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){
						 case GUI_KEY_PWM_INC:       
								 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
								 break;
              case GUI_KEY_RIGHT:
                   id  = WM_GetId(pMsg->hWin)-GUI_ID_BUTTON0;
									 flag_prevfocus = 1;
                   switch(id){
                   case 0:								
                        WM_SetFocus(dstSetMenuDlg);
                        break;
                   case 1:
												WM_SetFocus(alarmSetMenuDlg);
                        break;
                   case 2:
												WM_SetFocus(systemSetDlg);
                        break;
                   default:
                        break;
                   }
                   break;
							case GUI_KEY_BACKSPACE:
							case GUI_KEY_MENU:
									 WM_SetFocus(WM_GetDialogItem(WM_GetParent(pMsg->hWin), GUI_ID_BUTTON0));
									 if(CHECK_GetAlarmState())
									 {
										 WM_BringToTop(alarmMonitorWin);
										 WM_SetFocus(alarmMonitorWin);
									 }
									 else
									 {
										 WM_BringToTop(singleShipWin);
										 WM_SetFocus(singleShipWin);
									 }
									 break;
             default:
                   HSD_BUTTON_Callback(pMsg);
                   break;
           }
           break;
           
     default:
            HSD_BUTTON_Callback(pMsg);
            break;
   }
}




/**@brief 主菜单对话框的回调函数
 *
 *  @param [in] pMsg 消息指针
 */
static void myDialogCallBack(WM_MESSAGE* pMsg){
    switch(pMsg->MsgId){
			 
			 case USER_MSG_SKIN:
						pColors = &(mainMenuColors[pMsg->Data.v]);	
			 
						WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor); 
			 
						HSD_BUTTON_SetBkColor(buttons[0], pColors->btBkColor);
						HSD_BUTTON_SetTextColor(buttons[0], pColors->btTextColor);
						HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->btFocusTextColor);
			 
						HSD_BUTTON_SetBkColor(buttons[1], pColors->btBkColor);
						HSD_BUTTON_SetTextColor(buttons[1], pColors->btTextColor);
						HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->btFocusTextColor);
			 

						HSD_BUTTON_SetBkColor(buttons[2], pColors->btPrevFocusBkColor);
						HSD_BUTTON_SetTextColor(buttons[2], pColors->btTextColor);
						HSD_BUTTON_SetTextFocusColor(buttons[2], pColors->btFocusTextColor);
						break;
			 
       case WM_INIT_DIALOG:
						pColors = &mainMenuColors[t90_set.sys.nightmode];
            WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);           
            
						buttons[0] = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
            HSD_BUTTON_SetTxFont(buttons[0], &GUI_Font_t90_30);
            HSD_BUTTON_SetBkColor(buttons[0], pColors->btBkColor);
            WM_SetCallback(buttons[0], &myButtonCallback);
						HSD_BUTTON_SetText(buttons[0], "dstSet");
						HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->btFocusTextColor);
            
						buttons[1] = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
            HSD_BUTTON_SetTxFont(buttons[1], &GUI_Font24B_ASCII);
            HSD_BUTTON_SetBkColor(buttons[1], pColors->btBkColor);
            WM_SetCallback(buttons[1],&myButtonCallback);
						HSD_BUTTON_SetText(buttons[1], "almSet");
						HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->btFocusTextColor);
            
						buttons[2] = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
            HSD_BUTTON_SetTxFont(buttons[2], &GUI_Font24B_ASCII);
            HSD_BUTTON_SetBkColor(buttons[2], pColors->btBkColor);
            WM_SetCallback(buttons[2],&myButtonCallback);
						HSD_BUTTON_SetText(buttons[2], "sysSet");
						HSD_BUTTON_SetTextFocusColor(buttons[2], pColors->btFocusTextColor);
            
            break; 
						
			 case WM_PAINT:
						
						GUI_DrawGradientV( 0, 0,
															 MAIN_MENU_WIDTH-MAIN_MENU_ITEM_MARGIN-1, MAIN_MENU_ITEM_HEIGHT+MAIN_MENU_ITEM_MARGIN*2-1,
															 pColors->headTopColor, pColors->headBottomColor);
						GUI_SetFont(GUI_FONT_24_ASCII);
						GUI_SetTextMode(GUI_TM_TRANS);
						GUI_SetColor(pColors->headTextColor);
						GUI_DispStringAt("Menu", 40, 15);
					  GUI_SetColor(pColors->btBkColor);
						GUI_FillRect(0, MAIN_MENU_ITEM_HEIGHT*4+MAIN_MENU_ITEM_MARGIN*5, MAIN_MENU_ITEM_WIDTH-1, MAIN_MENU_HEIGHT-1);
						break;
            
       default:
            WM_DefaultProc(pMsg);
            break;
    }
}



/**@brief 创建主菜单
 *
 *  @return 所创建对话框的句柄
 */
WM_HWIN DLG_MainMenuCreate(void){
   WM_HWIN handle; 
   handle  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &myDialogCallBack, WM_HBKWIN, MAIN_MENU_X, MAIN_MENU_Y);
   return handle;
}

