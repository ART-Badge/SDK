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
#include "headset.h"
#include "module_font.h"
#include "stdlib.h"
#include "os_sync.h"
#include "hub_task.h"

#define NAME_OFFSET     8


T_HEADSET_INFO headset_info = {0};

void save_headset_list(uint8_t *name, uint32_t len)
{
    //malloc
    uint16_t *p_buf = NULL;
    p_buf = malloc(128 * sizeof(uint16_t));
    if (p_buf == NULL)
    {
        DBG_DIRECT("save_headset_list fail");
        while (1);
    }


    uint32_t len_unicode;
    uint8_t name_len = len - NAME_OFFSET;
    uint8_t *name_addr = name + NAME_OFFSET;
    if (name_len != 0)
    {
        headset_info.headset[headset_info.had_scaned_list_num].store_mac_addr = HEADSET_INFO_START_ADDRESS +
                                                                                headset_info.had_scaned_list_num * 256;
        //flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
        os_mutex_take(flash_mutex_handle, 500);
        flash_write_locked(headset_info.headset[headset_info.had_scaned_list_num].store_mac_addr, 6, name);
        os_mutex_give(flash_mutex_handle);
        headset_info.headset[headset_info.had_scaned_list_num].store_name_addr = HEADSET_INFO_START_ADDRESS
                                                                                 + headset_info.had_scaned_list_num * 256 + NAME_OFFSET;

        len_unicode = utf8_to_unicode(name_addr, name_len, p_buf, 128);
        //flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
        os_mutex_take(flash_mutex_handle, 500);
        flash_write_locked(headset_info.headset[headset_info.had_scaned_list_num].store_name_addr,
                           len_unicode * 2, (uint8_t *)p_buf);
        os_mutex_give(flash_mutex_handle);
        APP_PRINT_INFO1("[HEADSET] UTF-8: %b", TRACE_BINARY(name_len, name_addr));
        APP_PRINT_INFO1("[HEADSET] UNICODE: %b", TRACE_BINARY(len_unicode * 2,
                                                              headset_info.headset[headset_info.had_scaned_list_num].store_name_addr));

        headset_info.headset[headset_info.had_scaned_list_num].store_name_len = len_unicode;
    }
    else
    {
        headset_info.headset[headset_info.had_scaned_list_num].store_mac_addr = HEADSET_INFO_START_ADDRESS +
                                                                                headset_info.had_scaned_list_num * 256;
        //flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
        os_mutex_take(flash_mutex_handle, 500);
        flash_write_locked(headset_info.headset[headset_info.had_scaned_list_num].store_mac_addr, 6, name);
        os_mutex_give(flash_mutex_handle);
        headset_info.headset[headset_info.had_scaned_list_num].store_name_addr = HEADSET_INFO_START_ADDRESS
                                                                                 + headset_info.had_scaned_list_num * 256 + NAME_OFFSET;

        /* if no name we store NULL as its name*/
        static const char *name_null = "NULL";
        len_unicode = utf8_to_unicode((uint8_t *)name_null, strlen(name_null), p_buf, 128);
        //flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
        os_mutex_take(flash_mutex_handle, 500);
        flash_write_locked(headset_info.headset[headset_info.had_scaned_list_num].store_name_addr,
                           len_unicode * 2, (uint8_t *)p_buf);
        os_mutex_give(flash_mutex_handle);
        APP_PRINT_INFO1("[HEADSET null name] UTF-8: %b", TRACE_BINARY(strlen(name_null), name_null));
        APP_PRINT_INFO1("[HEADSET null name] UNICODE: %b", TRACE_BINARY(len_unicode * 2,
                                                                        headset_info.headset[headset_info.had_scaned_list_num].store_name_addr));
        headset_info.headset[headset_info.had_scaned_list_num].store_name_len = len_unicode;
    }


    headset_info.had_scaned_list_num++;
    headset_info.had_scaned_none_name_list_num++;

    /* free buffer */
    if (p_buf)
    {
        free(p_buf);
        p_buf = NULL;
    }
}

void prepare_get_headset_list(void)
{
    //do flash clear
    os_mutex_take(flash_mutex_handle, 500);
    //flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
    if (flash_erase_locked(FLASH_ERASE_SECTOR, HEADSET_INFO_START_ADDRESS) == false)
    {
        APP_PRINT_ERROR0("fs data erase error!");
    }
    os_mutex_give(flash_mutex_handle);
    //do media info struct clear
    memset(&headset_info, 0, sizeof(headset_info));
}



/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

