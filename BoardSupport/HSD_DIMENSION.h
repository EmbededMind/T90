#ifndef _HSD_DIMENSION_H
#define _HSD_DIMENSION_H

#include "WM.h"
#include "WIDGET.h"

#define HSD_DIMENSION_H2P(h)  (HSD_DIMENSION_Obj*)GUI_ALLOC_h2p(h)

#define HSD_DIMENSION_CF_HORIZONTAL 0x01
#define HSD_DIMENSION_CF_VERTICAL   0x02


#define HSD_DIMENSION_CI_UNFOCUS   0x01
#define HSD_DIMENSION_CI_FOCUS     0x02


typedef WM_HMEM  HSD_DIMENSION_Handle;


HSD_DIMENSION_Handle  HSD_DIMENSION_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                                             int WinFlags, int ExFlags, int Id);
HSD_DIMENSION_Handle   HSD_DIMENSION_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,
                                                    int x0, int y0, WM_CALLBACK* cb) ;
void HSD_DIMENSION_Callback(WM_MESSAGE* pMsg);

void HSD_DIMENSION_SetArrowLineColor(HSD_DIMENSION_Handle hObj, I16 flag,GUI_COLOR color);

void HSD_DIMENSION_SetBkColor(HSD_DIMENSION_Handle hObj, I16 flag, GUI_COLOR color);

void HSD_DIMENSION_SetValColor(HSD_DIMENSION_Handle hObj, I16 flag, GUI_COLOR color);
void HSD_DIMENSION_SetValText(HSD_DIMENSION_Handle hObj, const char *s);
void HSD_DIMENSION_SetValFont(HSD_DIMENSION_Handle hObj, const GUI_FONT GUI_UNI_PTR* pFont);

void HSD_DIMENSION_SetUnitText(HSD_DIMENSION_Handle hObj, const char * s);

#endif


