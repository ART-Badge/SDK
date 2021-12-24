/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file
* @brief
* @details
* @author
* @date
* @version
*********************************************************************************************************
*/
#include "board.h"
#include "trace.h"
#include "string.h"
#include "os_timer.h"
#include "os_mem.h"
#include "os_sync.h"
#include "hub_clock.h"
#include "hub_display.h"
#include "module_font.h"
#include "wristband_picture_res.h"
#include "wristband_global_data.h"
#include "math.h"
#include <string.h>
#include <stdarg.h>
#include "hub_interaction.h"
#include "wristband_gui.h"
#include "app_msg.h"
#include "app_task.h"
#include "touch_algorithm.h"
#include "stdio.h"
#include "gui_slide.h"
#include "gui_core.h"
#include "communicate_parse.h"
#include "stdlib.h"
#include "gui_lcd_callback.h"
#define MSGMODE_PAGE_NUM      29
#define MSG_BOX_HIGHT      104
#define PER_MSG_BOX_WIDGET_COUNT 7
#define DELETE_WIDGET_WIDTH 100
static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);
static void TouchDelMessage(uint32_t button_touch_type);

const static int16_t offset_up_y[PER_MSG_BOX_WIDGET_COUNT] = {240, 240, 252, 253, 252, 280, 312};
const static int16_t offset_down_y[PER_MSG_BOX_WIDGET_COUNT] = {-72, -72, -60, -59, -60, -32, 0};
const static uint8_t message_y_index[3] = {1, 1 + PER_MSG_BOX_WIDGET_COUNT, 1 + 2 * PER_MSG_BOX_WIDGET_COUNT};
static int8_t cur_show_msg = 0;
static int32_t cur_detal_y_mov = 0;
static int32_t detal_x_mov = 0;
static int32_t last_info_deltaX = 0;
static bool time_dot_flag = true;
static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;
static RtkTouchInfo_t info;

const static UI_WidgetTypeDef  messagemode_icons[MSGMODE_PAGE_NUM] =
{
    [0]     = {
        .x = 0,      .y = 0,     .width = 240,    .hight = 240,    .addr = MESSAGE_BACKGROUND0_BIN,    .widget_id_type = ICON_BACKGROUND
    },

    [1]     = {
        .x = 0,   .y = 32,    .width = 230, .hight = 1,     .addr = NULL/*SLIDING_DIVIDING_LINE_BIN*/, .widget_id_type = ICON_WIDGET,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [2]     = {
        .x = 0,   .y = 32,    .width = 40, .hight = 40,     .addr = NULL,   .widget_id_type = ICON_WIDGET,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [3]     = {//for clock hour
        .x = 90,   .y = 44,    .width = 13, .hight = 20,     .addr = NULL,   .widget_id_type = ICON_NUM,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [4]     = {//for dot
        .x = 116,   .y = 45,    .width = 6, .hight = 18,     .addr = NULL,   .widget_id_type = ICON_WIDGET,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [5]     = {//for clock minute
        .x = 122,   .y = 44,    .width = 13, .hight = 20,     .addr = NULL,   .widget_id_type = ICON_NUM,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [6]     = {
        .x = 0,   .y = 72,    .width = 230, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [7]     = {
        .x = 0,   .y = 104,    .width = 230, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },

    [8]     = {
        .x = 0,   .y = 136,    .width = 230, .hight = 1,     .addr = NULL,   .widget_id_type = ICON_WIDGET,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [9]     = {
        .x = 0,   .y = 136,    .width = 40, .hight = 40,     .addr = NULL,   .widget_id_type = ICON_WIDGET,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [10]     = {//for clock hour
        .x = 90,   .y = 148,    .width = 13, .hight = 20,     .addr = NULL,   .widget_id_type = ICON_NUM,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [11]     = {//for dot
        .x = 116,   .y = 149,    .width = 6, .hight = 18,     .addr = NULL,   .widget_id_type = ICON_WIDGET,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [12]     = {//for clock minute
        .x = 122,   .y = 148,    .width = 13, .hight = 20,     .addr = NULL,   .widget_id_type = ICON_NUM,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [13]     = {
        .x = 0,   .y = 176,    .width = 230, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [14]     = {
        .x = 0,   .y = 208,    .width = 230, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING,
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },



    [15]     = {
        .x = 0,   .y = 240,    .width = 230, .hight = 1,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [16]     = {
        .x = 0,   .y = 240,    .width = 40, .hight = 40,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [17]     = {//for clock hour
        .x = 90,   .y = 252,    .width = 13, .hight = 20,     .addr = NULL,   .widget_id_type = ICON_NUM, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [18]     = {//for dot
        .x = 116,   .y = 253,    .width = 6, .hight = 18,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [19]     = {//for clock minute
        .x = 122,   .y = 252,    .width = 13, .hight = 20,     .addr = NULL,   .widget_id_type = ICON_NUM, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },

    [20]     = {
        .x = 0,   .y = 280,    .width = 230, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },
    [21]     = {
        .x = 0,   .y = 312,    .width = 230, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false, .active_ys = 32, .active_ye = 240,
    },

    [22]     = {
        .x = 140,      .y = 70,     .width = DELETE_WIDGET_WIDTH,    .hight = DELETE_WIDGET_WIDTH,    .addr = NULL,/*SLIDING_DELETE_ICON_BIN*/.widget_id_type = ICON_WIDGET, \
        .touch_capability = true, .widget_touch_func = TouchDelMessage
    },
    [23]     = {//for clock hour
        .x = 95,   .y = 6,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [24]     = {//for clock minute
        .x = 122,   .y = 6,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [25]     = {//for dot
        .x = 116,   .y = 7,    .width = 6, .hight = 18,     .addr = SLIDING_TIME_DOT_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [26]     = {
        .x = 220,   .y = 6,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [27]     = {
        .x = 230,    .y = 32,    .width = 1,     .hight = 208,     .addr = SLIDING_BOX_BIN, .widget_id_type = ICON_WIDGET,
        .active_ys = 32, .active_ye = 240,
    },
    [28]     = {
        .x = 230,    .y = 32,    .width = 10,     .hight = 208,     .addr = SLIDER_BIN, .widget_id_type = ICON_WIDGET, \
        .active_ys = 32, .active_ye = 240,
    },

};

UI_MenuTypeDef MessageMenu =
{
    .name = (const char *)"MessageMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)messagemode_icons,
    .current_max_icon = MSGMODE_PAGE_NUM,
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  MessageMenu.name, strlen(MessageMenu.name)))
    {
        return;
    }

    if ((timer_id == 1) && (display_timer != NULL))
    {
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, BLACK);
        wristband_display_power_off();
        pCurrentMenu->destructor_func();
    }
    else if ((timer_id == 2) && (display_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        time_dot_flag = !time_dot_flag;
        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_menu_update(&MessageMenu, NULL, NULL, NULL, NULL, NULL, detal_x_mov, 0);
        }
    }
    else if ((timer_id == 3) && (display_timer != NULL))
    {
        UI_MenuTypeDef *p_menu = rtl_gui_menu_real_slide();
        if (p_menu != NULL)
        {
            rtl_gui_menu_update(&MessageMenu, NULL, NULL, NULL, NULL, NULL, detal_x_mov, 0);
            os_timer_stop(&page_switch_timer);
        }
    }
}


static void ConstructorCallbackFun(uint32_t type, void *pValue)
{
    if (pValue != NULL)
    {
        uint16_t data[MSG_DATA_PACKAGE_LEN] = {0};
        uint16_t value_length = ((MSG_DATA_PAYLOAD *)pValue)->length - 1;
        uint8_t msg_type = ((MSG_DATA_PAYLOAD *)pValue)->p_msg_value[0];
        uint8_t *p_payload = ((MSG_DATA_PAYLOAD *)pValue)->p_msg_value + 1;
        uint16_t show_length = value_length < MSG_DATA_PACKAGE_LEN - 4 ? value_length : MSG_DATA_PACKAGE_LEN
                               - 4 ;
        utf8_to_unicode(p_payload, value_length, data + 4, show_length);
        data[0] = show_length;
        data[1] = msg_type;
        data[2] = RtkWristbandSys.Global_Time.hour;
        data[3] = RtkWristbandSys.Global_Time.minutes;
        os_mutex_take(flash_mutex_handle, 500);
        WristBandMsgDataStore((uint8_t *)data);
        os_mutex_give(flash_mutex_handle);
    }

    wristband_display_power_on();
    wristband_display_enable();
    rtl_frame_rate(0x1f, 0x3f, 0x3f);
    pCurrentMenu->dynamicFlag = true;
    cur_detal_y_mov = 0;
    detal_x_mov = 0;
    cur_show_msg = 0;
    last_info_deltaX = 0;
    rtl_gui_menu_update(&MessageMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 500, true, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
    os_timer_start(&dynamic_timer);
    os_timer_start(&display_timer);
}


static void DestructorCallbackFun(void)
{
    pCurrentMenu->dynamicFlag = false;

    if (dynamic_timer)
    {
        os_timer_stop(&dynamic_timer);
        os_timer_delete(&dynamic_timer);
    }
    if (display_timer)
    {
        os_timer_stop(&display_timer);
        os_timer_delete(&display_timer);
    }
    if (page_switch_timer)
    {
        os_timer_stop(&page_switch_timer);
        os_timer_delete(&page_switch_timer);
    }
    dynamic_timer = NULL;
    display_timer = NULL;
    page_switch_timer = NULL;
    rtl_frame_rate(0x1f, 0x0c, 0x0c);
}

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }
    return_menu->current_max_icon = MessageMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, MessageMenu.pWidgetList,
           MessageMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

    static uint16_t msg_data[MSG_DATA_PACKAGE_LEN * 3] = {};
    static char time_hour_data[4 * 3] = {'\0'};
    static char time_min_data[4 * 3] = {'\0'};
    memset(time_hour_data, '\0', 4 * 3 * sizeof(char));
    memset(time_min_data, '\0', 4 * 3 * sizeof(char));
    static char time_hour_string[4] = {'\0'};
    memset(time_hour_string, '\0', sizeof(RtkWristbandSys.Global_Time.hour));
    sprintf(time_hour_string, "%02d", RtkWristbandSys.Global_Time.hour);
    return_menu->pWidgetList[23].u.num_string_addr = (uint32_t)time_hour_string;
    static char time_min_string[4] = {'\0'};
    memset(time_min_string, '\0', sizeof(RtkWristbandSys.Global_Time.minutes));
    sprintf(time_min_string, "%02d", RtkWristbandSys.Global_Time.minutes);
    return_menu->pWidgetList[24].u.num_string_addr = (uint32_t)time_min_string;

    if (time_dot_flag)
    {
        return_menu->pWidgetList[25].addr = NULL;
    }
    static char cur_msg_count_string[4] = {'\0'};
    memset(cur_msg_count_string, '\0', sizeof(WristBandGetCurMsgCount()));
    if (WristBandGetCurMsgCount())
    {
        sprintf(cur_msg_count_string, "%02d", WristBandGetCurMsgCount());
    }
    return_menu->pWidgetList[26].u.num_string_addr = (uint32_t)cur_msg_count_string;

    return_menu->pWidgetList[22].x = LCD_WIDTH;
    return_menu->pWidgetList[22].addr = SLIDING_DELETE_ICON_BIN;


    if (argv != NULL)
    {
        memcpy(&info, (RtkTouchInfo_t *)argv, sizeof(RtkTouchInfo_t));

        if (cur_detal_y_mov - info.deltaY < 0 || WristBandGetCurMsgCount() <= 2)
        {
            info.deltaY = cur_detal_y_mov;
        }
        else if (cur_detal_y_mov - info.deltaY >= (WristBandGetCurMsgCount() - 2)*MSG_BOX_HIGHT)
        {
            info.deltaY = cur_detal_y_mov - (WristBandGetCurMsgCount() - 2) * MSG_BOX_HIGHT;
        }


        if (info.deltaY > 0)
        {
            for (uint8_t i = 0; i < PER_MSG_BOX_WIDGET_COUNT; i++)
            {
                return_menu->pWidgetList[message_y_index[0] + i].y = offset_down_y[i] +
                                                                     (info.deltaY - 1) % MSG_BOX_HIGHT;
                return_menu->pWidgetList[message_y_index[1] + i].y = return_menu->pWidgetList[message_y_index[0] +
                                                                                              i].y + MSG_BOX_HIGHT;
                return_menu->pWidgetList[message_y_index[2] + i].y = return_menu->pWidgetList[message_y_index[1] +
                                                                                              i].y + MSG_BOX_HIGHT;
            }

        }
        else if (info.deltaY < 0)
        {
            for (uint8_t i = 0; i < PER_MSG_BOX_WIDGET_COUNT; i++)
            {
                return_menu->pWidgetList[message_y_index[2] + i].y = offset_up_y[i] + (info.deltaY) % MSG_BOX_HIGHT;
                return_menu->pWidgetList[message_y_index[1] + i].y = return_menu->pWidgetList[message_y_index[2] +
                                                                                              i].y - MSG_BOX_HIGHT;
                return_menu->pWidgetList[message_y_index[0] + i].y = return_menu->pWidgetList[message_y_index[1] +
                                                                                              i].y - MSG_BOX_HIGHT;
            }
        }

        if (info.type == TOUCH_HOLD_Y)
        {
            cur_show_msg = (cur_detal_y_mov - info.deltaY) / MSG_BOX_HIGHT;
        }
        else
        {
            cur_detal_y_mov -= info.deltaY;
            cur_show_msg = cur_detal_y_mov / MSG_BOX_HIGHT + ((cur_detal_y_mov % MSG_BOX_HIGHT) >
                                                              (MSG_BOX_HIGHT / 2) ? 1 : 0);
            for (uint8_t i = 0; i < PER_MSG_BOX_WIDGET_COUNT; i++)
            {
                return_menu->pWidgetList[message_y_index[0] + i].y = messagemode_icons[message_y_index[0] + i].y;
                return_menu->pWidgetList[message_y_index[1] + i].y = messagemode_icons[message_y_index[1] + i].y;
                return_menu->pWidgetList[message_y_index[2] + i].y = messagemode_icons[message_y_index[2] + i].y;

            }
            cur_detal_y_mov = cur_show_msg * MSG_BOX_HIGHT;
            info.deltaY = 0;
        }

    }

    for (uint8_t i = 0; i < 3; i++)
    {
        if (WristBandMsgDataRestore((uint8_t *)(msg_data + i * MSG_DATA_PACKAGE_LEN), (cur_show_msg + i)))
        {
            uint8_t line1_length = 0;
            uint8_t line2_length = 0;
            uint8_t temp1_length = 0;
            uint8_t temp2_length = 0;
            for (uint8_t j = 0; j < msg_data[i * MSG_DATA_PACKAGE_LEN]; j++)
            {
                if ((temp1_length + (msg_data[4 + i * MSG_DATA_PACKAGE_LEN + j] >= 0x4e00 ? 2 : 1)) <= 14)
                {
                    temp1_length += (msg_data[4 + i * MSG_DATA_PACKAGE_LEN + j] >= 0x4e00 ? 2 : 1);
                    line1_length++;

                }
                else
                {
                    break;
                }
            }
            for (uint8_t j = line1_length; j < msg_data[i * MSG_DATA_PACKAGE_LEN]; j++)
            {
                if ((temp2_length + (msg_data[4 + i * MSG_DATA_PACKAGE_LEN + j] >= 0x4e00 ? 2 : 1)) <= 14)
                {
                    temp2_length += (msg_data[4 + i * MSG_DATA_PACKAGE_LEN + j] >= 0x4e00 ? 2 : 1);
                    line2_length++;
                }
                else
                {
                    break;
                }
            }
            return_menu->pWidgetList[message_y_index[i] + 0].addr =  SLIDING_DIVIDING_LINE_BIN;
            if (msg_data[i * MSG_DATA_PACKAGE_LEN + 1] & (QQ_NOTIFY_BIT))
            {
                return_menu->pWidgetList[message_y_index[i] + 1].addr =  NOTIFY_QQ_BIN;
            }
            else if (msg_data[i * MSG_DATA_PACKAGE_LEN + 1] & (WECHAT_NOTIFY_BIT))
            {
                return_menu->pWidgetList[message_y_index[i] + 1].addr =  NOTIFY_WECHAT_BIN;
            }
            else if (msg_data[i * MSG_DATA_PACKAGE_LEN + 1] & (SHOTMESSAGE_NOTIFY_BIT))
            {
                return_menu->pWidgetList[message_y_index[i] + 1].addr =  NOTIFY_SMS_BIN;
            }
            else
            {
                return_menu->pWidgetList[message_y_index[i] + 1].addr =  NOTIFY_LINE_BIN;
            }

            sprintf(time_hour_data + 4 * i, "%02d", msg_data[i * MSG_DATA_PACKAGE_LEN + 2]);
            return_menu->pWidgetList[message_y_index[i] + 2].u.num_string_addr =
                (uint32_t)(time_hour_data + 4 * i);
            return_menu->pWidgetList[message_y_index[i] + 2].addr = (uint32_t)NUMBERS_13X20_BLUE_BIN;

            return_menu->pWidgetList[message_y_index[i] + 3].addr =  SLIDING_TIME_DOT_BIN;

            sprintf(time_min_data + 4 * i, "%02d", msg_data[i * MSG_DATA_PACKAGE_LEN + 3]);
            return_menu->pWidgetList[message_y_index[i] + 4].u.num_string_addr =
                (uint32_t)(time_min_data + 4 * i);
            return_menu->pWidgetList[message_y_index[i] + 4].addr = (uint32_t)NUMBERS_13X20_BLUE_BIN;

            return_menu->pWidgetList[message_y_index[i] + 5].u.string_mem.font_string_addr =
                (uint32_t)(msg_data + 4 + i * MSG_DATA_PACKAGE_LEN);
            return_menu->pWidgetList[message_y_index[i] + 5].u.string_mem.total_count = line1_length;
            return_menu->pWidgetList[message_y_index[i] + 5].u.string_mem.current_number = 0;
            return_menu->pWidgetList[message_y_index[i] + 5].u.string_mem.single_font_height = 32;
            return_menu->pWidgetList[message_y_index[i] + 5].u.string_mem.font_color = 0xFFFF;
            return_menu->pWidgetList[message_y_index[i] + 5].u.string_mem.new_font = true;

            return_menu->pWidgetList[message_y_index[i] + 6].u.string_mem.font_string_addr =
                (uint32_t)(msg_data + 4 + line1_length + i * MSG_DATA_PACKAGE_LEN);
            return_menu->pWidgetList[message_y_index[i] + 6].u.string_mem.total_count = line2_length;

            return_menu->pWidgetList[message_y_index[i] + 6].u.string_mem.current_number = 0;
            return_menu->pWidgetList[message_y_index[i] + 6].u.string_mem.single_font_height = 32;
            return_menu->pWidgetList[message_y_index[i] + 6].u.string_mem.font_color = 0xFFFF;
            return_menu->pWidgetList[message_y_index[i] + 6].u.string_mem.new_font = true;



        }
    }
    //Sliding box
    if (info.type != TOUCH_HOLD_Y)
    {
        info.deltaY = 0;
    }
    return_menu->pWidgetList[28].hight /= WristBandGetCurMsgCount() > 0 ? WristBandGetCurMsgCount() : 1;
    if (WristBandGetCurMsgCount() > 2)
    {
        return_menu->pWidgetList[28].y = return_menu->pWidgetList[28].y + (cur_detal_y_mov - info.deltaY)
                                         * (return_menu->pWidgetList[27].hight - return_menu->pWidgetList[28].hight)
                                         / ((WristBandGetCurMsgCount() - 2) * MSG_BOX_HIGHT);
    }

}
static void ButtonCallbackFun(uint8_t type)
{
    APP_PRINT_INFO0("[ButtonCallback]");
    detal_x_mov = 0;
    os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
    pCurrentMenu->destructor_func();
    pCurrentMenu = &ClockSpecialMenu;
    pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    os_timer_stop(&page_switch_timer);
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
#if 1
    uint32_t timer_state;
    os_timer_state_get(&page_switch_timer, &timer_state);
    if (timer_state)
    {
        return;
    }
#endif
    int32_t temp = info->deltaX - last_info_deltaX;
    os_timer_restart(&display_timer, 500000);
    os_timer_restart(&dynamic_timer, 500);
    if (info->type == TOUCH_HOLD)
    {
        if (detal_x_mov + temp >= -DELETE_WIDGET_WIDTH && detal_x_mov + temp <= 0)
        {
            detal_x_mov += temp;
        }
        else if (detal_x_mov + temp > 0)
        {
            detal_x_mov = 0;
        }
        else if (detal_x_mov + temp < -DELETE_WIDGET_WIDTH)
        {
            detal_x_mov = -DELETE_WIDGET_WIDTH;
        }
        rtl_gui_menu_update(&MessageMenu, NULL, NULL, NULL, NULL, NULL, detal_x_mov, 0);
        last_info_deltaX = info->deltaX;

    }
    if (info->type == TOUCH_HOLD_Y || info->type == TOUCH_ORIGIN_FROM_Y ||
        info->type == TOUCH_DOWN_SLIDE || info->type == TOUCH_UP_SLIDE)
    {
        rtl_gui_menu_update(&MessageMenu, info, NULL, NULL, NULL, NULL, detal_x_mov, 0);
    }
    if (info->type == TOUCH_RIGHT_SLIDE)
    {
        last_info_deltaX = 0;
    }
    if (info->type == TOUCH_LEFT_SLIDE)
    {
        last_info_deltaX = 0;
    }
    if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        last_info_deltaX = 0;
        if (detal_x_mov == 0 || detal_x_mov == -DELETE_WIDGET_WIDTH)
        {
        }
        else
        {
            os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
            if (info->deltaX >= 0)
            {
                detal_x_mov = info->deltaX > DELETE_WIDGET_WIDTH / 2 ? 0 : -DELETE_WIDGET_WIDTH;
                rtl_gui_menu_slide(&MessageMenu, NULL, NULL, NULL, NULL, NULL, info->deltaX - DELETE_WIDGET_WIDTH,
                                   detal_x_mov, 0, 0);
            }
            else
            {
                detal_x_mov = info->deltaX > -DELETE_WIDGET_WIDTH / 2 ? 0 : -DELETE_WIDGET_WIDTH;
                rtl_gui_menu_slide(&MessageMenu, NULL, NULL, NULL, NULL, NULL, info->deltaX, detal_x_mov, 0, 0);
            }
        }
    }

}
static void TouchDelMessage(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON && detal_x_mov == -DELETE_WIDGET_WIDTH)
    {
        APP_PRINT_INFO0("[DelMessage]");
        detal_x_mov = 0;
        os_mutex_take(flash_mutex_handle, 500);
        WristBandMSGDataBlockInit();
        os_mutex_give(flash_mutex_handle);
        rtl_gui_menu_update(&MessageMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
}
