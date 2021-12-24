#pragma once

#include <pm_barcode.h>
#include "js_utility.h"

class JsBarCode : public persim::BarCode
{
public:
    JsBarCode();

    virtual void setData(JsPage *page, const JsValue &data);

protected:
};
