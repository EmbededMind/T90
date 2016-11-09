#include "play_speech.h"

int isKeyTrigged  = 0;

void play_speech()
{
   Bool findASpeech = FALSE;
   static uint8_t flag_isPlayed = ~0x00;
   static uint8_t pri = 1;
   uint8_t  musics[30];
   uint8_t  musicCursor  = 0; 
   uint8_t  Nums[3];
   uint8_t  aNums[5];	
   int angle;   
   uint8_t playList  = 1;  
   BULY_BERTH* thisBulyBerth_gov  = NULL;
   BULY_BERTH* thisBulyBerth_highSpeed  = NULL;
   BERTH * thisinvdBerth = NULL;   
   ISD_Wait_PWRUp();  
   ISD_SetVolumn(t90_set.sys.volum);
   MUSIC_RESET;
   ISD_Play(SND_ID_WLCM);
   ISD_PWRDn();      
   while(1)
   { 
//printf("Play Task while begin\n");      
      if(monitorState == ON) 
      {
         findASpeech = FALSE;
         pri = 1;
         switch(pri)
         {
            case 1:
               if(flag_isPlayed & 0x01)
               {
                  thisBulyBerth_gov  = BULY_gov_fetchNextPlayBerth();
                  if(thisBulyBerth_gov)
                  {
                     findASpeech = TRUE;
                     switch(thisBulyBerth_gov->pBoatLink->Boat.category & 0xf0){
                        case NATION_CTB:
                           MUSIC_ADD(SND_ID_CTB);
                        break;
                        case NATION_JPN:
                           MUSIC_ADD(SND_ID_JPN);
                        break;
                        case NATION_KOR:
                           MUSIC_ADD(SND_ID_KOR);                  
                        break;
                        case NATION_PRK:
                           MUSIC_ADD(SND_ID_PRK);
                        break;
                        case NATION_INA:
                           MUSIC_ADD(SND_ID_INA);
                        break;
                        case NATION_VIE:
                           MUSIC_ADD(SND_ID_VIE);
                        break;
                     }                                  
                     angle = getAngleOfShip(thisBulyBerth_gov->pBoatLink);

                     if(angle>=0 && angle<360)
                     {
                        SND_ParseNum(angle*1000,aNums);
                        MUSIC_ADD(SND_ID_ANG);                                   
                        MUSIC_ADD_5NUMS;
                        MUSIC_ADD(SND_ID_DEG);
                     }                                                    
                     if(thisBulyBerth_gov->pBoatLink->Boat.dist < 99999)
                     {
                        SND_ParseNum((t90_set.sys.unit == NM)? thisBulyBerth_gov->pBoatLink->Boat.dist/100*100 : thisBulyBerth_gov->pBoatLink->Boat.dist * 37/20/100*100, aNums);
                        MUSIC_ADD(SND_ID_DST);      
                        MUSIC_ADD_5NUMS;                                   
                        if(t90_set.sys.unit == NM)
                        {   
                           MUSIC_ADD(SND_ID_NM);
                        }
                        else
                        {
                           MUSIC_ADD(SND_ID_KM);
                        }
                        MUSIC_ADD(SND_ID_SIS);                                  
                        SND_ParseNum(thisBulyBerth_gov->pBoatLink->Boat.SOG *100, aNums);
                        MUSIC_ADD_5NUMS;
                        MUSIC_ADD(SND_ID_KT);
                     }
                     flag_isPlayed &= ~0x01;
                  }
               }
            case 2:
               if(flag_isPlayed & (0x01<<1))
               {
                  thisBulyBerth_highSpeed = BULY_highSpeed_fetchNextPlayBerth();
                  if(thisBulyBerth_highSpeed)
                  {
                     if(findASpeech)   break;
                     findASpeech = TRUE;   
                     MUSIC_ADD(SND_ID_HSB);
                     angle = getAngleOfShip(thisBulyBerth_highSpeed->pBoatLink);
                     if(angle>=0 && angle<360)
                     {
                        SND_ParseNum(angle*1000,aNums);
                        MUSIC_ADD(SND_ID_ANG);                              
                        MUSIC_ADD_5NUMS;
                        MUSIC_ADD(SND_ID_DEG);
                     }                                                 
                     MUSIC_ADD(SND_ID_DST);     
                     SND_ParseNum((t90_set.sys.unit == NM)? thisBulyBerth_highSpeed->pBoatLink->Boat.dist/100*100 : thisBulyBerth_highSpeed->pBoatLink->Boat.dist * 37/20/100*100, aNums);
                     MUSIC_ADD_5NUMS;
                     if(t90_set.sys.unit == NM)
                     {
                        MUSIC_ADD(SND_ID_NM);
                     }
                     else
                     {
                        MUSIC_ADD(SND_ID_KM);
                     }
                     flag_isPlayed &= ~(0x01<<1);
                  }
               }
            case 3:
               if(flag_isPlayed & (0x01<<2))
               {
                  if(MS_isSpeeding == MNTState_Triggered)
                  {
                     if(findASpeech)   break;
                     findASpeech = TRUE;           
                     SND_ParseNum(t90_set.alarm.danger_sog*100,aNums);
                     MUSIC_ADD(SND_ID_MHS);
                     MUSIC_ADD_5NUMS;
                     MUSIC_ADD(SND_ID_KT);
                     flag_isPlayed &= ~(0x01<<2);
                  }
                  else if(MS_isMax_SOG == MNTState_Triggered)
                  {
                     if(findASpeech)   break;
                     findASpeech = TRUE;
                     SND_ParseNum(Triggered_SOG*100,aNums);
                     MUSIC_ADD(SND_ID_SN);                                   
                     MUSIC_ADD_5NUMS;
                     MUSIC_ADD(SND_ID_KT);
                     MUSIC_ADD(SND_ID_HIGH);
                     MUSIC_ADD(SND_ID_SNOR);
                     flag_isPlayed &= ~(0x01<<2);
                  }
                  else if(MS_isMin_SOG == MNTState_Triggered)
                  {
                     if(findASpeech)   break;
                     findASpeech = TRUE;
                     SND_ParseNum(Triggered_SOG*100,aNums);
                     MUSIC_ADD(SND_ID_SN);                                  
                     MUSIC_ADD_5NUMS;
                     MUSIC_ADD(SND_ID_KT);
                     MUSIC_ADD(SND_ID_LOW);
                     MUSIC_ADD(SND_ID_SNOR);
                     flag_isPlayed &= ~(0x01<<2);                 
                  }
               }
            case 4:
               if(flag_isPlayed & (0x01<<3))
               {
                  thisinvdBerth = SIMP_BERTH_fetchNextPlayBerth();
                  if(thisinvdBerth)
                  {
                     if(findASpeech)   break;
                     findASpeech = TRUE;   
                     MUSIC_ADD(SND_ID_INVD);
                     if(thisinvdBerth->y_to_cross > FetchMidStub())
                     {
                        MUSIC_ADD(SND_ID_MS);
                     }
                     else
                     {
                        MUSIC_ADD(SND_ID_NET);
                     }                                    
                     angle = getAngleOfShip(thisinvdBerth);                                           
                     if(angle>=0 && angle<360)
                     {
                        SND_ParseNum(angle*1000,aNums);
                        MUSIC_ADD(SND_ID_ANG);                                        
                        MUSIC_ADD_5NUMS;
                        MUSIC_ADD(SND_ID_DEG);
                     }                                         
                     MUSIC_ADD(SND_ID_DST);
                     SND_ParseNum((t90_set.sys.unit == NM)? thisinvdBerth->Boat.dist/100*100 : thisinvdBerth->Boat.dist * 37/20/100*100, aNums);																	 
                     MUSIC_ADD_5NUMS;
                     if(t90_set.sys.unit == NM)
                     {   
                        MUSIC_ADD(SND_ID_NM);
                     }
                     else
                     {
                        MUSIC_ADD(SND_ID_KM);
                     }
                     flag_isPlayed &= ~(0x01<<3);
                  }
               }
            case 5:
               if(flag_isPlayed & (0x01<<4))
               {
                  if(FetchSTime() == 0)
                  {
                     if(findASpeech)   break;
                     findASpeech = TRUE;   
                     MUSIC_ADD(SND_ID_STOF);
                     flag_isPlayed &= ~(0x01<<4);
                  }
               }
            default:
               flag_isPlayed = ~0x00;
               break;
         }
         OSTimeDlyHMSM(0, 0, 2, 0);
      }
      if(musicCursor){
         int i  = 0;

         ISD_Wait_PWRUp();             
         for(i=0; i< musicCursor; i++)
         {
            int timeOutCnt  = 0;
          
            ISD_Play(musics[i]);
            while(ISD_IsBusy())
            {
               if(timeOutCnt > 15)
               {
                  timeOutCnt  = 0;
                  break;
               }
               timeOutCnt ++;
               OSTimeDlyHMSM(0, 0, 0, 200);
            }
         }
         ISD_PWRDn();            
         MUSIC_RESET;                // ÏÂ±êÖÃ0
      } /// End. execute play 
      /// End . if(monitorState == FALSE) 
//printf("Play task while end\n");      
      OSTimeDlyHMSM(0, 0, 3, 0);

   } /// 'End'. while(1).In fact this will not happen
}