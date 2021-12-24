#pragma once

#include <pm_multi_textbox.h>
#include "js_utility.h"

class JsMultiTextBox : public persim::MultiTextBox, public JsEventHandler
{
public:
    JsMultiTextBox();
    virtual ~JsMultiTextBox();

    void onSignal(int h);
    virtual void setData(JsPage *page, const JsValue &data);
};
