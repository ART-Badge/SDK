#pragma once

#include <image_box.h>
#include <jsvm/jsvm.h>

class JsImageBox : public persim::ImageBox
{
public:
    JsImageBox();
    virtual ~JsImageBox();

    virtual void setData(JsPage *page, const JsValue &data);
};
