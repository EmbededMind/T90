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

WM_HWIN sogAlarmSetWin;

static WM_HWIN buttons[3];
static WM_HWIN slider;

static int agent_max_sog;
static int agent_min_sog;
static int isSOGON;

static const SetWinColor *pColors = setWinColors;
static const SetDlgColor *pColors_Slider = setDlgColors;

/**@brief 速度报警界面滑块的回调函数
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

				case GUI_KEY_BACKSPACE:
					   isSOGON = HSD_SLIDER_GetValue(slider);
                  if(t90_set.alarm.max_sog == agent_max_sog && t90_set.alarm.min_sog == agent_min_sog && (t90_set.alarm.on_off & (0x01<<2))>>2 == isSOGON)
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
           case GUI_KEY_UP:              
           case GUI_KEY_DOWN:
               WM_SetFocus(buttons[2]);
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
           }
           else{
              HSD_BUTTON_SetBkColor(pMsg->hWin, pColors->bkColor);
           }
           HSD_BUTTON_Callback(pMsg);
           break;
      
      case WM_KEY:
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key )
					  {
                    case GUI_KEY_SOUNDOFF:
                         monitorState = monitorState == ON? OFF: ON;
                         break;

              
              case GUI_KEY_MORIGHT:

                           myMsg.hWin = systemSetDlg;
                           myMsg.hWinSrc = pMsg->hWin;
                           myMsg.MsgId = USER_MSG_MOTHERPOS;
                           myMsg.Data.v = DEFAULT_RIGHT;
                           WM_SendMessage(myMsg.hWin, &myMsg);   
                        break; 


                       
							 case GUI_KEY_PWM_INC:       
                   WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
                   break;
               case GUI_KEY_BACKSPACE:
                           id = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
                           if(id == 0 || id == 1)
                           {
                              WM_SetFocus(buttons[2]);
                           }
                           else
                           {
                              isSOGON = HSD_SLIDER_GetValue(slider);
                              if(t90_set.alarm.max_sog == agent_max_sog && t90_set.alarm.min_sog == agent_min_sog && (t90_set.alarm.on_off & (0x01<<2))>>2 == isSOGON)
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
                           }
                    break;
							 case GUI_KEY_UP:
										                    index = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;							 
										                    if(index == 2)
                                 WM_SetFocus(slider);
                              if(index == 1)
                              {
                                 agent_max_sog+=5;
                                 if(agent_max_sog >= 100)
                                    agent_max_sog = 95;
                              }
                              if(index == 0)
                              {
                                 agent_min_sog+=5;
                                 if(agent_min_sog >= 100)
                                    agent_min_sog = 95;
                              }
                              if(agent_min_sog>agent_max_sog) agent_min_sog=agent_max_sog;
									                    	break;
                      case GUI_KEY_DOWN:
								 		                  index = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
                              if(index == 2)
                                 WM_SetFocus(slider);
                              else
                              {
                                 if(index==0) agent_min_sog-=5;
                                 if(index==1) agent_max_sog-=5;
                                 if(agent_max_sog<=agent_min_sog) agent_max_sog = agent_min_sog;
                                 if(agent_min_sog<0) agent_min_sog=0;  
                              }
										                    break;
							 case GUI_KEY_RIGHT:
                              id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;

                              if(id==0) 
                              {
                                 WM_SetFocus(buttons[1]);
                              }
                              if(id==1) 
                              {
                                 WM_SetFocus(buttons[0]);
                              }
                             
                              
                              break;
							 case GUI_KEY_LEFT:
                              id  = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
                              
                              if(id==0) WM_SetFocus(buttons[1]);
                              if(id==1) WM_SetFocus(buttons[0]);
                              
	
                              break;
        case GUI_KEY_ENTER:
                id = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
                if(id == 2) WM_SetFocus(buttons[0]);
            break; 
            }
            WM_Paint(sogAlarmSetWin);
            break;
               
     default :
           HSD_BUTTON_Callback(pMsg);
           break;
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
		      pColors_Slider = &(setDlgColors[pMsg->Data.v]);
      
				 for(i = 0; i < 2; i++)
				 {
						HSD_BUTTON_SetBkColor(buttons[i], pColors->bkColor);
						HSD_BUTTON_SetTextColor(buttons[i], pColors->textColor);
				 }
             HSD_SLIDER_SetBkColor(slider, pColors->bkColor);
				 HSD_SLIDER_SetFocusBkColor(slider, pColors->bkColor);
             HSD_SLIDER_SetSlotColor(slider, pColors_Slider->slotColor);
				 HSD_SLIDER_SetSliderColor(slider,pColors_Slider->sliderColor);
				 HSD_SLIDER_SetFocusSliderColor(slider, pColors_Slider->focusSliderColor);
				 HSD_SLIDER_SetFocusSlotColor(slider,pColors_Slider->focusSlotColor);
				 break;
		
    case WM_CREATE:			
			
            agent_max_sog = t90_set.alarm.max_sog;
            agent_min_sog = t90_set.alarm.min_sog;
            isSOGON = (t90_set.alarm.on_off & (0x01<<2))>>2;
             
            pColors = &setWinColors[t90_set.sys.nightmode];
            pColors_Slider = &setDlgColors[t90_set.sys.nightmode];

            buttons[0]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2-180,
                             ALARM_SET_HEIGHT/2-50, 
                             58, 
                             76, 
                             pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
            WM_SetCallback(buttons[0], &myButtonCallback); 
             WM_HideWindow(buttons[0]);

            buttons[1]  = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2+20,
                             ALARM_SET_HEIGHT/2-50, 
                             58, 
                             76, 
                             pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON1);  
            WM_SetCallback(buttons[1], &myButtonCallback); 
             WM_HideWindow(buttons[1]);

            

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
            HSD_SLIDER_SetValue(slider,(t90_set.alarm.on_off & (0x01<<2))>>2); 
            
            buttons[2] = HSD_BUTTON_CreateEx(ALARM_SET_WIDTH/2-45, 
                                ALARM_SET_HEIGHT/2-105,
                                0,
                                0,
                                pMsg->hWin,
                                BUTTON_CF_HIDE,
                                0,
                                GUI_ID_BUTTON2);
            WM_SetCallback(buttons[2], &myButtonCallback);
            
         break;
         
    case WM_PAINT:
                         
            GUI_SetBkColor(pColors->bkColor);
            WM_GetClientRectEx(pMsg->hWin, &clientRect);
            GUI_ClearRectEx(&clientRect);
            if(WM_HasFocus(buttons[2]))
               GUI_DrawGradientRoundedH(ALARM_SET_WIDTH/2-100, ALARM_SET_HEIGHT/2-105,ALARM_SET_WIDTH/2+120, ALARM_SET_HEIGHT/2-75,5,pColors->focusBkColor,pColors->focusBkColor);
            else 
               GUI_DrawGradientRoundedH(ALARM_SET_WIDTH/2-100, ALARM_SET_HEIGHT/2-105,ALARM_SET_WIDTH/2+120, ALARM_SET_HEIGHT/2-75,5,pColors->textColor,pColors->textColor);
    
            GUI_SetColor(pColors->bkColor);
    
            GUI_SetFont(&GUI_Font_T90_30);
            GUI_SetTextMode(GUI_TM_TRANS);
            GUI_DispStringAt("设置正常航速范围", ALARM_SET_WIDTH/2-75, ALARM_SET_HEIGHT/2-105);
            GUI_SetColor(pColors->textColor);

               
            GUI_SetFont(&GUI_Font_T90_24);
//            GUI_SetTextAlign(GUI_TA_HCENTER);
            if(WM_HasFocus(slider))
            {
               GUI_DispStringAt("使用",30, ALARM_SET_HEIGHT-30-32);
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString("  卞  ");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("选择开启/关闭，使用");
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString(" 咗祐 ");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("选择选项。");
		          }
            else if(WM_HasFocus(buttons[2]))
            {
               GUI_DispStringAt("按下",20, ALARM_SET_HEIGHT-30-32);
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString("【确认】");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("按键进入设置，使用");
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString("  卞  ");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("选择选项。");
            }
            else if(WM_HasFocus(buttons[0]) || WM_HasFocus(buttons[1]))
            {
               GUI_DispStringAt("使用",120, ALARM_SET_HEIGHT-30-64);
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString("【返回】");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("退出设置。");
             
               GUI_DispStringAt("使用",50, ALARM_SET_HEIGHT-30-32);
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString(" 咗祐 ");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("选择选项，使用");
               GUI_SetColor(pColors->focusBkColor);
               GUI_DispString("  卞  ");
               GUI_SetColor(pColors->textColor);
               GUI_DispString("调整数值。");
            }
            GUI_SetDrawMode(GUI_DM_NORMAL);
            GUI_SetColor(pColors->textColor);

//            GUI_FillRect(ALARM_SET_WIDTH/2-117, ALARM_SET_HEIGHT/2+6, ALARM_SET_WIDTH/2-107, ALARM_SET_HEIGHT/2+16);
//            GUI_FillRect(ALARM_SET_WIDTH/2+83, ALARM_SET_HEIGHT/2+6, ALARM_SET_WIDTH/2+93, ALARM_SET_HEIGHT/2+16);

            GUI_DrawHLine(ALARM_SET_HEIGHT/2-12, ALARM_SET_WIDTH/2-15, ALARM_SET_WIDTH/2+15);

            GUI_SetFont(&GUI_Font_T90_30);
//            GUI_DispStringAt("设置正常航速范围", ALARM_SET_WIDTH/2-75, ALARM_SET_HEIGHT/2-105);
            GUI_DispStringAt("节", ALARM_SET_WIDTH/2-39, ALARM_SET_HEIGHT/2-8);
            GUI_DispStringAt("节", ALARM_SET_WIDTH/2+161, ALARM_SET_HEIGHT/2-8);
		      
            GUI_DispStringAt("航速报警：", drawArea.x0, drawArea.y0 - 60);
            GUI_DispStringAt("关闭", drawArea.x0+125, drawArea.y0 - 60);
            GUI_DispStringAt("开启", drawArea.x0+125+SLIDER_WIDTH+50,drawArea.y0 - 60);
            if(WM_HasFocus(buttons[0]))
            {
               GUI_SetColor(pColors->focusBkColor);
               GUI_FillRect(ALARM_SET_WIDTH/2-177,ALARM_SET_HEIGHT/2-50, ALARM_SET_WIDTH/2-45 ,ALARM_SET_HEIGHT/2+24);
               GUI_SetColor(pColors->focusTextColor);
               DispSOGNums(ALARM_SET_WIDTH/2-172,ALARM_SET_HEIGHT/2-40,agent_min_sog,4);
            }
            else
            {
               GUI_SetColor(pColors->textColor);
               DispSOGNums(ALARM_SET_WIDTH/2-172,ALARM_SET_HEIGHT/2-40,agent_min_sog,4);
            }
            if(WM_HasFocus(buttons[1]))
            {
               GUI_SetColor(pColors->focusBkColor);
               GUI_FillRect(ALARM_SET_WIDTH/2+25,ALARM_SET_HEIGHT/2-50, ALARM_SET_WIDTH/2+157 ,ALARM_SET_HEIGHT/2+24);
               GUI_SetColor(pColors->focusTextColor);
               DispSOGNums(ALARM_SET_WIDTH/2+30,ALARM_SET_HEIGHT/2-40,agent_max_sog,4);
            }
            else
            {
               GUI_SetColor(pColors->textColor);
               DispSOGNums(ALARM_SET_WIDTH/2+30,ALARM_SET_HEIGHT/2-40,agent_max_sog,4);
            }
            
            
         break;
		
		case WM_SET_FOCUS:
				 if(pMsg->Data.v)
				 {
						WM_SetFocus(slider);
      WM_Paint(sogAlarmSetWin);
				 }
				 break;
         
		case USER_MSG_REPLY:
					if(pMsg->Data.v == REPLY_OK)
					{
						 t90_set.alarm.max_sog = agent_max_sog;
						 t90_set.alarm.min_sog = agent_min_sog;
                   T90_setAlarmON_OFF(isSOGON,2);
						 T90_Store();
					}
					else
					{
						agent_max_sog = t90_set.alarm.max_sog;
						agent_min_sog = t90_set.alarm.min_sog;
						

                  isSOGON = (t90_set.alarm.on_off & (0x01<<2))>>2;
                  HSD_SLIDER_SetValue(slider,isSOGON);
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




