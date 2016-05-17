#include "maintask.h"
#include "GUI.h"
#include "DIALOG.h"
#include "HSD_BUTTON.h"
#include "dlg.h"
#include "T90.h"
#include "Check.h"

#include "layout_sub_menu.h"
#include "layout_dst_set.h"

#define ID_WINDOW           (GUI_ID_USER + 0x00)

#define ID_BUTTON_0         (GUI_ID_USER + 0x10)
#define ID_BUTTON_1         (GUI_ID_USER + 0x11)
#define ID_BUTTON_2         (GUI_ID_USER + 0x12) 

WM_HWIN dstSetMenuDlg;

static WM_HWIN buttons[3];

static  const MenuColor *pColors = subMenuColors;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = {
   {WINDOW_CreateIndirect,     "clientWin",     ID_WINDOW,      0, 0,                                             SUB_MENU_WIDTH,    SUB_MENU_HEIGHT,      0, 0, 0},
   
   {HSD_BUTTON_CreateIndirect, "safety sign 0", GUI_ID_BUTTON0, 0, SUB_MENU_ITEM_HEIGHT+  SUB_MENU_ITEM_MARGIN*2, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {HSD_BUTTON_CreateIndirect, "safety sign 1", GUI_ID_BUTTON1, 0, SUB_MENU_ITEM_HEIGHT*2+SUB_MENU_ITEM_MARGIN*3, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {HSD_BUTTON_CreateIndirect, "safety sign 2", GUI_ID_BUTTON2, 0, SUB_MENU_ITEM_HEIGHT*3+SUB_MENU_ITEM_MARGIN*4, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0}
};


static void myButtonCallback(WM_MESSAGE* pMsg)
{
	int id;
	WM_MESSAGE myMsg;
	static int flag_prevfocus = 0;
   switch(pMsg->MsgId){
   case WM_SET_FOCUS:
        if(pMsg->Data.v)
				{
					 flag_prevfocus=0;
           id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
           HSD_BUTTON_SetBkColor(pMsg->hWin,pColors->btFocusBkColor);
					
					myMsg.hWin = singleShipDstSetWin;
					myMsg.hWinSrc = pMsg->hWin;
					myMsg.MsgId = USER_MSG_DST_SET;
					myMsg.Data.v = id;
					WM_SendMessage(myMsg.hWin, &myMsg);
        }     
        else
				{
					if(flag_prevfocus)
					{
						HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->btPrevFocusBkColor);
						HSD_BUTTON_SetTextColor(pMsg->hWin, pColors->btFocusTextColor);
					}
					else
					{
            HSD_BUTTON_SetBkColor(pMsg->hWin,pColors->btBkColor);
						HSD_BUTTON_SetTextColor(pMsg->hWin, pColors->btTextColor);
					}
        }
        HSD_BUTTON_Callback(pMsg);
        break;
           
   case WM_KEY:
        switch( ((WM_KEY_INFO*)(pMsg->Data.p))->Key){
				case GUI_KEY_PWM_INC:       
						 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
						 break;
        case GUI_KEY_RIGHT:
						 flag_prevfocus = 1;
				     id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
						 if(singleShipDstSetWin)
						 {		
								WM_SetFocus(WM_GetDialogItem(singleShipDstSetWin,ID_DMS_0+id*2));
						 }          
             break;
						 
				case GUI_KEY_BACKSPACE:
						 WM_SetFocus(buttons[0]);
						 WM_SetFocus(mainMenuDlg);
				
						 myMsg.hWin = singleShipDstSetWin;
						 myMsg.hWinSrc = pMsg->hWin;
						 myMsg.MsgId = USER_MSG_DST_SET;
						 myMsg.Data.v = -1;
						 WM_SendMessage(myMsg.hWin, &myMsg);
						 break;
				
				case GUI_KEY_MENU:
						 WM_SetFocus(buttons[0]);
						 WM_SetFocus(WM_GetDialogItem(mainMenuDlg, GUI_ID_BUTTON0));
				
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
							 
						 myMsg.hWin = singleShipDstSetWin;
						 myMsg.hWinSrc = pMsg->hWin;
						 myMsg.MsgId = USER_MSG_DST_SET;
						 myMsg.Data.v = -1;
						 WM_SendMessage(myMsg.hWin, &myMsg);
						 break;
				
        default:
             HSD_BUTTON_Callback(pMsg);
             break;        
        }
        break;        
        
   default:
        HSD_BUTTON_Callback(pMsg);
   }
}


static void myDialogCallback(WM_MESSAGE* pMsg)
{
   switch(pMsg->MsgId){

		 case USER_MSG_SKIN:
					pColors = &(subMenuColors[pMsg->Data.v]);	
		 
					WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor); 
		 
					HSD_BUTTON_SetBkColor(buttons[0], pColors->btBkColor);
					HSD_BUTTON_SetTextColor(buttons[0], pColors->btTextColor);
					HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->btFocusTextColor);
		 
					HSD_BUTTON_SetBkColor(buttons[1], pColors->btBkColor);
					HSD_BUTTON_SetTextColor(buttons[1], pColors->btTextColor);
					HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->btFocusTextColor);
		 

					HSD_BUTTON_SetBkColor(buttons[2], pColors->btBkColor);
					HSD_BUTTON_SetTextColor(buttons[2], pColors->btTextColor);
					HSD_BUTTON_SetTextFocusColor(buttons[2], pColors->btFocusTextColor);
					break;
		 case WM_INIT_DIALOG:
			 
					pColors = &subMenuColors[t90_set.sys.nightmode];
					
					WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
		 
					buttons[0]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
					WM_SetCallback(buttons[0], &myButtonCallback);
					HSD_BUTTON_SetBkColor(buttons[0], pColors->btBkColor);
	//        HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);       //这句好像并没有起作用
					HSD_BUTTON_SetTextColor(buttons[0], pColors->btTextColor);           //默认值：BLACK，可以在buttoncb里面改变
					HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->btFocusTextColor);    //默认值：BLACK，不能在buttoncb里面改变
					HSD_BUTTON_SetTxFont(buttons[0], &GUI_Font24_ASCII);
					HSD_BUTTON_SetText(buttons[0], "safety sign 0");
					
					buttons[1]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
					WM_SetCallback(buttons[1], &myButtonCallback);
					HSD_BUTTON_SetBkColor(buttons[1], pColors->btBkColor);
	//      HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);
					HSD_BUTTON_SetTextColor(buttons[1], pColors->btTextColor);             
					HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->btFocusTextColor);
					HSD_BUTTON_SetTxFont(buttons[1], &GUI_Font24_ASCII);
					HSD_BUTTON_SetText(buttons[1], "safety sign 1");
					
					buttons[2]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
					WM_SetCallback(buttons[2], &myButtonCallback);
					HSD_BUTTON_SetBkColor(buttons[2], pColors->btBkColor);
	//        HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);
					HSD_BUTTON_SetTextColor(buttons[2], pColors->btTextColor);
					HSD_BUTTON_SetTextFocusColor(buttons[2], pColors->btFocusTextColor);
					HSD_BUTTON_SetTxFont(buttons[2], &GUI_Font24_ASCII);
					HSD_BUTTON_SetText(buttons[2], "safety sign 2");
					
//					BUTTON_SetFocussable(buttons[1], 0);  //有用！！！！
					break;  
					
		 case WM_SET_FOCUS:
//					WM_DisableWindow(buttons[0]);
//					BUTTON_SetFocussable(buttons[1], 0);
					break;

		 case WM_PAINT:
			 
					GUI_DrawGradientV( 0, 0,
														 SUB_MENU_ITEM_WIDTH-1, SUB_MENU_ITEM_HEIGHT+MAIN_MENU_ITEM_MARGIN*2-1,
														 pColors->headTopColor, pColors->headBottomColor);
					GUI_SetFont(GUI_FONT_24_ASCII);
					GUI_SetTextMode(GUI_TM_TRANS);
					GUI_SetColor(pColors->headTextColor);
					GUI_DispStringAt("dstSet", 92, 15);
					GUI_SetColor(pColors->btBkColor);
					GUI_FillRect(0, SUB_MENU_ITEM_HEIGHT*4+SUB_MENU_ITEM_MARGIN*5, SUB_MENU_WIDTH-1, SUB_MENU_HEIGHT-1);
					break;	
				
		 default:
					WM_DefaultProc(pMsg);
					break;
		} 
}


WM_HWIN DLG_DstSetMenuCreate(void)
{
   WM_HWIN handle;
   
   handle  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &myDialogCallback, WM_HBKWIN ,SUB_MENU_X, SUB_MENU_Y);
   return handle;
}
