/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-25     chenyong     first version
 */

#ifndef __MCF_LINK_H__
#define __MCF_LINK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mcf.h>
#include <rtdevice.h>

#define MCF_LINK_STATIS_INIT           0x01
#define MCF_LINK_STATIS_READY          0x02
#define MCF_LINK_STATUS_RECV           0x03

#ifndef MCF_LINK_PORT_MAX
#define MCF_LINK_PORT_MAX              8
#endif

/* MCF link communicate protocol type */
enum mcf_link_type
{
    MCF_LINK_TYPE_SOCKET = 0x01,
    MCF_LINK_TYPE_UART,
    MCF_LINK_TYPE_SPI,
    MCF_LINK_TYPE_CAN,
    MCF_LINK_TYPE_SMEM,
    MCF_LINK_TYPE_USER,
    MCF_LINK_TYPE_USB,
};

struct mcf_arp
{
    uint8_t port_id;
    uint8_t dev_id;
    rt_tick_t tick;
    rt_slist_t list;
};

/* MCF link frame */
struct mcf_frame
{
    uint8_t type;                     /* packet type */
    void *payload;                    /* packet payload */
};

struct mcf_link;

/* MCF link communicate protocol operations */
struct mcf_link_ops
{
    mcf_err_t (*init)(struct mcf_link *link);
    mcf_err_t (*send)(struct mcf_link *link, struct mcf_pbuf *pbuf, int32_t timeout);
    mcf_err_t (*recv)(struct mcf_link *link, struct mcf_pbuf *pbuf, int32_t timeout);
    mcf_err_t (*control)(struct mcf_link *link, int cmd, void *arg);
};

/* MCF link communicate protocol  */
struct mcf_link
{
    uint8_t id;                        /* link port ID */
    uint8_t type;                      /* link port type */
    uint16_t mtu;                      /* link MTU */
    const struct mcf_link_ops *ops;    /* link operations */

    uint8_t *recv_buf;                 /* receive buffer */
    size_t recv_bufsz;                 /* maximum receive buffer size */
    size_t cur_recv_bufsz;             /* current receive buffer size */
    size_t frames_len;                 /* received frame buffer length */
    rt_sem_t recv_lock;                /* receive data lock */
    rt_sem_t send_lock;                /* send data lock */
    size_t notice_num;                 /* send data notice number */
    int status;                        /* MCF link status */

    rt_slist_t list;                    /* link list */
    rt_slist_t recv_list;               /* link receive data list */
    void *user_data;                    /* user-specific data */
};

/* MCF link event operations */
mcf_err_t mcf_link_notice_recv(struct mcf_link **link, int32_t timeout);
mcf_err_t mcf_link_notice_send(struct mcf_link *link);

mcf_err_t mcf_link_recv(struct mcf_link *link, struct mcf_pbuf *pbuf);
mcf_err_t mcf_link_send(struct mcf_link *link, struct mcf_pbuf *pbuf, int32_t timeout);

/* MCF link object get */
struct mcf_link *mcf_link_get_by_pid(uint8_t port_id);
struct mcf_link *mcf_link_get_by_did(uint8_t dev_id);

/* MCF link object register */
int mcf_link_register(struct mcf_link *link, uint8_t port_id,
                      uint8_t type, uint16_t mtu, void *user_data);

/* MCF link list get */
rt_slist_t mcf_link_list_get(void);
rt_slist_t mcf_link_arp_list_get(void);
uint8_t mcf_link_frame_id_add(void);

#ifdef  __cplusplus
    }
#endif

#endif /* __MCF_LINK_H__ */
