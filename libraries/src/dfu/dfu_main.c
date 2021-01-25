/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "profile_server.h"
#include "os_timer.h"
#include "otp_config.h"  //todo: when oepn otp needn't
#include "otp.h"
#include "trace.h"
#include "dfu_api.h"
#include "dfu_flash.h"
#include "dfu_service.h"
#include "dfu_main.h"
#include "dfu_task.h"
#include "dfu_application.h"
#include "board.h"

#if (SUPPORT_NORMAL_OTA == 1)
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define TIMER_ID_DFU_TOTAL              1
#define TIMER_ID_DFU_WAIT4_CONN         2
#define TIMER_ID_DFU_IMAGE_TRANSFER     3
#define TIMER_ID_DFU_CTITTV             4

#define BD_ADDR_SIZE                    6

/* What is the advertising interval when device is discoverable (units of 625us, 160=100ms)*/
#define DEFAULT_ADVERTISING_INTERVAL_MIN            160 /* 100ms */
#define DEFAULT_ADVERTISING_INTERVAL_MAX            176 /* 110ms */

/*============================================================================*
 *                               Types
 *============================================================================*/


/*============================================================================*
 *                               Variables
 *============================================================================*/
void *total_timer_handle;
void *wait4_conn_timer_handle;
void *image_transfer_timer_handle;
void *ctittv_timer_handle;
uint32_t timeout_value_total;
uint32_t timeout_value_wait4_conn;
uint32_t timeout_value_image_transfer;
uint32_t timeout_value_ctittv;
T_SERVER_ID rtk_dfu_service_id;

/*============================================================================*
 *                              Local Functions
 *============================================================================*/
/*
 * @fn          Initial gap parameters
 * @brief      Initialize peripheral and gap bond manager related parameters
 *
 * @return     void
 */
void dfu_le_gap_init(void)
{

    uint8_t bt_bd_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_bd_addr); //get OTP->bt_bd_addr

    //device name and device appearance
    char fmt[] = "%02x%02x%02x%02x%02x%02x";
    char device_name[GAP_DEVICE_NAME_LEN];
    sprintf(device_name, fmt,
            bt_bd_addr[5], bt_bd_addr[4], bt_bd_addr[3],
            bt_bd_addr[2], bt_bd_addr[1], bt_bd_addr[0]
           );
    uint16_t appearance = GAP_GATT_APPEARANCE_KEYBOARD;
    uint8_t  slave_init_mtu_req = true;

    //advertising parameters
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
    uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_RANDOM;
    //advertising data
    uint8_t adv_data_uuid128[31] =
    {
        /* Flags */
        0x02,                           /* length     */
        GAP_ADTYPE_FLAGS,               /* type="flags" */
        GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

        /* Service */
        0x11,                           /* length     */
        GAP_ADTYPE_128BIT_COMPLETE,     /* type="Complete list of 128-bit UUIDs" */
        GATT_UUID128_DFU_SERVICE,

        9,
        0xFF,
        0x5D,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    adv_data_uuid128[25] = bt_bd_addr[5];
    adv_data_uuid128[26] = bt_bd_addr[4];
    adv_data_uuid128[27] = bt_bd_addr[3];
    adv_data_uuid128[28] = bt_bd_addr[2];
    adv_data_uuid128[29] = bt_bd_addr[1];
    adv_data_uuid128[30] = bt_bd_addr[0];

    //scan response data
    uint8_t scan_rsp_data[] =
    {
        /*Complete local name*/
        7,                                    /* default target name length */
        GAP_ADTYPE_LOCAL_NAME_COMPLETE,       /* type="Complete local name" */
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
    };

    scan_rsp_data[2] = OTP->ota_rst_tgt_name[0];
    scan_rsp_data[3] = OTP->ota_rst_tgt_name[1];
    scan_rsp_data[4] = OTP->ota_rst_tgt_name[2];
    scan_rsp_data[5] = OTP->ota_rst_tgt_name[3];
    scan_rsp_data[6] = OTP->ota_rst_tgt_name[4];
    scan_rsp_data[7] = OTP->ota_rst_tgt_name[5];
    scan_rsp_data[8] = OTP->ota_rst_tgt_name[6];
    scan_rsp_data[9] = OTP->ota_rst_tgt_name[7];
    if (scan_rsp_data[9] != '\0')
    {
        scan_rsp_data[0] = 0x09; /* target name length */
    }
    else
    {
        scan_rsp_data[0] = strlen((char *)(scan_rsp_data + 2)) + 1;
    }

    //GAP Bond Manager parameters
    uint8_t  gap_param_cccd_storage = false;
    uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG | GAP_AUTHEN_BIT_MITM_FLAG;
    uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
#ifndef SUPPORT_ALONE_UPPERSTACK_IMG
    uint8_t  auth_oob = false;
#endif
    uint8_t  auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;
    uint8_t  auth_sec_req_enalbe = false;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_NONE;

    //Register gap callback
    le_register_app_cb(dfu_gap_callback);

    //Set device name and device appearance
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                     &slave_init_mtu_req);

    //Set advertising parameters
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
    le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
    le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data_uuid128), adv_data_uuid128);
    le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, scan_rsp_data[0] + 1, scan_rsp_data);

    // Setup the GAP Bond Manager
    le_bond_set_param(GAP_PARAM_BOND_CCCD_STORAGE, sizeof(gap_param_cccd_storage),
                      &gap_param_cccd_storage);
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
#ifndef SUPPORT_ALONE_UPPERSTACK_IMG
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
#endif
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enalbe), &auth_sec_req_enalbe);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

}

/******************************************************************
 * @fn          Initial profile
 * @brief      Add simple profile service and register callbacks
 *
 * @return     void
 */
void dfu_le_profile_init(void)
{
    server_init(1);
    rtk_dfu_service_id = dfu_add_service(dfu_profile_callback);
    server_register_app_cb(dfu_profile_callback);
}

void dfu_monitor_timeout_handler(void *p_xtimer)
{
    uint32_t timer_id = 0;

    os_timer_id_get(&p_xtimer, &timer_id);

    APP_PRINT_ERROR1("dfu_monitor_timeout_handler, TimerID(%u)", timer_id);

    switch (timer_id)
    {
    case TIMER_ID_DFU_TOTAL:
    case TIMER_ID_DFU_WAIT4_CONN:
    case TIMER_ID_DFU_IMAGE_TRANSFER:
    case TIMER_ID_DFU_CTITTV:
        dfu_fw_reboot(false);
        break;
    }
}

void dfu_timer_init(void)
{
    timeout_value_total = OTP->ota_timeout_total * 1000;
    timeout_value_wait4_conn = OTP->ota_timeout_wait4_conn * 1000;
    timeout_value_image_transfer = OTP->ota_timeout_wait4_image_transfer * 1000;
    timeout_value_ctittv = OTP->ota_timeout_ctittv * 1000;

    os_timer_create(&total_timer_handle, "dfuTotalTimer", TIMER_ID_DFU_TOTAL,
                    timeout_value_total, false, dfu_monitor_timeout_handler);

    os_timer_create(&wait4_conn_timer_handle, "dfuWait4ConTimer", TIMER_ID_DFU_WAIT4_CONN,
                    timeout_value_wait4_conn, false, dfu_monitor_timeout_handler);

    os_timer_create(&image_transfer_timer_handle, "dfuImageTransferTimer",
                    TIMER_ID_DFU_IMAGE_TRANSFER, timeout_value_image_transfer,
                    false, dfu_monitor_timeout_handler);

    os_timer_create(&ctittv_timer_handle, "dfuCtittvTimer", TIMER_ID_DFU_CTITTV,
                    timeout_value_ctittv, false, dfu_monitor_timeout_handler);

    os_timer_start(&total_timer_handle);

    os_timer_start(&wait4_conn_timer_handle);
}

void dfu_init(void)
{
    WDG_Disable();

    if (unlock_flash_bp_all())
    {
        DFU_PRINT_INFO0("[==>dfu_init: Flash unlock BP all success!");
    }
    else
    {
        DBG_DIRECT("dfu init unlock BP fail!");
    }
}
/*============================================================================*
 *                              Local Functions
 *============================================================================*/
void dfu_set_rand_addr(void)
{
    T_GAP_RAND_ADDR_TYPE rand_addr_type = GAP_RAND_ADDR_NON_RESOLVABLE;
    uint8_t random_bd[BD_ADDR_SIZE] = {0};
    le_gen_rand_addr(rand_addr_type, random_bd);
    DFU_PRINT_INFO1("dfu_set_rand_addr: rand_addr %b", TRACE_BDADDR(random_bd));
    le_set_rand_addr(random_bd);
}

void dfu_main(void)
{
    DBG_DIRECT("Enter DFU mode");
    le_gap_init(1);
    gap_lib_init();
    dfu_le_gap_init();
    dfu_le_profile_init();
    dfu_init();
    dfu_task_init();
}

#endif //end SUPPORT_NORMAL_OTA



