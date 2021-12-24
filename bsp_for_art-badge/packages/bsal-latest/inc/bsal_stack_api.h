/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#ifndef __BSAL_STACK_API_H__
#define __BSAL_STACK_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsal_common.h"

#include <stdbool.h>
#include <stdint.h>


/**
* BSAL compute the sum of the srv handle number.
*
* @param[in] p_srv the service table of the p_srv
* @param[out] att_res the result of the att resource
* @Note  compute the number of the service
*/
uint8_t bsal_util_sum_handle_num(struct bsal_gatt_app_srv_def *p_srv, bsal_gatt_res_t *att_res);

/**
* BSAL save the profile
*
* @param[in] stack_ptr the point of the stack ptr
* @param[in] handle the start_handle of the device
* @param[in] callback the callback function of the service
* @param[in] uuid the uuid of the service
* @Note
*/
BSAL_STATUS bsal_profile_insert(void *stack_ptr, uint16_t handle, void *callback, bsal_uuid_any_t uuid);

/**
* BSAL Stack ready callback
*
* @param[in] stack_ptr the point of the stack ptr
* @param[in] status true is the stack ready, false is stack fail
* @Note
*/
int bsal_stack_ready(void *stack_ptr, bool status);

/**
* BSAL Stack ready callback
*
* @param[in] stack_ptr the point of the stack ptr
* @param[in] conn_handle the conn_handle of the link
* @param[in] conn_status the connect status of the link
* @param[in] disconnect_status the disconnect reason of the link
* @Note  callback the link status of the devcie
*/
int bsal_gap_connect_status(void *stack_ptr, uint16_t conn_handle, uint8_t connect_status, uint16_t disconnect_status);

#ifdef __cplusplus
}
#endif

#endif


