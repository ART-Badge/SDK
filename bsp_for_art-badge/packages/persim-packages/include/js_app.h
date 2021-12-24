#pragma once

#include "js_utility.h"
#include "js_window.h"
#include "js_page.h"
#include <pm_panel.h>
#include <pm_application.h>

#include "js_app_x.h"

#include "app_management.h"

namespace persim
{
class ImageBox;
}; // namespace persim
class TransitionsAnimator;

class JsApp : public persim::Application, public JsNativeClassT<JsApp>
{
public:
    JsApp();
    virtual ~JsApp();

    void onUpdate(const JsValue &obj);
    void onShow(void);
    void jsWindowShow(void);

    static JsApp *getAppSelf();
    static JsPage *parsePage(const JsValue &data);
    JsPage *loadPage(const String &url);
    void navigate(const JsValue &value, const JsValue &num = JsValue());
    bool navigateTo(const JsValue &data);
    bool redirectTo(const JsValue &data);
    void navigateBack(const JsValue &data = JsValue(), int depth = 1);
    bool closePage(const JsValue &name);

    int getPagesNum(void);
    JsPage *getLastPage(unsigned int index = 0);
    JsPage *getCurrentPage();
    JsPage *getPagebyName(const String &name);

    const String &getJumpPageXml() const { return jumpPageXml; }

    JsWindow *window() { return (JsWindow *)Application::window(); }

    void setJumpPage(JsPage *page) { jumpPage = page; }

    void updateFpsLabel(void);
    void addListener(const JsValue &name, const JsValue &func);

    /* Animation */
    void doAnimShow(JsPage *page);
    void doAnimHide(JsPage *page, JsPage *pageBg);

    bool navigateTo(JsPage *page, int num = 0);
    void navigateBack(int depth);
    void closePage(const String &name);

    void onAnimShowFinished();
    void onAnimHideFinished();
    JsPage *popPages(int depth);

    void shieldTouch(bool shield = true)
    {
        shieldTouchPanel->setVisible(shield);
    }

protected:
    virtual bool event(Event *event);

private:
    void initJsObject(void);
    bool callListener(const char *name, const JsValue &value);
#ifdef PKG_PERSIMMON_USING_WATERMARK
    ImageBox *m_watermark;
#endif
    JsValue _jsobject;
    String jumpPageXml;
    JsPage *jumpPage;
    Listener listener;
    Panel *shieldTouchPanel;

    /* animator */
    JsPage *getPrevPage(JsPage *page);
    TransitionsAnimator *_animator;
    AnimatorGroup *_animGroup, *_animGroupDel;
    JsPage *_animFg, *_animBg;
};
