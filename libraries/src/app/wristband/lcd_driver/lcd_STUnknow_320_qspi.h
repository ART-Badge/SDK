#ifndef _LCD_STUNKNOW_QSPI_H_
#define _LCD_STUNKNOW_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void lcd_STUnknow_qspi_320_power_on(void);
void lcd_STUnknow_qspi_320_power_off(void);
void lcd_STUnknow_qspi_320_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd,
                                      uint16_t yEnd);
void lcd_STUnknow_qspi_320_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _LCD_STUNKNOW_QSPI_H_ */
