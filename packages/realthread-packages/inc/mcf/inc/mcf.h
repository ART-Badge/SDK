/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-25     chenyong     first version
 */

#ifndef __MCF_H__
#define __MCF_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rtthread.h>
#include <rthw.h>

#include <mcf_utils.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MCF_SW_VER                     "0.1.0"
#define MCF_SW_VER_NUM                 0x000100

/* MCF device ID type */
#define MCF_DEV_ID_RECEIVER            254
#define MCF_DEV_ID_BROADCAST           255

/* MCF packet flag */
#define MCF_PKT_FLAG_NEED_RSP          0x01
#define MCF_PKT_FLAG_NEED_ACK          0x02

/* MCF packet type */
#define MCF_PKT_TYPE_REQ               0x00
#define MCF_PKT_TYPE_RSP               0x01

/* MCF link control commands */
#define MCF_LINK_CTRL_NEED_CRC16       0x01

#ifndef MCF_PKT_MAX_SIZE
#define MCF_PKT_MAX_SIZE               1024
#endif

struct mcf_pkt;

/* MCF request execute feature callback */
typedef int *(*mcf_req_exec_fn)(struct mcf_pkt *in_pkt, struct mcf_pkt *out_pkt);

/* MCF error code */
enum mcf_err_code
{
    MCF_OK = 0,
    MCF_FAILD,
    MCF_MEM_FULL,                      /* memory full */
    MCF_SEND_TIMEOUT,                  /* send data timeout */
    MCF_SEND_RETRY_ERR,                /* send data retry error */
    MCF_RECV_TIMEOUT,                  /* receive data timeout */
    MCF_RECV_BUF_FULL,                 /* receive buffer full */
    MCF_REQ_PKT_ERR,                   /* request packet error */
    MCF_RSP_PKT_ERR,                   /* response packet error */
    MCF_ACK_PKT_ERR,                   /* response packet error */
};
typedef int mcf_err_t;

/* MCF packet information for receive packet */
struct mcf_pkt_info
{
    uint8_t dst_id;                    /* destination device ID */
    uint8_t src_id;                    /* source device ID */
    uint8_t pkt_id;                    /* packet ID */
    uint8_t pkt_type:2;                /* packet type */
    uint8_t need_ack:1;                /* packet need ACK */
    uint8_t need_rsp:1;                /* packet need response */
};

/* MCF packet */
struct mcf_pkt
{
    struct mcf_pbuf pbuf;              /* MCF packet buffer */
    struct mcf_pkt_info info;          /* MCF packet information */
};

/* MCF initialize and start */
int mcf_init(uint8_t id);
void mcf_start(void);

/* MCF D2D transport protocol operations */
int mcf_d2d_open(uint8_t dst_id);
int mcf_d2d_send(int sid, struct mcf_pkt *pkt, int32_t timeout);
int mcf_d2d_recv(int sid, struct mcf_pkt *pkt, int32_t timeout);
int mcf_d2d_close(int sid);

/* MCF register request execute callback function */
int mcf_req_exec_fn_register(mcf_req_exec_fn req_fn);

/* MCF packet operations */
struct mcf_pkt *mcf_pkt_make(struct mcf_pkt *pkt, void *buf, size_t len, size_t size);
struct mcf_pkt *mcf_pkt_alloc(int sid, size_t size);
int mcf_pkt_free(struct mcf_pkt *pkt);
struct mcf_pkt *mcf_pkt_info_set(struct mcf_pkt *pkt, uint8_t type, uint16_t flag);
int mcf_pkt_info_get(struct mcf_pkt *pkt, uint8_t *type, uint16_t *flag);

/* MCF link protocol operations */
int mcf_link_uart_register(uint8_t port_id, const char *dev_name, void *user_data);
int mcf_link_control(uint8_t port_id, int cmd, void *arg);

/* MCF link ARP object operations */
int mcf_link_arp_add(uint8_t port_id, uint8_t dev_id);
int mcf_link_arp_delete(uint8_t port_id, uint8_t dev_id);

#ifdef  __cplusplus
    }
#endif

#endif /* __MCF_H__ */
