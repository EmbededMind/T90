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
//#include "HSD_BUTTON.h"
#include "HSD_Toast.h"
#include "layout_system_set.h"
#include "xt_isd.h"

#define SLD_NUM  6

WM_HWIN systemSetDlg;

extern char comflg;
char comflg = 0;
static void myButtonCallback(WM_MESSAGE * pMsg);
static void sldListener(WM_MESSAGE * pMsg);
static void sldResetCallback(WM_MESSAGE* pMsg);


static const SetDlgColor *pColors = setDlgColors;
static const SetWinColor *pColors_win =  setWinColors;

static void _OnWorkModeChanged(WM_MESSAGE * pMsg,int val);
//static void _OnLaunchChanged(WM_MESSAGE * pMsg,int val);
static void _OnNightModeChanged(WM_MESSAGE * pMsg,int val);
static void _OnVolumChanged(WM_MESSAGE * pMsg,int val);
static void _OnBrightChanged(WM_MESSAGE * pMsg,int val);
static void _OnUnitChanged(WM_MESSAGE * pMsg,int val);
//static void _OnUpdateChanged(WM_MESSAGE * pMsg,int val);
static void _OnSOGAverageChanged(WM_MESSAGE * pMsg,int val);
static void _OnCOGAverageChanged(WM_MESSAGE * pMsg,int val);

static System_Set agentsys_set;


static  void (* const ProcChanging[SLD_NUM-1])(WM_MESSAGE *, int)  = {
   _OnWorkModeChanged,
//   _OnLaunchChanged,
   _OnNightModeChanged,
   _OnVolumChanged,
   _OnBrightChanged,
//   _OnSOGAverageChanged,
//   _OnCOGAverageChanged,
   _OnUnitChanged,
//   _OnUpdateChanged
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = 
{ 
   { WINDOW_CreateIndirect,  "Window",  ID_WINDOW_0, 0,                 0,                                                                   SYSTEM_SET_WIDTH,    SYSTEM_SET_HEIGHT,        0, 0, 0},

   { HSD_SLIDER_CreateIndirect, "sld0", ID_SLIDER_0, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y,                                                   SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect, "sld1", ID_SLIDER_1, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+ SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN,    SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect, "sld2", ID_SLIDER_2, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*2, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect, "sld3", ID_SLIDER_3, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*3, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect, "sld6", ID_SLIDER_4, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*6, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
//   { HSD_SLIDER_CreateIndirect, "sld7", ID_SLIDER_5, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*7, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect, "sld7", ID_SLIDER_5, SYSTEM_SET_ITEM_X, SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*8, SYSTEM_SET_ITEM_WIDTH, SYSTEM_SET_ITEM_HEIGHT, 0, 0, 0},
};


static WM_HWIN slideres[SLD_NUM];
static WM_HWIN buttons[2];
//static WM_HWIN text[2];
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
           if(t90_set.sys.workmode == DOUBLE_MODE)
           {
              if(t90_set.sys.motherpos == DEFAULT_LEFT)
              {
                 for(i = 0; i < 3; i++)
                 {
                     if(stubs[i+1].isValid)
                     {
                        comflg = 1;
                        Comm_addFrame(i+1,stubs[i+1].basePoint.x*MILLINM_TO_M,abs(stubs[i+1].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                     }
                 }
              }
              else
              {                            
                 for(i = 0; i < 3; i++)
                 {
                     if(stubs[i+1].isValid)
                     {
                        comflg = 1;
// 											 Comm_addFrame(i+1,stubs[i+1].basePoint.x*MILLINM_TO_M,abs(stubs[i+1].basePoint.y)*MILLINM_TO_M, t90_set.sys.SOG, t90_set.sys.COG);
                        Comm_addFrame(i+1,(stubs[i+1].basePoint.x - stubs[4].basePoint.x)*MILLINM_TO_M, abs(stubs[i+1].basePoint.y*MILLINM_TO_M),t90_set.sys.SOG, t90_set.sys.COG);
                     }
                 }                         
              }
           }
           break;
      
      case USER_MSG_DIM:   
           HSD_SLIDER_Loop(slideres[3]);
           t90_set.sys.bright = HSD_SLIDER_GetValue(slideres[3]);
           T90_Store();
           memcpy(&agentsys_set, &t90_set.sys, sizeof(t90_set.sys));
           StubRefresh();
           break;
      
      case USER_MSG_SKIN:    
           pColors  = &(setDlgColors[pMsg->Data.v]);
           pColors_win = &(setWinColors[pMsg->Data.v]); 
      
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
           for(i = 0; i < 2; i++)
           {
              BUTTON_SetFocusColor(buttons[i],  pColors_win->focusBkColor);
              BUTTON_SetBkColor(buttons[i],  BUTTON_CI_UNPRESSED, pColors_win->bkColor);
              BUTTON_SetTextColor(buttons[i],  BUTTON_CI_UNPRESSED, pColors_win->textColor);
           }
           break;
           
      case WM_INIT_DIALOG:  
           agentsys_set.workmode  =  t90_set.sys.workmode;
           agentsys_set.nightmode =  t90_set.sys.nightmode;
           agentsys_set.volum     =  t90_set.sys.volum;
           agentsys_set.bright    =  t90_set.sys.bright;
           agentsys_set.unit      =  t90_set.sys.unit;
           agentsys_set.update    =  t90_set.sys.update;
           agentsys_set.reset     =  t90_set.sys.reset;

           agentsys_set.COG = t90_set.sys.COG;    
           agentsys_set.SOG = t90_set.sys.SOG;
   
           pColors  = &(setDlgColors[t90_set.sys.nightmode]);
		         pColors_win = &(setWinColors[t90_set.sys.nightmode]);     
      
           slideres[0]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
           WM_SetCallback(slideres[0], &sldListener);
           HSD_SLIDER_SetRange(slideres[0], 0, 1); 
           HSD_SLIDER_SetValue(slideres[0], t90_set.sys.workmode);
           
           
           slideres[1]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
           WM_SetCallback(slideres[1], &sldListener);
           HSD_SLIDER_SetRange(slideres[1], 0, 1);
           HSD_SLIDER_SetValue(slideres[1], t90_set.sys.nightmode);
           
           slideres[2]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_2);
           WM_SetCallback(slideres[2], &sldListener);
			        HSD_SLIDER_SetNumTicks(slideres[2], 6);
           HSD_SLIDER_SetRange(slideres[2], 1, 6);
           HSD_SLIDER_SetValue(slideres[2], t90_set.sys.volum);
           
           slideres[3]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_3);
           WM_SetCallback(slideres[3], &sldListener);
           HSD_SLIDER_SetNumTicks(slideres[3], 6);
           HSD_SLIDER_SetRange(slideres[3], 1, 6);
           HSD_SLIDER_SetValue(slideres[3], t90_set.sys.bright);
           
           slideres[4]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_4);
           WM_SetCallback(slideres[4], &sldListener);
           HSD_SLIDER_SetRange(slideres[4], 0, 1);
           HSD_SLIDER_SetValue(slideres[4], t90_set.sys.unit);
					 
					 
			        slideres[5]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_5);
           WM_SetCallback(slideres[5], &sldResetCallback);
           HSD_SLIDER_SetRange(slideres[5], 0, 8); 
           HSD_SLIDER_SetValue(slideres[5], t90_set.sys.reset);
           
           
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
           GUI_SetFont(&GUI_Font_T90_24);

           buttons[0] = BUTTON_CreateEx(SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_RIGHT_CHOICE/2+SYSTEM_SET_ITEM_WIDTH-1, 
                                   SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*4 - 6, 
                                   27, 
                                   GUI_GetFontSizeY()+5, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON0);           
           WM_SetCallback(buttons[0], &myButtonCallback); 
			        BUTTON_SetFont(buttons[0], &GUI_Font_T90_24);
           BUTTON_SetBkColor(buttons[0], BUTTON_CI_UNPRESSED, pColors_win->bkColor);
			        BUTTON_SetTextColor(buttons[0], BUTTON_CI_UNPRESSED, pColors_win->textColor);
           BUTTON_SetFocusColor(buttons[0],  pColors_win->focusBkColor);


           
           buttons[1]  = BUTTON_CreateEx(SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_RIGHT_CHOICE/2+SYSTEM_SET_ITEM_WIDTH-1, 
                                   SYSTEM_SET_ITEM_Y+(SYSTEM_SET_ITEM_HEIGHT+SYSTEM_SET_ITEM_MARGIN)*5 - 6, 
                                   27, 
                                   GUI_GetFontSizeY()+5, 
                                   pMsg->hWin, WM_CF_SHOW,  0,  GUI_ID_BUTTON1);
                      
           WM_SetCallback(buttons[1], &myButtonCallback); 
			        BUTTON_SetFont(buttons[1], &GUI_Font_T90_24);
           BUTTON_SetBkColor(buttons[1], BUTTON_CI_UNPRESSED, pColors_win->bkColor);
			        BUTTON_SetTextColor(buttons[1], BUTTON_CI_UNPRESSED, pColors_win->textColor);
           BUTTON_SetFocusColor(buttons[1],  pColors_win->focusBkColor);


                                
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
                  if(t90_set.sys.workmode == SINGLE_MODE || (t90_set.sys.workmode == DOUBLE_MODE && t90_set.sys.motherpos == DEFAULT_LEFT))
                  {
                     for(i = 1; i < 4; i++)
                     {
                        if(stubs[i].isValid)
                        {
                           Comm_addFrame(i, stubs[i].basePoint.x, abs(stubs[i].basePoint.y), t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                  }
                  else if(t90_set.sys.workmode == DOUBLE_MODE)
                  {
                     
                     for(i = 1; i < 4; i++)
                     {
                        if(stubs[i].isValid)
                        {
                           Comm_addFrame(i, stubs[i].basePoint.x - stubs[4].basePoint.x, abs(stubs[i].basePoint.y), t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }                    
                  }
                  if(t90_set.sys.workmode == DOUBLE_MODE && t90_set.as_MMSI.port)
                  {
                    int flg = 0;
                    for(i = 0; i < 5; i++)
                      { 
                         
                         if(!flg && t90_set.shipout.MMSI[i] == t90_set.as_MMSI.MMSI)
                         {
                            flg = 1;
                         }
                         if(flg)
                         {
                            if(i < 4)
                               t90_set.shipout.MMSI[i] = t90_set.shipout.MMSI[i+1];
                            else
                               t90_set.shipout.MMSI[i] = 0;
                         }
                      }
                      if(flg)
                         t90_set.shipout.numShip--;
                  }
               }
               if(t90_set.sys.SOG != agentsys_set.SOG)
               {
                  t90_set.sys.SOG = agentsys_set.SOG;
                  StubRefresh();
                  
                  if(t90_set.sys.workmode == SINGLE_MODE || (t90_set.sys.workmode == DOUBLE_MODE && t90_set.sys.motherpos == DEFAULT_LEFT))
                  {
                     for(i = 1; i < 4; i++)
                     {
                        if(stubs[i].isValid)
                        {
                           Comm_addFrame(i, stubs[i].basePoint.x, abs(stubs[i].basePoint.y), t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                  }
                  else if(t90_set.sys.workmode == DOUBLE_MODE)
                  {
                     for(i = 1; i < 4; i++)
                     {
                        if(stubs[i].isValid)
                        {
                           Comm_addFrame(i, stubs[i].basePoint.x - stubs[4].basePoint.x, abs(stubs[i].basePoint.y), t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                  }                  
               }
               if(t90_set.sys.COG != agentsys_set.COG)
               {
                  t90_set.sys.COG = agentsys_set.COG;
                  
                  StubRefresh();
                  if(t90_set.sys.workmode == SINGLE_MODE || (t90_set.sys.workmode == DOUBLE_MODE && t90_set.sys.motherpos == DEFAULT_LEFT))
                  {
                     for(i = 1; i < 4; i++)
                     {
                        if(stubs[i].isValid)
                        {
                           Comm_addFrame(i, stubs[i].basePoint.x, abs(stubs[i].basePoint.y), t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                  }
                  else if(t90_set.sys.workmode == DOUBLE_MODE)
                  {
                     for(i = 1; i < 4; i++)
                     {
                        if(stubs[i].isValid)
                        {
                           Comm_addFrame(i, stubs[i].basePoint.x - stubs[4].basePoint.x, abs(stubs[i].basePoint.y), t90_set.sys.SOG, t90_set.sys.COG);
                        }
                     }
                  }                  
               }
               memcpy(&t90_set.sys, &agentsys_set, sizeof(t90_set.sys));
               T90_Store();
            }
            else 
            {
               HSD_SLIDER_SetValue(slideres[0], t90_set.sys.workmode);
               HSD_SLIDER_SetValue(slideres[1], t90_set.sys.nightmode);
               HSD_SLIDER_SetValue(slideres[2], t90_set.sys.volum);
					          HSD_SLIDER_SetValue(slideres[3], t90_set.sys.bright);
               HSD_SLIDER_SetValue(slideres[4], t90_set.sys.unit);
               HSD_SLIDER_SetValue(slideres[5], t90_set.sys.update);
               agentsys_set.SOG = t90_set.sys.SOG;
               agentsys_set.COG = t90_set.sys.COG;
            }
            
            WM_SetFocus(slideres[0]);
            WM_SetFocus(mainMenuDlg);

            break;
       
       case WM_NOTIFY_PARENT:       
            switch(pMsg->Data.v)  
            {
               case WM_NOTIFICATION_VALUE_CHANGED:
                    sldId  = WM_GetId(pMsg->hWinSrc) - ID_SLIDER_0;
                    if(sldId >=0  &&  sldId < 8)
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
            GUI_DispStringAt("作业方式：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5);
            GUI_DispStringAt("单拖",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5);
            GUI_DispStringAt("双拖",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5);
             
             
            GUI_DispStringAt("夜间模式：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN+SYSTEM_SET_ITEM_HEIGHT);
            GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN+SYSTEM_SET_ITEM_HEIGHT);
            GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN+SYSTEM_SET_ITEM_HEIGHT);
             
            GUI_DispStringAt("音量设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_ITEM_HEIGHT*2);
            GUI_DispStringAt("减少",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_ITEM_HEIGHT*2);
            GUI_DispStringAt("增加",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_ITEM_HEIGHT*2);
             
            GUI_DispStringAt("亮度设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*3+SYSTEM_SET_ITEM_HEIGHT*3);
            GUI_DispStringAt("减少",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*3+SYSTEM_SET_ITEM_HEIGHT*3);
            GUI_DispStringAt("增加",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*3+SYSTEM_SET_ITEM_HEIGHT*3);
                        
            GUI_DispStringAt("航速平均化：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_ITEM_HEIGHT*4);
//            GUI_DispStringAt("减少",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_ITEM_HEIGHT*4);
//            GUI_DispStringAt("增加",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*4+SYSTEM_SET_ITEM_HEIGHT*4);
                        
                        
            GUI_DispStringAt("航向平均化：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*5+SYSTEM_SET_ITEM_HEIGHT*5);
//            GUI_DispStringAt("减少",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*5+SYSTEM_SET_ITEM_HEIGHT*5);
//            GUI_DispStringAt("增加",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*5+SYSTEM_SET_ITEM_HEIGHT*5);
                        
                        
            GUI_DispStringAt("单位设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*6+SYSTEM_SET_ITEM_HEIGHT*6);
            GUI_DispStringAt("千米",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*6+SYSTEM_SET_ITEM_HEIGHT*6);
            GUI_DispStringAt("海里",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*6+SYSTEM_SET_ITEM_HEIGHT*6);
                        
            GUI_DispStringAt("软件更新：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);
//            GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);
//            GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);
            GUI_DispStringAt("软件版本 V1.0.6",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*7+SYSTEM_SET_ITEM_HEIGHT*7);            
            
            GUI_DispStringAt("恢复出厂设置：",SYSTEM_SET_ITEM_MARGIN*2,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);
            GUI_DispStringAt("关闭",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_LEFT_CHOICE,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);
            GUI_DispStringAt("开启",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH,SYSTEM_SET_ITEM_Y - 5+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);
                        
//            GUI_DispStringAt("软件版本 V1.0.0",SYSTEM_SET_ITEM_MARGIN*2+SYSTEM_SET_RIGHT_CHOICE+SYSTEM_SET_ITEM_WIDTH+80,SYSTEM_SET_ITEM_Y+SYSTEM_SET_ITEM_MARGIN*8+SYSTEM_SET_ITEM_HEIGHT*8);            

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
            

            BUTTON_SetTextColor(buttons[0], BUTTON_CI_UNPRESSED, pColors_win->textColor);
            BUTTON_SetTextColor(buttons[1], BUTTON_CI_UNPRESSED, pColors_win->textColor);

            

            sprintf(pStrBuf,"%d",agentsys_set.SOG);
            BUTTON_SetText(buttons[0],pStrBuf);



            sprintf(pStrBuf,"%d",agentsys_set.COG);
            BUTTON_SetText(buttons[1],pStrBuf);

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

static void myButtonCallback(WM_MESSAGE * pMsg)
{
   int id = 0;
   WM_MESSAGE myMsg; 
   switch(pMsg->MsgId)
   {
      case WM_SET_FOCUS:
         if(pMsg->Data.v){
            BUTTON_SetBkColor(pMsg->hWin,  BUTTON_CI_UNPRESSED, pColors_win->focusBkColor);
            BUTTON_SetTextColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors_win->focusTextColor);
         }
         else{
            BUTTON_SetBkColor(pMsg->hWin,  BUTTON_CI_UNPRESSED, pColors_win->bkColor);
            BUTTON_SetTextColor(pMsg->hWin, BUTTON_CI_UNPRESSED, pColors_win->textColor);
         }
            BUTTON_Callback(pMsg);
         break;
         
      case WM_KEY:
         id = WM_GetId(pMsg->hWin) - GUI_ID_BUTTON0;
         switch(((WM_KEY_INFO*)pMsg->Data.p)->Key)
         {
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
            case GUI_KEY_LEFT:
               if(id == 0)
               {
                  agentsys_set.SOG -= 1;
                  if(agentsys_set.SOG < 1)
                     agentsys_set.SOG = 1;
                  sprintf(pStrBuf,"%d",agentsys_set.SOG);
                  BUTTON_SetText(buttons[0],pStrBuf);
               }
               
               if(id == 1)
               {
                  agentsys_set.COG -= 1;
                  if(agentsys_set.COG < 1)
                     agentsys_set.COG = 1;
                  sprintf(pStrBuf, "%d", agentsys_set.COG);
                  BUTTON_SetText(buttons[1], pStrBuf);
               }
               break;
            case GUI_KEY_RIGHT:
               if(id == 0)
               {
                  agentsys_set.SOG += 1;
                  if(agentsys_set.SOG > 60)
                     agentsys_set.SOG = 60;
                  sprintf(pStrBuf,"%d",agentsys_set.SOG);
                  BUTTON_SetText(buttons[0],pStrBuf);
               }
               
               if(id == 1)
               {
                  agentsys_set.COG += 1;
                  if(agentsys_set.COG > 60)
                     agentsys_set.COG = 60;
                  sprintf(pStrBuf, "%d", agentsys_set.COG);
                  BUTTON_SetText(buttons[1], pStrBuf);
               }
               break;
            case GUI_KEY_UP:
               if(id == 0)
               {
                  WM_SetFocus(slideres[3]);
               }
               
               if(id == 1)
               {
                  WM_SetFocus(buttons[0]);
               }
               break;
               
            case GUI_KEY_DOWN:
               if(id == 0)
               {
                  WM_SetFocus(buttons[1]);
               }
               
               if(id == 1)
               {
                  WM_SetFocus(slideres[4]);
               }
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
               break;
        }
        break;
     default:
        BUTTON_Callback(pMsg);
   }
   
      
}

static void sldListener(WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pInfo;
   WM_MESSAGE myMsg;  
   int id = 0;
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           id = WM_GetId(pMsg->hWin) - ID_SLIDER_0;
           pInfo  = (WM_KEY_INFO*)(pMsg->Data.p);
           switch(pInfo->Key)
           {  
                             
              case GUI_KEY_UP:
                 if(id == 4)
                 {
                    WM_SetFocus(buttons[1]);
                    break;
                 }
                 if(id == 0)
                 {
                    WM_SetFocus(slideres[5]);
                    break;                  
                 }
                 HSD_SLIDER_Callback(pMsg);
                 break;
              case GUI_KEY_DOWN:
                 if(id == 3)
                 {
                     WM_SetFocus(buttons[0]);
                     break;
                 }
                 HSD_SLIDER_Callback(pMsg);
                 break;
               
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
      if(sound)
      {
         ISD_Wait_PWRUp();
         ISD_SetVolumn(val);
         ISD_Play(SND_ID_TEST);
         ISD_PWRDn();
      }
   }
}


static void _OnUnitChanged(WM_MESSAGE * pMsg,int val)
{
   agentsys_set.unit  = val;
}


static void sldResetCallback(WM_MESSAGE* pMsg)
{
   const WM_KEY_INFO* pKeyInfo;
   
   WM_MESSAGE  myMsg;
   int id = 0;
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           pKeyInfo  = (WM_KEY_INFO*)pMsg->Data.p;
           id = WM_GetId(pMsg->hWin) - ID_SLIDER_0;
           switch(pKeyInfo->Key)
           {
              case GUI_KEY_DOWN:
                   WM_SetFocus(slideres[0]);
                   break;
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
           WM_BringToTop(systemSetDlg);
           if(pMsg->Data.v == REPLY_OK )
           {
//              if(t90_set.sys.nightmode != NIGHT) 
//              {
//                 myMsg.MsgId  = USER_MSG_SKIN;
//                 myMsg.Data.v  = NIGHT;
//                 WM_BroadcastMessage(&myMsg);
//                 agentsys_set.nightmode  = t90_set.sys.nightmode;
//              }
              
              T90_Reset();
              
              NVIC_SystemReset();
              
              
           }
           else
           {
              WM_SetFocus(mainMenuDlg); 
           }               

           break;
           
    default:
           HSD_SLIDER_Callback(pMsg);
           break;
   }
}






