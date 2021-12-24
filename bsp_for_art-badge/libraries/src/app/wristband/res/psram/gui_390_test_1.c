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
#include "hub_battery.h"
#include <string.h>
#include <stdarg.h>

#include "hub_interaction.h"
#include "wristband_gui.h"
#include "app_msg.h"
#include "app_task.h"
#include "arm_math.h"
#include "rtl8763bf_task.h"
#include "touch_algorithm.h"
#include "stdio.h"
#include "gui_psram.h"
#include "gui_core.h"
#include "gui_slide.h"
#include "hub_task.h"

#define DISPLAY_INTERVAL    1000000
#define DYNAMIC_INTERVAL    100

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;

//RtkWristbandSys.Global_Time.hour = 23;
//RtkWristbandSys.Global_Time.minutes = 17;
//RtkWristbandSys.Global_Time.seconds = 45;
uint32_t demo_second = 23 * 60 * 60 + 17 * 60 + 45;



static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);

#define MY_PI   (-3.1415926f)
#define HOUR_HEIGHT            102
#define HOUR_WIDTH             26
#define HOUR_CENTER_HEIGHT     89

#define MIN_HEIGHT             154
#define MIN_WIDTH              22
#define MIN_CENTER_HEIGHT      141

#define SEC_HEIGHT             208
#define SEC_WIDTH              16
#define SEC_CENTER_HEIGHT      144

#define     TEST390_2___BIN    (0x1100000 + 0)
#define     TEST390_2_BATTERY_100_BIN    (TEST390_2___BIN + 520)
#define     TEST390_2_BG_BIN    (TEST390_2_BATTERY_100_BIN + 918)
#define     TEST390_2_LOGO_STEP_PICTURE_BIN    (TEST390_2_BG_BIN + 304200)
#define     TEST390_2_LOGO_STEP_STRING_BIN    (TEST390_2_LOGO_STEP_PICTURE_BIN + 882)
#define     TEST390_2_NUM_0_BIN    (TEST390_2_LOGO_STEP_STRING_BIN + 1470)
#define     TEST390_2_NUM_1_BIN    (TEST390_2_NUM_0_BIN + 520)
#define     TEST390_2_NUM_2_BIN    (TEST390_2_NUM_1_BIN + 520)
#define     TEST390_2_NUM_3_BIN    (TEST390_2_NUM_2_BIN + 520)
#define     TEST390_2_NUM_4_BIN    (TEST390_2_NUM_3_BIN + 520)
#define     TEST390_2_NUM_5_BIN    (TEST390_2_NUM_4_BIN + 520)
#define     TEST390_2_NUM_6_BIN    (TEST390_2_NUM_5_BIN + 520)
#define     TEST390_2_NUM_7_BIN    (TEST390_2_NUM_6_BIN + 520)
#define     TEST390_2_NUM_8_BIN    (TEST390_2_NUM_7_BIN + 520)
#define     TEST390_2_NUM_9_BIN    (TEST390_2_NUM_8_BIN + 520)
#define     TEST390_2_POINT_HOUR_BIN    (TEST390_2_NUM_9_BIN + 520)
#define     TEST390_2_POINT_MIN_BIN    (TEST390_2_POINT_HOUR_BIN + 5304)
#define     TEST390_2_POINT_SEC_BIN    (TEST390_2_POINT_MIN_BIN + 6776)
#define     TEST390_2_WEEK_1_BIN    (TEST390_2_POINT_SEC_BIN + 6656)
#define     TEST390_2_WEEK_2_BIN    (TEST390_2_WEEK_1_BIN + 1462)
#define     TEST390_2_WEEK_3_BIN    (TEST390_2_WEEK_2_BIN + 1462)
#define     TEST390_2_WEEK_4_BIN    (TEST390_2_WEEK_3_BIN + 1462)
#define     TEST390_2_WEEK_5_BIN    (TEST390_2_WEEK_4_BIN + 1462)
#define     TEST390_2_WEEK_6_BIN    (TEST390_2_WEEK_5_BIN + 1462)
#define     TEST390_2_WEEK_7_BIN    (TEST390_2_WEEK_6_BIN + 1462)

const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = LCD_WIDTH, .hight = LCD_HIGHT,     .addr = TEST390_2_BG_BIN,   .widget_id_type = ICON_BACKGROUND,
    },

    [1]     = {
        .x = 123,   .y = 83,    .width = 43, .hight = 17,     .addr = TEST390_2_WEEK_1_BIN,   .widget_id_type = ICON_WIDGET,
    },

    //for month
    [2]     = {
        .x = 170,   .y = 80,    .width = 13, .hight = 20,     .addr = TEST390_2_NUM_0_BIN,   .widget_id_type = ICON_NUM,
    },
    //for -
    [3]     = {
        .x = 170,   .y = 80,    .width = 13, .hight = 20,     .addr = TEST390_2___BIN,   .widget_id_type = ICON_WIDGET,
    },
    //for day
    [4]     = {
        .x = 170,   .y = 80,    .width = 13, .hight = 20,     .addr = TEST390_2_NUM_0_BIN,   .widget_id_type = ICON_NUM,
    },
    //for battery
    [5]     = {
        .x = 239,   .y = 82,    .width = 27, .hight = 17,     .addr = TEST390_2_BATTERY_100_BIN,   .widget_id_type = ICON_WIDGET,
    },


    //for hour
    [6]     = {
        .addr = TEST390_2_POINT_HOUR_BIN,   .widget_id_type = ICON_CLOCK_POINTER, \
        .u.clock_pointer.rotation_cosa = 1.0f, \
        .u.clock_pointer.rotation_sina = 0.0f, \
        .u.clock_pointer.center_x = LCD_WIDTH / 2, \
        .u.clock_pointer.center_y = LCD_HIGHT / 2, \
        .u.clock_pointer.source_pic_width = HOUR_WIDTH, \
        .u.clock_pointer.source_pic_height = HOUR_HEIGHT, \
        .u.clock_pointer.source_pic_center_height = HOUR_CENTER_HEIGHT,
    },
    //for min
    [7]     = {
        .addr = TEST390_2_POINT_MIN_BIN,    .widget_id_type = ICON_CLOCK_POINTER, \
        .u.clock_pointer.rotation_cosa = 1.0f, \
        .u.clock_pointer.rotation_sina = 0.0f, \
        .u.clock_pointer.center_x = LCD_WIDTH / 2, \
        .u.clock_pointer.center_y = LCD_HIGHT / 2, \
        .u.clock_pointer.source_pic_width = MIN_WIDTH, \
        .u.clock_pointer.source_pic_height = MIN_HEIGHT, \
        .u.clock_pointer.source_pic_center_height = MIN_CENTER_HEIGHT,
    },
    //for sec
    [8]     = {
        .addr = TEST390_2_POINT_SEC_BIN,    .widget_id_type = ICON_CLOCK_POINTER, \
        .u.clock_pointer.rotation_cosa = 1.0f, \
        .u.clock_pointer.rotation_sina = 0.0f, \
        .u.clock_pointer.center_x = LCD_WIDTH / 2, \
        .u.clock_pointer.center_y = LCD_HIGHT / 2, \
        .u.clock_pointer.source_pic_width = SEC_WIDTH, \
        .u.clock_pointer.source_pic_height = SEC_HEIGHT, \
        .u.clock_pointer.source_pic_center_height = SEC_CENTER_HEIGHT,
    },

};

/* change Here for UI */
UI_MenuTypeDef Test390Menu1 =
{
    /* change Here for UI */
    .name = (const char *)"Test390Menu1",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)icon,
    .current_max_icon = sizeof(icon) / sizeof(UI_WidgetTypeDef),
};

static UI_MenuTypeDef *pLocalMenu = &Test390Menu1;



static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  pLocalMenu->name, strlen(pLocalMenu->name)))
    {
        return;
    }

    if ((timer_id == 1) && (display_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, BLACK);
        wristband_display_power_off();
        pCurrentMenu->destructor_func();
    }
    else if ((timer_id == 2) && (dynamic_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        demo_second++;
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        os_timer_start(&dynamic_timer);
        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_prepare_frame_buffer(pLocalMenu, NULL, &FrameBufferOrigin, true);
            rtl_gui_update_detal(0, 0);
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
    rtl_gui_prepare_frame_buffer(pLocalMenu, NULL, &FrameBufferOrigin, true);
    rtl_gui_update_detal(0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, DISPLAY_INTERVAL, false,
                    rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, DYNAMIC_INTERVAL, false,
                    rtl_gui_timer_callback);
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
    APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
}

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }

    return_menu->current_max_icon = pLocalMenu->current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, pLocalMenu->pWidgetList,
           pLocalMenu->current_max_icon * sizeof(UI_WidgetTypeDef));


    /**for piont**/
    convert_to_utc_time(demo_second);
#if 1
    //RtkWristbandSys.Global_Time.hour = 23;
    //RtkWristbandSys.Global_Time.minutes = 17;
    //RtkWristbandSys.Global_Time.seconds = 45;
#endif
    float angle_hour = (RtkWristbandSys.Global_Time.hour % 12) * MY_PI / 6 +
                       RtkWristbandSys.Global_Time.minutes * MY_PI / 360;
    float angle_min  = RtkWristbandSys.Global_Time.minutes * MY_PI / 30 +
                       RtkWristbandSys.Global_Time.seconds * MY_PI / 1800;
    float angle_sec  = RtkWristbandSys.Global_Time.seconds * MY_PI / 30;
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

#define POINT_OFFSET    6
    return_menu->pWidgetList[6 + 0].u.clock_pointer.rotation_cosa = cos_sin[0];
    return_menu->pWidgetList[6 + 0].u.clock_pointer.rotation_sina = cos_sin[1];
    return_menu->pWidgetList[6 + 1].u.clock_pointer.rotation_cosa = cos_sin[2];
    return_menu->pWidgetList[6 + 1].u.clock_pointer.rotation_sina = cos_sin[3];
    return_menu->pWidgetList[6 + 2].u.clock_pointer.rotation_cosa = cos_sin[4];
    return_menu->pWidgetList[6 + 2].u.clock_pointer.rotation_sina = cos_sin[5];


    for (uint8_t i = 0; i < 3; i++)
    {
        if (angle[i] >= MY_PI / 2 && angle[i] < 0)
        {
            return_menu->pWidgetList[POINT_OFFSET + i].width = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i *
                                                                         2 + 1] +
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2 + 0]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].hight = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2
                                                                         + 0] -
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].x = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_x - ((
                                                                                            int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                                                    + 2]) * cos_sin[i * 2 + 1] + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 0]) + 1);
            return_menu->pWidgetList[POINT_OFFSET + i].y = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_y - ((
                                                                                            int16_t)(clock_length[i * 3 + 2] * cos_sin[i * 2 + 0]
                                                                                                    - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
        }
        else if (angle[i] >= MY_PI && angle[i] < MY_PI / 2)
        {
            return_menu->pWidgetList[POINT_OFFSET + i].width = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i *
                                                                         2 + 1] -
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].hight = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i *
                                                                         2] -
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].x = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_x - ((
                                                                                            int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                                                    + 2]) * cos_sin[i * 2 + 1] - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2]) + 1);
            return_menu->pWidgetList[POINT_OFFSET + i].y = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_y - ((
                                                                                            int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                                                    + 2]) * cos_sin[i * 2] - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
        }
        else if (angle[i] >= 3 * MY_PI / 2 && angle[i] < MY_PI)
        {
            return_menu->pWidgetList[POINT_OFFSET + i].width = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2
                                                                         + 1] -
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].hight = (int16_t)(-clock_length[i * 3 + 0] * cos_sin[i *
                                                                         2] +
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].x = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_x - ((
                                                                                            int16_t)((clock_length[i * 3 + 2]) * cos_sin[i * 2 + 1]
                                                                                                    - (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2]) + 1);
            return_menu->pWidgetList[POINT_OFFSET + i].y = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_y - ((
                                                                                            int16_t)(-(clock_length[i * 3 + 0] - clock_length[i * 3
                                                                                                    + 2]) * cos_sin[i * 2] + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
        }
        else
        {
            return_menu->pWidgetList[POINT_OFFSET + i].width = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2
                                                                         + 1] +
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2 + 0]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].hight = (int16_t)(clock_length[i * 3 + 0] * cos_sin[i * 2
                                                                         + 0] +
                                                                         clock_length[i * 3 + 1] * cos_sin[i * 2 + 1]) + 1;
            return_menu->pWidgetList[POINT_OFFSET + i].x = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_x - ((
                                                                                            int16_t)(clock_length[i * 3 + 2] * cos_sin[i * 2 + 1]
                                                                                                    + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 0]) + 1);
            return_menu->pWidgetList[POINT_OFFSET + i].y = return_menu->pWidgetList[POINT_OFFSET +
                                                                                    i].u.clock_pointer.center_y - ((
                                                                                            int16_t)(clock_length[i * 3 + 2] * cos_sin[i * 2 + 0]
                                                                                                    + (clock_length[i * 3 + 1] / 2) * cos_sin[i * 2 + 1]) + 1);
        }
    }
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &Test390Menu0;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_right = &Test390Menu0;
    UI_MenuTypeDef *ui_local_left = &Test390Menu0;
    UI_MenuTypeDef *ui_local_cur = pLocalMenu;
    UI_MenuTypeDef *ui_local_down = &Test390Menu0;
    UI_MenuTypeDef *ui_local_up = &Test390Menu0;
    uint32_t timer_state;
    os_timer_state_get(&page_switch_timer, &timer_state);
    if (timer_state)
    {
        return;
    }

    os_timer_restart(&display_timer, DISPLAY_INTERVAL);
    os_timer_restart(&dynamic_timer, DYNAMIC_INTERVAL);

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
        os_timer_stop(&dynamic_timer);
        rtl_gui_prepare_frame_buffer(ui_local_left, NULL, &FrameBufferLeft, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX,
                           LCD_WIDTH,
                           0, 0);
    }
    else if (info->type == TOUCH_LEFT_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_right, NULL, &FrameBufferRight, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX,
                           -LCD_WIDTH, 0, 0);
    }
    else if (info->type == TOUCH_UP_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           LCD_HIGHT);
    }
    else if (info->type == TOUCH_DOWN_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_down, NULL, &FrameBufferDown, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           -LCD_HIGHT);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_update_detal(0, 0);
    }

}

