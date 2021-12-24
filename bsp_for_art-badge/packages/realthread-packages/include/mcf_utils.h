/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-25     chenyong     first version
 */

#ifndef __MCF_UTILS_H__
#define __MCF_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* MCF packet buffer */
struct mcf_pbuf
{
    void *buf;                         /* real data address */
    size_t len;                        /* current data size */
    size_t size;                       /* maximum data size */
};

/* MCF packet operations */
struct mcf_pbuf *mcf_pbuf_alloc(size_t size);
struct mcf_pbuf *mcf_pbuf_strdup(struct mcf_pbuf *pbuf);
void mcf_pbuf_free(struct mcf_pbuf *pbuf);

/* MCF print raw packet data information */
void mcf_print_raw_data(const uint8_t *raw_data, size_t size);

/* crc16 calculation */
uint16_t mcf_crc16_calc(const uint8_t *data, size_t size);

#ifdef  __cplusplus
    }
#endif

#endif /* __MCF_UTILS_H__ */

