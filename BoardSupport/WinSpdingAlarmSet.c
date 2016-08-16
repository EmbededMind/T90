#include "Config.h"
#include "WM.h"
#include "MainTask.h"
#include "dlg.h"
#include "HSD_BUTTON.h"
#include "T90.h"
#include "t90font.h"
#include "HSD_SLIDER.h"
#include "DispSOGNums.h"

#include "layout_alarm_set.h"

static const GUI_RECT drawArea = {30, 120, ALARM_SET_WIDTH-30, ALARM_SET_HEIGHT-30};

WM_HWIN spdingAlarmSetWin;

static WM_HWIN buttons;
static WM_HWIN slider;

static int agentdst_set;
static int isSpON;

static const SetWinColor *pColors = setWinColors;
static const SetDlgColor *pColors_Slider = setDlgColors;

/**@brief 超速报警界面滑块的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void mySliderCallback(WM_MESSAGE* pMsg)
{
	WM_MESSAGE myMsg;
	switch(pMsg->MsgId)
	{
		case WM_KEY:
			switch(((WM_KEY_INFO*)(pMsg->Data.p))->Key)
			{
//            case GUI_KEY_MOLEFT:
//                        if(t90_set.sys.motherpos == DEFAULT_RIGHT && t90_set.sys.workmode == DOUBLE_MODE)
//                        {
//                           myMsg.hWin = systemSetDlg;
//                           myMsg.hWinSrc = pMsg->hWin;
//                           myMsg.MsgId = USER_MSG_MOTHERPOS;
//                           myMsg.Data.v = DEFAULT_LEFT;
//                           WM_SendMessage(myMsg.hWin, &myMsg);
//                        }                           
//                        break;
              case GUI_KEY_SOUNDOFF:
                  monitorState = monitorState == ON? OFF: ON;
                  break;
              case GUI_KEY_MORIGHT:
//                        if(t90_set.sys.motherpos == DEFAULT_LEFT && t90_set.sys.workmode == DOUBLE_MODE)
//                        {
                           myMsg.hWin = systemSetDlg;
                           myMsg.hWinSrc = pMsg->hWin;
                           myMsg.MsgId = USER_MSG_MOTHERPOS;
                           myMsg.Data.v = !t90_set.sys.motherpos;
                           WM_SendMessage(myMsg.hWin, &myMsg);
//                        }   
                        break; 
//                  case GUI_KEY_SINGLE:
//                         if(t90_set.sys.workmode == DOUBLE_MODE)
//                         {                            
//                            myMsg.hWin = systemSetDlg;
//                            myMsg.hWinSrc = pMsg->hWin;
//                            myMsg.MsgId = USER_MSG_WORKMODE;
//                            myMsg.Data.v = SINGLE_MODE;
//                            WM_SendMessage(myMsg.hWin, &myMsg);
//                         }
//                         
//                         break;
//                  case GUI_KEY_DOUBLE:
//                         if(t90_set.sys.workmode == SINGLE_MODE)
//                         {
//                            myMsg.hWin = systemSetDlg;
//                            myMsg.hWinSrc = pMsg->hWin;
//                            myMsg.MsgId = USER_MSG_WORKMODE;
//                            myMsg.Data.v = DOUBLE_MODE;
//                            WM_SendMessage(myMsg.hWin, &myMsg);
//                         }
//                         
//                         break;
				case GUI_KEY_PWM_INC:       
						 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
						 break;

                 
            case GUI_KEY_UP:
//                  WM_SetFocus(buttons[1]);
//               break;
            case GUI_KEY_DOWN:
                  WM_SetFocus(buttons);
               break;
				case GUI_KEY_BACKSPACE:
                  isSpON = HSD_SLIDER_GetValue(slider);
                  if(t90_set.alarm.danger_sog == agentdst_set && (t90_set.alarm.on_off & (0x01<<1))>>1 == isSpON)
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

				default:
					HSD_SLIDER_Callback(pMsg);
            break;
			}
		break;
		default:
			HSD_SLIDER_Callback(pMsg);
	}
}

/**@brief 超速报警界面按钮的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myButtonCallback(WM_MESSAGE* pMsg)
{
	WM_MESSAGE myMsg;
//   int id;	
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
               case GUI_KEY_SOUNDOFF:
                  monitorState = monitorState == ON? OFF: ON;
                  break;
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
               case GUI_KEY_BACKSPACE:
                              isSpON = HSD_SLIDER_GetValue(slider);
										if(t90_set.alarm.danger_sog == agentdst_set && (t90_set.alarm.on_off & (0x01<<1))>>1 == isSpON)
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
							 case GUI_KEY_UP:
                      case GUI_KEY_DOWN:
											WM_SetFocus(slider);
										break;
							 case GUI_KEY_RIGHT:

                                 agentdst_set+=5;
										if(agentdst_set>200) agentdst_set=200;
                      
									
                    break;
							 case GUI_KEY_LEFT:

                                 agentdst_set-=5;
                              if(agentdst_set<80) agentdst_set=80;
                               break;                      
            }
            WM_Paint(spdingAlarmSetWin);
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
		      pColors_Slider = &(setDlgColors[pMsg->Data.v]);
           
//				 for(i = 0; i < 2; i++)
//				 {
						HSD_BUTTON_SetBkColor(buttons, pColors->bkColor);
						HSD_BUTTON_SetTextColor(buttons, pColors->textColor);
//				 }
             
             HSD_SLIDER_SetBkColor(slider, pColors->bkColor);
				 HSD_SLIDER_SetFocusBkColor(slider, pColors->bkColor);
             HSD_SLIDER_SetSlotColor(slider, pColors_Slider->slotColor);
				 HSD_SLIDER_SetSliderColor(slider,pColors_Slider->sliderColor);
				 HSD_SLIDER_SetFocusSliderColor(slider, pColors_Slider->focusSliderColor);
				 HSD_SLIDER_SetFocusSlotColor(slider,pColors_Slider->focusSlotColor);
				 break;
		
    case WM_CREATE:		
			
            agentdst_set = t90_set.alarm.danger_sog;
            isSpON = (t90_set.alarm.on_off & (0x01<<1))>>1;        

            pColors = &setWinColors[t90_set.sys.nightmode];
            pColors_Slider = &setDlgColors[t90_set.sys.nightmode];
            buttons  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2-45,
                             ALARM_SET_HEIGHT/2-70,  
                             1,        //163
                             1,          //94
                             pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
            WM_SetCallback(buttons, &myButtonCallback); 
            WM_HideWindow(buttons);



            slider = HSD_SLIDER_CreateEx(drawArea.x0 + 175, drawArea.y0 - 60,
                         SLIDER_WIDTH , SLIDER_HEIGHT ,
                         pMsg->hWin , WM_CF_SHOW, 0, GUI_ID_SLIDER0);    
            HSD_SLIDER_SetRange(slider,0,1);
            HSD_SLIDER_SetBkColor(slider, pColors_Slider->bkColor);
            HSD_SLIDER_SetFocusBkColor(slider, pColors_Slider->bkColor);
            HSD_SLIDER_SetSlotColor(slider, pColors_Slider->slotColor);
            HSD_SLIDER_SetSliderColor(slider,pColors_Slider->sliderColor);
            HSD_SLIDER_SetFocusSliderColor(slider, pColors_Slider->focusSliderColor);
            HSD_SLIDER_SetFocusSlotColor(slider,pColors_Slider->focusSlotColor);
            WM_SetCallback(slider, &mySliderCallback);
            HSD_SLIDER_SetValue(slider,(t90_set.alarm.on_off & (0x01<<1))>>1);             
//				 WM_DefaultProc(pMsg);
         break;
         
    case WM_PAINT:

            GUI_SetBkColor(pColors->bkColor);
            WM_GetClientRectEx(pMsg->hWin, &clientRect);
            GUI_ClearRectEx(&clientRect);
            GUI_SetColor(pColors->textColor);
            GUI_SetFont(&GUI_Font_T90_24);

            GUI_DispStringAt("使用",50, ALARM_SET_HEIGHT-30-32);
            GUI_SetColor(pColors->focusBkColor);
            GUI_DispString("  卞  ");
            GUI_SetColor(pColors->textColor);
            GUI_DispString("选择选项，使用");
            GUI_SetColor(pColors->focusBkColor);
            GUI_DispString(" 咗祐 ");
            GUI_SetColor(pColors->textColor);
            GUI_DispString("选择数字。");


            GUI_SetDrawMode(GUI_DM_NORMAL);
            GUI_SetColor(pColors->textColor);


            GUI_SetFont(&GUI_Font_T90_30);
            GUI_DispStringAt("报警航速", 50, ALARM_SET_HEIGHT/2-10);
            GUI_DispStringAt("节", ALARM_SET_WIDTH-70, ALARM_SET_HEIGHT/2-10);
            GUI_DispStringAt("超速报警：", drawArea.x0, drawArea.y0 - 60);
            GUI_DispStringAt("关闭", drawArea.x0+125, drawArea.y0 - 60);
            GUI_DispStringAt("开启", drawArea.x0+125+SLIDER_WIDTH+50,drawArea.y0 - 60);
            if(WM_HasFocus(buttons))
            {
               GUI_SetColor(pColors->focusBkColor);
               GUI_FillRect(ALARM_SET_WIDTH/2-61,ALARM_SET_HEIGHT/2-47, ALARM_SET_WIDTH/2 + 128,ALARM_SET_HEIGHT/2+24);
               GUI_SetColor(pColors->focusTextColor);
               DispSOGNums(ALARM_SET_WIDTH/2-58,ALARM_SET_HEIGHT/2-37,agentdst_set,3);
            }
            else
            {
               GUI_SetColor(pColors->textColor);
               DispSOGNums(ALARM_SET_WIDTH/2-58,ALARM_SET_HEIGHT/2-37,agentdst_set,3);
            }
            
            
            break;
		
		case WM_SET_FOCUS:
				 if(pMsg->Data.v)
				 {
						WM_SetFocus(slider);
				 }
				 break;
				 
		 case USER_MSG_REPLY:
					if(pMsg->Data.v == REPLY_OK)
					{
						 t90_set.alarm.danger_sog = agentdst_set;
                   T90_setAlarmON_OFF(isSpON,1);
						 T90_Store();
					}
					else
					{
//						sprintf(pStrBuf,"%d",t90_set.alarm.danger_sog/10);
//						HSD_BUTTON_SetText(buttons[0], pStrBuf);
//						sprintf(pStrBuf,"%d",t90_set.alarm.danger_sog%10);
//						HSD_BUTTON_SetText(buttons[1], pStrBuf);
						agentdst_set = t90_set.alarm.danger_sog;
                  isSpON = (t90_set.alarm.on_off & (0x01<<1))>>1;
                  HSD_SLIDER_SetValue(slider,isSpON);
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




