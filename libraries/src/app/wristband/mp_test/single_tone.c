/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      single_tone.c
* @brief     single tone test implementation.
* @details   none.
* @author    Chenjie Jin
* @date      2018-05-30
* @version   v1.0
* *********************************************************************************************************
*/
#include "board.h"
#include "single_tone.h"
#include "trace.h"
#include "os_mem.h"
#include "os_task.h"
#include "os_sched.h"
#include "os_timer.h"
#include "rtl876x_wdg.h"
#include <app_section.h>

/* open EXIT_SINGLE_TONE_TEST_WHEN_TIMEOUT to enable the function of exiting Single Tone Test when Timeout */
//#define EXIT_SINGLE_TONE_TEST_WHEN_TIMEOUT
#define EXIT_SINGLE_TONE_TIME    (2*60*1000)    //2min

typedef void *TimerHandle_t;
#ifdef EXIT_SINGLE_TONE_TEST_WHEN_TIMEOUT
static TimerHandle_t single_tone_exit_timer = NULL;
#endif

static bool single_tone_is_sent_start_cmd = false;
static bool single_tone_is_sent_restart_cmd = false;
void *single_tone_task_handle;

static void single_tone_reset(uint8_t channel_num);
/**
  * @brief  reset singletone
  * @param  channel_num: channel of singletone
  * @retval none
  */
void single_tone_reset(uint8_t channel_num)
{
    T_SINGLE_TONE_VEND_CMD_PARAMS *p_vend_cmd_params = os_mem_alloc(RAM_TYPE_DATA_ON,
                                                                    sizeof(T_SINGLE_TONE_VEND_CMD_PARAMS));

    if (p_vend_cmd_params)
    {
        APP_PRINT_INFO0("Single Tone Reset Command!");

        p_vend_cmd_params->pkt_type = 1;
        p_vend_cmd_params->opcode = 0xfc78;
        p_vend_cmd_params->length = 4;
        p_vend_cmd_params->start = 0;
        p_vend_cmd_params->channle = channel_num;
        p_vend_cmd_params->tx_power = 6;
        p_vend_cmd_params->is_le = 1;

        hci_if_write((uint8_t *)p_vend_cmd_params, sizeof(T_SINGLE_TONE_VEND_CMD_PARAMS));

        single_tone_is_sent_restart_cmd = true;
    }
}

/**
  * @brief  start singletone
  * @param  channel_num: channel of singletone
  * @retval none
  */
void single_tone_start(uint8_t channel_num)
{
    APP_PRINT_INFO0("Single Tone Start!");

    T_SINGLE_TONE_VEND_CMD_PARAMS *p_vend_cmd_params = os_mem_alloc(RAM_TYPE_DATA_ON,
                                                                    sizeof(T_SINGLE_TONE_VEND_CMD_PARAMS));

    if (p_vend_cmd_params)
    {
        p_vend_cmd_params->pkt_type = 1;
        p_vend_cmd_params->opcode = 0xfc78;
        p_vend_cmd_params->length = 4;
        p_vend_cmd_params->start = 1;
        p_vend_cmd_params->channle = channel_num;
        /** note:
          * tx_power config:
          * 0x30/(-20dBm), 0x80/0dBm, 0x90/3dBm,
          * 0xA0/4dBm, 0xD0/7.5dBm(only for rtl876x)
          */
        p_vend_cmd_params->tx_power = 0x80;
        p_vend_cmd_params->is_le = 1;

        hci_if_write((uint8_t *)p_vend_cmd_params, sizeof(T_SINGLE_TONE_VEND_CMD_PARAMS));

        single_tone_is_sent_start_cmd = true;
    }
}

bool single_tone_handle_hci_cb(T_SINGLE_TONE_EVT evt, bool status, uint8_t *p_buf, uint32_t len)
{
    bool result = true;
    uint8_t channel_num = 20; //2442

    APP_PRINT_INFO1("[single_tone_handle_hci_cb] evt is %d", evt);

    switch (evt)
    {
    case SINGLE_TONE_EVT_OPENED:
        if (!single_tone_is_sent_restart_cmd)
        {
            single_tone_reset(channel_num);
        }
        break;

    case SINGLE_TONE_EVT_CLOSED:
        break;

    case SINGLE_TONE_EVT_DATA_IND:
        hci_if_confirm(p_buf);
        if (!single_tone_is_sent_start_cmd)
        {
            single_tone_start(channel_num);
        }
        break;

    case SINGLE_TONE_EVT_DATA_XMIT:
        os_mem_free(p_buf);
        break;

    case SINGLE_TONE_EVT_ERROR:
        break;

    default:
        break;
    }
    return (result);
}


void single_tone_task(void *p_param)
{
    os_delay(100);
    hci_if_open(single_tone_handle_hci_cb);

    while (1)
    {
        os_delay(1000);
    }
}

#ifdef EXIT_SINGLE_TONE_TEST_WHEN_TIMEOUT
DATA_RAM_FUNCTION
void single_tone_exit_timeout_cb(TimerHandle_t p_timer)
{
    WDG_SystemReset(RESET_ALL, SINGLE_TONE_TIMEOUT_RESET);
}
#endif

void single_tone_init(void)
{
    APP_PRINT_INFO0("Single Tone Init");

#ifdef EXIT_SINGLE_TONE_TEST_WHEN_TIMEOUT
    if (true == os_timer_create(&single_tone_exit_timer, "single_tone_exit_timer",  1,
                                EXIT_SINGLE_TONE_TIME, false, single_tone_exit_timeout_cb))
    {
        os_timer_start(&single_tone_exit_timer);
    }
#endif

    os_task_create(&single_tone_task_handle, "single_tone", single_tone_task, 0, 256, 1);
}


