
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
#include "module_vibrate_motor.h"
#include "wristband_global_data.h"
#include "ancs.h"
#include "ancs_client.h"
#include "wristband_picture_res.h"
#include "wristband_gui.h"
#include "hub_task.h"
#include "gui_core.h"
#include "hub_interaction.h"
#include "hub_touch.h"
#include "module_touch.h"

bool allowed_touch_enter_dlps = true;

static const T_IO_MODULE_CBS touch_cbs =
{
    wristband_touch_driver_init,
    wristband_touch_enter_dlps,
    wristband_touch_exit_dlps,
    wristband_touch_allowed_enter_dlps_check,
    wristband_touch_system_wakeup_dlps_check,
};

void touch_add_hub_task(void)
{
    wristband_touch_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&touch_cbs;
}

void wristband_touch_driver_init(void)
{
    //call module init
    cst0x6_driver_init();
}

void wristband_touch_enter_dlps(void)
{
    //call module init
    touch_gesture_enter_dlps();
}
void wristband_touch_exit_dlps(void)
{
    //call module init
    touch_gesture_exit_dlps();
}

bool wristband_touch_allowed_enter_dlps_check(void)
{
    return allowed_touch_enter_dlps;
}

bool wristband_touch_system_wakeup_dlps_check(void)
{
    if (System_WakeUpInterruptValue(TOUCH_INT) == SET)
    {
        APP_PRINT_INFO0("Touch WakeUp Int");
        Pad_ClearWakeupINTPendingBit(TOUCH_INT);
        allowed_touch_enter_dlps = false;
        return true;
    }
    return false;
}

void touch_event_handler(T_IO_MSG msg)
{
    switch (msg.subtype)
    {
    case TOUCH_MSG_INT:
        {
            cst0x6_read_key_value();
            break;
        }
    case TOUCH_MSG_TIMEOUT:
        {
            touch_gesture_process_timeout();
            break;
        }
    default:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            break;
        }
    }
}


