/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     wristband_gsensor.c
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
#include "os_timer.h"
#include "module_gsensor_stk8321.h"
#include "hub_gsensor.h"
#include "hub_task.h"
#include "module_algorithm_align.h"
#include "wristband_global_data.h"

bool allowed_gsensor_enter_dlps = true;
static bool gsensor_has_installed = false;

static const T_IO_MODULE_CBS gsensor_cbs =
{
    wristband_gsensor_driver_init,
    wristband_gsensor_enter_dlps,
    wristband_gsensor_exit_dlps,
    wristband_gsensor_allowed_enter_dlps_check,
};

void gsensor_add_hub_task(void)
{
    wristband_gsensor_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&gsensor_cbs;
}

void wristband_gsensor_driver_init(void)
{
    //call module init
    stk8321_driver_init();
}

void wristband_gsensor_enter_dlps(void)
{
    //call module init
    stk8321_enter_dlps();
}
void wristband_gsensor_exit_dlps(void)
{
    //call module init
    stk8321_exit_dlps();
}
bool wristband_gsensor_allowed_enter_dlps_check(void)
{
    return allowed_gsensor_enter_dlps;
}

bool wristband_gsensor_get_fifo_data(uint8_t *len, AxesRaw_t *buff)
{
    if (gsensor_has_installed == false)
    {
        return false;
    }
    return stk8321_get_fifo_data(len, buff);
}



uint8_t wristband_gsensor_get_fifo_length(void)
{
    if (gsensor_has_installed == false)
    {
        return false;
    }
    return stk8321_get_fifo_length();
}

static void wristband_gsensor_stop(void)
{
    if (gsensor_has_installed == false)
    {
        return;
    }
    stk8321_enable();
}
static void wristband_gsensor_start(void)
{
    if (gsensor_has_installed == false)
    {
        return;
    }
    stk8321_disable();
}

void wristband_gsensor_message_handle(void)
{
    AxesRaw_t accData[32];
    uint8_t fifo_data_count;

    if ((RtkWristbandSys.flag_field.bond_state == false) || RtkWristbandSys.flag_field.low_power_flag ||
        RtkWristbandSys.flag_field.power_off_flag)
    {
        return;
    }

    if (true == wristband_gsensor_get_fifo_data(&fifo_data_count, accData))
    {
        for (uint8_t i = 0; i < fifo_data_count; i++)
        {
            if (RtkWristbandSys.flag_field.algorithm_started  && (RtkWristbandSys.charger_status == NoCharge))
            {
                rtk_gsa_fsm((int16 *)&accData[i]);
            }
        }
    }
}

void gsensor_event_handler(T_IO_MSG msg)
{
    uint8_t value = msg.subtype;
    switch (value)
    {
    case GSENSOR_MSG_WAKEUP:
        {
            wristband_gsensor_message_handle();
            break;
        }
    case GSENSOR_MSG_START:
        {
            wristband_gsensor_start();
            break;
        }
    case GSENSOR_MSG_STOP:
        {
            wristband_gsensor_stop();
            break;
        }

    default:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            break;
        }
    }
}
