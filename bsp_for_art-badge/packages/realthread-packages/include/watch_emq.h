/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-07-30     chenyong     the first version
 */

#ifndef __WATCH_EMQ_H__
#define __WATCH_EMQ_H__

#include <rtthread.h>
#include <datatype.h>
#include <emq.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*emq_handle_t)(dt_value_t* value);
int watch_emq_bind(const struct emq_hub* hub, rt_uint16_t channel, emq_handle_t handle);
int watch_emq_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __WATCH_EMQ_H__ */