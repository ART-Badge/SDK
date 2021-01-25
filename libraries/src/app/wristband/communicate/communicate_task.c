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
#include <gap.h>
#include <gap_le.h>
#include <gap_msg.h>
#include <communicate_task.h>
#include <app_msg.h>
#include <app_task.h>
#include "trace.h"
#include "sync_data.h"
#include "wristband_application.h"
#include "communicate_protocol.h"
#include "communicate_parse.h"
#include "wristband_global_data.h"
#include "version.h"

/** @addtogroup  PERIPH_DEMO
    * @{
    */

/** @defgroup  PERIPH_APP_TASK Peripheral App Task
    * @brief This file handles the implementation of application task related functions.
    *
    * Create App task and handle events & messages
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/

#define L1SEND_TASK_PRIORITY          1        //!< Task priorities
#define L1SEND_TASK_STACK_SIZE        256 * 4  //!<  Task stack size


/*============================================================================*
 *                              Variables
 *============================================================================*/

void *l1send_task_handle;   //!< APP Task handle
void *l1send_queue_handle;  //!< Event queue handle
void *l1recv_queue_handle;  //!< Event queue handle

/*============================================================================*
 *                              Functions
 *============================================================================*/
void l1send_task(void *p_param);

/**
 * @brief  Initialize App task
 * @return void
 */
void communicate_task_init()
{
    os_task_create(&l1send_task_handle, "l1send", l1send_task, 0, L1SEND_TASK_STACK_SIZE,
                   L1SEND_TASK_PRIORITY);
}

void L1_send_event(L1SEND_TYPE_WRISTBAND event, void *res)
{
    if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
    {
        if (os_msg_send(l1send_queue_handle, &event, 0) == false)
        {
            APP_PRINT_ERROR0("send_msg_to_l1send_task_fail");
        }
    }
}

/**
 * @brief        L1send task
 * @param[in]    p_params    Parameters sending to the task
 * @return       void
 */

void l1send_task(void *pvParameters)
{
    uint8_t event;
    uint8_t psendbuf[30];

    os_msg_queue_create(&l1send_queue_handle, 0x10, sizeof(uint8_t));
    os_msg_queue_create(&l1recv_queue_handle, 0x10, sizeof(uint8_t));

    while (true)
    {
        if (os_msg_recv(l1send_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            APP_PRINT_INFO1("Send Event = %d", event);
            if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
            {
                switch (event)
                {
                case L1SEND_SPORT_DATA:
                    {
                        send_sport_data();
                        break;
                    }
                case L1SEND_SLEEP_DATA:
                    {
                        send_sleep_data();
                        break;
                    }
                case L1SEND_HEART_DATA:
                    {
                        send_heart_data();
                        break;
                    }
                case L1SEND_RETURN_CANCEL_HEART_SAMPLE:
                    {
                        psendbuf[8] = HEALTH_DATA_COMMAND_ID;       /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;            /*L2 header version */
                        psendbuf[10] = KEY_CANCEL_HEART_SAMPLE;     /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 0;                           /* length  */
                        L1_send(psendbuf, 13);
                        break;
                    }
                case L1SEND_RETURN_HEART_SETTING:
                    {
                        psendbuf[8] = HEALTH_DATA_COMMAND_ID;               /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;                    /*L2 header version */
                        psendbuf[10] = KEY_RETURN_HEART_SAMPLE_SETTING;     /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 2;                                   /* length  */
                        if (RtkWristbandSys.hrs_detect_period)
                        {
                            psendbuf[13] = 0x01;
                        }
                        else
                        {
                            psendbuf[13] = 0x00;
                        }
                        psendbuf[14] = RtkWristbandSys.hrs_detect_period;
                        L1_send(psendbuf, 15);
                        break;
                    }
                case L1SEND_HISTORY_DATA_SYNC_START:
                    {
                        psendbuf[8] = HEALTH_DATA_COMMAND_ID;       /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;            /*L2 header version */
                        psendbuf[10] = KEY_DATA_SYNC_START;         /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 0;                           /* length  */
                        L1_send(psendbuf, 13);
                        break;
                    }
                case L1SEND_HISTORY_DATA_SYNC_END:
                    {
                        psendbuf[8] = HEALTH_DATA_COMMAND_ID;       /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;            /*L2 header version */
                        psendbuf[10] = KEY_DATA_SYNC_END;           /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 0;                           /* length  */
                        L1_send(psendbuf, 13);

                        T_IO_MSG con_para_msg;
                        con_para_msg.type = IO_MSG_TYPE_WRISTBNAD;
                        con_para_msg.subtype = IO_MSG_UPDATE_CONPARA;
                        app_send_msg_to_apptask(&con_para_msg);
                        break;
                    }
                case L1SEND_BOND_FAIL_EVENT:
                    {
                        psendbuf[8] = BOND_COMMAND_ID;      /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;    /*L2 header version */
                        psendbuf[10] = 0x02;                /*first key, bond response*/
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                   /* length  = 1 */
                        psendbuf[13] = 0x01;                /* bond fail */
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_BOND_SUCCESS_EVENT:
                    {
                        psendbuf[8] = BOND_COMMAND_ID;      /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;    /*L2 header version */
                        psendbuf[10] = 0x02;                /*first key, bond response*/
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                   /* length  = 1 */
                        psendbuf[13] = 0;                   /* bond success */
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_LOGIN_FAIL_EVENT:
                    {
                        psendbuf[8] = BOND_COMMAND_ID;      /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;    /*L2 header version */
                        psendbuf[10] = 0x04;                /*first key, bond response*/
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                   /* length  = 1 */
                        psendbuf[13] = 0x01;                /* login fail */
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_LOGIN_SUCCESS_EVENT:
                    {
                        psendbuf[8] = BOND_COMMAND_ID;      /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;    /*L2 header version */
                        psendbuf[10] = 0x04;                /*first key, bond response*/
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                   /* length  = 1 */
                        psendbuf[13] = 0x00;                /* login success */
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_RETURN_ALARM_EVENT:
                    {
                        psendbuf[8] = SET_CONFIG_COMMAND_ID;    /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;        /*L2 header version */
                        psendbuf[10] = KEY_RETURN_ALARM_SETTINGS;         /*first key, */
                        psendbuf[11] = 0;
                        uint8_t alarm_item_count = 0;
                        for (uint8_t index = 0; index < RtkWristbandSys.alarm_num; index ++)
                        {
                            if (RtkWristbandSys.alarms[index].alarm.day_repeat_flag == 0 &&
                                RtkWristbandSys.alarms[index].alarm.reserved == 0)
                            {
                                //drop current alarm index
                                APP_PRINT_INFO0("delete alarm once item...\n");
                                continue;
                            }
                            psendbuf[13 + alarm_item_count * 5] =  RtkWristbandSys.alarms[index].data >>
                                                                   32;          /* bond success */
                            psendbuf[14 + alarm_item_count * 5] =  RtkWristbandSys.alarms[index].data >> 24;
                            psendbuf[15 + alarm_item_count * 5] =  RtkWristbandSys.alarms[index].data >> 16;
                            psendbuf[16 + alarm_item_count * 5] =  RtkWristbandSys.alarms[index].data >> 8;
                            psendbuf[17 + alarm_item_count * 5] =  RtkWristbandSys.alarms[index].data;
                            APP_PRINT_INFO3("hour:%d,min:%d,day_repeat_flag:%d", RtkWristbandSys.alarms[index].alarm.hour,
                                            RtkWristbandSys.alarms[index].alarm.minute, RtkWristbandSys.alarms[index].alarm.day_repeat_flag);
                            alarm_item_count++;
                        }
                        psendbuf[12] = alarm_item_count * 5;         /* length  */
                        L1_send(psendbuf, 13 + psendbuf[12]);
                        break;
                    }
                case L1SEND_RETURN_PHONE_CONTROL_CMD_EVENT:
                    {
                        psendbuf[8] = CONTROL_COMMAND_ID;      /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;       /*L2 header version */
                        psendbuf[10] = 0x01;                   /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 0;                      /* length  */
                        L1_send(psendbuf, 13);
                        break;
                    }
                case L1SEND_RETURN_FIND_MOBILE_COMMAND:
                    {
                        psendbuf[8] = CONTROL_COMMAND_ID;    /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;     /*L2 header version */
                        psendbuf[10] = 0x02;                 /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 0;                     /* length  */
                        L1_send(psendbuf, 13);
                        break;
                    }
                case L1SEND_RETURN_LIFT_SWITCH_EVENT:
                    {
                        psendbuf[8] = SET_CONFIG_COMMAND_ID;    /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;        /*L2 header version */
                        psendbuf[10] = KEY_LIFT_SWITCH_RETURN; /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                        /* length  */
                        psendbuf[13] = RtkWristbandSys.flag_field.lift_switch_status;    //need to write
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_RETURN_TWIST_SWITCH_EVENT:
                    {
                        psendbuf[8] = SET_CONFIG_COMMAND_ID;    /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;        /*L2 header version */
                        psendbuf[10] = KEY_TWIST_SWITCH_RETURN;  /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                        /* length  */
                        psendbuf[13] = RtkWristbandSys.flag_field.twist_switch_status;    //need to wtite
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_RETURN_INCOMMING_MESSAGE_SETTINGS:
                    {
                        psendbuf[8] = SET_CONFIG_COMMAND_ID;            /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;                /*L2 header version*/
                        psendbuf[10] = KEY_INCOMMING_MESSAGE_SETTINGS_RETURN; /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                                /* length  = 1 */
                        psendbuf[13] =
                            RtkWristbandSys.msg_switch.data;                               //incoming message need to write
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_SIT_SETTING_RETURN:
                    {
                        psendbuf[8] = SET_CONFIG_COMMAND_ID;            /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;                /*L2 header version */
                        psendbuf[10] = KEY_LONG_TIME_SIT_SETTING_RETURN; /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                                /* length  = 1 */
                        if (0x01 == RtkWristbandSys.sit_alert_data.sit_alert.on_off)
                        {
                            psendbuf[13] = 0x01;
                        }
                        else
                        {
                            psendbuf[13] = 0x00;
                        }
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_RETURN_CALL_REJECT_COMMAND:
                    {
                        psendbuf[8] = NOTIFY_COMMAND_ID;            /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;            /*L2 header version */
                        psendbuf[10] = KEY_INCOMMING_CALL_REJECT;   /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 0;                           /* length  = 1 */
                        L1_send(psendbuf, 13);
                        break;
                    }
                case L1SEND_RETURN_CHARGE_STATUS:
                    {
                        psendbuf[8] = NOTIFY_COMMAND_ID;            /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;                /*L2 header version */
                        psendbuf[10] = KEY_BATTERY_CHARGE_STATUS;   /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 1;                                /* length  = 1 */
                        psendbuf[13] = RtkWristbandSys.charger_status;
                        L1_send(psendbuf, 14);
                        break;
                    }
                case L1SEND_RETURN_DEVICE_INFO:
                    {
                        uint16_t deviceID = 124;
                        psendbuf[8] = SET_CONFIG_COMMAND_ID;            /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;                /*L2 header version */
                        psendbuf[10] = KEY_DEVICEINFO_RETURN;           /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 5;                               /* length  = 5 */
                        psendbuf[13] = deviceID >> 8;
                        psendbuf[14] = deviceID & 0xFF;
                        psendbuf[15] = VERSION_MAJOR;
                        psendbuf[16] = VERSION_MINOR;
                        psendbuf[17] = VERSION_REVISION;
                        L1_send(psendbuf, 18);
                    }
                    break;
                case L1SEND_RETURN_BBPRO_MAC_ADDR:
                    {
                        uint8_t return_bbpro_bd_addr[6];
                        memset(return_bbpro_bd_addr, 0x00, sizeof(return_bbpro_bd_addr));
                        psendbuf[8] = NOTIFY_COMMAND_ID;            /*command id*/
                        psendbuf[9] = L2_HEADER_VERSION;                /*L2 header version */
                        psendbuf[10] = KEY_BBPRO_MAC_ADDRESS_RETURN;   /*first key, */
                        psendbuf[11] = 0;
                        psendbuf[12] = 8;                                /* length  = 1 */
                        extern bool bbpro_bd_addr_got_flag;
                        extern uint8_t bbpro_bd_addr[];
                        if (bbpro_bd_addr_got_flag == true)
                        {
                            memcpy(return_bbpro_bd_addr, bbpro_bd_addr, 6);
                        }

                        memcpy(psendbuf + 13, bbpro_bd_addr, 6);
                        psendbuf[19] = RtkWristbandSys.paired_info.paired_flag;
                        psendbuf[20] = RtkWristbandSys.bbpro_device_status;
                        APP_PRINT_INFO1("[HOWIE TEST] L1SEND_BBPRO_MAC_ADDR: %b", \
                                        TRACE_BINARY(6, psendbuf + 13));
                        L1_send(psendbuf, 21);
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                APP_PRINT_ERROR0("Remove tx queue!");
            }

        }
    }
}

/** @} */ /* End of group PERIPH_APP_TASK */
/** @} */ /* End of group PERIPH_DEMO */

