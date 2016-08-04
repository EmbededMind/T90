/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.20 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  NXP Semiconductors USA, Inc.  whose
registered  office  is  situated  at 411 E. Plumeria Drive, San  Jose,
CA 95134, USA  solely for  the  purposes  of  creating  libraries  for
NXPs M0, M3/M4 and  ARM7/9 processor-based  devices,  sublicensed  and
distributed under the terms and conditions of the NXP End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Start.c
Purpose     : GUIDEMO initialization
----------------------------------------------------------------------
*/

#include "GUIDEMO.h"

void _DemoHardkey(void);

static void _cbBkWindow(WM_MESSAGE* pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			{
// 				int x, y, w, h;
// 				
 				GUI_SetBkColor(GUI_BLACK);
// 				GUI_SetColor(MAIN_TEXTCOLOR);
// 				GUI_SetFont(MAIN_FONT);
 				GUI_Clear();
// 				x = MAIN_LOGO_OFFSET_X + MAIN_BORDER;
// 				y = MAIN_LOGO_OFFSET_Y + ((MAIN_TITLE_HEIGHT - MAIN_LOGO_BITMAP->YSize) >> 1);
// 				GUI_DrawBitmap(MAIN_LOGO_BITMAP, x, y);
// 				x = MAIN_BORDER;
// 				y = MAIN_TITLE_HEIGHT;
// 				w = LCD_GetXSize() - (MAIN_BORDER * 2);
// 				h = LCD_GetYSize()  - (MAIN_BORDER + MAIN_TITLE_HEIGHT);
// 				_DrawDownRect(FRAME_EFFECT, x, y, x + w - 1, y + h - 1);
			} 
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}





/*************************** End of file ****************************/

