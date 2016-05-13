#ifndef _DRAW_H
#define _DRAW_H


#include"GUI.h"
#include"math.h"
#define FIXEDPIXEL  160     //???????(fixedpixel)  ??(_basex _basey,_basex,_basey?mShip??? )
#define MIDX        220   //
#define MIDY        390     //
#define _BASEX      195     //
#define _BASEY      136     //
extern double l1,l2,l3,space;
extern int basex;
extern int basey;

void DrawSingleShip(int space,int l1,int l2,int l3, GUI_COLOR color);
void DrawAlarmShip(void);


void DrawmShip(int mShipx,int mShipy,int prop);

void DrawNet(int basex,int basey,int l1,int l2,int l3,int porp);
void DrawArrow(int x,int y,int l1,int l2,int l3);

static void DrawNetOne(int netx,int nety,int porp);


extern double Diff;
extern int max_x ;
extern int max_y ;
extern int min_x ;
extern int min_y ;
extern int half_x;
extern int half_y;

#endif
