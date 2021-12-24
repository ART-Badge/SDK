/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     ChenYong     first version
 */

#include <rtthread.h>
#include <logmgr.h>

#ifdef LOGMGR_USING_ULOG_FILE
#include <ulog_file.h>
#endif

#define DBG_TAG "logmgr"
#define DBG_LVL DBG_LOG
#include <ulog.h>

static rt_bool_t is_init = RT_FALSE;

extern int logmgr_abort_init(void);

/* logmgr initialize */
int logmgr_init(void)
{
    if (is_init)
    {
        LOG_E("logmgr is already initialized");
        return -1;
    }

#ifdef LOGMGR_USING_ULOG_FILE
    ulog_file_backend_init();
#endif
#ifdef LOGMGR_USING_ABORT
    logmgr_abort_init();
#endif

    is_init = RT_TRUE;
    LOG_I("logmgr (v%s) initialized success.", ULOGMGR_SW_VERSION);
    return 0;
}
#ifdef LOGMGR_AUTO_INIT
INIT_APP_EXPORT(logmgr_init);
#endif

/* logmgr deinitialize */
int logmgr_deinit(void)
{
    if (is_init == RT_FALSE)
    {
        return -1;
    }

#ifdef PKG_USING_ULOG_FILE
    ulog_file_backend_deinit();
#endif

    return 0;
}
