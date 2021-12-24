
#pragma once

#include <map>

#include "js_app_x.h"
#include <pm_animator.h>

USING_PERSIMMON_NAMESPACE

typedef std::map<String, JsAppX *> AppManage;

class TransitionsAnimator;

class AppManagement
{
public:
    virtual ~AppManagement();

    static AppManagement *instance();

    void launcher();
    bool launch(const Uri &uri);
    bool launch(const String &uri);
    void exit(const char *app_id = nullptr, bool check = false);
    void hide(void);
    void appStackRm(JsAppX *app);
    JsAppX *appStackLast(void);
    void appTop(JsAppX *app);

    void insert(JsAppX *app, const String &url);
    void remove(const JsAppX *app, const String &url);
    JsAppX *AppManageGet(const String &url);

    void setLauncher(JsAppX *app, bool isLauncher);
    void setActivate(JsAppX *app = nullptr);
    JsAppX *activate() const { return _activateApp; }

    const JsValue &launchArgs() const { return _launchArgs; }
    const JsValue &pageArgs() const { return _pageArgs; }

    void checkAppPage();

    void listAppInstalledInit();
    JsValue listAppRunning() const;
    JsValue getInfo(const String &app_id = String());
    const JsValue &getListAppInstalled() const { return _listAppInstalled; }
	const Vector<String> getListAppName();

    void listAppInstalledDump();
    void listAppRunningDump();

    int systemAppInstall(const JsValue &arg, bool check = false);
    int appInstall(const JsValue &arg, bool check = false);
    bool appInstallDeploy(const char *app_id, bool launch, bool isUserApp = true);
    int appUninstall(const char *app_id, bool check = false);

    bool appInAnim(void);
    bool appAnimEnter(JsAppX *fg, JsAppX *bg);
    bool appAnimExit(JsAppX *fg, JsAppX *bg, bool isHide = false);
    void appAnimEnd(void);
    void pageAnimEnd(JsAppX *app);

private:
    AppManagement();
    void enterAnim();
    void exitAnim();
    void exitAnimHide();

    static AppManagement *_instance;
    AppManage _AppManage;
    JsAppX *_launcherApp, *_activateApp;
    Vector<JsAppX *> _appStack;
    JsValue _listAppInstalled, _launchArgs, _pageArgs;

    TransitionsAnimator *_animator;
    AnimatorGroup *_animGroup;
    JsAppX *_animFg, *_animBg;
};
