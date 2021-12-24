/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-16     RTT       the first version
 */
#ifndef __UGUI_PORT_H__
#define __UGUI_PORT_H__

#include "ugui.h"

int ugui_port_init(void);
void ugui_port_deinit(void);
UG_COLOR UG_ReadPixel(UG_S16 x, UG_S16 y);
UG_GUI *UG_GetGui(void);
UG_S32 UG_GetLcdWidth(void);
UG_S32 UG_GetLcdHeight(void);

#endif /* __UGUI_PORT_H__ */