#pragma once

#include <pm_checkbox.h>
#include "js_utility.h"

class JsCheckbox : public persim::Checkbox, public JsEventHandler
{
public:
    JsCheckbox();
    virtual ~JsCheckbox();
    virtual void setData(JsPage *page, const JsValue &data);

private:
    void onSignal(bool s);
};
