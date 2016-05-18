#include "Config.h"
#include "WM.h"
#include "MainTask.h"
#include "dlg.h"
#include "HSD_BUTTON.h"
#include "T90.h"
#include "t90font.h"

#include "layout_alarm_set.h"

//static const GUI_RECT drawArea = {30, 30, ALARM_SET_WIDTH-30, ALARM_SET_HEIGHT-30};

WM_HWIN sogAlarmSetWin;

static WM_HWIN buttons[4];

static int agent_max_sog;
static int agent_min_sog;

static const SetWinColor *pColors = setWinColors;


/**@brief 航速报警界面按钮的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myButtonCallback(WM_MESSAGE* pMsg)
{ 
	WM_MESSAGE myMsg;
	int id;
	int index;
   switch(pMsg->MsgId){
      case WM_SET_FOCUS:
           if(pMsg->Data.v){
              HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->focusBkColor);
//						  BUTTON_SetTextColor(pMsg->hWin,BUTTON_CI_UNPRESSED,GUI_LIGHTGRAY);
//上边这句话没用？						 
           }
           else{
              HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->bkColor);
						  HSD_BUTTON_SetTextColor(pMsg->hWin,pColors->textColor);
           }
           HSD_BUTTON_Callback(pMsg);
           break;
      
      case WM_KEY:
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key )
					  {						
							 case GUI_KEY_PWM_INC:       
                   WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
                   break;
               case GUI_KEY_BACKSPACE:
										if(t90_set.alarm.max_sog == agent_max_sog && t90_set.alarm.min_sog == agent_min_sog)
										{
											WM_SetFocus(alarmSetMenuDlg);
										}
										else
										{
											myMsg.hWin  = WM_GetClientWindow(confirmWin);
											myMsg.hWinSrc  = sogAlarmSetWin;
											myMsg.MsgId  = USER_MSG_CHOOSE;
											myMsg.Data.v  = SYS_SETTING;
											WM_SendMessage(myMsg.hWin, &myMsg);
											
											WM_BringToTop(confirmWin);
											WM_SetFocus(confirmWin);
										}
                    break;
							 case GUI_KEY_LEFT:
										index = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;							 
										index--;
										if(index == -1)
											index = 3;
										WM_SetFocus(buttons[index]);
										break;
               case GUI_KEY_RIGHT:
								 		index = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
										index++;
										if(index == 4)
											index = 0;
										WM_SetFocus(buttons[index]);
										break;
							 case GUI_KEY_UP:
										id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
							 
										if(id==0) agent_min_sog+=10;
										if(id==1) agent_min_sog+=1;
										if(agent_min_sog>agent_max_sog) agent_min_sog=agent_max_sog;
							      if(id==2) agent_max_sog+=10;
										if(id==3) agent_max_sog+=1;
							      if(agent_max_sog>99) agent_max_sog=99;
										sprintf(pStrBuf,"%d",agent_min_sog/10);
							      HSD_BUTTON_SetText(buttons[0], pStrBuf);
										sprintf(pStrBuf,"%d",agent_min_sog%10);
							      HSD_BUTTON_SetText(buttons[1], pStrBuf);	
										sprintf(pStrBuf,"%d",agent_max_sog/10);
							      HSD_BUTTON_SetText(buttons[2], pStrBuf);
										sprintf(pStrBuf,"%d",agent_max_sog%10);
							      HSD_BUTTON_SetText(buttons[3], pStrBuf);	
                    break;
							 case GUI_KEY_DOWN:
										id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
										if(id==2) agent_max_sog-=10;
										if(id==3) agent_max_sog-=1;
							      if(agent_min_sog>agent_max_sog) agent_max_sog=agent_min_sog;
										if(id==0) agent_min_sog-=10;
										if(id==1) agent_min_sog-=1;
							      if(agent_min_sog<0) agent_min_sog=0;
																	      
										sprintf(pStrBuf,"%d",agent_min_sog/10);
							      HSD_BUTTON_SetText(buttons[0], pStrBuf);
										sprintf(pStrBuf,"%d",agent_min_sog%10);
							      HSD_BUTTON_SetText(buttons[1], pStrBuf);	
										sprintf(pStrBuf,"%d",agent_max_sog/10);
							      HSD_BUTTON_SetText(buttons[2], pStrBuf);
										sprintf(pStrBuf,"%d",agent_max_sog%10);
							      HSD_BUTTON_SetText(buttons[3], pStrBuf);	
										break;
            }
            break;
           
     default :
           HSD_BUTTON_Callback(pMsg);
   }
}


/**@brief 航速报警界面窗口的回调函数
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
		
				 for(i = 0; i < 4; i++)
				 {
						HSD_BUTTON_SetBkColor(buttons[i], pColors->bkColor);
						HSD_BUTTON_SetTextColor(buttons[i], pColors->textColor);
				 }
				 break;
		
    case WM_CREATE:			
			
				 agent_max_sog = t90_set.alarm.max_sog;
				 agent_min_sog = t90_set.alarm.min_sog;
		
				 pColors = &setWinColors[t90_set.sys.nightmode];
		
         buttons[0]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2-180,
                                   ALARM_SET_HEIGHT/2-50, 
                                   58, 
                                   76, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
         WM_SetCallback(buttons[0], &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(buttons[0], GUI_FONT_D48X64); 				 
         sprintf(pStrBuf,"%d",t90_set.alarm.min_sog/10);
				 HSD_BUTTON_SetText(buttons[0], pStrBuf);
		
         HSD_BUTTON_SetBkColor(buttons[0], pColors->bkColor);
				 HSD_BUTTON_SetTextColor(buttons[0], pColors->textColor);
		     HSD_BUTTON_SetTextFocusColor(buttons[0], pColors->focusTextColor);
		
				 buttons[1]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2-102,
                                   ALARM_SET_HEIGHT/2-50, 
                                   58, 
                                   76, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON1);   
         WM_SetCallback(buttons[1], &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(buttons[1], GUI_FONT_D48X64); 				 
         sprintf(pStrBuf,"%d",t90_set.alarm.min_sog%10);
				 HSD_BUTTON_SetText(buttons[1], pStrBuf);
				 
         HSD_BUTTON_SetBkColor(buttons[1], pColors->bkColor);
				 HSD_BUTTON_SetTextColor(buttons[1], pColors->textColor);
		     HSD_BUTTON_SetTextFocusColor(buttons[1], pColors->focusTextColor);
				 
				 buttons[2]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2+20,
                                   ALARM_SET_HEIGHT/2-50, 
                                   58, 
                                   76, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON2);   
         WM_SetCallback(buttons[2], &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(buttons[2], GUI_FONT_D48X64); 				 
         sprintf(pStrBuf,"%d",t90_set.alarm.max_sog/10);
				 HSD_BUTTON_SetText(buttons[2], pStrBuf);
				 
         HSD_BUTTON_SetBkColor(buttons[2], pColors->bkColor);
				 HSD_BUTTON_SetTextColor(buttons[2], pColors->textColor);
		     HSD_BUTTON_SetTextFocusColor(buttons[2], pColors->focusTextColor);
				 
				 buttons[3]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2+98,
                                   ALARM_SET_HEIGHT/2-50, 
                                   58, 
                                   76, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON3);   
         WM_SetCallback(buttons[3], &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(buttons[3], GUI_FONT_D48X64); 				 
         sprintf(pStrBuf,"%d",t90_set.alarm.max_sog%10);
				 HSD_BUTTON_SetText(buttons[3], pStrBuf);
				 
         HSD_BUTTON_SetBkColor(buttons[3], pColors->bkColor);
				 HSD_BUTTON_SetTextColor(buttons[3], pColors->textColor);
		     HSD_BUTTON_SetTextFocusColor(buttons[3], pColors->focusTextColor);

				 WM_DefaultProc(pMsg);
         break;
         
    case WM_PAINT:
         GUI_SetBkColor(pColors->bkColor);
         WM_GetClientRectEx(pMsg->hWin, &clientRect);
         GUI_ClearRectEx(&clientRect);
         GUI_SetColor(GUI_RED);
//         GUI_SetLineStyle(GUI_LS_SOLID);
//         GUI_DrawRectEx(&drawArea);
				
				 GUI_DrawVLine(50, ALARM_SET_HEIGHT-30-32, ALARM_SET_HEIGHT-30);
				 GUI_DrawHLine(ALARM_SET_HEIGHT-30-32, 50, ALARM_SET_WIDTH-50);
				 GUI_DrawVLine(ALARM_SET_WIDTH-50, ALARM_SET_HEIGHT-30-32, ALARM_SET_HEIGHT-30);
		
				 GUI_SetDrawMode(GUI_DM_NORMAL);
				 GUI_SetColor(pColors->textColor);
		
				 GUI_FillRect(ALARM_SET_WIDTH/2-117, ALARM_SET_HEIGHT/2+6, ALARM_SET_WIDTH/2-107, ALARM_SET_HEIGHT/2+16);
				 GUI_FillRect(ALARM_SET_WIDTH/2+83, ALARM_SET_HEIGHT/2+6, ALARM_SET_WIDTH/2+93, ALARM_SET_HEIGHT/2+16);
		
				 GUI_DrawHLine(ALARM_SET_HEIGHT/2-12, ALARM_SET_WIDTH/2-15, ALARM_SET_WIDTH/2+15);
				 
				 GUI_SetFont(&GUI_Font_t90_30);
				 GUI_DispStringAt("正常航速", ALARM_SET_WIDTH/2-60, ALARM_SET_HEIGHT/2-90);
				 GUI_DispStringAt("节", ALARM_SET_WIDTH/2-39, ALARM_SET_HEIGHT/2-2);
				 GUI_DispStringAt("节", ALARM_SET_WIDTH/2+161, ALARM_SET_HEIGHT/2-2);
		
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
						 t90_set.alarm.max_sog = agent_max_sog;
						 t90_set.alarm.min_sog = agent_min_sog;
						 T90_Store();
					}
					else
					{
						agent_max_sog = t90_set.alarm.max_sog;
						agent_min_sog = t90_set.alarm.min_sog;
						
						sprintf(pStrBuf,"%d",agent_min_sog/10);
						HSD_BUTTON_SetText(buttons[0], pStrBuf);
						sprintf(pStrBuf,"%d",agent_min_sog%10);
						HSD_BUTTON_SetText(buttons[1], pStrBuf);	
						sprintf(pStrBuf,"%d",agent_max_sog/10);
						HSD_BUTTON_SetText(buttons[2], pStrBuf);
						sprintf(pStrBuf,"%d",agent_max_sog%10);
						HSD_BUTTON_SetText(buttons[3], pStrBuf);
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
WM_HWIN WIN_SogAlarmSetCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindow(ALARM_SET_X, ALARM_SET_Y, ALARM_SET_WIDTH, ALARM_SET_HEIGHT,  WM_CF_SHOW, &myWindowCallback , 0);
   return handle;
}




