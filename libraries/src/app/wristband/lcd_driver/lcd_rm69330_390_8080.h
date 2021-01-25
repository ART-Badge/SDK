#ifndef _LCD_RM69330_390_H_
#define _LCD_RM69330_390_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void lcd_rm69330_power_on(void);
void lcd_rm69330_power_off(void);
void lcd_rm69330_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void lcd_rm69330_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _LCD_RM69330_390_H_ */
