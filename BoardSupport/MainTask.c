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
#include "HSD_Toast.h"

Bool toast_flg = FALSE;
extern uint8_t ipcMsg;
extern char comflg;
extern OS_EVENT * updataMBox;
//extern OS_EVENT* pMSBOX; 

WM_HWIN handle;

GUI_MEMDEV_Handle hMute;


void MainTask(void)
{
   WM_MESSAGE myMsg;
   GUI_MEMDEV_Handle hMem0;
//printf("MainTask Start\n");

//printf("maintask\n");
   GUI_Init();
	//创建字体
   GUI_UC_SetEncodeUTF8();	
   WM_SetCreateFlags(WM_CF_MEMDEV);
   

    hMem0 = GUI_MEMDEV_Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    GUI_MEMDEV_Select(hMem0);
    GUI_SetBkColor(0x009c6f00);
    GUI_Clear();
    
    GUI_SetFont(&GUI_Font_T90_60);
    GUI_SetColor(GUI_WHITE);
    GUI_DispStringAt("T90拖网距离安全标终端",150,200);
    GUI_PNG_Draw(&aclogo_png,sizeof(aclogo_png),245,120);
 
		GUI_MEMDEV_Select(0);
    GUI_MEMDEV_CopyToLCD(hMem0);
    GUI_MEMDEV_Delete(hMem0);
    GUI_Delay(1800);
    

   //字体设置	//GUI_SetDefaultFont (&SIF_Font);
   GUI_SetDefaultFont (&GUI_Font_T90_30);	
   TEXT_SetDefaultFont(&GUI_Font_T90_30);
   BUTTON_SetDefaultFont (&GUI_Font_T90_30);
   HEADER_SetDefaultFont(&GUI_Font_T90_30);
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

  
   MMSISetWin = WIN_MMSISetCreate();
			SoftInputWin = WIN_SoftInputCreate();
			
   FleetWin = WIN_FleetCreate();

		
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
		
      highspshipsetWin = WIN_HighSpeedShipSetCreate();


		if(t90_set.sys.workmode == NONE_MODE)      //没有选择单双拖
		{         
			sound = OFF;
			WM_BringToTop(workModeWin);
			WM_SetFocus(workModeWin);
		}
		else
		{
			StubRefresh();
			WM_BringToTop(mainShipWin);
			WM_SetFocus(mainShipWin);
		}
  
			
   while(1)
   {
      INT8U err;
      OS_MBOX_DATA * updataMSdata;
      static int flag = 0;
      WM_MESSAGE pMsg;

      if(!flag && t90_set.sys.workmode == DOUBLE_MODE && t90_set.as_MMSI.port == 0 )
      {
//         
         flag = 1;
         pMsg.hWin = MMSISetWin;
									pMsg.hWinSrc = FleetWin;
									pMsg.MsgId = USER_MSG_MMSISET;
									pMsg.Data.v = MONITMMSI_SET;
									WM_SendMessage(pMsg.hWin,&pMsg);
                    
      }       
      if(ipcMsg){
         if(ipcMsg & 0x01 )
         {  
            ipcMsg &= (~0x01);            
            if(portStatus[0].port == 2)
            {
//               toast_flg = TRUE;
//               ToastCreate("01号安全标故障 ", &GUI_Font_T90_30, TOAST_OK, 2000);
            }
            if(portStatus[0].port == 1)
            {
//               ToastCreate("01号安全标插入 ", &GUI_Font_T90_30, NULL, 500);
            }
            if(portStatus[0].port == 0)
            {
//               ToastCreate("01号安全标拔出 ", &GUI_Font_T90_30, NULL, 500);
            }              
         }
         GUI_Delay(100);
         if(ipcMsg & 0x02)
         {
            ipcMsg &= (~0x02);
            if(portStatus[1].port == 2)
            {
//               toast_flg = TRUE;
//               ToastCreate("02号安全标故障 ", &GUI_Font_T90_30, TOAST_OK, 2000);
            }
            if(portStatus[1].port == 1)
            {
//               ToastCreate("02号安全标插入 ", &GUI_Font_T90_30, NULL, 500);
            }
            if(portStatus[1].port == 0)
            {
//               ToastCreate("02号安全标拔出 ", &GUI_Font_T90_30, NULL, 500);
            }
         }
         GUI_Delay(100);
         
         if(ipcMsg & 0x04)
         {
            ipcMsg &= (~0x04);
            if(portStatus[2].port == 2)
            {
//               toast_flg = TRUE;
//               ToastCreate("03号安全标故障 ", &GUI_Font_T90_30, TOAST_OK, 2000);
            }
            if(portStatus[2].port == 1)
            {
//               ToastCreate("03号安全标插入 ", &GUI_Font_T90_30, NULL, 500);
            }
            if(portStatus[2].port == 0)
            {
//               ToastCreate("03号安全标拔出 ", &GUI_Font_T90_30, NULL, 500);
            }
         }
         GUI_Delay(100);
         /// Data ack ok
         if(ipcMsg & 0x20){           
            ipcMsg &= (~0x20);   //数据应答
            if(comflg == 1)
            {
               ToastCreate("两船位置切换成功!", &GUI_Font_T90_30, NULL, 2000);
               comflg = 0;
            }
            else if(comflg == 2)
            {
               ToastCreate("距离设置修改成功!", &GUI_Font_T90_30, NULL, 2000);
               comflg = 0;
            }
            if(t90_set.sys.workmode == SINGLE_MODE)
            {
               pMsg.hWin = singleShipDstSetWin;
               pMsg.MsgId = USER_MSG_DATA_ACK_RESULT;
               pMsg.Data.v = DATA_ACK_OK;               
               WM_SendMessage(singleShipDstSetWin, &pMsg);
            }
            else
            {
               pMsg.hWin = doubleShipDstSetWin;
               pMsg.MsgId = USER_MSG_DATA_ACK_RESULT;
               pMsg.Data.v = DATA_ACK_OK;               
               WM_SendMessage(doubleShipDstSetWin, &pMsg);
            }
         }
         /// Data ack timeout
         else if(ipcMsg & 0x10){
           
            ipcMsg  &= (~0x10);  //数据超时
            if(comflg == 1)
            {
               ToastCreate("两船位置切换失败，请重试!", &GUI_Font_T90_30, NULL, 2000);
               t90_set.sys.motherpos = !t90_set.sys.motherpos;
               StubRefresh();
               WM_InvalidateWindow(doubleShipDstSetWin);
               comflg = 0;
            }
            else if(comflg == 2)
            {
               ToastCreate("距离设置修改失败，请重试!", &GUI_Font_T90_30, NULL, 2000);
               comflg = 0;
            }
            if(t90_set.sys.workmode == SINGLE_MODE)
            {
               pMsg.hWin = singleShipDstSetWin;
               pMsg.MsgId = USER_MSG_DATA_ACK_RESULT;
               pMsg.Data.v = DATA_ACK_TIME_OUT;               
               WM_SendMessage(pMsg.hWin, &pMsg);              
            }
            else if(t90_set.sys.workmode == DOUBLE_MODE)
            {
               pMsg.hWin = doubleShipDstSetWin;
               pMsg.MsgId = USER_MSG_DATA_ACK_RESULT;
               pMsg.Data.v = DATA_ACK_TIME_OUT;
               WM_SendMessage(pMsg.hWin, &pMsg);
            }
         }
         

         
         if(ipcMsg & 0x40){
           
            ipcMsg  &= (~0x40);  //握手超时
            toast_flg = TRUE;
//            ToastCreate("连接超时! ", &GUI_Font_T90_30, TOAST_OK, 2000);
         }
         
         
      }
      
      if(OSMboxPend(updataMBox, 1, &err))
      {
          
//          myMsg.hWin  = WM_GetClientWindow(confirmWin);
//          myMsg.hWinSrc  = WM_GetFocussedWindow();
//          myMsg.MsgId  = USER_MSG_CHOOSE;
//          myMsg.Data.v  = UPDATA;
//          WM_SendMessage(myMsg.hWin, &myMsg);
//            
//          WM_BringToTop(confirmWin);
//          WM_SetFocus(confirmWin);
         NVIC_SystemReset();
      }
//printf("MainTask end\n");     
      GUI_Delay(200);
     
   }

}


/*************************** End of file ****************************/
