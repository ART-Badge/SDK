/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-07     ChenYong     first version
 */

#include <rtthread.h>
#include <dfs_posix.h>
#include <dfs.h>

#include <ulog.h>
#include <ulog_file.h>

#define ULOG_FILE_BE_NAME    "file"

#ifndef ULOG_FILE_ROOT_PATH
#define ULOG_FILE_ROOT_PATH  "/logs"
#endif
#ifndef ULOG_FILE_NAME_BASE
#define ULOG_FILE_NAME_BASE  "ulog.log"
#endif

#ifndef ULOG_FILE_MAX_NUM
#define ULOG_FILE_MAX_NUM    5
#endif
#ifndef ULOG_FILE_MAX_SIZE
#define ULOG_FILE_MAX_SIZE   (1024 * 512)
#endif

#define ULOG_FILE_PATH_LEN   128

#if defined(ULOG_ASYNC_OUTPUT_THREAD_STACK) && (ULOG_ASYNC_OUTPUT_THREAD_STACK < 2048)
#error "The value of ULOG_ASYNC_OUTPUT_THREAD_STACK must be greater than 2048."
#endif

static struct ulog_backend ulog_file;
static char g_file_path[ULOG_FILE_PATH_LEN] = {0};
static int g_file_fd = -1;

/* rotate the log file xxx.log.n-1 => xxx.log.n, and xxx.log => xxx.log.0 */
static rt_bool_t ulog_file_rotate(void)
{
#define SUFFIX_LEN          10
    /* mv xxx.log.n-1 => xxx.log.n, and xxx.log => xxx.log.0 */
    static char old_path[ULOG_FILE_PATH_LEN], new_path[ULOG_FILE_PATH_LEN];
    int index = 0, err = 0, file_fd = 0;
    size_t base_len = 0;
    rt_bool_t result = RT_FALSE;

    rt_memcpy(old_path, g_file_path, ULOG_FILE_PATH_LEN);
    rt_memcpy(new_path, g_file_path, ULOG_FILE_PATH_LEN);
    base_len = rt_strlen(ULOG_FILE_ROOT_PATH) + rt_strlen(ULOG_FILE_NAME_BASE) + 1;

    if (g_file_fd >= 0)
    {
        close(g_file_fd);
    }

    for (index = ULOG_FILE_MAX_NUM - 2; index >= 0; --index)
    {
        rt_snprintf(old_path + base_len, SUFFIX_LEN, index ? ".%d" : "", index - 1);
        rt_snprintf(new_path + base_len, SUFFIX_LEN, ".%d", index);
        /* remove the old file */
        if ((file_fd = open(new_path, O_RDONLY)) >= 0)
        {
            close(file_fd);
            unlink(new_path);
        }
        /* change the new log file to old file name */
        if ((file_fd = open(old_path , O_RDONLY)) >= 0)
        {
            close(file_fd);
            err = rename(old_path, new_path);
        }

        if (err < 0)
        {
            result = RT_FALSE;
            goto __exit;
        }
    }

__exit:
    /* reopen the file */
    g_file_fd = open(g_file_path, O_CREAT | O_RDWR | O_APPEND);

    return result;
}

static void ulog_file_backend_output(struct ulog_backend *backend, rt_uint32_t level,
            const char *tag, rt_bool_t is_raw, const char *log, size_t len)
{
    size_t file_size = 0;

    /* check log file directory  */
    if (access(ULOG_FILE_ROOT_PATH, 0) < 0)
    {
        mkdir(ULOG_FILE_ROOT_PATH, 0);
    }

    if (g_file_fd < 0)
    {
        rt_snprintf(g_file_path, ULOG_FILE_PATH_LEN, "%s/%s", ULOG_FILE_ROOT_PATH, ULOG_FILE_NAME_BASE);
        g_file_fd = open(g_file_path, O_CREAT | O_RDWR);
        if (g_file_fd < 0)
        {
            rt_kprintf("ulog file(%s) open failed.", g_file_path);
            return;
        }
    }

    file_size = lseek(g_file_fd, 0, SEEK_END);
    if (file_size > ULOG_FILE_MAX_SIZE)
    {
        if (!ulog_file_rotate())
        {
            return;
        }
    }

    write(g_file_fd, log, len);
    /* flush file cache */
    fsync(g_file_fd);
}

/* initialize the ulog file backend */
int ulog_file_backend_init(void)
{
    ulog_file.output = ulog_file_backend_output;
    ulog_backend_register(&ulog_file, ULOG_FILE_BE_NAME, RT_FALSE);
    return 0;
}

/* uninitialize the ulog file backend */
int ulog_file_backend_deinit(void)
{
    if (g_file_fd >= 0)
    {
        close(g_file_fd);
        g_file_fd = -1;
    }
    ulog_backend_unregister(&ulog_file);
    return 0;
}
