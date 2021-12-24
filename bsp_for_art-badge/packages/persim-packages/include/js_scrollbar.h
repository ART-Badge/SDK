#pragma once

#include <pm_arcbar.h>
#include "js_utility.h"

class JsScrollBar : public persim::ScrollBar
{
public:
    JsScrollBar();
    virtual ~JsScrollBar();

    virtual void setData(JsPage *page, const JsValue &data);
};
