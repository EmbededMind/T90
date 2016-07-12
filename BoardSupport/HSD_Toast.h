#ifndef _HSD_TOAST_H
#define _HSD_TOAST_H

#include <lpc_types.h>
#include <GUI.h>

#define TOAST_OK              0x01  /**< Toast附带确定按钮 */
#define TOAST_CANCEL          0x02  /**< Toast附带取消按钮 */

/** @Warning 当Toast带有按钮时，下面的设置无效 */
#define TOAST_LONG            2000  /**< Toast维持较长时间  */
#define TOAST_SHORT           1000  /**< Toast维持较短时间  */


void ToastCreate(const char* text, const GUI_FONT GUI_UNI_PTR* pFont ,uint8_t btnFlags, uint16_t liveTime);

#endif
