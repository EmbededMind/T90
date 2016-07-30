//#include "maintask.h"
//#include "GUI.h"
//#include "DIALOG.h"
//#include "HSD_BUTTON.h"
//#include "dlg.h"
//#include "T90.h"
//#include "Check.h"
//#include "t90font.h"

//#include "layout_sub_menu.h"


//#define ID_WINDOW           (GUI_ID_USER + 0x00)



//WM_HWIN myFleetDlg;

//static const MenuColor *pColors = subMenuColors;

//static WM_HWIN buttons[5];

//static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = {

//};


//static void myButtonCallback(WM_MESSAGE* pMsg)
//{
//   WM_MESSAGE myMsg;
//	int id;
//	static int flag_prevfocus = 0;
//   switch(pMsg->MsgId){
//   case WM_SET_FOCUS:
//        
//        break;
//           
//   case WM_KEY:
//        
//   
//        

//   default:
//       
//        break;
//   }
//}


//static void myDialogCallback(WM_MESSAGE* pMsg)
//{
//  switch(pMsg->MsgId){

//	 case USER_MSG_SKIN:
//						
//						break;
//   case WM_INIT_DIALOG:
//		 
//				
//        break;
//	 					
//	 case WM_PAINT:
//		 
//			
//				GUI_FillRect(0, SUB_MENU_ITEM_HEIGHT*6+SUB_MENU_ITEM_MARGIN*7+20, SUB_MENU_WIDTH+50, SUB_MENU_HEIGHT-1);
//				break;				
//      
//   default:
//        WM_DefaultProc(pMsg);
//   } 
//}


//WM_HWIN DLG_MyFleetSetMenuCreate(void)
//{
//   WM_HWIN handle;
//   
//   handle  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &myDialogCallback, WM_HBKWIN ,SUB_MENU_X+20, SUB_MENU_Y);
//   return handle;
//}
