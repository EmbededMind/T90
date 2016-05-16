#ifndef   _T90_COLOR_H
#define   _T90_COLOR_H

#include "T90.h"

#define   FOCUS_COLOR_DAY      0x009c6f00
#define   FOCUS_COLOR_NIGHT    0x00005dff


typedef struct HomeColor
{
	GUI_COLOR bkColor;
	GUI_COLOR bbsTopColor;
	GUI_COLOR bbsBottomColor;
	GUI_COLOR textColor;
	GUI_COLOR numColor;
	GUI_COLOR bullyColor;
	GUI_COLOR forgBullyColor;
} HomeColor;

typedef struct MenuColor
{
	GUI_COLOR bkColor;
	GUI_COLOR headTopColor;
	GUI_COLOR headBottomColor;
	GUI_COLOR headTextColor;
	GUI_COLOR btBkColor;
	GUI_COLOR btTextColor;
	GUI_COLOR btFocusBkColor;
	GUI_COLOR btPrevFocusBkColor;
	GUI_COLOR btFocusTextColor;
} MenuColor;


typedef struct SetWinColor
{
	GUI_COLOR bkColor;
	GUI_COLOR textColor;
	GUI_COLOR focusBkColor;
	GUI_COLOR focusTextColor;	
	GUI_COLOR lineColor;
	GUI_COLOR arrowLineColor;
	GUI_COLOR focusArrowLineColor;
} SetWinColor;


typedef struct SetDlgColor
{
	GUI_COLOR bkColor;
	GUI_COLOR textColor;
	GUI_COLOR slotColor;
	GUI_COLOR sliderColor;
	GUI_COLOR focusSlotColor;
	GUI_COLOR focusSliderColor;
} SetDlgColor;


typedef struct ConfirmWinColor
{
	GUI_COLOR bkTopColor;
	GUI_COLOR bkBottomColor;
	GUI_COLOR textColor;
	GUI_COLOR btBkColor;
	GUI_COLOR btFocusBkColor;
	GUI_COLOR btTextColor;
	GUI_COLOR btFocusTextColor;
} ConfirmWinColor;

extern const HomeColor homeColors[2];

extern const MenuColor mainMenuColors[2];
extern const MenuColor subMenuColors[2];

extern const SetWinColor setWinColors[2];
extern const SetDlgColor setDlgColors[2];

extern const ConfirmWinColor confirmWinColors[2];

#endif


