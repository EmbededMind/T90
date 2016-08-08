#include "WinFleet.h"


/** @brief  monitMMSI */
long monitMMSI[5]={0};
int MonitShipNum=0;
extern long MMSI; //辅船MMSI

/*********************************************************************
*
*             Static Data
*
**********************************************************************
*/
/*********************************
*
*       Window Handles
*
*/

/** @brief  listText */
GUI_ConstString listText[4] = {""};

/** @brief  edit */
static WM_HWIN edit;

/** @brief  addButton */
static WM_HWIN addbutton;

/** @brief  delButton */
static WM_HWIN delButton[5];

/** @brief  lsitview */
static WM_HWIN listview;
static int listviewfocus=0;


/** @brief  MMSIListRect */
static GUI_RECT MMSIList[5] = {
	{50,215,300,245},
	{50,250,300,280},
	{50,285,300,315},
	{50,320,300,350},
	{50,355,300,385}
};

static GUI_RECT titlebk = {5,180,45,385};
static GUI_RECT title = {16,190,34,385};
static GUI_RECT Head = {50,180,300,210};
static GUI_RECT Operat = {305,180,402,210};
static GUI_RECT note = {407,180,620,385};// to change

/** @brief skin config */
static const MenuColor *pColor = subMenuColors;

//static const BUTTON_SKINFLEX_PROPS *pSkin;// = btSkin[1];

static int myMsgId;//发送出去的消息ID，等待响应
static int delMMSI;//选择删除的MMSI

/** @brief 删除船只按键窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void delBtCallback(WM_MESSAGE* pMsg){
	int id;
	WM_MESSAGE myMsg;
	switch(pMsg->MsgId){
		case WM_SET_FOCUS:
			    if(pMsg->Data.v==1)
							{
								BUTTON_SetBkColor(pMsg->hWin,BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
							}
							else
							{
								BUTTON_SetBkColor(pMsg->hWin,BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
							}
							BUTTON_Callback(pMsg);
			    break;
							
		case WM_KEY:
			    switch(((WM_KEY_INFO*)pMsg->Data.p)->Key ){
								case GUI_KEY_UP:
														WM_SetFocusOnPrevChild(FleetWin);
									    break;
								
								case GUI_KEY_DOWN:
									    id = WM_GetId(pMsg->hWin);
									    if(id!=GUI_ID_BUTTON5)
													{
														if(id-0x170<MonitShipNum)
															WM_SetFocusOnNextChild(FleetWin);
													}
									    break;
								
								case GUI_KEY_LEFT:
									    break;
								
								case GUI_KEY_RIGHT:
									    break;
								
								case GUI_KEY_ENTER:
									    myMsg.hWin = confirmWin;
									    myMsg.hWinSrc = FleetWin;
									    myMsg.MsgId = USER_MSG_CHOOSE;
									    myMsg.Data.v = MONITMMSI_DEL;
													myMsgId = MONITMMSI_DEL;
									    WM_SendMessage(myMsg.hWin,&myMsg);
								
													WM_BringToTop(confirmWin);
													WM_SetFocus(confirmWin); 
									    BUTTON_Callback(pMsg);
									    break;
                          case GUI_KEY_BACKSPACE:
									    WM_SetFocus(WM_GetDialogItem(mainMenuDlg,GUI_ID_BUTTON3));
									    break;
                          
								  case GUI_KEY_MENU:
                               WM_BringToTop(mainShipWin);
                               WM_SetFocus(mainShipWin);
                               break;
//								default:
//													WM_DefaultProc(pMsg);
//													break;
							}
							break;
		default:
							BUTTON_Callback(pMsg);
							break;
		}
}

		
/** @brief 添加船只按键窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void addBtCallback(WM_MESSAGE* pMsg){
	WM_MESSAGE myMsg;
	int xSize,ySize;
	GUI_RECT RECT[1];
	switch(pMsg->MsgId){
//		case WM_SET_FOCUS:
//							if(pMsg->Data.v==1)
//							{
//								BUTTON_SetBkColor(pMsg->hWin,BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btFocusBkColor);//pColor->btFocusBkColor
//							}
//							else
//							{
//								BUTTON_SetBkColor(pMsg->hWin,BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
//							}
//							BUTTON_Callback(pMsg);
//			    break;
		
		case WM_PAINT:
		     xSize = WM_GetWindowSizeX(pMsg->hWin);
		     ySize = WM_GetWindowSizeY(pMsg->hWin);
		     RECT->x0 = 0;
		     RECT->x1 = xSize;
		     RECT->y0 = 0;
		     RECT->y1 = ySize;
							
			    if(WM_HasFocus(pMsg->hWin))
								GUI_SetColor(subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
							else
								GUI_SetColor(subMenuColors[t90_set.sys.nightmode].btBkColor);
		     GUI_FillRoundedRect(0,0,xSize-1,ySize-1,5);
							GUI_SetFont(&GUI_Font_T90_30);
							GUI_SetColor(subMenuColors[t90_set.sys.nightmode].btTextColor);
							GUI_SetTextMode(GUI_TEXTMODE_TRANS);
							GUI_DispStringInRect("添加船队船只",RECT,GUI_TA_VCENTER|GUI_TA_HCENTER);
							break;
								
		case WM_KEY:
							switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){
								case GUI_KEY_ENTER:
									    if(MonitShipNum<5)
													{
														myMsg.hWin = MMSISetWin;
														myMsg.hWinSrc = FleetWin;
														myMsg.MsgId = USER_MSG_MMSISET;
														myMsg.Data.v = MONITMMSI_ADD;
														WM_SendMessage(myMsg.hWin,&myMsg);
													}
													else
													{
														myMsg.hWin = confirmWin;
									     myMsg.hWinSrc = FleetWin;
									     myMsg.MsgId = USER_MSG_CHOOSE;
									     myMsg.Data.v = MONITMMSI_FULL;
														myMsgId = MONITMMSI_FULL;
									     WM_SendMessage(myMsg.hWin,&myMsg);
									
									     WM_BringToTop(confirmWin);
									     WM_SetFocus(confirmWin); 
													}
									    break;
								
								case GUI_KEY_UP:
									    WM_SetFocus(edit);
									    break;
								
								case GUI_KEY_DOWN:
									    if(MonitShipNum>0)
														WM_SetFocus(delButton[0]);
									    break;
                               
                          case GUI_KEY_BACKSPACE:
							 		    WM_SetFocus(WM_GetDialogItem(mainMenuDlg,GUI_ID_BUTTON3));
									    break;
                          
								  case GUI_KEY_MENU:
									    WM_BringToTop(mainShipWin);
									    WM_SetFocus(mainShipWin);
									    break;
//								default:
//													WM_DefaultProc(pMsg);
//													break;
							}
							break;
		default:
						 BUTTON_Callback(pMsg);
							break;
	}
}
	
	

/** @brief MMSI edit窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void myEditCallback(WM_MESSAGE* pMsg){
	WM_MESSAGE myMsg;
	char UserData;//窗口数据
		switch(pMsg->MsgId){
			case WM_SET_FOCUS:
				    if(pMsg->Data.v==1)
								{
									EDIT_SetBkColor(edit,EDIT_CI_ENABLED,subMenuColors[t90_set.sys.nightmode].btFocusBkColor); //don't forget to change
								}
								else
								{
									EDIT_SetBkColor(edit,EDIT_CI_ENABLED,GUI_WHITE);
								}
								EDIT_Callback(pMsg);
								break;
								
			case WM_KEY:
								switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){	
									case GUI_KEY_ENTER:
														myMsg.hWin = MMSISetWin;
									     myMsg.hWinSrc = FleetWin;
									     myMsg.MsgId = USER_MSG_MMSISET;
									     myMsg.Data.v = MONITMMSI_SET;
									     WM_SendMessage(myMsg.hWin,&myMsg);
														break;
									
									case GUI_KEY_UP:
										    break;
									
									case GUI_KEY_DOWN:
														WM_SetFocus(addbutton);
									     break;
                           
                           case GUI_KEY_BACKSPACE: 
										  WM_SetFocus(WM_GetDialogItem(mainMenuDlg,GUI_ID_BUTTON3));
										  break;
									
									case GUI_KEY_MENU:
										  WM_BringToTop(mainShipWin);
										  WM_SetFocus(mainShipWin);
										  break;
                           
//									default:
//														WM_DefaultProc(pMsg);
//										    break;
								}
								break;
		 default:
				    EDIT_Callback(pMsg);
						 	break;
							}
}

/** @brief 删除MMSI
 *
 *    @param [in] 无
 */
void DelMMSI(int dex){
 int i;
	WM_HWIN hWin;
	for(i=dex-1;i<4;i++)
		monitMMSI[i] = monitMMSI[i+1];
	hWin = WM_GetDialogItem(FleetWin,GUI_ID_BUTTON0+MonitShipNum);
	BUTTON_SetText(hWin,"");
	MonitShipNum--;
	monitMMSI[MonitShipNum]=0;
	WM_Paint(FleetWin);
}

/** @brief 我的船队窗口回调
 *
 *    @param [in] pMsg  消息指针
 */
static void FleetWinCallback(WM_MESSAGE* pMsg){
	char i;
	int NCode, Id;
   char editText[10];
   long bits = 1000000000; 
	WM_MESSAGE myMsg;
	BUTTON_SKINFLEX_PROPS pProps[1];
	
	switch(pMsg->MsgId){
		case WM_CREATE:
			    //pColor = &subMenuColors[t90_set.sys.nightmode];
			    //pSkin = btSkin[t90_set.sys.nightmode];
		
			  edit = EDIT_CreateEx(281,50,191,38,pMsg->hWin,WM_CF_SHOW,0,GUI_ID_EDIT0,9);
		     EDIT_EnableBlink(edit,0,0);
           EDIT_SetFont(edit,&GUI_Font_T90_30);
		     WM_SetCallback(edit,&myEditCallback);
           MonitShipNum = t90_set.shipout.numShip;
           for(i=0;i<5;i++)
            monitMMSI[i]=t90_set.shipout.MMSI[i];
           MMSI=t90_set.as_MMSI.MMSI;

           for(i=0;i<9;i++) 
		   {
			   editText[i] = ((MMSI%bits)/(bits/10))+48; 
			   bits /=10; 
		   }

           EDIT_SetText(edit,editText);
           
			  addbutton = BUTTON_CreateEx(103,116,159,30,pMsg->hWin, WM_CF_SHOW,0,GUI_ID_BUTTON0);
		     WM_SetHasTrans(addbutton);
		     //WM_HideWin(addbutton);
//		     BUTTON_SetSkinFlexProps(btSkin[t90_set.sys.nightmode],BUTTON_SKINFLEX_PI_ENABLED);
//							BUTTON_SetSkinFlexProps(btSkin[t90_set.sys.nightmode]+1,BUTTON_SKINFLEX_PI_FOCUSSED);
							//BUTTON_SetSkin(addbutton,BUTTON_DrawSkinFlex);
		     //BUTTON_SetFocusColor(addbutton,subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
		     //BUTTON_SetBkColor(addbutton,BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
		     //BUTTON_SetFont(addbutton,&GUI_Font_T90_30);
       //BUTTON_SetText(addbutton,"添加船队船只");
		     WM_SetCallback(addbutton,&addBtCallback);
		
							delButton[0] = BUTTON_CreateEx(305,215,97,30,pMsg->hWin, WM_CF_SHOW,0,GUI_ID_BUTTON1);
		     BUTTON_SetFont(delButton[0],&GUI_Font_T90_24);
		     BUTTON_SetTextColor(delButton[0],BUTTON_CI_UNPRESSED,GUI_WHITE);
		     BUTTON_SetBkColor(delButton[0],BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
		     BUTTON_SetFocusColor(delButton[0],subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
		     WIDGET_SetEffect(delButton[0],&WIDGET_Effect_None);
		     WM_SetCallback(delButton[0],&delBtCallback);
		
							delButton[1] = BUTTON_CreateEx(305,250,97,30,pMsg->hWin, WM_CF_SHOW,0,GUI_ID_BUTTON2);
		     BUTTON_SetFont(delButton[1],&GUI_Font_T90_24);
		     BUTTON_SetTextColor(delButton[1],BUTTON_CI_UNPRESSED,GUI_WHITE);
		     BUTTON_SetBkColor(delButton[1],BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
		     BUTTON_SetFocusColor(delButton[1],subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
		     WIDGET_SetEffect(delButton[1],&WIDGET_Effect_None);
							WM_SetCallback(delButton[1],&delBtCallback);
							
							delButton[2] = BUTTON_CreateEx(305,285,97,30,pMsg->hWin, WM_CF_SHOW,0,GUI_ID_BUTTON3);
		     BUTTON_SetFont(delButton[2],&GUI_Font_T90_24);
		     BUTTON_SetTextColor(delButton[2],BUTTON_CI_UNPRESSED,GUI_WHITE);
		     BUTTON_SetBkColor(delButton[2],BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
		     BUTTON_SetFocusColor(delButton[2],subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
		     WIDGET_SetEffect(delButton[2],&WIDGET_Effect_None);
							WM_SetCallback(delButton[2],&delBtCallback);
							
							delButton[3] = BUTTON_CreateEx(305,320,97,30,pMsg->hWin, WM_CF_SHOW,0,GUI_ID_BUTTON4);
		     BUTTON_SetFont(delButton[3],&GUI_Font_T90_24);
		     BUTTON_SetTextColor(delButton[3],BUTTON_CI_UNPRESSED,GUI_WHITE);
		     BUTTON_SetBkColor(delButton[3],BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
		     BUTTON_SetFocusColor(delButton[3],subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
							WIDGET_SetEffect(delButton[3],&WIDGET_Effect_None);
							WM_SetCallback(delButton[3],&delBtCallback);
							
							delButton[4] = BUTTON_CreateEx(305,355,97,30,pMsg->hWin, WM_CF_SHOW,0,GUI_ID_BUTTON5);
		     BUTTON_SetFont(delButton[4],&GUI_Font_T90_24);
		     BUTTON_SetTextColor(delButton[4],BUTTON_CI_UNPRESSED,GUI_WHITE);
		     BUTTON_SetBkColor(delButton[4],BUTTON_CI_UNPRESSED,subMenuColors[t90_set.sys.nightmode].btBkColor);
		     BUTTON_SetFocusColor(delButton[4],subMenuColors[t90_set.sys.nightmode].btFocusBkColor);
							WIDGET_SetEffect(delButton[4],&WIDGET_Effect_None);
							WM_SetCallback(delButton[4],&delBtCallback);
	      break;		
		
		case WM_PAINT:
							GUI_SetBkColor(subMenuColors[t90_set.sys.nightmode].bkColor);
			    GUI_Clear();
		
		     GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		     GUI_SetColor(subMenuColors[t90_set.sys.nightmode].btTextColor);
		     GUI_SetFont(&GUI_Font_T90_30);
							GUI_DispStringAt("辅船作业船九位码",40,56);
		
		     GUI_SetColor(subMenuColors[t90_set.sys.nightmode].btBkColor);
		     GUI_FillRectEx(&titlebk);
		     GUI_FillRectEx(&Head);
		     GUI_FillRectEx(&Operat);
							GUI_FillRectEx(&note);
		     for(i=0;i<5;i++)
								GUI_FillRectEx(&MMSIList[i]);
		
		     GUI_SetColor(GUI_WHITE);
		     GUI_SetFont(&GUI_Font_T90_24);
		     GUI_DispStringInRectWrap("已屏蔽船只列表",&title,GUI_TA_VCENTER|GUI_TA_HCENTER,GUI_WRAPMODE_WORD);
		     GUI_DispStringInRect("九位码",&Head,GUI_TA_VCENTER|GUI_TA_HCENTER);
		     GUI_DispStringInRect("操作",&Operat,GUI_TA_VCENTER|GUI_TA_HCENTER);
							GUI_DispStringInRectWrap("船只添加到“我的船队”后,不触发闯入报警.",&note,GUI_TA_VCENTER,GUI_WRAPMODE_WORD);
		
		     GUI_SetFont(&GUI_Font_T90_30);
		     for(i=0;i<MonitShipNum;i++)
		     {
								GUI_DispDecAt(monitMMSI[i],MMSIList[i].x0+50,MMSIList[i].y0+3,9);
								BUTTON_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON1+i),"删除");
							}
							
			    break;
							
//		case WM_KEY:
//			    switch( ((WM_KEY_INFO*)pMsg->Data.p)->Key ){	
//								case GUI_KEY_BACKSPACE:
//									    WM_SetFocus(WM_GetDialogItem(mainMenuDlg,GUI_ID_BUTTON3));
//									    break;
//								case GUI_KEY_MENU:
//									    WM_BringToTop(mainShipWin);
//									    WM_SetFocus(mainShipWin);
//									    break;
//								//
//								// add key response
//								//
//								case GUI_KEY_SOUNDOFF:
//									    break;
//								
//								case GUI_KEY_PWM_INC:
//									    break;
//								
//								case GUI_KEY_MORIGHT:
//									    break;
//							}
		
		case USER_MSG_SKIN:
			    pColor = &subMenuColors[pMsg->Data.v];
       for(i=0;i<5;i++)
		     {
								BUTTON_SetFocusColor(delButton[i],pColor->btBkColor);
		      BUTTON_SetBkColor(delButton[i],BUTTON_CI_UNPRESSED,pColor->btBkColor);
							}
//		       pSkin = btSkin[pMsg->Data.v];
			    break;
			
		case WM_SET_FOCUS:
			    if(pMsg->Data.v==1)
							{
								WM_SetFocus(edit);
							}
				 		break;
							
		case WM_NOTIFY_PARENT:
			    Id = WM_GetId(pMsg->hWinSrc); 
       NCode = pMsg->Data.v; 
		     switch(Id)
							{
								case GUI_ID_BUTTON1:
													if(NCode==WM_NOTIFICATION_CLICKED)
													{
														delMMSI = 1;
													}
									    break;
								
								case GUI_ID_BUTTON2:
									    if(NCode==WM_NOTIFICATION_CLICKED)
													{
														delMMSI = 2;
													}
									    break;
								
								case GUI_ID_BUTTON3:
									    if(NCode==WM_NOTIFICATION_CLICKED)
													{
														delMMSI = 3;
													}
									    break;
								
								case GUI_ID_BUTTON4:
									    if(NCode==WM_NOTIFICATION_CLICKED)
													{
														delMMSI = 4;
													}
									    break;
								
								case GUI_ID_BUTTON5:
									    if(NCode==WM_NOTIFICATION_CLICKED)
													{
														delMMSI = 5;
													}
									    break;
							}
			    break;
							
		case USER_MSG_REPLY:
			    switch(myMsgId){
								case MONITMMSI_FULL:
													if(pMsg->Data.v==REPLY_OK)
													{
														WM_SetFocus(addbutton);
													}
													else if(pMsg->Data.v==REPLY_CANCEL)
													{
														WM_SetFocus(addbutton);
													}
									    break;
								
								case MONITMMSI_DEL:
													if(pMsg->Data.v==REPLY_OK)
													{
														DelMMSI(delMMSI);
														if(MonitShipNum>0)
														{
															if(delMMSI<MonitShipNum)
																WM_SetFocus(delButton[delMMSI-1]);
															else
																WM_SetFocus(delButton[MonitShipNum-1]);
														}
														else
														{
															WM_SetFocus(addbutton);
														}
                                          t90_set.shipout.numShip = 0;
                                          for(i = 0; i < MonitShipNum; i++)
                                          {
                                             t90_set.shipout.MMSI[i] = monitMMSI[i];
                                          }
                                          t90_set.shipout.numShip = MonitShipNum;
                                          T90_Store();                                          
													}
													else if(pMsg->Data.v==REPLY_CANCEL)
													{
														WM_SetFocus(delButton[delMMSI-1]);
													}
									    break;
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
WM_HWIN WIN_FleetCreate(void){
   WM_HWIN handle;
   handle  = WM_CreateWindow(FLEET_X, FLEET_Y, FLEET_WIDTH, FLEET_HEIGHT, WM_CF_SHOW, &FleetWinCallback , 0);
   return handle;
}
