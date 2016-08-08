#include "DIALOG.h"
#include "MainTask.h"
#include "Config.h"
//#include "Setting.h"
#include "dlg.h"
#include "28.h"
#include "T90.h"
#include "t90font.h"

#define ID_WINDOW_0      (GUI_ID_USER + 0x00)
//#define ID_BUTTON_OK     (GUI_ID_USER + 0x01)
//#define ID_BUTTON_CANCEL (GUI_ID_USER + 0x02)
#define ID_TEXT_CONTENT  (GUI_ID_USER + 0x01)

WM_HWIN confirmWin;

static WM_HWIN dlgTextContent;

static int Option  = 0;
static WM_MESSAGE myMsg;
static WM_HWIN buttons[3];

static void myButton(WM_MESSAGE * pMsg);

static const ConfirmWinColor *pColors = confirmWinColors;

static void _cbWindow(WM_MESSAGE * pMsg) {
	
	const WM_KEY_INFO* pInfo;
  WM_HWIN thisFrame  = pMsg->hWin;

  int     NCode;
  int     Id;
	
	int xSize;
	int ySize;  

	char UserData;
	
  switch (pMsg->MsgId) {
			  
	case WM_SET_FOCUS:
			 if(pMsg->Data.v)
			 {
					WM_GetUserData(pMsg->hWin,&UserData,4);
					if(UserData==MONITMMSI_FULL)
					{
						WM_SetFocus(buttons[2]);
					}
					else
					{
						WM_SetFocus(buttons[0]);
					}
			 }
			 break;		
	
  case WM_PAINT:
       
       xSize = WM_GetWindowSizeX(pMsg->hWin);
       ySize = WM_GetWindowSizeY(pMsg->hWin);
//       hMem0 = GUI_MEMDEV_Create(0, 0, xSize, ySize);
//       GUI_MEMDEV_Select(hMem0);
       GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 20, pColors->bkTopColor,pColors->bkBottomColor);
//       GUI_MEMDEV_Select(0);
//       GUI_MEMDEV_CopyToLCD(hMem0);
//       GUI_MEMDEV_Delete(hMem0);
       break;
	
	case USER_MSG_SKIN:
       pColors = &(confirmWinColors[pMsg->Data.v]);
    
       BUTTON_SetBkColor(buttons[0], BUTTON_BI_UNPRESSED, pColors->btBkColor);
       BUTTON_SetBkColor(buttons[1], BUTTON_BI_UNPRESSED, pColors->btBkColor);
      
       TEXT_SetTextColor(dlgTextContent, pColors->textColor);
       break;
 
  case WM_CREATE:
    //
    // Initialization of 'bt_OK'
    //
       BUTTON_CreateEx (60,110,80,40,thisFrame, WM_CF_HASTRANS  ,0,GUI_ID_BUTTON0);
       buttons[0] = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
       BUTTON_SetFocusColor(buttons[0],pColors->btFocusBkColor);
       WM_ShowWindow (buttons[0]);
       BUTTON_SetText(buttons[0], "确定");
       BUTTON_SetFont(buttons[0], &GUI_Font_T90_30);
	    WM_SetCallback(buttons[0], &myButton);
	
       BUTTON_SetBkColor(buttons[0],BUTTON_BI_UNPRESSED,pColors->btBkColor);
       BUTTON_SetTextColor(buttons[0],BUTTON_BI_UNPRESSED,pColors->btTextColor);
       
        //
        // Initialization of 'bt_Cancle'
        //
       BUTTON_CreateEx(260,  110,   80,  40,thisFrame,WM_CF_HASTRANS,0,GUI_ID_BUTTON1);
       buttons[1] = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
       BUTTON_SetFocusColor(buttons[1],pColors->btFocusBkColor);
       WM_ShowWindow (buttons[1]);
       BUTTON_SetText(buttons[1], "取消");
       BUTTON_SetFont(buttons[1], &GUI_Font_T90_30);
			 WM_SetCallback(buttons[1], &myButton);
			 
       BUTTON_SetBkColor(buttons[1],BUTTON_BI_UNPRESSED,pColors->btBkColor);
       BUTTON_SetTextColor(buttons[1],BUTTON_BI_UNPRESSED,pColors->btTextColor);
       
							//
							// Initialization of 'bt_OK'
							//							
							buttons[2] = BUTTON_CreateEx (180,110,80,40,thisFrame, WM_CF_HASTRANS  ,0,GUI_ID_BUTTON2);
       WM_HideWin(buttons[2]);
       BUTTON_SetText(buttons[2], "确定");
       BUTTON_SetFont(buttons[2], &GUI_Font_T90_30);
	      WM_SetCallback(buttons[2], &myButton);
	
       BUTTON_SetBkColor(buttons[0],BUTTON_BI_UNPRESSED,pColors->btBkColor);
       BUTTON_SetTextColor(buttons[0],BUTTON_BI_UNPRESSED,pColors->btTextColor);
							
    //
    // Initialization of 'Text'
    //
		 TEXT_CreateEx (0,   35,  400, 80, thisFrame,WM_CF_SHOW,0,ID_TEXT_CONTENT,NULL);
       dlgTextContent = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONTENT);
		 TEXT_SetTextAlign(dlgTextContent,TEXT_CF_HCENTER);
       TEXT_SetFont(dlgTextContent, &GUI_Font_T90_30);
			 
		 TEXT_SetTextColor (dlgTextContent,pColors->textColor);

       break;
	
	case WM_KEY:

		pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
	
	  switch(pInfo->Key)
		 {
		   	case GUI_KEY_LEFT:
		   	case GUI_KEY_RIGHT: 
							   WM_GetUserData(pMsg->hWin,&UserData,4);
							   if(UserData==MONITMMSI_FULL)
										{
											WM_SetFocus(buttons[2]);
										}
										else
										{
											if(WM_HasFocus(buttons[0]))
											{
													WM_SetFocus(buttons[1]);
											}
											else
											{
													WM_SetFocus(buttons[0]);
											}
										}
										break;	
       
    case GUI_KEY_BACKSPACE:
									{
										myMsg.hWin  = myMsg.hWinSrc;
										myMsg.hWinSrc  = pMsg->hWin;
										myMsg.MsgId    = USER_MSG_REPLY;
										myMsg.Data.v   = REPLY_CANCEL;
										WM_SendMessage(myMsg.hWin, &myMsg);  
										WM_BringToBottom(confirmWin);  
									}
									break;  
           
			   default:
			       break;
		}
		break;
		
  case WM_NOTIFY_PARENT:
       Id    = WM_GetId(pMsg->hWinSrc);    // Id of widget
       NCode = pMsg->Data.v;               // Notification code
       switch (NCode) 
       {
          case WM_NOTIFICATION_RELEASED:      // React only if released
               switch (Id) 
               {
                  case GUI_ID_BUTTON0: 
                       myMsg.hWin     = myMsg.hWinSrc;
                       myMsg.hWinSrc  = pMsg->hWin;
                       myMsg.MsgId    = USER_MSG_REPLY;
                       myMsg.Data.v   = REPLY_OK;
                       WM_SendMessage(myMsg.hWin, &myMsg);
                       break;
                       
                  case GUI_ID_BUTTON1:
                       WM_SetFocusOnPrevChild(confirmWin);
                       
                       myMsg.hWin     = myMsg.hWinSrc;
                       myMsg.hWinSrc  = pMsg->hWin;
                       myMsg.MsgId    = USER_MSG_REPLY;
                       myMsg.Data.v   = REPLY_CANCEL;
                       WM_SendMessage(myMsg.hWin, &myMsg);
                       break;
																		
																		case GUI_ID_BUTTON2:
                       myMsg.hWin     = myMsg.hWinSrc;
                       myMsg.hWinSrc  = pMsg->hWin;
                       myMsg.MsgId    = USER_MSG_REPLY;
                       myMsg.Data.v   = REPLY_OK;
                       WM_SendMessage(myMsg.hWin, &myMsg);
																							{
																								WM_ShowWin(buttons[0]);
																								WM_ShowWin(buttons[1]);
																								WM_HideWin(buttons[2]);
																							}
																			    break;
               }
               WM_BringToBottom(confirmWin);
               break;
    }
    break;

  case USER_MSG_CHOOSE:
      Option  = pMsg->Data.v;
      
      myMsg.hWinSrc  = pMsg->hWinSrc;
      myMsg.Data.v   = Option;
      
      switch(Option)   
      {
         case CANCEL_MONITED:         
              TEXT_SetText(dlgTextContent, "??????????");
              break;        
         case STORE_SETTING:       
              TEXT_SetText(dlgTextContent, "??????????");    
              break;
         case ADD_MONITED:             
              TEXT_SetText(dlgTextContent, "????????????");
              break;
         case SYS_SETTING:
              TEXT_SetText(dlgTextContent, "是否更改设置内容？");
									     UserData = SYS_SETTING;
									     WM_SetUserData(pMsg->hWin,&UserData,4);
							break;
         case SYS_RESET:
              TEXT_SetText(dlgTextContent, "是否恢复出厂设置？");
									     UserData = SYS_RESET;
									     WM_SetUserData(pMsg->hWin,&UserData,4);
              break;
									
									case MONITMMSI_SET:
										    TEXT_SetText(dlgTextContent, "是否修改辅助作业船九位码?");
									     
									     UserData = MONITMMSI_SET;
									     WM_SetUserData(pMsg->hWin,&UserData,4);
										printf("confimwin1");
              break;
									
									case MONITMMSI_ADD:
										    TEXT_SetText(dlgTextContent, "是否添加屏蔽报警船只?");
									     
									     UserData = MONITMMSI_ADD;
									     WM_SetUserData(pMsg->hWin,&UserData,4);
									     break;
									
									case MONITMMSI_DEL:
										  TEXT_SetText(dlgTextContent, "删除后,对该船只报警功能\n恢复正常,是否确认删除?");								     
									     UserData = MONITMMSI_DEL;
									     WM_SetUserData(pMsg->hWin,&UserData,4);
									     break;
									
									case MONITMMSI_FULL:
										  TEXT_SetText(dlgTextContent, "已经达到设置上限,请\n删除后再进行添加!");
									     WM_HideWin(buttons[0]);
									     WM_HideWin(buttons[1]);
									     WM_ShowWin(buttons[2]);
									     UserData = MONITMMSI_FULL;
									     WM_SetUserData(pMsg->hWin,&UserData,4);
									     break;
									
         default:       
              break;
      }
      break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


WM_HWIN WIN_ConfirmCreate(void) {
  WM_HWIN hWin;	
	hWin = WM_CreateWindow(200, 120, 400, 200,WM_CF_SHOW, _cbWindow, 4);
	WM_SetHasTrans(hWin);
  return hWin;
}


static void myButton (WM_MESSAGE *pMsg)
{
 	int index = 0;



	 switch(pMsg->MsgId)   
  { 	 
     case WM_SET_FOCUS:
          index  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
           
          if(pMsg->Data.v == 0)
          {
             BUTTON_SetBkColor(buttons[index], BUTTON_CI_UNPRESSED, pColors->btBkColor);
             //TEXT_SetBkColor(Texts[index], pSkin->MntSetWin_bkNOFOCUS);
          }
          else
          {
            BUTTON_SetBkColor(buttons[index], BUTTON_CI_UNPRESSED, pColors->btFocusBkColor);
            //TEXT_SetBkColor(Texts[index], GUI_DARKMAGENTA); 
          }
          
          BUTTON_Callback(pMsg);
          break;

			default:
			    BUTTON_Callback(pMsg);
			    break;
	}
}



