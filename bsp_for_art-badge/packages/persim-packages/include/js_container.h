#pragma once

#include <pm_widget.h>

class JsContainer : public Persimmon::Widget
{
public:
    JsContainer();
    virtual ~JsContainer();

    virtual void setData(void *data);

private:
};
