#ifndef __SPIFS_DRIVER_H__
#define __SPIFS_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../spiffs_config.h"

s32_t rtk_flash_read(u32_t addr, u32_t size, u8_t *dst);
s32_t rtk_flash_write(u32_t addr, u32_t size, u8_t *src);
s32_t rtk_flash_erase(u32_t addr, u32_t size);


#ifdef __cplusplus
}
#endif

#endif//__SPIFS_DRIVER_H__
