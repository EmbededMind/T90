#ifndef _HSD_TOAST_H
#define _HSD_TOAST_H

#include <lpc_types.h>
#include <GUI.h>

#define TOAST_OK              0x01  /**< Toast����ȷ����ť */
#define TOAST_CANCEL          0x02  /**< Toast����ȡ����ť */

/** @Warning ��Toast���а�ťʱ�������������Ч */
#define TOAST_LONG            2000  /**< Toastά�ֽϳ�ʱ��  */
#define TOAST_SHORT           1000  /**< Toastά�ֽ϶�ʱ��  */


void ToastCreate(const char* text, const GUI_FONT GUI_UNI_PTR* pFont ,uint8_t btnFlags, uint16_t liveTime);

#endif
