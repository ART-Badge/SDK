/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file
* @brief
* @details
* @author
* @date
* @version
*********************************************************************************************************
*/
#include "trace.h"
#include "string.h"
#include "os_timer.h"
#include "os_mem.h"
#include "os_sync.h"
#include "math.h"
#include <string.h>
#include <stdarg.h>
#include "wristband_gui.h"
#include "gui_core.h"
#include "app_section.h"
#include "rtl876x.h"
#include "flash_adv_cfg.h"
#include "app_msg.h"
#include "os_msg.h"
#include "gui_bmp.h"
#include "gui_graph.h"
#include "gui_string.h"
#include "gui_string_num.h"
#include "gui_rotation.h"
#include "gui_psram.h"
#include "gui_lcd_callback.h"
#include "hub_task.h"

#if (LCD_SECTION_HEIGHT * PIXEL_BYTES * 3 < 20)
#error "GUI CORE BUFFER LITTER THAN LINK LIST BUFFER!"
#endif

#if (LCD_SECTION_HEIGHT/LCD_HIGHT != 0)
#error "GUI LCD_SECTION_HEIGHT ERROR, WE MUST DIVIDE BY LCD_HIGHT!"
#endif


#define GUI_GLOBAL_WIDGET_MAX       29

UI_MenuTypeDef *pCurrentMenu = NULL;
GUI_ConfigDef rtl_gui_config = {false, false};
uint8_t __attribute__((aligned(4))) rtl_gui_core_buff[LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES *
                                                                3];
uint8_t *disp_write_buff1 = rtl_gui_core_buff;
uint8_t *disp_write_buff2 = NULL;
uint8_t *disp_read_buff = NULL;
uint8_t *dma_link_list_buff = rtl_gui_core_buff;

UI_MenuTypeDef *pShowMenu;
RtkGuiTimerInfo_t gui_timer_info[5];
bool lcd_screen_updating_flag = false;
GUI_FRAMEBUFFER_MODE FrameBufferMode = FRAMEBUFFER_MODE_NONE_PSRAM;

static UI_WidgetTypeDef  g_cur_widget[GUI_GLOBAL_WIDGET_MAX];
static UI_WidgetTypeDef  g_sub_widget[GUI_GLOBAL_WIDGET_MAX];
static UI_WidgetTypeDef  g_par_widget[GUI_GLOBAL_WIDGET_MAX];

UI_MenuTypeDef g_cur_menu =
{
    /* change Here for UI */
    .name = (const char *)"g_cur_menu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = NULL,
    .touch_func = NULL,
    .constructor_func = NULL,
    .destructor_func = NULL,
    .display_info_func = NULL,
    .timer_func = NULL,
    .pWidgetList = g_cur_widget,
    .current_max_icon = 0,
};
UI_MenuTypeDef g_sub_menu =
{
    /* change Here for UI */
    .name = (const char *)"g_sub_menu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = NULL,
    .touch_func = NULL,
    .constructor_func = NULL,
    .destructor_func = NULL,
    .display_info_func = NULL,
    .timer_func = NULL,
    .pWidgetList = g_sub_widget,
    .current_max_icon = 0,
};
UI_MenuTypeDef g_par_menu =
{
    /* change Here for UI */
    .name = (const char *)"g_par_menu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = NULL,
    .touch_func = NULL,
    .constructor_func = NULL,
    .destructor_func = NULL,
    .display_info_func = NULL,
    .timer_func = NULL,
    .pWidgetList = g_par_widget,
    .current_max_icon = 0,
};



void gui_init(void)
{
    disp_write_buff1 = rtl_gui_core_buff;
    disp_write_buff2 = disp_write_buff1 + LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES;
    disp_read_buff = disp_write_buff1 + LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES * 2;
#if FEATURE_PSRAM
    psram_frame_buffer_init();
#endif
}

void gui_set_te_function(bool flag)
{
    rtl_gui_config.te_enable = flag;
}


DATA_RAM_FUNCTION
void rtl_gui_refresh_by_dma_internal(uint8_t *readbuf, uint32_t count_for_section)
{
    uint32_t length = LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES;

    if (count_for_section % MAX_SECTION_COUNT == 0)
    {
        if (count_for_section)
        {
            rtl_gui_wait_lcd_control_transfer(LCD_WIDTH * LCD_SECTION_HEIGHT * MAX_SECTION_COUNT * PIXEL_BYTES);
        }

        uint16_t y_start = count_for_section * LCD_SECTION_HEIGHT;
        uint16_t y_end = count_for_section * LCD_SECTION_HEIGHT;

        if ((TOTAL_SECTION_COUNT - count_for_section) > MAX_SECTION_COUNT)
        {
            y_end += MAX_SECTION_COUNT * LCD_SECTION_HEIGHT - 1;
        }
        else
        {
            y_end += LCD_HIGHT - count_for_section * LCD_SECTION_HEIGHT - 1;

            if ((y_end - y_start + 1) < LCD_SECTION_HEIGHT)
            {
                length = (y_end - y_start + 1) * LCD_WIDTH * PIXEL_BYTES;
            }
        }
        rtl_gui_lcd_sectionconfig(0, y_start, LCD_WIDTH - 1, y_end);
        rtl_gui_dma_single_block_start(NULL, (uint32_t)readbuf, length);

        if (count_for_section == TOTAL_SECTION_COUNT - 1)
        {
            rtl_gui_wait_lcd_control_transfer((y_end - y_start + 1) * LCD_WIDTH * PIXEL_BYTES);
        }
    }
    else if (count_for_section == TOTAL_SECTION_COUNT - 1)
    {
        rtl_gui_wait_dma_transfer();

        if ((LCD_HIGHT - count_for_section * LCD_SECTION_HEIGHT) % LCD_SECTION_HEIGHT)
        {
            length = ((LCD_HIGHT - count_for_section * LCD_SECTION_HEIGHT) % LCD_SECTION_HEIGHT) *
                     LCD_WIDTH * PIXEL_BYTES;
        }
        rtl_gui_dma_single_block_start(NULL, (uint32_t)readbuf, length);

        uint32_t count = (LCD_HIGHT - (count_for_section / MAX_SECTION_COUNT) * MAX_SECTION_COUNT *
                          LCD_SECTION_HEIGHT) * LCD_WIDTH * PIXEL_BYTES;
        rtl_gui_wait_lcd_control_transfer(count);
    }
    else
    {
        rtl_gui_wait_dma_transfer();
        rtl_gui_dma_single_block_start(NULL, (uint32_t)readbuf, length);
    }

}


DATA_RAM_FUNCTION
static void rtl_gui_lcd_real_update_x(UI_MenuTypeDef *p_show_menu)
{
    /* cache buffer for dma source address */
    uint8_t *pWriteBuf = NULL;
    p_show_menu->MenuSub->detal_x = p_show_menu->detal_x + LCD_WIDTH;
    p_show_menu->MenuParent->detal_x = p_show_menu->detal_x - LCD_WIDTH;
    UI_MenuTypeDef cur_menu = *p_show_menu;
    UI_MenuTypeDef *local_cur_menu = &cur_menu;
    UI_WidgetTypeDef *local_cur_list = p_show_menu->pWidgetList;
    UI_MenuTypeDef sub_menu = *(p_show_menu->MenuSub);
    UI_MenuTypeDef *local_sub_menu = &sub_menu;
    UI_WidgetTypeDef *local_sub_list = p_show_menu->MenuSub->pWidgetList;
    UI_MenuTypeDef par_menu = *(p_show_menu->MenuParent);
    UI_MenuTypeDef *local_par_menu = &par_menu;
    UI_WidgetTypeDef *local_par_list = p_show_menu->MenuParent->pWidgetList;

    for (uint8_t i = 0; i < TOTAL_SECTION_COUNT; i++)
    {
        if (i % 2)
        {
            pWriteBuf = disp_write_buff2;
        }
        else
        {
            pWriteBuf = disp_write_buff1;
        }

        memset(pWriteBuf, 0x00, LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES);

        for (uint8_t j = 0; j < local_cur_menu->current_max_icon; j++)
        {
            if ((local_cur_list[j].y >= (i + 1)*LCD_SECTION_HEIGHT) ||
                ((local_cur_list[j].y + local_cur_list[j].hight) <= i * LCD_SECTION_HEIGHT))
            {
                continue;
            }

            if (local_cur_list[j].widget_id_type == ICON_WIDGET)
            {
                rtl_gui_show_bmp_sector(local_cur_list + j, \
                                        local_cur_menu->detal_x, 0, \
                                        i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, \
                                        pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_BACKGROUND)
            {
                if (local_cur_list[j].addr == 0)
                {
                    continue;
                }
                if (((local_cur_list[j].x + local_cur_menu->detal_x) >= 0) &&
                    ((local_cur_list[j].x + local_cur_menu->detal_x) <= LCD_WIDTH))
                {
                    for (uint8_t k = 0; k < LCD_SECTION_HEIGHT; k++)
                    {
                        memcpy(\
                               (uint8_t *)pWriteBuf + LCD_WIDTH * PIXEL_BYTES * k + PIXEL_BYTES * (local_cur_list[j].x +
                                                                                                   local_cur_menu->detal_x), \
                               (uint8_t *)(local_cur_list[j].addr + LCD_WIDTH * PIXEL_BYTES * k + LCD_WIDTH * i *
                                           LCD_SECTION_HEIGHT * PIXEL_BYTES), \
                               (LCD_WIDTH - (local_cur_list[j].x + local_cur_menu->detal_x)) * PIXEL_BYTES);
                    }
                }
                else if (((local_cur_list[j].x + local_cur_menu->detal_x) <= 0) &&
                         ((local_cur_list[j].x + local_cur_menu->detal_x) >= -LCD_WIDTH))
                {
                    for (uint8_t k = 0; k < LCD_SECTION_HEIGHT; k++)
                    {
                        memcpy(\
                               (uint8_t *)pWriteBuf + LCD_WIDTH * PIXEL_BYTES * k, \
                               (uint8_t *)(local_cur_list[j].addr + LCD_WIDTH * PIXEL_BYTES * k + LCD_WIDTH * i *
                                           LCD_SECTION_HEIGHT * PIXEL_BYTES  - PIXEL_BYTES
                                           *
                                           (local_cur_list[j].x + local_cur_menu->detal_x)), \
                               (LCD_WIDTH + (local_cur_list[j].x + local_cur_menu->detal_x)) * PIXEL_BYTES);
                    }
                }
            }
            else if (local_cur_list[j].widget_id_type == ICON_STRING)
            {
                rtl_gui_show_string(local_cur_list + j, \
                                    local_cur_menu->detal_x, 0, \
                                    i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_CLOCK_POINTER)
            {
                rtl_gui_show_clock_pointer(local_cur_list + j, local_cur_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                           (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_NUM)
            {
                rtl_gui_show_string_num(local_cur_list + j, local_cur_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                        (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_RECT)
            {
                rtl_gui_show_rect(local_cur_list + j, local_cur_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                  (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
        }

        if (local_sub_menu->detal_x < LCD_WIDTH)
        {
            for (uint8_t j = 0; j < local_sub_menu->current_max_icon; j++)
            {
                if ((local_sub_list[j].y >= (i + 1)*LCD_SECTION_HEIGHT) ||
                    ((local_sub_list[j].y + local_sub_list[j].hight) <= i * LCD_SECTION_HEIGHT))
                {
                    continue;
                }

                if (local_sub_list[j].widget_id_type == ICON_WIDGET)
                {
                    rtl_gui_show_bmp_sector(local_sub_list + j, \
                                            local_sub_menu->detal_x, 0, \
                                            i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, \
                                            pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_BACKGROUND)
                {
                    if (local_sub_list[j].addr == 0)
                    {
                        continue;
                    }
                    if (((local_sub_list[j].x + local_sub_menu->detal_x) >= 0) &&
                        ((local_sub_list[j].x + local_sub_menu->detal_x) <= LCD_WIDTH))
                    {
                        for (uint8_t k = 0; k < LCD_SECTION_HEIGHT; k++)
                        {
                            memcpy(\
                                   (uint8_t *)pWriteBuf + LCD_WIDTH * PIXEL_BYTES * k + PIXEL_BYTES * (local_sub_list[j].x +
                                                                                                       local_sub_menu->detal_x), \
                                   (uint8_t *)(local_sub_list[j].addr + LCD_WIDTH * PIXEL_BYTES * k + LCD_WIDTH * i *
                                               LCD_SECTION_HEIGHT * PIXEL_BYTES), \
                                   (LCD_WIDTH - (local_sub_list[j].x + local_sub_menu->detal_x)) * PIXEL_BYTES);
                        }
                    }
                    else if (((local_sub_list[j].x + local_sub_menu->detal_x) <= 0) &&
                             ((local_sub_list[j].x + local_sub_menu->detal_x) >= -LCD_WIDTH))
                    {
                        for (uint8_t k = 0; k < LCD_SECTION_HEIGHT; k++)
                        {
                            memcpy(\
                                   (uint8_t *)pWriteBuf + LCD_WIDTH * PIXEL_BYTES * k, \
                                   (uint8_t *)(local_sub_list[j].addr + LCD_WIDTH * PIXEL_BYTES * k + LCD_WIDTH * i *
                                               LCD_SECTION_HEIGHT * PIXEL_BYTES  - PIXEL_BYTES
                                               *
                                               (local_sub_list[j].x + local_sub_menu->detal_x)), \
                                   (LCD_WIDTH + (local_sub_list[j].x + local_sub_menu->detal_x)) * PIXEL_BYTES);
                        }
                    }
                }
                else if (local_sub_list[j].widget_id_type == ICON_STRING)
                {
                    rtl_gui_show_string(local_sub_list + j, \
                                        local_sub_menu->detal_x, 0, \
                                        i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_CLOCK_POINTER)
                {
                    rtl_gui_show_clock_pointer(local_sub_list + j, local_sub_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                               (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_NUM)
                {
                    rtl_gui_show_string_num(local_sub_list + j, local_sub_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                            (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_RECT)
                {
                    rtl_gui_show_rect(local_sub_list + j, local_sub_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                      (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
            }
        }

        if (local_par_menu->detal_x > -LCD_WIDTH)
        {
            for (uint8_t j = 0; j < local_par_menu->current_max_icon; j++)
            {
                if ((local_par_list[j].y >= (i + 1)*LCD_SECTION_HEIGHT) ||
                    ((local_par_list[j].y + local_par_list[j].hight) <= i * LCD_SECTION_HEIGHT))
                {
                    continue;
                }
                if (local_par_list[j].widget_id_type == ICON_WIDGET)
                {
                    rtl_gui_show_bmp_sector(local_par_list + j, \
                                            local_par_menu->detal_x, 0, \
                                            i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, \
                                            pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_BACKGROUND)
                {
                    if (local_par_list[j].addr == 0)
                    {
                        continue;
                    }
                    if (((local_par_list[j].x + local_par_menu->detal_x) >= 0) &&
                        ((local_par_list[j].x + local_par_menu->detal_x) <= LCD_WIDTH))
                    {
                        for (uint8_t k = 0; k < LCD_SECTION_HEIGHT; k++)
                        {
                            memcpy(\
                                   (uint8_t *)pWriteBuf + LCD_WIDTH * PIXEL_BYTES * k + PIXEL_BYTES * (local_par_list[j].x +
                                                                                                       local_par_menu->detal_x), \
                                   (uint8_t *)(local_par_list[j].addr + LCD_WIDTH * PIXEL_BYTES * k + LCD_WIDTH * i *
                                               LCD_SECTION_HEIGHT * PIXEL_BYTES), \
                                   (LCD_WIDTH - (local_par_list[j].x + local_par_menu->detal_x)) * PIXEL_BYTES);
                        }
                    }
                    else if (((local_par_list[j].x + local_par_menu->detal_x) <= 0) &&
                             ((local_par_list[j].x + local_par_menu->detal_x) >= -LCD_WIDTH))
                    {
                        for (uint8_t k = 0; k < LCD_SECTION_HEIGHT; k++)
                        {
                            memcpy(\
                                   (uint8_t *)pWriteBuf + LCD_WIDTH * PIXEL_BYTES * k, \
                                   (uint8_t *)(local_par_list[j].addr + LCD_WIDTH * PIXEL_BYTES * k + LCD_WIDTH * i *
                                               LCD_SECTION_HEIGHT * PIXEL_BYTES  - PIXEL_BYTES
                                               *
                                               (local_par_list[j].x + local_par_menu->detal_x)), \
                                   (LCD_WIDTH + (local_par_list[j].x + local_par_menu->detal_x)) * PIXEL_BYTES);
                        }
                    }
                }
                else if (local_par_list[j].widget_id_type == ICON_STRING)
                {
                    rtl_gui_show_string(local_par_list + j, \
                                        local_par_menu->detal_x, 0, \
                                        i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_CLOCK_POINTER)
                {
                    rtl_gui_show_clock_pointer(local_par_list + j, local_par_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                               (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_NUM)
                {
                    rtl_gui_show_string_num(local_par_list + j, local_par_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                            (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_RECT)
                {
                    rtl_gui_show_rect(local_par_list + j, local_par_menu->detal_x, 0, i * LCD_SECTION_HEIGHT,
                                      (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
            }
        }
        rtl_gui_refresh_by_dma_internal(pWriteBuf, i);
    }
}

DATA_RAM_FUNCTION
static void rtl_gui_lcd_real_update_y(UI_MenuTypeDef *p_show_menu)
{
    /* cache buffer for dma source address */
    uint8_t *pWriteBuf = NULL;
    p_show_menu->MenuSub->detal_y = p_show_menu->detal_y + LCD_HIGHT;
    p_show_menu->MenuParent->detal_y = p_show_menu->detal_y - LCD_HIGHT;
    UI_MenuTypeDef cur_menu = *p_show_menu;
    UI_MenuTypeDef *local_cur_menu = &cur_menu;
    UI_WidgetTypeDef *local_cur_list = p_show_menu->pWidgetList;
    UI_MenuTypeDef sub_menu = *(p_show_menu->MenuSub);
    UI_MenuTypeDef *local_sub_menu = &sub_menu;
    UI_WidgetTypeDef *local_sub_list = p_show_menu->MenuSub->pWidgetList;
    UI_MenuTypeDef par_menu = *(p_show_menu->MenuParent);
    UI_MenuTypeDef *local_par_menu = &par_menu;
    UI_WidgetTypeDef *local_par_list = p_show_menu->MenuParent->pWidgetList;

    for (uint8_t i = 0; i < TOTAL_SECTION_COUNT; i++)
    {
        if (i % 2)
        {
            pWriteBuf = disp_write_buff2;
        }
        else
        {
            pWriteBuf = disp_write_buff1;
        }

        memset(pWriteBuf, 0x00, LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES);

        for (uint8_t j = 0; j < local_cur_menu->current_max_icon; j++)
        {
            if ((local_cur_list[j].y + local_cur_menu->detal_y >= (i + 1)*LCD_SECTION_HEIGHT) ||
                ((local_cur_list[j].y + local_cur_menu->detal_y + local_cur_list[j].hight) <= i *
                 LCD_SECTION_HEIGHT))
            {
                continue;
            }
            if (local_cur_list[j].widget_id_type == ICON_WIDGET)
            {
                rtl_gui_show_bmp_sector(local_cur_list + j, \
                                        0, local_cur_menu->detal_y, \
                                        i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, \
                                        pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_BACKGROUND)
            {
                if (local_cur_list[j].addr == 0)
                {
                    continue;
                }

                if ((local_cur_menu->detal_y >= (i + 1)*LCD_SECTION_HEIGHT) || \
                    (local_cur_menu->detal_y + local_cur_list[j].hight < i * LCD_SECTION_HEIGHT))
                {
                    continue;
                }
                if (local_cur_menu->detal_y >= i * LCD_SECTION_HEIGHT)
                {
                    memcpy((uint8_t *)pWriteBuf + (local_cur_menu->detal_y - i * LCD_SECTION_HEIGHT) * LCD_WIDTH *
                           PIXEL_BYTES, (uint8_t *)local_cur_list[j].addr, \
                           ((i + 1) * LCD_SECTION_HEIGHT - local_cur_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES);
                }
                else if (local_cur_menu->detal_y + local_cur_list[j].hight < (i + 1)*LCD_SECTION_HEIGHT)
                {
                    memcpy((uint8_t *)pWriteBuf, (uint8_t *)local_cur_list[j].addr + (i * LCD_SECTION_HEIGHT -
                                                                                      local_cur_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES, \
                           (local_cur_list[j].hight + local_cur_menu->detal_y - i * LCD_SECTION_HEIGHT) * LCD_WIDTH *
                           PIXEL_BYTES);
                }
                else
                {
                    memcpy((uint8_t *)pWriteBuf, (uint8_t *)local_cur_list[j].addr + (i * LCD_SECTION_HEIGHT -
                                                                                      local_cur_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES, \
                           LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES);
                }
            }
            else if (local_cur_list[j].widget_id_type == ICON_STRING)
            {
                rtl_gui_show_string(local_cur_list + j, \
                                    0, local_cur_menu->detal_y, \
                                    i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_CLOCK_POINTER)
            {
                rtl_gui_show_clock_pointer(local_cur_list + j, 0, local_cur_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                           (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_NUM)
            {
                rtl_gui_show_string_num(local_cur_list + j, 0, local_cur_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                        (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_RECT)
            {
                rtl_gui_show_rect(local_cur_list + j, 0, local_cur_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                  (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
        }

        if (local_sub_menu->detal_y < LCD_HIGHT)
        {
            for (uint8_t j = 0; j < local_sub_menu->current_max_icon; j++)
            {
                if ((local_sub_list[j].y + local_sub_menu->detal_y >= (i + 1)*LCD_SECTION_HEIGHT) ||
                    ((local_sub_list[j].y + local_sub_menu->detal_y + local_sub_list[j].hight) <= i *
                     LCD_SECTION_HEIGHT))
                {
                    continue;
                }
                if (local_sub_list[j].widget_id_type == ICON_WIDGET)
                {
                    rtl_gui_show_bmp_sector(local_sub_list + j, \
                                            0, local_sub_menu->detal_y, \
                                            i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, \
                                            pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_BACKGROUND)
                {
                    if (local_sub_list[j].addr == 0)
                    {
                        continue;
                    }
                    if ((local_sub_menu->detal_y >= (i + 1)*LCD_SECTION_HEIGHT) || \
                        (local_sub_menu->detal_y + local_sub_list[j].hight < i * LCD_SECTION_HEIGHT))
                    {
                        continue;
                    }
                    if (local_sub_menu->detal_y >= i * LCD_SECTION_HEIGHT)
                    {
                        memcpy((uint8_t *)pWriteBuf + (local_sub_menu->detal_y - i * LCD_SECTION_HEIGHT) * LCD_WIDTH *
                               PIXEL_BYTES, (uint8_t *)local_sub_list[j].addr, \
                               ((i + 1) * LCD_SECTION_HEIGHT - local_sub_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES);
                    }
                    else if (local_sub_menu->detal_y + local_sub_list[j].hight < (i + 1) * LCD_SECTION_HEIGHT)
                    {
                        memcpy((uint8_t *)pWriteBuf, (uint8_t *)local_sub_list[j].addr + (i * LCD_SECTION_HEIGHT -
                                                                                          local_sub_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES, \
                               (local_sub_list[j].hight + local_sub_menu->detal_y - i * LCD_SECTION_HEIGHT) * LCD_WIDTH *
                               PIXEL_BYTES);
                    }
                    else
                    {
                        memcpy((uint8_t *)pWriteBuf, (uint8_t *)local_sub_list[j].addr + (i * LCD_SECTION_HEIGHT -
                                                                                          local_sub_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES, \
                               LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES);
                    }
                }
                else if (local_sub_list[j].widget_id_type == ICON_STRING)
                {
                    rtl_gui_show_string(local_sub_list + j, \
                                        0, local_sub_menu->detal_y, \
                                        i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_CLOCK_POINTER)
                {
                    rtl_gui_show_clock_pointer(local_sub_list + j, 0, local_sub_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                               (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_NUM)
                {
                    rtl_gui_show_string_num(local_sub_list + j, 0, local_sub_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                            (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_sub_list[j].widget_id_type == ICON_RECT)
                {
                    rtl_gui_show_rect(local_sub_list + j, 0, local_sub_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                      (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
            }
        }

        if (local_par_menu->detal_y > -LCD_HIGHT)
        {
            for (uint8_t j = 0; j < local_par_menu->current_max_icon; j++)
            {
                if ((local_par_list[j].y + local_par_menu->detal_y >= (i + 1)*LCD_SECTION_HEIGHT) ||
                    ((local_par_list[j].y + local_par_menu->detal_y + local_par_list[j].hight) <= i *
                     LCD_SECTION_HEIGHT))
                {
                    continue;
                }
                if (local_par_list[j].widget_id_type == ICON_WIDGET)
                {
                    rtl_gui_show_bmp_sector(local_par_list + j, \
                                            0, local_par_menu->detal_y, \
                                            i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, \
                                            pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_BACKGROUND)
                {
                    if (local_par_list[j].addr == 0)
                    {
                        continue;
                    }
                    if ((local_par_menu->detal_y >= (i + 1)*LCD_SECTION_HEIGHT) || \
                        (local_par_menu->detal_y + local_par_list[j].hight < i * LCD_SECTION_HEIGHT))
                    {
                        continue;
                    }
                    if (local_par_menu->detal_y >= i * LCD_SECTION_HEIGHT)
                    {
                        memcpy((uint8_t *)pWriteBuf + (local_par_menu->detal_y - i * LCD_SECTION_HEIGHT) * LCD_WIDTH *
                               PIXEL_BYTES, (uint8_t *)local_par_list[j].addr, \
                               ((i + 1) * LCD_SECTION_HEIGHT - local_par_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES);
                    }
                    else if (local_par_menu->detal_y + local_par_list[j].hight < (i + 1)*LCD_SECTION_HEIGHT)
                    {
                        memcpy((uint8_t *)pWriteBuf, (uint8_t *)local_par_list[j].addr + (i * LCD_SECTION_HEIGHT -
                                                                                          local_par_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES, \
                               (local_par_list[j].hight + local_par_menu->detal_y - i * LCD_SECTION_HEIGHT) * LCD_WIDTH *
                               PIXEL_BYTES);
                    }
                    else
                    {
                        memcpy((uint8_t *)pWriteBuf, (uint8_t *)local_par_list[j].addr + (i * LCD_SECTION_HEIGHT -
                                                                                          local_par_menu->detal_y) * LCD_WIDTH * PIXEL_BYTES, \
                               LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES);
                    }
                }
                else if (local_par_list[j].widget_id_type == ICON_STRING)
                {
                    rtl_gui_show_string(local_par_list + j, \
                                        0, local_par_menu->detal_y, \
                                        i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_CLOCK_POINTER)
                {
                    rtl_gui_show_clock_pointer(local_par_list + j, 0, local_par_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                               (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_NUM)
                {
                    rtl_gui_show_string_num(local_par_list + j, 0, local_par_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                            (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
                else if (local_par_list[j].widget_id_type == ICON_RECT)
                {
                    rtl_gui_show_rect(local_par_list + j, 0, local_par_menu->detal_y, i * LCD_SECTION_HEIGHT,
                                      (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
                }
            }
        }
        rtl_gui_refresh_by_dma_internal(pWriteBuf, i);
    }
}

DATA_RAM_FUNCTION
void rtl_gui_lcd_real_update_from_internal_ram(UI_MenuTypeDef *p_show_menu)
{
    if (p_show_menu == NULL)
    {
        return;
    }
    rtl_gui_dma_single_block_init(0x00000001);

    flash_ioctl(flash_ioctl_set_seq_trans_enable, 1, 0);

    if (p_show_menu->detal_y == 0)
    {
        rtl_gui_lcd_real_update_x(p_show_menu);
    }
    else
    {
        rtl_gui_lcd_real_update_y(p_show_menu);
    }

    flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);

}



void rtl_gui_lcd_update(void)
{
    uint32_t old_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
    lcd_screen_updating_flag = true;
    if (rtl_gui_config.te_enable == true)
    {
        rtl_gui_lcd_te_disable();
    }


    if (FrameBufferMode == FRAMEBUFFER_MODE_NONE_PSRAM)
    {
        rtl_gui_lcd_real_update_from_internal_ram(pShowMenu);
    }
#if FEATURE_PSRAM
    else
    {
        rtl_gui_lcd_update_from_psram();
    }
#endif

    lcd_screen_updating_flag = false;
    uint32_t new_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
#if 1
    extern uint32_t SystemCpuClock;
    APP_PRINT_INFO2("[rtl_gui_lcd_update SystemCpuClock = %dM; t=%ld ms]", \
                    SystemCpuClock / 1000000, ((new_stamp - old_stamp) * 1000) / SystemCpuClock);
#endif
}

DATA_RAM_FUNCTION
void rtl_gui_menu_update(UI_MenuTypeDef *ui_cur, void *p_ui_cur_argv, \
                         UI_MenuTypeDef *ui_sub, void *p_ui_sub_argv, \
                         UI_MenuTypeDef *ui_par, void *p_ui_par_argv, \
                         int16_t detal_x, int16_t detal_y)
{
    FrameBufferMode = FRAMEBUFFER_MODE_NONE_PSRAM;
    if (ui_cur != NULL)
    {
        ui_cur->display_info_func(&g_cur_menu, p_ui_cur_argv);
    }
    else
    {
        g_cur_menu.current_max_icon = 0;
    }
    if (ui_sub != NULL)
    {
        ui_sub->display_info_func(&g_sub_menu, p_ui_sub_argv);
    }
    else
    {
        g_sub_menu.current_max_icon = 0;
    }
    if (ui_par != NULL)
    {
        ui_par->display_info_func(&g_par_menu, p_ui_par_argv);
    }
    else
    {
        g_par_menu.current_max_icon = 0;
    }
    g_cur_menu.detal_x = detal_x;
    if (detal_x)
    {
        g_cur_menu.detal_y = 0;
    }
    else
    {
        g_cur_menu.detal_y = detal_y;
    }
    pShowMenu = &g_cur_menu;
    pShowMenu->MenuSub = &g_sub_menu;
    pShowMenu->MenuParent = &g_par_menu;
    if (rtl_gui_config.te_enable == true)
    {
        rtl_gui_lcd_te_enable();
    }
    else
    {
        rtl_gui_lcd_update();
    }
}


DATA_RAM_FUNCTION
void rtl_gui_prepare_frame_buffer(UI_MenuTypeDef *ui_cur, void *p_ui_cur_argv,
                                  UI_PSRAMBufferTypeDef *location, bool need_update_framebuffer)
{
    FrameBufferMode = FRAMEBUFFER_MODE_PSRAM;
    lcd_screen_updating_flag = true;
    if (need_update_framebuffer == true)
    {
        ui_cur->display_info_func(&g_cur_menu, p_ui_cur_argv);
        pShowMenu = &g_cur_menu;
        rtl_gui_update_to_psram_framebuffer(pShowMenu, location);
    }
    lcd_screen_updating_flag = false;
}

DATA_RAM_FUNCTION
void rtl_gui_update_detal(int16_t x, int16_t y) //this api only for psram mode
{
    psram_detal_x = x;
    psram_detal_y = y;

    if (rtl_gui_config.te_enable == true)
    {
        rtl_gui_lcd_te_enable();
    }
    else
    {
        rtl_gui_lcd_update();
    }
}


void rtl_gui_timer_callback(void *pxTimer)
{
    extern bool app_send_msg_to_apptask(T_IO_MSG * p_msg);
    uint32_t timer_id;
    static uint8_t cnt  = 0;
    os_timer_id_get(&pxTimer, &timer_id);
    T_IO_MSG msg;
    msg.type = HUB_MSG_LCD;
    msg.subtype = LCD_MSG_TIMER_MENU;
    if (++cnt == 5)
    {
        cnt = 0;
    }
    if (timer_id == 1)
    {
        gui_timer_info[cnt].timer_id = timer_id;
    }
    else if (timer_id == 2)
    {
        gui_timer_info[cnt].timer_id = timer_id;
    }
    else if (timer_id == 3)
    {
        gui_timer_info[cnt].timer_id = timer_id;
    }
    gui_timer_info[cnt].menu_name = (char *)pCurrentMenu->name;
    msg.u.buf = (void *)&gui_timer_info[cnt];
    send_msg_to_ui_task(&msg, __LINE__);
}

