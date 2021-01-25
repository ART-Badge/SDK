
#pragma once

#include <map>
#include <string>

#include "js_app_x.h"

using namespace Persimmon;

typedef std::map<std::string, JsAppX *> AppManage;

class AppManagement
{
public:
    ~AppManagement();

    static AppManagement *instance(void);

    void launcher(void);
    bool launcher(const char *uri);
    void exit(const char* name = NULL);

    static const std::string handleUrl(const char* url);
    void AppManageAdd(JsAppX *app, const char *url);
    void AppManageRm(const char *url);
    JsAppX *AppManageGet(const char *url);

    void launcherApp(JsAppX *app);
    void activateApp(JsAppX *app = NULL);
    JsAppX *activate(void);

    js_value_t launchArgs(void);
    js_value_t pageArgs(void);

    void checkAppPage(void);

    void listAppInstalledInit(bool reset = false);
    js_value_t listAppInstalled(js_value_t list, const char* name = 0);
    js_value_t listAppRunning(void);
    js_value_t getInfo(const char *name);
    js_value_t getListAppInstalled(void);

    void listAppInstalledDump(void);
    void listAppRunningDump(void);

    int systemAppInstall(const char* prc_path, bool check = false);
    int appInstall(const char* prc_path, bool check = false);
    int appUninstall(const char* name, bool check = false);

private:
    AppManagement();

    static AppManagement *_instance;
    AppManage _AppManage;
    JsAppX *_launcherApp, *_activateApp;

    js_value_t _listAppInstalled, _launchArgs, _pageArgs;
};
