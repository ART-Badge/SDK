enum { __FILE_NUM__ = 0 };

/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     main.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/
#include <stdlib.h>
#include "board.h"
#include "os_sched.h"
#include "string.h"
#include "trace.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "app_task.h"
#include "communicate_task.h"
#include "dlps.h"
#include "ftl.h"
#include "ias.h"
#include "bas.h"
#include "hids_media.h"
#include "ancs.h"
#include "ams.h"
#include <gatts_client.h>
#include "wristband_gap.h"
#include "rtl876x_io_dlps.h"
#include "rtl876x_rcc.h"
#include "rtl876x_i2c.h"
#include "otp_config.h"
#include "rtl876x_lib_platform.h"
#include "single_tone.h"

#include "wristband_private_service.h"
#include "wristband_application.h"
#include "profile_server.h"
#include "hub_clock.h"
#include "health_algo.h"
#include "sync_data.h"
#include "app_flash_config.h"
#include "profile_client.h"
#include "wristband_global_data.h"
#include "ota_service.h"
#include "hub_task.h"
#include "module_algorithm_align.h"
#include "hci_app.h"
#if APP_PRIVACY_EN
#include <privacy_mgnt.h>
#endif
#if SUPPORT_SILENT_OTA
#include "dfu_service.h"
#endif

#if FEATURE_AIRSYNC
#include "airsync_ble_service.h"
#endif
#if FEATURE_BREEZE
#include "ais.h"
#endif
#if FEATURE_8763BF
#include "hci_app.h"
#endif
#include "test_mode.h"
#include "mp_test_task.h"


// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL_MIN              0x640 //1000ms
#define DEFAULT_ADVERTISING_INTERVAL_MAX              0x640 //1000ms
//#define DEFAULT_ADVERTISING_INTERVAL_MIN                0x4B0 //750ms
//#define DEFAULT_ADVERTISING_INTERVAL_MAX                0x4B0 //750ms

#define GATT_UUID128_BWPS_ADV   0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0x01, 0x00, 0x00

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8_t scan_rsp_data[] =
{
    /* Service */
    17,             /* length     */
    GAP_ADTYPE_128BIT_MORE,            /* type="Complete 128-bit UUIDs available" */
    GATT_UUID128_BWPS_ADV,

    /* place holder for Local Name, filled by BT stack. if not present */
    /* BT stack appends Local Name.                                    */
    0x03,           /* length     */
    0x19,           /* type="Appearance" */
    0x42, 0x0c,     /* wrist worn */
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
// **note**: adverdata array will used to store device name which can be re-configured, so except to
// the first 5 Bytes, remaining 26 Byte space will used for device name configure.
uint8_t adv_data[ADVERTISING_PACKET_LENGTH] =
{
    /* Core spec. Vol. 3, Part C, Chapter 18 */
    /* Flags */
    /* place holder for Local Name, filled by BT stack. if not present */
    /* BT stack appends Local Name.                                    */
    0x02,            /* length     */
    GAP_ADTYPE_FLAGS,
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
    0x07,           /* length     */
    0x09,           /* type="Complete local name" */
    'V', '0', '8', 'P', 'r', 'o', /* SmartBracelet */
    /* Service */
    0x03,           /* length     */
    0x03,           /* type="More 16-bit UUIDs available, service uuid 0xFEE7 0xA00A" */
    0xE7,
    0xFE,
    /* Manufacture specified data*/
    0x09,           /* length     */
    0xFF,           /* type: manufacture specific data*/
    0xC5, 0xFE,     /* company id */
    0x20, 0x15, 0x09, 0x14, 0x14, 0x21, /* mac address*/
};

/**
  * @brief  Initialize peripheral and gap bond manager related parameters
  * @return void
  */
void app_le_gap_init(void)
{
    /* Device name and device appearance */
    uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "V08Pro";
    uint16_t appearance = GAP_GATT_APPEARANCE_WRIST_WORN;
    uint8_t  slave_init_mtu_req = true;


    /* Advertising parameters */
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
    uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MAX;

    /* GAP Bond Manager parameters */
    uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
    uint8_t  auth_io_cap = GAP_IO_CAP_KEYBOARD_DISPLAY;
    uint8_t  auth_oob = false;
    uint8_t  auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;

    uint8_t  auth_sec_req_enable = false;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

    uint8_t  ftl_name[DEVICE_NAME_SIZE];
    uint8_t  advdata_len = 29;
    if (0 == ftl_load(ftl_name, DEVICE_NAME_OFFSET, DEVICE_NAME_SIZE))
    {
        if (ftl_name[0] != 0xff && ftl_name[0] != 0)
        {
            //can not get bd address here, should set address when stack ready
            advdata_len = wristband_adv_data_init(ftl_name);
            memset(device_name, 0, GAP_DEVICE_NAME_LEN);
            memcpy(device_name, ftl_name + 2, ftl_name[0] - 1);
        }
    }

    /* Set device name and device appearance */
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                     &slave_init_mtu_req);

    /* Set advertising parameters */
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
    le_adv_set_param(GAP_PARAM_ADV_DATA, advdata_len, (void *)adv_data);
    le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

    /* Setup the GAP Bond Manager */
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

    /* register gap message callback */
    le_register_app_cb(app_gap_callback);
#if APP_PRIVACY_EN
    privacy_init(app_privacy_callback, false);
#endif
#if 0 //for use random addr
    extern uint32_t random_seed_value;
    srand(random_seed_value);
    uint8_t wristband_random_addr[6];
    uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_RANDOM;
    if (le_gen_rand_addr(GAP_RAND_ADDR_STATIC, wristband_random_addr) == GAP_CAUSE_SUCCESS)
    {
        le_cfg_local_identity_address(wristband_random_addr, GAP_IDENT_ADDR_RAND);
        le_set_gap_param(GAP_PARAM_RANDOM_ADDR, 6, wristband_random_addr);
        //only for peripheral,broadcaster
        le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
    }
#endif
}

/* wrisbtand application related services' ID. */
T_SERVER_ID  wristband_ser_id;
T_SERVER_ID  ias_ser_id;
T_SERVER_ID  ota_ser_id;
T_SERVER_ID  dfu_ser_id;
T_SERVER_ID  bas_ser_id;
T_SERVER_ID  nus_ser_id;
T_CLIENT_ID  gatts_client_id;        /**< gatt service client id*/

#if FEATURE_HID
T_SERVER_ID  hid_kb_ser_id;
#endif

#if FEATURE_VOICE
T_SERVER_ID  voice_ser_id;
#endif

#if FEATURE_AIRSYNC
T_SERVER_ID  airsync_ser_id;//mean wechat air sync service
#endif
/**
 * @brief  Add GATT services and register callbacks
 * @return void
 */
void app_le_profile_init(void)
{
    uint8_t service_count = 0;
    service_count = SERVICE_NUM;

#if SUPPORT_SILENT_OTA
    service_count++;
#endif

#if FEATURE_AIRSYNC
    service_count++;
#endif

#if FEATURE_HID
    service_count++;
#endif
#if FEATURE_BREEZE
    service_count++;
#endif
    server_init(service_count);
    wristband_ser_id = bwps_service_add_service((void *)app_profile_callback);
    ias_ser_id = ias_add_service((void *)app_profile_callback);
    ota_ser_id = ota_add_service((void *)app_profile_callback);
    bas_ser_id = bas_add_service((void *)app_profile_callback);
#if SUPPORT_SILENT_OTA
    dfu_ser_id = dfu_add_service((void *)app_profile_callback);
#endif
#if FEATURE_HID
    hid_kb_ser_id = hids_add_service((void *)app_profile_callback);
#endif
#if FEATURE_BREEZE
    ais_add_service((void *)app_profile_callback);
#endif
#if FEATURE_AIRSYNC
    airsync_ser_id = airsync_add_service(app_profile_callback);
#endif
    server_register_app_cb(app_profile_callback);

    client_init(3);
    ancs_init(1);
    ams_init(1);
    gatts_client_id  = gatts_add_client(app_client_callback, 1);
}
/**
 * @brief    Wake up by peripheral in dlps mode
 * @return   void
 */
void System_Handler(void)
{
    NVIC_DisableIRQ(System_IRQn);
    APP_PRINT_INFO0("system handler interrupt");
    /*When the parameter of System_WakeUpPinEnable is PAD_WK_DEBOUNCE_ENABLE, System_WakeUpInterruptValue can't work.*/
    for (uint8_t i = 0; i < wristband_sensor_hub_count; i++)
    {
        if (driver_modules[i]->system_wakeup_check_cb != NULL)
        {
            if (driver_modules[i]->system_wakeup_check_cb() == true)
            {
                NVIC_ClearPendingIRQ(System_IRQn);
                return;
            }
        }
    }
    APP_PRINT_INFO0("Unknown WakeUp Int");
    NVIC_ClearPendingIRQ(System_IRQn);
}

/**
 * @brief    Contains the power mode settings
 * @return   void
 */
void pwr_mgr_init(void)
{
#if CONFIG_DLPS_EN

    if (false == dlps_check_cb_reg(wristband_enter_dlps_check))
    {
        APP_PRINT_INFO0("Wristband_EnterDLPSCheck reg fail!");
    }
    DLPS_IORegUserDlpsEnterCb(wristband_enter_dlps);
    DLPS_IORegUserDlpsExitCb(wristband_exit_dlps);
    DLPS_IORegister();

    lps_mode_set(LPM_DLPS_MODE);

#endif
}


/**
 * @brief    Contains the initialization of all tasks
 * @note     There is only one task in BLE Peripheral APP, thus only one APP task is init here
 * @return   void
 */
void task_init(void)
{
    app_task_init();
    communicate_task_init();
    ui_task_init();
}

/**
* @brief  print all images version to check ota whether success.
* @return  void
*/
void print_all_images_version(void)
{
    T_IMG_ID image_id;
    T_IMAGE_VERSION image_version;

    image_id = OTA;
    if (get_active_bank_image_version(image_id, &image_version))
    {
        DBG_DIRECT("image:0x%x,version =0x%x, sub_version:%d.%d.%d.%d", image_id,
                   image_version.ver_info.version,
                   image_version.ver_info.header_sub_version._version_major,
                   image_version.ver_info.header_sub_version._version_minor,
                   image_version.ver_info.header_sub_version._version_revision,
                   image_version.ver_info.header_sub_version._version_reserve);
    }
    else
    {
        DBG_DIRECT("image:0x%x get_active_bank_image_version fail!!!", image_id);
    }

    for (image_id = SecureBoot; image_id < IMAGE_MAX; image_id++)
    {
        if (get_active_bank_image_version(image_id, &image_version))
        {
            DBG_DIRECT("image:0x%x,version =0x%x, sub_version:%d.%d.%d.%d", image_id,
                       image_version.ver_info.version,
                       image_version.ver_info.img_sub_version._version_major,
                       image_version.ver_info.img_sub_version._version_minor,
                       image_version.ver_info.img_sub_version._version_revision,
                       image_version.ver_info.img_sub_version._version_reserve);
        }
        else
        {
            DBG_DIRECT("image:0x%x get_active_bank_image_version fail!!!", image_id);
        }
    }

    //Read back user data version
    T_IMG_CTRL_HEADER_FORMAT user_ctrl_header;
    flash_read_locked(flash_get_bank_addr(FLASH_BKP_DATA1), sizeof(T_IMG_CTRL_HEADER_FORMAT),
                      (uint8_t *)&user_ctrl_header);
    if (user_ctrl_header.image_id == IMAGE_USER_DATA &&
        !user_ctrl_header.ctrl_flag.flag_value.not_ready)
    {
        T_IMAGE_VERSION usr_data_version;
        flash_read_locked(flash_get_bank_addr(FLASH_BKP_DATA1) + 0x60, sizeof(T_IMAGE_VERSION),
                          (uint8_t *)&usr_data_version);

        DBG_DIRECT("image:0x%x,version =0x%x, sub_version:%d.%d.%d.%d", user_ctrl_header.image_id,
                   usr_data_version.ver_info.version,
                   usr_data_version.ver_info.img_sub_version._version_major,
                   usr_data_version.ver_info.img_sub_version._version_minor,
                   usr_data_version.ver_info.img_sub_version._version_revision,
                   usr_data_version.ver_info.img_sub_version._version_reserve);
    }
    else
    {
        DBG_DIRECT("user data is not exist! image:0x%x, not_ready=%d", user_ctrl_header.image_id,
                   user_ctrl_header.ctrl_flag.flag_value.not_ready);
    }

}

/**
 * @brief    Entry of APP code
 * @return   int (To avoid compile warning)
 */
int main(void)
{
    extern uint32_t random_seed_value;
    srand(random_seed_value);
    if (set_system_clock(SYSTEM_90MHZ) == 0)
    {
        DBG_DIRECT("Set system clock 90M Fail!");
    }
    if (flash_try_high_speed(FLASH_MODE_4BIT) == 0)
    {
        DBG_DIRECT("Flash change to 4 BIT Mode Fail!");
    }
    //call switch_to_test_mode(WRISTBAND_TEST_MODE);
    uint8_t test_mode = get_test_mode();
    //uint8_t test_mode = WRISTBAND_TEST_MODE;
    reset_test_mode();

    switch (test_mode)
    {
    case NOT_TEST_MODE:
        {
            le_gap_init(1);
            gap_lib_init();
            app_le_gap_init();
            app_le_profile_init();
            pwr_mgr_init();
            task_init();
            os_sched_start();
        }
        break;
    case WRISTBAND_TEST_MODE:
        {
            mp_test_task_init();
            os_sched_start();
        }
        break;
    case SINGLE_TONE_MODE:
        {
            WDG_Disable();  /* Avoid unexpected reboot */
            single_tone_init();
            os_sched_start();
        }
        break;
    default:
        break;
    }

    return 0;
}


