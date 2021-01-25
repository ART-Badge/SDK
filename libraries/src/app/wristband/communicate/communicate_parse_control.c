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
#include "communicate_parse_control.h"
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
* @brief   resolve Control data command received from remote APP
* @param   key: L2 key
* @param   pValue: received value pointer
* @param   length: value length
* @retval  error code
*/
void resolve_Control_command(uint8_t key, const uint8_t *pValue, uint16_t length)
{
    switch (key)
    {
    case KEY_PHONE_CAMERA_STATUS:
        {
            if (length == 1)
            {
                uint8_t value = pValue[0];
                APP_PRINT_INFO1("take photo command id, value:%d", value);
                if (value == 0x00)
                {
                    RtkWristbandSys.flag_field.phone_camera_status = true;
                    rtk_gsa_act_switch(GSA_ACT_WAVE, true);
                }
                else if (value == 0x01)
                {
                    RtkWristbandSys.flag_field.phone_camera_status = false;
                    rtk_gsa_act_switch(GSA_ACT_WAVE, false);
                }
            }
        }
        break;
    default:
        break;
    }
}






