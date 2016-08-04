#include "DIALOG.h"
#include "Config.h"
#include "MainTask.h"
#include "dlg.h"
#include "str.h"
#include "HSD_SLIDER.h"
#include "28.h"
#include "GUI.h"
#include "pwm.h"
#include "sound.h"
#include "string.h"
#include "lpc177x_8x_nvic.h"
#include "T90.h"
#include "xt_isd.h"
#include "t90font.h"
#include "stub.h"
#include "comm.h"
#include "HSD_BUTTON.h"
#include "HSD_Toast.h"
#include "layout_system_set.h"


#define SLD_NUM  8

WM_HWIN systemSetDlg;



static void sldListener(WM_MESSAGE * pMsg);
static void sldResetCallback(WM_MESSAGE* pMsg);


static const SetDlgColor *pColors = setDlgColors;
static const SetWinColor *pColors_win =  setWinColors;

static void _OnWorkModeChanged(WM_MESSAGE * pMsg,int val);
static void _OnLaunchChanged(WM_MESSAGE * pMsg,int val);
static void _OnNightModeChanged(WM_MESSAGE * pMsg,int val);
static void _OnVolumChanged(WM_MESSAGE * pMsg,int val);
static void _OnBrightChanged(WM_MESSAGE * pMsg,int val);
static void _OnUnitChanged(WM_MESSAGE * pMsg,int val);
static void _OnUpdateChanged(WM_MESSAGE * pMsg,int val);
static void _OnSOGAverageChanged(WM_MESSAGE * pMsg,int val);
static void _OnCOGAverageChanged(WM_MESSAGE * pMsg,int val);

static System_Set agentsys_set;


static  void (* const ProcChanging[10-1])(WM_MESSAGE *, int)  = {
	  _OnWorkModeChanged,
	  _OnLaunchChanged,
   _OnNightModeChanged,
   _OnVolumChanged,
   _OnBrightChanged,
   _OnSOGAverageChanged,
   _OnCOGAverageChanged,
   _OnUnitChanged,
	  _OnUpdateChanged
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = 
{ 
 { WINDOW_CreateIndirect,  "Window",  ID_WINDOW_0, 0,                 0,                                                                   SYSTEM_SET_WIDTH,    SYSTEM_SET_HEIGHT,        0, 0, 0},

 { HSD_SLIDER_CreateIndirect, "sld0", ID_SLIDER_0, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y,                                                   SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld1", ID_SLIDER_1, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+ SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN,    SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld2", ID_SLIDER_2, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*2, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld3", ID_SLIDER_3, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*3, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld4", ID_SLIDER_4, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*4, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld5", ID_SLIDER_5, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*5, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld6", ID_SLIDER_6, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*6, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld7", ID_SLIDER_7, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*7, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld8", ID_SLIDER_8, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*8, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld9", ID_SLIDER_9, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*9, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0}
};


static WM_HWIN slideres[10];
static WM_HWIN buttons[2];

static void  _cbDialog(WM_MESSAGE * pMsg)
{
   WM_MESSAGE myMsg;
   
   int val;
   int sldId;
   int i;
      
   switch(pMsg->MsgId)
   {  
      case USER_MSG_MOTHERPOS:
           t90_set.sys.motherpos = pMsg->Data.v;
           memcpy(&agentsys_set, &t90_set.sys, sizeof(t90_set.sys));
           T90_Store();
           if(t90_set.sys.workmode == SINGLE_MODE || t90_set.sys.motherpos == DEFAULT_LEFT)
           {
              for(i = 0; i < 3; i++)
              {
						           if(portStatus[i].port)
                     Comm_addFrame(i+1,stubs[i+1].basePoint.x*MILLINM_TO_M,abs(stubs[i+1].basePoint.y)*MILLINM_TO_M);
              }
           }
           else
           {                            
              for(i = 0; i < 3; i++)
              {
						           if(portStatus[i].port)
                     Comm_addFrame(i+1,(stubs[i+1].basePoint.x - stubs[4].basePoint.x)*MILLINM_TO_M,abs(stubs[i+1].basePoint.y*MILLINM_TO_M));
              }                         
           }
           break;
//      case USER_MSG_WORKMODE:
//           HSD_SLIDER_SetValue(slideres[0],pMsg->Data.v);
//           t90_set.sys.workmode = HSD_SLIDER_GetValue(slideres[0]);
//           T90_Store();
//           memcpy(&agentsys_set, &t90_set.sys, sizeof(t90_set.sys));
//           StubRefresh();
//           if(t90_set.sys.workmode == SINGLE_MODE || t90_set.sys.motherpos == DEFAULT_LEFT)
//           {
//              for(i = 0; i < 3; i++)
//              {
//					            if(portStatus[i].port)
//                    Comm_addFrame(i+1,stubs[i+1].basePoint.x*MILLINM_TO_M,abs(stubs[i+1].basePoint.y)*MILLINM_TO_M);
//              }
//           }
//           else
//           {                            
//              for(i = 0; i < 3; i++)
//              {
//					            if(portStatus[i].port)
//                    Comm_addFrame(i+1,(stubs[i+1].basePoint.x - stubs[4].basePoint.x)*MILLINM_TO_M,abs(stubs[i+1].basePoint.y*MILLINM_TO_M));
//              }                         
//           }
//           break;      
      case USER_MSG_DIM:   
           HSD_SLIDER_Loop(slideres[4]);
           t90_set.sys.bright = HSD_SLIDER_GetValue(slideres[4]);
           T90_Store();
           memcpy(&agentsys_set, &t90_set.sys, sizeof(t90_set.sys));
           StubRefresh();
           break;
      case USER_MSG_SKIN:    
           pColors  = &(setDlgColors[pMsg->Data.v]);
           
           WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
           for(i = 0; i < SLD_NUM; i++)
           {	
              HSD_SLIDER_SetBkColor(slideres[i], pColors->bkColor);
              HSD_SLIDER_SetFocusBkColor(slideres[i], pColors->bkColor);
              HSD_SLIDER_SetSlotColor(slideres[i], pColors->slotColor);
              HSD_SLIDER_SetSliderColor(slideres[i],pColors->sliderColor);
              HSD_SLIDER_SetFocusSliderColor(slideres[i], pColors->focusSliderColor);
              HSD_SLIDER_SetFocusSlotColor(slideres[i],pColors->focusSlotColor);
           }
           
           break;
           
      case WM_INIT_DIALOG:  
           agentsys_set.workmode  =  t90_set.sys.workmode;
           agentsys_set.launch    =  t90_set.sys.launch;
           agentsys_set.nightmode =  t90_set.sys.nightmode;
           agentsys_set.volum     =  t90_set.sys.volum;
           agentsys_set.bright    =  t90_set.sys.bright;
           agentsys_set.unit      =  t90_set.sys.unit;
           agentsys_set.update    =  t90_set.sys.update;
           agentsys_set.reset     =  t90_set.sys.reset;
                 
           pColors  = &(setDlgColors[t90_set.sys.nightmode]);
		     pColors_win = &(setWinColors[t90_set.sys.nightmode]);     
      
           slideres[0]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
           WM_SetCallback(slideres[0], &sldListener);
           HSD_SLIDER_SetRange(slideres[0], 0, 1); 
           HSD_SLIDER_SetValue(slideres[0], t90_set.sys.workmode);
           
           slideres[1]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
           WM_SetCallback(slideres[1], &sldListener);
           HSD_SLIDER_SetRange(slideres[1], 0, 1);
           HSD_SLIDER_SetValue(slideres[1], t90_set.sys.launch);
           
           slideres[2]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_2);
           WM_SetCallback(slideres[2], &sldListener);
           HSD_SLIDER_SetRange(slideres[2], 0, 1);
           HSD_SLIDER_SetValue(slideres[2], t90_set.sys.nightmode);
           
           slideres[3]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_3);
           WM_SetCallback(slideres[3], &sldListener);
					      HSD_SLIDER_SetNumTicks(slideres[3], 6);
           HSD_SLIDER_SetRange(slideres[3], 1, 6);
           HSD_SLIDER_SetValue(slideres[3], t90_set.sys.volum);
           
           slideres[4]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_4);
           WM_SetCallback(slideres[4], &sldListener);
           HSD_SLIDER_SetNumTicks(slideres[4], 6);
           HSD_SLIDER_SetRange(slideres[4], 1, 6);
           HSD_SLIDER_SetValue(slideres[4], t90_set.sys.bright);
           
           slideres[7]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_7);
           WM_SetCallback(slideres[7], &sldListener);
           HSD_SLIDER_SetRange(slideres[7], 0, 1);
           HSD_SLIDER_SetValue(slideres[7], t90_set.sys.unit);
					 
			  slideres[8]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_8);
           WM_SetCallback(slideres[8], &sldListener);
           HSD_SLIDER_SetRange(slideres[8], 0, 1); 
           HSD_SLIDER_SetValue(slideres[8], t90_set.sys.update);
					 
			  slideres[9]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_9);
           WM_SetCallback(slideres[9], &sldResetCallback);
           HSD_SLIDER_SetRange(slideres[9], 0, 6); 
           HSD_SLIDER_SetValue(slideres[9], t90_set.sys.reset);
           
           slideres[5]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_5);
           WM_SetCallback(slideres[5], &sldListener);
           HSD_SLIDER_SetRange(slideres[5],0,1);
           
           slideres[6]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_6);
           WM_SetCallback(slideres[6], &sldListener);
           HSD_SLIDER_SetRange(slideres[6],0,1);
           
           
           WINDOW_SetBkColor(pMsg->hWin, pColors->bkColor);
           
           for(i = 0; i < 10; i++)
           {	
               HSD_SLIDER_SetBkColor(slideres[i], pColors->bkColor);
               HSD_SLIDER_SetFocusBkColor(slideres[i], pColors->bkColor);
               HSD_SLIDER_SetSlotColor(slideres[i], pColors->slotColor);
               HSD_SLIDER_SetSliderColor(slideres[i],pColors->sliderColor);
               HSD_SLIDER_SetFocusSliderColor(slideres[i], pColors->focusSliderColor);
               HSD_SLIDER_SetFocusSlotColor(slideres[i],pColors->focusSlotColor);
           }
           buttons[0]  = HSD_BUTTON_CreateEx(SYSTEM_SET_ITEM_MARGIN*5+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH, 
                                   SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*5, 
                                   20, 
                                   GUI_GetFontSizeY(), 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
//           WM_SetCallback(buttons[0], &myButtonCallback); 
			  HSD_BUTTON_SetTxFont(buttons[0], &GUI_Font_T90_30);
           HSD_BUTTON_SetBkColor(buttons[0], pColors_win->bkColor);
			  HSD_BUTTON_SetTextColor(buttons[0], pColors_win->textColor);
			  HSD_BUTTON_SetTextFocusColor(buttons[0], pColors_win->focusTextColor);
           
           buttons[1]  = HSD_BUTTON_CreateEx(SYSTEM_SET_ITEM_MARGIN*6+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH, 
                                   SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*5, 
                                   20, 
                                   GUI_GetFontSizeY(), 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);   
//           WM_SetCallback(buttons[0], &myButtonCallback); 
			  HSD_BUTTON_SetTxFont(buttons[1], &GUI_Font_T90_30);
           HSD_BUTTON_SetBkColor(buttons[1], pColors_win->bkColor);
			  HSD_BUTTON_SetTextColor(buttons[1], pColors_win->textColor);
			  HSD_BUTTON_SetTextFocusColor(buttons[0], pColors_win->focusTextColor);
           break;
           
           
       case USER_MSG_REPLY:
            if(pMsg->Data.v == REPLY_OK)
            {
               if(agentsys_set.nightmode != t90_set.sys.nightmode)
               {
                  myMsg.MsgId  = USER_MSG_SKIN;
                  myMsg.Data.v  = agentsys_set.nightmode;
                  WM_BroadcastMessage(&myMsg);
               }
               if(t90_set.sys.unit != agentsys_set.unit)
               {
                  myMsg.MsgId = USER_MSG_UNIT;
                  myMsg.Data.v = agentsys_set.unit;
                  myMsg.hWin = invdAlarmSetWin;
                  WM_SendMessage(invdAlarmSetWin, &myMsg);
               }
               if(t90_set.sys.workmode != agentsys_set.workmode)
               {
                  t90_set.sys.workmode = agentsys_set.workmode;
                  StubRefresh();
               }
               memcpy(&t90_set.sys, &agentsys_set, sizeof(t90_set.sys));
               T90_Store();
            }
            else 
            {
               HSD_SLIDER_SetValue(slideres[0], t90_set.sys.workmode);
               HSD_SLIDER_SetValue(slideres[1], t90_set.sys.launch);
               HSD_SLIDER_SetValue(slideres[2], t90_set.sys.nightmode);
               HSD_SLIDER_SetValue(slideres[3], t90_set.sys.volum);
					HSD_SLIDER_SetValue(slideres[4], t90_set.sys.bright);
               HSD_SLIDER_SetValue(slideres[5], t90_set.sys.SOG.on_off);
					HSD_SLIDER_SetValue(slideres[6], t90_set.sys.COG.on_off);
					HSD_SLIDER_SetValue(slideres[7], t90_set.sys.unit);
					HSD_SLIDER_SetValue(slideres[8], t90_set.sys.update);
            }
            
            WM_SetFocus(slideres[0]);
            WM_SetFocus(mainMenuDlg);

            break;
       
       case WM_NOTIFY_PARENT:       
            switch(pMsg->Data.v)  
            {
               case WM_NOTIFICATION_VALUE_CHANGED:
                    sldId  = WM_GetId(pMsg->hWinSrc) - ID_SLIDER_0;
                    if(sldId >=0  &&  sldId < 9)
                    {                  
                       val  = HSD_SLIDER_GetValue(slideres[sldId]);                     
                       ProcChanging[sldId](pMsg, val);
                    }
                    break;
            }
            break;
          
       case WM_PAINT:
						GUI_SetFont(&GUI_Font_T90_30);
						GUI_SetColor(pColors->textColor);
						GUI_DispStringAt("系统设置：",SYSTEM_SET_ITEM_X,SYSTEM_SET_Y+15);
                  GUI_SetFont(&GUI_Font_T90_24);
						GUI_DispStringAt("作业方式：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y);
			         GUI_DispStringAt("单拖",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y);
						GUI_DispStringAt("双拖",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y);
       
			         GUI_DispStringAt("发射功能：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN+SYSTEM_SET_ITEM_HEIGHT);
						GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN+SYSTEM_SET_ITEM_HEIGHT);
						GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN+SYSTEM_SET_ITEM_HEIGHT);
       
						GUI_DispStringAt("夜间模式：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_ITEM_HEIGHT*2);
						GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_ITEM_HEIGHT*2);
						GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_ITEM_HEIGHT*2);
       
						GUI_DispStringAt("音量设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*3+SYSTEM_SET_ITEM_HEIGHT*3);
						GUI_DispStringAt("减少",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*3+SYSTEM_SET_ITEM_HEIGHT*3);
						GUI_DispStringAt("增加",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*3+SYSTEM_SET_ITEM_HEIGHT*3);
       
						GUI_DispStringAt("亮度设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_ITEM_HEIGHT*4);
						GUI_DispStringAt("减少",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_ITEM_HEIGHT*4);
						GUI_DispStringAt("增加",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_ITEM_HEIGHT*4);
                  
						GUI_DispStringAt("单位设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);
						GUI_DispStringAt("千米",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);
						GUI_DispStringAt("海里",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);
                  
						GUI_DispStringAt("软件更新：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);
						GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);
						GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);
                  
						GUI_DispStringAt("恢复出厂设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*9+SYSTEM_SET_ITEM_HEIGHT*9);
						GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*9+SYSTEM_SET_ITEM_HEIGHT*9);
						GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*9+SYSTEM_SET_ITEM_HEIGHT*9);
                  
						GUI_DispStringAt("软件版本 V1.0.0",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH+80,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);            

						GUI_SetFont(&GUI_Font_T90_24);
                  GUI_DispStringAt("使用",100, 400);
                  GUI_SetColor(pColors->focusSliderColor);
                  GUI_DispString("  卞  ");
                  GUI_SetColor(pColors->textColor);
                  GUI_DispString("选择选项，使用");
                  GUI_SetColor(pColors->focusSliderColor);
                  GUI_DispString(" 咗祐 ");
                  GUI_SetColor(pColors->textColor);
                  GUI_DispString("切换开启关闭或者更改参数。");
                  
                  sprintf(pStrBuf,"%01d",agentsys_set.SOG.averageNum/10);
                  HSD_BUTTON_SetText(buttons[0],pStrBuf);
                  sprintf(pStrBuf, "%01d", agentsys_set.SOG.averageNum%10);
                  HSD_BUTTON_SetText(buttons[1],pStrBuf);
                        
            break;
       default:
           WM_DefaultProc(pMsg);
           break;
   }
}


WM_HWIN DLG_SystemSetCreate()
{
   WM_HWIN hWin;
   hWin  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, SYSTEM_SET_X, SYSTEM_SET_Y);
   return hWin;
}


static void sldListener(WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pInfo;
   WM_MESSAGE myMsg;  
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           pInfo  = (WM_KEY_INFO*)(pMsg->Data.p);
           switch(pInfo->Key)
           {          
              case GUI_KEY_MORIGHT:
                  myMsg.hWin = systemSetDlg;
                  myMsg.hWinSrc = pMsg->hWin;
                  myMsg.MsgId = USER_MSG_MOTHERPOS;
                  myMsg.Data.v = !t90_set.sys.motherpos;
                  WM_SendMessage(myMsg.hWin, &myMsg);   
                  break; 

              case GUI_KEY_PWM_INC:       
                   WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
                   break;
              case GUI_KEY_BACKSPACE:
                   if(Mem_isEqual(&t90_set.sys, &agentsys_set, sizeof(t90_set.sys)) )
                   {
                      WM_SetFocus(slideres[0]);
                      WM_SetFocus(mainMenuDlg);                      
                   }
                   else
                   {
                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
                      myMsg.hWinSrc  = systemSetDlg;
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
           break;
   }
}

static void _OnWorkModeChanged(WM_MESSAGE * pMsg, int val)
{
      agentsys_set.workmode = val;
         
}

static void _OnLaunchChanged(WM_MESSAGE * pMsg, int val)
{
   agentsys_set.launch = val;
}

static void _OnNightModeChanged(WM_MESSAGE * pMsg, int val)
{
	 WM_MESSAGE myMsg;
   if(agentsys_set.nightmode  != val)
   {	 
      agentsys_set.nightmode  = val;
          
	    myMsg.hWin = mainMenuDlg;
      myMsg.Data.v = val;
      myMsg.MsgId = USER_MSG_SKIN;
      WM_SendMessage(myMsg.hWin, &myMsg);
      
      myMsg.hWin  = confirmWin;
      WM_SendMessage(myMsg.hWin, &myMsg);
			
			myMsg.hWin = systemSetDlg;
			WM_SendMessage(myMsg.hWin, &myMsg);
   }
}


static void _OnBrightChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentsys_set.bright != val)
   {
      agentsys_set.bright  = val;      
      PWM_SET(val);
   }
}

static void _OnVolumChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentsys_set.volum != val)
   {
      agentsys_set.volum  = val;

		  ISD_Wait_PWRUp();
      ISD_SetVolumn(val);
      ISD_Play(SND_ID_TEST);
      ISD_PWRDn();
   }
}


static void _OnUnitChanged(WM_MESSAGE * pMsg,int val)
{
   agentsys_set.unit  = val;
}

static void _OnUpdateChanged(WM_MESSAGE * pMsg,int val)
{
   agentsys_set.update  = val;
}


static void sldResetCallback(WM_MESSAGE* pMsg)
{
   const WM_KEY_INFO* pKeyInfo;
   
   WM_MESSAGE  myMsg;
   
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           pKeyInfo  = (WM_KEY_INFO*)pMsg->Data.p;
           switch(pKeyInfo->Key)
           {
              case GUI_KEY_RIGHT:
                   myMsg.hWin      = WM_GetClientWindow(confirmWin);            
                   myMsg.hWinSrc   = pMsg->hWin;
                   myMsg.MsgId     = USER_MSG_CHOOSE;
                   myMsg.Data.v    = SYS_RESET;
                   WM_SendMessage(myMsg.hWin, &myMsg);
 
                   WM_BringToTop(confirmWin);
                   WM_SetFocus(WM_GetDialogItem(confirmWin, GUI_ID_BUTTON0));
                   break;
              
              case GUI_KEY_BACKSPACE:
                   if(Mem_isEqual(&t90_set.sys, &agentsys_set, sizeof(t90_set.sys)) )
                   {
                      WM_SetFocus(slideres[0]);
                      WM_SetFocus(mainMenuDlg);                      
                   }
                   else
                   {
                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
                      myMsg.hWinSrc  = systemSetDlg;
                      myMsg.MsgId  = USER_MSG_CHOOSE;
                      myMsg.Data.v  = SYS_SETTING;
                      WM_SendMessage(myMsg.hWin, &myMsg);
                      
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0));                    
                   }
                   
                   break;
                   
             default:
                   HSD_SLIDER_Callback(pMsg);
                   break;
           }
           break;
           
      case USER_MSG_REPLY:
           WM_SetFocus(slideres[7]);
           WM_BringToTop(systemSetDlg);
           if(pMsg->Data.v == REPLY_OK )
           {
              if(t90_set.sys.nightmode != NIGHT) 
              {
                 myMsg.MsgId  = USER_MSG_SKIN;
                 myMsg.Data.v  = NIGHT;
                 WM_BroadcastMessage(&myMsg);
                 agentsys_set.nightmode  = t90_set.sys.nightmode;
              }
              
              T90_Reset();
              
				  HSD_SLIDER_SetValue(slideres[0], t90_set.sys.workmode);
              HSD_SLIDER_SetValue(slideres[7], 0);
				  GUI_Delay(1000);
							
				  HSD_SLIDER_SetValue(slideres[1], t90_set.sys.launch);
              HSD_SLIDER_SetValue(slideres[7], 1);
				  GUI_Delay(1000);
							
              HSD_SLIDER_SetValue(slideres[2], t90_set.sys.nightmode);
              HSD_SLIDER_SetValue(slideres[7], 2);
              GUI_Delay(1000);
							
              HSD_SLIDER_SetValue(slideres[3], t90_set.sys.volum);
              HSD_SLIDER_SetValue(slideres[7], 3);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(slideres[4], t90_set.sys.bright);
              HSD_SLIDER_SetValue(slideres[7], 4);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(slideres[5], t90_set.sys.unit);
              HSD_SLIDER_SetValue(slideres[7], 5);
              GUI_Delay(1000);
							
				  HSD_SLIDER_SetValue(slideres[6], t90_set.sys.update);
              HSD_SLIDER_SetValue(slideres[7], 6);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(slideres[7], 0);
              
              NVIC_SystemReset();
              
              
           } 

           break;
           
    default:
           HSD_SLIDER_Callback(pMsg);
           break;
   }
}

static void _OnSOGAverageChanged(WM_MESSAGE * pMsg,int val)
{
   agentsys_set.SOG.on_off = val;
}
static void _OnCOGAverageChanged(WM_MESSAGE * pMsg,int val)
{
   agentsys_set.COG.on_off = val;
}



