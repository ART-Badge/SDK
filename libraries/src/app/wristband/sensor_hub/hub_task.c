/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      communicate_task.c
   * @brief     Routines to create App task and handle events & messages
   * @author    howie
   * @date      2019-12-26
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <os_msg.h>
#include <os_task.h>
#include <os_sync.h>
#include <hub_task.h>
#include <app_msg.h>
#include "trace.h"
#include "string.h"
#include "wristband_global_data.h"
#include "hub_clock.h"
#include "hub_display.h"
#include "hub_button.h"
#include "hub_battery.h"
#include "hub_gsensor.h"
#include "hub_charger.h"
#include "hub_touch.h"
#include "hub_hrm.h"
#include "hub_uart_debug.h"
#include "module_uart_debug.h"
#include "spic_1_psram.h"
#include "hci_app.h"
#include "math.h"



/** @addtogroup  PERIPH_DEMO
    * @{
    */

/** @defgroup  SENSOR_HUB_TASK Peripheral App Task
    * @brief This file handles the implementation of application task related functions.
    *
    * Create App task and handle events & messages
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/

#define UI_TASK_PRIORITY          1        //!< Task priorities
#define UI_TASK_STACK_SIZE        256 * 4  //!<  Task stack size


/*============================================================================*
 *                              Variables
 *============================================================================*/

void *hub_task_handle;
void *hub_queue_handle;
void *flash_mutex_handle;

T_IO_MODULE_CBS    *driver_modules[0x10]  = {NULL};
uint8_t             wristband_sensor_hub_count = 0;
/*============================================================================*
 *                              Functions
 *============================================================================*/
void ui_task(void *p_param);

/**
 * @brief  Initialize App task
 * @return void
 */
void ui_task_init()
{
    os_task_create(&hub_task_handle, "l1send", ui_task, 0, UI_TASK_STACK_SIZE,
                   UI_TASK_PRIORITY);
}

bool send_msg_to_ui_task(T_IO_MSG *p_msg, uint32_t line)
{
    if (os_msg_send(hub_queue_handle, p_msg, 0) == false)
    {
        APP_PRINT_ERROR1("send_io_msg_to_ui task fail io queue line = %d", line);
        return false;
    }
    return true;
}


void wristband_driver_init(void)
{
#if 1
    /* we must load config and do data reset here */
    wristband_config_data_init();

    clock_add_hub_task();
    psram_add_hub_task();
    button_add_hub_task();
    display_add_hub_task();
    //gsensor_add_hub_task();
    //battery_add_hub_task();
   // touch_add_hub_task();
    //charge_add_hub_task();
    //hrm_sensor_add_hub_task();
    //bbpro_hci_add_hub_task();
    //uart_debug_add_hub_task();

#endif
}
/**
* @brief  Wristband Exit DLPS callback function
* @param  No parameter.
* @return void
*/
void wristband_enter_dlps(void)
{
    for (uint8_t i = 0; i < wristband_sensor_hub_count; i++)
    {
        if (driver_modules[i]->enter_dlps_cb != NULL)
        {
            driver_modules[i]->enter_dlps_cb();
        }
    }
}


/**
* @brief   Wristband Exit DLPS callback function
* @param   No parameter.
* @return  void
*/
void wristband_exit_dlps(void)
{
    /*Here,System_WakeUpPinDisable couldn't be used*/
    for (uint8_t i = 0; i < wristband_sensor_hub_count; i++)
    {
        if (driver_modules[i]->exit_dlps_cb != NULL)
        {
            driver_modules[i]->exit_dlps_cb();
        }
    }
}
/**
* @brief   Wristband Enter DLPS Check
* @param   No parameter.
* @return  void
*/
bool wristband_enter_dlps_check(void)
{
    /*Here, check following condition to judge whether allow enter dlps or not*/
    for (uint8_t i = 0; i < wristband_sensor_hub_count; i++)
    {
        if (driver_modules[i]->allowed_dlps_cb != NULL)
        {
            if (driver_modules[i]->allowed_dlps_cb() == false)
            {
                return false;
            }
        }
    }
    return true;
}
/**
 * @brief        sensor hub task
 * @param[in]    p_params    Parameters sending to the task
 * @return       void
 */

void ui_task(void *pvParameters)
{
    T_IO_MSG hub_msg;

    os_msg_queue_create(&hub_queue_handle, 0x10, sizeof(T_IO_MSG));
    os_mutex_create(&flash_mutex_handle);

    wristband_driver_init();
    APP_PRINT_INFO0("[SENSOR HUB TASK] task test");
    while (true)
    {
        if (os_msg_recv(hub_queue_handle, &hub_msg, 0xFFFFFFFF) == true)
        {
            hub_task_handle_msg(hub_msg);
        }
    }
}

void hub_task_handle_msg(T_IO_MSG hub_msg)
{
    uint16_t msg_type = hub_msg.type;
    switch (msg_type)
    {
    case HUB_MSG_LCD:
        {
            display_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_GSENSOR:
        {
            gsensor_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_BATTERY_ADC:
        {
            battery_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_BUTTON:
        {
            button_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_TOUCH:
        {
            touch_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_CLOCK:
        {
            minute_system_clock_message_handle();
        }
        break;
    case HUB_MSG_CHARGE:
        {
            charge_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_HRM:
        {
            hrs_event_handler(hub_msg);
        }
        break;
    case HUB_MSG_UART_DEBUG:
        {
            uart_debug_event_handler(hub_msg);
        }
        break;
    default:
        break;
    }
}

/** @} */ /* End of group IO_TASK */
/** @} */ /* End of group PERIPH_DEMO */

