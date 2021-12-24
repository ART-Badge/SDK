
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STK8321_DRIVER__H__
#define __STK8321_DRIVER__H__
#include "stdint.h"
#include "hub_gsensor.h"



bool stk8321_read(uint8_t read_reg, uint8_t *buf, uint8_t read_len);
bool stk8321_write(uint8_t write_reg, uint8_t *buf, uint8_t write_len);
bool stk8321_get_fifo_data(uint8_t *len, AxesRaw_t *buf);
bool stk8321_driver_init(void);
void stk8321_enter_dlps(void);
void stk8321_exit_dlps(void);
void stk8321_enable(void);
void stk8321_disable(void);
uint8_t stk8321_get_fifo_length(void);




#endif /* __STK8321_DRIVER__H__ */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
