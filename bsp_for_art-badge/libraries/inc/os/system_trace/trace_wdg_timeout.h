/****************************************************************************************************//**
 * @file     trace_wdg_timeout.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 ****************************************************************************************************/

#ifndef TRACE_WDG_TIMEOUT_H
#define TRACE_WDG_TIMEOUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "trace_common.h"
#include "trace_port_config.h"

/******** magic pattern for wdg timeout reason record ********/
#define WDG_TIMEOUT_PATTERN_BEG     (0xfdcebaef)
#define WDG_TIMEOUT_PATTERN_END     (0xfcaebaef)

/************Dump info:  *********
4: beg pattern(value=0xfdcebaef)
4: reset reason(value=0x1)
4: lr
4: pc
4: sp
80: current TCB
512 or less: task callstck
4: end pattern(value=0xfcaebaef)
*********************************/
extern void patch_wdg_timeout_reason_save_to_flash_init(void);



#ifdef __cplusplus
}
#endif

#endif

