#pragma once

#include <pm_timer.h>
#include "js_utility.h"

class JsTimer : public JsNativeClassT<JsTimer>
{
public:
    JsTimer(const JsValue &func, int timeout);
    virtual ~JsTimer();
    void start(bool isPeriodic);
    const JsValue &jsObject() const { return _object; }

private:
    void onTimeout();
    persim::Timer _timer;
    JsValue _object, _function;
};
