#include "GUI.h"
#include "WM.h"
#include "maintask.h"
#include "layout_single_ship.h"
#include "dlg.h"
#include "T90.h"
#include "plot.h"
#include "Check.h"


extern boat mothership;

/** @brief 单拖模式窗口的句柄 */
WM_HWIN singleShipWin;

static WM_HTIMER timer;

const HomeColor *pColor = homeColors;

static void _onPaint1(void);
static void _onPaint2(void);

static int cursorOnStub = 0;
//static Point cursorPixel;

static int timeCnt = 0;


/**@brief 单拖模式下的主界面
 *
 *   @param [in] pMsg 消息指针 
 */
static void myWindowCallback(WM_MESSAGE* pMsg)
{
	GUI_RECT r;
   switch(pMsg->MsgId){
		 
		  case USER_MSG_SKIN:
					 pColor = &homeColors[pMsg->Data.v];
					 break;
			
			case WM_TIMER:
					 
					 if(CHECK_GetAlarmState())
					 {
						 WM_DeleteTimer(timer);
						 WM_BringToTop(alarmMonitorWin);
						 WM_SetFocus(alarmMonitorWin);
					 }					 
					 timeCnt++;
					 WM_Paint(singleShipWin);
					 WM_RestartTimer(timer, 500);
					 break;
		 
      case WM_CREATE:
           pColor = &homeColors[t90_set.sys.nightmode];
           break;
			
	
      case WM_KEY:
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){

						 case GUI_KEY_MENU:						 
									WM_DeleteTimer(timer);					 
									WM_BringToTop(mainMenuDlg);
									WM_SetFocus(mainMenuDlg);
									break;
						 
						 case GUI_KEY_UP:	 
									WM_DeleteTimer(timer);
									WM_BringToTop(alarmMonitorWin);
									WM_SetFocus(alarmMonitorWin);
									break;
						 
						 case GUI_KEY_LEFT:
									cursorOnStub--;
									if(cursorOnStub < 0)
									{
										cursorOnStub = STUB_NUM-1;
									}
									WM_Paint(singleShipWin);
									break;
												 
						 case GUI_KEY_RIGHT:
									cursorOnStub++;
									if(cursorOnStub >= STUB_NUM)
									{
										cursorOnStub = 0;
									}
									WM_Paint(singleShipWin);
									break;
						 
//						 case GUI_KEY_MONITORING:
//									monitorState = ON;
//									WM_Paint(singleShipWin);
//									break;
//						 
						 case GUI_KEY_CANCEL:
									monitorState = monitorState == ON? OFF: ON;
									WM_Paint(singleShipWin);
									break;
						 
						 default:
								  WM_DefaultProc(pMsg);
           }
           break;
           
      case WM_PAINT:

					 GUI_SetBkColor(pColor->bkColor);
				   WM_GetClientRect(&r);
					 GUI_ClearRectEx(&r);
			
					 if(cursorOnStub)
					 {
						 _onPaint2();
					 }
					 else
					 {
					   _onPaint1();
					 }
					
					 if(monitorState == OFF)
					 { 
						 GUI_SetColor(GUI_RED);
						 GUI_FillRoundedRect(20, 15, 109, 49, 6);
						 GUI_SetColor(pColor->bkColor);
						 GUI_SetFont(GUI_FONT_16B_ASCII);
						 GUI_DispStringAt("Monitor OFF", 27, 24);
					 }
           break;
			
			case WM_SET_FOCUS:
					 if(pMsg->Data.v)
					 {
						 	GUI_CURSOR_Show();
						 cursorOnStub = 0;
						 timer  = WM_CreateTimer(pMsg->hWin, 0, 500, 0);
					 }
					 else
					 {
						 GUI_CURSOR_Hide();
					 }
					 WM_DefaultProc(pMsg);
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
WM_HWIN WIN_SingleShipCreate(void){
   WM_HWIN handle; 
   handle  = WM_CreateWindow(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WM_CF_SHOW, &myWindowCallback, 0);
   return handle;
}



static void _onPaint1(void)
{
   /** Paint BBS background */
   GUI_DrawGradientRoundedV( BBS1_ABOVE_X,                         /// x0
                             BBS1_ABOVE_Y,                         /// y0
                             BBS1_ABOVE_X + BBS1_WIDTH-1,             /// x1
                             BBS1_ABOVE_Y + BBS1_HEIGHT-1,            /// y1
                             BBS1_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor); 

   /** Paint BBS context */ 
   GUI_SetTextMode(GUI_TM_TRANS);          
   GUI_SetColor(pColor->textColor);
   GUI_SetFont(&GUI_Font32_1);

   GUI_DispStringAt("SOG", BBS1_ABOVE_X+40, BBS1_ABOVE_Y+20);

   GUI_SetFont(&GUI_Font24_1);
   GUI_DispStringAt("COG:", BBS1_ABOVE_X+195, BBS1_ABOVE_Y+20);
	 sprintf(pStrBuf, "%3d", mothership.COG/10);
   pStrBuf[3]  = 194;
   pStrBuf[4]  = 176;
   pStrBuf[5]  = '\0';
	 GUI_DispStringAt(pStrBuf, BBS1_ABOVE_X+255, BBS1_ABOVE_Y+20);
   GUI_DispStringAt("kt" , BBS1_ABOVE_X+280, BBS1_ABOVE_Y+120);

   GUI_SetFont(GUI_FONT_D80);
	 
	 if(MS_isSpeeding)
	 {
		 if(timeCnt%2 == 0)
		 {
		   GUI_SetColor(GUI_RED);
			 sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
			 GUI_DispStringAt(pStrBuf, BBS1_ABOVE_X+40, BBS1_ABOVE_Y+60);
		 }
	 }
	 else if(MS_isMax_SOG || MS_isMin_SOG)
	 {
		 GUI_SetColor(GUI_RED);
		 sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		 GUI_DispStringAt(pStrBuf, BBS1_ABOVE_X+40, BBS1_ABOVE_Y+60);
	 }
	 else
	 {
		 GUI_SetColor(pColor->numColor);
		 sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		 GUI_DispStringAt(pStrBuf, BBS1_ABOVE_X+40, BBS1_ABOVE_Y+60);
	 }


   GUI_DrawGradientRoundedV( BBS1_BELOW_X,                         /// x0
                             BBS1_BELOW_Y,                         /// y0
                             BBS1_BELOW_X + BBS1_WIDTH-1,             /// x1
                             BBS1_BELOW_Y + BBS1_HEIGHT-1,            /// y1
                             BBS1_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor);  
                             
   GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_32B_1);
   
   GUI_DispCharAt('N', BBS1_BELOW_X+40, BBS1_BELOW_Y+20);
   GUI_DispCharAt('E', BBS1_BELOW_X+40, BBS1_BELOW_Y+60);  
   
   GUI_SetColor(pColor->numColor);  
	 lltostr(mothership.latitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+80, BBS1_BELOW_Y+20);
	 lltostr(mothership.longitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+80, BBS1_BELOW_Y+60);
	 
	 GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_32B_1);
	 sprintf(pStrBuf, "%02ld/%02ld/%02ld",SYS_Date%100,(SYS_Date%10000)/100,SYS_Date/10000);
	 GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+40, BBS1_BELOW_Y+110); 
	 sprintf(pStrBuf, "%02ld:%02ld", SYS_Time/10000+8, SYS_Time%10000/100);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+220, BBS1_BELOW_Y+110);
	 
	 sprintf(pStrBuf, "%d", timeCnt);
   GUI_DispStringAt(pStrBuf, 20, 20);
	 
	GUI_SetColor(pColor->textColor);
	DrawStubs(0);
	DrawCursor(motherShipPixel, 0);
}


static void _onPaint2(void)
{
   /** Paint BBS background */
   GUI_DrawGradientRoundedV( BBS2_ABOVE_X,                         /// x0
                             BBS2_ABOVE_Y,                         /// y0
                             BBS2_ABOVE_X + BBS2_ABOVE_WIDTH-1,             /// x1
                             BBS2_ABOVE_Y + BBS2_ABOVE_HEIGHT-1,            /// y1
                             BBS2_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor); 

   /** Paint BBS context */ 
   GUI_SetTextMode(GUI_TM_TRANS);          
   GUI_SetColor(pColor->textColor);
   GUI_SetFont(GUI_FONT_24B_1);

   GUI_DispStringAt("SOG", BBS2_ABOVE_X+30, BBS2_ABOVE_Y+20);

   GUI_SetFont(GUI_FONT_16B_1);
   GUI_DispStringAt("COG:", BBS2_ABOVE_X+110, BBS2_ABOVE_Y+20);
	 sprintf(pStrBuf, "%3d", mothership.COG/10);
   pStrBuf[3]  = 194;
   pStrBuf[4]  = 176;
   pStrBuf[5]  = '\0';
	 GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+155, BBS2_ABOVE_Y+20);
   GUI_DispStringAt("kt" , BBS2_ABOVE_X+165, BBS2_ABOVE_Y+80);

   GUI_SetFont(GUI_FONT_D48);
	 
	 if(MS_isSpeeding)
	 {
		 if(timeCnt%2 == 0)
		 {
		   GUI_SetColor(GUI_RED);
			 sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
			 GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+20, BBS2_ABOVE_Y+50);
		 }
	 }
	 else if(MS_isMax_SOG || MS_isMin_SOG)
	 {
		 GUI_SetColor(GUI_RED);
		 sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		 GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+20, BBS2_ABOVE_Y+50);
	 }
	 else
	 {
		 GUI_SetColor(pColor->numColor);
		 sprintf(pStrBuf, "%2d.%d", mothership.SOG/10, mothership.SOG%10);
		 GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+20, BBS2_ABOVE_Y+50);
	 }
	 
	 GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_24B_1);
   
   GUI_DispCharAt('N', BBS2_ABOVE_X+200, BBS2_ABOVE_Y+20);
   GUI_DispCharAt('E', BBS2_ABOVE_X+200, BBS2_ABOVE_Y+48);  
   
   GUI_SetColor(pColor->numColor);  
	 lltostr(mothership.latitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+230, BBS2_ABOVE_Y+20);
	 lltostr(mothership.longitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+230, BBS2_ABOVE_Y+48);
	 
	 GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_24B_1);
	 sprintf(pStrBuf, "%02ld/%02ld/%02ld",SYS_Date%100,(SYS_Date%10000)/100,SYS_Date/10000);
	 GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+200, BBS2_ABOVE_Y+76); 
	 sprintf(pStrBuf, "%02ld:%02ld", SYS_Time/10000+8, SYS_Time%10000/100);
   GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+300, BBS2_ABOVE_Y+76);
	 
	 GUI_DrawVLine(BBS2_ABOVE_X+188, BBS2_ABOVE_Y+25, BBS2_ABOVE_Y+BBS2_ABOVE_HEIGHT-20);


   GUI_DrawGradientRoundedV( BBS2_BELOW_X,                         /// x0
                             BBS2_BELOW_Y,                         /// y0
                             BBS2_BELOW_X + BBS2_BELOW_WIDTH-1,             /// x1
                             BBS2_BELOW_Y + BBS2_BELOW_HEIGHT-1,            /// y1
                             BBS2_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor);  
                             
	 GUI_SetFont(GUI_FONT_24B_1);
	 
	 GUI_DispStringAt("name:",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+40);
	 GUI_DispStringAt("MMSI:",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+40+32);
	 GUI_DispStringAt("distance:",  BBS2_BELOW_X+30,  BBS2_BELOW_Y+40+32*2);
	 GUI_DispStringAt("m",          BBS2_BELOW_X+210, BBS2_BELOW_Y+40+32*2);
	 GUI_DispStringAt("offset:",    BBS2_BELOW_X+30,  BBS2_BELOW_Y+40+32*3);
	 GUI_DispStringAt("m",          BBS2_BELOW_X+210, BBS2_BELOW_Y+40+32*3);
	 
	 GUI_SetColor(pColor->numColor);
	 switch(cursorOnStub)
	 {
		 case 1:
			 GUI_DispStringAt("TUOWANG1", BBS2_BELOW_X+110, BBS2_BELOW_Y+40);
			 sprintf(pStrBuf, "%4d", t90_set.dst.dst1);
			 GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+32*2);
			 sprintf(pStrBuf, "%4d", t90_set.dst.dst2);
			 GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+32*3);
			 break;
		 case 2:
			 GUI_DispStringAt("TUOWANG2", BBS2_BELOW_X+110, BBS2_BELOW_Y+40);
			 sprintf(pStrBuf, "%4d", t90_set.dst.dst3);
			 GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+32*2);
			 sprintf(pStrBuf, "%4d", 0);
			 GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+32*3);
			 break;
		 case 3:
			 GUI_DispStringAt("TUOWANG3", BBS2_BELOW_X+110, BBS2_BELOW_Y+40);
			 sprintf(pStrBuf, "%4d", t90_set.dst.dst5);
			 GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+32*2);
			 sprintf(pStrBuf, "%4d", t90_set.dst.dst4);
			 GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+32*3);
			 break;
	 }
	 
	 sprintf(pStrBuf, "%d", timeCnt);
   GUI_DispStringAt(pStrBuf, 20, 20);

	GUI_SetColor(pColor->textColor);
	DrawStubs(0);
	DrawCursor(GetItemPixel(stubs[cursorOnStub].basePoint), 0); 
}















