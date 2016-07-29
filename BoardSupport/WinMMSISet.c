#include "WinMMSISet.h"


/** @brief MMSI */
#define MMSI_NUM 9;
long MMSI=0;
static uint8_t MMSI_num=0;

/*********************************************************************
*
*             Static Data
*
**********************************************************************
*/

/** @brief  提示文本 */
static WM_HWIN Hint;

/** @brief MMSI输入按钮 */
static WM_HWIN buttons[13];

/** @brief MMSI编辑框 */
static WM_HWIN edit;

static const HomeColor *pColor = homeColors;

/** @brief MMSI编辑框 
	*
 *    @param [in] pMsg  消息指针
 */
																													
static void myEditCallback(WM_MESSAGE* pMsg){
		switch(pMsg->MsgId){
			case WM_SET_FOCUS:
				    if(pMsg->Data.v==1)
								{
									EDIT_SetBkColor(edit,EDIT_CI_ENABLED,pColor->numColor);
									EDIT_SetTextColor(edit,EDIT_CI_ENABELD,GUI_WHITE);
								}
								else
								{
									EDIT_SetBkColor(edit,EDIT_CI_ENABLED,GUI_WHITE);
									EDIT_SetTextColor(edit,EDIT_CI_ENABELD,GUI_BLACK);
								}
								EDIT_Callback(pMsg);
				    break;
								
			case WM_KEY:
								switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){			
									case GUI_KEY_ENTER:
														WM_ShowWindow(SoftInputWin);
									     WM_SetFocus(SoftInputWin);
														break;
									
									case GUI_KEY_RIGHT:
														WM_SetFocus(buttons[0]);
										    break;
								}
								break;
			default:
				EDIT_Callback(pMsg);
				break;
		}
}

/** @brief MMSI确定键回调 
 *
 *    @param [in] pMsg  消息指针
 */
static void btOkCallback(WM_MESSAGE* pMsg){
	int i;
	char textbuf[10];
	switch (pMsg->MsgId){						
		case WM_KEY:
							switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){	
								case GUI_KEY_ENTER:
									    EDIT_GetText(edit,textbuf,10);
													for(i=0;i<9;i++)
								      MMSI = MMSI*10+(textbuf[i]-48);
									    break;
								
								case GUI_KEY_LEFT:
									    WM_SetFocus(edit);
									    break;
							}
			    break;
		default:
			    BUTTON_Callback(pMsg);
							break;
	}
}

/** @brief 输入法按键窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void InputBtCallback(WM_MESSAGE* pMsg){
 WM_MESSAGE myMsg;
 int item_id;	
	char pBuf[2];
	char HintText[20];
	int key;
	switch(pMsg->MsgId){
      case WM_SET_FOCUS:
						     if(pMsg->Data.v)
											{
												BUTTON_SetBkColor(pMsg->hWin,BUTTON_CI_UNPRESSED,pColor->numColor);
											}
											else
											{
												BUTTON_SetBkColor(pMsg->hWin,BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
												
												//刷新MMSI输入错误提示
												TEXT_GetText(Hint,HintText,20);
												if(HintText[0]!=NULL)
												{
													TEXT_SetText(Hint,"");
												}
											}
											BUTTON_Callback(pMsg);
           break;
											
						case WM_KEY:
            switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){			
													
             case GUI_KEY_UP:
																		item_id = WM_GetId(pMsg->hWin);
													     if(item_id>ID_BUTTON_DEL)
																		{
																			WM_SetFocus(WM_GetDialogItem(SoftInputWin,item_id-6));
																		}
														    break;
																		
													case GUI_KEY_DOWN:
																		item_id = WM_GetId(pMsg->hWin);
													     if(item_id<ID_BUTTON_5)
																		{
																			WM_SetFocus(WM_GetDialogItem(SoftInputWin,item_id+6));
																		}
														    break;
													
													case GUI_KEY_LEFT:
																		WM_SetFocusOnPrevChild(SoftInputWin);
																		break;
													
												 case GUI_KEY_RIGHT:					
																		WM_SetFocusOnNextChild(SoftInputWin);
																		break;
													
													case GUI_KEY_ENTER:
														    BUTTON_GetText(pMsg->hWin,pBuf,2);
													     if(pBuf[0]=='X')
																			EDIT_AddKey(edit,GUI_KEY_BACKSPACE); //回删
																		else if(pBuf[0]=='C')
																			EDIT_SetText(edit,"");               //清空
																		else
																			EDIT_AddKey(edit,pBuf[0]);
																		
															 		//刷新MMSI输入错误提示
																		TEXT_GetText(Hint,HintText,20);
																		if(HintText[0]!=NULL)
																		{
																			TEXT_SetText(Hint,"");
																		}	
																		break;
																		
													case GUI_KEY_BACKSPACE:
														    if(EDIT_GetNumChars(edit)==9)
																		{
																			WM_HideWindow(SoftInputWin);
																			WM_SetFocus(edit);
																		}
																		else
																		{
																			TEXT_SetText(Hint,"MMSI ERR...");
																		}
														    break;
												}
												BUTTON_Callback(pMsg);
            WM_Paint(SoftInputWin);
											break;
												
						default :
           BUTTON_Callback(pMsg);
           break;			
					}						
}
/** @brief 输入数字按键控件创建
 *
 *    @param [in] pMsg  消息指针
 */
static void inputBtCreat(WM_MESSAGE *pMsg)
{
		buttons[1] = BUTTON_CreateEx(INPUT_BT0_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_0);   
		WM_SetCallback(buttons[1], &InputBtCallback);                                       
		BUTTON_SetText(buttons[1], "0");
		BUTTON_SetBkColor(buttons[1], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
	
		buttons[2] = BUTTON_CreateEx(INPUT_BT1_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_1);	
		WM_SetCallback(buttons[2], &InputBtCallback);                                       
		BUTTON_SetText(buttons[2], "1");
		BUTTON_SetBkColor(buttons[2], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);

		buttons[3] = BUTTON_CreateEx(INPUT_BT2_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_2);    
		WM_SetCallback(buttons[3], &InputBtCallback);                                       
		BUTTON_SetText(buttons[3], "2");
		BUTTON_SetBkColor(buttons[3], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);

		buttons[4] = BUTTON_CreateEx(INPUT_BT3_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_3);    
		WM_SetCallback(buttons[4], &InputBtCallback);                                       
		BUTTON_SetText(buttons[4], "3");		
		BUTTON_SetBkColor(buttons[4], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);

		buttons[5] = BUTTON_CreateEx(INPUT_BT4_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_4);    
		WM_SetCallback(buttons[5], &InputBtCallback);                                       
		BUTTON_SetText(buttons[5], "4");
		BUTTON_SetBkColor(buttons[5], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);

		buttons[6] = BUTTON_CreateEx(INPUT_BT5_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_DEL);    
		WM_SetCallback(buttons[6], &InputBtCallback);                                       
		BUTTON_SetText(buttons[6], "X");	
		BUTTON_SetBkColor(buttons[6], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
		
		buttons[7] = BUTTON_CreateEx(INPUT_BT0_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_5);    
		WM_SetCallback(buttons[7], &InputBtCallback);                                       
		BUTTON_SetText(buttons[7], "5");	
		BUTTON_SetBkColor(buttons[7], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
		
		buttons[8] = BUTTON_CreateEx(INPUT_BT1_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_6);    
		WM_SetCallback(buttons[8], &InputBtCallback);                                       
		BUTTON_SetText(buttons[8], "6");	
		BUTTON_SetBkColor(buttons[8], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
		
		buttons[9] = BUTTON_CreateEx(INPUT_BT2_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_7);    
		WM_SetCallback(buttons[9], &InputBtCallback);                                       
		BUTTON_SetText(buttons[9], "7");	
		BUTTON_SetBkColor(buttons[9], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
		
		buttons[10] = BUTTON_CreateEx(INPUT_BT3_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_8);    
		WM_SetCallback(buttons[10], &InputBtCallback);                                       
		BUTTON_SetText(buttons[10], "8");	
		BUTTON_SetBkColor(buttons[10], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
		
		buttons[11] = BUTTON_CreateEx(INPUT_BT4_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_9);    
		WM_SetCallback(buttons[11], &InputBtCallback);                                       
		BUTTON_SetText(buttons[11], "9");	
		BUTTON_SetBkColor(buttons[11], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);
		
		buttons[12] = BUTTON_CreateEx(INPUT_BT5_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_EMPTY);    
		WM_SetCallback(buttons[12], &InputBtCallback);                                       
		BUTTON_SetText(buttons[12], "C");	
		BUTTON_SetBkColor(buttons[12], BUTTON_CI_UNPRESSED,pColor->bbsBottomColor);	
}


/** @brief MMSI输入窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void MMSIWindowCallback(WM_MESSAGE* pMsg){

	int NCode, Id;
  switch(pMsg->MsgId){

    case WM_CREATE:
									
				     Hint = TEXT_CreateEx(100,75,150,30,pMsg->hWin,WM_CF_SHOW,0,GUI_ID_TEXT0,"");
				     TEXT_SetTextColor(Hint,GUI_RED);
				
									pColor = &homeColors[t90_set.sys.nightmode];
									edit = EDIT_CreateEx(MMSISET_X,MMSISET_Y,MMSISET_WIDTH,MMSISET_HEIGHT,pMsg->hWin,WM_CF_SHOW,0,GUI_ID_EDIT0,9);
									EDIT_SetBkColor(edit,EDIT_CI_ENABELD,GUI_WHITE);
				     EDIT_EnableBlink(edit,0,0);
				     WM_SetCallback(edit,&myEditCallback);
				
				     buttons[0] = BUTTON_CreateEx(MMSISET_BTOK_X,MMSISET_BTOK_Y,MMSISET_BTOK_WIDTH,MMSISET_BTOK_HEIGHT,pMsg->hWin, WM_CF_SHOW,0,ID_BUTTON_MMSIOK);  
				     BUTTON_SetTextColor(buttons[0],BUTTON_CI_UNPRESSED,GUI_WHITE);
				     BUTTON_SetSkinFlexProps(&noFocusskin,BUTTON_SKINFLEX_PI_ENABLED);
									BUTTON_SetSkinFlexProps(&noFocusskin,BUTTON_SKINFLEX_PI_PRESSED);
				     BUTTON_SetSkinFlexProps(&Focusskin,BUTTON_SKINFLEX_PI_FOCUSSED);
									BUTTON_SetSkin(buttons[0],BUTTON_DrawSkinFlex);
				     BUTTON_SetText(buttons[0],"OK");
									WM_SetCallback(buttons[0],&btOkCallback);   
         break;
         
    case WM_PAINT:
         GUI_SetBkColor(pColor->bkColor);
         GUI_Clear();
				     GUI_DispStringAt("INPUT MMSI", 30, 9);
									break;
    default:
         WM_DefaultProc(pMsg);
  }
}

/** @brief 输入法回调
 *
 *    @param [in] pMsg  消息指针
 */
static void InputWindowCallback(WM_MESSAGE* pMsg){
	int NCode, Id;
  switch(pMsg->MsgId){

    case WM_CREATE:
					    inputBtCreat(pMsg);  
         break;
				
				case WM_PAINT:
         GUI_SetBkColor(GUI_DARKGRAY);
         GUI_Clear();
				     
									break;
				
				case WM_SET_FOCUS:
								if(pMsg->Data.v)
								{
									WM_SetFocus(buttons[1]);
								}
								break;
								
    default:
         WM_DefaultProc(pMsg);
			}
}
	

/** @brief MMSI输入按键窗口创建
 *
 *    @param [out] 窗口句柄
 */
WM_HWIN WIN_MMSISetCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindow(MMSISET_X, MMSISET_Y, MMSISET_WIDTH, MMSISET_HEIGHT, WM_CF_SHOW, &MMSIWindowCallback , 0);
   return handle;
}


/** @brief 输入法窗口创建
 *
 *    @param [out] 窗口句柄
 */
WM_HWIN WIN_SoftInputCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindowAsChild(INPUTWIN_X, INPUTWIN_Y, INPUTWIN_WIDTH, INPUTWIN_HEIGHT,MMSISetWin, WM_CF_SHOW, &InputWindowCallback , 0);
   return handle;
}
