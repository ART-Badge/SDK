/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dfu_task.c
  * @brief   dfu task for normal ota.
  * @details
  * @author
  * @date
  * @version
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
#include "os_msg.h"
#include "os_task.h"
#include "gap_msg.h"
#include "app_msg.h"
#include "trace.h"
#include "dfu_application.h"
#include "dfu_main.h"
#include "dfu_task.h"
#include "board.h"

#if (SUPPORT_NORMAL_OTA == 1)
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define DFU_TASK_PRIORITY               1
#define DFU_TASK_STACK_SIZE             512 * 8  //todo: sync with bee2, may modify

#define MAX_NUMBER_OF_GAP_MESSAGE       0x20
#define MAX_NUMBER_OF_IO_MESSAGE        0x20
#define MAX_NUMBER_OF_EVENT_MESSAGE     (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)

void *dfu_task_handle;
void *dfu_evt_queue_handle;
void *dfu_io_queue_handle;

void dfu_main_task(void *p_param);

bool app_send_msg_to_dfutask(T_IO_MSG *p_msg)
{
    uint8_t event = EVENT_IO_TO_APP;

    if (os_msg_send(dfu_io_queue_handle, p_msg, 0) == false)
    {
        APP_PRINT_ERROR0("send_io_msg_to_app fail");
        return false;
    }
    if (os_msg_send(dfu_evt_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR0("send_evt_msg_to_app fail");
        return false;
    }
    return true;
}

void dfu_task_init()
{
    os_task_create(&dfu_task_handle, "dfu", dfu_main_task, 0, DFU_TASK_STACK_SIZE,
                   DFU_TASK_PRIORITY);
}

/**
* @brief
*
*
* @param   pvParameters
* @return  void
*/
void dfu_main_task(void *p_param)
{
    uint8_t event;

    os_msg_queue_create(&dfu_io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&dfu_evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    gap_start_bt_stack(dfu_evt_queue_handle, dfu_io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

    dfu_timer_init();

    while (true)
    {
        if (os_msg_recv(dfu_evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            //DBG_DIRECT("***os_msg_recv***");
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(dfu_io_queue_handle, &io_msg, 0) == true)
                {
                    dfu_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}
#endif //end SUPPORT_NORMAL_OTA
