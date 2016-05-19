#include "Config.h"
#include "WM.h"
#include "MainTask.h"
#include "dlg.h"
#include "T90.h"
#include "HSD_STICKFIGURE.h"
#include "HSD_DIMENSION.h"
#include "stdio.h"
#include "T90.h"
#include "layout_dst_set.h"
#include "t90font.h"

#define SF_NUM  3
#define DMS_NUM 5

WM_HWIN singleShipDstSetWin;

static const GUI_RECT drawArea  = {50, 50, DST_SET_WIDTH-50, DST_SET_HEIGHT-50};

static const GUI_RECT tipStrArea = {50, DST_SET_HEIGHT-50 +2, DST_SET_WIDTH-50, DST_SET_HEIGHT-50 +32};

static Dst_Set agent_set;

static HSD_STICKFIGURE_Handle hStickFigures[3];
static HSD_DIMENSION_Handle hDimensions[5];

static const SetWinColor *pColors = setWinColors;

static void dimensionCallback(WM_MESSAGE* pMsg)
{
	WM_MESSAGE myMsg;
   int id  = 0;
   switch(pMsg->MsgId)
	{
   case WM_KEY:
        switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key)
				{
					case GUI_KEY_PWM_INC:       
							 WM_SendMessageNoPara(systemSetDlg, USER_MSG_DIM);
							 break;
					 case GUI_KEY_UP:
						    id  = WM_GetId(pMsg->hWin) - ID_DMS_0;

								if(id==0)      {agent_set.dst1+=5;sprintf(pStrBuf,"%d",agent_set.dst1);}
					      else if(id==1) {agent_set.dst2+=5;sprintf(pStrBuf,"%d",agent_set.dst2);}
					      else if(id==2) {agent_set.dst3+=5;sprintf(pStrBuf,"%d",agent_set.dst3);}
								else if(id==3) {agent_set.dst4+=5;sprintf(pStrBuf,"%d",agent_set.dst4);}
								else if(id==4) {agent_set.dst5+=5;sprintf(pStrBuf,"%d",agent_set.dst5);}
							  HSD_DIMENSION_SetValText(hDimensions[id], pStrBuf);		 
					      break;
								
					 case GUI_KEY_DOWN:
						    id  = WM_GetId(pMsg->hWin) - ID_DMS_0;
								if(id==0)      {agent_set.dst1-=5;sprintf(pStrBuf,"%d",agent_set.dst1);}
					      else if(id==1) {agent_set.dst2-=5;sprintf(pStrBuf,"%d",agent_set.dst2);}
					      else if(id==2) {agent_set.dst3-=5;sprintf(pStrBuf,"%d",agent_set.dst3);}
								else if(id==3) {agent_set.dst4-=5;sprintf(pStrBuf,"%d",agent_set.dst4);}
								else if(id==4) {agent_set.dst5-=5;sprintf(pStrBuf,"%d",agent_set.dst5);}
							  HSD_DIMENSION_SetValText(hDimensions[id], pStrBuf);
					      break;
								
					 case GUI_KEY_LEFT:
					 case GUI_KEY_RIGHT:
								id  = WM_GetId(pMsg->hWin) - ID_DMS_0;
								switch(id)
								{
									case 0:
										WM_SetFocus(hDimensions[1]);
										break;
									case 1:
										WM_SetFocus(hDimensions[0]);
										break;
									case 2:
										break;
									case 3:
										WM_SetFocus(hDimensions[4]);
										break;
									case 4:
										WM_SetFocus(hDimensions[3]);
										break;
								}
								break;
					 
           case GUI_KEY_BACKSPACE:
								if(Mem_isEqual(&t90_set.dst, &agent_set, sizeof(agent_set)))
								{
									WM_SetFocus(dstSetMenuDlg);
								}
								else
								{
									myMsg.hWin  = WM_GetClientWindow(confirmWin);
									myMsg.hWinSrc  = singleShipDstSetWin;
									myMsg.MsgId  = USER_MSG_CHOOSE;
									myMsg.Data.v  = SYS_SETTING;
									WM_SendMessage(myMsg.hWin, &myMsg);
									
									WM_BringToTop(confirmWin);
									WM_SetFocus(confirmWin); 
								}
                break;
        }
        break;
        
   default:
         HSD_DIMENSION_Callback(pMsg);
         break;
   }

}





static void myWindowCallback(WM_MESSAGE* pMsg)
{
   GUI_RECT clientRect;
	 int i;
   
   switch(pMsg->MsgId){
		 
	 case USER_MSG_DST_SET:
				for(i = 0; i < SF_NUM; i++)  //clear
				{
//					HSD_DIMENSION_SetValColor(hDimensions[i], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
//					HSD_DIMENSION_SetArrowLineColor(hDimensions[i], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
					HSD_STICKFIGURE_SetPenColor(hStickFigures[i], HSD_STICKFIGURE_CI_UNFOCUS, pColors->arrowLineColor);
				}
				if(pMsg->Data.v != -1)
				{
//					HSD_DIMENSION_SetValColor(hDimensions[pMsg->Data.v], HSD_DIMENSION_CI_UNFOCUS, pColors->focusArrowLineColor);
//					HSD_DIMENSION_SetArrowLineColor(hDimensions[pMsg->Data.v], HSD_DIMENSION_CI_UNFOCUS, pColors->focusArrowLineColor);
					HSD_STICKFIGURE_SetPenColor(hStickFigures[pMsg->Data.v], HSD_STICKFIGURE_CI_UNFOCUS, pColors->focusBkColor); 
			  }
				break;

	 case USER_MSG_SKIN:
				pColors = &(setWinColors[pMsg->Data.v]);	
				
				for(i = 0; i < DMS_NUM; i++)
				{
					HSD_DIMENSION_SetBkColor(hDimensions[i], HSD_DIMENSION_CI_UNFOCUS, pColors->bkColor);
					HSD_DIMENSION_SetBkColor(hDimensions[i], HSD_DIMENSION_CI_FOCUS, pColors->bkColor);
					
					HSD_DIMENSION_SetValColor(hDimensions[i], HSD_DIMENSION_CI_FOCUS, pColors->focusBkColor);
					HSD_DIMENSION_SetValColor(hDimensions[i], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
					
					HSD_DIMENSION_SetArrowLineColor(hDimensions[i], HSD_DIMENSION_CI_FOCUS, pColors->focusArrowLineColor);
					HSD_DIMENSION_SetArrowLineColor(hDimensions[i], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
				}
	 
	      break;   
   case WM_CREATE:
   
				agent_set.dst1 = t90_set.dst.dst1;
				agent_set.dst2 = t90_set.dst.dst2;
				agent_set.dst3 = t90_set.dst.dst3;
				agent_set.dst4 = t90_set.dst.dst4;
				agent_set.dst5 = t90_set.dst.dst5;
	 
				pColors = &setWinColors[t90_set.sys.nightmode];
	 
        hStickFigures[0]  = HSD_STICKFIGURE_CreateEx(DST_SET_WIDTH/2-100-8, 251, 16, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_SF_0);
        WM_SetHasTrans(hStickFigures[0]);
        
        hStickFigures[2]  = HSD_STICKFIGURE_CreateEx(DST_SET_WIDTH/2+100-8, 251, 16, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_SF_2);
        WM_SetHasTrans(hStickFigures[2]);
        
        hStickFigures[1]  = HSD_STICKFIGURE_CreateEx(DST_SET_WIDTH/2-8,     301, 16, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_SF_1);
        WM_SetHasTrans(hStickFigures[1]);
          
        hDimensions[0]  = HSD_DIMENSION_CreateEx(DST_SET_WIDTH/2-145, 112, 90, 140, pMsg->hWin, WM_CF_SHOW,HSD_DIMENSION_CF_VERTICAL, ID_DMS_0);
        HSD_DIMENSION_SetBkColor(hDimensions[0], HSD_DIMENSION_CI_UNFOCUS, pColors->bkColor);
        HSD_DIMENSION_SetBkColor(hDimensions[0], HSD_DIMENSION_CI_FOCUS, pColors->bkColor);
        HSD_DIMENSION_SetValFont(hDimensions[0], &GUI_Font20_ASCII);
				HSD_DIMENSION_SetValColor(hDimensions[0], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
				HSD_DIMENSION_SetValColor(hDimensions[0], HSD_DIMENSION_CI_FOCUS, pColors->focusBkColor);
				HSD_DIMENSION_SetArrowLineColor(hDimensions[0], HSD_DIMENSION_CI_FOCUS, pColors->focusArrowLineColor);
				sprintf(pStrBuf,"%d",t90_set.dst.dst1);
        HSD_DIMENSION_SetValText(hDimensions[0], pStrBuf);
        HSD_DIMENSION_SetUnitText(hDimensions[0], "m");
        WM_SetHasTrans(hDimensions[0]);
        WM_SetCallback(hDimensions[0],&dimensionCallback);
        
        hDimensions[1]  = HSD_DIMENSION_CreateEx(DST_SET_WIDTH/2-95, 240, 95, 40, pMsg->hWin, WM_CF_SHOW,HSD_DIMENSION_CF_HORIZONTAL, ID_DMS_1);
        HSD_DIMENSION_SetBkColor(hDimensions[1], HSD_DIMENSION_CI_UNFOCUS, pColors->bkColor);
        HSD_DIMENSION_SetBkColor(hDimensions[1], HSD_DIMENSION_CI_FOCUS,   pColors->bkColor);
        HSD_DIMENSION_SetValFont(hDimensions[1], &GUI_Font20_ASCII);
				HSD_DIMENSION_SetValColor(hDimensions[1], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
				HSD_DIMENSION_SetValColor(hDimensions[1], HSD_DIMENSION_CI_FOCUS, pColors->focusBkColor);
				HSD_DIMENSION_SetArrowLineColor(hDimensions[1], HSD_DIMENSION_CI_FOCUS, pColors->focusArrowLineColor);
        sprintf(pStrBuf,"%d",t90_set.dst.dst2);
        HSD_DIMENSION_SetValText(hDimensions[1], pStrBuf);
        HSD_DIMENSION_SetUnitText(hDimensions[1], "m");
        WM_SetHasTrans(hDimensions[1]);  
        WM_SetCallback(hDimensions[1],&dimensionCallback);        

        hDimensions[4]  = HSD_DIMENSION_CreateEx(DST_SET_WIDTH/2+55, 112, 90, 140, pMsg->hWin, WM_CF_SHOW,HSD_DIMENSION_CF_VERTICAL, ID_DMS_4);
        HSD_DIMENSION_SetBkColor(hDimensions[4], HSD_DIMENSION_CI_UNFOCUS, pColors->bkColor);
        HSD_DIMENSION_SetBkColor(hDimensions[4], HSD_DIMENSION_CI_FOCUS,   pColors->bkColor);
        HSD_DIMENSION_SetValFont(hDimensions[4], &GUI_Font20_ASCII);
				HSD_DIMENSION_SetValColor(hDimensions[4], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
				HSD_DIMENSION_SetValColor(hDimensions[4], HSD_DIMENSION_CI_FOCUS, pColors->focusBkColor);
				HSD_DIMENSION_SetArrowLineColor(hDimensions[4], HSD_DIMENSION_CI_FOCUS, pColors->focusArrowLineColor);
        sprintf(pStrBuf,"%d",t90_set.dst.dst5);
        HSD_DIMENSION_SetValText(hDimensions[4], pStrBuf);
        HSD_DIMENSION_SetUnitText(hDimensions[4], "m");
        WM_SetHasTrans(hDimensions[4]);  
        WM_SetCallback(hDimensions[4], &dimensionCallback);
        
        hDimensions[3]  = HSD_DIMENSION_CreateEx(DST_SET_WIDTH/2, 240, 95, 40, pMsg->hWin, WM_CF_SHOW,HSD_DIMENSION_CF_HORIZONTAL, ID_DMS_3);
        HSD_DIMENSION_SetBkColor(hDimensions[3], HSD_DIMENSION_CI_UNFOCUS, pColors->bkColor);
        HSD_DIMENSION_SetBkColor(hDimensions[3], HSD_DIMENSION_CI_FOCUS,   pColors->bkColor);
        HSD_DIMENSION_SetValFont(hDimensions[3], &GUI_Font20_ASCII);
				HSD_DIMENSION_SetValColor(hDimensions[3], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
				HSD_DIMENSION_SetValColor(hDimensions[3], HSD_DIMENSION_CI_FOCUS, pColors->focusBkColor);
				HSD_DIMENSION_SetArrowLineColor(hDimensions[3], HSD_DIMENSION_CI_FOCUS, pColors->focusArrowLineColor);
        sprintf(pStrBuf,"%d",t90_set.dst.dst4);
        HSD_DIMENSION_SetValText(hDimensions[3], pStrBuf);
        HSD_DIMENSION_SetUnitText(hDimensions[3], "m");
        WM_SetHasTrans(hDimensions[3]);
        WM_SetCallback(hDimensions[3], &dimensionCallback);
        
        hDimensions[2]  = HSD_DIMENSION_CreateEx(DST_SET_WIDTH/2-45, 112, 90, 190, pMsg->hWin, WM_CF_SHOW,HSD_DIMENSION_CF_VERTICAL, ID_DMS_2);
        HSD_DIMENSION_SetBkColor(hDimensions[2], HSD_DIMENSION_CI_UNFOCUS, pColors->bkColor);
        HSD_DIMENSION_SetBkColor(hDimensions[2], HSD_DIMENSION_CI_FOCUS,   pColors->bkColor);
        HSD_DIMENSION_SetValFont(hDimensions[2], &GUI_Font20_ASCII);
				HSD_DIMENSION_SetValColor(hDimensions[2], HSD_DIMENSION_CI_UNFOCUS, pColors->arrowLineColor);
				HSD_DIMENSION_SetValColor(hDimensions[2], HSD_DIMENSION_CI_FOCUS, pColors->focusBkColor);
				HSD_DIMENSION_SetArrowLineColor(hDimensions[2], HSD_DIMENSION_CI_FOCUS, pColors->focusArrowLineColor);
        sprintf(pStrBuf,"%d",t90_set.dst.dst3);
        HSD_DIMENSION_SetValText(hDimensions[2], pStrBuf);
        HSD_DIMENSION_SetUnitText(hDimensions[2], "m");
        WM_SetHasTrans(hDimensions[2]);    
        WM_SetCallback(hDimensions[2], &dimensionCallback);
        break;
				
	 case USER_MSG_REPLY:
				if(pMsg->Data.v == REPLY_OK)
				{
					 memcpy(&t90_set.dst, &agent_set, sizeof(agent_set));
					 T90_Store();
				}
				else
				{
					sprintf(pStrBuf,"%d",t90_set.dst.dst1);
					HSD_DIMENSION_SetValText(hDimensions[0], pStrBuf);
					sprintf(pStrBuf,"%d",t90_set.dst.dst2);
					HSD_DIMENSION_SetValText(hDimensions[1], pStrBuf);
					sprintf(pStrBuf,"%d",t90_set.dst.dst3);
					HSD_DIMENSION_SetValText(hDimensions[2], pStrBuf);
					sprintf(pStrBuf,"%d",t90_set.dst.dst4);
					HSD_DIMENSION_SetValText(hDimensions[3], pStrBuf);
					sprintf(pStrBuf,"%d",t90_set.dst.dst5);
					HSD_DIMENSION_SetValText(hDimensions[4], pStrBuf);
					agent_set = t90_set.dst;
				}
				WM_SetFocus(dstSetMenuDlg);
				break;
        
   case WM_PAINT:
        GUI_SetBkColor(pColors->bkColor);
        WM_GetClientRectEx(pMsg->hWin, &clientRect);
        GUI_ClearRectEx(&clientRect);
        GUI_SetColor(GUI_RED);
        GUI_SetFont(&GUI_Font_T90_24);
        GUI_DrawRectEx(&tipStrArea);
        GUI_DispStringAt("使用   选择选项，使用   选择数字。",50, DST_SET_HEIGHT-50 +2);
   
        GUI_SetDrawMode(GUI_DM_NORMAL);
        GUI_SetColor(pColors->textColor);
        
        {
           int orgX  = DST_SET_WIDTH /2;
           int orgY  = drawArea.y0 +1;
           
           GUI_DrawLine(orgX, orgY, orgX+16, orgY+16);
           
           GUI_DrawVLine(orgX+16, orgY+17, orgY+60);
           GUI_DrawHLine(orgY+60, orgX-15, orgX+15);
           GUI_DrawVLine(orgX-16, orgY+17, orgY+60);         
           
           GUI_DrawLine(orgX-16, orgY +16, orgX , orgY);         //母船
           
           GUI_SetLineStyle(GUI_LS_DOT);
           
           GUI_DrawLine(drawArea.x0, orgY+60, orgX-17, orgY+60);
           GUI_DrawLine(orgX+17, orgY+60, drawArea.x1, orgY+60);
           
           GUI_DrawLine(orgX-16, orgY+60, orgX-100, orgY+200);
           GUI_DrawLine(orgX+16, orgY+60, orgX+100, orgY+200);
        }
        
        
        break;
        
        
   default:
        WM_DefaultProc(pMsg);
        break;
   }
}


WM_HWIN WIN_SingleShipDstSetCreate(void)
{
   WM_HWIN handle;
   
   handle  = WM_CreateWindow(DST_SET_X, DST_SET_Y, DST_SET_WIDTH, DST_SET_HEIGHT,  WM_CF_SHOW, &myWindowCallback, 0);
   return handle;
}
