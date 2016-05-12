#include"GUI.h"

#include"math.h"



void DrawArcScale(double basex,double basey,double space,double l1,double l2,double l3,double hypot1,double hypot2)
{
	

	struct Circle
	{
		int x;
		int y;
		int radii;
		int Statangle;
		int Endangle;
	};

	struct Line
	{
		int Statx;
		int Staty;
		int Endx;
		int Endy;
	};

	struct Circle Circlecarry[4]={
									{basex,basey-l1,space,atan2(l2,l1)*180.0/3.1416,    180-atan2(l2,l1)*180/3.1416},
									{basex,basey+l3,space,180+atan2(l2,l3)*180.0/3.1416,360-atan2(l2,l3)*180/3.1415 },
									{basex+l2,basey,space,360-atan2(l2,l3)*180.0/3.1416,360+atan2(l2,l1)*180.0/3.1416 },
									{basex-l2,basey,space,540-atan2(l2,l1)*180.0/3.1416,540+atan2(l2,l3)*180.0/3.1416}};
	struct Line Linecarry[5]={
									{basex-l1/hypot1*space,basey-l1-l2/hypot1*space,basex-l2-l1/hypot1*space,basey-l2/hypot1*space},
									{basex-l2-l3/hypot2*space,basey+l2/hypot2*space,basex-l3/hypot2*space,basey+l3+l2/hypot2*space},
									{basex+l1/hypot1*space,basey-l1-l2/hypot1*space,basex+l2+l1/hypot1*space,basey-l2/hypot1*space},
									{basex+l2+l3/hypot2*space,basey+l2/hypot2*space,basex+l3/hypot2*space,basey+l3+l2/hypot2*space}};

	

	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_SetPenSize( 2 );
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawArc( Circlecarry[0].x,Circlecarry[0].y,Circlecarry[0].radii,Circlecarry[0].radii,Circlecarry[0].Statangle,Circlecarry[0].Endangle );
	GUI_DrawArc( Circlecarry[1].x,Circlecarry[1].y,Circlecarry[1].radii,Circlecarry[1].radii,Circlecarry[1].Statangle,Circlecarry[1].Endangle );
	GUI_DrawArc( Circlecarry[2].x,Circlecarry[2].y,Circlecarry[2].radii,Circlecarry[2].radii,Circlecarry[2].Statangle,Circlecarry[2].Endangle );
	GUI_DrawArc( Circlecarry[3].x,Circlecarry[3].y,Circlecarry[3].radii,Circlecarry[3].radii,Circlecarry[3].Statangle,Circlecarry[3].Endangle );
	GUI_DrawLine(Linecarry[0].Statx,Linecarry[0].Staty,Linecarry[0].Endx,Linecarry[0].Endy);
	GUI_DrawLine(Linecarry[1].Statx,Linecarry[1].Staty,Linecarry[1].Endx,Linecarry[1].Endy);
	GUI_DrawLine(Linecarry[2].Statx,Linecarry[2].Staty,Linecarry[2].Endx,Linecarry[2].Endy);
	GUI_DrawLine(Linecarry[3].Statx,Linecarry[3].Staty,Linecarry[3].Endx,Linecarry[3].Endy);
	//	GUI_DrawArc(100,100,50,50,-30,30);
}
void MainTask(void);
void MainTask(void) {
	double l1=50*2,l2=20*2,l3=5*2,space=10*2,_hypot1,_hypot2;
	int hypot1,hypot2;
	int basex=160;
	int basey=150;
	//double spacel=1*20;
	//double spacelup=1*20;
	_hypot1=pow(l1,2)+pow(l2,2);
	_hypot2=pow(l2,2)+pow(l3,2);
	hypot1=sqrt(_hypot1);
	hypot2=sqrt(_hypot2);

  GUI_Init();
  GUI_EnableAlpha(1);
  DrawArcScale(basex,basey,space,l1,l2,l3,hypot1,hypot2);
  while (1) {
    GUI_Delay(100);
  }
}