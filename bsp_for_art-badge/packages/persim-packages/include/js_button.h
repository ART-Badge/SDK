#pragma once

#include <pm_button.h>
#include "js_utility.h"

class JsButton : public persim::Button, public JsEventHandler
{
public:
    JsButton();
    virtual ~JsButton();
    virtual void setData(JsPage *, const JsValue &data);
    void enableToggled(bool status) { _enableToggled = status; }

private:
    void onSignal(PressType type);
    JsValue jsCustomProperty;
    bool _enableToggled;
};
