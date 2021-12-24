#ifndef _GUI_CORE_H_
#define _GUI_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
#include "rtl876x.h"
#include "touch_algorithm.h"

typedef enum
{
    FRAMEBUFFER_MODE_NONE_PSRAM = 0,
    FRAMEBUFFER_MODE_PSRAM,
} GUI_FRAMEBUFFER_MODE;

typedef struct _UI_StringRecordTypeDef
{
    uint16_t single_font_height;
    uint16_t current_number;
    uint16_t total_count;
    bool     new_font;
    uint32_t font_color;
    uint32_t font_string_addr;
} UI_StringRecordTypeDef;

typedef struct _UI_ClockPointerTypeDef
{
    int16_t center_x;
    int16_t center_y;
    uint16_t source_pic_width;
    uint16_t source_pic_height;
    uint16_t source_pic_center_height;
    float   rotation_cosa;
    float   rotation_sina;
} UI_ClockPointerTypeDef;


typedef struct
{
    uint32_t timer_id;
    char     *menu_name;
} RtkGuiTimerInfo_t;

typedef struct _UI_WidgetTypeDef
{
    int16_t x;
    int16_t y;
    //int16_t touch_x;
    //int16_t touch_y;
    int16_t active_ys;
    int16_t active_ye;
    int16_t width;
    int16_t hight;
    uint32_t addr;
    //uint32_t mask_addr;
    //uint32_t color;
    //uint32_t string;
    //UI_ClockPointerTypeDef clock_pointer;
    //UI_StringRecordTypeDef string_mem;
    uint16_t widget_id_type;
    union
    {
        UI_ClockPointerTypeDef clock_pointer;
        UI_StringRecordTypeDef string_mem;
        uint32_t mask_addr;
        uint32_t graph_color;
        uint32_t num_string_addr;
    } u;
    bool touch_capability;
    void (*widget_touch_func)(uint32_t button_touch_type);
} UI_WidgetTypeDef;

typedef struct _UI_PSRAMBufferTypeDef
{
    const char *name;
    struct _UI_MenuTypeDef *Menu;
    uint8_t *pLCDBuffer;
    bool BufferReadFlag;
} UI_PSRAMBufferTypeDef;

typedef struct _UI_MenuTypeDef
{
    const char *name;
    struct _UI_MenuTypeDef *MenuParent;
    struct _UI_MenuTypeDef *MenuSub;
    void (*button_func)(uint8_t type);
    void (*touch_func)(RtkTouchInfo_t *info);
    void (*constructor_func)(uint32_t type, void *pValue);
    void (*destructor_func)(void);
    void (*display_info_func)(struct _UI_MenuTypeDef *return_menu, void *argv);
    void (*timer_func)(RtkGuiTimerInfo_t *timer_info);
    UI_WidgetTypeDef *pWidgetList;
    uint8_t current_max_icon;
    int16_t detal_x;
    int16_t detal_y;
    bool dynamicFlag;
} UI_MenuTypeDef;

typedef enum
{
    ICON_BACKGROUND,
    ICON_WIDGET,
    ICON_STRING,
    ICON_CLOCK_POINTER,
    ICON_NUM,
    ICON_MASK,
    ICON_RECT,
} ICON_TYPE;

typedef enum
{
    ICON_LEFT,
    ICON_RIGHT,
    ICON_UP,
    ICON_DOWN,
    ICON_MIDDLE
} CONSTRUCT_TYPE;



typedef struct
{
    bool        te_enable;
    bool        bak;
} GUI_ConfigDef;

extern bool lcd_screen_updating_flag;
extern UI_MenuTypeDef *pCurrentMenu;
extern GUI_FRAMEBUFFER_MODE FrameBufferMode;

void gui_set_te_function(bool flag);
void gui_init(void);
void rtl_gui_lcd_update(void);
void rtl_gui_timer_callback(void *pxTimer);
void rtl_gui_menu_update(UI_MenuTypeDef *ui_cur, void *p_ui_cur_argv, \
                         UI_MenuTypeDef *ui_sub, void *p_ui_sub_argv, \
                         UI_MenuTypeDef *ui_par, void *p_ui_par_argv, \
                         int16_t detal_x, int16_t delta_y);
void rtl_gui_prepare_frame_buffer(UI_MenuTypeDef *ui_cur, void *p_ui_cur_argv,
                                  UI_PSRAMBufferTypeDef *location, bool need_update_framebuffer);
void rtl_gui_update_detal(int16_t x, int16_t y);

extern uint8_t *disp_write_buff1;
extern uint8_t *disp_write_buff2;
extern uint8_t *disp_read_buff;
extern uint8_t *dma_link_list_buff;

#ifdef __cplusplus
}
#endif

#endif /* _GUI_CORE_H_ */
