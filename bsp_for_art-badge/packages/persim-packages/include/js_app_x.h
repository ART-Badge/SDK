#pragma once

#include <pm_page.h>
#include "js_utility.h"
#include <pm_resource.h>
#include <pm_pixmapanimator.h>

namespace persim
{
class Uri;
}

PERSIMMON_NAMESPACE_BEGIN

class JsAppX : public Resource, public PixmapAnimator
{
public:
    PM_DECLARE_CLASSNAME(JsAppX)

    JsAppX(const String &url);
    virtual ~JsAppX();

    void tempValue(String &path, const JsValue &value);
    void onLaunch(const JsValue &value = JsValue());
    void onShow(void);
    void onHide(void);
    void onExit(void);
    void onAppChoice(const String &apps, const Uri &uri);

    void show(bool isAnim = false);
    void hide(bool isAnim = false);
    void onShowPage(void);
    void onHidePage(void);
    void onExitPage(void);

    Vector<JsPage *> *getPages(void);
    Vector<JsPage *> *deletePages(void);
    void deleteHidePages();
    void setLastPage(JsPage *page);
    void delLastPage(void);
    void navigateTo();
    void setJsObject(const JsValue &value);
    const JsValue &getJsObject() const { return _jsobj; }

    bool isOnShow() { return _isOnShow; }

    String getUrl() const;
    String keyValue() const;

protected:
    virtual bool event(Event *event);
    virtual void paintEvent(PaintEvent *event);

private:

    Vector<JsPage *> _pages, _deletePages;
    JsPage *_lastPage;
    JsValue _jsobj, _tempValue;
    String _tempPath;
    bool _isLaunch, _isOnShow;
    class FontConfigMap *_facemap;
};

PERSIMMON_NAMESPACE_END
