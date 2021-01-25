/*
 * File      : js_api_wrapper.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-20     realthread   the first version
 */
#ifndef JS_API_WRAPPER_H__
#define JS_API_WRAPPER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint8_t js_char_t;
typedef uint32_t js_size_t;
typedef uint32_t js_length_t;
typedef uint32_t js_value_t;

/* value type information. */
typedef enum
{
  JS_TYPE_NONE = 0u, /**< no type information */
  JS_TYPE_UNDEFINED, /**< undefined type */
  JS_TYPE_NULL,      /**< null type */
  JS_TYPE_BOOLEAN,   /**< boolean type */
  JS_TYPE_NUMBER,    /**< number type */
  JS_TYPE_STRING,    /**< string type */
  JS_TYPE_OBJECT,    /**< object type */
  JS_TYPE_FUNCTION,  /**< function type */
  JS_TYPE_ERROR,     /**< error/abort type */
} js_type_t;

/* Error object types. */
typedef enum
{
  JS_ERROR_NONE = 0,  /**< No Error */
  JS_ERROR_COMMON,    /**< Error */
  JS_ERROR_EVAL,      /**< EvalError */
  JS_ERROR_RANGE,     /**< RangeError */
  JS_ERROR_REFERENCE, /**< ReferenceError */
  JS_ERROR_SYNTAX,    /**< SyntaxError */
  JS_ERROR_TYPE,      /**< TypeError */
  JS_ERROR_URI        /**< URIError */
} js_error_t;

/* Type of an external function handler. */
typedef js_value_t (*js_external_handler_t) (const js_value_t function_obj,
                                                   const js_value_t this_val,
                                                   const js_value_t args_p[],
                                                   const js_length_t args_count);

/* Native free callback of an object. */
typedef void (*js_object_native_free_callback_t) (void *native_p);

/* Type information of a native pointer. */
typedef struct
{
  js_object_native_free_callback_t free_cb; /**< the free callback of the native pointer */
} js_object_native_info_t;

typedef bool (*js_objects_foreach_t) (const js_value_t object,
                                         void *user_data_p);

typedef bool (*js_object_property_foreach_t) (const js_value_t property_name,
                                                 const js_value_t property_value,
                                                 void *user_data_p);

/* Get the global object. */
js_value_t js_get_global_object(void);

/* Checker js value type. */
bool js_value_is_abort(const js_value_t value);
bool js_value_is_array(const js_value_t value);
bool js_value_is_boolean(const js_value_t value);
bool js_value_is_constructor(const js_value_t value);
bool js_value_is_error(const js_value_t value);
bool js_value_is_function(const js_value_t value);
bool js_value_is_number(const js_value_t value);
bool js_value_is_null(const js_value_t value);
bool js_value_is_object(const js_value_t value);
bool js_value_is_promise(const js_value_t value);
bool js_value_is_string(const js_value_t value);
bool js_value_is_undefined(const js_value_t value);

/* Get js value type information. */
js_type_t js_value_get_type(const js_value_t value);

/* Error manipulation functions. */
js_value_t js_create_abort_from_value(js_value_t value, bool release);
js_value_t js_create_error_from_value(js_value_t value, bool release);
js_value_t js_get_value_from_error(js_value_t value, bool release);

/* Error object type */
js_error_t js_get_error_type(js_value_t value);

/* Getter value. */
bool js_get_boolean_value(const js_value_t value);
double js_get_number_value(const js_value_t value);

/* Functions for string values. */
js_size_t js_get_string_size(const js_value_t value);
js_size_t js_get_utf8_string_size(const js_value_t value);
js_length_t js_get_string_length(const js_value_t value);
js_length_t js_get_utf8_string_length(const js_value_t value);
js_size_t js_string_to_char_buffer(const js_value_t value, js_char_t *buffer_p, js_size_t buffer_size);
js_size_t js_string_to_utf8_char_buffer(const js_value_t value, js_char_t *buffer_p, js_size_t buffer_size);
js_size_t js_substring_to_char_buffer(const js_value_t value,
                                             js_length_t start_pos,
                                             js_length_t end_pos,
                                             js_char_t *buffer_p,
                                             js_size_t buffer_size);
js_size_t js_substring_to_utf8_char_buffer(const js_value_t value,
                                                  js_length_t start_pos,
                                                  js_length_t end_pos,
                                                  js_char_t *buffer_p,
                                                  js_size_t buffer_size);


uint32_t js_get_array_length(const js_value_t value);

bool js_value_to_boolean(const js_value_t value);
js_value_t js_value_to_number(const js_value_t value);
js_value_t js_value_to_object(const js_value_t value);
js_value_t js_value_to_primitive(const js_value_t value);
js_value_t js_value_to_string(const js_value_t value);

/* Acquire types with reference counter (increase the references). */
js_value_t js_acquire_value(js_value_t value);

/* Release the referenced values. */
void js_release_value(js_value_t value);

/* Create functions of API values. */
js_value_t js_create_array(uint32_t size);
js_value_t js_create_boolean(bool value);
js_value_t js_create_error(js_error_t error_type, const js_char_t *message_p);
js_value_t js_create_error_sz(js_error_t error_type, const js_char_t *message_p, js_size_t message_size);
js_value_t js_create_external_function(js_external_handler_t handler_p);
js_value_t js_create_number(double value);
js_value_t js_create_number_infinity(bool sign);
js_value_t js_create_number_nan(void);
js_value_t js_create_null(void);
js_value_t js_create_object(void);
js_value_t js_create_promise(void);
js_value_t js_create_string_from_utf8(const js_char_t *str_p);
js_value_t js_create_string_sz_from_utf8(const js_char_t *str_p, js_size_t str_size);
js_value_t js_create_string(const js_char_t *str_p);
js_value_t js_create_string_sz(const js_char_t *str_p, js_size_t str_size);
js_value_t js_create_undefined(void);

/* General API functions of JS objects. */
js_value_t js_has_property(const js_value_t obj_val, const js_value_t prop_name_val);
bool js_delete_property(const js_value_t obj_val, const js_value_t prop_name_val);
bool js_delete_property_by_index(const js_value_t obj_val, uint32_t index);

js_value_t js_get_property(const js_value_t obj_val, const js_value_t prop_name_val);
js_value_t js_get_property_by_index(const js_value_t obj_val, uint32_t index);
js_value_t js_set_property(const js_value_t obj_val, const js_value_t prop_name_val,
                                  const js_value_t value_to_set);
js_value_t js_set_property_by_index(const js_value_t obj_val, uint32_t index,
                                           const js_value_t value_to_set);

js_value_t js_call_function(const js_value_t func_obj_val, const js_value_t this_val,
                                   const js_value_t args_p[], js_size_t args_count);
js_value_t js_construct_object(const js_value_t func_obj_val, const js_value_t args_p[],
                                      js_size_t args_count);

js_value_t js_get_object_keys(const js_value_t obj_val);
js_value_t js_get_prototype(const js_value_t obj_val);
js_value_t js_set_prototype(const js_value_t obj_val, const js_value_t proto_obj_val);

bool js_get_object_native_pointer(const js_value_t obj_val,
                                      void **out_native_pointer_p,
                                      const js_object_native_info_t **out_pointer_info_p);
void js_set_object_native_pointer(const js_value_t obj_val,
                                      void *native_pointer_p,
                                      const js_object_native_info_t *native_info_p);

bool js_objects_foreach(js_objects_foreach_t foreach_p,
                            void *user_data);

bool js_foreach_object_property(const js_value_t obj_val, js_object_property_foreach_t foreach_p,
                                    void *user_data_p);

js_value_t js_json_parse(const js_char_t *string_p, js_size_t string_size);
js_value_t js_json_stringfy(const js_value_t object);

js_value_t js_handler_register_global(const js_char_t *name_p, js_external_handler_t handler_p);

bool js_is_valid_utf8_string(const char *str, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
