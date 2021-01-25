#ifndef _GUI_LCD_CALLBACK_H_
#define _GUI_LCD_CALLBACK_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
#include "rtl876x.h"
#include "wristband_gui.h"

typedef void (* rtl_gui_lcd_power_on_cb)(void);
typedef void (* rtl_gui_lcd_power_off_cb)(void);
typedef void (* rtl_gui_lcd_set_window_cb)(uint16_t xStart, uint16_t yStart, uint16_t xEnd,
                                           uint16_t yEnd);
typedef void (* rtl_gui_lcd_init_cb)(void);

typedef void (* rtl_gui_lcd_te_enable_cb)(void);
typedef void (* rtl_gui_lcd_te_disable_cb)(void);

typedef bool (* rtl_gui_get_font_dot_cb)(uint16_t unicode, uint32_t *addr, uint8_t *dot_width,
                                         uint8_t *font_width);
typedef void (* rtl_gui_show_char_cb)(uint16_t font_width, uint16_t font_height,
                                      const unsigned char BMP[], int16_t xs,
                                      int16_t ys, \
                                      uint32_t colour, int16_t Zs, int16_t Ze, uint8_t *buf);//todo

typedef void (* rtl_gui_lcd_clear_cb)(uint16_t xStart, uint16_t yStart, uint16_t xEnd,
                                      uint16_t yEnd, uint16_t color);
typedef void (* rtl_gui_dma_single_block_init_cb)(uint32_t dir_type);

typedef void (* rtl_gui_dma_single_block_start_cb)(uint32_t destination_addr, uint32_t source_addr,
                                                   uint32_t len);

typedef void (* rtl_gui_wait_dma_transfer_cb)(void);
typedef void (* rtl_gui_wait_lcd_transfer_cb)(uint32_t count);

typedef void (* rtl_gui_dma_set_multi_block_for_y_cb)(uint32_t picture0_addr,
                                                      uint32_t picture1_addr,
                                                      uint32_t offset,
                                                      uint32_t yStart, uint32_t yEnd);
typedef void (* rtl_gui_dma_set_multi_block_for_x_cb)(uint32_t picture0_addr,
                                                      uint32_t picture1_addr,
                                                      uint32_t offset,
                                                      uint32_t yStart, uint32_t yEnd);

typedef void (* rtl_gui_lcd_frame_rate_cb)(uint8_t RTNA, uint8_t FPA, uint8_t BPA);

extern rtl_gui_lcd_power_on_cb                 user_lcd_power_on_cb;
extern rtl_gui_lcd_power_off_cb                user_lcd_power_off_cb;
extern rtl_gui_lcd_set_window_cb               user_lcd_set_window_cb;
extern rtl_gui_lcd_init_cb                     user_lcd_init_cb;
extern rtl_gui_lcd_te_enable_cb                user_lcd_te_enable_cb;
extern rtl_gui_lcd_te_disable_cb               user_lcd_te_disable_cb;
extern rtl_gui_lcd_clear_cb                    user_lcd_clear_cb;
extern rtl_gui_lcd_frame_rate_cb               user_lcd_frame_rate_cb;
extern rtl_gui_dma_single_block_init_cb        user_dma_single_block_init_cb;
extern rtl_gui_dma_single_block_start_cb       user_dma_single_block_start_cb;
extern rtl_gui_wait_dma_transfer_cb            user_wait_dma_transfer_cb;
extern rtl_gui_wait_lcd_transfer_cb            user_wait_lcd_transfer_cb;
extern rtl_gui_dma_set_multi_block_for_x_cb    user_dma_set_multi_block_for_x_cb;
extern rtl_gui_dma_set_multi_block_for_y_cb    user_dma_set_multi_block_for_y_cb;
extern rtl_gui_get_font_dot_cb                 user_lcd_get_font_dot_cb;
extern rtl_gui_show_char_cb                    user_show_char_cb;



void rtl_gui_dma_single_block_init(uint32_t dir_type);
void rtl_gui_dma_single_block_start(uint32_t destination_addr, uint32_t source_addr, uint32_t len);
void rtl_gui_wait_dma_transfer(void);
void rtl_gui_dma_set_multi_block_for_x(uint32_t picture0_addr, uint32_t picture1_addr,
                                       uint32_t offset,
                                       uint32_t yStart, uint32_t yEnd);
void rtl_gui_dma_set_multi_block_for_y(uint32_t picture0_addr, uint32_t picture1_addr,
                                       uint32_t offset,
                                       uint32_t yStart, uint32_t yEnd);
void rtl_gui_wait_lcd_control_transfer(uint32_t count);
void rtl_gui_lcd_sectionconfig(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void rtl_gui_lcd_power_on(void);
void rtl_gui_lcd_power_off(void);
void rtl_gui_lcd_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd,
                       uint16_t color);

void rtl_gui_lcd_te_enable(void);
void rtl_gui_lcd_te_disable(void);
bool rtl_gui_get_font_dot(uint16_t unicode, uint32_t *addr, uint8_t *dot_width,
                          uint8_t *font_width);
void rtl_show_char(uint16_t font_width, uint16_t font_height,
                   const unsigned char BMP[], int16_t xs,
                   int16_t ys, \
                   uint32_t colour, int16_t Zs, int16_t Ze, uint8_t *buf);
void rtl_frame_rate(uint8_t RTNA, uint8_t FPA, uint8_t BPA);
#ifdef __cplusplus
}
#endif

#endif /* _GUI_LCD_CALLBACK_H_ */
