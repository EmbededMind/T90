#include "T90Color.h"



const HomeColor homeColors[2] = {
//  bkColor       bbsTopColor   bbsBottonColor  textColor       numColor	         bullyColor    forgbullyColor
	{ 0x00c3e0e9,   0x00e5f4f7,   0x00819298,     0x002f2f2f,     FOCUS_COLOR_DAY,   GUI_RED,      GUI_RED},
	{ 0x001A1A1A,   0x003F3F3F,     0x00292929,   0x00E1E1E1,  FOCUS_COLOR_NIGHT,    GUI_RED,      GUI_RED}
};
		

const MenuColor mainMenuColors[2] = {
//  bkColor     headTopColor  headBottomColor  headTextColor  btBkColor       btTextColor    btFocusBkColor       btPrevFocusBkColor  btFocusTextColor
	{ 0x00c3e0e9, 0x00646c71,   0x00343c41,      0x00eaeaea,    0x009baeb7,   0x002f2f2f,    FOCUS_COLOR_DAY,     0x007c8b92,         0x00eaeaea},
	{ GUI_BLACK,  0x00686c6f,   0x00383c3f,     0x00EAEAEA,      0x00202123, GUI_LIGHTGRAY, FOCUS_COLOR_NIGHT,   0x007c8b92,       GUI_LIGHTGRAY}
};


const MenuColor subMenuColors[2] = {
//  bkColor     headTopColor  headBottomColor  headTextColor  btBkColor   btTextColor    btFocusBkColor       btPrevFocusBkColor  btFocusTextColor
	{ 0x00c3e0e9, 0x00646c71,   0x00343c41,      0x00eaeaea,    0x008b9da5, 0x002f2f2f,    FOCUS_COLOR_DAY,     0x007c8b92,         0x00eaeaea},
	{ GUI_BLACK,  0x00686c6f,   0x00383c3f,      GUI_LIGHTGRAY, 0x00303030,  GUI_LIGHTGRAY, FOCUS_COLOR_NIGHT,   0x007c8b92,       GUI_LIGHTGRAY}
};


const SetWinColor setWinColors[2] = {
//  bkColor     textColor      focusBkColor        focusTextColor lineColor      arrwoLineColor  focusArrowLineColor
	{ 0x00c3e0e9, 0x002f2f2f,    FOCUS_COLOR_DAY,  0x00c3e0e9,    0x007c7c7c,    0x00a6a6a6,     0x002f2f2f},
	{ 0x00000004,  0x00E4E4E4, FOCUS_COLOR_NIGHT,  0x00E4E4E4,    0x00EAEAEA,    GUI_GRAY,       0x00EAEAEA}
};


const SetDlgColor setDlgColors[2] = {
//  bkColor     textColor      slotColor   sliderColor focusSlotColor  focusSliderColor
	{ 0x00c3e0e9, 0x002f2f2f,    GUI_GRAY,   0x0006b940, 0x002f2f2f,     FOCUS_COLOR_DAY},
	{ 0x00000004,  GUI_LIGHTGRAY, GUI_GRAY,	 0x0006b940, GUI_LIGHTGRAY,  FOCUS_COLOR_NIGHT}
};


const ConfirmWinColor confirmWinColors[2]  = {
//  bkTopColor  bkBottomColor  textColor   btBkColor    btFocusBkColor     btTextColor  btFocusTextColor
  { 0x00c8c8c8, 0x00c8c8c8,    0x001d1d1d, 0x00595959,  FOCUS_COLOR_DAY,   0xeaeaea,    0x00eaeaea},                               
  { 0x00c8c8c8, 0x00c8c8c8,    0x001d1d1d, 0x00595959,  FOCUS_COLOR_NIGHT, 0xeaeaea,    0x00eaeaea}
} ;



