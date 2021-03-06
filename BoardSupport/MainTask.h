#ifndef MAINTASK_H
#define MAINTASK_H

#include "WM.h"

#define USER_MSG           (WM_USER + 0x00)
#define USER_MSG_CHOOSE    (WM_USER + 0x01)
#define USER_MSG_REPLY     (WM_USER + 0x02)
#define USER_MSG_SKIN      (WM_USER + 0x03)
#define USER_MSG_SHAPE     (WM_USER + 0x04)
#define USER_MSG_UNIT      (WM_USER + 0x05)
//#define USER_MSG_BRING     (WM_USER + 0x06)
#define USER_MSG_LV_MOVE   (WM_USER + 0x06)
#define USER_MSG_DFULT_CNT (WM_USER + 0x07)
#define USER_MSG_DIM       (WM_USER + 0x08)
#define USER_MSG_LV_UPDATE (WM_USER + 0x09)
#define USER_MSG_FOCUS     (WM_USER + 0x10)
#define USER_MSG_MNT_SWT   (WM_USER + 0x11)
#define USER_MSG_DST_SET   (WM_USER + 0x12)
#define USER_MSG_DST_UPDATE (WM_USER + 0x13)
#define USER_MSG_PULSE_ACK_RESULT  (WM_USER + 0x14)
#define USER_MSG_DATA_ACK_RESULT   (WM_USER + 0x15)
#define USER_MSG_WORKMODE  (WM_USER + 0x16)
#define USER_MSG_MOTHERPOS (WM_USER + 0x17)
#define USER_MSG_MMSISET   (WM_USER + 0x19)


#define STORE_SETTING   1
#define CANCEL_MONITED  2
#define REPLY_OK        3
#define REPLY_CANCEL    4
#define ADD_MONITED     5
#define SYS_SETTING     6
#define SYS_RESET       7
#define MONITMMSI_SET   8
#define MONITMMSI_ADD   9
#define MONITMMSI_DEL   10
#define MONITMMSI_FULL  11
#define MONITMMSI_NINE  12
#define MONITMMSI_FIRST 13
//#define UPDATA          14

#define GUI_KEY_RELEASE           4
#define GUI_KEY_MENU              5
#define GUI_KEY_PWM_INC           6
#define GUI_KEY_SOUNDOFF          7    //single mode
#define GUI_KEY_BACKSPACE         8
#define GUI_KEY_MORIGHT           14   //double mode
#define GUI_KEY_LEFT              16
#define GUI_KEY_UP                17
#define GUI_KEY_RIGHT             18
#define GUI_KEY_DOWN              19
#define GUI_KEY_F1                11
#define GUI_KEY_F2                12
#define GUI_KEY_F3                13
#define GUI_KEY_TRACE_ENABLE      28
#define GUI_KEY_TRACE_DISABLE     31

#define PULSE_ACK_OK       1
#define PULSE_ACK_TIME_OUT 0
#define DATA_ACK_OK        1
#define DATA_ACK_TIME_OUT  0


#endif
