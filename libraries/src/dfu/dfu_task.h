
/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _DFU_TASK_H_
#define _DFU_TASK_H_
#include <stdint.h>
#include <stdbool.h>
#include "app_msg.h"


void dfu_task_init(void);
bool app_send_msg_to_dfutask(T_IO_MSG *p_msg);

#endif

