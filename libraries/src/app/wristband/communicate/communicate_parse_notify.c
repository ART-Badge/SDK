/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     communicate_parse_notify.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "communicate_parse.h"
#include "communicate_parse_notify.h"
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




/**
* @brief   resolve notify command received from remote APP
* @param   key: L2 key
* @param   pValue: received value pointer
* @param   length: value length
* @retval  error code
*/
void resolve_Notify_command(uint8_t key, uint8_t *pValue, uint16_t length)
{
    switch (key)
    {
    case KEY_INCOMMING_CALL:
        {
            if (RtkWristbandSys.msg_switch.bit.switch_call_msg)
            {
                pValue[length] = '\0';
                wristband_system_interact(INTERACT_CALLING, 0, pValue);
            }
            break;
        }
    case KEY_INCOMMING_CALL_ACCEPT:
        {
            if (RtkWristbandSys.msg_switch.bit.switch_call_msg)
            {
                wristband_system_interact(INTERACT_CALLING_ACCEPT, 0, NULL);
            }
            break;
        }
    case KEY_INCOMMING_CALL_REFUSE:
        {
            if (RtkWristbandSys.msg_switch.bit.switch_call_msg)
            {
                wristband_system_interact(INTERACT_CALLING_REFUSE, 0, NULL);
            }
            break;
        }
    case KEY_INCOMMING_MESSAGE:
        {
            uint8_t msg_notify_bit = pValue[0];
            static MSG_DATA_PAYLOAD payload;
            payload.length = length;
            payload.p_msg_value = pValue;
            if ((RtkWristbandSys.msg_switch.bit.switch_qq_msg) && (msg_notify_bit & (QQ_NOTIFY_BIT)) &&
                length > 1)
            {
                wristband_system_interact(INTERACT_QQ, 0, &payload);
            }
            if ((RtkWristbandSys.msg_switch.bit.switch_shortmessage_msg) &&
                (msg_notify_bit & (SHOTMESSAGE_NOTIFY_BIT)) && length > 1)
            {
                wristband_system_interact(INTERACT_PHONEMESSAGE, 0, &payload);
            }
            if ((RtkWristbandSys.msg_switch.bit.switch_wechat_msg) &&
                (msg_notify_bit & (WECHAT_NOTIFY_BIT)) && length > 1)
            {
                wristband_system_interact(INTERACT_WECHAT, 0, &payload);
            }
            if ((RtkWristbandSys.msg_switch.bit.switch_line_msg) &&
                (msg_notify_bit & (LINE_NOTIFY_BIT)) &&
                length > 1)
            {
                wristband_system_interact(INTERACT_LINE, 0, &payload);
            }
            break;
        }
    default:
        break;
    }
}



