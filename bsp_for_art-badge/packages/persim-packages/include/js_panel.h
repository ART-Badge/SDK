#pragma once

#include <pm_panel.h>
#include <pm_pixmap.h>
#include "js_utility.h"

class JsPanel : public persim::Panel
{
public:
    JsPanel();
    JsPanel(const JsPanel &other);
    virtual ~JsPanel();

    virtual void setData(JsPage *page, const JsValue &data);
    void bindTouch(const JsEventCallback &cb);
    using Panel::setLayout;

protected:
    virtual bool touchEvent(persim::TouchEvent *event);
    virtual bool gestureEvent(persim::GestureEvent *event);
    virtual void paintEvent(persim::PaintEvent *event);

private:
    void touchTrigger(const char *type, const persim::Point &point);
    void setLayout(const String &name);
    void drawSnapshot(Widget *widget);
    void deleteSnapshot();

private:
    uint32_t bindtouch : 1;
    uint32_t acceptHoriPan : 1;
    uint32_t acceptVertPan : 1;
    persim::Point _lastPoint;
    JsEventCallback _callback;
    bool _isTouched;
    persim::Pixmap *_snapshot;
};
