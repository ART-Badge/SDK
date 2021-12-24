/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-18     tyx          first implementation
 */

#ifndef __SIM_TYPE_H__
#define __SIM_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

// basic
typedef char                        sim_char_t;
typedef unsigned char               sim_uchar_t;
typedef short                       sim_short_t;
typedef unsigned short              sim_ushort_t;
typedef int                         sim_int_t;
typedef unsigned int                sim_uint_t;
typedef long                        sim_long_t;
typedef unsigned long               sim_ulong_t;
typedef void*                       sim_pointer_t;
typedef void const *                sim_cpointer_t;

typedef signed char                 sim_int8_t;
typedef unsigned char               sim_uint8_t;
typedef signed short                sim_int16_t;
typedef unsigned short              sim_uint16_t;
typedef signed int                  sim_int32_t;
typedef unsigned int                sim_uint32_t;
typedef signed long long            sim_int64_t;
typedef unsigned long long          sim_uint64_t;

typedef sim_int_t                   sim_bool_t;
typedef sim_uint8_t                 sim_byte_t;
typedef sim_pointer_t               sim_handle_t;
typedef sim_ulong_t                 sim_size_t;
typedef sim_ulong_t                 sim_flag_t;
typedef sim_ulong_t                 sim_type_t;
typedef sim_uint64_t                sim_time_t;
typedef sim_long_t                  sim_err_t;
typedef sim_long_t                  sim_off_t;

//typedef union __sim_value_t
//{
//    sim_char_t                  c;
//    sim_uchar_t                 uc;
//    sim_short_t                 s;
//    sim_ushort_t                us;
//    sim_int_t                   i;
//    sim_uint_t                  ui;
//    sim_long_t                  l;
//    sim_ulong_t                 ul;
//    sim_int8_t                  i8;
//    sim_uint8_t                 u8;
//    sim_int16_t                 i16;
//    sim_uint16_t                u16;
//    sim_int32_t                 i32;
//    sim_uint32_t                u32;
//    sim_int64_t                 i64;
//    sim_uint64_t                u64;
//    sim_bool_t                  b;
//    sim_pointer_t               ptr;
//    sim_atomic_t                a;
//}sim_value_t;

typedef struct __sim_object_t
{
    sim_handle_t parent;
}sim_object_t;

// bool
#define SIM_TRUE                     ((sim_bool_t)1)
#define SIM_FALSE                    ((sim_bool_t)0)

// error
#define SIM_OK                       ((sim_err_t)0)
#define SIM_ERROR                    ((sim_err_t)1)
#define SIM_TIMEOUT                  ((sim_err_t)2)
#define SIM_BUSY                     ((sim_err_t)3)
#define SIM_INVAL                    ((sim_err_t)4)

// null
#define SIM_NULL                  ((sim_pointer_t)0)

// limit
#define SIM_CHAR_BIT      8
#define SIM_SCHAR_MIN   (-128)
#define SIM_SCHAR_MAX     127
#define SIM_UCHAR_MAX     0xff

#define SIM_SHRT_MIN    (-32768)
#define SIM_SHRT_MAX      32767
#define SIM_USHRT_MAX     0xffff
#define SIM_INT_MIN     (-2147483647 - 1)
#define SIM_INT_MAX       2147483647
#define SIM_UINT_MAX      0xffffffff
#define SIM_LONG_MIN    (-2147483647L - 1)
#define SIM_LONG_MAX      2147483647L
#define SIM_ULONG_MAX     0xffffffffUL
#define SIM_LLONG_MAX     9223372036854775807i64
#define SIM_LLONG_MIN   (-9223372036854775807i64 - 1)
#define SIM_ULLONG_MAX    0xffffffffffffffffui64

#define SIM_INT8_MIN     (-127i8 - 1)
#define SIM_INT8_MAX       127i8
#define SIM_UINT8_MAX      0xffui8

#define SIM_INT16_MIN    (-32767i16 - 1)
#define SIM_INT16_MAX      32767i16
#define SIM_UINT16_MAX     0xffffui16

#define SIM_INT32_MIN    (-2147483647i32 - 1)
#define SIM_INT32_MAX      2147483647i32
#define SIM_UINT32_MAX     0xffffffffui32

#define SIM_INT64_MIN    (-9223372036854775807i64 - 1)
#define SIM_INT64_MAX      9223372036854775807i64
#define SIM_UINT64_MAX     0xffffffffffffffffui64

#ifdef __cplusplus
}
#endif

#endif /* __SIM_TYPE_H__ */
