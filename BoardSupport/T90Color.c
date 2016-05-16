#include "T90Color.h"



const HomeColor homeColors[2] = {
//  bkColor       bbsTopColor   bbsBottonColor  textColor       numColor	         bullyColor    forgbullyColor
	{ 0x00c3e0e9,   0x00e5f4f7,   0x00819298,     0x002f2f2f,     FOCUS_COLOR_DAY,   GUI_RED,      GUI_RED},
	{ GUI_DARKGRAY, GUI_GRAY,     GUI_DARKGRAY,   GUI_LIGHTGRAY,  FOCUS_COLOR_NIGHT, GUI_RED,      GUI_RED}
};
		

const MenuColor mainMenuColors[2] = {
//  bkColor     headTopColor  headBottomColor  headTextColor  btBkColor   btTextColor    btFocusBkColor       btPrevFocusBkColor  btFocusTextColor
	{ 0x00c3e0e9, 0x00404448,   0x0025292c,      0x00eaeaea,    0x009baeb7, 0x002f2f2f,    FOCUS_COLOR_DAY,     0x007c8b92,         0x00eaeaea},
	{ GUI_BLACK,  GUI_GRAY,     GUI_GRAY,        GUI_LIGHTGRAY, GUI_GRAY,   GUI_LIGHTGRAY, FOCUS_COLOR_NIGHT,   GUI_LIGHTRED,       GUI_LIGHTGRAY}
};


const MenuColor subMenuColors[2] = {
//  bkColor     headTopColor  headBottomColor  headTextColor  btBkColor   btTextColor    btFocusBkColor       btPrevFocusBkColor  btFocusTextColor
	{ 0x00c3e0e9, 0x00404448,   0x0025292c,      0x00eaeaea,    0x008b9da5, 0x002f2f2f,    FOCUS_COLOR_DAY,     0x007c8b92,         0x00eaeaea},
	{ GUI_BLACK,  GUI_GRAY,     GUI_GRAY,        GUI_LIGHTGRAY, GUI_GRAY,   GUI_LIGHTGRAY, FOCUS_COLOR_NIGHT,   GUI_LIGHTRED,       GUI_LIGHTGRAY}
};


const SetWinColor setWinColors[2] = {
//  bkColor     textColor      focusBkColor        focusTextColor lineColor      arrwoLineColor  focusArrowLineColor
	{ 0x00c3e0e9, 0x002f2f2f,    FOCUS_COLOR_DAY, 	 0x00c3e0e9,    0x007c7c7c,    0x00a6a6a6,     0x002f2f2f},
	{ GUI_BLACK,  GUI_LIGHTGRAY, FOCUS_COLOR_NIGHT,  GUI_LIGHTGRAY, GUI_LIGHTGRAY, GUI_GRAY,       GUI_LIGHTGRAY}
};


const SetDlgColor setDlgColors[2] = {
//  bkColor     textColor      slotColor   sliderColor focusSlotColor  focusSliderColor
	{ 0x00c3e0e9, 0x002f2f2f,    GUI_GRAY,   0x0006b940, 0x002f2f2f,     FOCUS_COLOR_DAY},
	{ GUI_BLACK,  GUI_LIGHTGRAY, GUI_GRAY,	 GUI_GREEN,	 GUI_LIGHTGRAY,  FOCUS_COLOR_NIGHT}
};


const ConfirmWinColor confirmWinColors[2]  = {
//  bkTopColor  bkBottomColor  textColor   btBkColor    btFocusBkColor     btTextColor  btFocusTextColor
  { 0x00c8c8c8, 0x00c8c8c8,    0x001d1d1d, 0x00595959,  FOCUS_COLOR_DAY,   0xeaeaea,    0x00eaeaea},                               
  { 0x00c8c8c8, 0x00c8c8c8,    0x001d1d1d, 0x00595959,  FOCUS_COLOR_NIGHT, 0xeaeaea,    0x00eaeaea}
} ;



