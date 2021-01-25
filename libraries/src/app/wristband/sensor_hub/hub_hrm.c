/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     wristband_hrm.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "string.h"
#include "trace.h"
#include "app_task.h"
#include "os_timer.h"
#include "hub_hrm.h"
#include "hub_task.h"
#include "module_heart_vc31.h"

bool allowed_hrm_enter_dlps = true;

static const T_IO_MODULE_CBS hrm_sensor_cbs =
{
    wristband_hrm_sensor_driver_init,
    wristband_hrm_sensor_enter_dlps,
    wristband_hrm_sensor_exit_dlps,
    wristband_hrm_sensor_allowed_enter_dlps_check,
};

void hrm_sensor_add_hub_task(void)
{
    wristband_hrm_sensor_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&hrm_sensor_cbs;
}

void wristband_hrm_sensor_driver_init(void)
{
    vc31_driver_init();

}

void wristband_hrm_sensor_enter_dlps(void)
{
    vc31_enter_dlps();
}
void wristband_hrm_sensor_exit_dlps(void)
{
    vc31_exit_dlps();
}

bool wristband_hrm_sensor_allowed_enter_dlps_check(void)
{
    return allowed_hrm_enter_dlps;
}

void wristband_hrm_enable(void)
{

}


void wristband_hrm_disable(void)
{

}


void wristband_hrm_interrupt_enable(void)
{

}

void hrs_event_handler(T_IO_MSG msg)
{
    switch (msg.subtype)
    {
    case HRM_SENSOR_MSG_WAKEUP:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            vc31_process();
            break;
        }
    default:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            break;
        }
    }
}



