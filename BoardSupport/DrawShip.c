#include "GUI.h" 
#include "math.h"
#include "Draw.h"



/*****************************************************/



void DrawSingleShip(int Space,int L1,int L2,int L3, GUI_COLOR color)
{
	
	int l1 = L1*FIXEDPIXEL/(L1+L3);
	int l2 = L2*FIXEDPIXEL/(L1+L3);
	int l3 = L3*FIXEDPIXEL/(L1+L3);
	int space = 5+Space/5*4*FIXEDPIXEL/(L1+L3)/8;
	int _hypot1 = l1*l1+l2*l2;
	int _hypot2 = l2*l2+l3*l3;
	int hypot1 = sqrt(_hypot1);
	int hypot2 = sqrt(_hypot2);
	int basex = _BASEX;
	int basey = _BASEY+l1;
	  

	


//	GUI_SetBkColor(GUI_WHITE);
////	GUI_Clear();
	GUI_SetPenSize( 1 );
	GUI_SetLineStyle(GUI_LS_SOLID);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(color);
	
	GUI_DrawArc( basex,basey-l1,space,space,atan2(l2,l1)*180.0/3.1416,    180-atan2(l2,l1)*180/3.1416);
	GUI_DrawArc( basex,basey+l3,space,space,180+atan2(l2,l3)*180.0/3.1416,360-atan2(l2,l3)*180/3.1415 );
	GUI_DrawArc( basex+l2,basey,space,space,360-atan2(l2,l3)*180.0/3.1416,360+atan2(l2,l1)*180.0/3.1416 );
	GUI_DrawArc( basex-l2,basey,space,space,540-atan2(l2,l1)*180.0/3.1416,540+atan2(l2,l3)*180.0/3.1416 );

	GUI_DrawLine(basex-l1*space/hypot1,basey-l1-l2*space/hypot1,basex-l2-l1*space/hypot1,basey-l2*space/hypot1);
//	GUI_DrawLine(basex-l2-l3*space/hypot2,basey+l2*space/hypot2,basex-l3*space/hypot2,basey+l3+l2*space/hypot2);
	GUI_DrawLine(basex+l1*space/hypot1,basey-l1-l2*space/hypot1,basex+l2+l1*space/hypot1,basey-l2*space/hypot1);
//	GUI_DrawLine(basex+l2+l3*space/hypot2,basey+l2*space/hypot2,basex+l3*space/hypot2,basey+l3+l2*space/hypot2);

//     GUI_DrawLine(basex-cos(atan2(l2,l1))*space,basey-l1-sin(atan2(l2,l1))*space,basex-l2-cos(atan2(l2,l1))*space,basey-sin(atan2(l2,l1))*space);
     GUI_DrawLine(basex-cos(atan2(l2,l3))*space,basey+l3+sin(atan2(l2,l3))*space,basex-l2-cos(atan2(l2,l3))*space,basey+sin(atan2(l2,l3))*space);
//     GUI_DrawLine(basex+cos(atan2(l2,l1))*space,basey-l1-sin(atan2(l2,l1))*space,basex+l2+cos(atan2(l2,l1))*space,basey-sin(atan2(l2,l1))*space);
     GUI_DrawLine(basex+cos(atan2(l2,l3))*space,basey+l3+sin(atan2(l2,l3))*space,basex+l2+cos(atan2(l2,l3))*space,basey+sin(atan2(l2,l3))*space);

	DrawmShip(basex,basey-l1,100);
	DrawNet(basex,basey,l1,l2,l3,100);
	GUI_SetPenSize(1);
	GUI_SetLineStyle(GUI_LS_DOT);
	DrawArrow(basex,basey,l1,l2,l3);
	GUI_SetPenSize(1);
	GUI_DispStringAt("01",basex-l2-5-5-10 , basey+5);
	GUI_DispStringAt("02",basex-5-5 ,basey+l3 +5);
	GUI_DispStringAt("03",basex+l2-5+10 ,basey +5);
}


