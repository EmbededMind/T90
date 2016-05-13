#ifndef _CONFIG_H
#define _CONFIG_H

#define P_AM128A



#define MOTHERSHIP_LA    1927265
#define MOTHERSHIP_LG    7128663



//#define DEBUG_LEVEL  0
//#define DEBUG_LEVEL DEBUG_LEVEL_ERROR
//#define DEBUG_LEVEL DEBUG_LEVEL_WARNING
//#define DEBUG_LEVEL DEBUG_LEVEL_WATCH
#define DEBUG_LEVEL DEBUG_LEVEL_TAG

#ifdef DEBUG_LEVEL_TAR
  #define DEBUG_LEVEL_WATCH
#endif

#ifdef DEBUG_LEVEL_WATCH
  #define DEBUG_LEVL_WARNING
#endif

#ifdef DEBUG_LEVEL_WARNING
  #define DEBUG_LEVEL_ERROR
#endif



#define VERSION  "V1.0.0"





/********************************************************************
 *
 *               timer counter config
 */
#define TIMESTAMP           60

#define MAP_REFRESH_SLOT    500
#define CURSOR_HIDE_CNT     200  



/********************************************************************
 *
 *              array size config
 */
#define MSG_QUEUE_TABNUM    20
#define BOAT_NUM_MAX        500
#define MNT_NUM_MAX         50
#define INVD_NUM_MAX        10
#define BULY_NUM_MAX         5

/********************************************************************
 *
 *              EEPROM address config
 */
#define SYSCONF_ADDR     0                        

#define MNT_PAGE_ID_HEADER  (uint8_t)1
#define MNT_PAGE_ID         (uint8_t)2       


 
#ifdef PRINT
#undef PRINT
#endif

#ifndef __PRINT_ENABLE
#define PRINT(format,...)
#else
#define PRINT(format,...) printf(""format"\r\n",##__VA_ARGS__)
#endif
 
/********************************************************************
 *
 *               printf config.
 */
#define __INFO_ENABLE 

#ifndef __INFO_ENABLE

#define INFO(format,...) 

#else

#define INFO(format,...)  printf("FILE:%s,LINE:%d,"format"\r\n",__FILE__,__LINE__,##__VA_ARGS__)

#endif


/********************************************************************
 *
 *               cursor config.
 */
#define CURSOR_SPEED   5


/********************************************************************
 *
 *               IC config.
 */
//#define SND_IC_XT


/********************************************************************
 *
 *               Boat type config.
 */
 #define TYPE_HSD     0x01
 #define TYPE_BULLY   0x02
 
 
 /********************************************************************
 *
 *               Nationality config.
 */
 #define NATION_NUM   6
 
#define NATION_CTB   0x10  /// Chinese Taiwan
#define NATION_JPN   0x20  /// Japan
#define NATION_KOR   0x30  /// Korea
#define NATION_PRK   0x40  /// People's Republic of Korea
#define NATION_INA   0x50  /// 
#define NATION_VIE   0x60  /// 
 
 /********************************************************************
*
*               High speed value.
*/
 #define HIGH_SPEED  80

#endif



