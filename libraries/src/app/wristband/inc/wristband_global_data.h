
#ifndef __WRISTBAND_GLOBAL_DATA_H__
#define __WRISTBAND_GLOBAL_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"
#include "communicate_protocol.h"
#include "profile_server.h"
#include "profile_client.h"
#include "gap_conn_le.h"
#include "hub_clock.h"
#include "health_algo.h"

#include "hub_display.h"
#include "hub_charger.h"
#include "wristband_gui.h"
#include "rtl8763bf_task.h"
#include "hci_app.h"
#include "communicate_parse_setting.h"
#include "gui_common_menu.h"
#include "rtl876x_wdg.h"


typedef struct
{
    volatile uint16_t quarter_steps;
    volatile uint32_t quarter_distance;
    volatile uint32_t quarter_calories;

    volatile uint32_t global_steps;
    volatile uint32_t global_distance;
    volatile uint32_t global_calorys;

    volatile uint32_t daily_step_target;
    volatile uint16_t daily_sleep_target;
} T_PEDO_DATA;


typedef struct
{
    volatile uint32_t   p_read_addr;
    volatile uint32_t   p_write_addr;
} T_STORAGE_ADDR;


typedef struct
{
    uint8_t sleep_data_month;
    uint8_t sleep_data_day;
    uint8_t sleep_data_hour;

    uint8_t  stamp_counts;

    uint32_t prev_sleep_stamp;

    uint16_t light_sleep_time;
    uint16_t deep_sleep_time;
    uint16_t wake_up_time;

    uint16_t light_sleep_time_18_1;
    uint16_t deep_sleep_time_18_1;
    uint16_t wake_up_time_18_1;

    uint16_t light_sleep_time_1_5;
    uint16_t deep_sleep_time_1_5;
    uint16_t wake_up_time_1_5;

    uint16_t light_sleep_time_5_10;
    uint16_t deep_sleep_time_5_10;
    uint16_t wake_up_time_5_10;

    uint16_t light_sleep_time_wake_to_10;
    uint16_t deep_sleep_time_wake_to_10;
    uint16_t wake_up_time_wake_to_10;

    uint8_t invalid_sleep_data; /*disable data change after this point*/

    uint8_t first_wake_up_after_5;
} T_SLEEEP_DATA;
/* RtkWristBandSys flag bit field */
typedef struct
{
    uint64_t low_power_flag             : 1;
    uint64_t power_off_flag             : 1;
    uint64_t is_paired                  : 1;
    uint64_t device_had_logged          : 1;
    uint64_t algorithm_started          : 1;
    uint64_t daily_target_achieved      : 1;
    uint64_t phone_camera_status        : 1;
    uint64_t lift_switch_status         : 1;
    uint64_t auto_sync_enable           : 1;
    uint64_t pedo_detect_enable         : 1;
    uint64_t hrs_detect_onetime         : 1;
    uint64_t heart_sample_by_phone      : 1;
    uint64_t bp_sample_by_phone         : 1;
    uint64_t bond_state                 : 1;
    uint64_t is_calling_action_alive    : 1;
    uint64_t is_calling_ring_start      : 1;
    uint64_t hour_format                : 1;
    uint64_t distance_unit              : 1;
    uint64_t bps_hidden                 : 1;
    uint64_t bps_detecting              : 1;
    uint64_t bps_timeout                : 1;
    uint64_t twist_switch_status        : 1;
    uint64_t hr_sensor_state            : 1;
    uint64_t alarm_snooze_status        : 1;
    uint64_t sleep_detect_enable        : 1;
    uint64_t system_lock_screen         : 1;
    uint64_t device_lock_screen         : 1;
    uint64_t headset_pair_button        : 1;
    uint64_t headset_pair_handler       : 1;
    uint64_t headset_pair_state         : 1;
    uint64_t restore_factory            : 1;
    uint64_t is_calling_action_accept   : 1;
    uint64_t is_wearing                 : 1;
    uint64_t stopwatch_status           : 1;
    uint64_t countdown_status           : 1;
    uint64_t findphone_status           : 1;
    uint64_t incoming_call_ready_accept : 1;
    uint64_t countdown_popopen          : 1;

    uint64_t reserved                   : 27;
} T_FLAG_FIELD;

/*Do not disturb mode configuration */
typedef union
{
    uint32_t data;
    struct
    {
        uint32_t end_min    : 6;
        uint32_t end_hour   : 5;
        uint32_t start_min  : 6;
        uint32_t start_hour : 5;
        uint32_t status     : 1;
        uint32_t DNDM_start : 1;
        uint32_t reserve    : 8;
    } config;
} T_DND_MODE;

typedef union
{
    uint32_t data;
    struct
    {
        uint32_t                        switch_call_msg :   1;
        uint32_t                        switch_qq_msg   :   1;
        uint32_t                        switch_wechat_msg :   1;
        uint32_t                        switch_shortmessage_msg :   1;
        uint32_t                        switch_line_msg :   1;
        uint32_t                        switch_twitter_msg :   1;
        uint32_t                        switch_facebook_msg :   1;
        uint32_t                        switch_facebookmessage_msg :   1;
        uint32_t                        switch_wahtapp_msg :   1;
        uint32_t                        switch_linkedin_msg :   1;
        uint32_t                        switch_instagram_msg :   1;
        uint32_t                        switch_skype_msg :   1;
        uint32_t                        switch_viber_msg :   1;
        uint32_t                        switch_kakaotalk_msg :   1;
        uint32_t                        switch_vkontakte_msg :   1;
        uint32_t                        switch_mail_msg :   1;
        uint32_t                        switch_calendar_msg :   1;
        uint32_t                        switch_appfacetime_msg :   1;
        uint32_t                        switch_tim_msg :   1;
        uint32_t                        switch_gmail_msg :   1;
        uint32_t                        switch_dingtalk_msg :   1;
        uint32_t                        switch_workwechat_msg :   1;
        uint32_t                        switch_aplus_msg :   1;
        uint32_t                        switch_link_msg :   1;
        uint32_t                        switch_beike_msg :   1;
        uint32_t                        switch_lianjia_msg :   1;
        uint32_t                        msg_reserved       : 6;
    } bit;
} T_MSG_SWITCH;

typedef struct
{
    volatile uint8_t    cur_msg_counts;
    volatile uint8_t    max_msg_counts;
    volatile uint8_t    single_package_len;
    volatile uint8_t    reserved;
    volatile uint32_t   p_read_start_addr;
    volatile uint32_t   p_write_start_addr;
} MSG_Data_Config;

typedef struct
{
    volatile T_FLAG_FIELD           flag_field;
    volatile T_MSG_SWITCH           msg_switch;
    volatile uint8_t                battery_level_value;
    volatile uint8_t                hrs_detect_period;
    volatile uint8_t                hrs_start_up_mode;
    volatile uint8_t                phone_os_version;
    volatile uint8_t                alarm_num;
    volatile uint8_t                oled_display_time;
    volatile uint8_t                language;
    volatile uint8_t                clock_screen_num;
    volatile uint8_t                weather_sync_hour;
    volatile uint8_t                weather_moment_count;
    volatile uint8_t                weather_day_count;
    volatile uint8_t                backlight_percent;
    volatile uint16_t               battery_vol_value;
    volatile uint16_t               current_stationary_time;
    volatile uint32_t               SecondCountRTC;
    volatile uint32_t               pre_rtc_tick_count;
    volatile uint32_t               weather_sync_secondcount;

    volatile T_SLEEP_STATUS         wristband_sleep_status;
    volatile T_CHARGE_STATUS        charger_status;
    volatile T_STORAGE_ADDR         sport_address;
    volatile T_STORAGE_ADDR         sleep_address;
    volatile T_STORAGE_ADDR         heart_address;
    volatile T_STORAGE_ADDR         gps_address;
    volatile T_STORAGE_ADDR         weather_location_address;
    volatile T_STORAGE_ADDR         weather_current_address;
    volatile T_STORAGE_ADDR         weather_future_hour_address;
    volatile T_STORAGE_ADDR         weather_future_day_address;
    volatile T_PEDO_DATA            gPedoData;
    volatile T_DND_MODE             DNDMode;
    volatile T_ALARM                alarms[MAX_ALARM_NUM];
    volatile T_USER_DATA            user_data;
    volatile T_SIT_ALERT            sit_alert_data;
    volatile T_UTC_TIME             Global_Time;
    volatile MSG_Data_Config        msg_data_config;

    volatile T_SLEEEP_DATA          sleep_data_show;
    volatile T_BBPRO_LINK_STATE     bbpro_hci_link_status;
    volatile EVENT_DEVICE_STATE     bbpro_device_status;        //315/320
    volatile T_BBPRO_PAIRED_INFO    paired_info;
    volatile T_GAP_DEV_STATE        gap_dev_state;       /**< GAP device state */
    volatile T_GAP_CONN_STATE       gap_conn_state;      /**< GAP connection state */
    volatile T_CLOCK_MENU_TYPE      clock_status;
    uint8_t                         wristband_conn_id;
    uint16_t                        conn_interval;
    uint16_t                        conn_latency;
    uint16_t                        conn_superv_tout;
    uint16_t                        wristband_mtu;
} __attribute__((packed)) RtkWristBandSysType_t;

typedef struct
{
    uint8_t *p_msg_value;
    uint16  length;
} __attribute__((packed)) MSG_DATA_PAYLOAD;

extern RtkWristBandSysType_t RtkWristbandSys;

/******************************************************************************
 *                              BT status parameters
 ******************************************************************************/

extern T_SERVER_ID              wristband_ser_id;
extern T_SERVER_ID              ias_ser_id;
extern T_SERVER_ID              ota_ser_id;
extern T_SERVER_ID              dfu_ser_id;
extern T_SERVER_ID              bas_ser_id;
#if FEATURE_HID
extern T_SERVER_ID              hid_kb_ser_id;
#endif
#if SUPPORT_SILENT_OTA
extern T_SERVER_ID              g_dfu_service_id;
#endif
#if FEATURE_AIRSYNC
extern T_SERVER_ID              airsync_ser_id;
#endif
extern T_CLIENT_ID   gatts_client_id;

#define MSG_DATA_PACKAGE_LEN 64

void WristBandMSGDataBlockInit(void);
bool WristBandMsgDataStore(uint8_t *buf);
bool WristBandMsgDataRestore(uint8_t *buf, uint16_t load_num);
uint8_t WristBandGetCurMsgCount(void);

void load_wristband_config(void);
void reset_wristband_config(void);
void wristband_config_data_init(void);
uint8_t wristband_get_reboot_reason(void);
#ifdef __cplusplus
}
#endif

#endif //__WRISTBAND_GLOBAL_DATA_H__

