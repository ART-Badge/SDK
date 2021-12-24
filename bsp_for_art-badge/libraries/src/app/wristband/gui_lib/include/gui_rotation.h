#ifndef _GUI_ROTATION_H_
#define _GUI_ROTATION_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
#include "rtl876x.h"
#include "wristband_gui.h"

typedef union
{
    uint16_t pixel;
    struct
    {
        uint16_t b : 5;
        uint16_t g : 6;
        uint16_t r : 5;
    };

} RGB565;

typedef union
{
    uint32_t pixel;
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t pad;
    };

} RGB888;

uint8_t rtl_gui_show_clock_pointer(UI_WidgetTypeDef *icon_clock, int16_t detal_x, int16_t detal_y,
                                   int Zs, int Ze, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* _GUI_ROTATION_H_ */
