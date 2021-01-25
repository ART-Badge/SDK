/*
 * File      : app_launch_cmd.cpp
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-11     realthread   the first version
 */
#include "rtthread.h"

#ifdef RT_USING_FINSH
#include <finsh.h>

extern rt_bool_t app_launch(const char *uri);

static int _app_launch(int argc, char** argv)
{
    return app_launch(argv[1]);
}
MSH_CMD_EXPORT_ALIAS(_app_launch, app_launch, app launch by event);
#endif
