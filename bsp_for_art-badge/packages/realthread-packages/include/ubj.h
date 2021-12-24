/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-18     tyx          first implementation
 */

#ifndef __UBJ_H__
#define __UBJ_H__

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UBJ_OK = 0,
    UBJ_ERROR,              // General error
    UBJ_NOMEM_ERROR,        // insufficient memory
    UBJ_PARAM_ERROR,        // Parameter error
    UBJ_PARSE_ERROR,        // Parsing error
    UBJ_WRITE_ERROR,        // write error
    UBJ_READ_ERROR,         // Read error
    UBJ_UNKNOWN_ERROR = -1, // Occupancy, compulsory ubj_err_t is a signed type
} ubj_err_t;

typedef enum
{
    UBJ_TYPE_MIXED = 0,             // Mixed type for containers
    UBJ_TYPE_NULL,                  // NULL
    UBJ_TYPE_NOOP,                  // NOOP
    UBJ_TYPE_TRUE,                  // TRUE
    UBJ_TYPE_FALSE,                 // FALSE
    UBJ_TYPE_CHAR,                  // CHAR
    UBJ_TYPE_STRING,                // STRING
    UBJ_TYPE_HIGH_PRECISION,        // PRECISION(STRING)
    UBJ_TYPE_INT8,                  // INTEGER
    UBJ_TYPE_UINT8,                 // UINT8
    UBJ_TYPE_INT16,                 // UINT16
    UBJ_TYPE_INT32,                 // UINT32
    UBJ_TYPE_INT64,                 // UINT64
    UBJ_TYPE_FLOAT32,               // FLOAT
    UBJ_TYPE_FLOAT64,               // DOUBLE
    UBJ_TYPE_ARRAY,                 // ARRAY
    UBJ_TYPE_OBJECT,                // OBJECT
    UBJ_NUM_TYPES                   // This is not a type, it only represents the number of types
} ubj_type_t;

#define UBJ_CONTAINER_IS_SIZED      0x1     /* Size properties */
#define UBJ_CONTAINER_IS_TYPED      0x2     /* Type properties */
#define UBJ_CONTAINER_IS_ARRAY      0x4     /* Array properties */
#define UBJ_CONTAINER_IS_OBJECT     0x8     /* Object properties */
#define UBJ_CONTAINER_EXPECTS_KEY   0x10    /* Keyword properties */
#define UBJ_CONTAINER_IGNORE_FLAG   0x20    /* Bare data attribute */

struct ubjson_container
{
    uint8_t flags;          // Container related marking
    int8_t type;            // Type of container
    int16_t remaining;      // Total container size
};

#ifndef UBJ_CONTAINER_STACK_DEPTH
#define UBJ_CONTAINER_STACK_DEPTH   16
#endif

typedef struct ubjsonw_object ubjsonw_t;
typedef struct ubjsonr_object ubjsonr_t;

/*
 * TODO:
 *     Distinguish between ordinary error and unrecoverable error.
 *     Unrecoverable error occurs and all operations are invalid
*/

struct ubjsonw_object
{
    /* Write callback. When the data pointer is null, it means that a 0 is written */
    size_t (*write)(ubjsonw_t *ubj, const void *data, size_t size, void *userdata);
    void (*close)(ubjsonw_t *ubj, void *userdata);   // close callback
    void *userdata;                     // uasr data
    size_t total;                       // write total
    struct ubjson_container stack[UBJ_CONTAINER_STACK_DEPTH];
    int16_t stack_depth;
    int16_t stack_point;
    ubj_err_t error;
};

#define UBJ_ITERATOR_SKIP_SUBOBJECT  0x2
#define UBJ_ITERATOR_PICK_VALID      0x4
#define UBJ_ITERATOR_READ_END        0x8

struct ubjson_iterator
{
    size_t promise_pos;                     // Expected starting position
    uint8_t head_value;                     // Record the value currently read
    uint8_t flags;                          // Mark, whether to access sub elements, whether pick is valid
};

#define UBJ_CONTAINER_ENTER_FLAG   0x01    /* Enter container */
#define UBJ_CONTAINER_EXIT_FLAG    0x02    /* Exit container */
#define UBJ_CONTAINER_ACCESS_FLAG  0x03    /* Access container */

typedef struct
{
    ubj_type_t type;
    ubj_err_t error;                        /* error code */
    uint8_t flags;                          /* Mark whether it is currently in a container, 
                                               whether it is a strong type, fixed size container */
    struct
    {
        ubj_type_t type;                    /* Type object container, array container */
        uint8_t act;                        /* Enter container, iterate container, exit container */
        int16_t level;                      /* Hierarchy of containers */
        int32_t count;                      /* Number of unreachable elements in container */
        int32_t keylen;                     /* Key string length */
        char *key;                          /* key */
    } container;
    union
    {
        char vchar;
        uint8_t vbool;
        uint8_t vuint8;
        int8_t vint8;
        int16_t vint16;
        int32_t vint32;
        float vfloat;
        double vdouble;
        int64_t vint64;
        struct
        {
            char *string;
            size_t size;
        } vstring;
        struct
        {
            char *hpstring;
            size_t size;
        }vhp;
    } value;
} ubj_value_t;

struct _ubjsonr_mem
{
    size_t size;                    // cache size
    size_t used;                    // cache used size
    ubj_value_t value;
};

struct ubjsonr_object
{
    /* Read callback. When the data pointer is null, it means that it is offset by a length */
    size_t (*read)(ubjsonr_t *ubj, void *data, size_t size, void *userdata);
    void (*close)(ubjsonr_t *ubj, void *userdata);   // close callback
    void *userdata;                     // user data
    size_t total;                       // read total
    struct ubjson_iterator iterator;    // iterator
    struct _ubjsonr_mem *mem;            // read cache
    struct ubjson_container stack[UBJ_CONTAINER_STACK_DEPTH];
    int16_t stack_depth;
    int16_t stack_point;
    ubj_err_t error;                    // error code
};

/* object write API */
ubjsonw_t *ubj_write_init(ubjsonw_t* ubj,
    size_t (*write)(ubjsonw_t *, const void *, size_t, void *),
    void (*close)(ubjsonw_t *, void *),
    void *userdata);
size_t ubj_write_end(ubjsonw_t* ubj);

/* container API */
ubj_err_t ubj_begin_array(ubjsonw_t *ubj);
ubj_err_t ubj_begin_fixed_array(ubjsonw_t *ubj, ubj_type_t type, long count);
ubj_err_t ubj_end_array(ubjsonw_t *ubj);
ubj_err_t ubj_begin_object(ubjsonw_t *ubj);
ubj_err_t ubj_begin_fixed_object(ubjsonw_t *ubj, ubj_type_t type, long count);
ubj_err_t ubj_end_object(ubjsonw_t *ubj);
ubj_err_t ubj_write_key(ubjsonw_t *ubj, const char *key);

/* Fill data API */
ubj_err_t ubj_write_string(ubjsonw_t *ubj, const char *v);
ubj_err_t ubj_write_char(ubjsonw_t *ubj, char v);
ubj_err_t ubj_write_uint8(ubjsonw_t *ubj, uint8_t v);
ubj_err_t ubj_write_int8(ubjsonw_t *ubj, int8_t v);
ubj_err_t ubj_write_int16(ubjsonw_t *ubj, int16_t v);
ubj_err_t ubj_write_int32(ubjsonw_t *ubj, int32_t v);
ubj_err_t ubj_write_int64(ubjsonw_t *ubj, int64_t v);
ubj_err_t ubj_write_high_precision(ubjsonw_t *ubj, const char *hp);
ubj_err_t ubj_write_integer(ubjsonw_t *ubj, int64_t v);

ubj_err_t ubj_write_float32(ubjsonw_t *ubj, float v);
ubj_err_t ubj_write_float64(ubjsonw_t *ubj, double v);

ubj_err_t ubj_write_noop(ubjsonw_t *ubj);
ubj_err_t ubj_write_null(ubjsonw_t *ubj);
ubj_err_t ubj_write_true(ubjsonw_t *ubj);
ubj_err_t ubj_write_false(ubjsonw_t *ubj);

/* write key and value */
ubj_err_t ubj_object_write_string(ubjsonw_t *ubj, const char *key, const char *v);
ubj_err_t ubj_object_write_char(ubjsonw_t *ubj, const char *key, char v);
ubj_err_t ubj_object_write_uint8(ubjsonw_t *ubj, const char *key, uint8_t v);
ubj_err_t ubj_object_write_int8(ubjsonw_t *ubj, const char *key, int8_t v);
ubj_err_t ubj_object_write_int16(ubjsonw_t *ubj, const char *key, int16_t v);
ubj_err_t ubj_object_write_int32(ubjsonw_t *ubj, const char *key, int32_t v);
ubj_err_t ubj_object_write_int64(ubjsonw_t *ubj, const char *key, int64_t v);
ubj_err_t ubj_object_write_high_precision(ubjsonw_t *ubj, const char *key, const char *hp);
ubj_err_t ubj_object_write_integer(ubjsonw_t *ubj, const char *key, int64_t v);

ubj_err_t ubj_object_write_float32(ubjsonw_t *ubj, const char *key, float v);
ubj_err_t ubj_object_write_float64(ubjsonw_t *ubj, const char *key, double v);

ubj_err_t ubj_object_write_null(ubjsonw_t *ubj, const char *key);
ubj_err_t ubj_object_write_true(ubjsonw_t *ubj, const char *key);
ubj_err_t ubj_object_write_false(ubjsonw_t *ubj, const char *key);

ubj_err_t ubj_object_write_object(ubjsonw_t *ubj, const char *key);
ubj_err_t ubj_object_write_array(ubjsonw_t *ubj, const char *key);
ubj_err_t ubj_object_write_fixed_object(ubjsonw_t *ubj, const char *key, ubj_type_t type, long count);
ubj_err_t ubj_object_write_fixed_array(ubjsonw_t *ubj, const char *key, ubj_type_t type, long count);

/* Not implemented */
ubj_err_t ubj_write_buffer(ubjsonw_t *ubj, const uint8_t *data, ubj_type_t type, size_t count);

/* ubj read API */
ubjsonr_t *ubj_read_init(ubjsonr_t* ubj, 
    size_t (*read)(ubjsonr_t *, void *, size_t, void *),
    void (*close)(ubjsonr_t *, void *),
    void *userdata);
const ubj_value_t *ubj_read_next(ubjsonr_t *ubj);
size_t ubj_read_end(ubjsonr_t* ubj);
ubj_err_t ubj_skip_subobject(ubjsonr_t* ubj);         /* Skip child elements */

/* port */
void* ubj_malloc(unsigned int size);
void* ubj_realloc(void* rmem, unsigned int newsize);
void ubj_free(void* rmem);
int ubj_printf(const char* format, ...);

/* other API */
ubjsonr_t *ubj_read_memory(const void *buff, size_t len);
ubjsonw_t *ubj_write_memory(void);
uint8_t *ubj_get_memory(ubjsonw_t *ubj);
ubjsonw_t *ubj_write_file(const char *path);
ubjsonr_t *ubj_read_file(const char *path);
ubj_err_t ubj_to_json(ubjsonr_t *ubj, char **buff, size_t *len); /* UBJSON to JSON */

#ifdef __cplusplus
}
#endif
#endif
