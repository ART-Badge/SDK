/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     communicate_parse.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "communicate_parse.h"
#include "communicate_parse_bond.h"
#include "wristband_global_data.h"
#include "string.h"
#include "os_timer.h"
#include "app_flash_config.h"
#include "ftl.h"
#include "hub_task.h"
#include "app_task.h"
#include "trace.h"
#include "gap_bond_le.h"
#include "hub_interaction.h"
#include "ancs.h"
#include "ancs_client.h"


/* disconnect timer */
void *unbond_disc_timer;

void unbond_timerout_callback(void *pxTimer)
{
    if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
    {
        APP_PRINT_INFO0("unbond timeout disconnect!");
        le_disconnect(RtkWristbandSys.wristband_conn_id);
    }
}
void communicate_parse_init(void)
{
    os_timer_create(&unbond_disc_timer, "unbond disc_timer", 1, 2000, false, unbond_timerout_callback);
}

/**
* @brief   resolve private bond command received from remote APP
* @param   key: L2 key
* @param   pValue: received value pointer
* @param   length: value length
* @retval  error code
*/
void resolve_private_bond_command(uint8_t key, const uint8_t *pValue, uint16_t length)
{
    switch (key)
    {
    case KEY_BOND_REQUEST:
        {
            if (length == USER_ID_LENGTH && RtkWristbandSys.flag_field.bond_state == false)
            {
                APP_PRINT_INFO0("Bond Request");
                memcpy((void *)RtkWristbandSys.user_data.user_id, pValue, USER_ID_LENGTH);

                //change bond status machine
                RtkWristbandSys.flag_field.bond_state = true;
                RtkWristbandSys.flag_field.device_had_logged = true;
                L1_send_event(L1SEND_BOND_SUCCESS_EVENT, NULL);

                /*set current user state*/
                RtkWristbandSys.user_data.user_profile.data = 0;
                RtkWristbandSys.gPedoData.daily_step_target = 10000;//DEFAULT_STEP_TASK

                T_IO_MSG gsensor_msg;
                gsensor_msg.type = HUB_MSG_GSENSOR;
                gsensor_msg.subtype = GSENSOR_MSG_START;
                send_msg_to_ui_task(&gsensor_msg, __LINE__);

                wristband_system_interact(INTERACT_BONDED, 0, NULL);
            }
            else
            {
                APP_PRINT_INFO0("Bond request Command Invalid");
                L1_send_event(L1SEND_BOND_FAIL_EVENT, NULL);
            }
        }
        break;
    case KEY_LOGIN_REQUEST:
        {
            if (length == USER_ID_LENGTH && RtkWristbandSys.flag_field.bond_state == true)
            {
                APP_PRINT_INFO0("login request");
                /* check_user_id_bonded */
                if (memcmp(pValue, (void *)RtkWristbandSys.user_data.user_id, length) == 0)
                {
                    APP_PRINT_INFO0("login success ");
                    RtkWristbandSys.flag_field.device_had_logged = true;
                    L1_send_event(L1SEND_LOGIN_SUCCESS_EVENT, NULL);
                    wristband_system_interact(INTERACT_LOGIN, 0, NULL);
                }
                else
                {
                    APP_PRINT_INFO0("login fail ");
                    L1_send_event(L1SEND_LOGIN_FAIL_EVENT, NULL);
                }
            }
            else
            {
                L1_send_event(L1SEND_LOGIN_FAIL_EVENT, NULL);
            }
        }
        break;
    case KEY_UNBOND:
        {
            APP_PRINT_INFO0("Unbond !!! ");

            T_IO_MSG gsensor_msg;
            gsensor_msg.type = HUB_MSG_GSENSOR;
            gsensor_msg.subtype = GSENSOR_MSG_STOP;
            send_msg_to_ui_task(&gsensor_msg, __LINE__);
            RtkWristbandSys.flag_field.bond_state = false;
            RtkWristbandSys.flag_field.device_had_logged = false;

            /*clear user data*/
            RtkWristbandSys.gPedoData.global_steps = 0;
            RtkWristbandSys.gPedoData.global_distance = 0;
            RtkWristbandSys.gPedoData.global_calorys = 0;
            RtkWristbandSys.gPedoData.quarter_steps = 0;
            RtkWristbandSys.gPedoData.quarter_distance = 0;
            RtkWristbandSys.gPedoData.quarter_calories = 0;

            /*erase user id */
            memset((void *)RtkWristbandSys.user_data.user_id, 0x00, USER_ID_LENGTH);
            //reset phone os version
            RtkWristbandSys.phone_os_version = NONE;
            RtkWristbandSys.flag_field.auto_sync_enable = false;

            reset_wristband_config();

            wristband_system_interact(INTERACT_CANCEL_BOND, 0, NULL);

            if (unbond_disc_timer != NULL)
            {
                os_timer_start(&unbond_disc_timer);
            }
        }
        break;
    default:
        break;
    }
}





