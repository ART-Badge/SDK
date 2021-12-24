/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_module_touch_gesture_button.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/
#define __TOUCH_PANEL_CAPACITY__
#ifdef __TOUCH_PANEL_CAPACITY__

#include "board.h"
#include "rtl876x_i2c.h"
#include "ctp_hynitron_ext.h"
#include "capacitive_hynitron_cst816s_update.h"
#include "os_sched.h"
#include "rtl876x_module_touch_gesture_button.h"
#include "os_sync.h"
#include "platform_utils.h"
#include <string.h>
#include "app_msg.h"
#include "app_task.h"
#include "rtl876x_module_data.h"
#if CTP_HYNITRON_EXT==1
#define REG_LEN_1B  1
#define REG_LEN_2B  2

#if CTP_HYNITRON_EXT_CST816S_UPDATE == 1
/*****************************************************************/
// For CSK0xx update
/*
 *
 */

void hctp_write(uint8_t *buf, uint32_t len)
{
    uint32_t time_out = 0xFFFFF;
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_TFE) == RESET) && (time_out-- != 0));
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_MST_ACTIVITY) == SET) && (time_out-- != 0));

    I2C_SetSlaveAddress(TOUCH_I2C_BUS, 0x6A);
    I2C_MasterWrite(TOUCH_I2C_BUS, buf, len);

    time_out = 0xFFFFF;
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_TFE) == RESET) && (time_out-- != 0));
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_MST_ACTIVITY) == SET) && (time_out-- != 0));
}
void hctp_read(uint8_t *write_buf, uint8_t write_len, uint8_t *readbuf, uint8_t read_len)
{
    uint32_t time_out = 0xFFFFF;

    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_TFE) == RESET) && (time_out-- != 0));
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_MST_ACTIVITY) == SET) && (time_out-- != 0));
    I2C_SetSlaveAddress(TOUCH_I2C_BUS, 0x6A);
    I2C_Status status = I2C_RepeatRead(TOUCH_I2C_BUS, write_buf, write_len, readbuf, read_len);

    time_out = 0xFFFFF;
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_TFE) == RESET) && (time_out-- != 0));
    while ((I2C_GetFlagState(TOUCH_I2C_BUS, I2C_FLAG_MST_ACTIVITY) == SET) && (time_out-- != 0));
}
static bool cst816s_enter_bootmode(void)
{
    uint8_t retrycnt = 10;
    while (retrycnt--)
    {
        uint8_t buf[3];
        uint8_t cmd = 0;
        buf[0] = 0xA0;
        buf[1] = 0x01;
        buf[2] = 0xAB;
        hctp_write(buf, 3);
        platform_delay_ms(4);

        buf[0] = 0xA0;
        buf[1] = 0x03;
        hctp_read(buf, 2, &cmd, 1);
        if (cmd == 0x55)
        {
            return true;
        }
        else
        {
            platform_delay_ms(4); // 4ms
            continue;
        }
    }
    return false;
}
/*
 *
 */
static bool cst816s_update(uint16_t startAddr, uint16_t len, uint8_t *src)
{
    uint16_t sum_len = 0;
    uint8_t cmd[10];
    uint8_t buf[4];

    if (cst816s_enter_bootmode() == false)
    {
        return false;
    }

#define PER_LEN 512
    uint8_t buffer[PER_LEN + 2] = {0};
    do
    {
        DBG_DIRECT("startAddr = %d,  src = %d, sum_len = %d", startAddr, src, sum_len);
        if (sum_len >= len)
        {
            return true;
        }

        // send address
        buf[0] = 0xA0;
        buf[1] = 0x14;
        buf[2] = startAddr & 0xFF;
        buf[3] = startAddr >> 8;

        hctp_write(buf, 4);

        buffer[0] = 0xA0;
        buffer[1] = 0x18;
        memcpy(buffer + 2, src, PER_LEN);
        hctp_write(buffer, PER_LEN + 2);

        buf[0] = 0xA0;
        buf[1] = 0x04;
        buf[2] = 0xEE;
        hctp_write(buf, 3);


        platform_delay_ms(100);
        {
            uint8_t retrycnt = 50;
            while (retrycnt--)
            {
                cmd[0] = 0;
                buf[0] = 0xA0;
                buf[1] = 0x05;
                hctp_read(buf, 2, cmd, 1);
                if (cmd[0] == 0x55)
                {
                    // success
                    break;
                }
                platform_delay_ms(10);
            }
        }
        startAddr += PER_LEN;
        src += PER_LEN;
        sum_len += PER_LEN;
    }
    while (len);

    // exit program mode

    buf[0] = 0xA0;
    buf[1] = 0x03;
    buf[2] = 0x00;
    hctp_write(buf, 3);
    return true;
}
/*
 *
 */
static uint32_t cst816s_read_checksum(uint16_t startAddr, uint16_t len)
{
    union
    {
        uint16_t sum;
        uint8_t buf[2];
    } checksum;
    uint8_t buf[3];

    if (cst816s_enter_bootmode() == false)
    {
        return false;
    }

    buf[0] = 0xA0;
    buf[1] = 0x03;
    buf[2] = 0;
    hctp_write(buf, 3);
    platform_delay_ms(500);

    checksum.sum = 0;
    buf[0] = 0xA0;
    buf[1] = 0x08;
    hctp_read(buf, 2, checksum.buf, 2);
    APP_PRINT_INFO1("checksum 1 = 0x%x", checksum.sum);
    return checksum.sum;
}
#endif

/*****************************************************************/
// common
/*
 *
 */
bool ctp_hynitron_update(void)
{
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    platform_delay_ms(10);
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    platform_delay_ms(8);
#if CTP_HYNITRON_EXT_CST816S_UPDATE==1
    if (cst816s_enter_bootmode() == true)
    {
        if (sizeof(app_bin) > 10)
        {
            uint16_t startAddr = app_bin[1];
            uint16_t length = app_bin[3];
            uint16_t checksum = app_bin[5];
            startAddr <<= 8;
            startAddr |= app_bin[0];
            length <<= 8;
            length |= app_bin[2];
            checksum <<= 8;
            checksum |= app_bin[4];
            APP_PRINT_INFO2("checksum 0 = 0x%x, length = %d", checksum, length);
//          if(cst816s_read_checksum(startAddr, length) != checksum)
//          {
            cst816s_read_checksum(startAddr, length);
            cst816s_update(startAddr, length, (uint8_t *)(app_bin + 6));
            cst816s_read_checksum(startAddr, length);
            Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
            os_delay(1);
            Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
            os_delay(30);
            static uint8_t chip_id = 0x59;
            cst0x6_get_chip_id(&chip_id);
//          }

        }
        return true;
    }
#endif
    return false;
}

void hctp_update_test()
{
    ctp_hynitron_update();
}

#endif  //CTP_HYNITRON_EXT==1

#endif //defined(__TOUCH_PANEL_CAPACITY__)

