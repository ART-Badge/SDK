/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-05     tyustli      first version
 */


#ifndef __DRV_FLASH_H_
#define __DRV_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtthread.h"

int rt_hw_flash_init(void);

int realtek_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size);
int realtek_flash_write(rt_uint32_t addr, const uint8_t *buf, size_t size);
int realtek_flash_erase(rt_uint32_t addr, size_t size);

#define REALTK_FLASH_START_ADDRESS                   0x00C00000
#define REALTEK_FLASH_SIZE                           (12 * 1024 *1024)
#define FLASH_SECTOR_SIZE                            4096
#define REALTEK_FLASH_END_ADDRESS                    (0x00800000 + 16 * 1024 * 1024)

#ifdef __cplusplus
}
#endif

#endif /* __DRV_FLASH_H_ */

/************************** end of file ******************************/

