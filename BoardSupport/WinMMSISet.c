#include "WinMMSISet.h"
int getflag();

/** @brief MMSI */ //for Liyi
long MMSI=0;

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

/** @brief color config */
//static const MMSISetWinColor *pColor = mmsiSetWinColor;
extern const MenuColor subMenuColors[2];

/** @brief skin config */
static const BUTTON_SKINFLEX_PROPS *pSkin; //= btSkin[0];

/** @brief MMSI OK */

static int myOperat;//发送出去的消息ID，等待响应
/** @brief MMSI编辑框 
	*
 *    @param [in] pMsg  消息指针
 */																												
BUTTON_SKINFLEX_PROPS inputBtSkin[2][2] = //FOR_LIYI
{
/* day */	  
           /* nofocus */      {{{GUI_WHITE,0x008b9da5,0x008b9da5},{0x008b9da5,0x008b9da5},{0x008b9da5,0x008b9da5},0}, 
            /* focus */       {{GUI_WHITE,FOCUS_COLOR_DAY,FOCUS_COLOR_DAY},{FOCUS_COLOR_DAY,FOCUS_COLOR_DAY},{FOCUS_COLOR_DAY,FOCUS_COLOR_DAY},0}},

/* night */	
         /* nofocus */        {{{GUI_WHITE,0x00303030,0x00303030},{0x00303030,0x00303030},{0x00303030,0x00303030},0}, 
           /* focus */        {{GUI_WHITE,FOCUS_COLOR_NIGHT,FOCUS_COLOR_NIGHT},{FOCUS_COLOR_NIGHT,FOCUS_COLOR_NIGHT},{FOCUS_COLOR_NIGHT,FOCUS_COLOR_NIGHT},0}}
};	



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
 long MMSI_tmp;
 char edittext[10];
 int i;
	switch(pMsg->MsgId){
      case WM_SET_FOCUS:
						     if(pMsg->Data.v)
											{
//												WM_SetFocus(buttons[1]);
											}
											else
											{
												
												
												//刷新MMSI输入错误提示
												TEXT_GetText(Hint,HintText,20);
												if(HintText[0]!=NULL)
												{
													TEXT_SetText(Hint,"");
												}
											}
											BUTTON_Callback(pMsg);
           break;
//											
//						case WM_PAINT:
//											
//							    break;
						
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
                  item_id = WM_GetId(pMsg->hWin);
                  
													     if(item_id == ID_BUTTON_DEL)
																			EDIT_AddKey(edit,GUI_KEY_BACKSPACE); //回删
																		else if(item_id == ID_BUTTON_EMPTY)
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
                  if(!getflag() && t90_set.as_MMSI.port)
                  {
                     if(EDIT_GetNumChars(edit)==0)
                     {
                        WM_BringToTop(FleetWin);
                        WM_SetFocus(FleetWin);
                        break;
                     }
                  }
														    if(EDIT_GetNumChars(edit)==9)
																		{
//																			WM_HideWindow(SoftInputWin);
//																			WM_SetFocus(edit);
                    EDIT_GetText(edit,edittext,10);
                    MMSI_tmp = 0;	
                    for(i=0;i<MMSI_LENGTH;i++)
                    {
                       MMSI_tmp = MMSI_tmp*10+(edittext[i]-48); 
                    }
                    if(MMSI_tmp == t90_set.as_MMSI.MMSI && t90_set.as_MMSI.port)
                    {
                       WM_SetFocus(FleetWin);
                       WM_BringToTop(FleetWin);
                       break;
                    }
                    if(myOperat==MONITMMSI_SET) //设置辅船MMSI
                    {
                    myMsg.hWin = confirmWin;
                    myMsg.hWinSrc = MMSISetWin;
                    myMsg.MsgId = USER_MSG_CHOOSE;
                    myMsg.Data.v = MONITMMSI_SET;
                    WM_SendMessage(myMsg.hWin,&myMsg);
                    WM_BringToTop(confirmWin);
                    WM_SetFocus(confirmWin);
                   }
                   else if(myOperat==MONITMMSI_ADD)//添加监控船舶
                   {
                    myMsg.hWin = confirmWin;
                    myMsg.hWinSrc = MMSISetWin;
                    myMsg.MsgId = USER_MSG_CHOOSE;
                    myMsg.Data.v = MONITMMSI_ADD;
                    WM_SendMessage(myMsg.hWin,&myMsg);
                    WM_BringToTop(confirmWin);
                    WM_SetFocus(confirmWin);
                   }
																		}
																		else
																		{
                   if(t90_set.as_MMSI.port)
                   {
                      myMsg.hWin = confirmWin;
                      myMsg.hWinSrc = SoftInputWin;
                      myMsg.MsgId = USER_MSG_CHOOSE;
                      myMsg.Data.v = MONITMMSI_NINE;
                      WM_SendMessage(myMsg.hWin,&myMsg);
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(confirmWin);
                   }
                   else
                   {
                      myMsg.hWin = confirmWin;
                      myMsg.hWinSrc = SoftInputWin;
                      myMsg.MsgId = USER_MSG_CHOOSE;
                      myMsg.Data.v = MONITMMSI_FIRST;
                      WM_SendMessage(myMsg.hWin,&myMsg);
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(confirmWin);
                   }
																		}
														    break;
												}
            WM_DefaultProc(pMsg);
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
	 uint8_t i;
		buttons[1] = BUTTON_CreateEx(INPUT_BT0_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_0); 
  WM_ShowWin(buttons[1]);
	 BUTTON_SetSkinFlexProps(inputBtSkin[t90_set.sys.nightmode],BUTTON_SKINFLEX_PI_ENABLED);
	 BUTTON_SetSkinFlexProps(inputBtSkin[t90_set.sys.nightmode]+1,BUTTON_SKINFLEX_PI_FOCUSSED);
		WM_SetCallback(buttons[1], &InputBtCallback);                                       
		BUTTON_SetText(buttons[1], "0");
		//BUTTON_SetBkColor(buttons[1], BUTTON_CI_UNPRESSED,GUI_GRAY);
	
		buttons[2] = BUTTON_CreateEx(INPUT_BT1_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_1);	
  WM_ShowWin(buttons[2]);
		WM_SetCallback(buttons[2], &InputBtCallback);                                       
		BUTTON_SetText(buttons[2], "1");
		//BUTTON_SetBkColor(buttons[2], BUTTON_CI_UNPRESSED,GUI_GRAY);

		buttons[3] = BUTTON_CreateEx(INPUT_BT2_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_2);   
  WM_ShowWin(buttons[3]); 
		WM_SetCallback(buttons[3], &InputBtCallback);                                       
		BUTTON_SetText(buttons[3], "2");
		//BUTTON_SetBkColor(buttons[3], BUTTON_CI_UNPRESSED,GUI_GRAY);

		buttons[4] = BUTTON_CreateEx(INPUT_BT3_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_3); 
  WM_ShowWin(buttons[4]);   
		WM_SetCallback(buttons[4], &InputBtCallback);                                       
		BUTTON_SetText(buttons[4], "3");		
		//BUTTON_SetBkColor(buttons[4], BUTTON_CI_UNPRESSED,GUI_GRAY);

		buttons[5] = BUTTON_CreateEx(INPUT_BT4_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_4);
  WM_ShowWin(buttons[5]);    
		WM_SetCallback(buttons[5], &InputBtCallback);                                       
		BUTTON_SetText(buttons[5], "4");
		//BUTTON_SetBkColor(buttons[5], BUTTON_CI_UNPRESSED,GUI_GRAY);

		buttons[6] = BUTTON_CreateEx(INPUT_BT5_X,INPUT_BT_LIN1_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_DEL);
  WM_ShowWin(buttons[6]);    
		WM_SetCallback(buttons[6], &InputBtCallback);
  BUTTON_SetFont(buttons[6],&GUI_Font_T90_24);  
		BUTTON_SetText(buttons[6], "回删");	
		//BUTTON_SetBkColor(buttons[6], BUTTON_CI_UNPRESSED,GUI_GRAY);
		
		buttons[7] = BUTTON_CreateEx(INPUT_BT0_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_5);  
  WM_ShowWin(buttons[7]);  
		WM_SetCallback(buttons[7], &InputBtCallback);                                       
		BUTTON_SetText(buttons[7], "5");	
		//BUTTON_SetBkColor(buttons[7], BUTTON_CI_UNPRESSED,GUI_GRAY);
		
		buttons[8] = BUTTON_CreateEx(INPUT_BT1_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_6); 
  WM_ShowWin(buttons[8]);   
		WM_SetCallback(buttons[8], &InputBtCallback);                                       
		BUTTON_SetText(buttons[8], "6");	
		//BUTTON_SetBkColor(buttons[8], BUTTON_CI_UNPRESSED,GUI_GRAY);
		
		buttons[9] = BUTTON_CreateEx(INPUT_BT2_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_7);   
  WM_ShowWin(buttons[9]); 
		WM_SetCallback(buttons[9], &InputBtCallback);                                       
		BUTTON_SetText(buttons[9], "7");	
		//BUTTON_SetBkColor(buttons[9], BUTTON_CI_UNPRESSED,GUI_GRAY);
		
		buttons[10] = BUTTON_CreateEx(INPUT_BT3_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_8); 
  WM_ShowWin(buttons[10]);   
		WM_SetCallback(buttons[10], &InputBtCallback);                                       
		BUTTON_SetText(buttons[10], "8");	
		//BUTTON_SetBkColor(buttons[10], BUTTON_CI_UNPRESSED,GUI_GRAY);
		
		buttons[11] = BUTTON_CreateEx(INPUT_BT4_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_9);   
  WM_ShowWin(buttons[11]); 
		WM_SetCallback(buttons[11], &InputBtCallback);  
		BUTTON_SetText(buttons[11], "9");	
		//BUTTON_SetBkColor(buttons[11], BUTTON_CI_UNPRESSED,GUI_GRAY);
		
		buttons[12] = BUTTON_CreateEx(INPUT_BT5_X,INPUT_BT_LIN2_Y,INPUT_BT_WIDTH,INPUT_BT_HEIGHT,pMsg->hWin, WM_CF_SHOW,  0,  ID_BUTTON_EMPTY);  
  WM_ShowWin(buttons[12]);  
		WM_SetCallback(buttons[12], &InputBtCallback);
  BUTTON_SetFont(buttons[12],&GUI_Font_T90_24);   
		BUTTON_SetText(buttons[12], "清空");  
		//BUTTON_SetBkColor(buttons[12], BUTTON_CI_UNPRESSED,GUI_GRAY);	
		for(i=1;i<13;i++)
  {  
		   BUTTON_SetSkin(buttons[i],BUTTON_DrawSkinFlex); 
     BUTTON_SetTextColor(buttons[i],0,GUI_WHITE); 
  }
}


/** @brief MMSI输入窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void MMSIWindowCallback(WM_MESSAGE* pMsg){
	
	int i;
	int comparflag=0;
 char edittext[10];
	WM_HWIN hWin;
	int NCode, Id;
	int xSize;
	int ySize;
   long MMSI_tmp = 0;

	switch(pMsg->MsgId){
		case WM_SET_FOCUS:
       WM_SetFocus(SoftInputWin);
       break;
		case USER_MSG_REPLY:
         
			    switch(myOperat){
								case MONITMMSI_SET:
             WM_Paint(MMSISetWin);
								     if(pMsg->Data.v==REPLY_OK)
													{
              if(t90_set.as_MMSI.port)
              {
               WM_BringToTop(FleetWin);
               WM_SetFocus(FleetWin);
              
              }
              else
              {
               WM_BringToTop(mainShipWin);
               WM_SetFocus(mainShipWin);
              }
                                          MMSI  = 0;
														EDIT_GetText(edit,edittext,10);
              for(i=0;i<MMSI_LENGTH;i++)
              {
               MMSI = MMSI*10+(edittext[i]-48); 
              }
														hWin = WM_GetDialogItem(FleetWin,GUI_ID_EDIT0);
														EDIT_SetText(hWin,edittext);
              t90_set.as_MMSI.MMSI = MMSI;
              t90_set.as_MMSI.port = 1;
              T90_Store();
														
                                         
													}
													else if(pMsg->Data.v==REPLY_CANCEL)
													{
              if(t90_set.as_MMSI.port)
              {
														   WM_BringToTop(FleetWin);
                 WM_BringToTop(mainMenuDlg);
														   WM_SetFocus(FleetWin);
              }
              else
              {
                 WM_BringToTop(MMSISetWin);
                 WM_SetFocus(MMSISetWin);
              }
													}
									    break;
								
								case MONITMMSI_ADD:
									    if(pMsg->Data.v==REPLY_OK)
													{
														EDIT_GetText(edit,edittext,10);
														MMSI_tmp = 0;	
														for(i=0;i<MMSI_LENGTH;i++)
              {
               MMSI_tmp = MMSI_tmp*10+(edittext[i]-48); 
              }
														//判断输入的MMSI是否已存在
														comparflag=0;
														for(i=0;i<MonitShipNum;i++)
														{
															if(monitMMSI[i]==MMSI_tmp)
																comparflag = 1;
														}
														if(comparflag==0)
														{
															monitMMSI[MonitShipNum] = MMSI_tmp;
															MonitShipNum++;
															hWin = WM_GetDialogItem(FleetWin,GUI_ID_BUTTON0+MonitShipNum);
															BUTTON_SetText(hWin,"删除");
															WM_BringToTop(FleetWin);
															WM_SetFocus(WM_GetDialogItem(FleetWin,GUI_ID_BUTTON0));
                                             t90_set.shipout.MMSI[MonitShipNum-1] = MMSI_tmp;
                                             t90_set.shipout.numShip = MonitShipNum;
                                             
                                             T90_Store();
                                            
														}
														else
														{
               GUI_SetColor(GUI_RED);
															TEXT_SetText(Hint,"该船只已存在。");//don't forget
															WM_ShowWin(SoftInputWin);
															WM_SetFocus(SoftInputWin);
														}
													}
													else if(pMsg->Data.v==REPLY_CANCEL)
													{
														WM_BringToTop(FleetWin);
														WM_SetFocus(WM_GetDialogItem(FleetWin,GUI_ID_BUTTON0));
													}
             WM_Paint(MMSISetWin);
												 break;
        
							}
			    break;
			case USER_MSG_SKIN:
				  BUTTON_SetSkinFlexProps(inputBtSkin[pMsg->Data.v],BUTTON_SKINFLEX_PI_ENABLED);
					  BUTTON_SetSkinFlexProps(inputBtSkin[pMsg->Data.v]+1,BUTTON_SKINFLEX_PI_FOCUSSED);
			     for(i=1;i<13;i++)
								{
									WM_Invalidate(buttons[i]);
								}
								break;
			
			case USER_MSG_MMSISET:
								WM_BringToTop(MMSISetWin);
			     WM_ShowWin(SoftInputWin);
								WM_SetFocus(SoftInputWin);
								myOperat = pMsg->Data.v;
			               if(myOperat==MONITMMSI_SET)//MMSISet
								{
									EDIT_GetText(WM_GetDialogItem(pMsg->hWinSrc,GUI_ID_EDIT0),edittext,10);
									EDIT_SetText(edit,edittext);
								}
								else if(myOperat==MONITMMSI_ADD)//MMSIAdd
								{
									EDIT_SetText(edit,"");
								}
								break;
			
			case WM_CREATE:
        Hint = TEXT_CreateEx(120,75,200,30,pMsg->hWin,WM_CF_SHOW,0,GUI_ID_TEXT0,"");
        TEXT_SetFont(Hint,&GUI_Font_T90_24);
        TEXT_SetTextColor(Hint,GUI_WHITE);

        edit = EDIT_CreateEx(MMSISET_ET_X,MMSISET_ET_Y,MMSISET_ET_WIDTH,MMSISET_ET_HEIGHT,pMsg->hWin,WM_CF_SHOW,0,GUI_ID_EDIT0,9);
        EDIT_SetBkColor(edit,EDIT_CI_ENABELD,GUI_WHITE);
        EDIT_EnableBlink(edit,0,0);
        EDIT_SetFont(edit,&GUI_Font_T90_30);
        MMSI = t90_set.as_MMSI.MMSI;
  
        break;
								
			case WM_PAINT:
				     xSize = WM_GetWindowSizeX(pMsg->hWin);
			      ySize = WM_GetWindowSizeY(pMsg->hWin);
			      GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 20, homeColors[t90_set.sys.nightmode].bbsTopColor,homeColors[t90_set.sys.nightmode].bbsBottomColor);
			
			      GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			      GUI_SetColor(setWinColors[t90_set.sys.nightmode].textColor);
                 
         GUI_SetFont(&GUI_Font_T90_24);
         GUI_SetColor(setWinColors[t90_set.sys.nightmode].focusBkColor);
         GUI_DispStringAt("【确认】", 30, 260);
         GUI_SetColor(GUI_WHITE);
         GUI_DispString("确认选择，"); 
         GUI_SetColor(setWinColors[t90_set.sys.nightmode].focusBkColor);
         GUI_DispString(" 卞 咗祐 ");
         GUI_SetColor(GUI_WHITE);
         GUI_DispString("选择选项，");
         GUI_SetColor(setWinColors[t90_set.sys.nightmode].focusBkColor);
         GUI_DispString("【返回】");
         GUI_SetColor(GUI_WHITE);
         GUI_DispString("退出"); 
         GUI_SetFont(&GUI_Font_T90_30);
         if(getflag())
            GUI_DispStringAt("请输入辅船九位码",160,40);   
         else
            GUI_DispStringAt("请输入船队船只的九位码", 130, 40);
         
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
	int xSize ,ySize;
	GUI_AUTODEV AutoDev;
	GUI_AUTODEV_INFO AutoDevInfo;
	
	xSize = WM_GetWindowSizeX(pMsg->hWin);
	ySize = WM_GetWindowSizeY(pMsg->hWin);
	
	switch(pMsg->MsgId){
   case USER_MSG_REPLY:
         if(pMsg->Data.v == REPLY_OK)
         {

            WM_SetFocus(MMSISetWin);
            WM_BringToTop(MMSISetWin);
          

         }
         else
         {            
            WM_BringToTop(FleetWin);
            WM_SetFocus(FleetWin);           
         }
         
         break;  
			case WM_CREATE:
				    //pColor = &mmsiSetWinColor[t90_set.sys.nightmode];
								inputBtCreat(pMsg);  
								break;
			
			case WM_PAINT:
			     GUI_SetColor(GUI_GRAY);
			     GUI_FillRect(0,0,xSize,ySize);
			     
								break;
			
			case WM_SET_FOCUS:
        if(pMsg->Data.v)
        {
         WM_SetFocus(buttons[1]);
        }
        break;
							
			default:
								WM_DefaultProc(pMsg);
        break;
		}
}
	

/** @brief MMSI输入按键窗口创建
 *
 *    @param [out] 窗口句柄
 */
WM_HWIN WIN_MMSISetCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindow(MMSISET_X, MMSISET_Y, MMSISET_WIDTH, MMSISET_HEIGHT, WM_CF_SHOW, &MMSIWindowCallback , 0);
			WM_SetHasTrans(handle);
   return handle;
}

/** @brief 输入法窗口创建
 *
 *    @param [out] 窗口句柄
 */
WM_HWIN WIN_SoftInputCreate(void){
   WM_HWIN handle;  
   handle  = WM_CreateWindowAsChild(INPUTWIN_X, INPUTWIN_Y, INPUTWIN_WIDTH, INPUTWIN_HEIGHT,MMSISetWin, WM_CF_SHOW|WM_CF_LATE_CLIP, &InputWindowCallback , 0);
//			WM_SetHasTrans(handle);
   return handle;
}
