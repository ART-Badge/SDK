#pragma once

#include "pm_internal.h"
#include "pm_rect.h"
#include "pm_color.h"

PERSIMMON_NAMESPACE_BEGIN

class Gradient
{
public:
    Gradient() {}
    virtual ~Gradient() {}
    void setCenter(const PointF &center, float radius)
    {
        m_center = center;
        m_radius = radius;
    }
    void setColor(const Color &c1, const Color &c2)
    {
        m_color1 = c1;
        m_color2 = c2;
    }

    virtual uint32_t value(int x, int y);

private:
    float m_radius;
    PointF m_center;
    Color m_color1, m_color2;
};

PERSIMMON_NAMESPACE_END
