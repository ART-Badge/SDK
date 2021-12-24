/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-10     tyx          first version.
 */

#include <rtthread.h>
#include "kdb_core.h"
#include "kdb_sys.h"

struct kdb_cmd_des
{
    const char *cmd;
    const char *help;
    int (*fun)(int argc, char *argv[]);
};

static int kdb_msh_help(int argc, char *argv[]);
static int kdb_msh_start(int argc, char *argv[]);
static int kdb_msh_dump(int argc, char *argv[]);
static int kdb_msh_stop(int argc, char *argv[]);
static int kdb_msh_init(int argc, char *argv[]);

static const struct kdb_cmd_des _kdb_cmd[] = 
{
    {"-h", "print help", kdb_msh_help},
    {"-s", "start take notes", kdb_msh_start},
    {"-d", "dump take notes", kdb_msh_dump},
    {"--init", "initialize kernel debug tool", kdb_msh_init},
    {"--stop", "stop takle notes", kdb_msh_stop},
};

static int kdb_msh_help(int argc, char *argv[])
{
    int i;

    rt_kprintf("%s\n", argv[0]);
    for (i = 0; i < sizeof(_kdb_cmd) / sizeof(_kdb_cmd[0]); i++)
    {
        rt_kprintf("%s %-8s  des: %s\n", argv[0], _kdb_cmd[i].cmd, _kdb_cmd[i].help);
    }
    return 0;
}

static int kdb_msh_init(int argc, char *argv[])
{
    if (argc != 2)
        return -1;

    kdb_sys_init();
    rt_kprintf("Kernel debug tool has initialized.\n");
    return 0;
}

static int kdb_msh_start(int argc, char *argv[])
{
    if (argc != 2)
        return -1;

    kdb_sys_start();
    rt_kprintf("Kernel debug tool starts...\n");
    return 0;
}

static int kdb_msh_dump(int argc, char *argv[])
{
    if (argc != 2)
        return -1;

    kdb_sys_dump();
    return 0;
}

static int kdb_msh_stop(int argc, char *argv[])
{
    if (argc != 2)
        return -1;

    kdb_sys_stop();
    rt_kprintf("Kernel debug tool stops.\n");
    return 0;
}

static int kdb_msh(int argc, char *argv[])
{
    int i, result;

    if (argc < 2)
    {
        kdb_msh_help(argc, argv);
        return 0;
    }

    for (i = 0; i < sizeof(_kdb_cmd) / sizeof(_kdb_cmd[0]); i++)
    {
        if (rt_strcmp(_kdb_cmd[i].cmd, argv[1]) == 0)
        {
            result = _kdb_cmd[i].fun(argc, argv);
            break;
        }
    }

    if (result != 0)
    {
        kdb_msh_help(argc, argv);
    }

    return 0;
}
MSH_CMD_EXPORT_ALIAS(kdb_msh, kdb, print to memory.);
