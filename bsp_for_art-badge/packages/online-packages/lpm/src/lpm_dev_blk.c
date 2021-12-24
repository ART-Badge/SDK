/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020.12.11     wanghaijing  first version
 */

#include <rtdevice.h>
#include <string.h>
#include "lpm.h"

#ifndef RT_FIOGETXIPADDR
    #define RT_FIOGETXIPADDR 0x52540001U
#endif

int lpm_blk_dev_init(struct lpm_dev *dev)
{
    RT_ASSERT(dev);

    return RT_EOK;
}

int lpm_blk_dev_read(struct lpm_dev *dev, uint32_t sector_no, uint8_t *buf, size_t size)
{
    rt_device_t device;

    RT_ASSERT(dev);

    device = ((struct rt_lpm_device*) dev)->rt_device;

    return rt_device_read(device, sector_no, buf, size);
}

int lpm_blk_dev_write(struct lpm_dev *dev, uint32_t sector_no, const uint8_t *buf, size_t size)
{
    rt_device_t device;

    RT_ASSERT(dev);

    device = ((struct rt_lpm_device*) dev)->rt_device;

    return rt_device_write(device, sector_no, buf, size);
}

int lpm_blk_dev_erase(struct lpm_dev *dev, uint32_t block_no, size_t num)
{
    RT_ASSERT(dev);

    return RT_EOK;
}

int lpm_blk_dev_control(struct lpm_dev *dev, int cmd, void *arg)
{
    rt_device_t device;
    
    RT_ASSERT(dev);
    
    device = ((struct rt_lpm_device*) dev)->rt_device;

    return rt_device_control(device, cmd, arg);
}

extern struct lpm lpm;

static struct lpm_dev_ops lpm_dev_ops = {
        .init  = lpm_blk_dev_init,
        .erase = lpm_blk_dev_erase,
        .read  = lpm_blk_dev_read,
        .write = lpm_blk_dev_write,
        .control = lpm_blk_dev_control
};

int lpm_dev_blk_append(struct rt_device *dev)
{
    rt_err_t result;
    struct rt_device_blk_geometry geometry;
    struct lpm_dev *lpm_blk_dev;
    uint32_t phy_start_addr = 0xFFFFFFFF;

    lpm_blk_dev = rt_malloc(sizeof(struct rt_lpm_device));
    if (lpm_blk_dev == RT_NULL)
    {
        return -RT_ERROR;
    }

    if ((dev->flag & RT_DEVICE_FLAG_RDWR) == RT_DEVICE_FLAG_RDWR)
    {
        result = rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
        if (result != RT_EOK && result != -RT_EBUSY)
        {
            return result;
        }
        rt_memset(&geometry, 0, sizeof(geometry));
        result = rt_device_control(dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
        if (result != RT_EOK)
        {
            rt_kprintf("device : %s cmd RT_DEVICE_CTRL_BLK_GETGEOME failed.\r\n");
            return result;
        }
        //rt_kprintf("device info:\r\n");
        //rt_kprintf("sector  size : %d byte\r\n", geometry.bytes_per_sector);
        //rt_kprintf("sector count : %d \r\n",     geometry.sector_count);
        //rt_kprintf("block   size : %d byte\r\n", geometry.block_size);

        result = rt_device_control(dev, RT_FIOGETXIPADDR, &phy_start_addr);
        if (result != RT_EOK)
        {
            rt_kprintf("device : cmd RT_FIOGETXIPADDR failed.\r\n");
            return result;
        }
        //rt_kprintf("block   addr : 0x%X \r\n", phy_start_addr);
    }

    rt_memset(lpm_blk_dev->name, 0, LPM_NAME_MAX);
    rt_strncpy(lpm_blk_dev->name, dev->parent.name, RT_NAME_MAX);

    lpm_blk_dev->sector_count = geometry.sector_count;
    lpm_blk_dev->bytes_per_sector = geometry.bytes_per_sector;
    lpm_blk_dev->block_size = geometry.block_size;
    lpm_blk_dev->phy_start_addr = phy_start_addr;
    lpm_blk_dev->dev_list.next = RT_NULL;
    lpm_blk_dev->part_list.next = RT_NULL;
    lpm_blk_dev->ops = &lpm_dev_ops;

    ((struct rt_lpm_device*) lpm_blk_dev)->rt_device = dev;

    lpm_dev_append(lpm_blk_dev);

    return RT_EOK;
}

static rt_size_t lpm_blk_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    int ret = 0;

    ret = lpm_partition_read(dev->user_data, pos , buffer, size);

    if (ret != size)
    {
        ret = 0;
    }
    else
    {
        ret = size;
    }

    return ret;
}

static rt_size_t lpm_blk_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    int ret = 0;

    ret = lpm_partition_write(dev->user_data, pos , buffer, size);

    if (ret != size)
    {
        ret = 0;
    }
    else
    {
        ret = size;
    }

    return ret;
}

static rt_err_t lpm_blk_control(rt_device_t dev, int cmd, void *args)
{
    if (cmd == RT_DEVICE_CTRL_BLK_GETGEOME)
    {
        struct rt_device_blk_geometry *geometry;

        geometry = (struct rt_device_blk_geometry *) args;
        if (geometry == RT_NULL)
        {
            return -RT_ERROR;
        }

        geometry->block_size =       ((struct lpm_partition*)(dev->user_data))->dev->block_size;
        geometry->bytes_per_sector = ((struct lpm_partition*)(dev->user_data))->dev->bytes_per_sector;
        geometry->sector_count =     ((struct lpm_partition*)(dev->user_data))->size *              \
                                     ((((struct lpm_partition*)(dev->user_data))->dev->block_size) / \
                                     ((struct lpm_partition*)(dev->user_data))->dev->bytes_per_sector);
    }
    else if (cmd == RT_DEVICE_CTRL_BLK_ERASE)
    {
        //todo
    }
    else if (cmd == RT_FIOGETXIPADDR)
    {
        *(uint32_t *) args = ((struct lpm_partition*)(dev->user_data))->dev->phy_start_addr;
    }

    return RT_EOK;
}

struct rt_device *lpm_blk_device_create(const char *dev_name, const char *parition_name)
{
    struct lpm_partition *lpm_par;
    struct rt_device *rt_dev;

    /*find lpm partition*/
    lpm_par = lpm_partition_find(dev_name, parition_name);
    if (lpm_par == RT_NULL)
    {
        rt_kprintf("Error: the partition name (%s) is not found.", parition_name);
        return RT_NULL;
    }

    rt_dev = (struct rt_device*) rt_malloc(sizeof(struct rt_device));

    rt_memset(rt_dev->parent.name, 0, RT_NAME_MAX);
    rt_strncpy(rt_dev->parent.name, parition_name, RT_NAME_MAX);

    rt_dev->type = RT_Device_Class_Block;

    rt_dev->init    = RT_NULL;
    rt_dev->open    = RT_NULL;
    rt_dev->close   = RT_NULL;
    rt_dev->read    = lpm_blk_read;
    rt_dev->write   = lpm_blk_write;
    rt_dev->control = lpm_blk_control;

    rt_dev->user_data = lpm_par;

    rt_kprintf("The LPM block device (%s) created successfully \r\n", parition_name);
    rt_device_register(rt_dev, parition_name, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);

    return rt_dev;
}
