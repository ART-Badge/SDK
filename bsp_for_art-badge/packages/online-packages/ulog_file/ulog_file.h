/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-07     ChenYong     first version
 */

#ifndef _ULOG_FILE_H_
#define _ULOG_FILE_H_

#define ULOG_FILE_SW_VERSION      "1.0.0"
#define ULOG_FILE_SW_VERSION_NUM  0x0100000

int ulog_file_backend_init(void);
int ulog_file_backend_deinit(void);

#endif /* _ULOG_FILE_H_ */
