/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      profile_init.h
* @brief
* @details
* @author    Chuanguo Xue
* @date      2015-3-27
* @version   v0.1
* *********************************************************************************************************
*/
#ifndef _PROFILE_INIT_H
#define _PROFILE_INIT_H

#ifdef __cplusplus
extern "C" {
#endif
#include "rtl876x.h"
#include "gap_msg.h"

extern void *con_parameter_update_timer;

void wristband_start_adv(void);
void wristband_stop_adv(void);
uint8_t wristband_adv_data_init(uint8_t *device_name);
void wristband_update_con_para(bool use_latency);
void wristband_gap_timer_init(void);
void start_connection_paramter_timer(void);
void start_ancs_rediscovery_timer(void);
void stop_connection_paramter_timer(void);
bool wristband_get_peer_public_bd_addr(uint8_t conn_id, uint8_t *p_bd_addr);
void bee2_magic_adv(void);
void wristband_gap_dev_change_cb(T_GAP_DEV_STATE cb_dev_state, T_GAP_CONN_STATE cb_con_state);
void wristband_gap_con_change_cb(T_GAP_CONN_STATE cb_con_state, T_GAP_DEV_STATE cb_dev_state);


#ifdef __cplusplus
}
#endif

#endif /* _PROFILE_INIT_H */

