/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      simple_ble_peripheral_application.h
* @brief     simple_ble_peripheral_application
* @details   simple_ble_peripheral_application
* @author    jane
* @date      2015-12-22
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _PERIPHERAL_APPLICATION__
#define _PERIPHERAL_APPLICATION__

#ifdef __cplusplus
extern "C" {
#endif
#include <app_msg.h>
#include <gap_le.h>
#include <profile_server.h>

typedef enum
{
    LANTENCY_OFF = 0,
    LANTENCY_UPDATING,
    LANTENCY_ON,
    LANTENCY_ON_CLOSE,
} LantencyStatus;

void app_handle_io_msg(T_IO_MSG io_driver_msg_recv);
T_APP_RESULT app_profile_callback(T_SERVER_ID service_id, void *p_data);
T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data);

#ifdef __cplusplus
}
#endif

#endif

