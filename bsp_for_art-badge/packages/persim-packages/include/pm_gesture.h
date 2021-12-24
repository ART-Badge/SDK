#pragma once

#include "pm_event.h"

PERSIMMON_NAMESPACE_BEGIN

class Gesture
{
public:
    enum State
    {
        Reset,
        Started,
        Updated,
        Finished,
        Canceled
    };
    enum Type
    {
        Unknow      = 0,
        Press       = 1 << 1,
        Pan         = 1 << 2,
        GestureMask = 0xff
    };
    virtual ~Gesture();
    Type type() const { return Type(_type); }
    State state() const { return State(_state); }
    bool isStarted() const { return state() == Started; }
    bool isUpdated() const { return state() == Updated; }
    bool isFinished() const { return state() == Finished; }
    bool isCanceled() const { return state() == Canceled; }
    const Point &point() const { return _point; }

protected:
    Gesture(Type type);
    void setPoint(const Point &point) { _point = point; }

private:
    uint32_t _type : 8;
    uint32_t _state : 4;
    uint32_t _ignored : 1;
    Point _point;
    friend class GestureManager;
    friend class GestureRecognizer;
};

class PressGesture : public Gesture
{
public:
    PressGesture();
    uint32_t duration() const { return _duration; }

private:
    uint32_t _duration;
    friend class PressGestureRecognizer;
};

class PanGesture : public Gesture
{
public:
    PanGesture();
    Point offset() const { return _offset; }
    Point delta() const { return _delta; }
    Point velocity() const { return _velocity; }
    int speed() const { return _velocity.modules(); }
    bool isHorizontal() const;

private:
    Point _offset, _delta, _velocity;
    uint32_t _start_ts, _last_ts;
    friend class PanGestureRecognizer;
};

class GestureEvent : public Event
{
public:
    GestureEvent(class Gesture *gesture);
    Gesture::Type gestureType() const { return _gesture->type(); }
    Gesture::State state() const { return _gesture->state(); }
    class Gesture *gesture() const { return _gesture; }

private:
    class Gesture *_gesture;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Gesture::State &t);
const Logger &operator<<(const Logger &, const Gesture::Type &t);
const Logger &operator<<(const Logger &, const GestureEvent *);
const Logger &operator<<(const Logger &, const Gesture *);
#endif

PERSIMMON_NAMESPACE_END
