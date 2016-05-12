#include "GUI.h" 
#include "math.h"
#include "Draw.h"
#include "layout.h"
#include "T90.h"
#include "Config.h"
#include "boat_struct.h"
#include "bully.h"
#include "snap.h"


static const HomeColor *pColor=homeColors;
int max_x = 0,max_y = 0,min_x = 0,min_y = 0;
int half_x = 0;
int half_y = 0;
double Diff = 1;

static void DrawCursor(int xOnBase, int yOnBase, GUI_COLOR color)
{
	double basex = screen_hcenter+(xOnBase-half_x)*Diff; 
	double basey = screen_vcenter-(yOnBase-half_y)*Diff;
	GUI_SetLineStyle(GUI_LS_SOLID);
	GUI_SetColor(color);
	GUI_DrawPoint(basex, basey);
	GUI_DrawHLine(basey, basex - 10, basex - 2);
	GUI_DrawHLine(basey, basex + 2, basex + 10);
	GUI_DrawVLine(basex, basey - 10, basey - 2);
	GUI_DrawVLine(basex, basey + 2, basey + 10);
	
	GUI_SetFont(GUI_FONT_13_1);
	GUI_SetColor(pColor->textColor);
	sprintf(pStrBuf,"name:%s",pSnapLink->Boat.name);
	GUI_DispStringAt(pStrBuf, basex + 10, basey + 10);
	
	GUI_DispStringAt("N", basex + 10, basey + 10 + GUI_GetFontSizeY());
	lltostr(pSnapLink->Boat.latitude, pStrBuf);
	GUI_DispStringAt(pStrBuf, basex + 23, basey+10+GUI_GetFontSizeY());
	
	GUI_DispStringAt("E", basex + 10, basey + 10+GUI_GetFontSizeY()*2);
	lltostr(pSnapLink->Boat.longitude, pStrBuf);
	GUI_DispStringAt(pStrBuf, basex + 23, basey+10+GUI_GetFontSizeY()*2);

	GUI_DispStringAt("SOG:", basex + 10, basey +10+ GUI_GetFontSizeY()*3);
	sprintf(pStrBuf, "%2d.%d", pSnapLink->Boat.SOG/10, pSnapLink->Boat.SOG%10);
	GUI_DispStringAt(pStrBuf, basex + 45, basey +10+ GUI_GetFontSizeY()*3);
	
	GUI_DispStringAt("COG:", basex + 10, basey + 10+GUI_GetFontSizeY()*4);
	sprintf(pStrBuf, "%3d", pSnapLink->Boat.COG/10);
  pStrBuf[3]  = 194;
  pStrBuf[4]  = 176;
  pStrBuf[5]  = '\0';
	GUI_DispStringAt(pStrBuf, basex + 45, basey + 10+GUI_GetFontSizeY()*4);
}
	
	
static void DrawBullyShip(int xOnBase,int yOnBase,double course,GUI_COLOR Color)
{
	double basex = screen_hcenter+(xOnBase-half_x)*Diff; 
	double basey = screen_vcenter-(yOnBase-half_y)*Diff;
	double _cos = cos(course);
	double _sin = sin(course);
	
	GUI_SetLineStyle(GUI_LS_SOLID);
	GUI_SetColor(Color);
     
	GUI_DrawLine(basex-7*_cos-10*_sin , basey-7*_sin+10*_cos ,basex+10*_sin        , basey-10*_cos);
	GUI_DrawLine(basex+10*_sin        , basey-10*_cos,basex+7*_cos-10*_sin , basey+7*_sin+10*_cos);
	GUI_DrawLine(basex+7*_cos-10*_sin , basey+7*_sin+10*_cos,basex-7*_cos-10*_sin , basey-7*_sin+10*_cos );
	GUI_SetColor(GUI_BLACK);
}
static void DrawForgBullyShip(int xOnBase,int yOnBase,double course)
{
	int drawsitex,drawsitey;
	double _cos = cos(course);
	double _sin = sin(course);
	int oppositex=xOnBase-half_x;
	int oppositey=yOnBase-half_y;
	int Flag;
     
	if(oppositex/Diff>350||oppositex/Diff<-350||oppositey/Diff<-160||oppositey/Diff>160)
	{
	 _cos = cos(course);
	 _sin = sin(course);
	
	
	if((2*fabs(oppositey))>fabs(oppositex))
	{
		if(oppositey>0)
		{
			Flag=0;
		}
		else
		{
			Flag=2;
		}
	}
	else
	{
		if(oppositex>0)
		{
			Flag=1;
		}
		else
		{
			Flag=3;
		}
	}

	switch(Flag){
		case 0: 
				
					drawsitex=390+195*oppositex/oppositey;
					drawsitey=65;
					break;
				
		case 1: 
				
					drawsitex=770;
					drawsitey=220-380*oppositey/oppositex;
					break;
				
		case 2: 
				
					drawsitex=390-195*oppositex/oppositey;
					drawsitey=460;
					break;
				
		case 3: 
				
					drawsitex=15;
					drawsitey=220+380*oppositey/oppositex;
					break;
		
	}
	GUI_SetColor(pColor->forgBullyColor);
	GUI_DrawLine(drawsitex-7*_cos-10*_sin , drawsitey-7*_sin+10*_cos,drawsitex+10*_sin        , drawsitey-10*_cos    );
	GUI_DrawLine(drawsitex+10*_sin        , drawsitey-10*_cos    ,drawsitex+7*_cos-10*_sin , drawsitey+7*_sin+10*_cos);
	GUI_DrawLine(drawsitex+7*_cos-10*_sin , drawsitey+7*_sin+10*_cos,drawsitex-7*_cos-10*_sin , drawsitey-7*_sin+10*_cos);
     GUI_SetColor(GUI_BLACK);	
     }
	else{
		
		DrawBullyShip(xOnBase,yOnBase,course,pColor->forgBullyColor);
	}
	 

}

static void DrawBodySingleShip(int Space,int L1,int L2,int L3)
{
     
	int l1 = L1*Diff;  
	int l2 = L2*Diff;
	int l3 = L3*Diff;
	int space = Space*Diff;
     
	int basex = screen_hcenter-half_x*Diff;      
	int basey = screen_vcenter+(half_y+L1)*Diff;

	GUI_Clear();
	GUI_SetPenSize( 1 );
	GUI_SetLineStyle(GUI_LS_SOLID);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(pColor->textColor);
	
	GUI_DrawArc( basex,basey-l1,space,space,atan2(l2,l1)*180.0/3.1416,    180-atan2(l2,l1)*180/3.1416);
	GUI_DrawArc( basex,basey+l3,space,space,180+atan2(l2,l3)*180.0/3.1416,360-atan2(l2,l3)*180/3.1416);
	GUI_DrawArc( basex+l2,basey,space,space,360-atan2(l2,l3)*180.0/3.1416,360+atan2(l2,l1)*180.0/3.1416 );
	GUI_DrawArc( basex-l2,basey,space,space,540-atan2(l2,l1)*180.0/3.1416,540+atan2(l2,l3)*180.0/3.1416 );

     GUI_DrawLine(basex-cos(atan2(l2,l1))*space+(t90_set.alarm.invd_dst!=0),basey-l1-sin(atan2(l2,l1))*space,basex-l2-cos(atan2(l2,l1))*space+(t90_set.alarm.invd_dst!=0),basey-sin(atan2(l2,l1))*space);
     GUI_DrawLine(basex-cos(atan2(l2,l3))*space+(t90_set.alarm.invd_dst!=0),basey+l3+sin(atan2(l2,l3))*space,basex-l2-cos(atan2(l2,l3))*space+(t90_set.alarm.invd_dst!=0),basey+sin(atan2(l2,l3))*space);
     GUI_DrawLine(basex+cos(atan2(l2,l1))*space,basey-l1-sin(atan2(l2,l1))*space,basex+l2+cos(atan2(l2,l1))*space,basey-sin(atan2(l2,l1))*space);
     GUI_DrawLine(basex+cos(atan2(l2,l3))*space,basey+l3+sin(atan2(l2,l3))*space,basex+l2+cos(atan2(l2,l3))*space,basey+sin(atan2(l2,l3))*space);

	DrawmShip(basex,basey-l1,Diff*400);
	DrawNet(basex,basey,l1,l2,l3,Diff*400);
	GUI_SetPenSize( 1 );
	GUI_SetLineStyle(GUI_LS_DOT);
	DrawArrow(basex,basey,l1,l2,l3);
}


void Draw_things(int x,int y)
{
  if(x > max_x) max_x = x;
	if(y > max_y) max_y = y;
	if(x < min_x) min_x = x;
	if(y < min_y) min_y = y;	
	
  half_x = (max_x+min_x)/2;
  half_y = (max_y+min_y)/2;
	
  if(((max_x-min_x))>(max_y-min_y)*2)
	{
		Diff = 330.0/(max_x-min_x);
	}
	else
	{
		Diff = 165.0/(max_y-min_y);
	} 
}


void DrawAlarmShip(void) {
	BULY_BERTH* thisbully;
  int i;	
	int L1 = t90_set.dst.dst1,L2 = t90_set.dst.dst2,L3 = t90_set.dst.dst3-t90_set.dst.dst1,Space = t90_set.alarm.invd_dst*37/20;	pColor = &homeColors[t90_set.sys.nightmode];
	thisbully = pBulyHeader;
  screen_hcenter=SCREEN_HCENTER;
	screen_vcenter=(SCREEN_HEIGHT-45)/2+45; //-(L1+L2)/Diff
  DrawBodySingleShip(Space,L1,L2,L3);
//	DrawBullyShip(2000,1000,3.1416/2,GUI_YELLOW);
     /*************************************************************/
     GUI_DrawRect(screen_hcenter-100,screen_vcenter-50,screen_hcenter+100,screen_vcenter+50) ;
//     GUI_DrawRect(screen_hcenter-150,screen_vcenter-75,screen_hcenter+150,screen_vcenter+75) ;
//     GUI_DrawRect(screen_hcenter-200,screen_vcenter-100,screen_hcenter+200,screen_vcenter+100) ;
//     GUI_DrawRect(screen_hcenter-250,screen_vcenter-125,screen_hcenter+250,screen_vcenter+125);
//     GUI_DrawRect(screen_hcenter-300,screen_vcenter-150,screen_hcenter+300,screen_vcenter+150) ;
     /*************************************************************/
     max_x = 0;
     max_y = 0;
     min_x = 0;
     min_y = 0;
     for(i = 0;i < N_boat; i++)
     {
				if(SimpBerthes[i].pBerth->isInvader) 
				Draw_things(SimpBerthes[i].pBerth->x_to_cross,SimpBerthes[i].pBerth->y_to_cross);
		 }
//	 if(monitorState == ON)
//	 {
     while(thisbully)
     {
          thisbully->pBoatLink->x_to_cross=(int)(cos(mothership.COG*3.14/1800)*(thisbully->pBoatLink->Boat.longitude - mothership.longitude)
																				 -sin(mothership.COG*3.14/1800)*(thisbully->pBoatLink->Boat.latitude  - mothership.latitude));
          thisbully->pBoatLink->y_to_cross=(int)(sin(mothership.COG*3.14/1800)*(thisbully->pBoatLink->Boat.longitude - mothership.longitude)
																				 +cos(mothership.COG*3.14/1800)*(thisbully->pBoatLink->Boat.latitude  - mothership.latitude) + t90_set.dst.dst1*20/37);
          if((thisbully->pBoatLink->Boat.category & 0xf0) == 0)
          {
               Draw_things(thisbully->pBoatLink->x_to_cross,thisbully->pBoatLink->y_to_cross);
						   while((max_x-half_x)*Diff > 390)
							 {
										Diff = Diff*1.1;
							 }
							 while((max_y-half_y)*Diff > 205)
							 {
										Diff = Diff*1.1;
							 }
							 while((half_x-min_x)*Diff < 30)
							 {
										Diff = Diff*1.1;
							 }
							 while((half_y-min_y)*Diff < 30)
							 {
										Diff = Diff*1.1;
							 }
               DrawBullyShip(thisbully->pBoatLink->x_to_cross*37/20, thisbully->pBoatLink->y_to_cross*37/20, (thisbully->pBoatLink->Boat.COG-mothership.COG)/1800.0*3.1416,GUI_GREEN);                   
          }     
          thisbully = thisbully->pNext;
     }
//	 }
	 
	 
  for(i = 0; i < N_boat; i++)
	{
		if(SimpBerthes[i].pBerth->isInvader)
		{
               if(SimpBerthes[i].pBerth->mntState == MNTState_Triggered)
               {
                    DrawBullyShip(SimpBerthes[i].pBerth->x_to_cross*37/20, SimpBerthes[i].pBerth->y_to_cross*37/20, (SimpBerthes[i].pBerth->Boat.COG-mothership.COG)/1800.0*3.1416,GUI_YELLOW);                   
               }
               else
               {			
                    DrawBullyShip(SimpBerthes[i].pBerth->x_to_cross*37/20, SimpBerthes[i].pBerth->y_to_cross*37/20, (SimpBerthes[i].pBerth->Boat.COG-mothership.COG)/1800.0*3.1416,pColor->bullyColor);                   
		     }
     }
		if(!SimpBerthes[i].pBerth->isInvader && SimpBerthes[i].pBerth->x_to_cross && SimpBerthes[i].pBerth->y_to_cross)
		{
			DrawBullyShip(SimpBerthes[i].pBerth->x_to_cross*37/20, SimpBerthes[i].pBerth->y_to_cross*37/20, (SimpBerthes[i].pBerth->Boat.COG-mothership.COG)/1800.0*3.1416,GUI_BLACK);        
		}
	}
     
     DrawBullyShip(half_x, half_y, 0,GUI_MAGENTA);        
		 DrawCursor(pSnapLink->x_to_cross*37/20, pSnapLink->y_to_cross*37/20, pColor->textColor);
	
}









