#pragma once

#include <pm_clock.h>
#include <resource.h>
#include <js_util.h>

class JsClock : public Persimmon::Clock
{
public:
    JsClock();
    virtual ~JsClock();

    virtual void setData(void* data);

private:
    struct PointerInfo
    {
        Image *image;
        PointF position;
        bool validPos;
    };
    PointerInfo parsePointer(js_value_t value);

private:

};

