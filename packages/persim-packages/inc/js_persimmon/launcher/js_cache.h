#ifndef __JS_CACHE_H__
#define __JS_CACHE_H__

#include "js_util.h"
#include <string/string_table.h>

#ifdef __cplusplus
extern "C" {
#endif

js_value_t js_script_parse(const char *fn);
int js_script_release(const char *fn);
js_value_t js_script_find(const char *fn);
int js_script_put(const js_value_t cache);
void js_script_empty(struct string_table *str_table);

#ifdef __cplusplus
}
#endif

#endif
