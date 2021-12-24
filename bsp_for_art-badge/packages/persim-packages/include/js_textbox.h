#pragma once

#include <pm_label.h>
#include "js_utility.h"

USING_PERSIMMON_NAMESPACE

class JsTextBox : public persim::Label, public JsEventHandler, public JsNativeClassT<JsTextBox>
{
public:
    JsTextBox();
    virtual ~JsTextBox();

    void setKeyboard(const String &url);
    void setText(const String &text);

    void onChanged(const JsValue &text);

    virtual void setData(JsPage *page, const JsValue &data);

protected:
    virtual bool gestureEvent(GestureEvent *event);

protected:
};
