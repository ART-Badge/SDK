/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     tyx          first implementation
 */

#ifndef __EMQ_VALUE_H__
#define __EMQ_VALUE_H__

#include "emq.h"
#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EMQ_MSG_SUBTYPE_VALUE (0x01)

void emq_value_free(dt_value_t **value);

int emq_send_value(dt_value_t *value, const struct emq_hub *hub, rt_uint32_t channel);
/* Hi Brother, when you see 'dt_value_t ***', you need to know what you are doing */
rt_err_t emq_recv_value(emq_t *queue, dt_value_t ***value, rt_base_t *mapping, rt_int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif


