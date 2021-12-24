#ifndef _LCD_ST7796_320_H_
#define _LCD_ST7796_320_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void lcd_st7796_power_on(void);
void lcd_st7796_power_off(void);
void lcd_st7796_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void lcd_st7796_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _LCD_ST7796_320_H_ */
