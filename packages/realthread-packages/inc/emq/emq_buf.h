/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-10     tyx          first implementation
 */

#ifndef __EMQ_BUF_H__
#define __EMQ_BUF_H__

#include "emq.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EMQ_BUFF_FLAG_CONST      (0x1 << 0)
#define EMQ_BUFF_FLAG_SENDING    (0x1 << 1)

/* get message buff */
#define EMQ_BUFF(msg) \
    ((msg) ? (msg)->data : RT_NULL)

struct emq_buff
{
    void *data;                             /* point to data */
    void (*free)(struct emq_buff *ebuff);   /* emq buff release function */
    rt_uint16_t all_ref;                    /* emq buff message reference count */
    rt_uint16_t free_cnt;                   /* emq buff message reference count */
    rt_uint8_t flags;                       /* flags */
    rt_uint8_t reserved;
};

#define EMQ_CONST_BUFF(buff) {(buff), RT_NULL, 1, EMQ_BUFF_FLAG_CONST}

#define EMQ_BUFF_SEND(msg, hub_name, channel) \
    (emq_send_buff(msg, EMQ_HUB(hub_name),channel))

rt_inline struct emq_buff *emq_buff_init(struct emq_buff *ebuf, void *data,
    void (*free)(struct emq_buff *))
{
    if (ebuf)
    {
        ebuf->data = data;
        ebuf->free = free;
        ebuf->all_ref = 1;
        ebuf->free_cnt = 0;
        ebuf->flags = 0;
    }
    return ebuf;
}

/* emq buff */
struct emq_buff *emq_buff_alloc(rt_size_t size);
void emq_buff_ref(struct emq_buff *ebuf);
void emq_buff_free(struct emq_buff *ebuf);
int emq_send_buff(struct emq_buff *ebuf, const struct emq_hub *hub, rt_uint32_t channel);

#ifdef __cplusplus
}
#endif

#endif
