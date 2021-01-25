/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-29     supperthomas first version
 */


#ifndef __BOARD_H
#define __BORAD_H

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "rtl876x_uart.h"
#include "rtl876x_lib_platform.h"
#include "flash_device.h"
#include "rtl876x_gpio.h"
#include "trace.h"
#include "rtl876x.h"
#include "mem_config.h"
#include "otp.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RWMEM$$ZI$$Limit;
#define HEAP_BEGIN      BUFFER_RAM_ADDR  //(&Image$$RWMEM$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      (&__bss_end)
#endif

#define HEAP_END        (BUFFER_RAM_ADDR+BUFFER_RAM_SIZE)


#ifdef __cplusplus
}
#endif

#endif
