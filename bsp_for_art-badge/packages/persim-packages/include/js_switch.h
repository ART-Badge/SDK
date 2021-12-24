#pragma once

#include <pm_switch.h>
#include "js_utility.h"

class JsSwitch : public persim::Switch, public JsEventHandler
{
public:
    JsSwitch();
    virtual ~JsSwitch();

    virtual void setData(JsPage *page, const JsValue &data);

private:
    void onSignal(bool status);
};
