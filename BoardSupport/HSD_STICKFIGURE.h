#ifndef _HSD_STICK_FIGURE_H
#define _HSD_STICK_FIGURE_H

#include "WM.h"
#include "WIDGET.h"

#define HSD_STICKFIGURE_CI_UNFOCUS  0x00
#define HSD_STICKFIGURE_CI_FOCUS    0x01


#define HSD_STICKFIGURE_H2P(h) (HSD_STICKFIGURE_Obj*) GUI_ALLOC_h2p(h)

typedef WM_HMEM HSD_STICKFIGURE_Handle;



HSD_STICKFIGURE_Handle HSD_STICKFIGURE_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                                                int WinFlags, int ExFlags, int Id);
HSD_STICKFIGURE_Handle HSD_STICKFIGURE_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,
                                                      int x0, int y0, WM_CALLBACK* cb) ;
                                                      
void HSD_STICKFIGURE_Callback(WM_MESSAGE* pMsg);                                                      
void HSD_STICKFIGURE_SetPenColor(HSD_STICKFIGURE_Handle hObj,  I16 flag, GUI_COLOR color);                                                      
#endif


