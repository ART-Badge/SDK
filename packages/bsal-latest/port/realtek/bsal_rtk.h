/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#ifndef __BSAL_RTK_H__
#define __BSAL_RTK_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "os_mem.h"
#include "os_msg.h"
#include "os_task.h"
#include "gap.h"
#include "gap_le.h"
#include "profile_server.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "app_msg.h"
#include "trace.h"
#include "gap_bond_le.h"
#include "gap_msg.h"
#include "gap_conn_le.h"
/**
 * @brief  Initialize App task
 * @return void
 */
void app_task_init(void);
static const T_FUN_GATT_SERVICE_CBS bsal_cbs;
static T_APP_RESULT bsal_app_gap_callback(uint8_t cb_type, void *p_cb_data);
static T_APP_RESULT bsal_app_profile_callback(T_SERVER_ID service_id, void *p_data);
#ifdef __cplusplus
}
#endif



#endif









