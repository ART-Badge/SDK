#pragma once

#include <pm_animator.h>

PERSIMMON_NAMESPACE_BEGIN

class CubicBezierInterpolator : public AbstractInterpolator
{
public:
    CubicBezierInterpolator();
    CubicBezierInterpolator(float x1, float y1, float x2, float y2);
    virtual ~CubicBezierInterpolator();

    float x1() const { return m_x1; }
    float y1() const { return m_y1; }
    float x2() const { return m_x2; }
    float y2() const { return m_y2; }
    void setX1(float value);
    void setY1(float value);
    void setX2(float value);
    void setY2(float value);

    virtual float interpolation(float x);

private:
    float m_x1, m_y1, m_x2, m_y2;
};

PERSIMMON_NAMESPACE_END
