#pragma once

#include "pm_internal.h"
#include "pm_rect.h"

PERSIMMON_NAMESPACE_BEGIN

class PaintEngine;

class PaintDevice
{
public:
    virtual ~PaintDevice();
    int width() const { return metric.width; };
    int height() const { return metric.height; };
    Size size() const { return Size(width(), height()); }
    virtual PaintEngine *paintEngine() = 0;
    virtual Point position() const;
    virtual void start();
    virtual void flush(const Rect &rect);

protected:
    struct DeviceMetric
    {
        uint16_t width;
        uint16_t height;
    };
    PaintDevice();
    DeviceMetric metric;
};

PERSIMMON_NAMESPACE_END
