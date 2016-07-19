#include "Config.h"
#include "WM.h"
#include "MainTask.h"
#include "dlg.h"
#include "HSD_BUTTON.h"
#include "T90.h"
#include "t90font.h"

#include "layout_alarm_set.h"

//static const GUI_RECT drawArea = {30, 30, ALARM_SET_WIDTH-30, ALARM_SET_HEIGHT-30};

WM_HWIN spdingAlarmSetWin;

static WM_HWIN buttons[2];

static int agentdst_set;

static const SetWinColor *pColors = setWinColors;


/**@brief 超速报警界面按钮的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myButtonCallback(WM_MESSAGE* pMsg)
{
	WM_MESSAGE myMsg;
   int id;	
   switch(pMsg->MsgId){
      case WM_SET_FOCUS:
           if(pMsg->Data.v){
              HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->focusBkColor);						 
           }
           else{
              HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->bkColor);
						  HSD_BUTTON_SetTextColor(pMsg->hWin,pColors->textColor);
           }
           HSD_BUTTON_Callback(pMsg);
           break;
      
      case WM_KEY:
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){
               case GUI_KEY_MOLEFT:
                        myMsg.hWin = systemSetDlg;
                        myMsg.hWinSrc = pMsg->hWin;
                        myMsg.MsgId = USER_MSG_MOTHERPOS;
                        myMsg.Data.v = DEFAULT_LEFT;
                        WM_SendMessage(myMsg.hWin, &myMsg);                 
                        break;
              
              case GUI_KEY_MORIGHT:
                        myMsg.hWin = systemSetDlg;
                        myMsg.hWinSrc = pMsg->hWin;
                        myMsg.MsgId = USER_MSG_MOTHERPOS;
                        myMsg.Data.v = DEFAULT_RIGHT;
                        WM_SendMessage(myMsg.hWin, &myMsg);                 
                        break;
               case GUI_KEY_SINGLE:                        
                         myMsg.hWin = systemSetDlg;
                         myMsg.hWinSrc = pMsg->hWin;
                         myMsg.MsgId = USER_MSG_WORKMODE;
                         myMsg.Data.v = SINGLE_MODE;
                         WM_SendMessage(myMsg.hWin, &myMsg);
                         
                         break;
                  case GUI_KEY_DOUBLE:                        
                         myMsg.hWin = systemSetDlg;
                         myMsg.hWinSrc = pMsg->hWin;
                         myMsg.MsgId = USER_MSG_WORKMODE;
                         myMsg.Data.v = DOUBLE_MODE;
                         WM_SendMessage(myMsg.hWin, &myMsg);
                         
							 case GUI_KEY_PWM_INC:       
                    WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
                    break;
               case GUI_KEY_BACKSPACE:
										if(t90_set.alarm.danger_sog == agentdst_set)
										{
											WM_SetFocus(alarmSetMenuDlg);
										}
										else
										{
											myMsg.hWin  = WM_GetClientWindow(confirmWin);
											myMsg.hWinSrc  = spdingAlarmSetWin;
											myMsg.MsgId  = USER_MSG_CHOOSE;
											myMsg.Data.v  = SYS_SETTING;
											WM_SendMessage(myMsg.hWin, &myMsg);
											
											WM_BringToTop(confirmWin);
											WM_SetFocus(confirmWin);
										}
                    break;
							 case GUI_KEY_LEFT:
               case GUI_KEY_RIGHT:
										if(pMsg->hWin == buttons[0])
											WM_SetFocus(buttons[1]);
										else
											WM_SetFocus(buttons[0]);
										break;
							 case GUI_KEY_UP:
										id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
										if(id==0) agentdst_set+=10;
										if(id==1) agentdst_set+=1;
										if(agentdst_set>99) agentdst_set=99;										
										sprintf(pStrBuf,"%d",agentdst_set/10);
							      HSD_BUTTON_SetText(buttons[0], pStrBuf);
										sprintf(pStrBuf,"%d",agentdst_set%10);
							      HSD_BUTTON_SetText(buttons[1], pStrBuf);										
                    break;
							 case GUI_KEY_DOWN:
										id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
							      if(id==0) agentdst_set-=10;
										if(id==1) agentdst_set-=1;
							      if(agentdst_set<0) agentdst_set=0;
							      sprintf(pStrBuf,"%d",agentdst_set/10);
							      HSD_BUTTON_SetText(buttons[0], pStrBuf);
										sprintf(pStrBuf,"%d",agentdst_set%10);
							      HSD_BUTTON_SetText(buttons[1], pStrBuf);		
            }
           break;
           
     default :
           HSD_BUTTON_Callback(pMsg);
           break;
   }
}


/**@brief 超速报警界面窗口的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myWindowCallback(WM_MESSAGE* pMsg)
{
	int i;
	GUI_RECT clientRect;
  switch(pMsg->MsgId){
		
		case USER_MSG_SKIN:
			   pColors = &(setWinColors[pMsg->Data.v]);	
		
				 for(i = 0; i < 2; i++)
				 {
						HSD_BUTTON_SetBkColor(buttons[i], pColors->bkColor);
						HSD_BUTTON_SetTextColor(buttons[i], pColors->textColor);
				 }
				 break;
		
    case WM_CREATE:		
			
				 agentdst_set = t90_set.alarm.danger_sog;
		
				 pColors = &setWinColors[t90_set.sys.nightmode];
		
         buttons[0]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2-40,
                                   ALARM_SET_HEIGHT/2-70, 
                                   70, 
                                   94, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
         WM_SetCallback(buttons[0], &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(buttons[0], GUI_FONT_D60X80); 				 
         sprintf(pStrBuf,"%d",t90_set.alarm.danger_sog/10);
				 HSD_BUTTON_SetText(buttons[0], pStrBuf);
		
         HSD_BUTTON_SetBkColor(buttons[0], pColors->bkColor);
				 HSD_BUTTON_SetTextColor(buttons[0], pColors->textColor);
		     HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->focusTextColor);
		
				 buttons[1]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2+53,
                                   ALARM_SET_HEIGHT/2-70, 
                                   70, 
                                   94, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON1);   
         WM_SetCallback(buttons[1], &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(buttons[1], GUI_FONT_D60X80); 				 
         sprintf(pStrBuf,"%d",t90_set.alarm.danger_sog%10);
				 HSD_BUTTON_SetText(buttons[1], pStrBuf);
				 
         HSD_BUTTON_SetBkColor(buttons[1], pColors->bkColor);
				 HSD_BUTTON_SetTextColor(buttons[1], pColors->textColor);
				 HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->focusTextColor);

				 WM_DefaultProc(pMsg);
         break;
         
    case WM_PAINT:

         GUI_SetBkColor(pColors->bkColor);
         WM_GetClientRectEx(pMsg->hWin, &clientRect);
         GUI_ClearRectEx(&clientRect);
         GUI_SetColor(pColors->textColor);
				 GUI_SetFont(&GUI_Font_T90_24);
//         GUI_DispStringAt("使用   选择选项，使用   选择数字。",50, ALARM_SET_HEIGHT-30-32);
         GUI_DispStringAt("使用",50, ALARM_SET_HEIGHT-30-32);
         GUI_SetColor(pColors->focusBkColor);
         GUI_DispString(" 咗祐 ");
         GUI_SetColor(pColors->textColor);
         GUI_DispString("选择选项，使用");
         GUI_SetColor(pColors->focusBkColor);
         GUI_DispString("  卞  ");
         GUI_SetColor(pColors->textColor);
         GUI_DispString("选择数字。");
//         GUI_SetLineStyle(GUI_LS_SOLID);
//         GUI_DrawRectEx(&drawArea);
				
//				 GUI_DrawVLine(50, ALARM_SET_HEIGHT-30-32, ALARM_SET_HEIGHT-30);
//				 GUI_DrawHLine(ALARM_SET_HEIGHT-30-32, 50, ALARM_SET_WIDTH-50);
//				 GUI_DrawVLine(ALARM_SET_WIDTH-50, ALARM_SET_HEIGHT-30-32, ALARM_SET_HEIGHT-30);
		
				 GUI_SetDrawMode(GUI_DM_NORMAL);
				 GUI_SetColor(pColors->textColor);
		
				 GUI_FillRect(ALARM_SET_WIDTH/2+35, ALARM_SET_HEIGHT/2-2, ALARM_SET_WIDTH/2+47, ALARM_SET_HEIGHT/2+10);
				 
				 GUI_SetFont(&GUI_Font_T90_30);
				 GUI_DispStringAt("报警航速", 50, ALARM_SET_HEIGHT/2-10);
				 GUI_DispStringAt("节", ALARM_SET_WIDTH-70, ALARM_SET_HEIGHT/2-10);
		
         break;
		
		case WM_SET_FOCUS:
				 if(pMsg->Data.v)
				 {
						WM_SetFocus(buttons[0]);
				 }
				 break;
				 
		 case USER_MSG_REPLY:
					if(pMsg->Data.v == REPLY_OK)
					{
						 t90_set.alarm.danger_sog = agentdst_set;
						 T90_Store();
					}
					else
					{
						sprintf(pStrBuf,"%d",t90_set.alarm.danger_sog/10);
						HSD_BUTTON_SetText(buttons[0], pStrBuf);
						sprintf(pStrBuf,"%d",t90_set.alarm.danger_sog%10);
						HSD_BUTTON_SetText(buttons[1], pStrBuf);
						agentdst_set = t90_set.alarm.danger_sog;
					}
					WM_SetFocus(alarmSetMenuDlg);
					break;
         
    default:
         WM_DefaultProc(pMsg);
  }
}

/** @brief 创建单双拖模式选择窗口
 *
 *    @param [out] 所创建窗口的句柄
 */
WM_HWIN WIN_SpdingAlarmSetCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindow(ALARM_SET_X, ALARM_SET_Y, ALARM_SET_WIDTH, ALARM_SET_HEIGHT,  WM_CF_SHOW, &myWindowCallback , 0);
   return handle;
}




