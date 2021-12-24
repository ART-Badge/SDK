#pragma once

#include <pm_arcbar.h>
#include "js_utility.h"

class JsArcBar : public persim::ArcBar
{
public:
    JsArcBar();
    virtual ~JsArcBar();

    virtual void setData(JsPage *page, const JsValue &data);
};

class JsArcDotIndicator : public persim::ArcDotIndicator
{
public:
    JsArcDotIndicator();
    virtual ~JsArcDotIndicator();

    virtual void setData(JsPage *page, const JsValue &data);
};
