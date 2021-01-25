#pragma once

#include "js_object.h"

#include <pm_timer.h>

class JsTimer : public Persimmon::Timer
{
public:
    JsTimer(js_value_t obj, js_value_t func, int tick, Timer::flag flag);
    ~JsTimer();

    void onTimeout();

private:
    js_value_t timeoutObj;
    struct js_listener *listener;
};

class JsSyncTimer : public Persimmon::SyncTimer
{
public:
    JsSyncTimer(js_value_t function, js_value_t name = 0);
    ~JsSyncTimer();

    void onTimeout();

private:
    js_value_t m_timeoutFunction, timerName;
};
