#include "plot.h"
#include "GUI.h"
#include "T90.h"
#include "math.h"
#include "bully.h"
#include "snap.h"


Point motherShipPixel;

int scale = 100;  //100像素代表的距离


static void FigureMotherShipPixel(Point center, int flag);
static void FigureScale(int flag);

void DrawStubs(int flag)
{
	int alarmLineZoom;
	StubRefresh();
	FigureScale(flag);
	DrawShipFamily(flag);

	alarmLineZoom = flag? 1: 2000/scale;
	DrawAlarmLine(alarmLineZoom);
}

void DrawShipFamily(int flag)
{
	int i;
	Point pixelTmp[STUB_NUM];
	
	GUI_SetLineStyle(GUI_LS_SOLID);
	
	//mothership
	GUI_DrawLine(motherShipPixel.x-(7-scale/500), motherShipPixel.y,                motherShipPixel.x,               motherShipPixel.y-(10-scale/500));  
	GUI_DrawLine(motherShipPixel.x,               motherShipPixel.y-(10-scale/500), motherShipPixel.x+(7-scale/500), motherShipPixel.y );
	GUI_DrawLine(motherShipPixel.x+(7-scale/500), motherShipPixel.y,                motherShipPixel.x+(5-scale/500), motherShipPixel.y+(20-scale/500));
	GUI_DrawLine(motherShipPixel.x+(5-scale/500), motherShipPixel.y+(20-scale/500), motherShipPixel.x-(5-scale/500), motherShipPixel.y+(20-scale/500));
	GUI_DrawLine(motherShipPixel.x-(5-scale/500), motherShipPixel.y+(20-scale/500), motherShipPixel.x-(7-scale/500), motherShipPixel.y);
	
	GUI_SetFont(GUI_FONT_13B_ASCII);
	
	for(i = 0; i < STUB_NUM; i++)
	{
		pixelTmp[i] = GetItemPixel(stubs[i].basePoint);
		
		if(i)
		{
			GUI_DrawLine(pixelTmp[i].x-(5-scale/500), pixelTmp[i].y+(5-scale/500), pixelTmp[i].x,               pixelTmp[i].y-(5-scale/500));  //三角标
			GUI_DrawLine(pixelTmp[i].x,               pixelTmp[i].y-(5-scale/500), pixelTmp[i].x+(5-scale/500), pixelTmp[i].y+(5-scale/500));
			GUI_DrawLine(pixelTmp[i].x+(5-scale/500), pixelTmp[i].y+(5-scale/500), pixelTmp[i].x-(5-scale/500), pixelTmp[i].y+(5-scale/500));
			if(!flag)
			{
				sprintf(pStrBuf, "%02d", i);
				GUI_DispStringAt(pStrBuf, pixelTmp[i].x - 6, pixelTmp[i].y + 5);
				
				GUI_DrawVLine(motherShipPixel.x-50, motherShipPixel.y, -t90_set.dst.dst3*TO_PIXEL + motherShipPixel.y);
				
			}
		}
	}
	
	GUI_SetLineStyle(GUI_LS_DOT);
	for(i = 0; i < STUB_NUM; i++)
	{
		GUI_DrawLine(pixelTmp[i].x, pixelTmp[i].y, pixelTmp[i+1 == STUB_NUM? 0: i+1].x, pixelTmp[i+1 == STUB_NUM? 0: i+1].y);    //虚线
	}
}

void DrawAlarmLine(int zoom)   //zoom：缩放比例
{
	int i;
	for(i = 0; i < STUB_NUM; i++)
	{
		int j = (i+1 == STUB_NUM)? 0: i+1;
		
		if(stubs[i].tang2.angle > 0 && stubs[i].tang1.angle < 0)
		{
			stubs[i].tang1.angle += 360;
		}
		GUI_SetLineStyle(GUI_LS_SOLID);
		GUI_DrawArc(stubs[i].basePoint.x*TO_PIXEL + motherShipPixel.x, -stubs[i].basePoint.y*TO_PIXEL + motherShipPixel.y, 
								(t90_set.alarm.invd_dst*TO_PIXEL+1)/zoom, (t90_set.alarm.invd_dst*TO_PIXEL+1)/zoom, 
								stubs[i].tang2.angle, stubs[i].tang1.angle);
		GUI_DrawLine(((stubs[i].tang1.point.x-stubs[i].basePoint.x)/zoom+stubs[i].basePoint.x)*TO_PIXEL + motherShipPixel.x, 
								-((stubs[i].tang1.point.y-stubs[i].basePoint.y)/zoom+stubs[i].basePoint.y)*TO_PIXEL + motherShipPixel.y,
								 ((stubs[j].tang2.point.x-stubs[j].basePoint.x)/zoom+stubs[j].basePoint.x)*TO_PIXEL + motherShipPixel.x, 
								-((stubs[j].tang2.point.y-stubs[j].basePoint.y)/zoom+stubs[j].basePoint.y)*TO_PIXEL + motherShipPixel.y);
//		GUI_DrawLine(stubs[i].tang1.point.x*TO_PIXEL + motherShipPixel.x,
//								-stubs[i].tang1.point.y*TO_PIXEL + motherShipPixel.y,
//								 stubs[i].tang2.point.x*TO_PIXEL + motherShipPixel.x,
//								-stubs[i].tang2.point.y*TO_PIXEL + motherShipPixel.y);
 					
	}
}

void DrawCursor(Point pixel, int flag)
{
	int start_x, start_y;
	start_x = pixel.x + 10;
	start_y = pixel.y + 10;
//	GUI_DrawPoint(pixel.x, pixel.y);
//	GUI_DrawHLine(pixel.y, pixel.x - 10, pixel.x - 2);
//	GUI_DrawHLine(pixel.y, pixel.x + 2,  pixel.x + 10);
//	GUI_DrawVLine(pixel.x, pixel.y - 10, pixel.y - 2);
//	GUI_DrawVLine(pixel.x, pixel.y + 2,  pixel.y + 10);
	
	if(flag)
	{
		GUI_SetFont(GUI_FONT_13_1);
		sprintf(pStrBuf,"name:%s",pSnapLink->Boat.name);
		GUI_DispStringAt(pStrBuf, start_x, start_y);
		
		GUI_DispStringAt("N", start_x, start_y+GUI_GetFontSizeY());
		lltostr(pSnapLink->Boat.latitude, pStrBuf);
		GUI_DispStringAt(pStrBuf, start_x+13, start_y+GUI_GetFontSizeY());
		
		GUI_DispStringAt("E", start_x, start_y+GUI_GetFontSizeY()*2);
		lltostr(pSnapLink->Boat.longitude, pStrBuf);
		GUI_DispStringAt(pStrBuf, start_x+13, start_y+GUI_GetFontSizeY()*2);

		GUI_DispStringAt("SOG:", start_x, start_y+GUI_GetFontSizeY()*3);
		sprintf(pStrBuf, "%2d.%d", pSnapLink->Boat.SOG/10, pSnapLink->Boat.SOG%10);
		GUI_DispStringAt(pStrBuf, start_x+35, start_y+ GUI_GetFontSizeY()*3);
		
		GUI_DispStringAt("COG:", start_x, start_y+GUI_GetFontSizeY()*4);
		sprintf(pStrBuf, "%3d", pSnapLink->Boat.COG/10);
		pStrBuf[3]  = 194;
		pStrBuf[4]  = 176;
		pStrBuf[5]  = '\0';
		GUI_DispStringAt(pStrBuf, pixel.x + 45, start_y+GUI_GetFontSizeY()*4);
	}
	
	GUI_CURSOR_SetPosition(pixel.x, pixel.y);
}

Point GetItemPixel(Point itemPoint)
{
	Point itemPixel;
	itemPixel.x =  itemPoint.x*TO_PIXEL + motherShipPixel.x;
	itemPixel.y = -itemPoint.y*TO_PIXEL + motherShipPixel.y;
	return itemPixel;
}


static void FigureMotherShipPixel(Point center, int flag)
{
	Point screenCenter;
	if(!flag)
	{
		screenCenter.x = 200;
		screenCenter.y = SCREEN_VCENTER;
	}
	else
	{
		screenCenter.x = SCREEN_HCENTER;
		screenCenter.y = (45+SCREEN_HEIGHT)/2;
	}
	motherShipPixel.x = screenCenter.x - center.x*TO_PIXEL;
	motherShipPixel.y = screenCenter.y + center.y*TO_PIXEL;
}
	
static void FigureScale(int flag)
{
	int i;
	int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
	Point center;
	BULY_BERTH *pBully = pBulyHeader;
	
	for(i = 0; i < STUB_NUM; i++)
	{
		if(stubs[i].basePoint.x < x_min)
		{
			x_min = stubs[i].basePoint.x;
		}
		else if(stubs[i].basePoint.x > x_max)
		{
			x_max = stubs[i].basePoint.x;
		}
		if(stubs[i].basePoint.y < y_min)
		{
			y_min = stubs[i].basePoint.y;
		}
		else if(stubs[i].basePoint.y > y_max)
		{
			y_max = stubs[i].basePoint.y;
		}
	}
	if(!flag)
	{
		scale = (y_max-y_min)*100/194;
		if(scale < (x_max-x_min)*100/120)
		{
			scale = (x_max-x_min)*100/120;
		}
	}
	else
	{
		for(i = 0; i < N_boat; i++)
		{
			if(SimpBerthes[i].pBerth->isInvader)
			{
				if(SimpBerthes[i].pBerth->x_to_cross < x_min)
				{
					x_min = SimpBerthes[i].pBerth->x_to_cross;
				}
				else if(SimpBerthes[i].pBerth->x_to_cross > x_max)
				{
					x_max = SimpBerthes[i].pBerth->x_to_cross;
				}
				if(SimpBerthes[i].pBerth->y_to_cross < y_min)
				{
					y_min = SimpBerthes[i].pBerth->y_to_cross;
				}
				else if(SimpBerthes[i].pBerth->y_to_cross > y_max)
				{
					y_max = SimpBerthes[i].pBerth->y_to_cross;
				}
			}
		}
//		if(monitorState == ON)
//		{
			while(pBully)
			{
				if(pBully->pBoatLink->x_to_cross < x_min)
				{
					x_min = pBully->pBoatLink->x_to_cross;
				}
				else if(pBully->pBoatLink->x_to_cross > x_max)
				{
					x_max = pBully->pBoatLink->x_to_cross;
				}
				if(pBully->pBoatLink->y_to_cross < y_min)
				{
					y_min = pBully->pBoatLink->y_to_cross;
				}
				else if(pBully->pBoatLink->y_to_cross > y_max)
				{
					y_max = pBully->pBoatLink->y_to_cross;
				}
				pBully = pBully->pNext;
			}
//		}
		scale = (y_max-y_min)*100/320;
		if(scale < (x_max-x_min)*100/700)
		{
			scale = (x_max-x_min)*100/700;
		}
	}
	center.x = (x_max+x_min)/2;
	center.y = (y_max+y_min)/2;
	FigureMotherShipPixel(center, flag);
}

void DrawOtherShip(Point pixel, int course)  //course:航向（角度制）
{
	double _cos = cos(course*ANGLE_TO_RAD);
	double _sin = sin(course*ANGLE_TO_RAD);
	
	GUI_SetLineStyle(GUI_LS_SOLID);
     
//	GUI_DrawLine(pixel.x-7*_cos-10*_sin, pixel.y-7*_sin+10*_cos, pixel.x+10*_sin       , pixel.y-10*_cos);
//	GUI_DrawLine(pixel.x+10*_sin,        pixel.y-10*_cos,        pixel.x+7*_cos-10*_sin, pixel.y+7*_sin+10*_cos);
//	GUI_DrawLine(pixel.x+7*_cos-10*_sin, pixel.y+7*_sin+10*_cos, pixel.x-7*_cos-10*_sin, pixel.y-7*_sin+10*_cos );
	
	GUI_DrawLine(pixel.x-7*_cos-7*_sin, pixel.y-7*_sin+7*_cos, pixel.x+13*_sin       , pixel.y-13*_cos);
	GUI_DrawLine(pixel.x+13*_sin,        pixel.y-13*_cos,        pixel.x+7*_cos-7*_sin, pixel.y+7*_sin+7*_cos);
	GUI_DrawLine(pixel.x+7*_cos-7*_sin, pixel.y+7*_sin+7*_cos, pixel.x-7*_cos-7*_sin, pixel.y-7*_sin+7*_cos );
}

void DrawAllOtherShips()
{
	int i;
	Point point;
	BULY_BERTH *pBully = pBulyHeader;
	
	GUI_SetColor(GUI_RED);
	for(i = 0; i < N_boat; i++)
	{
		if(SimpBerthes[i].pBerth->isInvader)
		{
			point.x = SimpBerthes[i].pBerth->x_to_cross;
			point.y = SimpBerthes[i].pBerth->y_to_cross;
			DrawOtherShip(GetItemPixel(point), SimpBerthes[i].pBerth->Boat.COG/10);
		}
	}
	
	GUI_SetColor(GUI_GREEN);
//	if(monitorState == ON)
//	{
		while(pBully)
		{
			point.x = pBully->pBoatLink->x_to_cross;
			point.y = pBully->pBoatLink->y_to_cross;
			DrawOtherShip(GetItemPixel(point), pBully->pBoatLink->Boat.COG/10);
			pBully = pBully->pNext;
		}
//	}
}






