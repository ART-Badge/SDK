/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */

#ifndef __BSAL_SRV_BAS_H__
#define __BSAL_SRV_BAS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include "bsal.h"
#define GATT_UUID_BATTERY                       BSAL_GATT_SERVICE_BATTERY_SERVICE
#define GATT_UUID_CHAR_BAS_LEVEL                BSAL_UUID_CHAR_BATTERY_LEVEL

#define GATT_SVC_BAS_BATTERY_LEVEL_INDEX        2 /**< @brief Index for battery level chars's value */
#define GATT_SVC_BAS_CHAR_CCCD_INDEX            (GATT_SVC_BAS_BATTERY_LEVEL_INDEX+1) /**< @brief CCCD Index for battery level chars's value */
/**
* BSAL battery service init
*
* @param stack_ptr the point of the stack object
* @param app_callback the callback of the service
* @Note init the service
*/
void bsal_le_bas_svr_init(void *stack_ptr, void *app_callback);

/**
* BSAL set the battery level local
*
* @param stack_ptr the point of the stack object
* @param app_callback the callback of the service
* @Note init the service
*/
void bsal_set_battery_level_static(uint8_t battery_level);

/**
* BSAL set the battery level and send notify
*
* @param stack_ptr the point of the stack object
* @param conn_id the conn_handle of the link
* @param battery_level the battery level of the battery service
* @Note set the notify level
*/
void bsal_bas_send_notify_level(void *stack_ptr, uint16_t conn_id,  uint8_t battery_level);

#ifdef __cplusplus
}
#endif
#endif
