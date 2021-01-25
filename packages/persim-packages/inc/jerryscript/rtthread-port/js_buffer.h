/*
 * Licensed under the Apache License, Version 2.0 (the "License")
 * Copyright (c) 2016-2020, Intel Corporation.
 *
 * ported from zephyr.js
 * COPYRIGHT (C) 2018, RT-Thread Development Team
 */
#ifndef JS_BUFFER_H__
#define JS_BUFFER_H__

#include <js_util.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct js_buffer
{
    uint8_t *buffer;
    uint32_t bufsize;
} js_buffer_t;

int js_buffer_init(void);
int js_buffer_cleanup(void);

js_buffer_t *js_buffer_find(const js_value_t obj);
bool js_value_is_buffer(const js_value_t value);
js_value_t js_buffer_create(uint32_t size, js_buffer_t **ret_buf);

#ifdef __cplusplus
}
#endif

#endif
