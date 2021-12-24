/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-22     liukang      first version
 * 2021-09-08	  WJL		   second version
 */

#ifndef __UDBD_H
#define __UDBD_H


#ifdef __cplusplus
extern "C" {
#endif

enum udbd_link_device_type
{
    UDBD_LINK_UART = 0,
    UDBD_LINK_USB,
};
int udbd_init(int type, char* udbd_name);

#ifdef  __cplusplus
}
#endif

#endif  /* __UDBD_H__ */
