enum { __FILE_NUM__ = 0 };

/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     app_flash.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "trace.h"
#include <string.h>
#include "ftl.h"
#include "platform_sdk_cb.h"
#include "sync_data.h"
#include "wristband_global_data.h"
#include "rtl876x_wdg.h"
#include "flash_device.h"
#include "flash_adv_cfg.h"
#include "hub_gsensor.h"
#include "app_flash_config.h"

USER_CALL_BACK app_pre_main_cb = wristband_app_pre_main_cb;
//BOOL_DFU_STATUS_CB user_dfu_status_cb = (BOOL_DFU_STATUS_CB)wristband_app_user_dfu_cb;


void wristband_app_pre_main_cb(void)
{
    if (BIT5 & btaon_fast_read_safe(0x2))
    {
        Pad_Config(BATTERY_CHECK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
                   PAD_OUT_LOW);
    }
}

#if 0
static uint8_t page_num = 0;
static uint32_t image_total_length;
static uint32_t current_recv_length;
bool wristband_app_user_dfu_cb(DFU_PROGRESS_STATUS status, uint32_t length)
{
    if (status == DFU_START)
    {
        image_total_length = length;
    }
    else if (status == DFU_DOING)
    {
        current_recv_length += length;
        if (10 * current_recv_length / image_total_length > page_num)
        {
            page_num = 10 * current_recv_length / image_total_length;
        }
    }
    else if (status == DFU_END)
    {
//      lcd_8080_show_ota(10);
    }
    return true;
}
#endif

bool wristband_wdg_reboot_callback(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason)
{
    DBG_DIRECT("Wristband Test: wdg reboot callback valid");
    uint32_t reason = reset_reason;
    if (wdg_mode == RESET_ALL_EXCEPT_AON) // ota wdg mode, ota reset reason 0x0
    {
        reason = 0xAB;
    }
    ftl_save(&reason, REBOOT_REASON_OFFSET, REBOOT_REASON_SIZE);
    uint8_t prev_bp_lv = 0;
    uint32_t len = sizeof(RtkWristBandSysType_t);
    flash_sw_protect_unlock_by_addr_locked(WRISTBAND_CONFIG_START_ADDR, &prev_bp_lv);
    /*step 1: erase flash for save wristband config*/
    if (flash_erase_locked(FLASH_ERASE_SECTOR, WRISTBAND_CONFIG_START_ADDR) == false)
    {
        DBG_DIRECT("wristband config erase error!");
    }
    /*step 2: save wristband config*/

    if (flash_write_locked(WRISTBAND_CONFIG_START_ADDR, len, (uint8_t *)&RtkWristbandSys) == false)
    {
        DBG_DIRECT("wristband config save error!");
    }
    flash_set_block_protect_locked(prev_bp_lv);
    return true;
}







