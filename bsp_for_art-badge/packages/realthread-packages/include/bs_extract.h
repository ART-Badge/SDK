/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-04     RTT       the first version
 */
#ifndef _BS_EXTRACT_H_
#define _BS_EXTRACT_H_

#include <stddef.h>
#include <stdint.h>
#include "bspatch.h"

typedef struct bs_extract bs_extract_t;

typedef struct extract_ops
{
    int (*init)(void);
    int (*decompress)(const struct bspatch_stream* stream, void* buffer, int length);
    int (*deinit)(void);
} extract_ops_t;

struct bs_extract
{
    const char *name;
    extract_ops_t ops;
};

int bsdiff_extract_init(void);
const bs_extract_t *bsdiff_extract_find(const char *name);

#endif /* _BS_EXTRACT_H_ */
