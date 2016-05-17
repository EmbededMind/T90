#include "Config.h"
#include "WM.h"
#include "MainTask.h"
#include "dlg.h"
#include "HSD_BUTTON.h"
#include "T90.h"

#include "layout_alarm_set.h"

static const GUI_RECT drawArea = {30, 30, ALARM_SET_WIDTH-30, ALARM_SET_HEIGHT-30};

WM_HWIN invdAlarmSetWin;

static WM_HWIN button;

static int agent_set;

static const SetWinColor *pColors = setWinColors;


/**@brief 闯入报警界面按钮的回调函数
 *  
 *   @param [in] pMsg 消息指针
 */
static void myButtonCallback(WM_MESSAGE* pMsg)
{ 
	WM_MESSAGE myMsg;
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
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){
							case GUI_KEY_PWM_INC:       
								   WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
								   break;
               case GUI_KEY_BACKSPACE:
										if(t90_set.alarm.invd_dst == agent_set)
										{
											WM_SetFocus(alarmSetMenuDlg);
										}
										else
										{
											myMsg.hWin  = WM_GetClientWindow(confirmWin);
											myMsg.hWinSrc  = invdAlarmSetWin;
											myMsg.MsgId  = USER_MSG_CHOOSE;
											myMsg.Data.v  = SYS_SETTING;
											WM_SendMessage(myMsg.hWin, &myMsg);
											
											WM_BringToTop(confirmWin);
											WM_SetFocus(confirmWin);
										}
                    break;
							 
               case GUI_KEY_UP:
										if(t90_set.sys.unit == NM)
										{
											agent_set+=100;
											if(agent_set > 9900) agent_set = 9900;
										}
										else
										{
											agent_set+=54;
											if(agent_set > 5352) agent_set = 5352;
										}
										WM_Paint(invdAlarmSetWin);
//										WM_InvalidateWindow(button);
										break;
							 
							 case GUI_KEY_DOWN:
										if(t90_set.sys.unit == NM)
										{
											agent_set-=100;
											if(agent_set < 0) agent_set = 0;
										}
										else
										{
											agent_set-=54;
											if(agent_set < 0) agent_set = 0;
										}
										WM_Paint(invdAlarmSetWin);
										break;
							 
							 case GUI_KEY_ENTER:
                    break;
            }
           break;
           
     default :
           HSD_BUTTON_Callback(pMsg);
           break;
   }
}

static void myWindowCallback(WM_MESSAGE* pMsg)
{
	GUI_RECT clientRect;
  switch(pMsg->MsgId){
		
		case USER_MSG_UNIT:
				 if(pMsg->Data.v == NM)
				 {
					 t90_set.alarm.invd_dst = t90_set.alarm.invd_dst/100*100;
					 agent_set = t90_set.alarm.invd_dst;
				 }
				 break;

		case USER_MSG_SKIN:
			   pColors = &(setWinColors[pMsg->Data.v]);	
		
				 HSD_BUTTON_SetBkColor(button, pColors->bkColor);
				 HSD_BUTTON_SetTextColor(button, pColors->textColor);
				 break;
		
    case WM_CREATE:		
			
				 agent_set = t90_set.alarm.invd_dst;
		
				 pColors = &setWinColors[t90_set.sys.nightmode];
				 GUI_SetFont(&GUI_Font24_ASCII);	
         button  = HSD_BUTTON_CreateEx(drawArea.x1-96,
                                   145, 
                                   40, 
                                   GUI_GetFontSizeY(), 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
         WM_SetCallback(button, &myButtonCallback); 
				 HSD_BUTTON_SetTxFont(button, &GUI_Font24_ASCII);
         HSD_BUTTON_SetBkColor(button, pColors->bkColor);
				 HSD_BUTTON_SetTextColor(button, pColors->textColor);
				 HSD_BUTTON_SetTextFocusColor(button, pColors->focusTextColor);
//         HSD_BUTTON_SetTextColor(button, GUI_RED);
		
				 WM_DefaultProc(pMsg);
         break;
         
    case WM_PAINT:
         GUI_SetBkColor(pColors->bkColor);
         WM_GetClientRectEx(pMsg->hWin, &clientRect);
         GUI_ClearRectEx(&clientRect);
         GUI_SetColor(GUI_RED);
//         GUI_SetLineStyle(GUI_LS_SOLID);
//         GUI_DrawRectEx(&drawArea);
		
				 GUI_DrawVLine(ALARM_SET_WIDTH/2+25, ALARM_SET_HEIGHT-30-32, ALARM_SET_HEIGHT-30);
				 GUI_DrawHLine(ALARM_SET_HEIGHT-30-32, ALARM_SET_WIDTH/2+25, ALARM_SET_WIDTH-30);
		
				 GUI_SetDrawMode(GUI_DM_NORMAL);
				 GUI_SetColor(pColors->textColor);
		
				 {
					 int orgX = (drawArea.x0 + drawArea.x1) / 2 - ALARM_RADIUS;
					 int orgY = (drawArea.y0 + ALARM_RADIUS-16);                   //母船头坐标
					 
					 int subX = orgX;
					 int subY = drawArea.y1 - ALARM_RADIUS - 13;                   //拖网头坐标
					 
					 GUI_DrawLine(orgX, orgY, orgX+16, orgY+16);         //画母船
           GUI_DrawVLine(orgX+16, orgY+17, orgY+60);
           GUI_DrawHLine(orgY+60, orgX-15, orgX+15);
           GUI_DrawVLine(orgX-16, orgY+17, orgY+60);          
           GUI_DrawLine(orgX-16, orgY +16, orgX , orgY);
										 
					 GUI_SetColor(pColors->lineColor);
					 GUI_DrawArc(orgX, drawArea.y0 + ALARM_RADIUS, ALARM_RADIUS, ALARM_RADIUS, 0, 180);            //画报警边框
					 GUI_DrawArc(orgX, drawArea.y1 - ALARM_RADIUS, ALARM_RADIUS, ALARM_RADIUS, 180, 360);
					 GUI_DrawVLine(orgX-ALARM_RADIUS, drawArea.y0 + ALARM_RADIUS, drawArea.y1-ALARM_RADIUS);
					 GUI_DrawVLine(orgX+ALARM_RADIUS, drawArea.y0 + ALARM_RADIUS, drawArea.y1-ALARM_RADIUS);
					 
					 GUI_SetColor(pColors->textColor);
					
					 GUI_DrawLine(subX, subY,subX - 12, subY + 26);                                                  //画拖网三角形标志
					 GUI_DrawLine(subX, subY,subX + 12, subY + 26);
					 GUI_DrawLine(subX + 12, subY + 26,subX - 12, subY + 26);
					 
					 GUI_DrawLine(orgX, (orgY+40+subY)/2, orgX+ALARM_RADIUS, (orgY+40+subY)/2);                       //带箭头横线
					 GUI_DrawLine(orgX+ALARM_RADIUS/2, (orgY+40+subY)/2, orgX+ALARM_RADIUS+10, (orgY+40+subY)/2-40);
					 GUI_DrawLine(orgX+ALARM_RADIUS+10, (orgY+40+subY)/2-40, drawArea.x1-20, (orgY+40+subY)/2-40);
					 
					 GUI_DrawHLine((orgY+40+subY)/2-1, orgX+4, orgX+9);                                                //左侧箭头
					 GUI_DrawHLine((orgY+40+subY)/2-2, orgX+7, orgX+9);
					 GUI_DrawHLine((orgY+40+subY)/2+1, orgX+4, orgX+9);
					 GUI_DrawHLine((orgY+40+subY)/2+2, orgX+7, orgX+9);
					 
					 GUI_DrawHLine((orgY+40+subY)/2-1, orgX+ALARM_RADIUS-9, orgX+ALARM_RADIUS-4);                     //右侧箭头
					 GUI_DrawHLine((orgY+40+subY)/2-2, orgX+ALARM_RADIUS-9, orgX+ALARM_RADIUS-7);
					 GUI_DrawHLine((orgY+40+subY)/2+1, orgX+ALARM_RADIUS-9, orgX+ALARM_RADIUS-4);
					 GUI_DrawHLine((orgY+40+subY)/2+2, orgX+ALARM_RADIUS-9, orgX+ALARM_RADIUS-7);
					 
					 GUI_SetLineStyle(GUI_LS_DOT);
					 
					 GUI_DrawLine(orgX, orgY + 60, subX, subY);                                   //中间虚线
					 
					 GUI_SetFont(&GUI_Font24_ASCII);
					 GUI_DispStringAt("range:", orgX+ALARM_RADIUS+20, 145);
					 
					 if(t90_set.sys.unit == NM)
					 {
							GUI_DispStringAt("nm", drawArea.x1-55, 145);
						  sprintf(pStrBuf,"%01d.%01d",agent_set/1000, (agent_set%1000)/100);
					 }
					 else
					 {
						 GUI_DispStringAt("km", drawArea.x1-55, 145);
						 sprintf(pStrBuf,"%01d.%01d",agent_set*37/20000, ((agent_set*37/20)%1000)/100);
					 }
					 HSD_BUTTON_SetText(button, pStrBuf);
INFO("paint");					 
				 }
         break;
		
		case WM_SET_FOCUS:
				 if(pMsg->Data.v)
				 {
						WM_SetFocus(button);
				 }
				 break;
				 
		 case USER_MSG_REPLY:
					if(pMsg->Data.v == REPLY_OK)
					{
						 t90_set.alarm.invd_dst = agent_set;
						 T90_Store();
					}
					else
					{
						agent_set = t90_set.alarm.invd_dst;
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
WM_HWIN WIN_InvdAlarmSetCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindow(ALARM_SET_X, ALARM_SET_Y, ALARM_SET_WIDTH, ALARM_SET_HEIGHT,  WM_CF_SHOW, &myWindowCallback , 0);
   return handle;
}




