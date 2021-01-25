/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-07     tyx          first implementation
 */

#ifndef __EMQ_H__
#define __EMQ_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct emq_container
{
    rt_slist_t node;                        /* points to the next message container */
    rt_base_t message;                      /* emq message */
    rt_base_t mapping;                      /* mapping relationship */
};

#define EMQ_HUB_TYPE_CONST      (0x1)
#define EMQ_HUB_TYPE_STATIC     (0x2)
#define EMQ_HUB_TYPE_MALLOC     (0x3)

struct emq_hub_ext
{
    rt_slist_t recv_cb_list;
    rt_slist_t send_cb_list;
    rt_slist_t bind_cb_list;
    rt_slist_t unbind_cb_list;
    rt_slist_t channel[];        /* channel list */
};

struct emq_hub
{
    const char *name;           /* hub name */
    rt_uint16_t channels;       /* number of channels */
    rt_uint8_t type;            /* hub type. CONST STATIC MALLOC */
    rt_uint8_t reserved;
    struct emq_hub_ext *ext;     /* hub ext */
};

struct emq_ep
{
    rt_slist_t hub_node;            /* append hub list */
    struct emq_queue *queue;
    rt_base_t mapping;
    rt_slist_t *hub_head;           /* point hub list head */
    rt_slist_t queue_node;          /* append queue list */
};

#define EMQ_QUEUE_MAGIC         (0x454D514D)

#define EMQ_FLAG_FIFO        (0x00)
#define EMQ_FLAG_PRIO        (0x01)
#define EMQ_FLAG_NEW         (0x00)     /* drop the new buffered message */
#define EMQ_FLAG_OLD         (0x02)     /* drop the old buffered message */

#define EMQ_CMD_SET_FLAG    (0x1)
#define EMQ_CMD_GET_FLAG    (0x2)
#define EMQ_CMD_CLEAR_MSG   (0x3)

typedef struct emq_queue emq_t;
struct emq_queue
{
    rt_uint32_t magic;                  /* magic number */
    struct emq_container *msg_pool;     /* message pool */
    rt_uint32_t size;                   /* pool size */

    rt_uint16_t entry;                  /* messages */
    rt_uint16_t flags;                  /* flag */

    rt_list_t suspend_thread;           /* suspend thread */
    rt_list_t q_list;                   /*  */

    rt_slist_t  free;                   /* free list */
    rt_slist_t  head;                   /* msg list head */
    rt_slist_t  *tail;                  /* point list tail */

    rt_slist_t  ep_list;                /* point all ep */
    void (*recv_fn)(emq_t *queue);       /* recv msg callback */
};

typedef rt_base_t emq_msg_t;

struct hub_cb_node
{
    rt_slist_t node;
    void *user_data;
    union
    {
        void (*recv)(rt_uint16_t channel, rt_base_t msg, void *user_data);
        void (*send)(rt_uint16_t channel, rt_base_t msg, void *user_data);
        void (*bind)(rt_uint16_t channel, struct emq_queue *queue, void *user_data);
        void (*unbind)(rt_uint16_t channel, struct emq_queue *queue, void *user_data);
    }cb;
};

/* export HUB */
#define EMQ_HUB_EXPORT(hub_name, channels)                \
    static struct                       \
    {                                   \
        rt_slist_t recv_cb_list;        \
        rt_slist_t send_cb_list;        \
        rt_slist_t bind_cb_list;        \
        rt_slist_t unbind_cb_list;      \
        rt_slist_t channel[channels];   \
    }__emq_channel_##hub_name##__;      \
    const struct emq_hub __emq_hub_##hub_name##__ =             \
        {#hub_name, (channels), EMQ_HUB_TYPE_CONST, 0xFF,       \
        (struct emq_hub_ext *)&__emq_channel_##hub_name##__}
/* import HUB */
#define EMQ_HUB_IMPORT(hub_name) \
    extern const struct emq_hub __emq_hub_##hub_name##__

/* get HUB object */
#define EMQ_HUB(hub_name) \
    (&__emq_hub_##hub_name##__)

/* bind channel */
#define EMQ_BIND(queue, hub_name, channel, mapping) \
    (emq_bind(queue, EMQ_HUB(hub_name), channel, mapping))

/* unbind channel */
#define EMQ_UNBIND(queue, hub_name, channel) \
    (emq_unbind(queue, EMQ_HUB(hub_name), channel))

/* receive message */
#define EMQ_RECV(queue, uid) \
    emq_recv_forever(queue, uid)

/* send message */
#define EMQ_SEND(msg, hub_name, channel) \
    (emq_send(msg, EMQ_HUB(hub_name),channel))

#define EMQ_SEND_URGENT(msg, hub_name, channel) \
    (emq_send_urgent(msg, EMQ_HUB(hub_name),channel))

/* queue */
rt_err_t emq_queue_init(emq_t *queue, struct emq_container *msg_pool,
    rt_uint16_t size, rt_uint16_t flag);
void emq_queue_detach(emq_t *queue);
emq_t *emq_queue_create(rt_uint16_t max_msgs, rt_uint16_t flag);
void emq_queue_delete(emq_t *queue);
void emq_queue_recv_notify(emq_t *queue, void (*recv_fn)(emq_t *queue));
void emq_queue_size(emq_t* queue, rt_uint16_t *total, rt_uint16_t *used);

/* relation */
rt_err_t emq_bind(emq_t *queue, const struct emq_hub *hub,
    rt_uint16_t channel, rt_base_t mapping);
rt_err_t emq_unbind(emq_t *queue, const struct emq_hub *hub, rt_uint16_t channel);
rt_err_t emq_control(emq_t *queue, int cmd, void *arg);
/* get mapping */
rt_base_t emq_mapping(emq_t *queue, const struct emq_hub *hub, rt_uint16_t channel);

/* recv */
rt_err_t emq_recv(emq_t *queue, rt_base_t *msg, rt_base_t *uid, rt_int32_t timeout);
rt_base_t emq_recv_forever(emq_t* queue, rt_base_t* uid);
int emq_send(rt_base_t msg, const struct emq_hub *hub, rt_uint16_t channel);
int emq_send_urgent(rt_base_t msg, const struct emq_hub *hub, rt_uint16_t channel);

#ifdef __cplusplus
}
#endif

#endif
