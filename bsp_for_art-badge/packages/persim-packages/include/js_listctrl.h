#pragma once

#include <pm_listctrl.h>
#include "js_utility.h"

class JsListCtrl : public persim::ListCtrl, public JsEventHandler
{
public:
    JsListCtrl();
    virtual ~JsListCtrl();

    void onSignal(MoveType type, int value);
    virtual void setData(JsPage *page, const JsValue &data);
    virtual void getData(JsValue &ret, const persim::String &name) const;

protected:
    virtual void scrolled(int x, int y);

private:
    JsValue _onscorll;
};
