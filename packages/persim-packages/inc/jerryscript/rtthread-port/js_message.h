/*
 * File      : js_message.c
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-09-05     realthread   the first version
 */
#ifndef JS_MESSAGE_H__
#define JS_MESSAGE_H__

#include <js_util.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*js_message_send_func)(const char *name, rt_uint8_t *data, rt_uint32_t size);

void js_message_send_func_init(js_message_send_func func);
rt_bool_t js_message_send(const char *name, rt_uint8_t *data, rt_uint32_t size);
rt_bool_t js_message_send_data(const char *name, js_value_t data);
js_value_t js_message_obj_get(void);
int js_message_init(void);
int js_message_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
