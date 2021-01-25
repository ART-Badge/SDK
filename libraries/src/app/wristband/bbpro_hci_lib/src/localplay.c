/**
*********************************************************************************************************
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

/* Includes ------------------------------------------------------------------*/
#include "hci_app.h"
#include "rtl8763bf_task.h"
#include "rtl876x_pinmux.h"
#include "wristband_gap.h"
#include "gap_adv.h"
#include "app_task.h"
#include "board.h"
#include "communicate_protocol.h"
#include "wristband_global_data.h"
#include "app_flash_config.h"
#include "localplay.h"



T_MEDIA_INFO media_info = {0};

void save_media_list(uint8_t *name, uint32_t len)
{
    APP_PRINT_INFO2("[HOWIE]media_info.had_scaned_list_num = %d, LINE = %d",
                    media_info.had_scaned_list_num, __LINE__);
    media_info.music[media_info.had_scaned_list_num].name_addr = MUSIC_INFO_START_ADDRESS +
                                                                 media_info.had_scaned_list_num * 256;
    media_info.music[media_info.had_scaned_list_num].name_len = len / 2;
    flash_write_locked(media_info.music[media_info.had_scaned_list_num].name_addr, len, name);
    media_info.had_scaned_list_num++;

}

void prepare_get_media_list(void)
{
    //do flash clear
    if (flash_erase_locked(FLASH_ERASE_SECTOR, MUSIC_INFO_START_ADDRESS) == false)
    {
        APP_PRINT_ERROR0("fs data erase error!");
    }
    //do media info struct clear
    memset(&media_info, 0, sizeof(media_info));
}



/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

