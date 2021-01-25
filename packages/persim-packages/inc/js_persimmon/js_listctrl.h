#pragma once

#include <pm_listctrl.h>
#include "js_object.h"

class JsListCtrl : public Persimmon::ListCtrl
{
public:
    JsListCtrl();
    virtual ~JsListCtrl();

    virtual Widget *depthCopy(void) const;
    void setJsFunc(js_value_t object, const char *func);
    void onSignal(MoveType type, int value);
    virtual void setData(void *data);
    virtual void getData(void *ret, const std::string &name) const;

private:
    js_value_t jsObj, jsCallFunc;
};
