#ifndef _DLG_H
#define _DLG_H
#include "WM.h"
#include "Config.h"
#include "HSD_STICKFIGURE.h"


#ifdef P_AM128A
#define SUBWIN_NUM  5
#else
#define SUBWIN_NUM  4
#endif

extern WM_HWIN confirmWin;



/** T90 **/
extern WM_HWIN workModeWin;

extern WM_HWIN mainShipWin;
//extern WM_HWIN doubleShipWin;
extern WM_HWIN alarmMonitorWin;

extern WM_HWIN mainMenuDlg;
extern WM_HWIN dstSetMenuDlg;
extern WM_HWIN alarmSetMenuDlg;
extern WM_HWIN systemSetDlg;
extern WM_HWIN myFleetDlg;

extern WM_HWIN singleShipDstSetWin;
extern WM_HWIN doubleShipDstSetWin;

extern WM_HWIN invdAlarmSetWin;
extern WM_HWIN spdingAlarmSetWin;
extern WM_HWIN sogAlarmSetWin;
extern WM_HWIN specialAlarmSetDlg;
extern WM_HWIN highspshipsetWin;

extern WM_HWIN confirmWin;
//extern WM_HWIN textprowin;

WM_HWIN WIN_WorkModeCreate(void);

WM_HWIN WIN_SingleShipCreate(void);
WM_HWIN WIN_DoubleShipCreate(void);
WM_HWIN WIN_AlarmMonitorCreate(void);
//WM_HWIN WIN_textproCreate(void);
WM_HWIN DLG_MainMenuCreate(void);

WM_HWIN DLG_DstSetMenuCreate(void);
WM_HWIN DLG_AlarmSetMenuCreate(void);


WM_HWIN WIN_SingleShipDstSetCreate(void);
WM_HWIN WIN_doubleShipDstSetCreate(void);
WM_HWIN DLG_SystemSetCreate(void);
WM_HWIN DLG_MyFleetSetMenuCreate(void);

WM_HWIN WIN_InvdAlarmSetCreate(void);
WM_HWIN WIN_SpdingAlarmSetCreate(void);
WM_HWIN WIN_SogAlarmSetCreate(void);
WM_HWIN DLG_SpecialAlarmSetCreate(void);
WM_HWIN WIN_HighSpeedShipSetCreate(void);
WM_HWIN WIN_ConfirmCreate(void);

//WM_HWIN DLG_testCustomedWidgetCreate(void);

#ifdef P_AM128A
WM_HWIN sub4WinCreate(void);
#endif

#endif

