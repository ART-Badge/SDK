/*
 * File      : js_callbacks.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-09-05     realthread   the first version
 */
#ifndef JS_CALLBACKS_H__
#define JS_CALLBACKS_H__

#include <js_util.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*js_callback_func)(void *parameter);
typedef rt_bool_t (*js_callback_send_func)(js_callback_func callback, void *parameter);

rt_bool_t js_send_callback(js_callback_func callback, js_value_t parameter);

#ifdef __cplusplus
}
#endif

#endif
