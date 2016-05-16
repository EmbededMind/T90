#include "stub.h"
#include "boat_struct.h"
#include "T90.h"
#include "math.h"
<<<<<<< HEAD
#include "detect.h"
=======
#include "transform.h"
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
#include "sound.h"

#define  MYABS(X)    ((X)<0 ? -(X) : (X))  

#define   GETANGLE(Y,X)   (int)(atan2(MYABS(Y),MYABS(X))*180/3.1416)
    

void llToxy(BERTH *pBerth)
{
    
    pBerth->x_to_cross = (int)(cos(mothership.COG*3.14/1800)*(pBerth->Boat.longitude - mothership.longitude)
																				 -sin(mothership.COG*3.14/1800)*(pBerth->Boat.latitude  - mothership.latitude));
		pBerth->y_to_cross = (int)(sin(mothership.COG*3.14/1800)*(pBerth->Boat.longitude - mothership.longitude)
																				 +cos(mothership.COG*3.14/1800)*(pBerth->Boat.latitude  - mothership.latitude));    
}


int getAngleOfShip(BERTH *pBerth)
{
<<<<<<< HEAD
    int flag;
    int angle;
=======
//    int flag;
//    int angle;
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
    if(pBerth->x_to_cross > 0)
    {
        if(pBerth->y_to_cross >= 0)
        {            
             return 90 - GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross);
        }
        else
        {           
            return GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross) + 90;
        }
    }else if(pBerth->x_to_cross < 0)
    {
        if(pBerth->y_to_cross <= 0)
        {
            return 270 - GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross);
        }
        else
        {
            return 270 + GETANGLE(pBerth->y_to_cross,pBerth->x_to_cross);
        }
    }
    else
    {
         if(pBerth->y_to_cross >= 0)
        {
            return 0;
        }
        else 
        {
            return 180;
        }

    }

}


int SND_ParseNum(int num, uint8_t *pNum)
{
<<<<<<< HEAD
=======
	  memset(pNum,0,sizeof(uint8_t)*5);
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
    pNum[0] = num/100000;
    if(pNum[0])//百位不为零
    {
        pNum[1] = SND_ID_HAND;
        pNum[2] = num%100000/10000;
        if(pNum[2])//十位不为零
        {
            pNum[3] = SND_ID_TEN;
            pNum[4] = num%10000/1000;                   
        }
        else//十位为零                              
        {
            pNum[2] = num%10000/1000;
            if(pNum[2])//个位不为零
            {
                pNum[2] = SND_ID_ZRO;//点几  不够了  所以pNum[4]=0
                pNum[3] =num%10000/1000;
                pNum[4] = 0;
                return NULL;            
            }
            else
            {
                
                pNum[2] = num%1000/100;
                if(pNum[2])
                {
                    pNum[2] = SND_ID_DOT;
                    pNum[3] = num%1000/100;
                    pNum[4] = num%100/10;
                }
                else
                {
<<<<<<< HEAD
                    pNum[2] = SND_ID_DOT;   
                    pNum[3] = SND_ID_ZRO;
                    pNum[4] = num%100/10;       
=======
										pNum[4] = num%100/10;
									  if(pNum[4])
									  {
											pNum[2] = SND_ID_DOT;   
											pNum[3] = SND_ID_ZRO;
										}
									 
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
                }
            }
        }
    }
    else
    {
        pNum[0] = num/10000;
        if(pNum[0])
        {
<<<<<<< HEAD
=======
						if(pNum[0]==1)
							pNum[0] = 0;
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
            pNum[1] = SND_ID_TEN;
            pNum[2] = num%10000/1000;
            if(pNum[2])
            {
                pNum[3] = num%1000/100;
                if(pNum[3])
                {
                    pNum[3] =  SND_ID_DOT;
                    pNum[4] = num%1000/100;
                }
                else
                {
                    pNum[4] = 0;                      //点零几  不够了
                }
                        
            }                                                                 ////////////////////////
            else
            {
                pNum[2] = num%1000/100;
                if(pNum[2])
                {
                    pNum[2] = SND_ID_DOT;
                    pNum[3] = num%1000/100;
                    pNum[4] = num%100/10;
                   
                }
                else
                {
                    pNum[2] = num%100/10;
                    if(pNum[2])
                    {
                        pNum[2] = SND_ID_DOT;
                        pNum[3] = SND_ID_ZRO;
                        pNum[4] = num%100/10;
                    }
                    else
                    {
                        pNum[2] = 0;            //点零零几 不够了
                        pNum[3] = 0;
                        pNum[4] = 0;
                    }
                }
            
            }
        
        }
<<<<<<< HEAD
=======
				
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
        else            
        {
            pNum[0] = num/1000;
            if(pNum[0])
            {

                pNum[1] = num%1000/100;
                if(pNum[1])
                {
                    pNum[1] = SND_ID_DOT;
                    pNum[2] = num%1000/100;
                    pNum[3] = num%100/10;
                    if(pNum[3])
                    {
                        pNum[4] = num%10;                       
                    }
                    else
                    {
                         pNum[3] = num%10;
                            if(pNum[3])
                            {
                                pNum[3] = SND_ID_ZRO;
                                pNum[4] = num%10;
                                
                            }
                            else
                            {
                                pNum[4] = 0;
                            }
                    
                    }
                }
                else
                {
                    pNum[1] = num%100/10;
                    if(pNum[1])
                    {
                        pNum[1] = SND_ID_DOT;
                        pNum[2] = SND_ID_ZRO;
                        pNum[3] = num%100/10;
                        pNum[4] = num%10;
                    }
                    else
                    {
                        pNum[1] = num%10;
                        if(pNum[1])
                        {    
                            
                            pNum[4] = num%10;
                            if(pNum[4])
                            {                          
                                pNum[1] = SND_ID_DOT;
                                pNum[2] = SND_ID_ZRO;
                                pNum[3] = SND_ID_ZRO;
                                pNum[4] = num%10;
                            }
                            else
                            {
                                pNum[1] = 0;
                                pNum[2] = 0;
                                pNum[3] = 0;
                                pNum[4] = 0;
                            }
                        }
                    }
                }
            }
            else
            {
                pNum[0] = num/100;
                if(pNum[0])
                {
                    pNum[0] = SND_ID_ZRO;
                    pNum[1] = SND_ID_DOT;
                    pNum[2] = num/100;
                    pNum[3] = num%100/10;
                    if(pNum[3])
                    {
                        pNum[4] = num%10;
                    }
                    else
                    {
                        pNum[3] = num%10;
                        if(pNum[3])
                        {
                            pNum[3] = SND_ID_ZRO;
                            pNum[4] =  num%10;
                        }
                        else
                        {
                            pNum[3] = 0;
                            pNum[4] = 0;
                        }
                    }
                }
                else
                {
                     pNum[0] = num/10;
                    if(pNum[0])
                    {
                        pNum[0] =  SND_ID_ZRO;
                        pNum[1] = SND_ID_DOT;
                        pNum[2] = SND_ID_ZRO;
                        pNum[3] = num/10;
                        pNum[4] = num%10;
                    }
                    else
                    {
                        pNum[0] = num%10;
                        if(pNum[0])
                        {
                            pNum[0] = SND_ID_ZRO;
                            pNum[1] = SND_ID_DOT;
                            pNum[2] = SND_ID_ZRO;
                            pNum[3] = SND_ID_ZRO;
                            pNum[4] = num%10; 
                        }
                        else
                        {
                            pNum[0] = SND_ID_ZRO;
                        }
                    }
                }
              
            }
        }
    
    }
<<<<<<< HEAD
=======
		return NULL;
>>>>>>> dbea55b0602f76c55db45af16018aece8eb77247
}





