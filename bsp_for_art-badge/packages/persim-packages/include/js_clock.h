#pragma once

#include <pm_clock.h>
#include <jsvm/jsvm.h>

class JsClock : public persim::Clock
{
public:
    JsClock();
    virtual ~JsClock();

    virtual void setData(JsPage *page, const JsValue &data);

private:
    struct PointerInfo
    {
        persim::Image image;
        persim::PointF position;
        bool validPos;
        PointerInfo() : validPos(false) {}
    };
    PointerInfo parsePointer(const JsValue &value);

private:
};
