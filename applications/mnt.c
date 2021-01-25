/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-12     tyustli      first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#ifdef RT_USING_DFS
#include "dfs_file.h"
#include "dfs_fs.h"
#include "dfs_posix.h"
#endif /* RT_USING_DFS */

#ifdef PKG_USING_FAL
#include "fal.h"
#endif /* PKG_USING_FAL */

#include <drv_flash.h>

#define DBG_ENABLE
#define DBG_TAG     "MNT"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

#define ROMFS_MOUNT_PATH                         "/"
#define FATFS_MOUNT_PATH                         "/user"
#define FATFS_MOUNT_NAME                         "fatfs"

#ifdef RT_USING_DFS

int filesystem_mount(void)
{
#ifdef RT_USING_DFS
    if (dfs_mount(RT_NULL, ROMFS_MOUNT_PATH, "rom", 0, (void *)(REALTK_FLASH_START_ADDRESS)) == 0)
    {
        LOG_I("ROM file system initializated!\n");
    }
    else
    {
        LOG_E("ROM file system initializate failed!\n");
        return -1;
    }
#endif

#ifdef RT_USING_DFS_ELMFAT
    struct rt_device *flash_dev = fal_blk_device_create(FATFS_MOUNT_NAME);

    if (flash_dev == NULL)
    {
        LOG_E("Can't create a block device on '%s' partition.\n", FATFS_MOUNT_NAME);
        return-1;
    }
    else
    {
        LOG_I("Create a block device on the %s partition of flash successful.\n", FATFS_MOUNT_NAME);
    }

    if(rt_device_find(FATFS_MOUNT_NAME) != RT_NULL)
    {
        if (dfs_mount(FATFS_MOUNT_NAME, FATFS_MOUNT_PATH, "elm", 0, 0) == RT_EOK)
        {
            LOG_I("norflash elm filesystem mount to %s success !\r\n", FATFS_MOUNT_PATH);
            int fd = 0;
            fd = open("/user/download", O_DIRECTORY | O_CREAT);
            if (fd >=0) close(fd);
            
            fd = open("/user/apps", O_DIRECTORY | O_CREAT);
            if (fd >=0) close(fd);
            
            fd = open("/user/db", O_DIRECTORY | O_CREAT);
            if (fd >=0) close(fd);
        }
        else
        {
            dfs_mkfs("elm", FATFS_MOUNT_NAME);
            if (dfs_mount(FATFS_MOUNT_NAME, FATFS_MOUNT_PATH, "elm", 0, 0) == RT_EOK)
            {
                LOG_I("norflash elm filesystem mount to %s success !\r\n", FATFS_MOUNT_PATH);
            }
            else
            {
                LOG_E("norflash elm filesystem mount to FATFS_MOUNT_PATH failed!\r\n", FATFS_MOUNT_PATH);
                return -1;
            }
        }
    }
    else
    {
        LOG_E("find filesystem portion failed\r\n");
        return -1;
    }
#endif /* RT_USING_DFS_ELMFAT */

    return 0;
}

#endif /* RT_USING_DFS */

/************************ end of file ****************************/
