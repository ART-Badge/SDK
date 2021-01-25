/**
************************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     dfu_main.h
* @brief    Normal ota APIs
* @details
* @author
* @date
* @version
*************************************************************************************************************
*/

#ifndef _DFU_MAIN_H_
#define  _DFU_MAIN_H_

#include <stdint.h>
#include <stdbool.h>
#include "profile_server.h"

extern void *total_timer_handle;
extern void *wait4_conn_timer_handle;
extern void *image_transfer_timer_handle;
extern void *ctittv_timer_handle;
extern uint32_t timeout_value_total;
extern uint32_t timeout_value_wait4_conn;
extern uint32_t timeout_value_image_transfer;
extern uint32_t timeout_value_ctittv;
extern T_SERVER_ID rtk_dfu_service_id;

void dfu_timer_init(void);

void dfu_set_rand_addr(void);

void dfu_main(void);

#endif
