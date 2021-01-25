/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-25     chenyong     first version
 */

#ifndef __MCF_TRANS_H__
#define __MCF_TRANS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mcf.h>

/* MCF D2D packet header length (4 bytes),
   SRC ID(1byte) + DST ID(1byte) + PKT ID(1byte) + PKT type(1byte) */
#define MCF_D2D_PKT_HEAD_LEN 4
/* MCF D2D packet buffer max */
#define MCF_D2D_PKT_MAX_SIZE (MCF_PKT_MAX_SIZE + MCF_D2D_PKT_HEAD_LEN)

struct mcf_d2d_pkt
{
    uint8_t dst_id;          /* destination device ID */
    uint8_t src_id;          /* source device ID */
    uint8_t pkt_id;          /* packet ID */
    uint8_t pkt_type:2;      /* packet type */
    uint8_t need_ack:1;      /* need ACK */
    uint8_t need_rsp:1;      /* need response */
    uint8_t priority:2;      /* packet priority */
    uint8_t reserve:2;       /* reserve data */
    struct mcf_pbuf *pbuf;   /* MCF d2d packet buffer */
    size_t len;              /* MCF d2d packet length */
};

enum mcf_d2d_pkt_type
{
    MCF_D2D_PKT_TYPE_REQ = 0,
    MCF_D2D_PKT_TYPE_RSP,
    MCF_D2D_PKT_TYPE_ACK,
    MCF_D2D_PKT_TYPE_BROAD,
    MCF_D2D_PKT_TYPE_PROXY,
    MCF_D2D_PKT_TYPE_MAX,
};

enum mcf_d2d_pkt_index
{
    MCF_D2D_PKT_INDEX_SRC_ID = 0,
    MCF_D2D_PKT_INDEX_DST_ID,
    MCF_D2D_PKT_INDEX_PKT_ID,
    MCF_D2D_PKT_INDEX_PKT_INFO,
};

/* MCF transport layer protocol type */
enum mcf_trans_type
{
    MCF_TRANS_TYPE_D2D = 0x01,
    MCF_TRANS_TYPE_P2P,
    MCF_TRANS_TYPE_USER
};

#ifdef MCF_SECURITY_AES256
/* AES data length align */
#define MCF_AES_ALIGN(len)             RT_ALIGN(len, 16)
/* AES data header length */
#define MCF_AES_DATA_HEAD              2
#endif

/* get MCF object  */
struct mcf_object *mcf_object_get(void);

/* MCF security operations */
mcf_err_t mcf_security_init(void);
mcf_err_t mcf_security_pack(void *input, size_t in_len, void **output, size_t *out_len);
mcf_err_t mcf_security_unpack(void *input, size_t in_len, void **output, size_t *out_len);

#ifdef  __cplusplus
    }
#endif

#endif /* __MCF_TRANS_H__ */
