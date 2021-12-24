/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-03     RTT       the first version
 */
#ifndef _BS_DEF_H_
#define _BS_DEF_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rtconfig.h"

/* bsdiff default using in bootloader */
#if defined(BSDIFF_LIB_USING_IN_BOOTLOADER) || defined(BSDIFF_LIB_USING_IN_APP)
#else
#define BSDIFF_LIB_USING_IN_BOOTLOADER
#endif
/* bsdiff library software version */
#define BSDIFF_LIB_SW_VERSION               "0.0.1"
/* download partition name */
#ifndef BSDIFF_LIB_DOWNLOAD_PARTITION_NAME
#define BSDIFF_LIB_DOWNLOAD_PARTITION_NAME  "download"
#endif
/* download partition length unit: KB */
#ifndef BSDIFF_LIB_DOWNLOAD_PARTITION_LEN
#define BSDIFF_LIB_DOWNLOAD_PARTITION_LEN   (1 * 1024)
#endif
/* app partition name */
#ifndef BSDIFF_LIB_APP_PARTITION_NAME
#define BSDIFF_LIB_APP_PARTITION_NAME       "app"
#endif
/* app partition length unit: KB */
#ifndef BSDIFF_LIB_APP_PARTITION_LEN
#define BSDIFF_LIB_APP_PARTITION_LEN        (1 * 1024)
#endif
/* image backup partition name */
#ifndef BSDIFF_LIB_BACKUP_PARTITION_NAME
#define BSDIFF_LIB_BACKUP_PARTITION_NAME    "bk_image"
#endif
/* image backup partition length unit: KB */
#ifndef BSDIFF_LIB_BACKUP_PARTITION_LEN
#define BSDIFF_LIB_BACKUP_PARTITION_LEN     (1 * 1024)
#endif
/* new image partition name */
#ifndef BSDIFF_LIB_NEW_IMAGE_PARTITION_NAME
#define BSDIFF_LIB_NEW_IMAGE_PARTITION_NAME "app"
#endif
/* new image partition length unit: KB */
#ifndef BSDIFF_LIB_NEW_IMAGE_PARTITION_LEN
#define BSDIFF_LIB_NEW_IMAGE_PARTITION_LEN  (1 * 1024)
#endif
/* args partition name */
#ifndef BSDIFF_LIB_ARGS_PARTITION_NAME
#define BSDIFF_LIB_ARGS_PARTITION_NAME      "args"
#endif
/* args partition length */
#ifndef BSDIFF_LIB_ARGS_PARTITION_LEN
#define BSDIFF_LIB_ARGS_PARTITION_LEN       (1 * 1024)
#endif
/* recovery partition name */
#ifndef BSDIFF_LIB_RECOVERY_PARTITION_NAME
#define BSDIFF_LIB_RECOVERY_PARTITION_NAME  "recovery"
#endif
/* recovery partition length unit: KB */
#ifndef BSDIFF_LIB_RECOVERY_PARTITION_LEN
#define BSDIFF_LIB_RECOVERY_PARTITION_LEN   (1 * 1024)
#endif

/* bsdiff default extractor name */
#ifndef BSDIFF_LIB_DEFAULT_EXTRACTOR_NAME
#define BSDIFF_LIB_DEFAULT_EXTRACTOR_NAME   "zlib"
#endif

/* diff file header version */
#ifndef BSDIFF_VERSION
#define BSDIFF_VERSION                      "BSDIFF43"
#endif
/* buffer chunk size */
#define BS_PATCH_CHUNK              (512)
/* bapatch buffer size */
#define BS_COPY_CHUNK               (512)
/* error code */
#define BS_OK                       (  0)
#define BS_DIFF_UPDATE              (  1)
#define BS_COMPLETE_UPDATE          (  2)
#define BS_CRC_ERROR                ( -1)
#define BS_DATA_ERROR               ( -2)
#define BS_MEDIUM_ERROR             ( -3)
#define BS_DIFF_PART_ERROR          ( -4)
#define BS_DIFF_HEADER_ERROR        ( -5)
#define BS_DIFF_CRC_ERROR           ( -6)
#define BS_IMAGE_BACKUP_ERROR       ( -7)
#define BS_IMAGE_CRC_ERROR          ( -8)
#define BS_APP_PART_ERROR           ( -9)
#define BS_APP_ERASE_ERROR          (-10)
#define BS_DIFF_EXTRACT_ERROR       (-11)
#define BS_PATCH_ERROR              (-12)
#define BS_LAST_UPDATE_FAILED       (-13)

#ifndef BS_MEMCMP
#define BS_MEMCMP                   memcmp
#endif

#ifndef BS_MEMSET
#define BS_MEMSET                   memset
#endif

#ifndef BS_MEMCPY
#define BS_MEMCPY                   memcpy
#endif

#ifndef BS_STRCMP
#define BS_STRCMP                   strcmp
#endif

#ifndef BS_PRINTF
#ifdef RT_VER_NUM
/* for RT-Thread platform */
extern void rt_kprintf(const char *fmt, ...);
#define BS_PRINTF                   rt_kprintf
#else
#define BS_PRINTF                   printf
#endif /* RT_VER_NUM */
#endif /* FAL_PRINTF */

/* define assert */
#ifndef BS_ASSERT
#define BS_ASSERT(EXPR)                                                     \
if (!(EXPR))                                                                \
{                                                                           \
    BS_PRINTF("(%s) has assert failed at %s.\n", #EXPR, __FUNCTION__);      \
    while (1);                                                              \
}
#endif /* BS_ASSERT */

/* error level log */
#ifdef  bs_log_e
#undef  bs_log_e
#endif
#define bs_log_e(...)               BS_PRINTF("\033[31;22m[E/BS] (%s:%d) ", __FUNCTION__, __LINE__);BS_PRINTF(__VA_ARGS__);BS_PRINTF("\033[0m\n")

/* info level log */
#ifdef  bs_log_i
#undef  bs_log_i
#endif
#define bs_log_i(...)               BS_PRINTF("\033[32;22m[I/BS] ");                                BS_PRINTF(__VA_ARGS__);BS_PRINTF("\033[0m\n")

/* debug level log */
#ifdef  bs_log_d
#undef  bs_log_d
#endif
#define bs_log_d(...)               BS_PRINTF("[D/BS] (%s:%d) ", __FUNCTION__, __LINE__);           BS_PRINTF(__VA_ARGS__);BS_PRINTF("\n")

#endif /* _BS_DEF_H_ */
