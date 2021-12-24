/*
 * File      : js_listener.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-02     realthread   the first version
 */
#ifndef JS_LISTENER_H__
#define JS_LISTENER_H__

#include <js_util.h>

#ifdef __cplusplus
extern "C" {
#endif

struct js_listener
{
    js_value_t func;
    void *context;
};

struct js_listener *js_create_listener(js_value_t func);
int js_call_listener(struct js_listener *listener, const js_value_t this_val,
                     const js_value_t args_p[], js_size_t args_count);
void js_release_listener(struct js_listener *listener);

#ifdef __cplusplus
}
#endif

#endif
