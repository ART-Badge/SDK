#ifndef _LCD_JB5858_360_H_
#define _LCD_JB5858_360_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void lcd_jb5858_power_on(void);
void lcd_jb5858_power_off(void);
void lcd_jb5858_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void lcd_jb5858_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _LCD_JB5858_360_H_ */
