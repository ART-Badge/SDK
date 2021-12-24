#pragma once

#include "pm_internal.h"
#include "pm_rect.h"
#include "pm_event.h"
#include "pm_vector.h"
#include "pm_widget.h"

PERSIMMON_NAMESPACE_BEGIN

class Painter;
class TouchEvent;
class GestureEvent;

class WidgetExtent
{
public:
    WidgetExtent() {}
    WidgetExtent(const Rect &rect) : _rect(rect), _offset(rect.topLeft()) {}
    WidgetExtent(const Rect &rect, const Point &offset) : _rect(rect), _offset(offset) {}
    bool isEmpty() const { return _rect.isEmpty(); }
    void join(const Rect &rect);
    const Rect &rect() const { return _rect; }
    const Point &offset() const { return _offset; }
    bool contains(const Point &point) const;

private:
    Rect _rect;
    Point _offset;
};

class WidgetManager
{
public:
    WidgetManager(Widget *root = nullptr);
    ~WidgetManager();
    const Rect &dirty() const { return m_dirty; }
    Widget *rootWidget() const { return m_root; }
    void setRootWidget(Widget *root);
    void fullScreenDraw(bool enable);
    void paintAll();
    bool addDirty(const Widget *widget, const Rect &rect);
    void sendEvent(Event *event);
    bool sendEvent(Widget *receiver, Event *event) { return receiver->event(event); }
    void markDelete(Widget *widget);
    void deleteLater(Widget *widget);
    void deleteDeadObjects();

    static void paintTree(Widget *widget, WidgetExtent extent);
    static bool paintOne(Widget *widget, const WidgetExtent &extent);

protected:
    template<class T> void targetEvent(T *event);

private:
    Rect globalExposed(const Widget *widget, const Rect &rect) const;

private:
    Vector<Widget*> m_deadlist;
    Widget *m_root;
    class GestureManager *m_gestmanager;
    class EncoderManager *m_encodermanager;
    Rect m_dirty;
    bool m_draw;
};

inline void WidgetExtent::join(const Rect &rect)
{
    _rect &= rect + _offset;
    _offset += rect.topLeft();
}

inline bool WidgetExtent::contains(const Point &point) const
{
    return _rect.contains(point);
}

PERSIMMON_NAMESPACE_END
