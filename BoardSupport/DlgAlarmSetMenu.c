#include "maintask.h"
#include "GUI.h"
#include "DIALOG.h"
#include "HSD_BUTTON.h"
#include "dlg.h"
#include "T90.h"
#include "Check.h"
#include "t90font.h"

#include "layout_sub_menu.h"


#define ID_WINDOW           (GUI_ID_USER + 0x00)

//#define ID_BUTTON_0         (GUI_ID_USER + 0x10)
//#define ID_BUTTON_1         (GUI_ID_USER + 0x11)
//#define ID_BUTTON_2         (GUI_ID_USER + 0x12) 
//#define ID_BUTTON_3         (GUI_ID_USER + 0x13) 
//#define ID_BUTTON_4         (GUI_ID_USER + 0x14) 

WM_HWIN alarmSetMenuDlg;

static const MenuColor *pColors = subMenuColors;

static WM_HWIN buttons[5];

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = {
   {WINDOW_CreateIndirect,    "clientWin",    ID_WINDOW,      0, 0,                                             SUB_MENU_WIDTH,      SUB_MENU_HEIGHT,      0, 0, 0},
   
   {HSD_BUTTON_CreateIndirect, "invade",            GUI_ID_BUTTON0, 0, SUB_MENU_ITEM_HEIGHT+  SUB_MENU_ITEM_MARGIN*2, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {HSD_BUTTON_CreateIndirect, "speeding",          GUI_ID_BUTTON1, 0, SUB_MENU_ITEM_HEIGHT*2+SUB_MENU_ITEM_MARGIN*3, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {HSD_BUTTON_CreateIndirect, "sog range",         GUI_ID_BUTTON2, 0, SUB_MENU_ITEM_HEIGHT*3+SUB_MENU_ITEM_MARGIN*4, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
	{HSD_BUTTON_CreateIndirect, "special",           GUI_ID_BUTTON3, 0, SUB_MENU_ITEM_HEIGHT*4+SUB_MENU_ITEM_MARGIN*5, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {HSD_BUTTON_CreateIndirect, "highspeedship",     GUI_ID_BUTTON4, 0, SUB_MENU_ITEM_HEIGHT*5+SUB_MENU_ITEM_MARGIN*6, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0}, 
};


static void myButtonCallback(WM_MESSAGE* pMsg)
{
   WM_MESSAGE myMsg;
	int id;
	static int flag_prevfocus = 0;
   switch(pMsg->MsgId){
   case WM_SET_FOCUS:
        if(pMsg->Data.v)
				{
					flag_prevfocus = 0;
          id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
//INFO("id:%d",id);				
					 switch(id)
					{
						case 0:
							WM_BringToTop(invdAlarmSetWin);
							break;
						case 1:
							WM_BringToTop(spdingAlarmSetWin);
							break;
						case 2:
							WM_BringToTop(sogAlarmSetWin);
							break;
						case 3:
							WM_BringToTop(specialAlarmSetDlg);
							break;
                  case 4:
                     WM_BringToTop(highspshipsetWin);;
                     break;
					}
           HSD_BUTTON_SetBkColor(pMsg->hWin,pColors->btFocusBkColor);
        }     
        else
				{
//INFO("flag_prevfocus:%d",flag_prevfocus);					
					if(flag_prevfocus)
					{
						HSD_BUTTON_SetBkColor(pMsg->hWin,pColors->btPrevFocusBkColor);
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
           case GUI_KEY_MOLEFT:
                        if(t90_set.sys.motherpos == DEFAULT_RIGHT && t90_set.sys.workmode == DOUBLE_MODE)
                        {
                           myMsg.hWin = systemSetDlg;
                           myMsg.hWinSrc = pMsg->hWin;
                           myMsg.MsgId = USER_MSG_MOTHERPOS;
                           myMsg.Data.v = DEFAULT_LEFT;
                           WM_SendMessage(myMsg.hWin, &myMsg);
                        }                           
                        break;
              
              case GUI_KEY_MORIGHT:
                        if(t90_set.sys.motherpos == DEFAULT_LEFT && t90_set.sys.workmode == DOUBLE_MODE)
                        {
                           myMsg.hWin = systemSetDlg;
                           myMsg.hWinSrc = pMsg->hWin;
                           myMsg.MsgId = USER_MSG_MOTHERPOS;
                           myMsg.Data.v = DEFAULT_RIGHT;
                           WM_SendMessage(myMsg.hWin, &myMsg);
                        }   
                        break; 
                  case GUI_KEY_SINGLE:
                         if(t90_set.sys.workmode == DOUBLE_MODE)
                         {                            
                            myMsg.hWin = systemSetDlg;
                            myMsg.hWinSrc = pMsg->hWin;
                            myMsg.MsgId = USER_MSG_WORKMODE;
                            myMsg.Data.v = SINGLE_MODE;
                            WM_SendMessage(myMsg.hWin, &myMsg);
                         }
                         
                         break;
                  case GUI_KEY_DOUBLE:
                         if(t90_set.sys.workmode == SINGLE_MODE)
                         {
                            myMsg.hWin = systemSetDlg;
                            myMsg.hWinSrc = pMsg->hWin;
                            myMsg.MsgId = USER_MSG_WORKMODE;
                            myMsg.Data.v = DOUBLE_MODE;
                            WM_SendMessage(myMsg.hWin, &myMsg);
                         }
                         
                         break;
				case GUI_KEY_PWM_INC:       
						 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
						 break;
				
        case GUI_KEY_RIGHT:
					flag_prevfocus = 1;
          id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;			
					switch(id)
					{
						case 0:
							WM_SetFocus(invdAlarmSetWin);
							break;
						case 1:
							WM_SetFocus(spdingAlarmSetWin);
							break;
						case 2:
							WM_SetFocus(sogAlarmSetWin);
							break;
						case 3:
							WM_SetFocus(specialAlarmSetDlg);						
							break;
                  case 4:
                     break;
					}
          break;
				case GUI_KEY_BACKSPACE:
						 WM_SetFocus(buttons[0]);
						 WM_SetFocus(mainMenuDlg);
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
							 WM_BringToTop(mainShipWin);
							 WM_SetFocus(mainShipWin);
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
	 
						HSD_BUTTON_SetBkColor(buttons[3], pColors->btBkColor);
						HSD_BUTTON_SetTextColor(buttons[3], pColors->btTextColor);
						HSD_BUTTON_SetTextFocusColor(buttons[3], pColors->btFocusTextColor);
    
                  HSD_BUTTON_SetBkColor(buttons[4], pColors->btBkColor);
						HSD_BUTTON_SetTextColor(buttons[4], pColors->btTextColor);
						HSD_BUTTON_SetTextFocusColor(buttons[4], pColors->btFocusTextColor);
						break;
   case WM_INIT_DIALOG:
		 
				pColors = &(subMenuColors[t90_set.sys.nightmode]);
        WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
        
        buttons[0]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
        WM_SetCallback(buttons[0], &myButtonCallback);
        HSD_BUTTON_SetBkColor(buttons[0], pColors->btBkColor);
//        HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);       //这句好像并没有起作用
        HSD_BUTTON_SetTextColor(buttons[0], pColors->btTextColor);           //默认值：BLACK，可以在buttoncb里面改变
        HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->btFocusTextColor);    //默认值：BLACK，不能在buttoncb里面改变
        HSD_BUTTON_SetTxFont(buttons[0], &GUI_Font_T90_30);
        HSD_BUTTON_SetText(buttons[0], "闯入报警");
        
        buttons[1]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
        WM_SetCallback(buttons[1], &myButtonCallback);
        HSD_BUTTON_SetBkColor(buttons[1], pColors->btBkColor);
//      HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);
        HSD_BUTTON_SetTextColor(buttons[1], pColors->btTextColor);             
        HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->btFocusTextColor);
        HSD_BUTTON_SetTxFont(buttons[1], &GUI_Font_T90_30);
        HSD_BUTTON_SetText(buttons[1], "超速报警");
        
        buttons[2]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
        WM_SetCallback(buttons[2], &myButtonCallback);
        HSD_BUTTON_SetBkColor(buttons[2], pColors->btBkColor);
//        HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);
        HSD_BUTTON_SetTextColor(buttons[2], pColors->btTextColor);
        HSD_BUTTON_SetTextFocusColor(buttons[2], pColors->btFocusTextColor);
        HSD_BUTTON_SetTxFont(buttons[2], &GUI_Font_T90_30);
        HSD_BUTTON_SetText(buttons[2], "航速报警");
				
				buttons[3]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);
        WM_SetCallback(buttons[3], &myButtonCallback);
        HSD_BUTTON_SetBkColor(buttons[3], pColors->btBkColor);
//        HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);
        HSD_BUTTON_SetTextColor(buttons[3], pColors->btTextColor);
        HSD_BUTTON_SetTextFocusColor(buttons[3], pColors->btFocusTextColor);
        HSD_BUTTON_SetTxFont(buttons[3], &GUI_Font_T90_30);
        HSD_BUTTON_SetText(buttons[3], "特殊报警");
        
        buttons[4]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON4);
        WM_SetCallback(buttons[4], &myButtonCallback);
        HSD_BUTTON_SetBkColor(buttons[4], pColors->btBkColor);
//        HSD_BUTTON_SetFocusBkColor(handle, GUI_BLUE);
        HSD_BUTTON_SetTextColor(buttons[4], pColors->btTextColor);
        HSD_BUTTON_SetTextFocusColor(buttons[4], pColors->btFocusTextColor);
        HSD_BUTTON_SetTxFont(buttons[4], &GUI_Font_T90_30);
        HSD_BUTTON_SetText(buttons[4], "高速船报警");
        break;
	 					
	 case WM_PAINT:
		 
				GUI_DrawGradientV( 0, 0,
													 SUB_MENU_ITEM_WIDTH-1, SUB_MENU_ITEM_HEIGHT+MAIN_MENU_ITEM_MARGIN*2-1,
													 pColors->headTopColor, pColors->headBottomColor);
				GUI_SetFont(&GUI_Font_T90_30);
				GUI_SetTextMode(GUI_TM_TRANS);
				GUI_SetColor(pColors->headTextColor);
				GUI_DispStringAt("报警设置", 80, 9);
				GUI_SetColor(pColors->btBkColor);
				GUI_FillRect(0, SUB_MENU_ITEM_HEIGHT*6+SUB_MENU_ITEM_MARGIN*7, SUB_MENU_WIDTH-1, SUB_MENU_HEIGHT-1);
				break;				
      
   default:
        WM_DefaultProc(pMsg);
   } 
}


WM_HWIN DLG_AlarmSetMenuCreate(void)
{
   WM_HWIN handle;
   
   handle  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &myDialogCallback, WM_HBKWIN ,SUB_MENU_X, SUB_MENU_Y);
   return handle;
}
