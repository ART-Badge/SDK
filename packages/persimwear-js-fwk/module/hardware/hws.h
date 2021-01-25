/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-09     zhangsz      init first
 */

#ifndef __HWS_H__
#define __HWS_H__

#include <rtthread.h>

#if defined(JSFW_USING_HARDWARE)
#include <rthw.h>
#include <rtdevice.h>
#include "dcmtype.h"
#include "emq_hub.h"

EMQ_HUB_IMPORT(hws);

enum EMQ_HWS_HUB_ID
{
    EMQ_HWS_KEY = 0,
    EMQ_HWS_RTC = 1,
    EMQ_HWS_TP = 2,
    EMQ_HWS_LCD = 3,
    EMQ_HWS_PM = 4,
    EMQ_HWS_BEEP = 5,
    EMQ_HWS_MAX,
};

enum MSG_HW_ID
{
    /*MSG ID for KEY service*/
    MSG_ID_KEY_BASE = 0x0000,
    MSG_ID_KEY_UP = 0x0001,
    MSG_ID_KEY_PRESS = 0x0002,
    MSG_ID_KEY_LONGPRESS = 0x0003,
    MSG_ID_KEY_LONG_UP = 0x0004,

    /*MSG ID for VBAT service*/
    MSG_ID_VBAT_BASE = 0x0600,
    MSG_ID_VBAT_READ = 0x0601,

    /*MSG ID for LCD service*/
    MSG_ID_LCD_BASE = 0x0620,
    MSG_ID_LCD_ON = 0x0621,
    MSG_ID_LCD_OFF = 0x0622,
    MSG_ID_LCD_SET_MODE = 0x0623,
    MSG_ID_LCD_SET_BRIGHTNESS = 0x0624,
    MSG_ID_LCD_GET_STATUS = 0x0625,
    MSG_ID_LCD_GET_MODE = 0x0626,

    /*MSG ID for TP service*/
    MSG_ID_TP_BASE = 0x0630,
    MSG_ID_TP_ON = 0x0631,
    MSG_ID_TP_OFF = 0x0632,
    MSG_ID_TP_GET_STATUS = 0x0633,

    /*MSG ID for sys service*/
    MSG_ID_SYS_BASE = 0x0640,
    MSG_ID_SYS_REBOOT = 0x0641,
    MSG_ID_SYS_SHUTDOWN = 0x0642,
    MSG_ID_SYS_VIBRATE = 0x0643,
    MSG_ID_SYS_INFO = 0x0644,

    /*MSG ID for BEEP service*/
    MSG_ID_BEEP_BASE = 0x0650,
    MSG_ID_BEEP_ON = 0x0651,
    MSG_ID_BEPEP_OFF = 0x0652,
    MSG_ID_BEEP_SET_PERIOD_DUTY = 0x0653,
};

/* export HWS API List */
void hws_key_send_event(rt_uint32_t msg_id);
void hws_battery_level_update(rt_uint8_t level);
void hws_charge_status_update(rt_uint8_t status);
void hws_screen_on(void);
void hws_screen_off(void);
void hws_touch_panel_on(void);
void hws_touch_panel_off(void);
void hws_screen_status_update(rt_uint8_t lcdStatus);
void hws_screen_mode_update(rt_uint8_t lcdmode);
void hws_screen_set_brightness(rt_uint8_t brightness);
void hws_brightness_update(rt_uint8_t brightness);
void hws_touch_panel_status_update(rt_uint8_t tpstatus);
void hws_rtc_realtime_update(time_t timestamp);
void hws_system_info_update(const char *sysinfo);
rt_err_t hws_screen_graphic_update(void);
void hws_realtime_timer_start(void);
void hws_realtime_timer_stop(void);

#endif

#endif
