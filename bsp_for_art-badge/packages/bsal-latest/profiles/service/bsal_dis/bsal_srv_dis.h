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

#include "stdint.h"
#include "stdbool.h"

void bsal_le_blufi_svr_init(void *stack_ptr, void *app_callback);
#ifdef __cplusplus
}
#endif

#endif

