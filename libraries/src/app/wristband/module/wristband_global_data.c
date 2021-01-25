#include "wristband_global_data.h"
#include "app_flash_config.h"
#include "platform_sdk_cb.h"
#include "ftl.h"
#include "flash_device.h"
#include "flash_adv_cfg.h"
#include "sync_data.h"




/******************************************************************************
 *                              User setting and flag parameters
 ******************************************************************************/

RtkWristBandSysType_t RtkWristbandSys __attribute__((aligned(4)));

void wristband_config_data_init(void)
{
    user_wdg_cb = (BOOL_WDG_CB)wristband_wdg_reboot_callback;

    memset((uint8_t *)&RtkWristbandSys, 0x00, sizeof(RtkWristbandSys));

    uint8_t reason = wristband_get_reboot_reason();
    APP_PRINT_INFO1("[WRISTBAND CONFIG INIT] reason = %d!", reason);
    load_wristband_config();
}

void load_wristband_config(void)
{
    uint32_t temp;
    if (ftl_load((void *)RtkWristbandSys.alarms, ALARM_OFFSET, ALARM_SIZE))
    {
        memset((void *)RtkWristbandSys.alarms, 0, ALARM_SIZE);
        ftl_save((void *)RtkWristbandSys.alarms, ALARM_OFFSET, ALARM_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] ALARM_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] ALARM_SIZE FTL NO INIT!");
    }

    if (ftl_load(&temp, ALARM_NUM_OFFSET, ALARM_NUM_SIZE))
    {
        temp = 0;
        ftl_save(&temp, ALARM_NUM_OFFSET, ALARM_NUM_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] ALARM_NUM_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] ALARM_NUM_SIZE FTL NO INIT!");
    }
    RtkWristbandSys.alarm_num = temp;

    if (ftl_load((void *)&RtkWristbandSys.sit_alert_data, SIT_ALERT_OFFSET, SIT_ALERT_SIZE))
    {
        RtkWristbandSys.sit_alert_data.data = 0;
        ftl_save((void *)&RtkWristbandSys.sit_alert_data, SIT_ALERT_OFFSET, SIT_ALERT_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] SIT_ALERT_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] SIT_ALERT_SIZE FTL NO INIT!");
    }

    if (ftl_load((void *)&RtkWristbandSys.user_data.user_profile, USER_PROFILE_OFFSET,
                 USER_PROFILE_SIZE))
    {
        RtkWristbandSys.user_data.user_profile.data = 0;
        ftl_save((void *)&RtkWristbandSys.user_data.user_profile, USER_PROFILE_OFFSET, USER_PROFILE_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] USER_PROFILE_OFFSET FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] USER_PROFILE_OFFSET FTL NO INIT!");
    }

    if (ftl_load(&temp, MSG_SWITCH_OFFSET, MSG_SWITCH_SIZE))
    {
        temp = 0;
        ftl_save(&temp, MSG_SWITCH_OFFSET, MSG_SWITCH_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] MSG_SWITCH_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] MSG_SWITCH_SIZE FTL NO INIT!");
    }
    RtkWristbandSys.msg_switch.data = temp;

    if (ftl_load(&temp, LIFT_SWITCH_OFFSET, LIFT_SWITCH_SIZE))
    {
        temp = 0;
        ftl_save(&temp, LIFT_SWITCH_OFFSET, LIFT_SWITCH_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] LIFT_SWITCH_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] LIFT_SWITCH_SIZE FTL NO INIT!");
    }
    RtkWristbandSys.flag_field.lift_switch_status = temp;

    if (ftl_load(&temp, TWIST_SWITCH_OFFSET, TWIST_SWITCH_SIZE))
    {
        temp = 0;
        ftl_save(&temp, TWIST_SWITCH_OFFSET, TWIST_SWITCH_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] TWIST_SWITCH_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] TWIST_SWITCH_SIZE FTL NO INIT!");
    }
    RtkWristbandSys.flag_field.twist_switch_status = temp;

    if (ftl_load(&temp, HRS_PERIOD_OFFSET, HRS_PERIOD_SIZE))
    {
        temp = 0;
        ftl_save(&temp, HRS_PERIOD_OFFSET, HRS_PERIOD_SIZE);
        APP_PRINT_INFO0("[LOAD CONFIG] HRS_PERIOD_SIZE FTL INIT!");
    }
    else
    {
        APP_PRINT_INFO0("[LOAD CONFIG] HRS_PERIOD_SIZE FTL NO INIT!");
    }
    RtkWristbandSys.hrs_detect_period = temp;

}

void reset_wristband_config(void)
{
    uint32_t temp = 0;

    memset((void *)RtkWristbandSys.alarms, 0, ALARM_SIZE);
    ftl_save((void *)RtkWristbandSys.alarms, ALARM_OFFSET, ALARM_SIZE);

    ftl_save(&temp, ALARM_NUM_OFFSET, ALARM_NUM_SIZE);
    RtkWristbandSys.alarm_num = temp;

    RtkWristbandSys.sit_alert_data.data = 0;
    ftl_save((void *)&RtkWristbandSys.sit_alert_data, SIT_ALERT_OFFSET, SIT_ALERT_SIZE);

    RtkWristbandSys.user_data.user_profile.data = 0;
    ftl_save((void *)&RtkWristbandSys.user_data.user_profile, USER_PROFILE_OFFSET, USER_PROFILE_SIZE);

    ftl_save(&temp, MSG_SWITCH_OFFSET, MSG_SWITCH_SIZE);
    RtkWristbandSys.msg_switch.data = temp;

    ftl_save(&temp, LIFT_SWITCH_OFFSET, LIFT_SWITCH_SIZE);
    RtkWristbandSys.flag_field.lift_switch_status = temp;

    ftl_save(&temp, TWIST_SWITCH_OFFSET, TWIST_SWITCH_SIZE);
    RtkWristbandSys.flag_field.twist_switch_status = temp;

    ftl_save(&temp, HRS_PERIOD_OFFSET, HRS_PERIOD_SIZE);
    RtkWristbandSys.hrs_detect_period = temp;
}

void wristband_hw_reboot_handle(void)
{
    /* reset config data when HW reboot reason*/
    uint8_t prev_bp_lv = 0;
    flash_sw_protect_unlock_by_addr_locked(PEDO_DATA_START_ADDRESS, &prev_bp_lv);
    flash_erase_locked(FLASH_ERASE_SECTOR, WRISTBAND_CONFIG_START_ADDR);
    flash_set_block_protect_locked(prev_bp_lv);
    /* reset health data when hardware reset */
    WristBandPedoDataBlockInit();
    WristBandSleepDataBlockInit();
    WristBandHeartDataBlockInit();
    WristBandMSGDataBlockInit();
    APP_PRINT_INFO0("[WRISTBAND RESET => HW]!");
}

void wristband_sw_reboot_handle(void)
{
    //todo
    uint32_t len = sizeof(RtkWristBandSysType_t);
    memcpy((uint8_t *)&RtkWristbandSys, (uint8_t *)(WRISTBAND_CONFIG_START_ADDR | 0x1000000), len);
    RtkWristbandSys.pre_rtc_tick_count = 0;
    APP_PRINT_INFO0("[WRISTBAND RESET => SW]!");
}

uint8_t wristband_get_reboot_reason(void)
{
    uint32_t reason = 0;
    if (ftl_load(&reason, REBOOT_REASON_OFFSET, REBOOT_REASON_SIZE) == 0)
    {
        if (reason == 0xF0 || reason == 0xAB)
        {
            APP_PRINT_INFO1("soft reboot reason, reason value = %d", reason);
            wristband_sw_reboot_handle();
        }
        else if (reason == 0x00)
        {
            APP_PRINT_INFO1("HW reboot reason, reason value = %d", reason);
            wristband_hw_reboot_handle();
        }
        else if (reason == 0xF1)
        {
            APP_PRINT_INFO1("UNBOND reboot reason, reason value = %d", reason);
            wristband_hw_reboot_handle();
        }
        else
        {
            APP_PRINT_INFO0("Unknow reboot reason");
            wristband_hw_reboot_handle();
        }
        reason = 0;
        ftl_save(&reason, REBOOT_REASON_OFFSET, REBOOT_REASON_SIZE);
    }
    else
    {
        APP_PRINT_INFO0("wristband load reboot Fail");
        wristband_hw_reboot_handle();
        reason = 0;
        ftl_save(&reason, REBOOT_REASON_OFFSET, REBOOT_REASON_SIZE);
    }
    return reason;
}

