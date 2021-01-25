/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    app_task.c
  * @brief   application task.
  * @details
  * @author  ranhui
  * @date    2016-02-18
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
#include <os_msg.h>
#include <os_task.h>
#include <gap.h>
#include <gap_le.h>
#include <gap_msg.h>
#include <trace.h>
#include <app_task.h>
#include <app_msg.h>
#include <app_task.h>
#include "test_app.h"
#include "silent_ota_application_swreset.h"
#include "otp_config.h"

#define APP_TASK_PRIORITY          1   /* Task priorities. */
#define APP_TASK_STACK_SIZE        512 * 4

#define MAX_NUMBER_OF_GAP_MESSAGE    0x20
#define MAX_NUMBER_OF_IO_MESSAGE      0x20
#define MAX_NUMBER_OF_EVENT_MESSAGE   (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)

void *app_task_handle;
void *evt_queue_handle;
void *io_queue_handle;

void app_main_task(void *p_param);

/**
 * \brief    send msg queue to app task.
 *
 * \param[in]   p_handle   The handle to the message queue being peeked.
 *
 * \return           The status of the message queue peek.
 * \retval true      Message queue was peeked successfully.
 * \retval false     Message queue was failed to peek.
 */
bool app_send_msg_to_apptask(T_IO_MSG *p_msg)
{
    uint8_t event = EVENT_IO_TO_APP;

    if (os_msg_send(io_queue_handle, p_msg, 0) == false)
    {
        APP_PRINT_ERROR0("send_io_msg_to_app fail");
        return false;
    }
    if (os_msg_send(evt_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR0("send_evt_msg_to_app fail");
        return false;
    }
    return true;
}

void app_task_init()
{
    os_task_create(&app_task_handle, "app", app_main_task, 0, APP_TASK_STACK_SIZE,
                   APP_TASK_PRIORITY);
}

void driver_init(void);
int codecInit(void);
/**
* @brief
*
*
* @param   pvParameters
* @return  void
*/
void app_main_task(void *p_param)
{
    uint8_t event;

    os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

#if (ROM_WATCH_DOG_ENABLE == 1)
    extern void reset_watch_dog_timer_enable(void);
    reset_watch_dog_timer_enable();
#endif

#if (DLPS_WAKEUP_RTC_EVENT == 1)
    extern void RTC_SampleCode(void);
    RTC_SampleCode();
#endif

    driver_init();

#if (DEBUG_TEST_DLPS_APP_ENABLE == 1)

#if (DLPS_WAKEUP_SWTIMER_EVENT == 1 || DLPS_WAKEUP_BT_EVENT == 1)
    test_DLPS_wakeup_swtimer_init();
#endif

    test_DLPS_debug_swtimer_init();
#endif

    while (true)
    {

#if (DLPS_WAKEUP_TASKDELAY_EVENT == 1)
        os_msg_recv(evt_queue_handle, &event, 100);
#else
        if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
#endif
        {
            //DBG_DIRECT("***os_msg_recv***");
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                {
                    app_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}



