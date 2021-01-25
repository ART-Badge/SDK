enum { __FILE_NUM__ = 0 };
/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     health_algo.c
* @brief
* @details
* @author
* @date
* @version  v0.2
*********************************************************************************************************
*/
#include "string.h"
#include "board.h"
#include "trace.h"

#include "hub_clock.h"
#include "hub_gsensor.h"
#include "app_flash_config.h"
#include "rtl876x_gpio.h"
#include "wristband_global_data.h"
#include "os_msg.h"
#include "sync_data.h"
#include "hub_interaction.h"
#include "app_msg.h"
#include "hub_task.h"

//new algo headers
#include "health_algo.h"


// Private Functions Declaration
static void pedo_algo_cb(gsa_pedo_info_t *info);
static void sleep_algo_cb(gsa_sleep_stt_e stt, uint32 offset);
static void act_algo_cb(gsa_act_type_e type, uint16 para);
static void act_lift_hdl(uint16 para);
static void act_twist_hdl(uint16 para);
static void act_wave_hdl(uint16 para);


// Variables
static gsa_cbs_t gsa_cbs[] =
{
    pedo_algo_cb,
    sleep_algo_cb,
    act_algo_cb
};
static void (*act_cbs[])(uint16) =
{
    act_lift_hdl,
    act_twist_hdl,
    act_wave_hdl
};

static void sleep_algo_cb(gsa_sleep_stt_e stt, uint32 offset)
{
    SleepData_U mSleepData;
    SleepHead_t mSleepHead;
    algorithm_event_sleep_t event = {0};
    uint8_t buffer[8] = {0};
    uint32_t time_stamp = RtkWristbandSys.SecondCountRTC - 60 * offset;

    /*Monitoring period : 18:00 ~ 10:00 +1*/
    if (RtkWristbandSys.flag_field.sleep_detect_enable == false)
    {
        return;
    }

    APP_PRINT_INFO1("************sleep status:%d**************", stt);
    switch (stt)
    {
    case GSA_SLEEP_OFF:                // band not wear TODO:add state corresponding actions
    case GSA_SLEEP_WAKE:                 //  Wake->Sleep
        {
            if (RtkWristbandSys.wristband_sleep_status == TGET_UP)
            {
                return; // avoid consistent wake state
            }

            event.mode = TGET_UP;
            event.starting_time_stamp = time_stamp;
            RtkWristbandSys.wristband_sleep_status = TGET_UP;
        }
        break;
    case GSA_SLEEP_SLEEP:                // 1. Sleep -> Wake  2. Sleep->Deep Sleep
        {
            event.mode = TSLEEP;
            event.starting_time_stamp = time_stamp;
            RtkWristbandSys.wristband_sleep_status = TSLEEP;
        }
        break;
    case GSA_SLEEP_DEEP:                //  1. Deep Sleep->Sleep  2. Deep Sleep -> Wake
        {
            event.mode = TDEEP_SLEEP;
            event.starting_time_stamp = time_stamp;
            RtkWristbandSys.wristband_sleep_status = TDEEP_SLEEP;
        }
        break;
    default:
        APP_PRINT_INFO1("invalid sleep status:%d\n", stt);
        break;
    }
    convert_to_utc_time(time_stamp);
    mSleepHead.Date.date.day = RtkWristbandSys.Global_Time.day;
    mSleepHead.Date.date.month = RtkWristbandSys.Global_Time.month;
    mSleepHead.Date.date.year = RtkWristbandSys.Global_Time.year - 2000;
    convert_to_utc_time(RtkWristbandSys.SecondCountRTC);
    /* fix sleep item length = 1 */
    mSleepHead.length = 1;
    mSleepData.bits.timeStamp = (uint16_t)((event.starting_time_stamp / 60) % 1440);
    mSleepData.bits.mode = event.mode;

    buffer[0] = mSleepHead.Date.data >> 8;
    buffer[1] = mSleepHead.Date.data;
    buffer[2] = mSleepHead.length >> 8;
    buffer[3] = mSleepHead.length;
    buffer[4] = mSleepData.data >> 24;
    buffer[5] = mSleepData.data >> 16;
    buffer[6] = mSleepData.data >> 8;
    buffer[7] = mSleepData.data;

    WristBandSleepDataStore(buffer, SLEEP_ITEM_LENGTH + SLEEP_HEAD_LENGTH);

    APP_PRINT_INFO2(" timeStamp:%d,mode:%d\n\n", mSleepData.bits.timeStamp, mSleepData.bits.mode);
    APP_PRINT_INFO2(" mSleepHead.length:%d,  sleep data:%8x\n\n", mSleepHead.length, mSleepData.data);
}


void pedo_algo_cb(gsa_pedo_info_t *info)
{
    uint8_t offset = ((RtkWristbandSys.SecondCountRTC / 60) % 1440) / 15;
    if (info != NULL)
    {
        APP_PRINT_INFO4("@cb Steps=%d Distance =%d, Calory=%d, offset = %d",
                        info->steps,
                        info->distance,
                        info->calories,
                        offset);

        if (info->steps > 0)
        {
            RtkWristbandSys.gPedoData.global_steps += info->steps;
            RtkWristbandSys.gPedoData.quarter_steps += info->steps;
        }
        if (info->distance > 0)
        {
            RtkWristbandSys.gPedoData.global_distance += info->distance;
            RtkWristbandSys.gPedoData.quarter_distance += info->distance;
        }
        if (info->calories > 0)
        {
            RtkWristbandSys.gPedoData.global_calorys += info->calories;
            RtkWristbandSys.gPedoData.quarter_calories += info->calories;
        }


        if (RtkWristbandSys.gPedoData.global_steps >= RtkWristbandSys.gPedoData.daily_step_target &&
            !RtkWristbandSys.flag_field.daily_target_achieved)
        {
            RtkWristbandSys.flag_field.daily_target_achieved = true;
            wristband_system_interact(INTERACT_TARGET, 0, NULL);
        }
        if (RtkWristbandSys.flag_field.auto_sync_enable == true)
        {
            L1_send_event(L1SEND_SPORT_DATA, NULL);
        }
    }
    else
    {
        APP_PRINT_ERROR0("error: algorithm callback failed...");
    }

}

static void act_algo_cb(gsa_act_type_e type, uint16 para)
{
    act_cbs[type](para);
}

static void act_lift_hdl(uint16 para)
{
    APP_PRINT_INFO0("======lift_action_callback======\n");
    T_IO_MSG button_msg;
    button_msg.type = HUB_MSG_BUTTON;
    button_msg.subtype = SHORT_BUTTON_0;
    send_msg_to_ui_task(&button_msg, __LINE__);

}

static void act_wave_hdl(uint16 para)
{
    APP_PRINT_INFO1("~~~~~wave action call back:%d\n", para);

    /* send take photo action */
}

static void act_twist_hdl(uint16 para)
{
    APP_PRINT_INFO0("======lift_action_callback======\n");
    T_IO_MSG button_msg;
    button_msg.type = HUB_MSG_BUTTON;
    button_msg.subtype = SHORT_BUTTON_0;
    send_msg_to_ui_task(&button_msg, __LINE__);
}


void health_algo_init(void)
{
    usr_prof_t      *prof;
    gsa_gs_pos_t    pos;

    /*get user profile from flash in wristband config_data_init*/
    APP_PRINT_INFO4("f:%d a:%d h%d w%d\r\n", RtkWristbandSys.user_data.user_profile.bit_field.gender,
                    RtkWristbandSys.user_data.user_profile.bit_field.age,
                    RtkWristbandSys.user_data.user_profile.bit_field.height,
                    RtkWristbandSys.user_data.user_profile.bit_field.weight);

    prof = (usr_prof_t *)&RtkWristbandSys.user_data.user_profile.bit_field;
    pos.xpos = GSENSOR_XPOS;
    pos.zpos = GSENSOR_ZPOS;
    rtk_gsa_init(prof, &pos, gsa_cbs);

    RtkWristbandSys.flag_field.algorithm_started = true;
    RtkWristbandSys.wristband_sleep_status = TGET_UP;

    gsa_ver_t *rtk_algo_ver;
    rtk_algo_ver = rtk_gsa_get_ver();
    APP_PRINT_INFO3("health algo version:%d.%d.%d", rtk_algo_ver->major,
                    rtk_algo_ver->minor, rtk_algo_ver->revision);

    /*set action detection on/off*/
    rtk_gsa_act_switch(GSA_ACT_LIFT, RtkWristbandSys.flag_field.lift_switch_status);
    rtk_gsa_act_switch(GSA_ACT_TWIST,
                       RtkWristbandSys.flag_field.lift_switch_status);  // DO NOT modify to xxx.twist_switch_status
    rtk_gsa_act_switch(GSA_ACT_WAVE, RtkWristbandSys.flag_field.phone_camera_status);
}

void health_algo_reset(void)
{
    usr_prof_t      *prof;
    gsa_gs_pos_t    pos;
    prof = (usr_prof_t *)&RtkWristbandSys.user_data.user_profile.bit_field;
    pos.xpos = GSENSOR_XPOS;
    pos.zpos = GSENSOR_ZPOS;
    rtk_gsa_init(prof, &pos, gsa_cbs);
}









