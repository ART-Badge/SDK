/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-08-04     tyustli  first version
 */

#include "rtconfig.h"

#ifdef BSP_USING_FLASH
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "drv_flash.h"
#include "string.h"
#include "flash_device.h"

#if defined(PKG_USING_FAL)
#include "fal.h"
#endif

#define DRV_DEBUG
#define LOG_TAG                "drv.flash"
#include <drv_log.h>

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */

int realtek_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size)
{
    if ((addr + size) > REALTEK_FLASH_END_ADDRESS)
    {
        rt_kprintf("read outrange flash size! addr is (0x%p)", (void*)(addr + size));
        return -RT_EINVAL;
    }

    rt_memcpy(buf, (rt_uint8_t *)(addr | 0x4000000), size);
    return size;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */

int realtek_flash_write(rt_uint32_t addr, const uint8_t *buf, size_t size)
{
    if (flash_write_locked(addr, size, (uint8_t *)buf) == false)
    {
        rt_kprintf("realtek data write error!\r\n");
        return 0;
    }

    return size;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */

int realtek_flash_erase(rt_uint32_t addr, size_t size)
{
    rt_uint32_t div_size;
    rt_uint32_t yu_size;

    div_size = size / FLASH_SECTOR_SIZE;
    yu_size = size % FLASH_SECTOR_SIZE;

    rt_uint32_t count_sector; /* Number of sector to erase */
    count_sector = div_size;

    if(count_sector == 0)
    {
        if(yu_size == 0)
            count_sector = 0;
        else
            count_sector = 1;
    }
    else
    {
        if(yu_size == 0)
        {
            count_sector = count_sector;
        }
        else
        {
            count_sector++;
        }
    }

    rt_uint32_t start_sector; /* Start sector in LBA */
    start_sector = addr - REALTK_FLASH_START_ADDRESS;
    div_size = start_sector / FLASH_SECTOR_SIZE;
    yu_size = start_sector % FLASH_SECTOR_SIZE;

    if(div_size == 0)
    {
        start_sector = 0;
    }
    else
    {
        if(yu_size == 0)
        {
            start_sector = div_size;
        }
        else
        {
            start_sector = div_size + 1;
        }
    }

    for (uint32_t i = 0; i < count_sector; i++)
    {
        if (flash_erase_locked(FLASH_ERASE_SECTOR,
                               REALTK_FLASH_START_ADDRESS + start_sector * FLASH_SECTOR_SIZE +  FLASH_SECTOR_SIZE * i) == false)
        {
            rt_kprintf("fs data erase error!");
            return 0;
        }
    }

    return size;
}

#define TEST_DATA_SIZE                           1024
#define TEST_DATA_VALUE                          0x22
rt_uint8_t write_buf[TEST_DATA_SIZE];
rt_uint8_t read_buf[TEST_DATA_SIZE];
const struct fal_partition *fal;

static void fal_test(void)
{
    rt_uint32_t i = 0;

    /* write buffer data */
    for(i = 0; i < sizeof(write_buf); i++)
        write_buf[i] = TEST_DATA_VALUE;

    /* clear read buffer */
    for(i = 0; i < sizeof(read_buf); i++)
        read_buf[i] = 0x00;

    fal = fal_partition_find("gui");

    fal_partition_erase(fal, 0, TEST_DATA_SIZE);
    fal_partition_write(fal, 0, write_buf, TEST_DATA_SIZE);
    fal_partition_read(fal, 0, read_buf, TEST_DATA_SIZE);

    /* check data */
    for(i = 0; i < sizeof(read_buf); i++)
    {
        if(read_buf[i] != TEST_DATA_VALUE)
        {
            rt_kprintf("read data failed i = %d data is 0x%x\r\n", i, read_buf[i]);
            break;
        }
    }

    return;
}


#if defined(PKG_USING_FAL)

static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size);
static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size);
static int fal_flash_erase(long offset, size_t size);

const struct fal_flash_dev realtek_nor_flash = { "nor_flash", REALTK_FLASH_START_ADDRESS, REALTEK_FLASH_SIZE, 
FLASH_SECTOR_SIZE, {NULL, fal_flash_read, fal_flash_write, fal_flash_erase} };

static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size)
{
    return realtek_flash_read(realtek_nor_flash.addr + offset, buf, size);
}

static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size)
{
    return realtek_flash_write(realtek_nor_flash.addr + offset, buf, size);
}

static int fal_flash_erase(long offset, size_t size)
{
    return realtek_flash_erase(realtek_nor_flash.addr + offset, size);
}

#endif /* PKG_USING_FAL */

int flash_driver_test(void)
{
    rt_kprintf("first test start \r\n");
    fal_test();
    rt_kprintf("first test end \r\n");

    rt_kprintf("second test start \r\n");
    fal_test();
    rt_kprintf("second test end \r\n");

    return 0;
}
MSH_CMD_EXPORT(flash_driver_test, flash driver test)

#endif /* BSP_USING_FLASH */

/************************** end of file ******************************/

