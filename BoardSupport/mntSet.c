/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.24                          *
*        Compiled Jan 27 2014, 11:28:24                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "MainTask.h"
#include "Config.h"
#include "Setting.h"
#include "sysConf.h"
#include "GUI.h"
#include "skinColor.h"
#include "str.h"
#include <string.h>
#include "dlg.h"
#include "28.h"
#include "drawThings.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)

#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_2 (GUI_ID_USER + 0x03)
#define ID_TEXT_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_4 (GUI_ID_USER + 0x05)
#define ID_TEXT_5 (GUI_ID_USER + 0x06)
#define ID_TEXT_6 (GUI_ID_USER + 0x07)
#define ID_TEXT_7 (GUI_ID_USER + 0x08)
#define ID_TEXT_8 (GUI_ID_USER + 0x09)
#define ID_TEXT_9 (GUI_ID_USER + 0x0a)

#define ID_BUTTON_0 (GUI_ID_USER + 0x10)
#define ID_BUTTON_1 (GUI_ID_USER + 0x11)
#define ID_BUTTON_2 (GUI_ID_USER + 0x12)
#define ID_BUTTON_3 (GUI_ID_USER + 0x13)
#define ID_BUTTON_4 (GUI_ID_USER + 0x14)
#define ID_BUTTON_5 (GUI_ID_USER + 0x15)
#define ID_BUTTON_6 (GUI_ID_USER + 0x16)

/*--------------------- Global variables -------------------------*/
WM_HWIN mntSettingWin ;



/*------------------- local    variables -------------------------*/

static WM_HWIN hBts[7];
static MNT_BERTH* pCurMntBerth  = NULL;

static MNT_SETTING mntSetting;

static void myButtonListener(WM_MESSAGE* pMsg);
static void btReset(void);


extern GUI_CONST_STORAGE GUI_BITMAP bmyu;
extern GUI_CONST_STORAGE GUI_BITMAP bmmao;



// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*   
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
 { WINDOW_CreateIndirect, "ETWin", ID_WINDOW_0, ETWin_X, SubWin_Y+40, ETWin_WIDHT, ETWin_HEIGHT-81, 0, 0x0, 0 },

 { BUTTON_CreateIndirect, "bt_0", ID_BUTTON_0, 115, 10,  70, 30, 0, 0xa,  0 },
 { BUTTON_CreateIndirect, "bt_1", ID_BUTTON_1, 150, 45,  70, 30, 0, 0x64, 0 },
 { BUTTON_CreateIndirect, "bt_2", ID_BUTTON_2, 130, 80,  50, 30, 0, 0x64, 0 },
 { BUTTON_CreateIndirect, "bt_3", ID_BUTTON_3, 130, 160, 70, 30, 0, 0x64, 0 },
 { BUTTON_CreateIndirect, "bt_4", ID_BUTTON_4, 150, 190, 70, 30, 0, 0x64, 0 },
 { BUTTON_CreateIndirect, "bt_5", ID_BUTTON_5, 130, 225, 50, 30, 0, 0x64, 0},
 { BUTTON_CreateIndirect, "bt_6", ID_BUTTON_6, 130, 305, 70, 30, 0, 0x64, 0}
	
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static const MntSetWin_COLOR * pSkin  = &MntSetWinSkins[0];
// USER START (Optionally insert additional static code)
// USER END




/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	
  WM_HWIN  hItem  = 0;
  WM_MESSAGE myMsg;
  
  int     i  = 0;
  
  switch (pMsg->MsgId) {
		
		case WM_PAINT:
       GUI_SetFont(&GUI_Font30);
       GUI_SetColor(pSkin->txColor);
       GUI_SetTextMode(GUI_TM_TRANS);
       GUI_DispStringAt("消失报警:",10,10);
       GUI_DispStringAt("走锚报警功能:",10,45);
       GUI_DispStringAt("距离:",80,80);
       GUI_DispStringAt("声音:",80,160);
       GUI_DispStringAt("防盗报警功能:",10,190);
       GUI_DispStringAt("距离:",80,225);
       GUI_DispStringAt("声音:",80,305);
       
       if(SysConf.Unit == UNIT_nm)
       {
          GUI_DispStringAt("nm", 180, 82);
          GUI_DispStringAt("nm", 180, 227);
       }
       else
       {
          GUI_DispStringAt("km", 180, 82);
          GUI_DispStringAt("km", 180, 227);       
       }
       
       GUI_SetFont(&GUI_Font24);
       GUI_DispStringAt("使用",40,350);
//       GUI_SetFont(&GUI_Font24);
       GUI_SetColor(pSkin->bt_txColor);
       GUI_DispStringAt("左右",80,350);
       GUI_SetColor(GUI_WHITE);
       GUI_DispStringAt("切换设置",120, 350);
//       GUI_DrawPolygon(Points_fish, 11,43, 130);
       GUI_DrawPolygon(Points_fish, 11, 43, 280);
//       GUI_DrawBitmap(&bmmao,23,260);
       GUI_DrawBitmap(&bmmao, 23, 110);
       
       //GUI_SetPenSize(3);
       GUI_SetColor(DRG_BOAT_COLOR);
       GUI_DrawCircle(43,130,40);
       GUI_SetColor(BGL_BOAT_COLOR);
       GUI_DrawCircle(43,280,40);
       
       GUI_SetColor(GUI_BROWN);
       GUI_DrawHLine(130, 43, 83);
       GUI_DrawLine(70, 130, 83, 110);
       GUI_DrawHLine(110, 83, 140);
       
       GUI_DrawHLine(276, 43, 83);
       GUI_DrawLine(70, 276, 83, 256);
       GUI_DrawHLine(256, 83, 140);
       break;
       
  case USER_MSG_REPLY:
       switch(pMsg->Data.v)
       { 
          case REPLY_OK:         
               WM_SetFocus(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));
               i  = MNT_makeSettingUp(&mntSetting);  
 //               MNT_initSetting();
               btReset();  
               myMsg.hWin  = WM_GetClientWindow(menuWin);               
               myMsg.MsgId  = USER_MSG_DFULT_CNT;
               myMsg.Data.v  = i;
               WM_SendMessage(myMsg.hWin, &myMsg);
               WM_SetFocus(menuWin);
               break;
          case REPLY_CANCEL:   
               WM_SetFocus(subWins[1]);
               break;
               
           default:
               INFO("Something err!");           
               break;
       }
       break;       
       
  case USER_MSG_FOCUS:
       WM_SetFocus(hBts[0]);
       
       if(pMsg->Data.p)
       {
          pCurMntBerth  = (MNT_BERTH*)(pMsg->Data.p);
          memcpy((void*)&mntSetting, (void*)(&pCurMntBerth->mntBoat.mntSetting), sizeof(MNT_SETTING));           
       }
       break;
       
  case USER_MSG_LV_MOVE:
       if(pMsg->Data.p) 
       {
          MNT_SETTING *  pMntSetting  = NULL;
          
          pMntSetting    = (MNT_SETTING*)&(  ( (MNT_BERTH*)pMsg->Data.p)->mntBoat.mntSetting);
   
          if(pMntSetting)  
          {          
             BUTTON_SetText(hBts[0], pMntSetting->DSP_Setting.isEnable?"开启":"关闭");
             
             BUTTON_SetText(hBts[1], pMntSetting->DRG_Setting.isEnable?"开启":"关闭");

             BUTTON_SetText(hBts[3], pMntSetting->DRG_Setting.isSndEnable?"开启":"关闭");
             
             BUTTON_SetText(hBts[4], pMntSetting->BGL_Setting.isEnable?"开启":"关闭");

             BUTTON_SetText(hBts[6], pMntSetting->BGL_Setting.isSndEnable?"开启":"关闭");          
             
             if(SysConf.Unit == UNIT_nm)
             {
                sprintf(pStrBuf, "%d.%02d", pMntSetting->DRG_Setting.Dist/1000,pMntSetting->DRG_Setting.Dist%1000/10);
                BUTTON_SetText(hBts[2], pStrBuf);
                sprintf(pStrBuf, "%d.%02d", pMntSetting->BGL_Setting.Dist/1000, pMntSetting->BGL_Setting.Dist%1000/10);
                BUTTON_SetText(hBts[5], pStrBuf);             
             }
             else 
             {
                int tmpDist  = 0;
                tmpDist  = pMntSetting->DRG_Setting.Dist *100 /54;
                
                sprintf(pStrBuf, "%d.%02d", tmpDist/1000, (tmpDist%1000)/10);
                BUTTON_SetText(hBts[2], pStrBuf);
                
                tmpDist  = pMntSetting->BGL_Setting.Dist *100 /54;
                sprintf(pStrBuf, "%d.%02d", tmpDist/1000, (tmpDist%1000)/10);
                BUTTON_SetText(hBts[5], pStrBuf);
             }
          }
       }
       else
       {
          btReset();
       }
       break;
  
  case USER_MSG_SKIN: 
       pSkin  = &(MntSetWinSkins[pMsg->Data.v]);
       
       WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);


       for(i=0;i<7;i++)
       {
          BUTTON_SetBkColor(hBts[i], BUTTON_CI_UNPRESSED, pSkin->bt_bkColor);
          BUTTON_SetTextColor(hBts[i], BUTTON_CI_UNPRESSED,pSkin->bt_txColor);
       } 
       break;    
       
  case WM_INIT_DIALOG:
       pSkin  = &(MntSetWinSkins[SysConf.Skin]);
	
       WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
     
       //
       // Initialization of 'text'
       //    
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
       TEXT_SetFont(hItem,&GUI_Font24);
       TEXT_SetTextColor(hItem, pSkin->bt_bkColor);
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
       TEXT_SetTextColor(hItem, pSkin->txColor); 
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
       TEXT_SetTextColor(hItem, pSkin->txColor); 
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
       TEXT_SetTextColor(hItem, pSkin->txColor); 
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
       TEXT_SetTextColor(hItem, pSkin->txColor); 
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
       TEXT_SetTextColor(hItem, pSkin->txColor); 
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
       TEXT_SetTextColor(hItem, pSkin->txColor); 
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
       TEXT_SetTextColor(hItem, pSkin->txColor);    
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
       TEXT_SetFont(hItem, GUI_FONT_24_1);
       TEXT_SetTextColor(hItem, pSkin->txColor);
//       TEXT_SetText(hItem, SysConf.Unit==UNIT_km?"km":"nm");
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
       TEXT_SetFont(hItem, GUI_FONT_24_1);
       TEXT_SetTextColor(hItem, pSkin->txColor);   
//       TEXT_SetText(hItem, SysConf.Unit==UNIT_km?"km":"nm");    
     
       //
       // Initialization of 'et_0'
       //
       
       hBts[0] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
//       BUTTON_SetText(hBts[0], mntSetting.DSP_Setting.isEnable>DISABLE?"开启":"关闭");   
       WM_SetCallback(hBts[0],&myButtonListener);
       
       //
       // Initialization of 'et_1'
       //
       hBts[1] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//       BUTTON_SetText(hBts[1], mntSetting.DRG_Setting.isEnable>DISABLE?"开启":"关闭"); 
       WM_SetCallback(hBts[1],&myButtonListener);  
       //
       // Initialization of 'et_2'
       //
       hBts[2] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
//       if(SysConf.Unit == UNIT_nm)
//       {
//          sprintf(pStrBuf, "%d.%02d", mntSetting.DRG_Setting.Dist/1000, (mntSetting.DRG_Setting.Dist%1000)/10);
//          BUTTON_SetText(hBts[2], pStrBuf);  
//       }
//       else
//       {
//          int tmpDist  = mntSetting.DRG_Setting.Dist *100 /54;
//          sprintf(pStrBuf, "%d.%02d", tmpDist /1000,tmpDist %1000 /10);
//          BUTTON_SetText(hBts[2],pStrBuf);
//       }
       WM_SetCallback(hBts[2],&myButtonListener);	
       //
       // Initialization of 'et_3'
       //
       hBts[3] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
//       BUTTON_SetText(hBts[3], mntSetting.DRG_Setting.isSndEnable>DISABLE?"开启":"关闭");
       WM_SetCallback(hBts[3] ,&myButtonListener);	  
       //
       // Initialization of 'et_4'
       //
       hBts[4]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);

//       BUTTON_SetText(hBts[4], mntSetting.BGL_Setting.isEnable>DISABLE?"开启":"关闭");
       WM_SetCallback(hBts[4],&myButtonListener);    
       //
       // Initialization of 'et_5'
       //
       hBts[5]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);   
//       if(SysConf.Unit == UNIT_nm)
//       {
//          sprintf(pStrBuf, "%d.%02d", mntSetting.BGL_Setting.Dist/1000, mntSetting.BGL_Setting.Dist%10);
//          BUTTON_SetText(hBts[5], pStrBuf);     
//       }
//       else
//       {
//          int tmpDist  = mntSetting.BGL_Setting.Dist *100 / 54;
//          sprintf(pStrBuf, "%d.%02d", tmpDist /1000,tmpDist %1000 /10);
//          BUTTON_SetText(hBts[5], pStrBuf);
//       }
       WM_SetCallback(hBts[5],&myButtonListener);
       
       
       hBts[6]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
//       BUTTON_SetText(hBts[6], mntSetting.BGL_Setting.isSndEnable>DISABLE?"开启":"关闭");
       WM_SetCallback(hBts[6], &myButtonListener);;
       
       for(i=0;i<7;i++)
       {
          BUTTON_SetBkColor(hBts[i], BUTTON_CI_UNPRESSED, pSkin->bt_bkColor);
          BUTTON_SetTextColor(hBts[i], BUTTON_CI_UNPRESSED,pSkin->bt_txColor);
       } 
       // USER START (Optionally insert additional code for further widget initialization)
       // USER END
       btReset();
       break;
       
       
  default:
       WM_DefaultProc(pMsg);
       break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateETWin
*/
WM_HWIN mntSettingWinCreate(void) {

  mntSettingWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return mntSettingWin;
}

// USER START (Optionally insert additional public code)

static void myButtonListener(WM_MESSAGE* pMsg)
{
	const WM_KEY_INFO* pInfo;
	WM_MESSAGE myMsg;
 
 MNT_BERTH * pIterator  = NULL;
	
	WM_HWIN focussedButton  = 0;
	
	int i  = 0;
	int Step  = 0;
 int tmpDist  = 0;

	
	switch(pMsg->MsgId)
	{
      case WM_SET_FOCUS:         
           i  = WM_GetId(pMsg->hWin) - ID_BUTTON_0;
           
           if(pMsg->Data.v)
           {
              BUTTON_SetBkColor(hBts[i], BUTTON_CI_UNPRESSED, pSkin->bt_bkFocus);
						       	BUTTON_SetTextColor(hBts[i], BUTTON_CI_UNPRESSED,pSkin->bt_txFocus);
           }
           else
           {
              BUTTON_SetBkColor(hBts[i], BUTTON_CI_UNPRESSED, pSkin->bt_bkColor);
						        BUTTON_SetTextColor(hBts[i], BUTTON_CI_UNPRESSED,pSkin->bt_txColor);
           }
           BUTTON_Callback(pMsg);
           break;				
		
		case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		
		  switch(pInfo->Key)
			{
				case GUI_KEY_UP:
         GUI_StoreKeyMsg(GUI_KEY_BACKTAB,1);
         break;
				
				case GUI_KEY_DOWN: 
 
         GUI_StoreKeyMsg(GUI_KEY_TAB,1);
         break;
      
    case GUI_KEY_RIGHT:
         Step  = 100;
    case GUI_KEY_LEFT:
         Step  = (Step>50)?50:-50;
         focussedButton  = WM_GetFocussedWindow();
         i  = WM_GetId(focussedButton) - ID_BUTTON_0;
        
         switch(i)
         {
            case 0:
                 if(mntSetting.DSP_Setting.isEnable)
                 {
                    mntSetting.DSP_Setting.isEnable  = DISABLE;
                    BUTTON_SetText(hBts[0], "关闭");
                 }
                 else
                 {
                    mntSetting.DSP_Setting.isEnable  = ENABLE;
                    BUTTON_SetText(hBts[0], "开启");
                 }
                 break;
                 
            case 1:
                 if(mntSetting.DRG_Setting.isEnable)
                 {
                    mntSetting.DRG_Setting.isEnable  = DISABLE;
                    BUTTON_SetText(hBts[1], "关闭");
                 }
                 else
                 {
                    mntSetting.DRG_Setting.isEnable  = ENABLE;
                    BUTTON_SetText(hBts[1], "开启");
                 }
                 break;
                 
            case 2:
                 mntSetting.DRG_Setting.Dist  += Step;
                 mntSetting.DRG_Setting.Dist  = (mntSetting.DRG_Setting.Dist+1050) %1050;
//INFO("DRG dist:%d",mntSetting.DRG_Setting.Dist);
                 
                 if(SysConf.Unit == UNIT_nm)
                 {
                    sprintf(pStrBuf, "%d.%02d", mntSetting.DRG_Setting.Dist /1000, mntSetting.DRG_Setting.Dist %1000 /10);
                    BUTTON_SetText(hBts[2], pStrBuf);
                 }
                 else 
                 {
                    tmpDist  = mntSetting.DRG_Setting.Dist *100 / 54;
                    sprintf(pStrBuf, "%d.%02d", tmpDist /1000, tmpDist %1000 /10);
                    BUTTON_SetText(hBts[2], pStrBuf);
                 }
                 break;
                 
            case 3:
                 if(mntSetting.DRG_Setting.isSndEnable) 
                 {                 
                    mntSetting.DRG_Setting.isSndEnable = DISABLE;
                    BUTTON_SetText(hBts[3], "关闭");
                 } 
                 else
                 {                
                    mntSetting.DRG_Setting.isSndEnable  = ENABLE;
                    BUTTON_SetText(hBts[3], "开启");
                 }                   
                 break;  
            case 4:
                 if(mntSetting.BGL_Setting.isEnable)
                 { 
                    mntSetting.BGL_Setting.isEnable  = DISABLE;
                    BUTTON_SetText(hBts[4], "关闭");
                 }
                 else
                 {
                    mntSetting.BGL_Setting.isEnable  = ENABLE;
                    BUTTON_SetText(hBts[4], "开启");
                 }
                 break;            
                 
            case 5:
                 mntSetting.BGL_Setting.Dist  += Step;
                 mntSetting.BGL_Setting.Dist  = (mntSetting.BGL_Setting.Dist+1050) %1050;
//INFO("DRG dist:%d",mntSetting.BGL_Setting.Dist);                
                 if(SysConf.Unit == UNIT_nm)
                 {
                    sprintf(pStrBuf, "%d.%02d", mntSetting.BGL_Setting.Dist /1000, mntSetting.BGL_Setting.Dist %1000 /10);
                    BUTTON_SetText(hBts[5], pStrBuf);
                 }
                 else
                 {
                    tmpDist  = mntSetting.BGL_Setting.Dist *100 /54;
                    sprintf(pStrBuf, "%d.%02d", tmpDist /1000, tmpDist %1000 / 10);
                    BUTTON_SetText(hBts[5], pStrBuf);
                 }
                 break; 
            
            case 6:
                 if(mntSetting.BGL_Setting.isSndEnable)
                 {
                    mntSetting.BGL_Setting.isSndEnable  = DISABLE;
                    BUTTON_SetText(hBts[6], "关闭");
                 }
                 else
                 {
                    mntSetting.BGL_Setting.isSndEnable  = ENABLE;
                    BUTTON_SetText(hBts[6], "开启");
                 }
                 break;             
         }
         break;
    	
    case GUI_KEY_BACKSPACE:
         pIterator  = pMntHeader;
         while(pIterator)
         {
            if(pIterator->chsState == MNTState_Choosen ||  pIterator->chsState == MNTState_Default)
            {
              if(Mem_isEqual(&pIterator->mntBoat.mntSetting, &mntSetting, sizeof(mntSetting)) == FALSE)
              {
                 myMsg.hWin  = WM_GetClientWindow(confirmWin);
                 myMsg.hWinSrc  = WM_GetClientWindow(mntSettingWin);
                 myMsg.MsgId  = USER_MSG_CHOOSE;
                 myMsg.Data.v = STORE_SETTING;
                 WM_SendMessage(myMsg.hWin, &myMsg);
                 
                 WM_BringToTop(confirmWin);
                 WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0)); 
                 return ;
              }
              {                  
              }
            }
            
            pIterator  = pIterator->pNext;
         }
         WM_SetFocus(subWins[1]);         
         break; 
         
				default:
					    BUTTON_Callback(pMsg);
				     break;
			}
   break;
			       
				default:
					   BUTTON_Callback(pMsg);
				break;
	}	 
  
}

// USER END

static void btReset(void)
{
     int tmpDist  = 0;
     mntSetting.DSP_Setting.isEnable  = ENABLE;
     
     mntSetting.DRG_Setting.isEnable  = ENABLE;
     mntSetting.DRG_Setting.isSndEnable  = ENABLE;
     mntSetting.DRG_Setting.Dist         = 100;
     
     mntSetting.BGL_Setting.isEnable  = ENABLE;
     mntSetting.BGL_Setting.isSndEnable  = ENABLE;
     mntSetting.BGL_Setting.Dist         = 50;
     
     BUTTON_SetText(hBts[0], mntSetting.DSP_Setting.isEnable>DISABLE?"开启":"关闭");
     BUTTON_SetText(hBts[1], mntSetting.DRG_Setting.isEnable>DISABLE?"开启":"关闭");
     BUTTON_SetText(hBts[3], mntSetting.DRG_Setting.isSndEnable>DISABLE?"开启":"关闭");
     BUTTON_SetText(hBts[4], mntSetting.BGL_Setting.isEnable>DISABLE?"开启":"关闭");
     BUTTON_SetText(hBts[6], mntSetting.BGL_Setting.isSndEnable>DISABLE?"开启":"关闭");
     
     if(SysConf.Unit == UNIT_nm)
     {
        sprintf(pStrBuf, "%d.%02d", mntSetting.DRG_Setting.Dist /1000, mntSetting.DRG_Setting.Dist %1000 /10);
        BUTTON_SetText(hBts[2], pStrBuf);
        
        sprintf(pStrBuf, "%d.%02d", mntSetting.BGL_Setting.Dist /1000, mntSetting.BGL_Setting.Dist %1000 /10);
        BUTTON_SetText(hBts[5], pStrBuf);
     }
     else
     {
        tmpDist  = mntSetting.DRG_Setting.Dist *100 /54;
        sprintf(pStrBuf, "%d.%02d", tmpDist /1000, tmpDist %1000 / 10);
        BUTTON_SetText(hBts[2], pStrBuf);
        
        tmpDist  = mntSetting.BGL_Setting.Dist *1000 /54;
        sprintf(pStrBuf, "%d.%02d", tmpDist /1000, tmpDist %1000 /10);
        BUTTON_SetText(hBts[5], pStrBuf);
     }
}





/*************************** End of file ****************************/
