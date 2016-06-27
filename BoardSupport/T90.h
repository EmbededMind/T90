#ifndef   _T90_H
#define   _T90_H

#include "GUI.h"
#include "stdio.h"
#include "T90Color.h"
#include "str.h"
#include "string.h"
#include "boat_struct.h"
#include "GUI.h"
#include "uart.h"
#include "lpc177x_8x_eeprom.h"
#include "Config.h"
//#include "stub.h"

#define T90

#define X_MAX   0x01
#define X_MIN   0x02
#define Y_MAX   0x03
#define Y_MIN   0x04

extern unsigned long SYS_Date;
extern unsigned long SYS_Time;

extern int monitorState;

extern int N_boat;
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];

#define SINGLE_MODE   0
#define DOUBLE_MODE   1
#define NONE_MODE     2

#define DAY      0
#define NIGHT    1

#define KM       0
#define NM       1

#define OFF      0
#define ON       1

#define DEFAULT_DST1   400
#define DEFAULT_DST2   25
#define DEFAULT_DST3   500
#define DEFAULT_DST4   25
#define DEFAULT_DST5   400
#define DEFAULT_DST6   100
#define DEFAULT_DOUDST1 2000
#define DEFAULT_DOUDST2 1000
#define DEFAULT_DOUDST3 4000
#define DEFAULT_DOUDST4 2000
#define DEFAULT_DOUDST5 4000
#define DEFAULT_DOUDST6 3000

#define DEFAULT_INVD_DST         2000
#define DEFAULT_DANGER_SOG_X10   80
#define DEFAULT_MIN_SOG_X10      15
#define DEFAULT_MAX_SOG_X10      55
#define DEFAULT_NATION           0x00

#define DEFAULT_VOLUM      1
#define DEFAULT_BRIGHT     4





/**



 */
typedef struct Dst_Set
{
	int dst1;
	int dst2;
	int dst3;
	int dst4;
	int dst5;
//   int dst6; ///   Distance of mothership to brother ship
// int dst7; ///   Distance of fishing net end points
// int dst8; ///   Distance of left end point  to motherhip
// int dst9; ///   Distance of net middle dot to mothership
} Dst_Set;


/**À´Õ–æ‡¿Î…Ë÷√≤Œ ˝
 *
 *
 */
typedef struct
{
   int dst1;
   int dst2;
   int dst3;
   int dst4;
   int dst5;
   int dst6;
} DouDstSet;



typedef struct Alarm_Set
{
	int invd_dst;
	int danger_sog;
	int min_sog;
	int max_sog;
	char nation;
} Alarm_Set;

typedef struct System_Set
{
	int workmode;
	int launch;
	int nightmode;
	int volum;
	int bright;
	int unit;
	int update;
	int reset;
} System_Set;
	

typedef struct T90_Set
{
   int motoas;
	Dst_Set dst;
	Alarm_Set alarm;
	System_Set sys;
} T90_Set;



typedef enum {
   PGEvent_None  = 0,
   PGEvent_Pull  = 1,
   PGEvent_Insert  = 2,
   PGEvent_Ack  = 4,
   PGEvent_Data  = 8
} PlugEventType;

typedef struct T90_PlugEvent
{
   unsigned char whichPort;
   unsigned char status;
   PlugEventType eventType;
   int dist_1;
   int dist_2;
   int dist_3;
   int dist_4;
   int dist_5;
   int dist_6;
   long mmsi[3];
}T90_PlugEvent;

typedef struct 
{
   uint16_t motoas;
   uint16_t motostub;
   uint16_t stubtostub;
}DoubleDstSet;

extern T90_Set t90_set;



extern boat mothership;
extern int MS_isSpeeding;
extern int MS_isMin_SOG;
extern int MS_isMax_SOG;



extern int screen_hcenter;
extern int screen_vcenter;




void T90_Init(void);
Bool T90_Load(void);
void T90_Store(void);
void T90_Reset(void);

DoubleDstSet* fetchdoudstset();
#endif







