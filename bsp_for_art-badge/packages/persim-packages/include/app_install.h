/*
 * File      : app_install.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2020, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-27     realthread   first version
 */
#pragma once

#include <jsvm/jsvm.h>
#include <pm_file.h>

void AppInstall(const JsValue &arg);
void AppInstallSystem(const JsValue &arg);

bool AppInstallSync(const char *prc_path);
bool AppInstallSystemSync(const char *prc_path);
bool AppUninstallSync(const char *app_id);

#ifdef GUIENGINE_USING_RES
bool app_install_icon(const persim::ResPack &res, const String &url, String icon);
#endif

bool app_install_deploy(const String &url, const String &appId);
bool app_uninstall(const String &url, const String &appId);
