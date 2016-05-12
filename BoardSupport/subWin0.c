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
#include "Config.h"
#include "MainTask.h"
#include "drawThings.h"
#include "Setting.h"
#include "sysConf.h"
#include "LISTVIEW.h"
#include "dlg.h"
#include "skinColor.h"
#include "str.h"
#include "28.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x10)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x11)
#define ID_TEXT_0     (GUI_ID_USER + 0x12)
#define ID_TEXT_1     (GUI_ID_USER + 0x13)
#define ID_TEXT_2     (GUI_ID_USER + 0x14)
#define ID_TEXT_3     (GUI_ID_USER + 0x15)
#define ID_TEXT_4     (GUI_ID_USER + 0x16)
#define ID_TEXT_5     (GUI_ID_USER + 0x17)
#define ID_TEXT_6     (GUI_ID_USER + 0x18)
#define ID_TEXT_7     (GUI_ID_USER + 0x19)
#define ID_TEXT_8     (GUI_ID_USER + 0x1a)

unsigned char isSub0Inited  = 0;

/*----------------- external variables ---------------------*/
extern int N_boat;
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];


/*----------------- local    variables ---------------------*/
static int SelRow  = -1;

/*----------------- local     function ---------------------*/
static void myListViewListener(WM_MESSAGE* pMsg);
static void updateListViewContent(WM_HWIN thisHandle);


/*----------------- static & const --------------------------*/

/* Rect 为监控船舶信息所占的区域 
* @ Attention: 矩形坐标是相对于窗口 subWins[0] 左上角点的坐标
*/
static const GUI_RECT infoRect={LV_MoniteList_WIDTH+10,LV_MoniteList_Y,Win_Main_WIDTH-MenuLabel_WIDTH ,LV_MoniteList_HEIGHT+40};
static const GUI_RECT lvIndicate  = {LV_MoniteList_WIDTH-120,LV_MoniteList_Y-25,Win_Main_WIDTH-MenuLabel_WIDTH,LV_MoniteList_Y};


// USER START (Optionally insert additional defines)
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
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, SubWin_X, SubWin_Y, SubWin_WIDTH, SubWin_HEIGHT, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, LV_MoniteList_X, LV_MoniteList_Y, LV_MoniteList_WIDTH, LV_MoniteList_HEIGHT, 0, 0x0, 0 }
  // USER START (Optionally insert additional widgets)
  // USER END
};



static const LVWin_COLOR * pSkin  = &lvWinSkins[0];

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int  SelectedRow  = -1;
  int i  = 0;
  int TotalRows  = 0;
  MNT_BERTH * pIterator  = NULL;
  
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case USER_MSG_LV_UPDATE: 
       updateListViewContent(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));   
       break;
  
  case USER_MSG_SKIN: 
       pSkin  = &(lvWinSkins[pMsg->Data.v]);
       
       WINDOW_SetBkColor(pMsg->hWin,pSkin->bkColor);
         
            
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, pSkin->itm_bkUnsel);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL,   pSkin->itm_bkSel);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, pSkin->itm_bkFocus);
       
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL, pSkin->itm_txUnsel);
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,   pSkin->itm_txSel);
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS, pSkin->itm_txFocus);
       
       hItem  = LISTVIEW_GetHeader(hItem);
       HEADER_SetBkColor(hItem,pSkin->Header_Bk);
       HEADER_SetTextColor(hItem,pSkin->Header_Text);
       break;
  
  case WM_INIT_DIALOG:	
       pSkin  = &(lvWinSkins[SysConf.Skin]);
       //
       // Initialization of 'Window'
       //
       hItem = pMsg->hWin;
       WINDOW_SetBkColor(hItem, pSkin->bkColor);

       //
       // Initialization of 'Listview'
       //
       hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
       WM_SetCallback(hItem, &myListViewListener);
      
       LISTVIEW_AddColumn(hItem, LV_MoniteList_Col_0_WIDTH, "距离", GUI_TA_HCENTER | GUI_TA_VCENTER);
       LISTVIEW_AddColumn(hItem, LV_MoniteList_Col_1_WIDTH, "MMSI", GUI_TA_HCENTER | GUI_TA_VCENTER);
       LISTVIEW_AddColumn(hItem, LV_MoniteList_Col_2_WIDTH, "状态", GUI_TA_HCENTER | GUI_TA_VCENTER);

 
       LISTVIEW_SetGridVis(hItem, 1);
       LISTVIEW_SetHeaderHeight(hItem,LV_MoniteList_Header_HEIGHT);
       LISTVIEW_SetRowHeight(hItem,LV_MoniteList_Row_HEIGHT);
      
       LISTVIEW_SetFont(hItem, &GUI_Font30);	  
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, pSkin->itm_bkUnsel);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL,   pSkin->itm_bkSel);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, pSkin->itm_bkFocus);
       
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL, pSkin->itm_txUnsel);
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,   pSkin->itm_txSel);
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS, pSkin->itm_txFocus);
       
       hItem  = LISTVIEW_GetHeader(hItem);
       HEADER_SetBkColor(hItem,pSkin->Header_Bk);
       HEADER_SetTextColor(hItem,pSkin->Header_Text);
			
			 //LISTVIEW_SetFont (WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),&GUI_Font31);
			 //LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),NULL);
			 //LISTVIEW_SetItemText (WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),2,0,"O");
       // USER START (Optionally insert additional code for further widget initialization)
       // USER END
       isSub0Inited  = 1;
       break;

  case WM_PAINT:
       GUI_SetColor(pSkin->inf_bkColor);
       GUI_FillRectEx(&infoRect);
       
       GUI_SetFont(&GUI_Font30);
       GUI_SetTextMode(GUI_TM_TRANS); 
       
       GUI_SetColor(pSkin->ttl_Text);
       GUI_DispStringAt("监控列表",          (LV_MoniteList_WIDTH-90)/2, LV_MoniteList_Y-30);
       GUI_DispStringAt("监控船舶信息",      LV_MoniteList_WIDTH+100,   LV_MoniteList_Y-30) ;
       
       GUI_SetColor(pSkin->inf_Label);
       GUI_DispStringAt("船名",              LV_MoniteList_WIDTH+20, LV_MoniteList_Y+40);
       GUI_DispStringAt("北纬",              LV_MoniteList_WIDTH+20, LV_MoniteList_Y+80 );
       GUI_DispStringAt("东经",              LV_MoniteList_WIDTH+20, LV_MoniteList_Y+120);
       GUI_DispStringAt("航速",              LV_MoniteList_WIDTH+20, LV_MoniteList_Y+160);
       GUI_DispStringAt("航向",              LV_MoniteList_WIDTH+200, LV_MoniteList_Y+160);
       GUI_DispStringAt("消失报警",     LV_MoniteList_WIDTH+20, LV_MoniteList_Y+200);
       GUI_DispStringAt("走锚报警",     LV_MoniteList_WIDTH+20, LV_MoniteList_Y+240);
       GUI_DispStringAt("防盗报警",     LV_MoniteList_WIDTH+20, LV_MoniteList_Y+280);
//       if(SysConf.Unit == UNIT_km)
//       {
//          GUI_DispStringAt("km",             LV_MoniteList_WIDTH+210,LV_MoniteList_Y+240);
//          GUI_DispStringAt("km",             LV_MoniteList_WIDTH+210,LV_MoniteList_Y+280);
//       }
//       else
//       {
//          GUI_DispStringAt("nm",             LV_MoniteList_WIDTH+210,LV_MoniteList_Y+240);
//          GUI_DispStringAt("nm",             LV_MoniteList_WIDTH+210,LV_MoniteList_Y+280);
//       }
       GUI_SetColor(pSkin->inf_txColor);

       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
       
       if(WM_HasFocus(hItem) == FALSE)
       {
          sprintf(pStrBuf,"  0/%3d", TotalRows);
          GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH-80, LV_MoniteList_Y - 25);
          break;
       }
       SelectedRow  = LISTVIEW_GetSel(hItem);
       if( !pMntHeader  ||  SelectedRow < 0)
       {
          GUI_DispStringAt("  0/0 ",LV_MoniteList_WIDTH-80, LV_MoniteList_Y - 25);
          break;
       }
          
       TotalRows  = LISTVIEW_GetNumRows(hItem);     
       sprintf(pStrBuf,"%3d/%3d",SelectedRow+1, TotalRows);
       GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH-80, LV_MoniteList_Y-25);
       
       pIterator  = pMntHeader;
       for(i=0;i<SelectedRow;i++)
       {
          pIterator  = pIterator->pNext;
       }  
       if(pIterator == NULL  ||  pIterator ->chsState == MNTState_Cancel)
       {
          break;
       }
       
       GUI_DispStringAt(pIterator->mntBoat.name, LV_MoniteList_WIDTH+80, 80);         
       
       /// Display LL information of boats which is not gone.
       if(pIterator->pBerth) 
       {
          if(pIterator->pBerth->Boat.dist < 100000)
          {
             lltostr(pIterator->pBerth->Boat.latitude, pStrBuf);
             GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH+100, 120);
             
             lltostr(pIterator->pBerth->Boat.longitude, pStrBuf);
             GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH+100, 160);
             
   //          if(SysConf.Unit == UNIT_km)
   //          {
   //             int sog  = pIterator->pBerth->Boat.SOG * 18;
   //             sprintf(pStrBuf, "%3d.%02dkm", sog/100, sog%100);
   //          }
   //          else
   //          {
   //             sprintf(pStrBuf, "%2d.%dkt",pIterator->pBerth->Boat.SOG/10, pIterator->pBerth->Boat.SOG%10);
   //          }
             sprintf(pStrBuf, "%2d.%dkt", pIterator->pBerth->Boat.SOG/10, pIterator->pBerth->Boat.SOG%10);
             GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH+80, 200);
             
             sprintf(pStrBuf, "%3d", pIterator->pBerth->Boat.COG/10);
             pStrBuf[3]  = 194;
             pStrBuf[4]  = 176;
             pStrBuf[5]  = '\0'; 
             GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH+260, 200);
          }
       }
       /// Display LL information of boats which is gone.
       else if(pIterator->cfgState == MNTState_Monitored)
       {
          lltostr(pIterator->snapLt, pStrBuf);
          GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH+100, 120);
          
          lltostr(pIterator->snapLg, pStrBuf);
          GUI_DispStringAt(pStrBuf, LV_MoniteList_WIDTH+100, 160);
       }
      
       if(pIterator->mntBoat.mntSetting.DSP_Setting.isEnable == DISABLE)
          GUI_DispStringAt("关闭", LV_MoniteList_WIDTH+160,240);
       else
          GUI_DispStringAt("开启",  LV_MoniteList_WIDTH+160,240);
       
       if(pIterator->mntBoat.mntSetting.DRG_Setting.isEnable)
       {
          if(SysConf.Unit == UNIT_nm)       
          {
             sprintf(pStrBuf, "%d.%02d nm", pIterator->mntBoat.mntSetting.DRG_Setting.Dist/1000,
                                        (pIterator->mntBoat.mntSetting.DRG_Setting.Dist%1000)/10);
             GUI_DispStringAt(pStrBuf,  LV_MoniteList_WIDTH+160,280);                                   
          }
          else
          {
             int tmpDist  = 0;
             
             tmpDist  = pIterator->mntBoat.mntSetting.DRG_Setting.Dist * 100 / 54;
             sprintf(pStrBuf, "%d.%2d km", tmpDist/1000, (tmpDist%1000)/100);
             GUI_DispStringAt(pStrBuf,  LV_MoniteList_WIDTH+160,280);
          }       
       }
       else
       {
          GUI_DispStringAt("开启", LV_MoniteList_WIDTH+160, 280);
       }
       
       if(pIterator->mntBoat.mntSetting.BGL_Setting.isEnable)
       {
          if(SysConf.Unit == UNIT_nm)       
          {          
             sprintf(pStrBuf, "%d.%02d nm", pIterator->mntBoat.mntSetting.BGL_Setting.Dist/1000,
                                        (pIterator->mntBoat.mntSetting.BGL_Setting.Dist%1000)/10);
             GUI_DispStringAt(pStrBuf,  LV_MoniteList_WIDTH+160,320);                                     
          }
          else
          {
             int tmpDist  = 0;
             
             tmpDist  = pIterator->mntBoat.mntSetting.BGL_Setting.Dist * 100 / 54;
             sprintf(pStrBuf, "%d.%2d km", tmpDist/1000, (tmpDist%1000)/100);
             GUI_DispStringAt(pStrBuf,  LV_MoniteList_WIDTH+160,320);  
          } 
       }
       else
       {
          GUI_DispStringAt("关闭",LV_MoniteList_WIDTH+160, 320);
       }      
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
*       CreateWindow
*/
WM_HWIN sub0WinCreate(void) {
  WM_HWIN hWin;
	
	
//  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, menuWin, 0, 0);
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END




/**监控列表 LISTVIEW 的监听器（回调函数）
*
*/
static void myListViewListener(WM_MESSAGE* pMsg)
{
 int selectedRow  = -1;
  int lastRow  = 0;
  int i   = 0;
  
  long MMSI  = 0;
	 const WM_KEY_INFO * pInfo;
  
	 WM_HWIN thisListView  = pMsg->hWin; 
  WM_MESSAGE myMsg;
  
	switch(pMsg->MsgId)
	{
		case WM_SET_FOCUS:
       if(pMsg->Data.v)
       {
          if(LISTVIEW_GetNumRows(pMsg->hWin))
             LISTVIEW_SetSel(pMsg->hWin, 0);
          WM_InvalidateRect(subWins[0], &infoRect);
          WM_InvalidateRect(subWins[0], &lvIndicate);
       }   
       else
       {
           WM_InvalidateRect(subWins[0], &lvIndicate);   
           WM_InvalidateRect(subWins[0],&infoRect);
       }       
       LISTVIEW_Callback(pMsg);

       break;
       
		case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		
		  switch(pInfo->Key)
			{
    case GUI_KEY_PWM_INC:       
      WM_SendMessageNoPara(subWins[3], USER_MSG_DIM);
      break;
				case GUI_KEY_UP:
				case GUI_KEY_DOWN:
         LISTVIEW_Callback(pMsg);
         WM_InvalidateRect(subWins[0], &infoRect);    
         WM_InvalidateRect(subWins[0], &lvIndicate);       
         break;
         
				case GUI_KEY_BACKSPACE:		   
         WM_SetFocus(menuWin);
         break;
         
    case GUI_KEY_RIGHT:
         selectedRow  = LISTVIEW_GetSel(thisListView);
         lastRow      = LISTVIEW_GetNumRows(thisListView);
         

         
         if(selectedRow/10 < lastRow/10)
         {
            LISTVIEW_SetSel(thisListView, lastRow);         
            LISTVIEW_SetSel(thisListView, selectedRow-selectedRow%10+10);
         }
         WM_InvalidateRect(subWins[0], &lvIndicate);
         break;
         
    case GUI_KEY_LEFT:
         selectedRow  = LISTVIEW_GetSel(thisListView);
         lastRow      = LISTVIEW_GetNumRows(thisListView);
         
         if(selectedRow > 9)
         {
            LISTVIEW_SetSel(thisListView, lastRow);
            LISTVIEW_SetSel(thisListView, selectedRow-selectedRow%10-10);
         }
         WM_InvalidateRect(subWins[0], &lvIndicate);         
         break;
         
				case GUI_KEY_MENU:
         WM_BringToTop(mapWin);
         WM_SetFocus(mapWin);
         break;
    
    case GUI_KEY_ENTER:
    case GUI_KEY_CANCEL:
         SelRow  = LISTVIEW_GetSel(thisListView);
         if(SelRow>=0)
         {
           LISTVIEW_GetItemText(thisListView, 1, SelRow, pStrBuf, 11);
           
           if(strtoi(pStrBuf))
           {
             myMsg.hWin  = WM_GetClientWindow(confirmWin);
             myMsg.hWinSrc  = thisListView;
             myMsg.MsgId  = USER_MSG_CHOOSE;
             myMsg.Data.v  = CANCEL_MONITED;
             WM_SendMessage(myMsg.hWin, &myMsg);
             WM_BringToTop(confirmWin);
             WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0));
             }
         }
         break;
         
				default:
				    	LISTVIEW_Callback(pMsg);
					break;
			}
			break;
		
  case USER_MSG_REPLY:
       switch(pMsg->Data.v)
       {
          case REPLY_OK:  
          
               LISTVIEW_GetItemText(thisListView, 1, SelRow, pStrBuf, 11);
               MMSI  = strtoi(pStrBuf);
               if(MNT_removeById(MMSI))
               {              
                  for(i=N_boat-1; i>=0; i--)
                  {
                     if(SimpBerthes[i].pBerth->Boat.user_id == MMSI)
                     {
                        SimpBerthes[i].pBerth->mntState  = MNTState_None;
                        break;
                     }
                  }                
                  LISTVIEW_DeleteRow(thisListView, SelRow);
                  
                  myMsg.hWin  = WM_GetClientWindow(menuWin);               
                  myMsg.MsgId  = USER_MSG_DFULT_CNT;
                  myMsg.Data.v  = MNT_getDefaultNum();;
                  WM_SendMessage(myMsg.hWin, &myMsg);
                  
                  WM_SetFocus(subWins[0]);
               }
               else
               {
                  WM_SetFocus(menuWin);
               } 
               break;
               
          case REPLY_CANCEL:
               WM_SetFocus(pMsg->hWin);
               break;
           
          default:
INFO("reply error!");          
               break;
       }
       break;
  
		default:
         LISTVIEW_Callback(pMsg);
         break;
	}
}


/**监控列表 LISTVIEW 条目更新
*
*/
static void updateListViewContent(WM_HWIN thisHandle)
{
   WM_HWIN  thisListView  = thisHandle;
   int Cnt  = 0;
   int NumOfRows  = 0;
   unsigned char tmpTrgState  = 0;
   MNT_BERTH * pIterator  = NULL;
   
   NumOfRows  = LISTVIEW_GetNumRows(thisListView);

   pIterator  = pMntHeader;
   while(pIterator)
   {
      if(pIterator->chsState != MNTState_Cancel)
      {
         Cnt++;
         if(Cnt > NumOfRows)
         {
            LISTVIEW_AddRow(thisListView, NULL);
            NumOfRows  = LISTVIEW_GetNumRows(thisListView);
         }
         
         if(pIterator->pBerth && pIterator->pBerth->Boat.user_id == pIterator->mntBoat.mmsi)
         {
            disttostr(pStrBuf, pIterator->pBerth->Boat.dist);
            LISTVIEW_SetItemText(thisListView, 0, Cnt-1, pStrBuf);     
         }      
         else
         {
            LISTVIEW_SetItemText(thisListView, 0, Cnt-1, "????");
         }
         
         sprintf(pStrBuf, "%09ld", pIterator->mntBoat.mmsi);
         LISTVIEW_SetItemText(thisListView, 1, Cnt-1, pStrBuf);   
         
         tmpTrgState  = pIterator->trgState>>4;                 
         switch(tmpTrgState)
         {
            case 0x08:
                 LISTVIEW_SetItemText(thisListView, 2, Cnt-1, "消失");
                 break;   

            case 0x04:/// DRG
                 LISTVIEW_SetItemText(thisListView, 2, Cnt-1, "走锚");
                 break;
                 
            case 0x02:/// BLG
                 LISTVIEW_SetItemText(thisListView, 2, Cnt-1, "防盗");
                 break;

            default:
                 if(pIterator->cfgState == MNTState_Pending || pIterator->trgState == MNTState_Pending)
                    LISTVIEW_SetItemText(thisListView, 2, Cnt-1, "??");
                 else
                    LISTVIEW_SetItemText(thisListView, 2, Cnt-1, " ");
                 break;
         }
 
      }
      
      pIterator  = pIterator->pNext;
   }

   while(NumOfRows > Cnt+1)
   {
      LISTVIEW_DeleteRow(thisListView, NumOfRows-1);
      NumOfRows  = LISTVIEW_GetNumRows(thisListView);
   }
   
   if(NumOfRows == 0)
   {
      LISTVIEW_AddRow(thisListView, NULL);
   }
   
   WM_InvalidateRect(subWins[0], &lvIndicate);   
}



/*************************** End of file ****************************/
