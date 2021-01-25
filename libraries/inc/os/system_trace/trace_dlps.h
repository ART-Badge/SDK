/****************************************************************************************************//**
 * @file     trace_dlps.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_DLPS_H_
#define _TRACE_DLPS_H_

#include "trace_common.h"
#include "trace_port_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*TRACE_DLPS_USER_CB)();

bool trace_dlps_init(uint32_t trace_period_time_ms, uint32_t trace_total_time_ms,
                     TRACE_DLPS_USER_CB trace_dlps_user_cb);

#ifdef __cplusplus
}
#endif

#endif /*_TRACE_DLPS_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
