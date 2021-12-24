/*
 * Copyright (c) 2020, Armink, <armink.ztl@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief configuration file
 */

#ifndef _FDB_CFG_H_
#define _FDB_CFG_H_

/* #define FDB_USING_FILE_POSIX_MODE */
/* #define FDB_USING_FAL_MODE */
/* more configuration macro is in rtconfig.h */

#include <rtthread.h>
#ifdef RT_USING_DFS
#include <dfs_posix.h>
#endif

extern void rt_kprintf(const char *fmt, ...);
#define FDB_PRINT(...)             rt_kprintf(__VA_ARGS__)


#endif /* _FDB_CFG_H_ */
