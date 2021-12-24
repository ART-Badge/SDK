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
#include "hub_display.h"
#include "wristband_picture_res.h"
#include "wristband_global_data.h"
#include "math.h"
#include "hub_battery.h"
#include <string.h>
#include <stdarg.h>

#include "hub_interaction.h"
#include "wristband_gui.h"
#include "app_msg.h"
#include "app_task.h"
#include "touch_algorithm.h"
#include "arm_math.h"
#include "stdio.h"
#include "gui_core.h"
#include "gui_slide.h"
#include "gui_psram.h"
#include "hub_task.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);

#define WATCHFACE_PAGE_NUM      7

#define MY_PI                  (-3.1415926f)
#define HOUR_HEIGHT            (210 - 74)
#define HOUR_WIDTH             (192 - 168)
#define HOUR_CENTER_HEIGHT     (180 - 74)
#define MIN_HEIGHT             (216 - 47)
#define MIN_WIDTH              (191 - 169)
#define MIN_CENTER_HEIGHT      (180 - 47)
#define SEC_HEIGHT             (222 - 40)
#define SEC_WIDTH              (186 - 173)
#define SEC_CENTER_HEIGHT      (180 - 40)

const static UI_WidgetTypeDef  icons[WATCHFACE_PAGE_NUM] =
{
    [0]     = {
        .x = 0,    .y = 0,    .width = 240,    .hight = 240,    .addr = WATCH_BACKGROUND_BIN,    .widget_id_type = ICON_BACKGROUND, \
        .touch_capability = false,
    },
    [1]     = {.x = 109,  .y = 30,   .width = 22,    .hight = 14,    .addr = COMMON_DISCONNECT_BIN,    .widget_id_type = ICON_WIDGET},
    [2]     = {.x = 180,  .y = 95,   .width = 10,     .hight = 19,   .addr = COMMON_P10X19WHITE_BIN,  .widget_id_type = ICON_NUM},
    [3]     = {.x = 170,  .y = 120,  .width = 26,    .hight = 12,    .addr = COMMON_WEEK_WHITE_BIN,    .widget_id_type = ICON_WIDGET},
    [4]     = {
        .x = 110,  .y = 64,   .width = 10,     .hight = 60,      .addr = WATCH_HOUR_HAND_BIN,   .widget_id_type = ICON_CLOCK_POINTER, \
        .u.clock_pointer.rotation_cosa = 1.0f, .u.clock_pointer.rotation_sina = 0.0f, .u.clock_pointer.center_x = 120, .u.clock_pointer.center_y = 120, .u.clock_pointer.source_pic_width = 240, .u.clock_pointer.source_pic_height = 240
    },
    [5]     = {
        .x = 110,  .y = 32,   .width = 10,     .hight = 90,      .addr = WATCH_MINUTE_HAND_BIN,    .widget_id_type = ICON_CLOCK_POINTER, \
        .u.clock_pointer.rotation_cosa = 1.0f, .u.clock_pointer.rotation_sina = 0.0f, .u.clock_pointer.center_x = 120, .u.clock_pointer.center_y = 120, .u.clock_pointer.source_pic_width = 240, .u.clock_pointer.source_pic_height = 240
    },
    [6]     = {
        .x = 110,  .y = 22,   .width = 10,     .hight = 101,      .addr = WATCH_SECOND_HAND_BIN,    .widget_id_type = ICON_CLOCK_POINTER, \
        .u.clock_pointer.rotation_cosa = 1.0f, .u.clock_pointer.rotation_sina = 0.0f, .u.clock_pointer.center_x = 120, .u.clock_pointer.center_y = 120, .u.clock_pointer.source_pic_width = 240, .u.clock_pointer.source_pic_height = 240
    },
};

UI_MenuTypeDef TestPSRAM1Menu =
{
    .name = (const char *)"TestPSRAM1Menu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)icons,
    .current_max_icon = WATCHFACE_PAGE_NUM,
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  TestPSRAM1Menu.name, strlen(TestPSRAM1Menu.name)))
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
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_prepare_frame_buffer(&ClockSpecialMenu, NULL, &FrameBufferOrigin, true);
        }
    }
    else if ((timer_id == 3) && (display_timer != NULL))
    {
        UI_MenuTypeDef *p_menu = rtl_gui_menu_real_slide();
        if (p_menu != NULL)
        {
            pCurrentMenu->MenuSub = p_menu;
            pCurrentMenu->destructor_func();
            pCurrentMenu = pCurrentMenu->MenuSub;
            pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
            os_timer_stop(&page_switch_timer);
        }
    }
}


static void ConstructorCallbackFun(uint32_t type, void *pValue)
{
    wristband_display_power_on();
    wristband_display_enable();
    pCurrentMenu->dynamicFlag = true;

    psram_frame_buffer_init();
    rtl_gui_prepare_frame_buffer(&TestPSRAM1Menu, NULL, &FrameBufferOrigin, true);
    rtl_gui_update_detal(0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 5000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 1000, true, rtl_gui_timer_callback);
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
}
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }

    return_menu->current_max_icon = WatchFaceAWMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, WatchFaceAWMenu.pWidgetList,
           WatchFaceAWMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        uint16_t seconds = get_system_clock_second();
        float angle_hour = (RtkWristbandSys.Global_Time.hour % 12) * MY_PI / 6 +
                           RtkWristbandSys.Global_Time.minutes * MY_PI / 360;
        float angle_min  = RtkWristbandSys.Global_Time.minutes * MY_PI / 30 + seconds * MY_PI / 1800;
        float angle_sec  = seconds * MY_PI / 30;
        float angle[3] = {angle_hour, angle_min, angle_sec};
        int16_t clock_length[9] = {HOUR_HEIGHT, HOUR_WIDTH, HOUR_CENTER_HEIGHT, MIN_HEIGHT, MIN_WIDTH, MIN_CENTER_HEIGHT,
                                   SEC_HEIGHT, SEC_WIDTH, SEC_CENTER_HEIGHT
                                  };
        float cos_sin[6];
        cos_sin[0] = arm_cos_f32(angle_hour);
        cos_sin[1] = arm_sin_f32(angle_hour);
        cos_sin[2] = arm_cos_f32(angle_min);
        cos_sin[3] = arm_sin_f32(angle_min);
        cos_sin[4] = arm_cos_f32(angle_sec);
        cos_sin[5] = arm_sin_f32(angle_sec);

        return_menu->pWidgetList[4].u.clock_pointer.rotation_cosa = cos_sin[0];
        return_menu->pWidgetList[4].u.clock_pointer.rotation_sina = cos_sin[1];
        return_menu->pWidgetList[5].u.clock_pointer.rotation_cosa = cos_sin[2];
        return_menu->pWidgetList[5].u.clock_pointer.rotation_sina = cos_sin[3];
        return_menu->pWidgetList[6].u.clock_pointer.rotation_cosa = cos_sin[4];
        return_menu->pWidgetList[6].u.clock_pointer.rotation_sina = cos_sin[5];

        for (uint8_t i = 0; i < 3; i++)
        {
            if (angle[i] >= MY_PI / 2 && angle[i] < 0)
            {
                return_menu->pWidgetList[4 + i].width = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i * 2 + 1] +
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2 + 0]) + 1;
                return_menu->pWidgetList[4 + i].hight = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2 + 0] -
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
                return_menu->pWidgetList[4 + i].x = return_menu->pWidgetList[4 + i].u.clock_pointer.center_x - ((
                                                        int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                   + 2]) * cos_sin[i * 2 + 1] + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 0]) + 1);
                return_menu->pWidgetList[4 + i].y = return_menu->pWidgetList[4 + i].u.clock_pointer.center_y - ((
                                                        int16_t)(clock_length[i * 3 + 2] * cos_sin[i * 2 + 0]
                                                                 - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
            }
            else if (angle[i] >= MY_PI && angle[i] < MY_PI / 2)
            {
                return_menu->pWidgetList[4 + i].width = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i * 2 + 1] -
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2]) + 1;
                return_menu->pWidgetList[4 + i].hight = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i * 2] -
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
                return_menu->pWidgetList[4 + i].x = return_menu->pWidgetList[4 + i].u.clock_pointer.center_x - ((
                                                        int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                   + 2]) * cos_sin[i * 2 + 1] - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2]) + 1);
                return_menu->pWidgetList[4 + i].y = return_menu->pWidgetList[4 + i].u.clock_pointer.center_y - ((
                                                        int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                   + 2]) * cos_sin[i * 2] - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
            }
            else if (angle[i] >= 3 * MY_PI / 2 && angle[i] < MY_PI)
            {
                return_menu->pWidgetList[4 + i].width = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2 + 1] -
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2]) + 1;
                return_menu->pWidgetList[4 + i].hight = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i * 2] +
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
                return_menu->pWidgetList[4 + i].x = return_menu->pWidgetList[4 + i].u.clock_pointer.center_x - ((
                                                        int16_t)((clock_length[i * 3 + 2]) * cos_sin[i * 2 + 1]
                                                                 - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2]) + 1);
                return_menu->pWidgetList[4 + i].y = return_menu->pWidgetList[4 + i].u.clock_pointer.center_y - ((
                                                        int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                   + 2]) * cos_sin[i * 2] + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
            }
            else
            {
                return_menu->pWidgetList[4 + i].width = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2 + 1] +
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2 + 0]) + 1;
                return_menu->pWidgetList[4 + i].hight = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2 + 0] +
                                                                  clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
                return_menu->pWidgetList[4 + i].x = return_menu->pWidgetList[4 + i].u.clock_pointer.center_x - ((
                                                        int16_t)(clock_length[i * 3 + 2] * cos_sin[i * 2 + 1]
                                                                 + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 0]) + 1);
                return_menu->pWidgetList[4 + i].y = return_menu->pWidgetList[4 + i].u.clock_pointer.center_y - ((
                                                        int16_t)(clock_length[i * 3 + 2] * cos_sin[i * 2 + 0]
                                                                 + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
            }
        }
    }
#if 1
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        static char week_day_string[3] = {'\0'};
        memset(week_day_string, '\0', sizeof(week_day_string));
        uint8_t week_day = RtkWristbandSys.Global_Time.day;
        sprintf(week_day_string, "%02d", week_day);
        return_menu->pWidgetList[2].u.num_string_addr = (uint32_t)week_day_string;

        DAY_OF_WEEK day_of_week;
        day_of_week = get_day_of_week(RtkWristbandSys.SecondCountRTC); // day of week
        return_menu->pWidgetList[3].addr = COMMON_WEEK_WHITE_BIN + \
                                           day_of_week * return_menu->pWidgetList[3].hight * return_menu->pWidgetList[3].width * 2;
    }
    else
    {
        static char week_day_string[3] = "00\0";
        return_menu->pWidgetList[2].u.num_string_addr = (uint32_t)week_day_string;
    }
    if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
    {
        return_menu->pWidgetList[1].addr = COMMON_CONNECT_BIN;
    }
#endif
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
    }
    else if (type == LONG_BUTTON_0)
    {
    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_right = &TestPSRAM0Menu;
    UI_MenuTypeDef *ui_local_left = &TestPSRAM0Menu;
    UI_MenuTypeDef *ui_local_cur = &TestPSRAM1Menu;
    UI_MenuTypeDef *ui_local_down = &TestPSRAM0Menu;
    UI_MenuTypeDef *ui_local_up = &TestPSRAM0Menu;
    uint32_t timer_state;
    os_timer_state_get(&page_switch_timer, &timer_state);
    if (timer_state)
    {
        return;
    }

    os_timer_restart(&display_timer, 50000);
    os_timer_restart(&dynamic_timer, 500);

    if (info->type == TOUCH_HOLD)
    {
        if (info->deltaX >= 0)
        {
            rtl_gui_prepare_frame_buffer(ui_local_left, NULL, &FrameBufferLeft, true);
        }
        else
        {
            rtl_gui_prepare_frame_buffer(ui_local_right, NULL, &FrameBufferRight, true);
        }
        rtl_gui_update_detal(info->deltaX, 0);
    }
    else if (info->type == TOUCH_HOLD_Y)
    {
        if (info->deltaY >= 0)
        {
            rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        }
        else
        {
            rtl_gui_prepare_frame_buffer(ui_local_down, NULL, &FrameBufferDown, true);
        }
        rtl_gui_update_detal(0, info->deltaY);
    }
    else if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        if (info->deltaX >= 0)
        {
            rtl_gui_prepare_frame_buffer(ui_local_left, NULL, &FrameBufferLeft, true);
        }
        else
        {
            rtl_gui_prepare_frame_buffer(ui_local_right, NULL, &FrameBufferRight, true);
        }
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 0,
                           0, 0);
    }
    else if (info->type == TOUCH_ORIGIN_FROM_Y)
    {
        if (info->deltaY >= 0)
        {
            rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        }
        else
        {
            rtl_gui_prepare_frame_buffer(ui_local_down, NULL, &FrameBufferDown, true);
        }
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, 0, 0,
                           info->deltaY, 0);
    }
    else if (info->type == TOUCH_RIGHT_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_left, NULL, &FrameBufferLeft, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 240,
                           0, 0);
    }
    else if (info->type == TOUCH_LEFT_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_right, NULL, &FrameBufferRight, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX,
                           -240, 0, 0);
    }
    else if (info->type == TOUCH_UP_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           240);
    }
    else if (info->type == TOUCH_DOWN_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_down, NULL, &FrameBufferDown, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           -240);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_update_detal(0, 0);
    }
}

