/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     communicate_parse.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "communicate_parse.h"
#include "communicate_parse_setting.h"
#include "wristband_global_data.h"
#include "string.h"
#include "os_timer.h"
#include "app_flash_config.h"
#include "ftl.h"
#include "hub_task.h"
#include "app_task.h"
#include "trace.h"
#include "gap_bond_le.h"
#include "hub_interaction.h"
#include "ancs.h"
#include "ancs_client.h"
#include "communicate_sync_pedo.h"




void resolve_settings_config_command(uint8_t key, const uint8_t *pValue,
                                     uint16_t length)
{
    switch (key)
    {
    case KEY_PHONE_OS_VERSION:
        {
            if (length == 0x02)
            {
                APP_PRINT_INFO2("Phone OS Version:%d; Pair Flag = %d", pValue[0],
                                RtkWristbandSys.paired_info.paired_flag); //0x01:ios,0x02:android

                RtkWristbandSys.phone_os_version = pValue[0];

                if (IOS == RtkWristbandSys.phone_os_version && !RtkWristbandSys.paired_info.paired_flag)
                {
                    le_bond_pair(RtkWristbandSys.wristband_conn_id);
                }
            }
        }
        break;
    case KEY_TIME_SETTINGS:
        {
            if (length == 0x04)
            {
                time_union_t time;

                time.data = 0;
                time.data |= pValue[3];
                time.data |= pValue[2] << 8;
                time.data |= pValue[1] << 16;
                time.data |= pValue[0] << 24;
                //if set time pass a day,reset step count
                uint32_t old_sec = RtkWristbandSys.SecondCountRTC;
                set_wristband_clock(time);
                uint32_t new_sec = RtkWristbandSys.SecondCountRTC;
                system_clock_init(RtkWristbandSys.SecondCountRTC);
                APP_PRINT_INFO3("SET TIME, Date: %d-%d-%d", time.time.year, time.time.month, time.time.day);
                APP_PRINT_INFO3("SET TIME, Time: %d-%d-%d", time.time.hours, time.time.minute, time.time.seconds);
#if FEATURE_USE_FLASH
                ftl_save((void *)&RtkWristbandSys.Global_Time, GLOABAL_TIME_OFFSET, GLOABAL_TIME_SIZE);
#endif
                //check_passed_onetime_alarm();
                L1_send_event(L1SEND_RETURN_CHARGE_STATUS, NULL);

                doNewTimeSettingForPedoData(old_sec, new_sec);
            }
        }
        break;
    case KEY_ALARM_SETTINGS:
        {
            if (length % 5 == 0)
            {
                uint8_t index;
                uint64_t alarmData;
                uint8_t num;
                T_ALARM alarm;

                num = length / 5;

                if (num > MAX_ALARM_NUM)
                {
                    num = MAX_ALARM_NUM;
                }
                RtkWristbandSys.alarm_num = num;
                for (index = 0; index < num; index ++)
                {
                    alarmData = pValue[0 + index * 5];
                    alarm.data = alarmData << 32;
                    alarmData = pValue[1 + index * 5];
                    alarm.data |= alarmData << 24;
                    alarmData = pValue[2 + index * 5];
                    alarm.data |= alarmData << 16;
                    alarmData = pValue[3 + index * 5];
                    alarm.data |= alarmData << 8;
                    alarmData = pValue[4 + index * 5];
                    alarm.data |= alarmData;
                    if (alarm.alarm.day_repeat_flag == 0)
                    {
                        alarm.alarm.reserved = 0x1;
                    }

                    memcpy((void *) & (RtkWristbandSys.alarms[index]), &alarm, sizeof(T_ALARM));
#if 0
                    APP_PRINT_INFO4("alarm day:%d, hour:%d, min:%d,repeat_flag:0x%x\n",
                                    alarm.alarm.day,
                                    alarm.alarm.hour,
                                    alarm.alarm.minute,
                                    alarm.alarm.day_repeat_flag);
#endif
                }
#if FEATURE_USE_FLASH
                ftl_save((void *)RtkWristbandSys.alarms, ALARM_OFFSET, ALARM_SIZE);
                uint32_t temp = RtkWristbandSys.alarm_num;
                ftl_save(&temp, ALARM_NUM_OFFSET, ALARM_NUM_SIZE);
#endif
            }
        }
        break;
    case KEY_REQUEST_ALARM_SETTINGS: //request alarm clock list
        {
            if (length == 0)
            {
                L1_send_event(L1SEND_RETURN_ALARM_EVENT, NULL);
            }
        }
        break;
    case KEY_STEP_TARGET_SETTINGS:
        {
            if (length == 4)
            {
                uint32_t target  = 0;
                target |= pValue[3];
                target |= pValue[2] << 8;
                target |= pValue[1] << 16;
                target |= pValue[0] << 24;

                if (target == 0)
                {
                    target = 10000;
                }
                RtkWristbandSys.gPedoData.daily_step_target = target;
#if FEATURE_USE_FLASH
                uint32_t temp = RtkWristbandSys.gPedoData.daily_step_target;
                ftl_save(&temp, STEP_TARGET_OFFSET, STEP_TARGET_SIZE);
#endif

                if (RtkWristbandSys.gPedoData.daily_step_target >= RtkWristbandSys.gPedoData.global_steps)
                {
                    RtkWristbandSys.flag_field.daily_target_achieved = false;
                }
            }
        }
        break;
    case KEY_SLEEP_TARGET_SETTINGS:
        {
            if (length == 2)
            {
                uint16_t target  = 0;
                target |= pValue[1];
                target |= pValue[0] << 8;

                if (target == 0)
                {
                    target = 480;
                }
                RtkWristbandSys.gPedoData.daily_sleep_target = target;

#if FEATURE_USE_FLASH
                uint32_t temp = RtkWristbandSys.gPedoData.daily_sleep_target;
                ftl_save(&temp, SLEEP_TARGET_OFFSET, SLEEP_TARGET_SIZE);
#endif
            }
        }
        break;
    case KEY_PROFILE_SETTINGS:
        {
            if (length == 0x04)
            {
                userprofile_union_t profile;
                profile.data = 0;
                profile.data |= pValue[3];
                profile.data |= pValue[2] << 8;
                profile.data |= pValue[1] << 16;
                profile.data |= pValue[0] << 24;

                /*only update user profile when setting differrent data*/
                if (memcmp((void *)&RtkWristbandSys.user_data.user_profile.data, (void *)&profile.data,
                           sizeof(userprofile_union_t)) != 0)
                {
                    RtkWristbandSys.user_data.user_profile.data = profile.data;
                    /* update new user profile to algorithm */
                    rtk_gsa_act_switch(GSA_ACT_LIFT, RtkWristbandSys.flag_field.lift_switch_status);
                    rtk_gsa_act_switch(GSA_ACT_TWIST, RtkWristbandSys.flag_field.lift_switch_status);
                }
            }
        }
        break;
    case KEY_LONG_TIME_SIT_ALERT:
        {
            if (length == 8)
            {
                T_SIT_ALERT sit_alert;

                memcpy((uint8_t *) & (sit_alert), pValue, 8);
                APP_PRINT_INFO1("sit_alert on or off:0x%x\n", sit_alert.data);

                if (sit_alert.sit_alert.on_off == 0)
                {
                    //clear stationary time counter
                    RtkWristbandSys.current_stationary_time = 0;
                }
                RtkWristbandSys.sit_alert_data = sit_alert;
            }
        }
        break;
    case KEY_LIFT_SWITCH_SETTING:
        {
            if ((pValue[0] == 0x01) && (length == 0x01))
            {
                RtkWristbandSys.flag_field.lift_switch_status = true;
            }
            else
            {
                RtkWristbandSys.flag_field.lift_switch_status = false;
            }
        }
        break;
    case KEY_LIFT_SWITCH_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_LIFT_SWITCH_EVENT, NULL);
            }
        }
        break;
    case KEY_TWIST_SWITCH_SETTING:
        {
            if ((pValue[0] == 0x01) && (length == 0x01))
            {
                RtkWristbandSys.flag_field.twist_switch_status = true;
            }
            else
            {
                RtkWristbandSys.flag_field.twist_switch_status = false;
            }
        }
        break;
    case KEY_TWIST_SWITCH_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_TWIST_SWITCH_EVENT, NULL);
            }
        }
        break;
    case KEY_INCOMMING_MESSAGE_SETTINGS:
        {
            APP_PRINT_INFO2("KEY_INCOMMING_MESSAGE_SETTINGS length&pValue[0] = %d || 0x%x", length, pValue[0]);
            if (length == 1)
            {
                uint8_t messageType = pValue[0];
                switch (messageType)
                {
                case 0x01:
                    RtkWristbandSys.msg_switch.bit.switch_call_msg =  true;
                    break;
                case 0x02:
                    RtkWristbandSys.msg_switch.bit.switch_call_msg =  false;
                    break;
                case 0x03:
                    RtkWristbandSys.msg_switch.bit.switch_qq_msg =  true;
                    break;
                case 0x04:
                    RtkWristbandSys.msg_switch.bit.switch_qq_msg =  false;
                    break;
                case 0x05:
                    RtkWristbandSys.msg_switch.bit.switch_wechat_msg =  true;
                    break;
                case 0x06:
                    RtkWristbandSys.msg_switch.bit.switch_wechat_msg =  false;
                    break;
                case 0x07:
                    RtkWristbandSys.msg_switch.bit.switch_shortmessage_msg =  true;
                    break;
                case 0x08:
                    RtkWristbandSys.msg_switch.bit.switch_shortmessage_msg =  false;
                    break;
                case 0x09:
                    RtkWristbandSys.msg_switch.bit.switch_line_msg =  true;
                    break;
                case 0x0a:
                    RtkWristbandSys.msg_switch.bit.switch_line_msg =  false;
                    break;
                case 0x0b:
                    RtkWristbandSys.msg_switch.bit.switch_twitter_msg =  true;
                    break;
                case 0x0c:
                    RtkWristbandSys.msg_switch.bit.switch_twitter_msg =  false;
                    break;
                default:
                    break;
                }

                if (RtkWristbandSys.msg_switch.data != 0)
                {
                    if (IOS == RtkWristbandSys.phone_os_version && !RtkWristbandSys.paired_info.paired_flag)
                    {
                        le_bond_pair(RtkWristbandSys.wristband_conn_id);
                    }
                    else if (IOS == RtkWristbandSys.phone_os_version && RtkWristbandSys.paired_info.paired_flag)
                    {
                        ancs_set_data_source_notify(RtkWristbandSys.wristband_conn_id, true);
                    }
                }
                else
                {
                    if (IOS == RtkWristbandSys.phone_os_version && RtkWristbandSys.paired_info.paired_flag)
                    {
                        ancs_set_data_source_notify(RtkWristbandSys.wristband_conn_id, false);
                    }
                }
                uint32_t temp = RtkWristbandSys.msg_switch.data;
                ftl_save(&temp, MSG_SWITCH_OFFSET, MSG_SWITCH_SIZE);
            }
        }
        break;
    case KEY_INCOMMING_MESSAGE_ALL_SETTINGS:
        {
            if (length == 4)
            {
                RtkWristbandSys.msg_switch.data  = pValue[3];
                RtkWristbandSys.msg_switch.data |= pValue[2] << 8;
                RtkWristbandSys.msg_switch.data |= pValue[1] << 16;
                RtkWristbandSys.msg_switch.data |= pValue[0] << 24;

                if (RtkWristbandSys.msg_switch.data != 0)
                {
                    if (IOS == RtkWristbandSys.phone_os_version && !RtkWristbandSys.paired_info.paired_flag)
                    {
                        le_bond_pair(RtkWristbandSys.wristband_conn_id);
                    }
                    else if (IOS == RtkWristbandSys.phone_os_version && RtkWristbandSys.paired_info.paired_flag)
                    {
                        ancs_set_data_source_notify(RtkWristbandSys.wristband_conn_id, true);
                    }
                }
                else
                {
                    if (IOS == RtkWristbandSys.phone_os_version && RtkWristbandSys.paired_info.paired_flag)
                    {
                        ancs_set_data_source_notify(RtkWristbandSys.wristband_conn_id, false);
                    }
                }
            }
        }
        break;
    case KEY_LONG_TIME_SIT_SETTING_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_SIT_SETTING_RETURN, NULL);
            }
        }
        break;
    case KEY_INCOMMING_MESSAGE_SETTINGS_REQUEST:
        {

            if (length == 0x00)
            {
                if ((!RtkWristbandSys.paired_info.paired_flag) && (RtkWristbandSys.phone_os_version == IOS) &&
                    (RtkWristbandSys.msg_switch.data != 0))
                {
                    le_bond_pair(RtkWristbandSys.wristband_conn_id);
                }
                L1_send_event(L1SEND_RETURN_INCOMMING_MESSAGE_SETTINGS, NULL);
            }
        }
        break;
    case KEY_FUNCTIONS_REQUEST:
        {
            APP_PRINT_INFO0("<><><><><><><><><><> KEY_FUNCTIONS_REQUEST <><><><><><><><><><>");
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_FUNCTIONS_EVENT, NULL);
                L1_send_event(L1SEND_RETURN_BBPRO_CONN_INFO, NULL);
                L1_send_event(L1SEND_RETURN_HIDDEN_FUNC, NULL);
            }
        }
        break;
    case KEY_EXERCISEMODE_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_EXERCISEMODE_EVENT, NULL);
            }
        }
        break;
    case KEY_DIAL_SETTING:
        {
            if (length == 0x01)
            {
                if (pValue[0] < 6)
                {
                    if (RtkWristbandSys.clock_status != pValue[0])
                    {
                        RtkWristbandSys.clock_status = (T_CLOCK_MENU_TYPE)pValue[0];
                        //uint32_t temp = RtkWristbandSys.clock_status;
                    }
                }
            }
        }
        break;
    case KEY_DIAL_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_DAIL_CHANGE, NULL);
            }
        }
        break;
    case KEY_HR_SAMPLE_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_HR_SAMPLE_EVENT, NULL);
            }
        }
        break;
    case KEY_HOUR_FORMAT_SETTING:
        {
            if (length == 0x01)
            {
                RtkWristbandSys.flag_field.hour_format = pValue[0];
            }
        }
        break;
    case KEY_HOUR_FORMAT_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_HOUR_FORMAT_SETTING, NULL);
            }
        }
        break;
    case KEY_DISTANCE_UNIT_SETTING:
        {
            if (length == 0x01)
            {
                RtkWristbandSys.flag_field.distance_unit = pValue[0];
            }
        }
        break;
    case KEY_DISTANCE_UNIT_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_DISTANCE_UNIT_SETTING, NULL);
            }
        }
        break;
    case KEY_DNDM_SETTING:
        {
            if (length == 0x03)
            {
                bool temp_DNDM_start = RtkWristbandSys.DNDMode.config.DNDM_start;
                memset((void *)&RtkWristbandSys.DNDMode, 0x00, sizeof(T_DND_MODE));
                RtkWristbandSys.DNDMode.data |= pValue[0] << 16;
                RtkWristbandSys.DNDMode.data |= pValue[1] << 8;
                RtkWristbandSys.DNDMode.data |= pValue[2];
                RtkWristbandSys.DNDMode.config.DNDM_start = temp_DNDM_start;
            }
        }
        break;
    case KEY_DNDM_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_DNDM_SETTING, NULL);
            }
        }
        break;
    case KEY_OLED_DISPLAY_TIME_SETTING:
        {
            if (length == 0x01)
            {
                if (pValue[0] > 2 && pValue[0] <= 30)
                {
                    RtkWristbandSys.oled_display_time = pValue[0];
                }
            }
        }
        break;
    case KEY_OLED_DISPLAY_TIME_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_OLED_DISPLAY_TIME, NULL);
            }
        }
        break;
    case KEY_LANGUAGE_SETTING:
        {
            if (length == 0x01)
            {
                switch (pValue[0])
                {
                case 1:
                    RtkWristbandSys.language = language_SChinese;
                    break;
                case 18:
                    RtkWristbandSys.language = language_German;
                    break;
                case 20:
                    RtkWristbandSys.language = language_French;
                    break;
                case 65:
                    RtkWristbandSys.language = language_Japanese;
                    break;
                case 87:
                    RtkWristbandSys.language = language_Spanish;
                    break;
                case 96:
                    RtkWristbandSys.language = language_Italian;
                    break;
                default:
                    RtkWristbandSys.language = language_English;
                    break;
                }
                uint32_t temp = RtkWristbandSys.language;
            }
        }
        break;
    case KEY_LANGUAGE_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_LANGUAGE, NULL);
            }
        }
        break;
    case KEY_DEVICEINFO_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_DEVICE_INFO, NULL);
            }
        }
        break;
    case KEY_BACKLIGHT_SETTING:
        {
            if (length == 0x01)
            {
                if (pValue[0] > 19 && pValue[0] < 101)
                {
                    RtkWristbandSys.backlight_percent = pValue[0];
                }
            }
        }
        break;
    case KEY_BACKLIGHT_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_BACKLIGHT_EVENT, NULL);
            }
        }
        break;
    case KEY_HIDDEN_FUNC_SETTING:
        {
            if (length == 0x04)
            {
                Hidden_FunC_t  hidden_func;
                hidden_func.data = 0;
                hidden_func.data |= pValue[3];
                hidden_func.data |= pValue[2] << 8;
                hidden_func.data |= pValue[1] << 16;
                hidden_func.data |= pValue[0] << 24;

                RtkWristbandSys.flag_field.stopwatch_status = hidden_func.status.stopwatch_sw;
                RtkWristbandSys.flag_field.findphone_status = hidden_func.status.findPhone_sw;
                RtkWristbandSys.flag_field.system_lock_screen = hidden_func.status.lockScreen_sw;
            }
        }
        break;
    case KEY_HIDDEN_FUNC_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_HIDDEN_FUNC, NULL);
            }
        }
        break;
    case KEY_BBPRO_MAC_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_BBPRO_MAC_ADDR, NULL);
                if (RtkWristbandSys.bbpro_device_status == APP_STATE_PAIRING)
                {
                    break;
                }
                extern bool bbpro_bd_addr_got_flag;
                if (bbpro_bd_addr_got_flag == true)
                {
                    T_HCI_MSG msg;
                    msg.type = HCI_MSG_TYPE_8763BF;
                    msg.subtype = BBPRO_HCI_SEND_POWER_ON_PRESSED;
                    bbpro_hci_msg_queue_send(&msg);
                    msg.subtype = BBPRO_HCI_SEND_POWER_ON_RELEASED;
                    bbpro_hci_msg_queue_send(&msg);
                    msg.subtype = BBPRO_HCI_SEND_ENTER_PAIRING_MODE;
                    bbpro_hci_msg_queue_send(&msg);
                    RtkWristbandSys.flag_field.headset_pair_state = false;
                    RtkWristbandSys.flag_field.headset_pair_button = true;
                    RtkWristbandSys.flag_field.headset_pair_handler = true;
                }
            }
        }
        break;
    case KEY_BBPRO_STATE_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_BBPRO_STATE, NULL);
            }
        }
        break;
    case KEY_BBPRO_CONNECTED_STATE_REQUEST:
        {
            if (length == 0x00)
            {
                L1_send_event(L1SEND_RETURN_BBPRO_CONNECTED_STATE, NULL);
            }
        }
        break;
    case KEY_BBPRO_CREATE_CONNECTION_REQUEST:
        {
            if (length == 0x00)
            {
                /*iOS Initiate pairing, wirstband display dynamic picture*/
                RtkWristbandSys.flag_field.headset_pair_button = true;
                RtkWristbandSys.flag_field.headset_pair_handler = true;
                RtkWristbandSys.flag_field.headset_pair_state = false;
            }
        }
        break;
    default:
        break;
    }
}

