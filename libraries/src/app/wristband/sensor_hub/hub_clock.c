/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     wristband_clock.c
* @brief
* @details
* @author
* @date     2017-10-31
* @version  v0.1
*********************************************************************************************************
*/

#include "trace.h"
#include "string.h"
#include "ftl.h"
#include "app_flash_config.h"
#include "app_task.h"
#include "rtl876x_rcc.h"
#include "rtl876x_rtc.h"
#include "rtl876x_nvic.h"
#include "hub_clock.h"
#include "wristband_global_data.h"
#include "communicate_protocol.h"
#include "communicate_sync_pedo.h"
#include "hub_interaction.h"
#include "hub_battery.h"
#include "bas.h"
#include <hub_task.h>
#include "otp.h"

#define     ClOCK_RTC_COMPARATOR        0
#define     RTC_CLOCK_SOURCE_FREQ       (OTP->bw_rf_low_clk_frac ? 32000:32768)


bool allowed_clock_enter_dlps = true;
pSystemClockCB SystemClockCallBack = NULL;

static const T_IO_MODULE_CBS clock_cbs =
{
    wristband_clock_driver_init,
    wristband_clock_enter_dlps,
    wristband_clock_exit_dlps,
    wristband_clock_allowed_enter_dlps_check,

};

void clock_add_hub_task(void)
{
    wristband_clock_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&clock_cbs;
}

static void wristband_clock_driver_init(void)
{
    //call module init
    system_clock_init(0);
}

static void wristband_clock_enter_dlps(void)
{
    return;
}
static void wristband_clock_exit_dlps(void)
{
    return;
}
bool wristband_clock_allowed_enter_dlps_check(void)
{
    return allowed_clock_enter_dlps;
}
/**
  * @brief   system clock init
  * @param   init offset secod, sometimes is zero
  * @return  void
  */
void system_clock_init(uint32_t second)
{
    RTC_DeInit();
    RTC_SetPrescaler(RTC_PRESCALER_VALUE);
    RTC_MaskINTConfig(RTC_INT_COMP0, ENABLE);
    RTC_INTConfig(RTC_INT_COMP0, DISABLE);

    /* Config RTC interrupt */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    RTC_SystemWakeupConfig(ENABLE);
    convert_to_utc_time(second);
    /* Init ticks from RTC */
    RtkWristbandSys.SecondCountRTC = second;       /* This should read from flash */
    RtkWristbandSys.pre_rtc_tick_count = 0;
    wall_clock_start();
}
/**
  * @brief   RTC interrupt handler
  * @param   No parameter.
  * @return  void
  */
void RTC_Handler()
{
    uint32_t CompareValue = 0;
    APP_PRINT_INFO3("RTC->INT_SR = 0x%x, RTC->CR0 = 0x%x, RTC INT_MASK = 0x%x", RTC->INT_SR, RTC->CR0,
                    RTC->INT_CLR);
    if (RTC_GetINTStatus(RTC_INT_COMP0) == SET)
    {
        APP_PRINT_INFO0("RTC_INT_CMP0");
        update_system_clock();

        CompareValue = RTC_GetCompValue(ClOCK_RTC_COMPARATOR) + \
                       (RTC_CLOCK_SOURCE_FREQ / (RTC_PRESCALER_VALUE + 1)) * (60 - RtkWristbandSys.SecondCountRTC %
                                                                              60); //minute interrupt

        RTC_SetCompValue(ClOCK_RTC_COMPARATOR, CompareValue & 0xFFFFFF);
        RTC_ClearCompINT(ClOCK_RTC_COMPARATOR);
    }
    if (RTC_GetINTStatus(RTC_INT_OVF) == SET)
    {
        APP_PRINT_INFO0("RTC Overflow");
        RTC_ClearOverFlowINT();
    }
}

/**
  * @brief   minute system clock messafe handle
  * @param   void
  * @return  void
  */
void minute_system_clock_message_handle(void)
{
    /* get UTCTime time */
    convert_to_utc_time(RtkWristbandSys.SecondCountRTC);

    APP_PRINT_INFO6("year:%d,month:%d,day:%d,hour:%d,minute:%d,second:%d\r\n",
                    RtkWristbandSys.Global_Time.year,
                    RtkWristbandSys.Global_Time.month, RtkWristbandSys.Global_Time.day,
                    RtkWristbandSys.Global_Time.hour, RtkWristbandSys.Global_Time.minutes,
                    RtkWristbandSys.Global_Time.seconds);
    if (SystemClockCallBack != NULL)
    {
        SystemClockCallBack();
    }
    /* check alarm event every minute */
    check_alarm();

    /* organize sportdata every minute*/
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        minute_sport_handler();
    }

    if ((RtkWristbandSys.Global_Time.hour >= 18) || \
        (RtkWristbandSys.Global_Time.hour <= 10))
    {
        RtkWristbandSys.flag_field.sleep_detect_enable = true;
    }
    else
    {
        RtkWristbandSys.flag_field.sleep_detect_enable = false;
    }
    if (RtkWristbandSys.Global_Time.hour == 17 && RtkWristbandSys.Global_Time.minutes == 59)
    {
        APP_PRINT_INFO0("do sleep algorithm reset for 19:59!");
        if (RtkWristbandSys.wristband_sleep_status != TGET_UP)
        {
            //health_algo_init();
        }
    }
    /* check battery level every minute */
    uint8_t current_battery_level_value = RtkWristbandSys.battery_level_value;
    RtkWristbandSys.battery_level_value = wristband_get_battery_level();
    if (RtkWristbandSys.battery_level_value != current_battery_level_value)
    {
        bas_battery_level_value_notify(RtkWristbandSys.wristband_conn_id, bas_ser_id,
                                       RtkWristbandSys.battery_level_value);
    }
    if ((RtkWristbandSys.battery_level_value < LOW_POWER_BATTERY_LEVEL) && \
        (RtkWristbandSys.flag_field.low_power_flag == false) &&
        (RtkWristbandSys.charger_status != InCharging))
    {
        APP_PRINT_INFO0("Wristband will coming into Low Power Protect Mode");
        T_IO_MSG msg;
        msg.type = HUB_MSG_BATTERY_ADC;
        msg.subtype = BATTERY_MSG_LOW_POWER;
        send_msg_to_ui_task(&msg, __LINE__);
    }

    if ((RtkWristbandSys.battery_level_value > RESTORE_POWER_BATTERY_LEVEL) && \
        (RtkWristbandSys.flag_field.low_power_flag == true))
    {
        APP_PRINT_INFO0("Wristband will coming out Low Power Protect Mode");
        T_IO_MSG msg;
        msg.type = HUB_MSG_BATTERY_ADC;
        msg.subtype = BATTERY_MSG_LOW_POWER;
        send_msg_to_ui_task(&msg, __LINE__);
    }
}



static uint8_t month_length_calc(uint8_t lpyr, uint8_t mon)
{
    uint8_t days = 31;

    if (mon == 1)   // feb
    {
        days = (28 + lpyr);
    }
    else
    {
        if (mon > 6)   // aug-dec
        {
            mon--;
        }

        if (mon & 1)
        {
            days = 30;
        }
    }
    return (days);
}


void convert_to_utc_time(uint32_t sec_time)
{
    /* calculate the time less than a day - hours, minutes, seconds */
    {
        uint32_t day = sec_time % DAY;
        RtkWristbandSys.Global_Time.seconds = day % 60UL;
        RtkWristbandSys.Global_Time.minutes = (day % 3600UL) / 60UL;
        RtkWristbandSys.Global_Time.hour = day / 3600UL;
    }

    /* Fill in the calendar - day, month, year */
    {
        uint16_t numDays = sec_time / DAY;
        RtkWristbandSys.Global_Time.year = BEGYEAR;
        while (numDays >= YearLength(RtkWristbandSys.Global_Time.year))
        {
            numDays -= YearLength(RtkWristbandSys.Global_Time.year);
            RtkWristbandSys.Global_Time.year++;
        }

        RtkWristbandSys.Global_Time.month = 0;
        while (numDays >= month_length_calc(IsLeapYear(RtkWristbandSys.Global_Time.year),
                                            RtkWristbandSys.Global_Time.month))
        {
            numDays -= month_length_calc(IsLeapYear(RtkWristbandSys.Global_Time.year),
                                         RtkWristbandSys.Global_Time.month);
            RtkWristbandSys.Global_Time.month++;
        }

        RtkWristbandSys.Global_Time.day = numDays;
        RtkWristbandSys.Global_Time.month = RtkWristbandSys.Global_Time.month + 1;
        RtkWristbandSys.Global_Time.day = RtkWristbandSys.Global_Time.day + 1;
    }
}


void check_alarm(void)
{
    uint8_t index;
    DAY_OF_WEEK day_of_week;
    bool alarmchanged = false;

    for (index = 0; index < RtkWristbandSys.alarm_num; index ++)
    {
        if (RtkWristbandSys.Global_Time.hour == RtkWristbandSys.alarms[index].alarm.hour &&
            RtkWristbandSys.Global_Time.minutes == RtkWristbandSys.alarms[index].alarm.minute) // time
        {
            day_of_week = get_day_of_week(RtkWristbandSys.SecondCountRTC); // day of week

            if (RtkWristbandSys.alarms[index].alarm.day_repeat_flag == 0)
            {
                if ((RtkWristbandSys.Global_Time.day == RtkWristbandSys.alarms[index].alarm.day &&
                     RtkWristbandSys.Global_Time.month == RtkWristbandSys.alarms[index].alarm.month &&
                     RtkWristbandSys.Global_Time.year == RtkWristbandSys.alarms[index].alarm.year + 2000))
                {
                    APP_PRINT_INFO0("HIT ALARM OF ONE TIME");
                    do_rtc_alarm();
                    //use reserved bit to mark alarm has been alarmed
                    RtkWristbandSys.alarms[index].alarm.reserved = 0x0;
                    alarmchanged = true;
                }
            }
            else
            {
                if (RtkWristbandSys.alarms[index].alarm.day_repeat_flag & (1 << (day_of_week)))
                {
                    APP_PRINT_INFO0("HIT ALARM DAY OF WEEK");
                    do_rtc_alarm();
                }
            }
        }
    }
    if (alarmchanged)
    {
        ftl_save((void *)RtkWristbandSys.alarms, ALARM_OFFSET, ALARM_SIZE);
    }

}

void check_passed_onetime_alarm(void)
{
    uint8_t index;
    bool alarmchanged = false;
    time_union_t time;

    for (index = 0; index < RtkWristbandSys.alarm_num; index ++)
    {
        /* check if the alarm is passed*/
        if (RtkWristbandSys.alarms[index].alarm.day_repeat_flag == 0)
        {
            time.time.seconds = 0;
            time.time.minute = RtkWristbandSys.alarms[index].alarm.minute;
            time.time.hours = RtkWristbandSys.alarms[index].alarm.hour;
            time.time.day = RtkWristbandSys.alarms[index].alarm.day;
            time.time.month = RtkWristbandSys.alarms[index].alarm.month;
            time.time.year = RtkWristbandSys.alarms[index].alarm.year;
            uint32_t time_stamp = convert_time_to_second(time);

            if ((RtkWristbandSys.SecondCountRTC >= time_stamp) && RtkWristbandSys.alarms[index].alarm.reserved)
            {
                RtkWristbandSys.alarms[index].alarm.reserved = 0x0;
                alarmchanged = true;
            }
            else if ((RtkWristbandSys.SecondCountRTC < time_stamp) &&
                     (!(RtkWristbandSys.alarms[index].alarm.reserved)))
            {
                RtkWristbandSys.alarms[index].alarm.reserved = 0x01;
                alarmchanged = true;
            }
        }
    }

    if (alarmchanged)
    {
        ftl_save((void *)RtkWristbandSys.alarms, ALARM_OFFSET, ALARM_SIZE);
        L1_send_event(L1SEND_RETURN_ALARM_EVENT, NULL);
    }
}


void update_system_clock(void)
{
    T_IO_MSG clock_msg;
    RtkWristbandSys.SecondCountRTC = RtkWristbandSys.SecondCountRTC + (60 -
                                                                       RtkWristbandSys.SecondCountRTC % 60);
    RtkWristbandSys.pre_rtc_tick_count = RTC_GetCounter();

    clock_msg.type = HUB_MSG_CLOCK;
    send_msg_to_ui_task(&clock_msg, __LINE__);

}




void do_rtc_alarm(void)
{
    APP_PRINT_INFO0("do_rtc_alarm\n");
    if (RtkWristbandSys.flag_field.power_off_flag == false)
    {
        wristband_system_interact(INTERACT_ALARM, 0, NULL);
    }
    L1_send_event(L1SEND_RETURN_ALARM_EVENT, NULL);
}

void wall_clock_start(void)
{
    uint32_t CompareValue;
    APP_PRINT_INFO1("wristband clock_start second_diff_value: %d",
                    (60 - RtkWristbandSys.SecondCountRTC % 60));

    CompareValue = RTC_GetCounter() + (RTC_CLOCK_SOURCE_FREQ / (RTC_PRESCALER_VALUE + 1)) *
                   (60 - RtkWristbandSys.SecondCountRTC % 60);

    RTC_SetCompValue(ClOCK_RTC_COMPARATOR, CompareValue & 0xFFFFFF);

    RTC_MaskINTConfig(RTC_INT_COMP0, DISABLE);
    RTC_INTConfig(RTC_INT_COMP0, ENABLE);
    RTC_NvCmd(ENABLE);
    RTC_Cmd(ENABLE);
}


uint32_t convert_time_to_second(time_union_t time)
{
    uint32_t i = 0;
    uint32_t offset = 0;

    /* day time */
    offset += time.time.seconds;
    offset += time.time.minute * 60;
    offset += time.time.hours * 60 * 60;

    uint8_t leapYear = IsLeapYear(time.time.year + 2000);

    offset += DAY * (time.time.day - 1);

    for (i = 0; i < time.time.month - 1; ++i)
    {
        /* month start from 1 */
        offset += month_length_calc(leapYear, i) * DAY;
    }

    for (i = 0; i < time.time.year ; ++i)
    {
        if (IsLeapYear(i + 2000))
        {
            offset += DAY * 366;
        }
        else
        {
            offset += DAY * 365;
        }
    }

    return offset;
}


void set_wristband_clock(time_union_t time)
{
    uint32_t i = 0;
    uint32_t offset = 0;

    //day time
    offset += time.time.seconds;
    offset += time.time.minute * 60;
    offset += time.time.hours * 60 * 60;

    uint8_t leapYear = IsLeapYear(time.time.year + 2000);

    offset += DAY * (time.time.day - 1);

    for (i = 0; i < time.time.month - 1; ++i)
    {
        //month start from 1
        offset += month_length_calc(leapYear, i) * DAY;
    }

    for (i = 0; i < time.time.year ; ++i)
    {
        if (IsLeapYear(i + 2000))
        {
            offset += DAY * 366;
        }
        else
        {
            offset += DAY * 365;
        }
    }
    RtkWristbandSys.SecondCountRTC = offset;
    convert_to_utc_time(RtkWristbandSys.SecondCountRTC);
}



/* calculate day of week */
DAY_OF_WEEK get_day_of_week(uint32_t secTime)
{
    uint32_t day = secTime / DAY;

    DAY_OF_WEEK today = (DAY_OF_WEEK)(((day % LENGTH_OF_WEEK) + SYSTEM_ORIGIN_DAY_OF_WEEK) %
                                      LENGTH_OF_WEEK);

    return today;
}


uint8_t get_system_clock_second(void)
{
    if (RtkWristbandSys.flag_field.bond_state == false)
    {
        APP_PRINT_INFO0("get system clock second error !!! ");
        return 0x55;
    }
    uint32_t cur_rtc_tick_count = RTC_GetCounter();
    uint32_t diff_second = 0;
    if (cur_rtc_tick_count > RtkWristbandSys.pre_rtc_tick_count)
    {
        diff_second = (cur_rtc_tick_count - RtkWristbandSys.pre_rtc_tick_count) / RTC_CLOCK_SOURCE_FREQ;
    }
    else
    {
        diff_second = (cur_rtc_tick_count + 0xffffff - RtkWristbandSys.pre_rtc_tick_count) /
                      RTC_CLOCK_SOURCE_FREQ;
    }
    return (RtkWristbandSys.SecondCountRTC + diff_second) % 60;
}
