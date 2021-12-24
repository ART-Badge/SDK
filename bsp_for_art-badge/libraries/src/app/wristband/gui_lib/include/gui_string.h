#ifndef _GUI_STRING_H_
#define _GUI_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
#include "rtl876x.h"
#include "wristband_gui.h"

uint8_t rtl_gui_show_string(UI_WidgetTypeDef *icon_font, int16_t detal_x, int16_t detal_y, int Zs,
                            int Ze, uint8_t *writebuf);

#ifdef __cplusplus
}
#endif

#endif /* _GUI_STRING_H_ */
