/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-18     tyx          first implementation
 */

#ifndef __UBJ_INTERNAL_H__
#define __UBJ_INTERNAL_H__

#define UBJ_BIGENDIAN16(_in, _out)  \
    do {    \
        uint32_t isbig = 1; \
        if (*((uint8_t*)&isbig) == 1)   \
        {   \
            uint8_t *__o = (uint8_t *)(_out);  \
            uint8_t *__i = (uint8_t *)(_in); \
            *__o++ = __i[1]; \
            *__o++ = __i[0]; \
        }   \
        else    \
        {   \
            memcpy(_out, _in, sizeof(int16_t));  \
        }   \
    } while (0)

#define UBJ_BIGENDIAN32(_in, _out)  \
    do {    \
        uint32_t isbig = 1; \
        if (*((uint8_t*)&isbig) == 1)   \
        {   \
            uint8_t *__o = (uint8_t *)(_out);  \
            uint8_t *__i = (uint8_t *)(_in); \
            *__o++ = __i[3]; \
            *__o++ = __i[2]; \
            *__o++ = __i[1]; \
            *__o++ = __i[0]; \
        }   \
        else    \
        {   \
            memcpy(_out, _in, sizeof(int32_t));  \
        }   \
    } while (0)

#define UBJ_BIGENDIAN64(_in, _out)  \
    do {    \
        uint32_t isbig = 1; \
        if (*((uint8_t*)&isbig) == 1)   \
        {   \
            uint8_t *__o = (uint8_t *)(_out);  \
            uint8_t *__i = (uint8_t *)(_in); \
            *__o++ = __i[7]; \
            *__o++ = __i[6]; \
            *__o++ = __i[5]; \
            *__o++ = __i[4]; \
            *__o++ = __i[3]; \
            *__o++ = __i[2]; \
            *__o++ = __i[1]; \
            *__o++ = __i[0]; \
        }   \
        else    \
        {   \
            memcpy(_out, _in, sizeof(int32_t));  \
        }   \
    } while (0)

#define UBJ_CONTAINER_WALK()    (++ubj->stack_point < ubj->stack_depth ? &ubj->stack[ubj->stack_point] : NULL)
#define UBJ_CONTAINER_POP()     (ubj->stack_point > 0 ? ubj->stack_point-- : -1)
#define UBJ_CONTAINER_PICK()    (&ubj->stack[ubj->stack_point])

#define UBJ_TYPE_TAB    "\x00ZNTFCSHiUIlLdD[{"

#endif
