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

extern WM_HWIN mapWin;
extern WM_HWIN menuWin;

extern WM_HWIN subWins[SUBWIN_NUM];

extern WM_HWIN confirmWin;
extern WM_HWIN mntSettingWin;
extern WM_HWIN _mntSettingWin;
extern WM_HWIN _sldWin;

//extern WM_HWIN menuDlg;

WM_HWIN mapWinCreate(void);
WM_HWIN menuWinCreate(void);
WM_HWIN confirmWinCreate(void);
WM_HWIN mntSettingWinCreate(void);
WM_HWIN _mntSettingWinCreate(void);
WM_HWIN sub0WinCreate(void);
WM_HWIN sub1WinCreate(void);
WM_HWIN sub2WinCreate(void);
WM_HWIN sub3WinCreate(void);


/** T90 **/
extern WM_HWIN workModeWin;

extern WM_HWIN singleShipWin;
extern WM_HWIN doubleShipWin;
extern WM_HWIN alarmMonitorWin;

extern WM_HWIN mainMenuDlg;
extern WM_HWIN dstSetMenuDlg;
extern WM_HWIN alarmSetMenuDlg;
extern WM_HWIN systemSetDlg;
extern WM_HWIN singleShipDstSetWin;

extern WM_HWIN invdAlarmSetWin;
extern WM_HWIN spdingAlarmSetWin;
extern WM_HWIN sogAlarmSetWin;
extern WM_HWIN specialAlarmSetDlg;

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
WM_HWIN DLG_SystemSetCreate(void);

WM_HWIN WIN_InvdAlarmSetCreate(void);
WM_HWIN WIN_SpdingAlarmSetCreate(void);
WM_HWIN WIN_SogAlarmSetCreate(void);
WM_HWIN DLG_SpecialAlarmSetCreate(void);

WM_HWIN WIN_ConfirmCreate(void);

//WM_HWIN DLG_testCustomedWidgetCreate(void);

#ifdef P_AM128A
WM_HWIN sub4WinCreate(void);
#endif

#endif

