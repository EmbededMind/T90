#ifndef _PLOT_H
#define _PLOT_H

#include "stub.h"
#include "GUI.h"
#define TO_PIXEL         100/scale
//#define TO_REALITY       scale/100

extern Point motherShipPixel;
extern int scale;


void DrawStubs(int flag);
void DrawShipFamily(int);
void DrawAlarmLine(int zooma,int zoomb);
void DrawCursor(Point pixel, int flag);
Point GetItemPixel(Point itemPoint);

void DrawInvdShip(Point pixel, int course);
void DrawBullyShip(Point pixel, int course, GUI_COLOR BullyColor);
void DrawAllOtherShips(void);
void DrawBlueTri(Point pixelTemp);

#endif













