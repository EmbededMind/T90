#ifndef _HSD_DIMENSION_EX_H

#define _HSD_DIMENSION_EX_H

#include "WM.h"
#include "WIDGET.h"

#define HSD_DIMENSION_EX_H2P(h)  (HSD_DIMENSION_EX_Obj*) GUI_ALLOC_h2p(h)

#define HSD_DIMENSION_EX_CI_UNFOCUS   0x01
#define HSD_DIMENSION_EX_CI_FOCUS     0x02


typedef WM_HMEM  HSD_DIMENSION_EX_Handle;


void HSD_DIMENSION_EX_Callback(WM_MESSAGE* pMsg);

HSD_DIMENSION_EX_Handle HSD_DIMENSION_EX_CreateEx(int x0, int y0, int xSize, int ySize,WM_HWIN hParent,
                                                  int WinFlags, int ExFlags, int Id);

HSD_DIMENSION_EX_Handle  HSD_DIMENSION_EX_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,
                                                        int x0, int y0, WM_CALLBACK* cb);
                                                        
                                                        
void HSD_DIMENSION_EX_SetArrowLineColor(HSD_DIMENSION_EX_Handle hObj,  I16 flag, GUI_COLOR color);
void HSD_DIMENSION_EX_SetBkColor(HSD_DIMENSION_EX_Handle hObj, I16 flag, GUI_COLOR color);
void HSD_DIMENSION_EX_SetValColor(HSD_DIMENSION_EX_Handle hObj, I16 flag, GUI_COLOR color);
void HSD_DIMENSION_EX_SetArrows(HSD_DIMENSION_EX_Handle hObj, GUI_POINT* paPoint);
void HSD_DIMENSION_EX_SetValText(HSD_DIMENSION_EX_Handle hObj, const char* s);
void HSD_DIMENSION_EX_SetUnitText(HSD_DIMENSION_EX_Handle hObj, const char* s);
void HSD_DIMENSION_EX_SetFont(HSD_DIMENSION_EX_Handle hObj, const GUI_FONT GUI_UNI_PTR* pFont);
#endif

