#include "config.h"
#include "maintask.h"
#include "dlg.h"
#include "HSD_SLIDER.h"
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "T90.h"
#include "t90font.h"

#include "layout_alarm_set.h"
#include "xt_isd.h"
#define GUI_ID_WINDOW0           (GUI_ID_USER + 0x00)

WM_HWIN specialAlarmSetDlg;

static WM_HWIN slider[6];

static char agentdst_set;

static const SetDlgColor *pColors = setDlgColors;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ WINDOW_CreateIndirect, "Window", GUI_ID_WINDOW0, 0,               0,                                            ALARM_SET_WIDTH,   ALARM_SET_HEIGHT,   0, 0, 0},
	{ HSD_SLIDER_CreateIndirect, "s0", GUI_ID_SLIDER0, SLIDER_ORG_X,    SLIDER_ORG_Y,                                 SLIDER_WIDTH,      SLIDER_HEIGHT,      0, 0, 0},
	{ HSD_SLIDER_CreateIndirect, "s1", GUI_ID_SLIDER1, SLIDER_ORG_X,    SLIDER_ORG_Y+SLIDER_HEIGHT+SLIDER_MARGIN,     SLIDER_WIDTH,      SLIDER_HEIGHT,      0, 0, 0},
	{ HSD_SLIDER_CreateIndirect, "s2", GUI_ID_SLIDER2, SLIDER_ORG_X,    SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*2, SLIDER_WIDTH,      SLIDER_HEIGHT,      0, 0, 0},
	{ HSD_SLIDER_CreateIndirect, "s3", GUI_ID_SLIDER3, SLIDER_ORG_X,    SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*3, SLIDER_WIDTH,      SLIDER_HEIGHT,      0, 0, 0},
	{ HSD_SLIDER_CreateIndirect, "s4", GUI_ID_SLIDER4, SLIDER_ORG_X,    SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*4, SLIDER_WIDTH,      SLIDER_HEIGHT,      0, 0, 0},
	{ HSD_SLIDER_CreateIndirect, "s5", GUI_ID_SLIDER5, SLIDER_ORG_X,    SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*5, SLIDER_WIDTH,      SLIDER_HEIGHT,      0, 0, 0}
};

/**@brief 特殊报警界面滑条的回调函数
 *
 * @param [in] pMsg 消息指针
 */
static void mySliderCallback(WM_MESSAGE* pMsg)
{
	WM_MESSAGE myMsg;
	switch(pMsg->MsgId)
	{
		case WM_KEY:
			switch(((WM_KEY_INFO*)(pMsg->Data.p))->Key)
			{
    case WM_SET_FOCUS:
     if(pMsg->Data.v)
     {
        HSD_SLIDER_Callback(pMsg);
     }
     else
     {
        WM_SetFocus(slider[0]);
     }
              case GUI_KEY_SOUNDOFF:
                  sound = sound == ON? OFF: ON;
                  ISD_Wait_PWRUp();
                  if(sound)
                  {                     
                     ISD_SetVolumn(t90_set.sys.volum);
                  }
                  else
                  {
                     ISD_SetVolumnZero();
                  }
                  break;
              case GUI_KEY_F2:
                  Silence = !Silence;
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
					if(t90_set.alarm.nation == agentdst_set)
					{
					WM_SetFocus(slider[0]);                  //退出之前把焦点给第一个成员
					WM_SetFocus(alarmSetMenuDlg);
					}
					else
					{
						myMsg.hWin  = WM_GetClientWindow(confirmWin);
						myMsg.hWinSrc  = specialAlarmSetDlg;
						myMsg.MsgId  = USER_MSG_CHOOSE;
						myMsg.Data.v  = SYS_SETTING;
						WM_SendMessage(myMsg.hWin, &myMsg);
						
						WM_BringToTop(confirmWin);
						WM_SetFocus(confirmWin);
					}
				break;

				default:
					HSD_SLIDER_Callback(pMsg);
			}
		break;
		default:
			HSD_SLIDER_Callback(pMsg);
	}
}

/**@brief 特殊报警界面窗口的回调函数
 *
 * @param [in] pMsg 消息指针
 */
static void myDialogCallback(WM_MESSAGE*pMsg)
{
	int i;
	GUI_RECT clientRect;
	int id;
	int sldValue;
	switch(pMsg->MsgId)
	{
		case USER_MSG_SKIN:
			
			pColors = &setDlgColors[pMsg->Data.v];
		
			WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
			for(i = 0; i < 6; i++)
			{	
				HSD_SLIDER_SetBkColor(slider[i], pColors->bkColor);
				HSD_SLIDER_SetFocusBkColor(slider[i], pColors->bkColor);
				HSD_SLIDER_SetSlotColor(slider[i], pColors->slotColor);
				HSD_SLIDER_SetSliderColor(slider[i],pColors->sliderColor);
				HSD_SLIDER_SetFocusSliderColor(slider[i], pColors->focusSliderColor);
				HSD_SLIDER_SetFocusSlotColor(slider[i],pColors->focusSlotColor);
			}
		break;
			
		case WM_INIT_DIALOG:
		
			agentdst_set = t90_set.alarm.nation;
		
			pColors = &setDlgColors[t90_set.sys.nightmode];
		
			WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
		
			for(i = 0; i < 6; i++)
			{
				slider[i] = WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0+i);
				WM_SetCallback(slider[i], &mySliderCallback);
				HSD_SLIDER_SetRange(slider[i], 0, 1);
				HSD_SLIDER_SetValue(slider[i], ((0x01<<i)&t90_set.alarm.nation)>>i);
				
				HSD_SLIDER_SetBkColor(slider[i], pColors->bkColor);
				HSD_SLIDER_SetFocusBkColor(slider[i], pColors->bkColor);
				HSD_SLIDER_SetSlotColor(slider[i], pColors->slotColor);
				HSD_SLIDER_SetSliderColor(slider[i],pColors->sliderColor);
				HSD_SLIDER_SetFocusSliderColor(slider[i], pColors->focusSliderColor);
				HSD_SLIDER_SetFocusSlotColor(slider[i],pColors->focusSlotColor);
			}
			break;
		case WM_PAINT:
                     GUI_SetBkColor(pColors->bkColor);
            WM_GetClientRectEx(pMsg->hWin, &clientRect);
            GUI_ClearRectEx(&clientRect);
			GUI_SetColor(pColors->textColor);
			GUI_SetFont(&GUI_Font_T90_24);
//            GUI_DispStringAt("使用      选择选项，使用      选择数字。",30, ALARM_SET_HEIGHT-62);
            GUI_DispStringAt("使用",50, ALARM_SET_HEIGHT-30-32);
            GUI_SetColor(pColors->focusSliderColor);
            GUI_DispString("  卞  ");
            GUI_SetColor(pColors->textColor);
            GUI_DispString("选择选项，使用");
            GUI_SetColor(pColors->focusSliderColor);
            GUI_DispString(" 咗祐 ");
            GUI_SetColor(pColors->textColor);
            GUI_DispString("切换开关。");
//			GUI_DrawRect(30, ALARM_SET_HEIGHT-62, ALARM_SET_WIDTH-30, ALARM_SET_HEIGHT-30);
			GUI_SetFont(&GUI_Font_T90_30);
			GUI_SetColor(pColors->textColor);
			for(i = 0; i < 6; i++)
			{
				GUI_DispStringAt("关闭", SLIDER_ORG_X-50,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*i);
				GUI_DispStringAt("开启",  SLIDER_ORG_X+SLIDER_WIDTH+2, SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*i);
			}
			GUI_DispStringAt("台湾：", 45,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*0);
			GUI_DispStringAt("日本：", 45,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*1);
			GUI_DispStringAt("韩国：", 45,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*2);
			GUI_DispStringAt("朝鲜：", 45,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*3);
			GUI_DispStringAt("印尼：", 45,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*4);
			GUI_DispStringAt("越南：", 45,             SLIDER_ORG_Y+(SLIDER_HEIGHT+SLIDER_MARGIN)*5);
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_VALUE_CHANGED)
			{
				id = WM_GetId(pMsg->hWinSrc) - GUI_ID_SLIDER0;
				sldValue = HSD_SLIDER_GetValue(slider[id]);
				if(sldValue)
				{
					agentdst_set |= (0x01<<id);
				}
				else
				{
					agentdst_set &= (0xff-(0x01<<id));
				}		
			}
			break;
			
		case USER_MSG_REPLY:
				if(pMsg->Data.v == REPLY_OK)
				{
					 t90_set.alarm.nation = agentdst_set;
					 T90_Store();
				}
				else
				{
					for(i = 0; i < 6; i++)
					{
						HSD_SLIDER_SetValue(slider[i], ((0x01<<i)&t90_set.alarm.nation)>>i);
					}
				}
    WM_SetFocus(slider[0]);
				WM_SetFocus(alarmSetMenuDlg);
				break;
			
		default:
			WM_DefaultProc(pMsg);
	}
}

/**@brief 特殊报警界面窗口的创建
 *
 * @param [out] WM_HWIN 窗口句柄
 */
WM_HWIN DLG_SpecialAlarmSetCreate(void)
{
	WM_HWIN handle;
	handle = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), myDialogCallback, WM_HBKWIN, ALARM_SET_X, ALARM_SET_Y);
	return handle;
}



