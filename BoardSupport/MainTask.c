#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
#include "exfuns.h"
//#include "Setting.h"
#include "logo.h"
#include "120.h"
#include "28.h"
#include "dlg.h"
#include "sound.h"
#include "Config.h"
#include "layout.h"
#include "T90.h"

//extern unsigned char isSub0Inited;
//extern unsigned char isSub2Inited;
//extern unsigned char isChecked;

WM_HWIN handle;

GUI_MEMDEV_Handle hMute;

void MainTask(void)
{
//   GUI_MEMDEV_Handle hMem0;
INFO("MainTask Start");
//printf("\r\n ab \r c \n d \n\r");
   GUI_Init();	
   WM_SetCreateFlags(WM_CF_MEMDEV);
   
   //创建字体
   GUI_UC_SetEncodeUTF8();	
   //字体设置	//GUI_SetDefaultFont (&SIF_Font);
   GUI_SetDefaultFont (&GUI_Font30);	
   TEXT_SetDefaultFont(&GUI_Font30);
   BUTTON_SetDefaultFont (&GUI_Font30);
   HEADER_SetDefaultFont(&GUI_Font30);
   HEADER_SetDefaultBkColor(GUI_BLUE);
   //EDIT_SetDefaultFont(&GUI_Font30);
   WIDGET_SetDefaultEffect(&WIDGET_Effect_None); 
	
//	 GUI_SetBkColor(GUI_GRAY);
//   GUI_Clear();
	 GUI_SetColor(GUI_BLACK);
	 GUI_FillRect(SCREEN_WIDTH, 0, 799, 479);
	 GUI_FillRect(0, SCREEN_HEIGHT, 799, 479);
	 
	 GUI_CURSOR_Select(&GUI_CursorCrossS);
	 GUI_CURSOR_Hide();
	
	 
//DLG_testDimensinCreate();
	
		workModeWin = WIN_WorkModeCreate(); 
	
		singleShipWin = WIN_SingleShipCreate();	
		doubleShipWin = WIN_DoubleShipCreate();
		
		alarmMonitorWin = WIN_AlarmMonitorCreate();           //创建窗口 
		
		mainMenuDlg = DLG_MainMenuCreate();	
		dstSetMenuDlg = DLG_DstSetMenuCreate();		
		alarmSetMenuDlg = DLG_AlarmSetMenuCreate();
		systemSetDlg = DLG_SystemSetCreate();
		
		singleShipDstSetWin = WIN_SingleShipDstSetCreate();
		
		invdAlarmSetWin = WIN_InvdAlarmSetCreate();
		spdingAlarmSetWin = WIN_SpdingAlarmSetCreate();
		sogAlarmSetWin = WIN_SogAlarmSetCreate();
		specialAlarmSetDlg = DLG_SpecialAlarmSetCreate();
		
		confirmWin = WIN_ConfirmCreate();
		
INFO("singleShipWin:%ld", singleShipWin);		
INFO("alarmMonitorWin:%ld", alarmMonitorWin);


		if(t90_set.sys.workmode == NONE_MODE)
		{                         	
			WM_BringToTop(workModeWin);
			WM_SetFocus(workModeWin);
		}
		else if(t90_set.sys.workmode == SINGLE_MODE)
		{
			WM_BringToTop(singleShipWin);
			WM_SetFocus(singleShipWin);
		}
		else
		{
			WM_BringToTop(doubleShipWin);
			WM_SetFocus(doubleShipWin);
		}

//DLG_testCustomedWidgetCreate();
   while(1)
   {
      GUI_Delay(200);
   }
}

/*************************** End of file ****************************/
