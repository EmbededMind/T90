
/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.24                          *
*        Compiled Jan 27 2014, 11:28:24                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "MainTask.h"
#include "DIALOG.h"
#include "Config.h"
#include "WM.h"
#include "sysConf.h"
#include "dlg.h"
#include "skinColor.h"
#include "HSD_BUTTON.h"
#include "28.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_BUTTON_2 (GUI_ID_USER + 0x03)
#define ID_BUTTON_3 (GUI_ID_USER + 0x04)

#ifdef P_AM128A
#define ID_BUTTON_4  (GUI_ID_USER + 0x05)

#define MENU_ITEM_NUM  5

#else

#define MENU_ITEM_NUM  4

#endif



/*---------------------------- Global variables -------------------------------*/
WM_HWIN menuWin;



static const MenuWin_COLOR * pSkin  = &menuWinSkins[0];


/*********************************************************************
*
*       Static data
*
**********************************************************************/
static WM_HWIN hButtons[MENU_ITEM_NUM];


static void myButtonListener(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, MenuLabel_WIDTH, 480, 0, 0x0, 0 },
  
//  { TEXT_CreateIndirect,   "MainMenu", ID_TEXT_0, 0, 10, MenuLabel_WIDTH, 40, 0, 0, 0},
  
  { HSD_BUTTON_CreateIndirect, "bt_0", ID_BUTTON_0, MenuLabel_X, MenuLabel_Y,                    MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { HSD_BUTTON_CreateIndirect, "bt_1", ID_BUTTON_1, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT,  MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { HSD_BUTTON_CreateIndirect, "bt_2", ID_BUTTON_2, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*2,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { HSD_BUTTON_CreateIndirect, "bt_3", ID_BUTTON_3, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*3,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
#ifdef P_AM128A
  { HSD_BUTTON_CreateIndirect, "bt_4", ID_BUTTON_4, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*4,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0}
#endif  
};






static int btIndex  = 0;

/*********************************************************************
*
*       Static code
*
**********************************************************************/

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
 	char     i;
  
  switch (pMsg->MsgId) { 
     
    case USER_MSG_DFULT_CNT:
         HSD_BUTTON_SetValue(hButtons[1], pMsg->Data.v);
         break;
              
    case USER_MSG_SKIN: 
INFO("case msg skin");    
         pSkin  = &(menuWinSkins[pMsg->Data.v]);
         
         WINDOW_SetBkColor(pMsg->hWin,pSkin->bkColor);         
                 
         for(i=0; i<MENU_ITEM_NUM; i++ )
         {
            HSD_BUTTON_SetBkColor(hButtons[i], pSkin->btBkColor);
            HSD_BUTTON_SetTBLineColor(hButtons[i], pSkin->tbColor);
            HSD_BUTTON_SetFocusBkColor(hButtons[i], pSkin->btBkFocus);
            HSD_BUTTON_SetTextColor(hButtons[i], pSkin->btTxColor);
            HSD_BUTTON_SetTextFocusColor(hButtons[i], pSkin->btTxFocus);
            HSD_BUTTON_SetVColor(hButtons[i], pSkin->btTxColor);
            HSD_BUTTON_SetVFocusColor(hButtons[i], pSkin->btTxFocus);           
         }
         break;
         
   case WM_PAINT:
        GUI_DrawGradientV(0,0,MenuLabel_WIDTH,40,pSkin->ttl_bkTop,pSkin->ttl_bkBottom);
        
        GUI_SetColor(pSkin->ttl_Text);
        GUI_SetTextMode(GUI_TEXTMODE_TRANS);
        GUI_DispStringAt("主菜单",25,5);      
        break;
        
  case WM_INIT_DIALOG:
		
	  	pSkin  = &(menuWinSkins[SysConf.Skin]);
    //
    // Initialization of 'bt_0'
    //
    hButtons[0] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    HSD_BUTTON_SetText(hButtons[0], "监控列表");
    
    //
    // Initialization of 'bt_1'
    //
    hButtons[1] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    HSD_BUTTON_SetText(hButtons[1], "监控设置");
 
    //
    // Initialization of 'bt_2'
    //
    hButtons[2] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    HSD_BUTTON_SetText(hButtons[2], "船舶列表");	
    
    //
    // Initialization of 'bt_3'
    //
    hButtons[3] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    HSD_BUTTON_SetText(hButtons[3], "系统设置");	    
    
#ifdef P_AM128A
   hButtons[4]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
   HSD_BUTTON_SetText(hButtons[4], "特殊报警");
#endif    
/**
 *  emWin Bug
 */  
//  WM_SetFocus(hButtons[1]);
  
    WINDOW_SetBkColor(pMsg->hWin,pSkin->bkColor);  
//    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), pSkin->Menu_Label);
    
		  for (i=0; i<MENU_ITEM_NUM; i++)
	  	{		
       HSD_BUTTON_SetTxFont(hButtons[i], &GUI_Font30);
       HSD_BUTTON_SetBkColor(hButtons[i], pSkin->btBkColor);
       HSD_BUTTON_SetTBLineColor(hButtons[i], pSkin->tbColor);
       HSD_BUTTON_SetFocusBkColor(hButtons[i], pSkin->btBkFocus);
       HSD_BUTTON_SetTextColor(hButtons[i], pSkin->btTxColor);
       HSD_BUTTON_SetTextFocusColor(hButtons[i], pSkin->btTxFocus);
       HSD_BUTTON_SetVColor(hButtons[i], pSkin->btTxColor);
       HSD_BUTTON_SetVFocusColor(hButtons[i], pSkin->btTxFocus); 
       WM_SetCallback(hButtons[i], &myButtonListener);
		  }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN menuWinCreate(void) {
  WM_HWIN hWin;
	
	
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	menuWin = hWin;
  return hWin;
}

// USER START (Optionally insert additional public code)

static void myButtonListener(WM_MESSAGE * pMsg)
{
 static int selIndex  = 2;
	const WM_KEY_INFO * pInfo;
	WM_HWIN thisButton  = pMsg->hWin;
	
	
	switch(pMsg->MsgId)
	{
 
    case WM_SET_FOCUS:
         btIndex  = WM_GetId(pMsg->hWin) - ID_BUTTON_0;
         if(btIndex < MENU_ITEM_NUM   &&  btIndex >= 0)
         {
            if(pMsg->Data.v)
            {      
               selIndex  = btIndex;   
               
               /**Background color can't change with focus in HSD_BUTTON_Callbak  */
               HSD_BUTTON_SetBkColor(thisButton, pSkin->btBkFocus);    
               WM_BringToTop(subWins[btIndex]);
               if(btIndex == 1)
               {
                  WM_BringToTop(mntSettingWin);
                  WM_SendMessageNoPara(subWins[btIndex], USER_MSG_LV_UPDATE);
               }
             }
             else
             {      
                if(selIndex == btIndex)
                {
                   HSD_BUTTON_SetBkColor(thisButton, pSkin->btBkSel);
                }
                else
                {
                   HSD_BUTTON_SetBkColor(thisButton, pSkin->btBkColor);
                }
             }
         
             HSD_BUTTON_Callback(pMsg);
          }
          else
          {
             INFO("focus err!");      
          }
           break;

	   	case WM_KEY:
          pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
          switch(pInfo->Key)
          {
             case GUI_KEY_PWM_INC:       
               WM_SendMessageNoPara(subWins[3], USER_MSG_DIM);
               break;
             case GUI_KEY_DOWN:  
                  selIndex = -1;  
                  HSD_BUTTON_Callback(pMsg);
                  break;
             
             case GUI_KEY_UP:
                  selIndex  = -1;
                  HSD_BUTTON_Callback(pMsg);
                  break;
             
             case GUI_KEY_RIGHT: 
                  HSD_BUTTON_SetBkColor(thisButton, pSkin->btBkSel);
                  WM_SetFocus(subWins[btIndex]); 
                  break;
                  
             case GUI_KEY_MENU:       
             case GUI_KEY_BACKSPACE:
                  selIndex  = -1;
                  WM_BringToBottom(menuWin);
                  WM_HideWindow(subWins[0]);
                  WM_HideWindow(subWins[1]);
                  WM_HideWindow(subWins[2]);
                  WM_HideWindow(subWins[3]);
#ifdef P_AM128A
WM_HideWindow(subWins[4]);
#endif                  
                  WM_HideWindow(mntSettingWin);     
                  HSD_BUTTON_SetBkColor(thisButton, pSkin->btBkColor);   
                  WM_SetFocus(hButtons[0]);         
                  WM_SetFocus(mapWin);
                  GUI_CURSOR_Show();   
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

// USER END

/*************************** End of file ****************************/
