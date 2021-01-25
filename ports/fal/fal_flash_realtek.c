/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-12-15     tyustli       first version
 */

#include <fal.h>
#include <fal_def.h>

#define SRAM_END_ADDRESS   ((uint32_t)(20* 1024))

static int psm_init(void);
static int psm_read(long offset, uint8_t *buf, size_t size);
static int psm_write(long offset, const uint8_t *buf, size_t size);
static int psm_erase(long offset,  size_t size);

struct fal_flash_dev sram0 =
{
    .name       = "sram0",
    .addr       = 0,
    .len        = 20 * 1024,
    .blk_size   = 512,//65532,//128*1024,
    .ops        = {psm_init, psm_read, psm_write, psm_erase},
    .write_gran = 8
};

static int psm_init(void)
{
    uint8_t *temp = (uint8_t *)rt_malloc(20 * 1024);
    if(temp == NULL)
    {
        rt_kprintf("sram malloc filed!!!\r\n");
        return -1;
    }
    else
    {
        sram0.addr  = (uint32_t)temp;
        return 0;
    }
    ;
}
static int psm_read(long offset, uint8_t *buf, size_t size)
{
    uint32_t addr = sram0.addr + offset;

    if ((offset + size) > SRAM_END_ADDRESS )
    {
        rt_kprintf("Erase err flash address!!!\r\n");
        return -1;
    }
    else
    {
        rt_memcpy(buf, (uint8_t *)addr, size);

    }
    return size;


}
static int psm_write(long offset, const uint8_t *buf, size_t size)
{
    uint32_t addr = sram0.addr + offset;

    if ((offset + size) > SRAM_END_ADDRESS)
    {
        rt_kprintf("Erase err flash address!!!\r\n");
        return -1;
    }
    else
    {
        rt_memcpy((uint8_t*)addr, buf, size);
    }
    return size;
}
static int psm_erase(long offset, size_t size)
{
    uint32_t addr = sram0.addr + offset;

    if ((offset + size) > SRAM_END_ADDRESS)
    {
        rt_kprintf("Erase err flash address!!!\r\n");
        return -1;
    }
    else
    {
        rt_memset((uint8_t *)addr, 0XFF, size);
    }
    return size;
}

/*********************** end of file ***********************/
