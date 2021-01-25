/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file
* @brief
* @details
* @author
* @date
* @version
*********************************************************************************************************
*/

#include "board.h"
#include "trace.h"
#include <string.h>
#include "sync_data.h"
#include "wristband_global_data.h"
#include "communicate_parse.h"
#include "flash_device.h"
#include "flash_adv_cfg.h"
#include "os_sync.h"
#include "app_flash_config.h"

void WristBandMSGDataBlockInit(void)
{
    RtkWristbandSys.msg_data_config.p_write_start_addr = MSG_DATA_START_ADDRESS;
    RtkWristbandSys.msg_data_config.p_read_start_addr = MSG_DATA_START_ADDRESS;
    RtkWristbandSys.msg_data_config.cur_msg_counts = 0;
    RtkWristbandSys.msg_data_config.max_msg_counts = FLASH_SECTOR_SIZE / MSG_DATA_PACKAGE_LEN;
    RtkWristbandSys.msg_data_config.single_package_len = MSG_DATA_PACKAGE_LEN;
    uint8_t prev_bp_lv = 0;
    flash_sw_protect_unlock_by_addr_locked(MSG_DATA_START_ADDRESS, &prev_bp_lv);
    uint32_t s;
    s = os_lock();
    flash_erase_locked(FLASH_ERASE_SECTOR, MSG_DATA_START_ADDRESS);
    flash_erase_locked(FLASH_ERASE_SECTOR, MSG_DATA_START_ADDRESS + FLASH_SECTOR_SIZE);
    os_unlock(s);
    flash_set_block_protect_locked(prev_bp_lv);
}


static bool BandMsgDataStore(uint8_t *buf, uint16_t len)
{
    if (len > RtkWristbandSys.msg_data_config.single_package_len)
    {
        APP_PRINT_ERROR0("Msg data more than single package max len!");
        return false;
    }
    else if ((len % 4) != 0)
    {
        return false;
    }
    else
    {
        uint8_t prev_bp_lv = 0;
        flash_sw_protect_unlock_by_addr_locked(MSG_DATA_START_ADDRESS, &prev_bp_lv);

        uint32_t data = 0;
        for (uint8_t i = 0; i < len; i = i + 4)
        {
            memcpy(&data, buf + i, 4);
            if (flash_auto_write_locked(RtkWristbandSys.msg_data_config.p_write_start_addr + i, data) == false)
            {
                APP_PRINT_ERROR0("Msg data save error!");
            }
        }

        flash_set_block_protect_locked(prev_bp_lv);
        return true;
    }
}

static bool BandMsgDataRestore(uint8_t *buf, uint32_t p_read_addr, uint16_t len)
{
    if ((len % 4) != 0)
    {
        return false;
    }
    if (
        ((RtkWristbandSys.msg_data_config.p_write_start_addr <
          RtkWristbandSys.msg_data_config.p_read_start_addr) && \
         (
             ((p_read_addr  + len) < FLASH_SECTOR_SIZE + MSG_DATA_START_ADDRESS && \
              (p_read_addr  + len) > RtkWristbandSys.msg_data_config.p_write_start_addr)

             || \
             ((p_read_addr  + len) > FLASH_SECTOR_SIZE + MSG_DATA_START_ADDRESS && \
              ((p_read_addr  + len) < RtkWristbandSys.msg_data_config.p_read_start_addr))
         )
        ) ||
        (
            (RtkWristbandSys.msg_data_config.p_write_start_addr >
             RtkWristbandSys.msg_data_config.p_read_start_addr) && \
            (
                ((p_read_addr  + len) < FLASH_SECTOR_SIZE + MSG_DATA_START_ADDRESS && \
                 (p_read_addr  + len) < RtkWristbandSys.msg_data_config.p_read_start_addr)

                || \
                ((p_read_addr  + len) > FLASH_SECTOR_SIZE + MSG_DATA_START_ADDRESS && \
                 ((p_read_addr  + len) > RtkWristbandSys.msg_data_config.p_write_start_addr))
            )
        )
    )
    {
        return false;
    }
    if (p_read_addr  == RtkWristbandSys.msg_data_config.p_write_start_addr)
    {
        return false;
    }
    uint32_t data = 0;
    for (uint8_t i = 0; i < len; i = i + 4)
    {
        if (flash_auto_read_locked((p_read_addr + i) | 0x1000000, &data) == false)
        {
            APP_PRINT_ERROR0("msg data read error!");
        }
        memcpy(buf + i, &data, 4);
    }
    return true;
}
//0 is latest
bool WristBandMsgDataRestore(uint8_t *buf, uint16_t load_num)
{
    if (RtkWristbandSys.msg_data_config.cur_msg_counts - (load_num + 1) < 0)
    {
        return false;
    }

    uint32_t temp_address = 0;
    if (RtkWristbandSys.msg_data_config.p_read_start_addr + \
        RtkWristbandSys.msg_data_config.single_package_len * (RtkWristbandSys.msg_data_config.cur_msg_counts
                                                              - (load_num + 1))\
        < MSG_DATA_START_ADDRESS + 2 * FLASH_SECTOR_SIZE)
    {

        temp_address = RtkWristbandSys.msg_data_config.p_read_start_addr + \
                       RtkWristbandSys.msg_data_config.single_package_len * (RtkWristbandSys.msg_data_config.cur_msg_counts
                                                                             - (load_num + 1));
    }
    else
    {

        temp_address = RtkWristbandSys.msg_data_config.p_read_start_addr + \
                       RtkWristbandSys.msg_data_config.single_package_len * (RtkWristbandSys.msg_data_config.cur_msg_counts
                                                                             - (load_num + 1)) - \
                       2 * FLASH_SECTOR_SIZE;

    }
    if (BandMsgDataRestore(buf, temp_address,
                           RtkWristbandSys.msg_data_config.single_package_len) == false)
    {
        return false;
    }
    return true;
}
bool WristBandMsgDataStore(uint8_t *buf)
{
    if (BandMsgDataStore(buf, RtkWristbandSys.msg_data_config.single_package_len) == false)
    {
        return false;
    }



    if ((RtkWristbandSys.msg_data_config.p_write_start_addr +
         RtkWristbandSys.msg_data_config.single_package_len < MSG_DATA_START_ADDRESS + FLASH_SECTOR_SIZE))
    {
        RtkWristbandSys.msg_data_config.p_write_start_addr +=
            RtkWristbandSys.msg_data_config.single_package_len;
        if (RtkWristbandSys.msg_data_config.cur_msg_counts ==
            RtkWristbandSys.msg_data_config.max_msg_counts)
        {
            RtkWristbandSys.msg_data_config.p_read_start_addr +=
                RtkWristbandSys.msg_data_config.single_package_len;
        }
    }
    else if (RtkWristbandSys.msg_data_config.p_write_start_addr +
             RtkWristbandSys.msg_data_config.single_package_len >= MSG_DATA_START_ADDRESS + 2 *
             FLASH_SECTOR_SIZE)
    {
        RtkWristbandSys.msg_data_config.p_write_start_addr +=
            RtkWristbandSys.msg_data_config.single_package_len;
        if (RtkWristbandSys.msg_data_config.p_write_start_addr == MSG_DATA_START_ADDRESS + 2 *
            FLASH_SECTOR_SIZE)
        {
            /* erase 1 SECTOR*/
            APP_PRINT_ERROR0("erase 1 SECTOR!");
            uint8_t prev_bp_lv = 0;
            flash_sw_protect_unlock_by_addr_locked(MSG_DATA_START_ADDRESS, &prev_bp_lv);
            uint32_t s;
            s = os_lock();
            if (flash_erase_locked(FLASH_ERASE_SECTOR, MSG_DATA_START_ADDRESS) == false)
            {
                APP_PRINT_ERROR0("erase 1 SECTOR error!");
            }
            os_unlock(s);
            flash_set_block_protect_locked(prev_bp_lv);
        }
        RtkWristbandSys.msg_data_config.p_write_start_addr = MSG_DATA_START_ADDRESS;
        RtkWristbandSys.msg_data_config.p_read_start_addr +=
            RtkWristbandSys.msg_data_config.single_package_len;
    }
    else
    {
        RtkWristbandSys.msg_data_config.p_write_start_addr +=
            RtkWristbandSys.msg_data_config.single_package_len;
        if (RtkWristbandSys.msg_data_config.cur_msg_counts ==
            RtkWristbandSys.msg_data_config.max_msg_counts)
        {
            if (RtkWristbandSys.msg_data_config.p_read_start_addr +
                RtkWristbandSys.msg_data_config.single_package_len >= MSG_DATA_START_ADDRESS + 2 *
                FLASH_SECTOR_SIZE)
            {
                RtkWristbandSys.msg_data_config.p_read_start_addr = MSG_DATA_START_ADDRESS;
            }
            else
            {
                RtkWristbandSys.msg_data_config.p_read_start_addr +=
                    RtkWristbandSys.msg_data_config.single_package_len;
            }
        }
        if (RtkWristbandSys.msg_data_config.p_write_start_addr == MSG_DATA_START_ADDRESS +
            FLASH_SECTOR_SIZE)
        {
            /* erase 2 SECTOR*/
            APP_PRINT_ERROR0("erase 2 SECTOR!");
            uint8_t prev_bp_lv = 0;
            flash_sw_protect_unlock_by_addr_locked(MSG_DATA_START_ADDRESS + FLASH_SECTOR_SIZE, &prev_bp_lv);
            uint32_t s;
            s = os_lock();
            if (flash_erase_locked(FLASH_ERASE_SECTOR, MSG_DATA_START_ADDRESS + FLASH_SECTOR_SIZE) == false)
            {
                APP_PRINT_ERROR0("erase 2 SECTOR error!");
            }
            os_unlock(s);
            flash_set_block_protect_locked(prev_bp_lv);
        }
    }
    if (RtkWristbandSys.msg_data_config.cur_msg_counts < RtkWristbandSys.msg_data_config.max_msg_counts)
    {
        RtkWristbandSys.msg_data_config.cur_msg_counts++;
    }
    return true;
}
uint8_t WristBandGetCurMsgCount()
{
    return RtkWristbandSys.msg_data_config.cur_msg_counts;
}
