#include "plot.h"
#include "GUI.h"
#include "T90.h"
#include "math.h"
#include "bully.h"
#include "snap.h"
#include "stub.h"
#include "transform.h"


Point motherShipPixel;

int scale = 100;  //100б̘պҭքߠk

extern const HomeColor* pColor;
extern int cursorOnStub;


static void FigureMotherShipPixel(Point center, int flag);
static void FigureScale(int flag);

//ۭԬۍѨޯП
void DrawStubs(int flag)														//flag=0ҭʾmainShipWinַԃՋگ˽û
																										//flag=1ҭʾalarmmonitorWinַԃՋگ˽
{
	int alarmLineZoom;
//	StubRefresh();
	FigureScale(flag);
	DrawShipFamily(flag);

	alarmLineZoom = flag? 1: 2000/scale;
	DrawAlarmLine(alarmLineZoom);
}

void DrawShipFamily(int flag)     //ۭԬۍвȫҪӔ̻ܰć֮ݤքlП
{
	int safetySignNum = 1;    //вȫҪѲۅ
	int ms_zoom = scale/500;
	int net_zoom = scale/300;
	Point pixelTmp1, pixelTmp2;
	StubNode *pIndex = pStubHead;
	
   GUI_POINT shipe[3];
 
	ms_zoom = ms_zoom>5 ? 5 : ms_zoom;
	net_zoom = net_zoom>5 ? 5 : net_zoom;
	
	GUI_SetLineStyle(GUI_LS_SOLID);
	GUI_SetFont(GUI_FONT_13B_ASCII);
 

                       
  shipe[0].x  = net_zoom -5;
  shipe[0].y  = 5-net_zoom;
  shipe[1].x  = 0;
  shipe[1].y  = net_zoom -5;
  shipe[2].x  = 5-net_zoom;
  shipe[2].y  = 5-net_zoom;  
	
	if(pIndex)
	{
		do
		{
			if(pIndex->pStub->type == motherStub)     //mothership
			{
				GUI_DrawLine(motherShipPixel.x-(7-ms_zoom), motherShipPixel.y,                motherShipPixel.x,               motherShipPixel.y-(10-ms_zoom));  
				GUI_DrawLine(motherShipPixel.x,               motherShipPixel.y-(10-ms_zoom), motherShipPixel.x+(7-ms_zoom), motherShipPixel.y );
				GUI_DrawLine(motherShipPixel.x+(7-ms_zoom), motherShipPixel.y,                motherShipPixel.x+(5-ms_zoom), motherShipPixel.y+(20-ms_zoom));
				GUI_DrawLine(motherShipPixel.x+(5-ms_zoom), motherShipPixel.y+(20-ms_zoom), motherShipPixel.x-(5-ms_zoom), motherShipPixel.y+(20-ms_zoom));
				GUI_DrawLine(motherShipPixel.x-(5-ms_zoom), motherShipPixel.y+(20-ms_zoom), motherShipPixel.x-(7-ms_zoom), motherShipPixel.y);
			}
			else if(pIndex->pStub->type == safetySignStub)      //ɽއҪ
			{
				pixelTmp1 = GetItemPixel(pIndex->pStub->basePoint);
//    if(&stubs[cursorOnStub] == pIndex->pStub){
//       GUI_SetColor(pColor->numColor);
//    }
    GUI_FillPolygon(shipe, 3, pixelTmp1.x, pixelTmp1.y);
//    GUI_DrawCircle(pixelTmp1.x, pixelTmp1.y, 3);
//				GUI_DrawLine(pixelTmp1.x-(5-net_zoom), pixelTmp1.y+(5-net_zoom), pixelTmp1.x,               pixelTmp1.y-(5-net_zoom));  
//				GUI_DrawLine(pixelTmp1.x,               pixelTmp1.y-(5-net_zoom), pixelTmp1.x+(5-net_zoom), pixelTmp1.y+(5-net_zoom));
//				GUI_DrawLine(pixelTmp1.x+(5-net_zoom), pixelTmp1.y+(5-net_zoom), pixelTmp1.x-(5-net_zoom), pixelTmp1.y+(5-net_zoom));
    GUI_SetColor(pColor->textColor);
				if(!flag)    //Ѳۅ
				{
					sprintf(pStrBuf, "%02d", safetySignNum);
					GUI_DispStringAt(pStrBuf, pixelTmp1.x - 6, pixelTmp1.y + 5);
					safetySignNum++;
				}
			}
			else if(pIndex->pStub->type == aidedStub)   //بԬ
			{
				pixelTmp1 = GetItemPixel(pIndex->pStub->basePoint);
				GUI_DrawLine(pixelTmp1.x-(7-ms_zoom), pixelTmp1.y,                pixelTmp1.x,               pixelTmp1.y-(10-ms_zoom));  
				GUI_DrawLine(pixelTmp1.x,               pixelTmp1.y-(10-ms_zoom), pixelTmp1.x+(7-ms_zoom), pixelTmp1.y );
				GUI_DrawLine(pixelTmp1.x+(7-ms_zoom), pixelTmp1.y,                pixelTmp1.x+(5-ms_zoom), pixelTmp1.y+(20-ms_zoom));
				GUI_DrawLine(pixelTmp1.x+(5-ms_zoom), pixelTmp1.y+(20-ms_zoom), pixelTmp1.x-(5-ms_zoom), pixelTmp1.y+(20-ms_zoom));
				GUI_DrawLine(pixelTmp1.x-(5-ms_zoom), pixelTmp1.y+(20-ms_zoom), pixelTmp1.x-(7-ms_zoom), pixelTmp1.y);
			}
			pIndex = pIndex->pNext;
		}
		while(pIndex != pStubHead);
	}
	
	if(!flag)                                  //سӠҪע
	{
		int y_min = STUB_GetMostValue(Y_MIN);
		y_min = -y_min;
		GUI_SetFont(GUI_FONT_24_ASCII);
  if(y_min != 0){
     GUI_DrawHLine(motherShipPixel.y, 34, 46);
     GUI_DrawHLine(y_min*TO_PIXEL + motherShipPixel.y, 34, 46);
     GUI_DrawVLine(40, motherShipPixel.y, y_min*TO_PIXEL/2 - 20 + motherShipPixel.y);
     GUI_DrawVLine(40, y_min*TO_PIXEL/2 + 20 + motherShipPixel.y, y_min*TO_PIXEL + motherShipPixel.y);
  }
		sprintf(pStrBuf,"%dm", rectifyNum(y_min*MILLINM_TO_M, 5));
		GUI_DispStringAt(pStrBuf, 40 - GUI_GetStringDistX(pStrBuf)/2 + 3, y_min*TO_PIXEL/2 - 12 + motherShipPixel.y);
	}
	
	GUI_SetLineStyle(GUI_LS_DOT);
	if(pIndex)                                //stubs֮ݤРlքѩП
	{
		do
		{
			pixelTmp1 = GetItemPixel(pIndex->pStub->basePoint);
			pixelTmp2 = GetItemPixel(pIndex->pNext->pStub->basePoint);
			GUI_DrawLine(pixelTmp1.x, pixelTmp1.y, pixelTmp2.x, pixelTmp2.y);
			pIndex = pIndex->pNext;
		}
		while(pIndex != pStubHead);
	}
}

void DrawAlarmLine(int zoom)   //ۭѨޯП   zoomú̵؅҈} 
{
	StubNode *pIndex = pStubHead;
	int r;
	if(pIndex)
	{
		r = sqrt(pIndex->pStub->tang1.point.x*pIndex->pStub->tang1.point.x + pIndex->pStub->tang1.point.y*pIndex->pStub->tang1.point.y);
		do
		{
			if(pIndex->pStub->tang2.angle > pIndex->pStub->tang1.angle)
			{
				pIndex->pStub->tang1.angle += 360;
			}         
			GUI_SetLineStyle(GUI_LS_SOLID);
//         printf("%d\n",pIndex->pStub->tang2.angle);
//         printf("%d\n",pIndex->pStub->tang1.angle);
         if(pIndex->pStub->tang1.angle - pIndex->pStub->tang2.angle < 180)
         {
			   GUI_DrawArc(pIndex->pStub->basePoint.x*TO_PIXEL + motherShipPixel.x, -pIndex->pStub->basePoint.y*TO_PIXEL + motherShipPixel.y, 
									(r*TO_PIXEL)/zoom, (r*TO_PIXEL)/zoom,
									pIndex->pStub->tang2.angle, pIndex->pStub->tang1.angle);
			}
          GUI_DrawLine(((pIndex->pStub->tang1.point.x-pIndex->pStub->basePoint.x)/zoom+pIndex->pStub->basePoint.x)*TO_PIXEL + motherShipPixel.x, 
									-((pIndex->pStub->tang1.point.y-pIndex->pStub->basePoint.y)/zoom+pIndex->pStub->basePoint.y)*TO_PIXEL + motherShipPixel.y,
									 ((pIndex->pNext->pStub->tang2.point.x-pIndex->pNext->pStub->basePoint.x)/zoom+pIndex->pNext->pStub->basePoint.x)*TO_PIXEL + motherShipPixel.x, 
									-((pIndex->pNext->pStub->tang2.point.y-pIndex->pNext->pStub->basePoint.y)/zoom+pIndex->pNext->pStub->basePoint.y)*TO_PIXEL + motherShipPixel.y);
//    
//         if(pIndex->pStub->tang1.angle - pIndex->pStub->tang2.angle < 180)
//         {
//            if(pIndex->pNext->pStub->tang1.angle - pIndex->pNext->pStub->tang2.angle < 180)
//            {
//                      }
//            else
//            {
//               GUI_DrawLine(((pIndex->pStub->tang1.point.x-pIndex->pStub->basePoint.x)/zoom+pIndex->pStub->basePoint.x)*TO_PIXEL + motherShipPixel.x, 
//                           -((pIndex->pStub->tang1.point.y-pIndex->pStub->basePoint.y)/zoom+pIndex->pStub->basePoint.y)*TO_PIXEL + motherShipPixel.y,
//                           ((pIndex->pNext->pStub->crossPoint.x-pIndex->pNext->pStub->basePoint.x)/zoom+pIndex->pNext->pStub->basePoint.x)*TO_PIXEL + motherShipPixel.x, 
//                           -((pIndex->pNext->pStub->crossPoint.y-pIndex->pNext->pStub->basePoint.y)/zoom+pIndex->pNext->pStub->basePoint.y)*TO_PIXEL + motherShipPixel.y);

//            }
//         }
//         else
//         {
//            if(pIndex->pNext->pStub->tang1.angle - pIndex->pNext->pStub->tang2.angle < 180)
//            {
//            }
//            else
//            {
//            
//            }
//               
//         }
            pIndex = pIndex->pNext;
		}
		while(pIndex != pStubHead);
	}
}

void DrawCursor(Point pixel, int flag)  //ДʾڢҪܰהӦքхϢ
{
	int start_x, start_y;	
	Point point;
	char strTmp[15];
	int infoWidth, infoHeight;
	start_x = pixel.x + 10;
	start_y = pixel.y + 10;

	
	if(flag)
	{	
		GUI_SetFont(GUI_FONT_24_1);
		GUI_SetTextMode(GUI_TM_NORMAL);
		
		lltostr(pSnapLink->Boat.longitude, strTmp);
		sprintf(pStrBuf, "E  %s", strTmp);
		infoWidth = GUI_GetStringDistX(pStrBuf);
		infoHeight = strlen(pSnapLink->Boat.name) ? GUI_GetFontSizeY()*4 : GUI_GetFontSizeY()*3;
		if(start_x+infoWidth > SCREEN_WIDTH)    //؀ֹսДʾхϢӬԶǁĻ׶Χ
		{
			start_x = pixel.x - infoWidth -10;
		}
		if(start_y+infoHeight > SCREEN_HEIGHT)
		{
			start_y = pixel.y - infoHeight -10;
		}
		
		start_y -= GUI_GetFontSizeY();
		if(strlen(pSnapLink->Boat.name))
		{
			sprintf(pStrBuf," %s",pSnapLink->Boat.name);
			GUI_DispStringAt(pStrBuf, start_x, start_y+=GUI_GetFontSizeY());
		}
		
		lltostr(pSnapLink->Boat.latitude, strTmp);
		sprintf(pStrBuf, " N  %s", strTmp);
		GUI_DispStringAt(pStrBuf, start_x, start_y+=GUI_GetFontSizeY());
		
		lltostr(pSnapLink->Boat.longitude, strTmp);
		sprintf(pStrBuf, " E  %s", strTmp);
		GUI_DispStringAt(pStrBuf, start_x, start_y+=GUI_GetFontSizeY());
		
		sprintf(strTmp, "%3d", pSnapLink->Boat.COG/10);
		strTmp[3]  = 194;
		strTmp[4]  = 176;
		strTmp[5]  = '\0';
		sprintf(pStrBuf, " %2d.%d    %s", pSnapLink->Boat.SOG/10, pSnapLink->Boat.SOG%10, strTmp);
		GUI_DispStringAt(pStrBuf, start_x, start_y+=GUI_GetFontSizeY());
		
		
		GUI_SetColor(homeColors[t90_set.sys.nightmode].numColor);
		point.x = pSnapLink->x_to_cross;
		point.y = pSnapLink->y_to_cross;
		if((pSnapLink->Boat.category & 0x0f) == TYPE_BULLY)
		{
			DrawBullyShip(GetItemPixel(point), (pSnapLink->Boat.COG - mothership.COG)/10);
		}
		else
		{
			DrawInvdShip(GetItemPixel(point), (pSnapLink->Boat.COG - mothership.COG)/10);
		}
		
		GUI_SetTextMode(GUI_TM_TRANS);
	}
	
	GUI_CURSOR_SetPosition(pixel.x, pixel.y);
//	GUI_CURSOR_Show();
}



Point GetItemPixel(Point itemPoint)    //փսהгքб̘ظҪ
{
	Point itemPixel;
	itemPixel.x =  itemPoint.x*TO_PIXEL + motherShipPixel.x;
	itemPixel.y = -itemPoint.y*TO_PIXEL + motherShipPixel.y;
	return itemPixel;
}


static void FigureMotherShipPixel(Point center, int flag)    //݆̣ĸԬքб̘ظҪ    centerúʵ݊ѨҪДʾ׶Χքאф֣
{
	Point screenCenter; //֨ӥǁĻДʾȸб̘אф֣
	if(!flag)
	{
		screenCenter.x = 215;
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
	
static void FigureScale(int flag)                  // ݆̣҈}ԟ  flag=0ҭʾmainShipWinַԃՋگ˽û
																										//flag=1ҭʾalarmmonitorWinַԃՋگ˽
{
	int i;
	int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
	Point center;
	BULY_BERTH *pBully = pBulyHeader;

	x_min = STUB_GetMostValue(X_MIN);
	x_max = STUB_GetMostValue(X_MAX);
	y_min = STUB_GetMostValue(Y_MIN);
	y_max = STUB_GetMostValue(Y_MAX);
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

void DrawInvdShip(Point pixel, int course)  //course:ڽвèއ׈׆é  ۭһٶԳɫԬ
{
	double _cos = cos(course*ANGLE_TO_RAD);
	double _sin = sin(course*ANGLE_TO_RAD);
	
	GUI_SetPenSize(2);
	GUI_SetLineStyle(GUI_LS_SOLID);
	
	GUI_DrawLine(pixel.x-7*_cos-9*_sin, pixel.y-7*_sin+9*_cos, pixel.x+16*_sin      , pixel.y-16*_cos);
	GUI_DrawLine(pixel.x+16*_sin,       pixel.y-16*_cos,       pixel.x+7*_cos-9*_sin, pixel.y+7*_sin+9*_cos);
	GUI_DrawLine(pixel.x+7*_cos-9*_sin, pixel.y+7*_sin+9*_cos, pixel.x-7*_cos-9*_sin, pixel.y-7*_sin+9*_cos );
}

void DrawBullyShip(Point pixel, int course)    //ۭһٶٟ̙ԬܲԦ־Ԭ
{
	double _cos = cos(course*ANGLE_TO_RAD);
	double _sin = sin(course*ANGLE_TO_RAD);
	
	GUI_SetPenSize(3);
	GUI_SetLineStyle(GUI_LS_SOLID);
  GUI_DrawLine(pixel.x-10.5*_cos-13.5*_sin, pixel.y-10.5*_sin+13.5*_cos, pixel.x+24*_sin       , pixel.y-24*_cos);
	GUI_DrawLine(pixel.x+24*_sin,        pixel.y-24*_cos,        pixel.x+10.5*_cos-13.5*_sin, pixel.y+10.5*_sin+13.5*_cos);
	GUI_DrawLine(pixel.x+10.5*_cos-13.5*_sin, pixel.y+10.5*_sin+13.5*_cos, pixel.x-10.5*_cos-13.5*_sin, pixel.y-10.5*_sin+13.5*_cos );
}

void DrawAllOtherShips()    //ۭ̹ԐքԬ
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
			if((SimpBerthes[i].pBerth->Boat.category & 0x0f) == TYPE_BULLY)
			{
				DrawBullyShip(GetItemPixel(point), (SimpBerthes[i].pBerth->Boat.COG - mothership.COG)/10);
			}
			else
				DrawInvdShip(GetItemPixel(point), (SimpBerthes[i].pBerth->Boat.COG - mothership.COG)/10);
		}
	}
	
//	if(monitorState == ON)
//	{
	
		while(pBully)
		{
			point.x = pBully->pBoatLink->x_to_cross;
			point.y = pBully->pBoatLink->y_to_cross;
			DrawBullyShip(GetItemPixel(point), (pBully->pBoatLink->Boat.COG - mothership.COG)/10);
			pBully = pBully->pNext;
		}
//	}
}



