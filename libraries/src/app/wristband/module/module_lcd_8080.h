#ifndef _RTL876X_MODULE_LCD_8080_H_
#define _RTL876X_MODULE_LCD_8080_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void lcd_driver_init(void);
void lcd_device_init(void);
void lcd_exit_dlps(void);
void lcd_enter_dlps(void);
void lcd_set_backlight(uint32_t percent);
void lcd_te_enable(void);
void lcd_te_disable(void);
void lcd_frame_sync_handler(void);

void lcd_te_device_init(void);


void lcd_dma_single_block_init(uint32_t dir_type);
void lcd_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);
void lcd_dma_single_block_start(uint32_t destination_addr, uint32_t source_addr, uint32_t len);
void lcd_wait_dma_transfer(void);
void lcd_wait_lcd_control_transfer(uint32_t count);
void lcd_dma_set_multi_block_for_y(uint32_t picture0_addr, uint32_t picture1_addr,
                                   uint32_t offset,
                                   uint32_t yStart, uint32_t yEnd);
void lcd_dma_set_multi_block_for_x(uint32_t picture0_addr, uint32_t picture1_addr,
                                   uint32_t offset,
                                   uint32_t yStart, uint32_t yEnd);
bool get_font_dot(uint16_t unicode, uint32_t *addr, uint8_t *dot_width, uint8_t *font_width);
void lcd_show_char(uint16_t font_width, uint16_t font_height,
                   const unsigned char BMP[], int16_t xs,
                   int16_t ys, \
                   uint32_t colour, int16_t Zs, int16_t Ze, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_MODULE_LCD_8080_H_ */
