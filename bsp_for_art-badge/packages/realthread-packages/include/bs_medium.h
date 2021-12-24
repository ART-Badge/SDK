/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-02     RTT       the first version
 */
#ifndef _BS_MEDIUM_H_
#define _BS_MEDIUM_H_

#include <stddef.h>
#include <stdint.h>

typedef struct bs_medium bs_medium_t;

typedef struct bs_ops
{
    int (*init) (bs_medium_t *medium);
    int (*read) (const bs_medium_t *medium, uint32_t addr, uint8_t *buffer, size_t size);
    int (*write)(const bs_medium_t *medium, uint32_t addr, uint8_t *buffer, size_t size);
    int (*erase)(const bs_medium_t *medium, uint32_t addr, size_t size);
} bs_ops_t;

struct bs_medium
{
    const char *name;
    const void *storage;
    const char *path;
    size_t size;
    bs_ops_t ops;
};

int bsdiff_medium_init(void);
const bs_medium_t *bsdiff_medium_find(const char *name);
int bsdiff_medium_read(const bs_medium_t *medium, uint32_t addr, uint8_t *buffer, size_t size);
int bsdiff_medium_write(const bs_medium_t *medium, uint32_t addr, uint8_t *buffer, size_t size);
int bsdiff_medium_erase(const bs_medium_t *medium, uint32_t addr, size_t size);

#endif /* _BS_MEDIUM_H_ */
