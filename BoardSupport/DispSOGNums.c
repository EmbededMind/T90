
/*  显示主界面方块字   */

#include "GUI.h"
#include "DispSOGNums.h"


static uint8_t numcode[10]={0x3f,0x0c,0x76,0x5e,0x4d,0x5b,0x7b,0x0e,0x7f,0x5f} ;   

void DrawNums(Point Atpoint,uint8_t numCode, int type)
{
    if(type == 1)
    {
        if((numCode & 0x01) == 1)
            GUI_FillRect(Atpoint.x, Atpoint.y, Atpoint.x+26, Atpoint.y+49);
        if((numCode & (0x01<<1)) == 2)
            GUI_FillRect(Atpoint.x, Atpoint.y, Atpoint.x+102, Atpoint.y+20);
        if((numCode & (0x01<<2)) == 4)
            GUI_FillRect(Atpoint.x+76, Atpoint.y, Atpoint.x+102, Atpoint.y+49);
        if((numCode & (0x01<<3)) == 8)
            GUI_FillRect(Atpoint.x+76, Atpoint.y+29, Atpoint.x+102, Atpoint.y+79);
        if((numCode & (0x01<<4)) == 16)
            GUI_FillRect(Atpoint.x, Atpoint.y+59, Atpoint.x+102, Atpoint.y+79);
        if((numCode & (0x01<<5)) == 32)
            GUI_FillRect(Atpoint.x, Atpoint.y+29, Atpoint.x+26, Atpoint.y+79);
        if((numCode & (0x01<<6)) == 64)
            GUI_FillRect(Atpoint.x, Atpoint.y+29, Atpoint.x+79, Atpoint.y+49);
    }
    else if(type == 0)
    {
        if((numCode & 0x01) == 1)
            GUI_FillRect(Atpoint.x, Atpoint.y, Atpoint.x+15, Atpoint.y+29);
        if((numCode & (0x01<<1)) == 2)
            GUI_FillRect(Atpoint.x, Atpoint.y, Atpoint.x+60, Atpoint.y+12);
        if((numCode & (0x01<<2)) == 4)
            GUI_FillRect(Atpoint.x+45, Atpoint.y, Atpoint.x+60, Atpoint.y+29);
        if((numCode & (0x01<<3)) == 8)
            GUI_FillRect(Atpoint.x+45, Atpoint.y+17, Atpoint.x+60, Atpoint.y+47);
        if((numCode & (0x01<<4)) == 16)
            GUI_FillRect(Atpoint.x, Atpoint.y+35, Atpoint.x+60, Atpoint.y+47);
        if((numCode & (0x01<<5)) == 32)
            GUI_FillRect(Atpoint.x, Atpoint.y+17, Atpoint.x+15, Atpoint.y+47);
        if((numCode & (0x01<<6)) == 64)
            GUI_FillRect(Atpoint.x, Atpoint.y+17, Atpoint.x+47, Atpoint.y+29);
    
    }
    else if(type == 3)
    {
        if((numCode & 0x01) == 1)
            GUI_FillRect(Atpoint.x, Atpoint.y, Atpoint.x+12, Atpoint.y+31);
        if((numCode & (0x01<<1)) == 2)
            GUI_FillRect(Atpoint.x, Atpoint.y, Atpoint.x+52, Atpoint.y+10);
        if((numCode & (0x01<<2)) == 4)
            GUI_FillRect(Atpoint.x+40, Atpoint.y, Atpoint.x+52, Atpoint.y+31);
        if((numCode & (0x01<<3)) == 8)
            GUI_FillRect(Atpoint.x+40, Atpoint.y+21, Atpoint.x+52, Atpoint.y+53);
        if((numCode & (0x01<<4)) == 16)
            GUI_FillRect(Atpoint.x, Atpoint.y+43, Atpoint.x+52, Atpoint.y+53);
        if((numCode & (0x01<<5)) == 32)
            GUI_FillRect(Atpoint.x, Atpoint.y+21, Atpoint.x+12, Atpoint.y+53);
        if((numCode & (0x01<<6)) == 64)
            GUI_FillRect(Atpoint.x, Atpoint.y+21, Atpoint.x+52, Atpoint.y+31);
    
    }
    
}

void DispSOGNums(int x, int y, int num, int type)
{
    int tenth; 
    int ones;
    int tens;
   
	Point atpointtenth;
	Point atpointones;
   Point atpointtens;
   
	
    if(type == 1)
    {
        atpointones.x = x;
	     atpointones.y = y;
        atpointtenth.x = atpointones .x + 148;
        atpointtenth.y = atpointones.y;
        ones = num/10;
        tenth = num%10;
        if(ones < 10)
        {
            GUI_FillRect(atpointones.x+112,atpointones.y+59,atpointones.x+138,atpointones.y+79);      
            DrawNums(atpointones,numcode[ones],type);
            DrawNums(atpointtenth,numcode[tenth],type);
        }
        else
        {
            ones = num/100;
            tenth = num%100/10;
            if(ones)
                DrawNums(atpointones,numcode[ones],type);
            DrawNums(atpointtenth,numcode[tenth],type);
        }
    }
    else if(type == 0)
    {
        atpointones.x = x;
	     atpointones.y = y;
        atpointtenth.x = atpointones .x + 87;
        atpointtenth.y = atpointones.y;
        ones = num/10;
        tenth = num%10;
        if(ones < 9)
        {
            GUI_FillRect(atpointones.x+66,atpointones.y+35,atpointones.x+81,atpointones.y+47);        
            DrawNums(atpointones,numcode[ones],type);
            DrawNums(atpointtenth,numcode[tenth],type);
        }
        else
        {
            ones = num/100;
            tenth = num%100/10;
            if(ones)
                DrawNums(atpointones,numcode[ones],type);
            DrawNums(atpointtenth,numcode[tenth],type);
        }
    
    
    }
    else if(type == 3)
    {  
        atpointtens.x = x;
        atpointtens.y = y;
        atpointones.x = x + 64;
        atpointones.y = y;
        atpointtenth.x = x +  132;
        atpointtenth.y = y;
        tens = num / 100;
        ones = num % 100 /10;
        tenth = num % 10;
        GUI_FillRect(x + 118, y + 41, x + 130, y + 53);
        if(tens)
            DrawNums(atpointtens,numcode[tens],3);
        DrawNums(atpointones,numcode[ones],3);
        DrawNums(atpointtenth,numcode[tenth],3);
    }
    else if(type == 4)
    {
       atpointones.x = x;
       atpointones.y = y;
       atpointtenth.x = x+68;
       atpointtenth.y = y;
       ones = num / 10;
       tenth = num % 10;
       GUI_FillRect(x + 54, y + 41, x + 66, y + 53);
       DrawNums(atpointones,numcode[ones],3);
       DrawNums(atpointtenth,numcode[tenth],3);
    }
    
}






