#include "GUI.h"
#include "WM.h"
#include "maintask.h"
#include "layout_single_ship.h"
#include "dlg.h"
#include "T90.h"
#include "plot.h"
#include "Check.h"
#include "t90font.h"
#include "DispSOGNums.h"
#include "snap.h"
#include "xt_isd.h"
extern boat mothership;
extern Bool toast_flg;

/** @brief 单拖模式窗口的句柄 */
WM_HWIN mainShipWin;

static WM_HTIMER timer;

const HomeColor *pColor = homeColors;



static void _onPaint1(void);
static void _onPaint2(void);

static int tmpCursor;
static int cursorOnStub;
//extern long portStatus[3];

static Stub *pCursorStub = &stubs[0];
static Point cursorPixel;

static int timeCnt = 0;

int getcursor(int position);

/**@brief 单拖模式下的主界面
 *
 *   @param [in] pMsg 消息指针 
 */
static void myWindowCallback(WM_MESSAGE* pMsg)
{
   
   WM_MESSAGE myMsg;
	GUI_RECT r;
   switch(pMsg->MsgId){

		  case USER_MSG_SKIN:
					 pColor = &homeColors[pMsg->Data.v];
					 break;
			
			case WM_TIMER:
					 if(CHECK_GetAlarmState())
					 {
         if(toast_flg == FALSE)
         {
            if(!(t90_set.sys.workmode == DOUBLE_MODE && t90_set.as_MMSI.port == 0))
            {
               WM_DeleteTimer(timer);
               WM_BringToTop(alarmMonitorWin);
               WM_SetFocus(alarmMonitorWin);
            }
         }
					 }					 
					 timeCnt++;
					 WM_Paint(mainShipWin);
					 WM_RestartTimer(timer, 500);
					 break;
		 
      case WM_CREATE:
           pColor = &homeColors[t90_set.sys.nightmode];           
           _onPaint1();
           break;
			
	
      case WM_KEY:
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){          
               case GUI_KEY_MORIGHT:
                  myMsg.hWin = systemSetDlg;
                  myMsg.hWinSrc = pMsg->hWin;
                  myMsg.MsgId = USER_MSG_MOTHERPOS;
                  myMsg.Data.v = !t90_set.sys.motherpos;
                  WM_SendMessage(myMsg.hWin, &myMsg);
                  if(t90_set.sys.workmode == DOUBLE_MODE && t90_set.sys.motherpos == DEFAULT_RIGHT)
                  {
                     cursorOnStub = 4;
                  }
                  else
                  {
                     cursorOnStub = 0;
                  }                           
                  break;
                  
               case GUI_KEY_PWM_INC:
                  WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
                  break;
               
               case GUI_KEY_MENU:
                  WM_DeleteTimer(timer);
                  WM_BringToTop(mainMenuDlg);
                  WM_SetFocus(mainMenuDlg);
                  break;

               case GUI_KEY_UP:	 
                  cursorOnStub = getcursor(UP);                                             
                  WM_Paint(mainShipWin);
                  break;

               case GUI_KEY_DOWN:

                  cursorOnStub = getcursor(DOWN);
                  WM_Paint(mainShipWin);
                  break;

               case GUI_KEY_LEFT:
                  cursorOnStub = getcursor(LEFT);                              
                  WM_Paint(mainShipWin);
                  break;
                               
               case GUI_KEY_RIGHT:
                  cursorOnStub = getcursor(RIGHT);                              
                  WM_Paint(mainShipWin);
                  break;

						 
               case GUI_KEY_SOUNDOFF:
                  monitorState = monitorState == ON? OFF: ON;
                  ISD_Wait_PWRUp();
                  if(monitorState)
                  {                     
                     ISD_SetVolumn(t90_set.sys.volum);
                  }
                  else
                  {
                     ISD_SetVolumnZero();
                  }
//                  if(!ISD_IsBusy())
//                     ISD_PWRDn();
                  WM_Paint(mainShipWin);
                  break;

               default:
                  WM_DefaultProc(pMsg);
           }
           break;
           
      case WM_PAINT:

					 GUI_SetBkColor(pColor->bkColor);
				   WM_GetClientRect(&r);
					 GUI_ClearRectEx(&r);
                 tmpCursor = cursorOnStub;
                if(t90_set.sys.workmode == DOUBLE_MODE)
                {
                   if(cursorOnStub == 0 || cursorOnStub == 4)
                   {
                      if(t90_set.sys.motherpos == DEFAULT_LEFT)
                      {
                         tmpCursor = cursorOnStub;
                      }
                      else
                      {
                         tmpCursor = cursorOnStub == 4? 0:4;
                      }
                   }
                }
					 if(tmpCursor == 0 )
					 {
					    _onPaint1();
					 }
					 else
					 {
						 _onPaint2();
					 }

					 if(monitorState == OFF)
					 { 
						 GUI_SetColor(GUI_RED);
						 GUI_FillRoundedRect(20, 15, 100, 50, 6);
						 GUI_SetColor(pColor->bkColor);
						 GUI_SetFont(&GUI_Font_T90_30);
						 GUI_DispStringAt("静音", 38, 15);
					 }
           break;
			
			case WM_SET_FOCUS:
                if(t90_set.sys.workmode == DOUBLE_MODE && t90_set.sys.motherpos == DEFAULT_RIGHT)
                {
                   cursorOnStub = 4;
                }
                else
                {
                   cursorOnStub = 0;
                }
                if(pMsg->Data.v)
                {                  
          //						 cursorOnStub = 0;
          //						 timeCnt = 0;
                 GUI_CURSOR_Show();
                 timer  = WM_CreateTimer(pMsg->hWin, 0, 500, 0);
                }
                else
                {
                 GUI_CURSOR_Hide();
          //						 timeCnt = 0;
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
   GUI_SetColor(pColor->textColor);
   DrawStubs(0);
   /** Paint BBS background */
   GUI_DrawGradientRoundedV( BBS1_ABOVE_X,                         /// x0
                             BBS1_ABOVE_Y,                         /// y0
                             BBS1_ABOVE_X + BBS1_WIDTH-1,             /// x1
                             BBS1_ABOVE_Y + BBS1_HEIGHT-1,            /// y1
                             BBS1_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor); 

   /** Paint BBS context */ 
   
   GUI_SetTextMode(GUI_TM_TRANS);          
   GUI_SetColor(pColor->textColor);
   GUI_SetFont(&GUI_Font_T90_40);

   GUI_DispStringAt("航速", BBS1_ABOVE_X+60, BBS1_ABOVE_Y+20);

   GUI_SetFont(&GUI_Font_T90_30);
   GUI_DispStringAt("航向：", BBS1_ABOVE_X+215, BBS1_ABOVE_Y+20);
   sprintf(pStrBuf, "%3d", mothership.COG/10);
   GUI_SetFont(&GUI_Font32B_1); 
   pStrBuf[3]  = 194;
   pStrBuf[4]  = 176;
   pStrBuf[5]  = '\0';
   GUI_DispStringAt(pStrBuf, BBS1_ABOVE_X+295, BBS1_ABOVE_Y+20);
   GUI_SetFont(&GUI_Font_T90_30);
   GUI_DispStringAt("节" , BBS1_ABOVE_X+317, BBS1_ABOVE_Y+120);

   GUI_SetFont(GUI_FONT_D80);
	 
   if(MS_isSpeeding)
   {
      if(timeCnt%2 == 0)
      {
         GUI_SetColor(pColor->bullyColor);
                 
         DispSOGNums(BBS1_ABOVE_X+60, BBS1_ABOVE_Y+70, mothership.SOG, BIG);
      }
   }
   else if(MS_isMax_SOG || MS_isMin_SOG)
   {
      GUI_SetColor(pColor->bullyColor);
      DispSOGNums(BBS1_ABOVE_X+60, BBS1_ABOVE_Y+70, mothership.SOG, BIG);
   }
   else
   {
      GUI_SetColor(pColor->numColor);
      DispSOGNums(BBS1_ABOVE_X+60, BBS1_ABOVE_Y+70, mothership.SOG, BIG);
   }


   GUI_DrawGradientRoundedV( BBS1_BELOW_X,                         /// x0
                             BBS1_BELOW_Y,                         /// y0
                             BBS1_BELOW_X + BBS1_WIDTH-1,             /// x1
                             BBS1_BELOW_Y + BBS1_HEIGHT-1,            /// y1
                             BBS1_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor);  
                             
   GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(&GUI_Font_T90_45B);
   if(MS_EWNS & 0x01)
      GUI_DispCharAt('N', BBS1_BELOW_X+60, BBS1_BELOW_Y+20);
   else
      GUI_DispCharAt('S', BBS1_BELOW_X+60, BBS1_BELOW_Y+20);
   if(MS_EWNS & 0x10)
      GUI_DispCharAt('E', BBS1_BELOW_X+60, BBS1_BELOW_Y+60);
   else   
      GUI_DispCharAt('W', BBS1_BELOW_X+60, BBS1_BELOW_Y+60);
   
   GUI_SetColor(pColor->numColor);  
   lltostr(mothership.latitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+110, BBS1_BELOW_Y+20);
   lltostr(mothership.longitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+110, BBS1_BELOW_Y+60);
	 
   GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_32_1);
   sprintf(pStrBuf, "20%02ld/%02ld/%02ld",SYS_Date%100,(SYS_Date%10000)/100,SYS_Date/10000);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+60, BBS1_BELOW_Y+110); 
   sprintf(pStrBuf, "%02ld:%02ld", SYS_Time/10000+8, SYS_Time%10000/100);
   GUI_DispStringAt(pStrBuf, BBS1_BELOW_X+240, BBS1_BELOW_Y+110);
	 
//	 sprintf(pStrBuf, "%d", timeCnt);
//   GUI_DispStringAt(pStrBuf, 20, 20);
	 
	  GUI_SetColor(pColor->textColor);
//	DrawStubs(0);
//	DrawCursor(motherShipPixel, 0);
   DrawCursor(GetItemPixel(stubs[cursorOnStub].basePoint), 0);
   GUI_SetColor(GUI_BLACK);
  	GUI_FillRect(SCREEN_WIDTH, 0, 799, 479);
   GUI_FillRect(0, SCREEN_HEIGHT, 799, 479);
}


static void _onPaint2(void)
{
   int i  = 0;
   BERTH* pBerth  = NULL;
   GUI_RECT Rect = {BBS2_ABOVE_X+230, BBS2_ABOVE_Y+20,BBS2_ABOVE_X+230+117,BBS2_ABOVE_Y+20+40};
   GUI_SetColor(pColor->textColor);
   DrawStubs(0);
   /** Paint BBS background */
   GUI_DrawGradientRoundedV( BBS2_ABOVE_X,                         /// x0
                             BBS2_ABOVE_Y,                         /// y0
                             BBS2_ABOVE_X + BBS2_ABOVE_WIDTH-1,             /// x1
                             BBS2_ABOVE_Y + BBS2_ABOVE_HEIGHT-1,            /// y1
                             BBS2_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor); 

   /** Paint BBS context */ 
   GUI_SetTextMode(GUI_TM_TRANS);          
   GUI_SetColor(pColor->textColor);
   
   GUI_SetFont(&GUI_Font_T90_24);
   GUI_DispStringAt("航速", BBS2_ABOVE_X+30, BBS2_ABOVE_Y+20);

   GUI_SetFont(&GUI_Font_T90_20);
   GUI_DispStringAt("航向：", BBS2_ABOVE_X+110, BBS2_ABOVE_Y+20);
   GUI_SetFont(GUI_FONT_16B_1);
   sprintf(pStrBuf, "%3d", mothership.COG/10);
   pStrBuf[3]  = 194;
   pStrBuf[4]  = 176;
   pStrBuf[5]  = '\0';
   GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+150, BBS2_ABOVE_Y+23);
   GUI_SetFont(&GUI_Font_T90_20);
   GUI_DispStringAt("节" , BBS2_ABOVE_X+173, BBS2_ABOVE_Y+80);

   GUI_SetFont(GUI_FONT_D48);
	 
   if(MS_isSpeeding)
   {
      if(timeCnt%2 == 0)
      {
         GUI_SetColor(pColor->bullyColor);
         DispSOGNums(BBS2_ABOVE_X+20, BBS2_ABOVE_Y+50,mothership.SOG, SMALL);
      }
   }
   else if(MS_isMax_SOG || MS_isMin_SOG)
   {
      GUI_SetColor(pColor->bullyColor);
      DispSOGNums(BBS2_ABOVE_X+20, BBS2_ABOVE_Y+50, mothership.SOG, SMALL);
   }
   else
   {
      GUI_SetColor(pColor->numColor);
      DispSOGNums(BBS2_ABOVE_X+20, BBS2_ABOVE_Y+50, mothership.SOG, SMALL);
   }
	 
   GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_24B_1);

   GUI_DispCharAt('N', BBS2_ABOVE_X+200, BBS2_ABOVE_Y+20);
   GUI_DispCharAt('E', BBS2_ABOVE_X+200, BBS2_ABOVE_Y+48);  

   GUI_SetColor(pColor->numColor);
   lltostr(mothership.latitude, pStrBuf);

   GUI_DispStringInRect(pStrBuf,&Rect,GUI_TA_RIGHT);
   lltostr(mothership.longitude, pStrBuf);

   Rect.y0 = BBS2_ABOVE_Y+48;
   Rect.y1 = BBS2_ABOVE_Y+68;
   GUI_DispStringInRect(pStrBuf,&Rect,GUI_TA_RIGHT);

   GUI_SetColor(pColor->textColor);                             
   GUI_SetFont(GUI_FONT_20_1);
   sprintf(pStrBuf, "20%02ld/%02ld/%02ld",SYS_Date%100,(SYS_Date%10000)/100,SYS_Date/10000);
   GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+200, BBS2_ABOVE_Y+79); 
   sprintf(pStrBuf, "%02ld:%02ld", SYS_Time/10000+8, SYS_Time%10000/100);
   GUI_DispStringAt(pStrBuf, BBS2_ABOVE_X+300, BBS2_ABOVE_Y+79);

   GUI_DrawVLine(BBS2_ABOVE_X+191, BBS2_ABOVE_Y+25, BBS2_ABOVE_Y+BBS2_ABOVE_HEIGHT-20);


   GUI_DrawGradientRoundedV( BBS2_BELOW_X,                         /// x0
                             BBS2_BELOW_Y,                         /// y0
                             BBS2_BELOW_X + BBS2_BELOW_WIDTH-1,             /// x1
                             BBS2_BELOW_Y + BBS2_BELOW_HEIGHT-1,            /// y1
                             BBS2_RADIUS, pColor->bbsTopColor, pColor->bbsBottomColor);  
                             
	 GUI_SetFont(&GUI_Font_T90_30);
	 
	 
  if(t90_set.sys.workmode==SINGLE_MODE)
  {
     GUI_DispStringAt("船名：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+35);
	    GUI_DispStringAt("MMSI：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+35+40);
	    GUI_DispStringAt("距离船尾：",  BBS2_BELOW_X+30,  BBS2_BELOW_Y+35+40*2);
     GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+35+40*2);
  }
  else
  {
     GUI_DispStringAt("船名：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+25);
	    GUI_DispStringAt("MMSI：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38);
//	    GUI_DispStringAt("MMSI：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*2);
//     GUI_DispStringAt("MMSI：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*3);
//     GUI_DispStringAt("MMSI：",      BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*4);
  }
	 
	 
  if(tmpCursor < 4)
       DrawBlueTri(GetItemPixel(stubs[tmpCursor].basePoint));
  if(t90_set.sys.workmode == SINGLE_MODE)
  {   
     switch(tmpCursor)
     {
      case 1:
              GUI_SetColor(pColor->numColor);
              GUI_SetFont(GUI_FONT_24B_1);
              for(i=0; i<N_boat; i++){
                 if(portStatus[0].MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                    GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+40);
                    break;
                 }
              }

              if(portStatus[0].port)
              {
                 sprintf(pStrBuf, "%09ld", portStatus[0].MMSI);
                 GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+35+45);
              }
              sprintf(pStrBuf, "%4d", t90_set.singledst_set.dst1);
              GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+40*2);
              sprintf(pStrBuf, "%4d", t90_set.singledst_set.dst2);
              GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+45+40*3);
              GUI_SetFont(&GUI_Font_T90_30);
              GUI_SetColor(pColor->textColor);
              GUI_DispStringAt("左舷偏移：",    BBS2_BELOW_X+30,  BBS2_BELOW_Y+40+40*3);
              GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+40+40*3);
              
              break;
      case 2:
              GUI_SetColor(pColor->numColor);
              GUI_SetFont(GUI_FONT_24B_1);
              
              
              for(i=0; i<N_boat; i++){
                 if(portStatus[1].MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                    GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+40);
                    break;
                 }
              }
               
              if(portStatus[1].port)
              {
                 sprintf(pStrBuf, "%09ld", portStatus[1].MMSI);
                 GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+35+45);
              }
              sprintf(pStrBuf, "%4d", t90_set.singledst_set.dst3);
              GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+40*2);
              sprintf(pStrBuf, "%4d", 0);
              GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+45+40*3);
              GUI_SetFont(&GUI_Font_T90_30);
              GUI_SetColor(pColor->textColor);
              GUI_DispStringAt("左舷偏移：",    BBS2_BELOW_X+30,  BBS2_BELOW_Y+40+40*3);
              GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+40+40*3);

              break;
      case 3:
              GUI_SetColor(pColor->numColor);
              GUI_SetFont(GUI_FONT_24B_1);
              
              for(i=0; i<N_boat; i++){
                 if(portStatus[2].MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                    GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+40);
                    break;
                 }
              }

              if(portStatus[2].port)
              {
                 sprintf(pStrBuf, "%09ld", portStatus[2].MMSI);
                 GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+35+45);
              }
              sprintf(pStrBuf, "%4d", t90_set.singledst_set.dst5);
              GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+40+40*2);

              sprintf(pStrBuf, "%4d", t90_set.singledst_set.dst4);
              GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+45+40*3);

              GUI_SetFont(&GUI_Font_T90_30);
              GUI_SetColor(pColor->textColor);
              GUI_DispStringAt("右舷偏移：",    BBS2_BELOW_X+30,  BBS2_BELOW_Y+40+40*3);
              GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+40+40*3);
              break;
      
          default :
            printf("tmpCursor error\n"); 
             break;
     }
  }
  else if(t90_set.sys.workmode == DOUBLE_MODE)
  {
      switch(tmpCursor){
       case 1:
          GUI_SetColor(pColor->numColor);
          GUI_SetFont(GUI_FONT_24B_1);
          for(i=0; i<N_boat; i++){
             if(portStatus[0].MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+30); 
                break;
             }
          }
          if(portStatus[0].port)
          {
             sprintf(pStrBuf, "%09ld", portStatus[0].MMSI);
             GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+30+38);
          }
          
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.mo_to_as);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*2);
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.safety1_to_mo);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*3);
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.net_port);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*4);
          
          GUI_SetFont(&GUI_Font_T90_30);
          GUI_SetColor(pColor->textColor);
          GUI_DispStringAt("两船间距：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("绳长：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*3);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*3);
          GUI_DispStringAt("网口：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*4);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*4);
          break;
       case 2:
          GUI_SetColor(pColor->numColor);
          GUI_SetFont(GUI_FONT_24B_1);                         
          for(i=0; i<N_boat; i++){
             if(portStatus[1].MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+30);
                break;
             }
          }
          if(portStatus[1].port)
          {
             sprintf(pStrBuf, "%09ld", portStatus[1].MMSI);
             GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+30+38);
          }
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.mo_to_as);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*2);
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.safety2_to_mo);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*3);
          
          GUI_SetFont(&GUI_Font_T90_30);
          GUI_SetColor(pColor->textColor);
          GUI_DispStringAt("两船间距：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("绳网总长：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*3);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*3);          
          break;
       case 3:
          GUI_SetColor(pColor->numColor);
          GUI_SetFont(GUI_FONT_24B_1);              
          for(i=0; i<N_boat; i++){
             if(portStatus[2].MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+30); 
                break;
             }
          }          

          if(portStatus[2].port)
          {
             sprintf(pStrBuf, "%09ld", portStatus[2].MMSI);
             GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+30+38);
          }
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.mo_to_as);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*2);
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.safety3_to_mo);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*3);
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.net_port);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*4);
          
          GUI_SetFont(&GUI_Font_T90_30);
          GUI_SetColor(pColor->textColor);
          GUI_DispStringAt("两船间距：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("绳长：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*3);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*3);
          GUI_DispStringAt("网口：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*4);
          GUI_DispStringAt("米",          BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*4);
          break;
       case 4:
          GUI_SetColor(pColor->numColor);
          GUI_SetFont(GUI_FONT_24B_1);          
          for(i=0; i<N_boat; i++){
             if(t90_set.as_MMSI.MMSI == SimpBerthes[i].pBerth->Boat.user_id){
                GUI_DispStringAt(SimpBerthes[i].pBerth->Boat.name, BBS2_BELOW_X+110, BBS2_BELOW_Y+30);
                break;
             }
          }             
          sprintf(pStrBuf, "%09ld", t90_set.as_MMSI.MMSI);
          GUI_DispStringAt(pStrBuf,  BBS2_BELOW_X+115, BBS2_BELOW_Y+30+38);
          sprintf(pStrBuf, "%4d", t90_set.doubledst_set.mo_to_as);
          GUI_DispStringAt(pStrBuf, BBS2_BELOW_X+140, BBS2_BELOW_Y+30+38*2);
          
//          GUI_DispStringAt("TUOWANG-2", BBS2_BELOW_X+130, BBS2_BELOW_Y+30);
//          GUI_DispStringAt("123456789",  BBS2_BELOW_X+130, BBS2_BELOW_Y+30+38);
          
          GUI_SetFont(&GUI_Font_T90_30);
          GUI_SetColor(pColor->textColor);
          GUI_DispStringAt("两船间距：", BBS2_BELOW_X+30,  BBS2_BELOW_Y+25+38*2);
          GUI_DispStringAt("米",         BBS2_BELOW_X+210, BBS2_BELOW_Y+25+38*2);
          break;
       default:    
          break;
      }
  }
  
	 GUI_SetColor(pColor->textColor);
//	 DrawStubs(0);
	 DrawCursor(GetItemPixel(stubs[cursorOnStub].basePoint), 0);
  GUI_SetColor(GUI_BLACK);
	 GUI_FillRect(SCREEN_WIDTH, 0, 799, 479);    
}

int getcursor(int position)
{
   int i, which,diff;
   
   diff = 99999;
   which = cursorOnStub;
   
   switch(position){
      case UP:
         for(i = 0; i < STUB_NUM_MAX_D; i++)
         {
            if((stubs[i].isValid == TRUE) && (stubs[i].basePoint.y > stubs[cursorOnStub].basePoint.y))
            {
               if(diff > stubs[i].basePoint.y - stubs[cursorOnStub].basePoint.y)
               {
                  diff =  stubs[i].basePoint.y - stubs[cursorOnStub].basePoint.y;
                  which = i;
               }
            }
         }
         break;
      case DOWN:
         for(i = 0; i < STUB_NUM_MAX_D; i++)
         {
            if((stubs[i].isValid == TRUE) && (stubs[i].basePoint.y < stubs[cursorOnStub].basePoint.y))
            {
               if(diff > stubs[cursorOnStub].basePoint.y - stubs[i].basePoint.y)
               {
                  diff =  stubs[cursorOnStub].basePoint.y - stubs[i].basePoint.y;
                  which = i;
               }
            }
         }
         break;
      case LEFT:
         for(i = 0; i < STUB_NUM_MAX_D; i++)
         {
            if((stubs[i].isValid == TRUE) && (stubs[i].basePoint.x < stubs[cursorOnStub].basePoint.x))
            {
               if(diff > stubs[cursorOnStub].basePoint.x - stubs[i].basePoint.x)
               {
                  diff =  stubs[cursorOnStub].basePoint.x - stubs[i].basePoint.x;
                  which = i;
               }
            }
         }
         break;
      case RIGHT:
         for(i = 0; i < STUB_NUM_MAX_D; i++)
         {
            if((stubs[i].isValid == TRUE) && (stubs[i].basePoint.x > stubs[cursorOnStub].basePoint.x))
            {
               if(diff > stubs[i].basePoint.x - stubs[cursorOnStub].basePoint.x)
               {
                  diff =  stubs[i].basePoint.x - stubs[cursorOnStub].basePoint.x;
                  which = i;
               }
            }
         }
         break;
   }
   return which;
}













