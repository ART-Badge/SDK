#ifndef _GUI_GRAPH_H_
#define _GUI_GRAPH_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
#include "rtl876x.h"
#include "wristband_gui.h"

uint8_t rtl_gui_show_rect(UI_WidgetTypeDef *widget, int16_t detal_x, int16_t detal_y, int Zs,
                          int Ze, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* _GUI_GRAPH_H_ */
