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
#include "app_section.h"
#include "rtl876x.h"
#include "flash_adv_cfg.h"
#include "app_msg.h"
#include "os_msg.h"
#include "gui_bmp.h"
#include "gui_graph.h"
#include "gui_psram.h"
#include "gui_string.h"
#include "gui_string_num.h"
#include "gui_rotation.h"
#include "gui_core.h"
#include "app_section.h"
#include "module_touch.h"
#include "board.h"
#include "gui_lcd_callback.h"
#include "psram_platform.h"


uint8_t *PSRAM_LCD_FRAME_BUFFER_ORIGIN = (uint8_t *)(0x6800000 + 0x100000);
uint8_t *PSRAM_LCD_FRAME_BUFFER_LEFT = (uint8_t *)(0x6800000 + 0x200000);
uint8_t *PSRAM_LCD_FRAME_BUFFER_RIGHT = (uint8_t *)(0x6800000 + 0x300000);
uint8_t *PSRAM_LCD_FRAME_BUFFER_DOWN = (uint8_t *)(0x6800000 + 0x400000);
uint8_t *PSRAM_LCD_FRAME_BUFFER_UP = (uint8_t *)(0x6800000 + 0x500000);


UI_PSRAMBufferTypeDef FrameBufferOrigin;
UI_PSRAMBufferTypeDef FrameBufferLeft;
UI_PSRAMBufferTypeDef FrameBufferRight;
UI_PSRAMBufferTypeDef FrameBufferDown;
UI_PSRAMBufferTypeDef FrameBufferUp;



void psram_frame_buffer_init(void)
{
    FrameBufferOrigin.name = "origin";
    FrameBufferOrigin.pLCDBuffer = PSRAM_LCD_FRAME_BUFFER_ORIGIN;
    FrameBufferOrigin.Menu = NULL;
    FrameBufferOrigin.BufferReadFlag = false;

    FrameBufferLeft.name = "left";
    FrameBufferLeft.pLCDBuffer = PSRAM_LCD_FRAME_BUFFER_LEFT;
    FrameBufferLeft.Menu = NULL;
    FrameBufferLeft.BufferReadFlag = false;

    FrameBufferRight.name = "right";
    FrameBufferRight.pLCDBuffer = PSRAM_LCD_FRAME_BUFFER_RIGHT;
    FrameBufferRight.Menu = NULL;
    FrameBufferRight.BufferReadFlag = false;

    FrameBufferDown.name = "down";
    FrameBufferDown.pLCDBuffer = PSRAM_LCD_FRAME_BUFFER_DOWN;
    FrameBufferDown.Menu = NULL;
    FrameBufferDown.BufferReadFlag = false;

    FrameBufferUp.name = "up";
    FrameBufferUp.pLCDBuffer = PSRAM_LCD_FRAME_BUFFER_UP;
    FrameBufferUp.Menu = NULL;
    FrameBufferUp.BufferReadFlag = false;
}

int16_t psram_detal_x = 0;
int16_t psram_detal_y = 0;


DATA_RAM_FUNCTION
void rtl_gui_lcd_update_from_psram(void)
{
    if (psram_detal_x)
    {
        psram_detal_y = 0;
    }

    if (psram_detal_y == 0)
    {
        if (psram_detal_x >= 0)
        {
            rtl_gui_copy_psram_to_lcd_x(FrameBufferLeft, FrameBufferOrigin, LCD_WIDTH - psram_detal_x);
        }
        else
        {
            rtl_gui_copy_psram_to_lcd_x(FrameBufferOrigin, FrameBufferRight, -psram_detal_x);
        }
    }
    else
    {
        if (psram_detal_y > 0)
        {
            rtl_gui_copy_psram_to_lcd_y(FrameBufferUp, FrameBufferOrigin, LCD_HIGHT - psram_detal_y);
        }
        else
        {
            rtl_gui_copy_psram_to_lcd_y(FrameBufferOrigin, FrameBufferDown, -psram_detal_y);
        }
    }
}

DATA_RAM_FUNCTION
void rtl_gui_copy_psram_to_lcd_x(UI_PSRAMBufferTypeDef left, UI_PSRAMBufferTypeDef right,
                                 uint32_t offset)//offset mean left menu not display in current menu
{
    if (offset > LCD_WIDTH)
    {
        return;
    }

    psram_seq_trans_enable(true);

    rtl_gui_lcd_sectionconfig(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1);
    rtl_gui_dma_set_multi_block_for_x((uint32_t)(left.pLCDBuffer), (uint32_t)(right.pLCDBuffer), offset,
                                      0, 0);
    rtl_gui_wait_lcd_control_transfer(0);

    psram_seq_trans_enable(false);
}

DATA_RAM_FUNCTION
void rtl_gui_copy_psram_to_lcd_y(UI_PSRAMBufferTypeDef up, UI_PSRAMBufferTypeDef down,
                                 uint32_t offset) //offset mean up menu not display in current menu
{
    if (offset > LCD_HIGHT)
    {
        return;
    }
    psram_seq_trans_enable(true);

    rtl_gui_lcd_sectionconfig(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1);
    rtl_gui_dma_set_multi_block_for_y((uint32_t)(up.pLCDBuffer), (uint32_t)(down.pLCDBuffer), offset, 0,
                                      0);
    rtl_gui_wait_lcd_control_transfer(0);

    psram_seq_trans_enable(false);
}


DATA_RAM_FUNCTION
void rtl_gui_copy_ram_to_psram(uint8_t *readbuf, uint32_t count, UI_PSRAMBufferTypeDef *location,
                               uint32_t length)
{
#if 1
    if (count == 0)
    {
        rtl_gui_dma_single_block_start((uint32_t)location->pLCDBuffer + count *
                                       LCD_SECTION_BYTE_LEN, (uint32_t)readbuf,
                                       LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES);
    }
    else if (count == (TOTAL_SECTION_COUNT - 1)) //the last one
    {
        rtl_gui_wait_dma_transfer();
        uint32_t last_len = 0;
        if (LCD_HIGHT % LCD_SECTION_HEIGHT == 0)
        {
            last_len = LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES;
        }
        else
        {
            last_len = (LCD_HIGHT % LCD_SECTION_HEIGHT) * LCD_WIDTH * PIXEL_BYTES;
        }
        rtl_gui_dma_single_block_start((uint32_t)location->pLCDBuffer + count *
                                       LCD_SECTION_BYTE_LEN, (uint32_t)readbuf,
                                       last_len);
        rtl_gui_wait_dma_transfer();
    }
    else
    {
        rtl_gui_wait_dma_transfer();
        rtl_gui_dma_single_block_start((uint32_t)location->pLCDBuffer + count *
                                       LCD_SECTION_BYTE_LEN, (uint32_t)readbuf,
                                       LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES);
    }
#else
    memcpy((uint8_t *)(location->pLCDBuffer + count * LCD_SECTION_BYTE_LEN), readbuf,
           LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES);
#endif
}
DATA_RAM_FUNCTION
void rtl_gui_update_to_psram_framebuffer(UI_MenuTypeDef *p_show_menu,
                                         UI_PSRAMBufferTypeDef *location)
{
    if (p_show_menu == NULL)
    {
        return;
    }
    /* cache buffer for dma source address */
    uint8_t *pWriteBuf = NULL;
    UI_MenuTypeDef cur_menu = *p_show_menu;
    UI_MenuTypeDef *local_cur_menu = &cur_menu;
    UI_WidgetTypeDef *local_cur_list = p_show_menu->pWidgetList;

    rtl_gui_dma_single_block_init(0x00000000);

    for (uint16_t i = 0; i < TOTAL_SECTION_COUNT; i++)
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

        for (uint32_t j = 0; j < local_cur_menu->current_max_icon; j++)
        {
            if (local_cur_list[j].widget_id_type == ICON_WIDGET)
            {
                rtl_gui_show_bmp_sector(local_cur_list + j, \
                                        0, 0, \
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
                                    0, 0, \
                                    i * LCD_SECTION_HEIGHT, (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_CLOCK_POINTER)
            {
                rtl_gui_show_clock_pointer(local_cur_list + j, 0, 0, i * LCD_SECTION_HEIGHT,
                                           (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_NUM)
            {
                rtl_gui_show_string_num(local_cur_list + j, 0, 0, i * LCD_SECTION_HEIGHT,
                                        (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
            else if (local_cur_list[j].widget_id_type == ICON_RECT)
            {
                rtl_gui_show_rect(local_cur_list + j, 0, 0, i * LCD_SECTION_HEIGHT,
                                  (i + 1)*LCD_SECTION_HEIGHT, pWriteBuf);
            }
        }

        rtl_gui_copy_ram_to_psram(pWriteBuf, i, location, LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES);
    }
}




