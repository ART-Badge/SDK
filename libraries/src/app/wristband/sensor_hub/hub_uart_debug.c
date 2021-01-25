
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
#include "gui_core.h"
#include "hub_display.h"
#include "gui_common_menu.h"
#include "debug_cmd_tp.h"

bool allowed_uart_debug_enter_dlps = true;
static bool uart_debug_has_installed = false;


static const T_IO_MODULE_CBS uart_debug_cbs =
{
    wristband_uart_debug_driver_init,
    wristband_uart_debug_enter_dlps,
    wristband_uart_debug_exit_dlps,
    wristband_uart_debug_allowed_enter_dlps_check,
    wristband_uart_debug_system_wakeup_dlps_check,
};

void uart_debug_add_hub_task(void)
{
    wristband_uart_debug_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&uart_debug_cbs;
    uart_debug_has_installed = true;
}

void wristband_uart_debug_driver_init(void)
{
    uart_debug_init();
    sim_tp_init();
}

void wristband_uart_debug_enter_dlps(void)
{
    //call module init
    uart_debug_enter_dlps();
}

void wristband_uart_debug_exit_dlps(void)
{
    //call module init
    uart_debug_exit_dlps();
}

bool wristband_uart_debug_allowed_enter_dlps_check(void)
{
    return allowed_uart_debug_enter_dlps;
}

bool wristband_uart_debug_system_wakeup_dlps_check(void)
{
    if (System_WakeUpInterruptValue(UART_DEBUG_RX) == SET)
    {
        APP_PRINT_INFO0("UART DEBUG WakeUp Int");
        Pad_ClearWakeupINTPendingBit(UART_DEBUG_RX);
        allowed_uart_debug_enter_dlps = false;
        return true;
    }
    return false;
}

void ATCmdHandler(uint8_t *buf, uint32_t len);

/**
* @brief   AT cmd parse.
* @param   void
* @return  void
*/
void AtCmdParse(void)
{
    APP_PRINT_INFO1("ENTER cmd parse LINE = %d", __LINE__);
    if (raw_package.write_index < AT_CMD_MIN_LENGTH)
    {
        uart_debug_buf_reset();
        return;
    }

    if (((raw_package.buf[raw_package.read_index] != 'A') &&
         (raw_package.buf[raw_package.read_index] != 'a')) ||
        ((raw_package.buf[raw_package.read_index + 1] != 'T') &&
         (raw_package.buf[raw_package.read_index + 1] != 't')))
    {
        uart_debug_buf_reset();
        return;
    }

    while (raw_package.write_index != raw_package.read_index)
    {
        uint8_t *ret = memchr(raw_package.buf + raw_package.read_index, '\n',
                              raw_package.write_index - raw_package.read_index);
        if (ret != NULL)
        {
            uint32_t offset = ret - (raw_package.buf + raw_package.read_index) + 1;
            ATCmdHandler(raw_package.buf + raw_package.read_index, offset);
            raw_package.read_index = raw_package.read_index + offset;
        }
        else
        {
            break;
        }
    }
    if (raw_package.write_index == raw_package.read_index)
    {
        uart_debug_buf_reset();
        APP_PRINT_INFO0("do clear!");
    }

}

void ATCmdHandler(uint8_t *buf, uint32_t len)
{
    if (memcmp("AT+TP+", raw_package.buf, strlen("AT+TP+")) == 0)
    {
        sim_tp_handle_package(buf + strlen("AT+TP+"), len - strlen("AT+TP+"));
    }
    else if (memcmp("AT+GS+", raw_package.buf, strlen("AT+GS+")) == 0)
    {

    }
    else if (memcmp("AT+HR+", raw_package.buf, strlen("AT+HR+")) == 0)
    {

    }
}


void uart_debug_event_handler(T_IO_MSG msg)
{
    if (uart_debug_has_installed == false)
    {
        return;
    }
    uint8_t value = msg.subtype;
    switch (value)
    {
    case UART_DEBUG_MSG_INTERRUPT:
        {
            //parse the package
            AtCmdParse();
            break;
        }
    case UART_DEBUG_SIM_TP_INT:
        {
            sim_tp_interrupt();
            break;
        }
    case UART_DEBUG_SIM_TP_RELEASE:
        {
            sim_tp_release();
            break;
        }
    default:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            break;
        }
    }
}


