
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODULE_HEART_VC31__H__
#define __MODULE_HEART_VC31__H__
#include "stdint.h"
#include "stdbool.h"
#include "VC31Hci.h"



VC31Ret_t vc31_read(uint8_t read_reg, uint8_t *buf, uint8_t read_len);
VC31Ret_t vc31_write(uint8_t write_reg, uint8_t *buf, uint8_t write_len);
uint32_t vc31_gettime(void);
bool vc31_driver_init(void);
void vc31_enter_dlps(void);
void vc31_exit_dlps(void);
void vc31_process(void);





#endif /* __MODULE_HEART_VC31__H__ */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
