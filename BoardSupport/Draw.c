#include "Draw.h"

static void DrawNetOne(int netx,int nety,int porp)
{
	GUI_DrawLine(netx-porp/20 , nety+porp/20,netx , nety-porp/20);
	GUI_DrawLine(netx , nety-porp/20,netx+porp/20 , nety+porp/20);
	GUI_DrawLine(netx+porp/20 , nety+porp/20,netx-porp/20 , nety+porp/20);

}

void DrawmShip(int mShipx,int mShipy,int prop)
{
	GUI_DrawLine(mShipx-prop/13 , mShipy,mShipx , mShipy-prop/10);
	GUI_DrawLine(mShipx , mShipy-prop/10,mShipx+prop/13 , mShipy );
	GUI_DrawLine(mShipx+prop/13 , mShipy ,mShipx+prop/20,mShipy+prop/5);
	GUI_DrawLine(mShipx+prop/20,mShipy+prop/5,mShipx-prop/20 , mShipy+prop/5);
	GUI_DrawLine(mShipx-prop/20 , mShipy+prop/5,mShipx-prop/13, mShipy);
}


void DrawArrow(int x,int y,int l1,int l2,int l3)
{
	GUI_DrawLine(x-l2 , y,x ,	   y-l1);
	GUI_DrawLine(x ,	   y-l1,x+l2 , y );
	GUI_DrawLine(x+l2 , y ,x ,       y+l3);
	GUI_DrawLine(x ,       y+l3,x-l2 , y);
	
}
void DrawNet(int basex,int basey ,int l1,int l2,int l3,int porp)
{
	DrawNetOne(basex-l2,basey,porp);
	DrawNetOne(basex+l2,basey,porp);
	DrawNetOne(basex,basey+l3,porp);

}

//void DrawCursor(int x_to_mother, int y_to_mother, GUI_COLOR color)
//{
//	GUI_SetColor(color);
//	GUI_DrawPoint(x_to_mother, y_to_mother);
//	GUI_DrawLine(
//}
	
	
