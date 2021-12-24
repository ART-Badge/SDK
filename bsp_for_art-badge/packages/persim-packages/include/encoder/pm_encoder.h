#pragma once

#ifdef __cplusplus
#include "pm_widgetmanager.h"
#include "pm_widget_p.h"
#include "pm_internal.h"
#include "pm_event.h"

PERSIMMON_NAMESPACE_BEGIN

class EncoderEvent : public Event
{
public:
    enum State
    {
        NullState,
        StateChange,
    };

    int delta() const { return _delta; }
    int velocity() const { return _velocity; }
    bool direction() const { return _direction; }
    int duration() const {return _now_ts - _last_ts; }
    void setState(State state) { _state = state; }
    State state() const { return _state; }
private:
    EncoderEvent();
    int _delta;
    int _velocity;
    bool _direction;
    int _now_ts, _last_ts;
    int _cumDelta;
    int _duration;
    State _state;
    friend class EncoderManager;
};

class EncoderManager
{

public:
    EncoderManager();
    ~EncoderManager();
    void encoderEvent(Widget* w, EncoderEvent *event);
    void post(int d);
private:
    bool widgetDispatch(Widget* widget, EncoderEvent* event);
    bool eventTrans(Widget* w, EncoderEvent* event);
    int calculateVelocity(EncoderEvent* event);
    int _now_ts, _last_ts;
    int _lastDuration;
    int _cumDelta;
    EncoderEvent *_event;

};

PERSIMMON_NAMESPACE_END

extern "C" void pm_encoder_send(int d);

#else
#include <stdbool.h>

void pm_encoder_send(int d);
#endif
