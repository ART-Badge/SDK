#pragma once

#include "pm_internal.h"
#include "pm_rect.h"

PERSIMMON_NAMESPACE_BEGIN

class Widget;

class Event
{
public:
    enum Type
    {
        Tick,
        Paint,
        UpdateChild,
        Geometry,
        Visible,
        Touch,
        Gesture,
        Promise,
        Encoder,
        /* user command event. It should always be the last command type. */
        UserEvent = 0x100
    };

    virtual ~Event();
    int type() const { return m_type; }
    bool isAccepted() const { return m_accept; }
    void setAccepted(bool accepted) { m_accept = accepted; }
    void accept() { setAccepted(true); }
    void ignore() { setAccepted(false); }

protected:
    Event(int type) : m_type(type), m_accept(false) {}

private:
    uint16_t m_type;
    uint16_t m_accept : 1;
};

class WidgetEvent : public Event
{
public:
    Widget *target() const { return _target; }

protected:
    WidgetEvent(int type, Widget *target = nullptr) : Event(type), _target(target) {}

private:
    Widget *_target;
};

class TickEvent : public Event
{
public:
    TickEvent(uint32_t tick);
    uint32_t clock() { return _tick; }

private:
    uint32_t _tick;
};

class PaintEvent : public Event
{
public:
    PaintEvent();
};

class ChildEvent : public WidgetEvent
{
public:
    enum UpdateType
    {
        Insert = 0,
        Remove = 1
    };
    ChildEvent(Widget *target, int index, UpdateType type);
    int index() const { return _index; }
    bool isInsert() const { return _type == Insert; }

private:
    int _index;
    UpdateType _type;
};

class GeometryEvent : public WidgetEvent
{
public:
    GeometryEvent(Widget *target, const Rect &oldRect, const Rect &newRect);
    void setNewRect(const Rect &rect) { _newRect = rect; }
    const Rect &oldRect() const { return _oldRect; }
    const Rect &newRect() const { return _newRect; }

private:
    Rect _oldRect, _newRect;
};

class VisibleEvent : public WidgetEvent
{
public:
    VisibleEvent(Widget *target, bool visible);
    bool isVisible() const { return _visible; }
    void setVisible(bool visible) { _visible = visible; }

private:
    bool _visible;
};

class PromiseEvent : public Event
{
public:
    PromiseEvent();
    virtual void work() = 0;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Event::Type &t);
const Logger &operator<<(const Logger &, const Event *);
#endif

PERSIMMON_NAMESPACE_END
