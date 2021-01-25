#ifndef _GUI_SLIDE_H_
#define _GUI_SLIDE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
#include "rtl876x.h"
#include "wristband_gui.h"

UI_MenuTypeDef *rtl_gui_menu_real_slide(void);
void rtl_gui_menu_slide(UI_MenuTypeDef *ui_cur, void *p_ui_cur_argv, \
                        UI_MenuTypeDef *ui_sub, void *p_ui_sub_argv, \
                        UI_MenuTypeDef *ui_par, void *p_ui_par_argv, \
                        int16_t from_x, int16_t to_x, \
                        int16_t from_y, int16_t to_y);


#ifdef __cplusplus
}
#endif

#endif /* _GUI_SLIDE_H_ */
