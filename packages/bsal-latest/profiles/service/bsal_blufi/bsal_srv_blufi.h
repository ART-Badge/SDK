/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */




#ifndef __BSAL_SRV_BLUFI_H__
#define __BSAL_SRV_BLUFI_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define GATT_UUID_BLUFI_PROFILE                   0xFFFF
#define GATT_UUID_CHAR_BLUFI_V2_WRITE             0xFF01
#define GATT_UUID_CHAR_BLUFI_V4_INDICATE          0xFF02

#define BLUEFI_BLE_SERVICE_CHAR_V2_WRITE_INDEX          0x02
#define BLUEFI_BLE_SERVICE_CHAR_V3_INDICATE_INDEX         0x04
#define BLUEFI_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX     (BLUEFI_BLE_SERVICE_CHAR_V3_INDICATE_INDEX + 1)

void bsal_le_blufi_svr_init(void *stack_ptr, void *app_callback);
#ifdef __cplusplus
}
#endif

#endif

