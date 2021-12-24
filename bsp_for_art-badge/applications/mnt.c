/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-06-03     tyustli      first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtconfig.h"

#ifdef RT_USING_DFS
    #include <dfs_fs.h>
    #include <dfs_posix.h>
#endif

#if defined(PKG_USING_FAL)
    #include "fal.h"
#endif

#include <drv_flash.h>

#define DBG_TAG             "mnt"
#define DBG_LVL             DBG_INFO
#include <rtdbg.h>

#define FATFS_MOUNT_NAME                         "fatfs"
#define USER_PARTITION_N                         "user"
#define USER_FILE_PATH                          "/user"
#define DATA_FILE_PATH                          "/data"
static int mnt_device_create(const char *parition_name, enum rt_device_class_type type)
{
    int result = RT_EOK;

    struct rt_device *flash_dev = NULL;

    switch (type)
    {
        case RT_Device_Class_Block:
            flash_dev = fal_blk_device_create(parition_name);
            break;

        case RT_Device_Class_MTD:
            flash_dev = fal_mtd_nor_device_create(parition_name);
            break;

        default:
            return -RT_ERROR;
    }

    if (flash_dev == NULL)
    {
        LOG_W("Can't create a block device on '%s' partition.", parition_name);

        return -RT_ERROR;
    }
    else
    {
        LOG_D("Create a block device on the %s partition of flash successfully.", parition_name);
    }

    rt_device_t block_device = RT_NULL;
    block_device = rt_device_find(parition_name);

    if (block_device == RT_NULL)
    {
        result = -RT_ERROR;

        LOG_W("The %s block device not find.", parition_name);
    }

    return result;
}

static void mnt_directory_delete(const char *pathname)
{
    DIR *dir = NULL;
    struct dirent *dirent = NULL;
    char *full_path;

    if (pathname == RT_NULL)
        return;

    full_path = (char *)rt_malloc(DFS_PATH_MAX);

    if (full_path == RT_NULL)
        return;

    dir = opendir(pathname);

    if (dir == RT_NULL)
    {
        rt_free(full_path);
        return;
    }

    while (1)
    {
        dirent = readdir(dir);

        if (dirent == RT_NULL)
            break;

        if (rt_strcmp(".", dirent->d_name) != 0 &&
                rt_strcmp("..", dirent->d_name) != 0)
        {
            rt_sprintf(full_path, "%s/%s", pathname, dirent->d_name);

            if (dirent->d_type == DT_REG)
            {
                if (unlink(full_path) != 0)
                {
                    LOG_W("cannot remove '%s'", full_path);
                }
            }
            else if (dirent->d_type == DT_DIR)
            {
                mnt_directory_delete(full_path);
            }
        }
    }

    closedir(dir);
    rt_free(full_path);

    if (unlink(pathname) != 0)
    {
        LOG_W("cannot remove '%s'", pathname);
    }
}

static int mnt_filesystem_mount(const char *parition_name, const char *path, const char *fstype, enum rt_device_class_type type, const void *data)
{
    int result = RT_EOK;

    result = mnt_device_create(parition_name, type);

    if (result == RT_EOK)
    {
        if (dfs_mount(parition_name, path, fstype, 0, data) != RT_EOK)
        {
            if (rt_strcmp(path, "/") != 0)
            {
                /* Empty files in directory */
                mnt_directory_delete(path);
                mkdir(path, 0);
            }

            dfs_mkfs(fstype, parition_name);

            if (dfs_mount(parition_name, path, fstype, 0, data) != RT_EOK)
            {
                rt_kprintf("%s (%s) parition mount to '%s' failed!\n", fstype, parition_name, path);
                return -RT_ERROR;
            }
        }

        rt_kprintf("%s (%s) parition mount to '%s' successfully.\n", fstype, parition_name, path);
    }
    else
    {
        rt_kprintf("find (%s) portion failed!\n", parition_name);
    }

    return result;
}

int mnt_init(void)
{
    #ifdef RT_USING_DFS_ROMFS

    if (dfs_mount(RT_NULL, "/", "rom", 0, (void *)(REALTK_FLASH_START_ADDRESS)) == 0)
    {
        rt_kprintf("ROM file system initializated successful!\n");
    }
    else
    {
        rt_kprintf("ROM file system initializate failed!\n");
    }

    #endif /* RT_USING_DFS_ROMFS */

    #ifdef RT_USING_DFS_ELMFAT
    mnt_filesystem_mount(FATFS_MOUNT_NAME, DATA_FILE_PATH, "elm", RT_Device_Class_Block, 0);

    if(RT_EOK == mnt_filesystem_mount(USER_PARTITION_N, USER_FILE_PATH, "elm", RT_Device_Class_Block, 0))
    {
        LOG_I("norflash elm filesystem mount to %s success !\r\n", "/user");
        int fd = 0;
        fd = open("/user/download", O_DIRECTORY | O_CREAT);

        if (fd >= 0) close(fd);

        fd = open("/user/apps", O_DIRECTORY | O_CREAT);

        if (fd >= 0) close(fd);

        fd = open("/user/db", O_DIRECTORY | O_CREAT);

        if (fd >= 0) close(fd);
    }
    else
    {
        LOG_E("find filesystem portion failed\r\n");
        return -1;
    }

    #endif /* PKG_USING_LITTLEFS */
    return 1;
}

/********************** end of file *******************/
