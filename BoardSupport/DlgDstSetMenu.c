#include "maintask.h"
#include "GUI.h"
#include "DIALOG.h"
#include "dlg.h"
#include "T90.h"
#include "Check.h"
#include "t90font.h"
#include "BUTTON.h"
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
   {WINDOW_CreateIndirect,     "clientWin",     ID_WINDOW,      0, 0,                                            SUB_MENU_WIDTH,    SUB_MENU_HEIGHT,      0, 0, 0},
   
//   {HSD_BUTTON_CreateIndirect, "safety sign 0", GUI_ID_BUTTON0, 0, SUB_MENU_ITEM_HEIGHT+  SUB_MENU_ITEM_MARGIN*2, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
//   {HSD_BUTTON_CreateIndirect, "safety sign 1", GUI_ID_BUTTON1, 0, SUB_MENU_ITEM_HEIGHT*2+SUB_MENU_ITEM_MARGIN*3, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
//   {HSD_BUTTON_CreateIndirect, "safety sign 2", GUI_ID_BUTTON2, 0, SUB_MENU_ITEM_HEIGHT*3+SUB_MENU_ITEM_MARGIN*4, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0}
   {BUTTON_CreateIndirect, "safety sign 0", GUI_ID_BUTTON0, 0, SUB_MENU_ITEM_HEIGHT+  SUB_MENU_ITEM_MARGIN*2, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {BUTTON_CreateIndirect, "safety sign 1", GUI_ID_BUTTON1, 0, SUB_MENU_ITEM_HEIGHT*2+SUB_MENU_ITEM_MARGIN*3, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0},
   {BUTTON_CreateIndirect, "safety sign 2", GUI_ID_BUTTON2, 0, SUB_MENU_ITEM_HEIGHT*3+SUB_MENU_ITEM_MARGIN*4, SUB_MENU_ITEM_WIDTH, SUB_MENU_ITEM_HEIGHT, 0, 0, 0}
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
           id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
           flag_prevfocus  = 0;
           
           BUTTON_SetBkColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors->btFocusBkColor);
           BUTTON_SetTextColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors->btFocusTextColor);
           
           
           if(t90_set.sys.workmode == SINGLE_MODE)
           {
              myMsg.hWin = singleShipDstSetWin;  
           }
           else
           {
              myMsg.hWin = doubleShipDstSetWin;
           } 
           myMsg.hWinSrc = pMsg->hWin;
           myMsg.MsgId = USER_MSG_DST_SET;
           myMsg.Data.v = id;
           WM_SendMessage(myMsg.hWin, &myMsg);

        }     
        else
        {
           if(flag_prevfocus)
           {
              BUTTON_SetBkColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors->btPrevFocusBkColor);
              BUTTON_SetTextColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors->btFocusTextColor);
           }
           else
           {
              BUTTON_SetBkColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors->btBkColor);
              BUTTON_SetTextColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors->btTextColor);
           }
        }
        BUTTON_Callback(pMsg);
        break;
           
   case WM_KEY:
        switch( ((WM_KEY_INFO*)(pMsg->Data.p))->Key){
        case GUI_KEY_PWM_INC:       
             WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
             break;
        case GUI_KEY_RIGHT:
             flag_prevfocus = 1;
             id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
             if(t90_set.sys.workmode == SINGLE_MODE  &&  singleShipDstSetWin)
             {		
                WM_SetFocus(WM_GetDialogItem(singleShipDstSetWin,ID_DMS_0+id*2));
             }   
             else if(t90_set.sys.workmode == DOUBLE_MODE &&  doubleShipDstSetWin){
                WM_MESSAGE myMsg;
                myMsg.hWin  = doubleShipDstSetWin;
                myMsg.Data.v  = id;
                myMsg.MsgId  = USER_MSG_FOCUS;
                WM_SendMessage(myMsg.hWin, &myMsg);
             }
             break;
						  case GUI_KEY_UP:
             GUI_StoreKeyMsg(GUI_KEY_BACKTAB, 1);
             break;
             
        case GUI_KEY_DOWN:
             GUI_StoreKeyMsg(GUI_KEY_TAB, 1);
             break;
        
        case GUI_KEY_BACKSPACE:
             WM_SetFocus(buttons[0]);
             WM_SetFocus(mainMenuDlg);
          
             
             myMsg.hWinSrc = pMsg->hWin;
             myMsg.MsgId = USER_MSG_DST_SET;
             myMsg.Data.v = -1;
             if(t90_set.sys.workmode == SINGLE_MODE)
             {
                myMsg.hWin = singleShipDstSetWin;  
             }
             else
             {
                myMsg.hWin = doubleShipDstSetWin;
             }            
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
             BUTTON_Callback(pMsg);
             break;        
        }
        break;        
        
   default:
        BUTTON_Callback(pMsg);
        break;
   }
}


static void myDialogCallback(WM_MESSAGE* pMsg)
{
   switch(pMsg->MsgId){

		 case USER_MSG_SKIN:
        pColors = &(subMenuColors[pMsg->Data.v]);	
      
        WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor); 
      
        BUTTON_SetBkColor(buttons[0], BUTTON_CI_UNPRESSED, pColors->btBkColor);
        BUTTON_SetTextColor(buttons[0], BUTTON_CI_UNPRESSED, pColors->btTextColor);
        BUTTON_SetTextColor(buttons[0], BUTTON_CI_PRESSED,   pColors->btTextColor);
        BUTTON_SetFocusColor(buttons[0], pColors->btFocusBkColor);
        
        BUTTON_SetBkColor(buttons[1], BUTTON_CI_UNPRESSED, pColors->btBkColor);
        BUTTON_SetTextColor(buttons[1], BUTTON_CI_UNPRESSED, pColors->btTextColor);
        BUTTON_SetTextColor(buttons[1], BUTTON_CI_PRESSED,   pColors->btTextColor);
        BUTTON_SetFocusColor(buttons[1], pColors->btFocusBkColor);
        
        BUTTON_SetBkColor(buttons[2], BUTTON_CI_UNPRESSED, pColors->btBkColor);
        BUTTON_SetTextColor(buttons[2], BUTTON_CI_UNPRESSED, pColors->btTextColor);
        BUTTON_SetTextColor(buttons[2], BUTTON_CI_PRESSED,   pColors->btTextColor);     
        BUTTON_SetFocusColor(buttons[2], pColors->btFocusBkColor);
        break;
		 case WM_INIT_DIALOG:
       
        pColors = &subMenuColors[t90_set.sys.nightmode];
        
        WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
      
        buttons[0]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
        WM_SetCallback(buttons[0], &myButtonCallback);
        BUTTON_SetBkColor(buttons[0], BUTTON_CI_UNPRESSED, pColors->btBkColor);
        BUTTON_SetBkColor(buttons[0], BUTTON_CI_PRESSED,   pColors->btFocusBkColor);
        BUTTON_SetTextColor(buttons[0], BUTTON_CI_UNPRESSED, pColors->btTextColor);
        BUTTON_SetTextAlign(buttons[0], GUI_TA_HCENTER | GUI_TA_VCENTER);
        BUTTON_SetTextColor(buttons[0], BUTTON_CI_PRESSED, pColors->btFocusTextColor);
        BUTTON_SetFocusColor(buttons[0], pColors->btFocusBkColor);
        BUTTON_SetFont(buttons[0], &GUI_Font_T90_30); 
        BUTTON_SetText(buttons[0], "01号安全标");
        
        buttons[1]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
        WM_SetCallback(buttons[1], &myButtonCallback);
        BUTTON_SetBkColor(buttons[1], BUTTON_CI_UNPRESSED, pColors->btBkColor);
        BUTTON_SetBkColor(buttons[1], BUTTON_CI_PRESSED,   pColors->btFocusBkColor);
        BUTTON_SetTextColor(buttons[1], BUTTON_CI_UNPRESSED, pColors->btTextColor);
        BUTTON_SetTextColor(buttons[1], BUTTON_CI_PRESSED, pColors->btFocusTextColor);
        BUTTON_SetTextAlign(buttons[1], GUI_TA_HCENTER | GUI_TA_VCENTER);
        BUTTON_SetFocusColor(buttons[0], pColors->btFocusBkColor);
        BUTTON_SetFont(buttons[1], &GUI_Font_T90_30); 
        BUTTON_SetText(buttons[1], "02号安全标");
        BUTTON_SetFocussable(buttons[1], 1);
        
        buttons[2]  = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
        WM_SetCallback(buttons[2], &myButtonCallback);
        BUTTON_SetBkColor(buttons[2], BUTTON_CI_UNPRESSED, pColors->btBkColor);
        BUTTON_SetBkColor(buttons[2], BUTTON_CI_PRESSED,   pColors->btFocusBkColor);
        BUTTON_SetTextColor(buttons[2], BUTTON_CI_UNPRESSED, pColors->btTextColor);
        BUTTON_SetTextColor(buttons[2], BUTTON_CI_PRESSED, pColors->btFocusTextColor);
        BUTTON_SetTextAlign(buttons[2], GUI_TA_HCENTER | GUI_TA_VCENTER);
        BUTTON_SetFocusColor(buttons[2], pColors->btFocusBkColor);
        BUTTON_SetFont(buttons[2], &GUI_Font_T90_30); 
        BUTTON_SetText(buttons[2], "03号安全标");
        break; 

		 case WM_PAINT:			 
        GUI_DrawGradientV( 0, 0,
        SUB_MENU_ITEM_WIDTH-1, SUB_MENU_ITEM_HEIGHT+MAIN_MENU_ITEM_MARGIN*2-1,
        pColors->headTopColor, pColors->headBottomColor);
        GUI_SetFont(&GUI_Font_T90_30);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetColor(pColors->headTextColor);
        GUI_DispStringAt("距离设置", 80, 9);
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
