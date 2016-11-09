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

#define MUSIC_ADD_5NUMS if(aNums[0]) \
                        { \
							                     MUSIC_ADD(aNums[0]); \
						                  } \
                        if(aNums[1]) \
						                  { \
                            MUSIC_ADD(aNums[1]); \
					                  	} \
                        if(aNums[2]) \
						                  { \
                            MUSIC_ADD(aNums[2]); \
						                  } \
                        if(aNums[3]) \
					                  	{ \
                            MUSIC_ADD(aNums[3]); \
						                  } \
                        if(aNums[4]) \
					                  	{ \
                            MUSIC_ADD(aNums[4]); \
						                  } 

extern int Triggered_SOG;
void play_speech();