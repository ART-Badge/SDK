/*
 * File      : partition.h
 * COPYRIGHT (C) 2016, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-09-20     Bernard      the first version
 * 2019-09-20     Ernest       add partition
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>

#include "partition.h"

//#define PARTITION_DEBUG

#ifdef PARTITION_DEBUG
    #define DEBUG_TRACE         rt_kprintf("[part] "); rt_kprintf
#else
    #define DEBUG_TRACE(...)
#endif

extern uint32_t g_emmc_availiable_size;
struct partition_device
{
    struct rt_device parent;            /* parent block device */

    int sector_size;
    int block_size;
    int block_cnt;
    int pos;

    struct rt_device *block_device;
    const struct rt_partition *partition;
};

/* RT-Thread device interface */
static rt_err_t partition_init(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t partition_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t partition_close(rt_device_t dev)
{
    return RT_EOK;
}

#if RTTHREAD_VERSION >= 30000
    static rt_err_t partition_control(rt_device_t dev, int cmd, void *args)
#else
    static rt_err_t partition_control(rt_device_t dev, rt_uint8_t cmd, void *args)
#endif
{
    struct partition_device *part;

    part = (struct partition_device *) dev;
    RT_ASSERT(part != RT_NULL);

    if (cmd == RT_DEVICE_CTRL_BLK_GETGEOME)
    {
        struct rt_device_blk_geometry *geometry;

        geometry = (struct rt_device_blk_geometry *)args;
        if (geometry == RT_NULL) return -RT_ERROR;

        geometry->bytes_per_sector = part->sector_size;
        geometry->sector_count = part->block_cnt;
        geometry->block_size = part->block_size;
    }

    return RT_EOK;
}

static rt_size_t partition_read(rt_device_t dev,
                                rt_off_t pos,
                                void *buffer,
                                rt_size_t size)
{
    struct partition_device *part;

    part = (struct partition_device *) dev;
    RT_ASSERT(part != RT_NULL);

    DEBUG_TRACE("read block: %d, size=%d\n", pos + part->pos, size);

    return rt_device_read(part->block_device, pos + part->pos, buffer, size);
}

static rt_size_t partition_write(rt_device_t dev,
                                 rt_off_t pos,
                                 const void *buffer,
                                 rt_size_t size)
{
    struct partition_device *part;

    part = (struct partition_device *) dev;
    RT_ASSERT(part != RT_NULL);

    if (part->partition->flags & PARTITION_WRITEABLE)
    {
        DEBUG_TRACE("write block: %d, size=%d\n", pos + part->pos, size);

        return rt_device_write(part->block_device, pos + part->pos, buffer, size);
    }

    DEBUG_TRACE("read-only partition!\n");
    return 0;
}

int rt_partition_init(const char *flash_device, const struct rt_partition *parts, rt_size_t num)
{
    struct rt_device *device;
    struct partition_device *part_dev;
    const struct rt_partition *part;
    struct rt_device_blk_geometry geometry;

    device = (struct rt_device *)rt_device_find(flash_device);
    if (device == RT_NULL)
    {
        DEBUG_TRACE("block device %s not found!\r\n", flash_device);
        return -RT_ENOSYS;
    }

    /* open the block device */
    rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
    /* get block device geometry */
    rt_device_control(device, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);

    /* create each partition device */
    while (num --)
    {
        part = parts ++;
        part_dev = (struct partition_device *)rt_malloc(sizeof(struct partition_device));
        if (part_dev)
        {
            part_dev->block_device = device;
            part_dev->partition = part;
            part_dev->sector_size = geometry.bytes_per_sector;
            part_dev->block_size  = geometry.block_size;

            part_dev->block_cnt = part_dev->partition->size / part_dev->sector_size;
            part_dev->pos = part_dev->partition->offset / part_dev->sector_size;

            /* register device */
            part_dev->parent.type    = RT_Device_Class_Block;
            part_dev->parent.init    = partition_init;
            part_dev->parent.open    = partition_open;
            part_dev->parent.close   = partition_close;
            part_dev->parent.read    = partition_read;
            part_dev->parent.write   = partition_write;
            part_dev->parent.control = partition_control;
            /* no private */
            part_dev->parent.user_data = RT_NULL;

            DEBUG_TRACE("add partition: %s\n", part->name);
            rt_device_register(RT_DEVICE(part_dev), part->name,
                               RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
        }
    }

    return RT_EOK;
}

int partition_emmc(void)
{
    const char flash_device[] = "sd0";
    struct rt_partition *parts = calloc(4, sizeof(struct rt_partition));

    if (MMCSD_HOST_PLUGED != mmcsd_wait_cd_changed(RT_TICK_PER_SECOND * 8))
    {
        DEBUG_TRACE("MMCSD_HOST_PLUGED timeout!");
        return 0;
    }

    parts[0].name = "system";
    parts[0].offset = 0;
    parts[0].size =  1024 * 1024 * 300; // 300MB
    parts[0].flags = PARTITION_WRITEABLE;

    parts[1].name = "update";
    parts[1].offset = parts[0].offset + parts[0].size;
    parts[1].size =  1024 * 1024 * 300; // 300MB
    parts[1].flags = PARTITION_WRITEABLE;

    parts[2].name = "user";
    parts[2].offset = parts[1].offset + parts[1].size;
    parts[2].size =  1024 * 1024 * 300; // 300MB
    parts[2].flags = PARTITION_WRITEABLE;

    parts[3].name = "sdcard";
    parts[3].offset = parts[2].offset + parts[2].size;
    parts[3].size =  1024 * 1024 * 1024 - parts[3].offset; // 1024MB
    parts[3].flags = PARTITION_WRITEABLE;

    return rt_partition_init(flash_device, parts, 4);
}
INIT_APP_EXPORT(partition_emmc);
