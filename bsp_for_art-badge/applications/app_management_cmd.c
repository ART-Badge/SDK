/*
 * File      : app_management_cmd.c
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-26     realthread   the first version
 */
#include "rtthread.h"

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <api/js_callback.h>

extern int sys_app_install(const char* prc);
extern int user_app_install(const char* prc);
extern int user_app_uninstall(const char* name);
extern void app_exit(const char* name);
extern void list_app_installed(void);
extern void list_app_running(void);

static void sys_app_install_callback(void* parameter)
{
    sys_app_install((const char*)parameter);
    RT_PM_FREE(parameter);
}

rt_bool_t __sys_app_install(const char* uri)
{
    rt_bool_t ret = RT_FALSE;
    char* parameter = uri ? rt_strdup(uri) : 0;
    ret = js_callback_send(sys_app_install_callback, (void*)parameter);

    if (!ret)
    {
        RT_PM_FREE(parameter);
    }

    return ret;
}

static void user_app_install_callback(void* parameter)
{
    user_app_install((const char*)parameter);
    RT_PM_FREE(parameter);
}

rt_bool_t __user_app_install(const char* uri)
{
    rt_bool_t ret = RT_FALSE;
    char* parameter = uri ? rt_strdup(uri) : 0;
    ret = js_callback_send(user_app_install_callback, (void*)parameter);

    if (!ret)
    {
        RT_PM_FREE(parameter);
    }

    return ret;
}

static void user_app_uninstall_callback(void* parameter)
{
    user_app_uninstall((const char*)parameter);
    RT_PM_FREE(parameter);
}

rt_bool_t __user_app_uninstall(const char* uri)
{
    rt_bool_t ret = RT_FALSE;
    char* parameter = uri ? rt_strdup(uri) : 0;
    ret = js_callback_send(user_app_uninstall_callback, (void*)parameter);

    if (!ret)
    {
        RT_PM_FREE(parameter);
    }

    return ret;
}

static void app_exit_callback(void* parameter)
{
    app_exit((const char*)parameter);
    RT_PM_FREE(parameter);
}

rt_bool_t __app_exit(const char* uri)
{
    rt_bool_t ret = RT_FALSE;
    char* parameter = uri ? rt_strdup(uri) : 0;
    ret = js_callback_send(app_exit_callback, (void*)parameter);

    if (!ret)
    {
        RT_PM_FREE(parameter);
    }

    return ret;
}

static int _sys_app_install(int argc, char** argv)
{
    if (argc > 1)
        __sys_app_install(argv[1]);

    return 0;
}
MSH_CMD_EXPORT_ALIAS(_sys_app_install, app_install_sys, system app install);

static int _user_app_install(int argc, char** argv)
{
    if (argc > 1)
        user_app_install(argv[1]);

    return 0;
}
MSH_CMD_EXPORT_ALIAS(_user_app_install, app_install, user app install);

static int _user_app_uninstall(int argc, char** argv)
{
    if (argc > 1)
        user_app_uninstall(argv[1]);

    return 0;
}
MSH_CMD_EXPORT_ALIAS(_user_app_uninstall, app_uninstall, user app uninstall);

static int _app_exit(int argc, char** argv)
{
    if (argc > 1)
        app_exit(argv[1]);

    return 0;
}
MSH_CMD_EXPORT_ALIAS(_app_exit, app_exit, app exit);

static int _list_app_installed(int argc, char** argv)
{
    list_app_installed();
    return 0;
}
MSH_CMD_EXPORT_ALIAS(list_app_installed, app_installed_dump, dump app installed list);

static int _list_app_running(int argc, char** argv)
{
    list_app_running();
    return 0;
}
MSH_CMD_EXPORT_ALIAS(_list_app_running, app_running_dump, dump app running list);

#endif
