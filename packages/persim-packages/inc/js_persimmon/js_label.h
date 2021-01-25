#pragma once

#include <pm_label.h>
#include "js_util.h"

class JsLabel : public Persimmon::Label
{
public:
    JsLabel();
    JsLabel(const JsLabel &other);
    virtual ~JsLabel() {}

    virtual Widget *depthCopy(void) const;
    void setDataValue(js_value_t value);
    virtual void setData(void *data);
    virtual void getData(void *ret, const std::string &name) const;

protected:
};
