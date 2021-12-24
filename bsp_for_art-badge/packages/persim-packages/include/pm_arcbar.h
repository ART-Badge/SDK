/*
 * File      : pm_arcbar.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-01     realthread   the first version
 * 2021-03-05     realthread   Update to Persim v4.1.0
 */
#pragma once

#include <pm_scrollbar.h>
#include <pm_painter.h>
#include <pm_widget.h>

PERSIMMON_NAMESPACE_BEGIN

class ArcBar : public AbstractBar
{
public:
    PM_DECLARE_CLASSNAME(ArcBar)

    ArcBar();
    virtual ~ArcBar();

    void setShape(float cx, float cy, float radius, float angleStart = -10, float angleEnd = 10,
                  float lineWidth = 10);
    void setCenterX(float cx);
    void setCenterY(float cy);
    void setRadius(float radius);
    void setAngleStart(float angle);
    void setAngleEnd(float angle);
    void setAngle(float angleStart, float angleEnd);
    void setLineWidth(float lineWidth);

    virtual void paintEvent(PaintEvent* event);

private:
    void drawBar(Painter *p);
    void drawBgBar(Painter *p);

private:
    float m_centerX, m_centerY, m_radius;
    float m_angleStart, m_angleEnd, m_lineWidth;
};

class ArcDotIndicator : public AbstractBar
{
public:
    PM_DECLARE_CLASSNAME(ArcDotIndicator)

    enum DotStyle
    {
        MoveDot,
        LightDot
    };
    ArcDotIndicator();
    virtual ~ArcDotIndicator();

    void setDotStyle(DotStyle dotStyle);
    void setShape(float cx, float cy, float radius, float angleStart = -10, float angleEnd = 10,
                  float dotSizeFocus = 8, float dotSizeNormal = 8);
    void setCenterX(float cx);
    void setCenterY(float cy);
    void setRadius(float radius);
    void setAngleStart(float angle);
    void setAngleEnd(float angle);
    void setAngle(float angleStart, float angleEnd);
    void setDotSizeFocus(float size);
    void setDotSizeNormal(float size);
    void setDotCount(int count);
    void setKeepSpacing(bool enabled);

    virtual void paintEvent(PaintEvent* event);

private:
    void drawDots(Painter *p);
    void drawMoveDot(Painter *p);
    void drawLightDot(Painter *p);
    void updateAngleSpacing();
    void updateAngleRange();

private:
    DotStyle m_dotStyle;
    float m_centerX, m_centerY, m_radius;
    float m_angleStart, m_angleEnd;
    float m_dotSizeFocus, m_dotSizeNormal;
    float m_angleSpacing;
};

PERSIMMON_NAMESPACE_END
