#pragma once

#include <pm_page.h>
#include <js_window.h>
#include "js_utility.h"
#include <map>

typedef std::map<String, JsValue> Listener;

namespace persim
{
class JsAnimator;
class AnimatorGroup;
} // namespace persim

class JsPage : public persim::Page, public JsEventHandler, public JsNativeClassT<JsPage>
{
public:
    JsPage();
    virtual ~JsPage();

    void onLoad(const JsValue &obj);
    void onResume(void);
    void onShow(void);
    void onHide(void);
    void onExit(void);
    void onUpdate(const JsValue &obj);
    void endAnim(void);

    virtual void setData(JsPage *page, const JsValue &data);

    void requestPrerender(const JsValue &func);
    void addListener(const JsValue &name, const JsValue &func);
    bool callListener(const String &name, const JsValue &value);

    const JsValue &getJsObject() const { return pageObj; }

    bool insertItems(Widget *widget, const JsValue &value);
    void setDataForeach(Widget *widget, const JsValue &data);

    static JsPage *loadPage(const JsValue &object, const String &url);

    void enterAnim(JsAnimator *animator, JsValue anim, bool snapshot);
    AnimatorGroup *enterAnim(void);
    void exitAnim(JsAnimator *animator, JsValue anim, bool snapshot);
    AnimatorGroup *exitAnim(void);

    void setAnim(bool state);
    bool isInAnim(void);
    bool isOnResume(void);

    void setResPack(const ResPack &res) { _res = res; }
    const ResPack &resPack(void) { return _res; }

protected:
    virtual bool event(Event *event);
    virtual bool touchEvent(TouchEvent *event);
    virtual bool gestureEvent(GestureEvent *event);
    virtual void paintEvent(PaintEvent *event);

private:
    void setJsObject(const JsValue &data);
    void tickEvent(TickEvent *event);
    void touchTrigger(const char *type, const Point &point);

private:
    typedef std::map<JsValue, int> PrerenderMap;
    JsValue pageObj;
    int _isOnResume;
    bool _isOnShow, _isOnExit;
    PrerenderMap m_prerenderMap;
    Listener listener;
    Point _lastPoint;
    bool _isTouched;
    JsAnimator *_enterAnim, *_exitAnim;
    JsValue _enterAnimJs, _exitAnimJs;
    bool _inAnim, _snapshot;
    ResPack _res;
};
