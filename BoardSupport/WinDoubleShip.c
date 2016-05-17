#include "GUI.h"
#include "WM.h"
#include "maintask.h"
#include "layout_double_ship.h"
#include "boat_struct.h"
#include "dlg.h"

#define BBS_ABOVE_H(x)  (BBS_ABOVE_X+x)
#define BBS_ABOVE_V(y)  (BBS_ABOVE_Y+y)
#define BBS_BELOW_H(x)  (BBS_BELOW_X+x)
#define BBS_BELOW_V(y)  (BBS_BELOW_Y+y)

extern boat mothership;

/** @brief 双拖模式窗口的句柄 */
WM_HWIN doubleShipWin;

static void _onPaint(void);


/**@brief 单拖模式下的主界面
 *
 *   @param [in] pMsg 消息指针 
 */
static void myWindowCallback(WM_MESSAGE* pMsg){

   switch(pMsg->MsgId){
			
      case WM_CREATE:
           
           break;
      
			case WM_SET_FOCUS:
		     	
			     WM_DefaultProc(pMsg);
				   break;
			
      case WM_KEY:
           switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key){
						case GUI_KEY_PWM_INC:       
								 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
								 break;
						 case GUI_KEY_UP:
									break;
						 
						 case GUI_KEY_MENU:
									WM_BringToTop(mainMenuDlg);
									WM_SetFocus(mainMenuDlg);
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
WM_HWIN WIN_DoubleShipCreate(void){
   WM_HWIN handle; 
   handle  = WM_CreateWindow(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WM_CF_SHOW, &myWindowCallback, 0);
   return handle;
}



static void _onPaint(void){
//   unsigned char bbsStyleShift  = 10;
   GUI_RECT r;
   GUI_SetColor(GUI_WHITE);
   WM_GetClientRect(&r);
   GUI_FillRectEx(&r);
   /** Paint BBS background */
   GUI_DrawGradientRoundedV( BBS_ABOVE_X,                         /// x0
                             BBS_ABOVE_Y,                         /// y0
                             BBS_ABOVE_X + BBS_WIDTH,             /// x1
                             BBS_ABOVE_Y + BBS_HEIGHT,            /// y1
                             BBS_RADIUS, GUI_WHITE, GUI_DARKGRAY); 

   /** Paint BBS context */ 
   GUI_SetTextMode(GUI_TM_TRANS);          
   GUI_SetColor(GUI_BLACK);
   GUI_SetFont(&GUI_Font32_1);

   GUI_DispStringAt("SOG", BBS_ABOVE_H(40), BBS_ABOVE_V(20));

   GUI_SetFont(&GUI_Font24_1);

   GUI_DispStringAt("COG:", BBS_ABOVE_H(195), BBS_ABOVE_V(20));
	 GUI_DispStringAt("127'", BBS_ABOVE_H(255), BBS_ABOVE_V(20));
   GUI_DispStringAt("kt" , BBS_ABOVE_H(280), BBS_ABOVE_V(120));

   GUI_SetFont(GUI_FONT_D60X80);
   GUI_SetColor(GUI_LIGHTBLUE);
   GUI_DispStringAt("3.25", BBS_ABOVE_H(40), BBS_ABOVE_V(60));



   GUI_DrawGradientRoundedV( BBS_BELOW_X,                         /// x0
                             BBS_BELOW_Y,                         /// y0
                             BBS_BELOW_X + BBS_WIDTH,             /// x1
                             BBS_BELOW_Y + BBS_HEIGHT,            /// y1
                             BBS_RADIUS, GUI_WHITE, GUI_DARKGRAY);  
                             
   GUI_SetColor(GUI_BLACK);                             
   GUI_SetFont(GUI_FONT_32B_1);
   
   GUI_DispCharAt('N', BBS_BELOW_H(40), BBS_BELOW_V(20));
   GUI_DispCharAt('E', BBS_BELOW_H(40), BBS_BELOW_V(60));  
   
   GUI_SetColor(GUI_LIGHTBLUE);   
   GUI_DispStringAt("32'18.116'", BBS_BELOW_H(80), BBS_BELOW_V(20));
   GUI_DispStringAt("118'28.076'", BBS_BELOW_H(80), BBS_BELOW_V(60));
	 
	 GUI_SetColor(GUI_BLACK);                             
   GUI_SetFont(GUI_FONT_32B_1);
	 GUI_DispStringAt("2016/03/01", BBS_BELOW_H(40), BBS_BELOW_V(110)); 
   GUI_DispStringAt("18:00", BBS_BELOW_H(220), BBS_BELOW_V(110));
}


