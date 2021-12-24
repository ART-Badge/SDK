#ifndef JS_SNAPSHOT_H__
#define JS_SNAPSHOT_H__

#include "jerryscript.h"
#include "jerry-snapshot.h"
#include "js_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef JERRY_ENABLE_SNAPSHOT_SAVE
int js_generate_snapshot(const jerry_char_t *resource_name_p, size_t resource_name_length,
                         const js_char_t *source_p, size_t source_size, uint32_t *buffer_p,
                         size_t buffer_size);
int js_generate_snapshot_save(const char *filename, uint32_t *buffer_p, size_t buffer_size);
int js_generate_snapshot_do(const char *source, const char *fileout, int is_module);
#endif
#ifdef JERRY_ENABLE_SNAPSHOT_EXEC
js_value_t js_exec_snapshot(const uint32_t *snapshot_p, size_t snapshot_size);
js_value_t js_load_function_snapshot_x(const uint32_t *function_snapshot_p,
                                       const size_t function_snapshot_size);
#endif

#ifdef __cplusplus
}
#endif

#endif
