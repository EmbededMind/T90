#include <ucos_ii.h>
#include "sound.h"
#include "T90.h"
#include "bully.h"
#include "xt_isd.h"
#include "check.h"
#include "transform.h"
#include "stub.h"

#define MUSIC_ADD(x)  if(x==0) \
                         musics[musicCursor]  = SND_ID_ZRO; \
                      else \
                         musics[musicCursor]  = x; \
                      musicCursor++
                      
#define MUSIC_RESET   musicCursor  = 0;\
                      memset(musics,0,sizeof(uint8_t)*30);

#define MUSIC_ADD_5NUMS if(aNums[0]>=1 && aNums[0]<=13) \
                        { \
							                     MUSIC_ADD(aNums[0]); \
						                  } \
                        if(aNums[1]>=1 && aNums[1]<=13) \
						                  { \
                            MUSIC_ADD(aNums[1]); \
					                  	} \
                        if(aNums[2]>=1 && aNums[2]<=13) \
						                  { \
                            MUSIC_ADD(aNums[2]); \
						                  } \
                        if(aNums[3]>=1 && aNums[3]<=13) \
					                  	{ \
                            MUSIC_ADD(aNums[3]); \
						                  } \
                        if(aNums[4]>=1 && aNums[4]<=13) \
					                  	{ \
                            MUSIC_ADD(aNums[4]); \
						                  } \
                        if(aNums[5]>=1 && aNums[5]<=13) \
					                  	{ \
                            MUSIC_ADD(aNums[5]); \
						                  } \
                        if(aNums[6]>=1 && aNums[6]<=13) \
					                  	{ \
                            MUSIC_ADD(aNums[6]); \
						                  } \
                        if(aNums[7]>=1 && aNums[7]<=13) \
					                  	{ \
                            MUSIC_ADD(aNums[7]); \
						                  } 

extern int Triggered_SOG;
void play_speech();