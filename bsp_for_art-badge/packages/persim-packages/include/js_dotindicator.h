#pragma once

#include <pm_dotindicator.h>
#include "js_utility.h"

class JsDotIndicator : public persim::DotIndicator
{
public:
    JsDotIndicator();
    virtual ~JsDotIndicator();
    virtual void setData(JsPage *page, const JsValue &data);

private:
};
