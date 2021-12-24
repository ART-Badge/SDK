#pragma once

#ifdef __cplusplus

#include "pm_internal.h"
#include "pm_event.h"

PERSIMMON_NAMESPACE_BEGIN

class TouchEvent : public Event
{
public:
    enum State
    {
        Release,
        Start,
        Moving
    };
    const Point &point() const { return _point; }
    const Point &startPoint() const { return _start; }
    State state() const { return _state; }
    bool isPressed() const { return _state != Release; }
    uint32_t timestamp() const { return _ts; }
    uint32_t startTimestamp() const { return _start_ts; }
    uint32_t duration() const { return _ts - _start_ts; }
    void setState(State state) { _state = state; }

private:
    TouchEvent();
    Point _point, _start;
    State _state;
    uint32_t _start_ts, _ts;
    friend class TouchManager;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const TouchEvent::State &t);
const Logger &operator<<(const Logger &, const TouchEvent *);
#endif

PERSIMMON_NAMESPACE_END

extern "C" void pm_touch_send(int x, int y, bool pressed);

#else
#include <stdbool.h>

void pm_touch_send(int x, int y, bool pressed);
#endif
