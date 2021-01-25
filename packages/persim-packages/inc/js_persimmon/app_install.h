/*
 * File      : app_install.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2020, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-27     realthread   first version
 */
#ifndef APP_INSTALL_H__
#define APP_INSTALL_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

int system_app_install(const char* prc_path);
int app_install(const char* prc_path);
int app_uninstall(const char* app_name);
char *app_install_name(const char* prc_path);

#ifdef __cplusplus
}
#endif

#endif
