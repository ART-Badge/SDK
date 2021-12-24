#pragma once

#include <pm_qrcode.h>
#include "js_utility.h"

class JsQRCode : public persim::QRCode
{
public:
    JsQRCode();

    virtual void setData(JsPage *page, const JsValue &data);

protected:
};
