#pragma once

#include <pm_card.h>
#include "js_utility.h"

class JsPage;

class JsCard : public persim::Card, public JsEventHandler
{
public:
    JsCard();
    virtual ~JsCard();

    virtual void setData(JsPage *page, const JsValue &data);
    virtual void getData(JsValue &ret, const persim::String &name) const;

protected:
    void setLooping(bool status);
    virtual void changeEvent(CardEvent event);
};
