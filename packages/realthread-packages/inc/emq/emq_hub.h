/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-22     tyx          first implementation
 */

#ifndef __EMQ_HUB_EXT_H__
#define __EMQ_HUB_EXT_H__

#include "emq.h"

#ifdef __cplusplus
extern "C" {
#endif

struct emq_hub *emq_hub_create(const char *name, rt_uint16_t channels);
const struct emq_hub *emq_hub_lookup(const char *name);
rt_err_t emq_hub_append(const struct emq_hub *hub);

#ifdef __cplusplus
}
#endif

#endif
