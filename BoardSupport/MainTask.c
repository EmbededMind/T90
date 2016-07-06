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
#include "t90font.h"
#include "stub.h"
#include "ucos_ii.h"

//extern unsigned char isSub0Inited;
//extern unsigned char isSub2Inited;
//extern unsigned char isChecked;
extern unsigned char  isDstSetChanged;
extern unsigned char isDstSetNeedUpdate;
extern OS_EVENT* pMSBOX; 

WM_HWIN handle;

GUI_MEMDEV_Handle hMute;

void MainTask(void)
{
   WM_MESSAGE myMsg;
   GUI_MEMDEV_Handle hMem0;
//INFO("MainTask Start");

   GUI_Init();
	//创建字体
   GUI_UC_SetEncodeUTF8();	
   WM_SetCreateFlags(WM_CF_MEMDEV);
   
    hMem0 = GUI_MEMDEV_Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    GUI_MEMDEV_Select(hMem0);
    GUI_SetBkColor(0x009c6f00);
    GUI_Clear();
    
    GUI_SetFont(&GUI_Font_T90_60);
    GUI_SetColor(0x00c3e0e9);
    GUI_DispStringAt("拖网距离安全标终端",200,200);
    
		GUI_MEMDEV_Select(0);
    GUI_MEMDEV_CopyToLCD(hMem0);
    GUI_MEMDEV_Delete(hMem0);
    GUI_Delay(1000);
    

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
	 GUI_FillRect(0, SCREEN_HEIGHT, 799, 479);   //右边和下边被遮盖边框填充黑色
	 
	 
	 GUI_CURSOR_Select(&GUI_CursorCrossS);       //选择光标类型
	 GUI_CURSOR_Hide();
	 
//DLG_testDimensinCreate();
	
		workModeWin = WIN_WorkModeCreate(); 
	
		mainShipWin = WIN_SingleShipCreate();	
//		doubleShipWin = WIN_DoubleShipCreate();

  

		
		alarmMonitorWin = WIN_AlarmMonitorCreate();           //创建窗口 
		
		mainMenuDlg = DLG_MainMenuCreate();	
		dstSetMenuDlg = DLG_DstSetMenuCreate();		
		alarmSetMenuDlg = DLG_AlarmSetMenuCreate();
		systemSetDlg = DLG_SystemSetCreate();
		
		singleShipDstSetWin = WIN_SingleShipDstSetCreate();
      doubleShipDstSetWin  = WIN_doubleShipDstSetCreate();
		
		invdAlarmSetWin = WIN_InvdAlarmSetCreate();
		spdingAlarmSetWin = WIN_SpdingAlarmSetCreate();
		sogAlarmSetWin = WIN_SogAlarmSetCreate();
		specialAlarmSetDlg = DLG_SpecialAlarmSetCreate();
		
		confirmWin = WIN_ConfirmCreate();
		
//INFO("mainShipWin:%ld", mainShipWin);		
//INFO("alarmMonitorWin:%ld", alarmMonitorWin);


		if(t90_set.sys.workmode == NONE_MODE)      //没有选择单双拖
		{         
			monitorState = OFF;
			WM_BringToTop(workModeWin);
			WM_SetFocus(workModeWin);
		}

		else
		{
			StubRefresh();
			WM_BringToTop(mainShipWin);
			WM_SetFocus(mainShipWin);
		}
      
//		else if(t90_set.sys.workmode == SINGLE_MODE)
//		{
//         StubRefresh();
//			WM_BringToTop(mainShipWin);
//			WM_SetFocus(mainShipWin);
//		}
//		else if(t90_set.sys.workmode == DOUBLE_MODE)
//		{
//         StubRefresh();
//			WM_BringToTop(doubleShipWin);
//			WM_SetFocus(doubleShipWin);
//		}

//DLG_testCustomedWidgetCreate();
   while(1)
   {    
      T90_PlugEvent *plugEvent;
//      printf("bfstatus = %x\n",plugEvent->status);
//      int *i;
//printf("oldi=%d\n",*i);
      plugEvent = OSMboxAccept(pMSBOX);
//      i = OSMboxAccept(&MSBOX);
//printf("newi=%d\n",*i);
      
      if(plugEvent != NULL)
      {
         printf("afstatus = %x\n",plugEvent->status);
 
//         if(plugEvent->status)
//         {
            myMsg.MsgId = USER_MSG_PLUG;
//            myMsg.Data.v = plugEvent->whichPort*2+1;
            myMsg.Data.v  = plugEvent->status;
            myMsg.hWin = dstSetMenuDlg;
            WM_SendMessage(myMsg.hWin, &myMsg);
//            printf("plug in\n");
//         }
//         else
//         {
//            myMsg.MsgId = USER_MSG_PLUG;
//            myMsg.Data.v = plugEvent->whichPort*2;
//            myMsg.hWin = dstSetMenuDlg;
//            WM_SendMessage(myMsg.hWin, &myMsg);
//            printf("plug out \n");
//         }
       }
      
//      if(plugEvent->status & 0x04)
//      {
//         myMsg.MsgId = USER_MSG_PLUG;
//         myMsg.Data.v = 1;
//         myMsg.Data.Color = 1;
//         myMsg.hWin = dstSetMenuDlg;
//         WM_SendMessage(myMsg.hWin, &myMsg);
//      }
//      else
//      {
//         myMsg.MsgId = USER_MSG_PLUG;
//         myMsg.Data.v = 1;
//         myMsg.Data.Color = 0;
//         myMsg.hWin = dstSetMenuDlg;
//         WM_SendMessage(myMsg.hWin, &myMsg);
//      }
//      
//      if(plugEvent->status & 0x10)
//      {
//         myMsg.MsgId = USER_MSG_PLUG;
//         myMsg.Data.v = 2;
//         myMsg.Data.Color = 1;
//         myMsg.hWin = dstSetMenuDlg;
//         WM_SendMessage(myMsg.hWin, &myMsg);
//      }
//      else
//      {
//         myMsg.MsgId = USER_MSG_PLUG;
//         myMsg.Data.v = 2;
//         myMsg.Data.Color = 0;
//         myMsg.hWin = dstSetMenuDlg;
//         WM_SendMessage(myMsg.hWin, &myMsg);
//      }
      
      if(isDstSetNeedUpdate){
         isDstSetNeedUpdate  = 0;
         if(t90_set.sys.workmode == SINGLE_MODE)
         {
            WM_SendMessageNoPara(singleShipDstSetWin, USER_MSG_DST_UPDATE);
         }
         else
         {
            WM_SendMessageNoPara(doubleShipDstSetWin, USER_MSG_DST_UPDATE);
         }
      }
      GUI_Delay(200);      
   }

}

/*************************** End of file ****************************/
