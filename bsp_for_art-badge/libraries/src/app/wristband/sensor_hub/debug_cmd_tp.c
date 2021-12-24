
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
#include "hub_task.h"
#include "hub_uart_debug.h"
#include "module_uart_debug.h"
#include "debug_cmd_tp.h"
#include "gui_core.h"
#include "hub_display.h"
#include "gui_common_menu.h"
#include "stdlib.h"
#include "module_touch.h"
#include "rtl876x_rtc.h"
#include "rtl8763bf_task.h"


static void *sim_tp_timer   = NULL;
static void *sim_tp_release_timer   = NULL;
//static RtkTouchInfo_t info;
//static bool hold_flag = false;
static TOUCH_DATA cur_point;

void sim_tp_timer_callback(void *pxTimer)
{
    extern bool lcd_screen_updating_flag;
    if (lcd_screen_updating_flag == false)
    {
        T_IO_MSG msg;
        msg.type = HUB_MSG_UART_DEBUG;
        msg.subtype = UART_DEBUG_SIM_TP_INT;
        if (send_msg_to_ui_task(&msg, __LINE__) == false)
        {
            APP_PRINT_ERROR0("send uart cmd to io queue fail !\n");
        }
    }
}

void sim_tp_release_callback(void *pxTimer)
{
    T_IO_MSG msg;
    msg.type = HUB_MSG_UART_DEBUG;
    msg.subtype = UART_DEBUG_SIM_TP_RELEASE;
    if (send_msg_to_ui_task(&msg, __LINE__) == false)
    {
        APP_PRINT_ERROR0("send uart cmd to io queue fail !\n");
    }
}

static void sim_tp_callback(RtkTouchInfo_t *info)
{
    pCurrentMenu->touch_func(info);
}


void sim_tp_init(void)
{
    os_timer_create(&sim_tp_timer, "sim tp timer", 1, 20, true, sim_tp_timer_callback);
    os_timer_create(&sim_tp_release_timer, "sim tp release", 1, 20, false, sim_tp_release_callback);
    RtkCallbackReg((touch_algorithm_func)sim_tp_callback);
}

void sim_tp_release(void)
{
    cur_point.t++;

    RtkTouchRawData_t In[1];
    In->x = cur_point.x;
    In->y = cur_point.y;
    In->counter = cur_point.t;
    In->pressFlag = cur_point.is_press;
    In->rtc_counter = RTC_GetCounter();

    RtkTouchProcess(In);
    cur_point.t = 0;
}

void sim_tp_interrupt(void)
{
    //pCurrentMenu->touch_func(&info);
    cur_point.t++;

    RtkTouchRawData_t In[1];
    In->x = cur_point.x;
    In->y = cur_point.y;
    In->counter = cur_point.t;
    In->pressFlag = cur_point.is_press;
    In->rtc_counter = RTC_GetCounter();

    RtkTouchProcess(In);
}

void sim_tp_handle_package(uint8_t *buf, uint32_t len)
{
    uint8_t local_buf[32] = {0};
    memcpy(local_buf, buf, len);
    //APP_PRINT_INFO1("sim_tp_handle_package = %s", TRACE_STRING(buf));

    if (memcmp("P", buf, strlen("P")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] PRESS");
        cur_point.x = 0;
        cur_point.y = 0;
        cur_point.t = 0;
        cur_point.is_press = true;
        os_timer_start(&sim_tp_timer);
    }
    else if (memcmp("Rel", buf, strlen("Rel")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] RELEASE");
        cur_point.x = 0;
        cur_point.y = 0;
        cur_point.is_press = false;
        os_timer_stop(&sim_tp_timer);
        os_timer_start(&sim_tp_release_timer);
    }
    else if (memcmp("UP", buf, strlen("UP")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] UP");
        APP_PRINT_INFO0("tp_vol_up test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_VOL_UP;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("DOWN", buf, strlen("DOWN")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] DOWN");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_VOL_DOWN;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("LEFT", buf, strlen("LEFT")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] LEFT");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_AV_FWD;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("RIGHT", buf, strlen("RIGHT")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] RIGHT");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_AV_BWD;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("VALX=", buf, strlen("VALX=")) == 0)
    {
        uint8_t num_string[10] = {0};
        memcpy(num_string, local_buf + strlen("VALX="), len - strlen("VALX="));

        cur_point.x = atoi((const char *)num_string);
        APP_PRINT_INFO1("[DEBUG CMD] VALX= %d", cur_point.x);
    }
    else if (memcmp("VALY=", buf, strlen("VALY=")) == 0)
    {
        uint8_t num_string[10] = {0};
        memcpy(num_string, local_buf + strlen("VALY="), len - strlen("VALY="));

        cur_point.y = atoi((const char *)num_string);
        APP_PRINT_INFO1("[DEBUG CMD] VALY= %d", cur_point.y);
    }
    else if (memcmp("F1", buf, strlen("F1")) == 0)
    {
        APP_PRINT_INFO0("tp_start_and_pause test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_AV_PLAY_PAUSE;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("F2", buf, strlen("F2")) == 0)
    {
        T_HCI_MSG msg;
        msg.subtype = BBPRO_HCI_SET_PLAY_MODE;
        msg.u.param = BOM_PLAY_MODE_LOCAL_PLAYBACK;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("F3", buf, strlen("F3")) == 0)
    {
        T_HCI_MSG msg;
        msg.subtype = BBPRO_HCI_SET_PLAY_MODE;
        msg.u.param = BOM_PLAY_MODE_A2DP_SINK;
        bbpro_hci_msg_queue_send(&msg);
        msg.subtype = BBPRO_HCI_SEND_ENTER_PAIRING_MODE;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("F4", buf, strlen("F4")) == 0)
    {
        T_HCI_MSG msg;
        msg.subtype = BBPRO_HCI_SET_PLAY_MODE;
        msg.u.param = BOM_PLAY_MODE_A2DP_SOURCE;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (memcmp("F5", buf, strlen("F5")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] short button");
        if ((wristbandMenuStatus.DisplayActiveFlag == false) || (pCurrentMenu == NULL))
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
        }
        else
        {
            pCurrentMenu->button_func(SHORT_BUTTON_0);
        }
    }
    else if (memcmp("F6", buf, strlen("F6")) == 0)
    {
        APP_PRINT_INFO0("[DEBUG CMD] long button");
        if (pCurrentMenu != NULL)
        {
            /* if OLED is not active, can not ack long button*/
            if (wristbandMenuStatus.DisplayActiveFlag == false)
            {
                return;
            }
            pCurrentMenu->button_func(LONG_BUTTON_0);
        }
    }
    else
    {
        APP_PRINT_INFO0("[DEBUG CMD] no this CMD;");

    }
}


