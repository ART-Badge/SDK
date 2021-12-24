/*
 * File      : js_util.c
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-09-05     realthread   the first version
 */
#ifndef JS_UTIL_H__
#define JS_UTIL_H__

#include <rtthread.h>
#include "js_api_wrapper.h"

#define DECLARE_HANDLER(NAME)                                                                  \
    static js_value_t NAME##_handler(const js_value_t func_value, const js_value_t this_value, \
                                     const js_value_t args[], const js_length_t args_cnt)

#define REGISTER_HANDLER(NAME) js_handler_register_global((js_char_t *)#NAME, NAME##_handler)
#define REGISTER_HANDLER_ALIAS(NAME, HANDLER) \
    js_handler_register_global((js_char_t *)#NAME, HANDLER)

#define REGISTER_METHOD(OBJ, NAME)                js_handler_register(OBJ, #NAME, NAME##_handler)
#define REGISTER_METHOD_ALIAS(OBJ, NAME, HANDLER) js_handler_register(OBJ, #NAME, HANDLER)

#ifdef RT_USING_FINSH
#include <finsh.h>
#define JS_MODULE(NAME, MODULE_INIT) FINSH_FUNCTION_EXPORT_CMD(MODULE_INIT, __jsm_##NAME, RT_NULL)
#else
#define JS_MODULE(NAME, MODULE_INIT)
#endif

#define JS_STRING_BUFSZ 64

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*js_util_user)(void);

void js_set_property_x(const js_value_t obj, const char *name, const js_value_t prop);
js_value_t js_get_property_x(const js_value_t obj, const char *name);
bool js_delete_property_x(const js_value_t obj, const char *name);
bool js_has_property_x(const js_value_t obj, const char *name);

void *js_get_pointer_property(const js_value_t obj, const char *name,
                              const js_object_native_info_t **out_native_info_p);

void js_set_string_property(const js_value_t obj, const char *name, const char *value);
void js_set_boolean_property(const js_value_t obj, const char *name, bool value);
void js_set_number_property(const js_value_t obj, const char *name, double value);
void js_set_pointer_property(const js_value_t obj, const char *name, void *value,
                             const js_object_native_info_t *native_info_p);

void js_handler_register(const js_value_t obj, const char *name, js_external_handler_t func);
js_value_t js_call_func_obj(const js_value_t func_obj_val, const js_value_t this_val,
                            const js_value_t args_p[], js_size_t args_count);
js_value_t js_call_function_x(const js_value_t obj, const char *name, const js_value_t args[],
                              js_size_t args_cnt);

js_value_t js_string_to_value(const char *value);
char *js_value_get_string(const js_value_t value);
char *js_value_get_string_x(const js_value_t value, char *str_buf, unsigned int str_buf_size);

bool js_resolve_error(js_value_t ret_value);

void js_value_dump(const js_value_t value);

void *RT_JS_MALLOC(size_t size);
void RT_JS_FREE(void *ptr);
char *RT_JS_STRDUP(const char *str);

int js_util_init(void);
int js_util_cleanup(void);

#ifndef JS_LOCK_DUMP_ENABLE
int js_util_lock(void);
int js_util_lock_timeout(rt_int32_t time);
int js_util_unlock(void);
#else
int _js_util_lock(const char *func, int line);
int _js_util_lock_timeout(rt_int32_t time, const char *func, int line);
int _js_util_unlock(const char *func, int line);

#define js_util_lock()             _js_util_lock(__FUNCTION__, __LINE__)
#define js_util_lock_timeout(time) _js_util_lock_timeout(time, __FUNCTION__, __LINE__)
#define js_util_unlock()           _js_util_unlock(__FUNCTION__, __LINE__)
#endif

#ifdef __cplusplus
}
#endif

#endif
