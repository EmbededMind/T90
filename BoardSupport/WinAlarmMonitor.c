#include "GUI.h"
#include "WM.h"
#include "maintask.h"
#include "layout_single_ship.h"
#include "boat_struct.h"
#include "dlg.h"
//#include "Draw.h"
#include "T90.h"
#include "snap.h"
#include "plot.h"
#include "Check.h"

//#define MNT_LABEL_TIME   3

/** @brief 单拖模式窗口的句柄 */
WM_HWIN alarmMonitorWin;


static WM_HTIMER timer;

static int timeCnt = 0;
//static int mntLabelTimeCnt;

static void _onPaint(void);
static const HomeColor *pColor = homeColors;


/**@brief 单拖模式下的主界面
 *
 *   @param [in] pMsg 消息指针 
 */

static void myWindowCallback(WM_MESSAGE* pMsg){

   switch(pMsg->MsgId){
		 case USER_MSG_SKIN:			    
					pColor = &homeColors[pMsg->Data.v];
					break;
		 
		 case WM_TIMER:
			 
					if(!CHECK_GetAlarmState())
					{
						WM_DeleteTimer(timer);
						WM_BringToTop(singleShipWin);
						WM_SetFocus(singleShipWin);
					}
					
					timeCnt++;
					WM_Paint(alarmMonitorWin);					
					WM_RestartTimer(timer, 500);
					break;
		 
      case WM_CREATE:
					 pColor = &homeColors[t90_set.sys.nightmode];
           break;     
			
			case WM_SET_FOCUS:	
					 SNAP_Refresh();
					 if(pMsg->Data.v)
					 {					 
						 	GUI_CURSOR_Show();
						 timer = WM_CreateTimer(pMsg->hWin, 0, 500, 0);
					 }
					 else
					 {
						 GUI_CURSOR_Hide();
					 }
					 WM_DefaultProc(pMsg);
					 break;
			
      case WM_KEY:
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){
						 case GUI_KEY_PWM_INC:       
                   WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
                   break;
						 
						 case GUI_KEY_LEFT:
									SNAP_searchNearestObj(-1, 0);
									WM_Paint(alarmMonitorWin);
									break;
						 
						 case GUI_KEY_RIGHT:
									SNAP_searchNearestObj(1, 0);
									WM_Paint(alarmMonitorWin);
									break;
						 
						 case GUI_KEY_UP:
									SNAP_searchNearestObj(0, 1);
									WM_Paint(alarmMonitorWin);
									break;
						 
						 case GUI_KEY_DOWN:
									SNAP_searchNearestObj(0, -1);
									WM_Paint(alarmMonitorWin);
									break;

						 case GUI_KEY_MENU:
	
						      WM_DeleteTimer(timer);
						 
									WM_BringToTop(mainMenuDlg);
									WM_SetFocus(mainMenuDlg);
									break;
						 
//						 case GUI_KEY_MONITORING:
//									monitorState = ON;
//									mntLabelTimeCnt = MNT_LABEL_TIME;
//									WM_Paint(alarmMonitorWin);
//									break;
						 
						 case GUI_KEY_CANCEL:
							    monitorState = monitorState == ON? OFF: ON;
//									mntLabelTimeCnt = MNT_LABEL_TIME;
									WM_Paint(alarmMonitorWin);							
									break;
						 
           }
           break;
           
      case WM_PAINT:
           _onPaint();
           break;
           
     default:
           WM_DefaultProc(pMsg);
           break;
   }
}


/**@brief 创建单拖模式下的主界面窗口
 *
 *  @return 所创建窗口的句柄
 */
WM_HWIN WIN_AlarmMonitorCreate(void){
   WM_HWIN handle; 
   handle  = WM_CreateWindow(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WM_CF_SHOW, &myWindowCallback, 0);
   return handle;
}



static void _onPaint(void)
{	
	Point point;
   GUI_RECT r;
   GUI_SetBkColor(pColor->bkColor);
   WM_GetClientRect(&r);
   GUI_ClearRectEx(&r);
	 GUI_SetTextMode(GUI_TM_TRANS);
   
//   DrawAlarmShip(); 
	GUI_SetColor(pColor->textColor);
	DrawStubs(1);
	GUI_SetColor(GUI_RED);
	DrawAllOtherShips();
	
	SNAP_Refresh();
	
	point.x = pSnapLink->x_to_cross;
	point.y = pSnapLink->y_to_cross;
	GUI_SetColor(pColor->textColor);
	if(point.x == 0 && point.y == 0)
	{
		DrawCursor(GetItemPixel(point), 0);
	}
	else
	{
		DrawCursor(GetItemPixel(point), 1);
	}

	GUI_DrawGradientV(0, 0, SCREEN_WIDTH-1, 45-1, pColor->bbsTopColor, pColor->bbsBottomColor);
	GUI_SetColor(pColor->textColor);                             
  GUI_SetFont(GUI_FONT_24B_1);
	GUI_DispCharAt('N', 15, 10);
	GUI_DispCharAt('E', 165, 10);
	GUI_DispStringAt("SOG:", 335, 10);
	GUI_DispStringAt("kt", 445, 10);
	GUI_DispStringAt("COG:", 480, 10);
	sprintf(pStrBuf, "%02ld/%02ld/%02ld",SYS_Date%100,(SYS_Date%10000)/100,SYS_Date/10000);
	GUI_DispStringAt(pStrBuf, 595, 10);
	sprintf(pStrBuf, "%02ld:%02ld", SYS_Time/10000+8, SYS_Time%10000/100);
	GUI_DispStringAt(pStrBuf, 715, 10);     
  
	GUI_SetColor(pColor->numColor);
	lltostr(mothership.latitude, pStrBuf);
	GUI_DispStringAt(pStrBuf, 40, 10);
	lltostr(mothership.longitude, pStrBuf);
	GUI_DispStringAt(pStrBuf, 195, 10);
	
//	sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
//	GUI_DispStringAt(pStrBuf, 390, 10);
	sprintf(pStrBuf, "%3d", mothership.COG/10);
  pStrBuf[3]  = 194;
  pStrBuf[4]  = 176;
  pStrBuf[5]  = '\0';
	GUI_DispStringAt(pStrBuf, 535, 10);
	sprintf(pStrBuf, "%d", timeCnt);
  GUI_DispStringAt(pStrBuf, 20, 60);
	
	
	
	if(MS_isSpeeding)
	{
		if(timeCnt%2 == 0)
		{
		GUI_SetColor(GUI_RED);
		sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		GUI_DispStringAt(pStrBuf, 390, 10);
		}
	}
	else if(MS_isMax_SOG || MS_isMin_SOG)
	{
		GUI_SetColor(GUI_RED);
		sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		GUI_DispStringAt(pStrBuf, 390, 10);
	}
	else
	{
		GUI_SetColor(pColor->numColor);
		sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		GUI_DispStringAt(pStrBuf, 390, 10);
	}
//	
//	if(mntLabelTimeCnt)
//	{
//		mntLabelTimeCnt--;
	if(monitorState == OFF)
	{
		GUI_SetColor(GUI_RED);
		GUI_FillRoundedRect(20, 60, 160, 110, 8);
		GUI_SetColor(pColor->bkColor);
		GUI_DispStringAt("OFF", 30, 73);	
	}
//		else
//		{
//			GUI_DispStringAt("Monitor OFF", 30, 73);
//		}
//	}
}








