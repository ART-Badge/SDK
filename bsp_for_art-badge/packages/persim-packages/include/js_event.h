/*
 * File      : js_event.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-09-05     realthread   the first version
 */
#ifndef JS_EVENT_H__
#define JS_EVENT_H__

#include <js_util.h>

#ifdef __cplusplus
extern "C" {
#endif

void js_set_event_callback_mode(js_value_t obj, const char *event_name, bool mode);
void js_add_event_listener(js_value_t obj, const char *event_name, js_value_t func);
void js_remove_listener(js_value_t obj, const char *event_name, js_value_t func);
void js_remove_event_listener(js_value_t obj, const char *event_name);
rt_bool_t js_emit_event(js_value_t obj, const char *event_name, const js_value_t argv[],
                        const js_length_t argc);
void js_destroy_emitter(js_value_t obj);
void js_make_emitter(js_value_t obj, js_value_t prototype);
int js_event_init(void);
int js_event_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
