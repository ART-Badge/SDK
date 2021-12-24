/*
 * File      : pm_slider.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 * 2021-02-24     realthread   Update to Persim v4.1.0
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>
#include <pm_sigslot.h>
#include <pm_painter.h>

PERSIMMON_NAMESPACE_BEGIN
class Slider : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Slider);
    enum Type
    {
        change,
        release
    };

    Slider();
    Slider(const Color &nor, const Color &bar);
    Slider(const Color &nor, const Color &bar, const Image &sliderImg);
    virtual ~Slider();

    void setDirection(Direction t = Direction::Horizontal);
    void setMinimum(int min);
    void setMaximum(int max);
    void setValue(int value);
    int getCurrentValue(void);
    void setBarPounds(int pounds);
    void setNorColor(Color c);
    void setBarColor(Color c);
    void setSliderImg(const Image &img);
    void setBarImage(const Image &img);
    void setNorImage(const Image &img);
    Signal<Type, int> changed;

protected:
    virtual bool gestureEvent(GestureEvent *event);
    virtual void paintEvent(PaintEvent *event);

private:
    Direction m_direction;
    Image sliderImage;
    Image barImage, norImage;
    Color barColor, norColor;
    int barPounds;
    int progress, baseProgress;
    int minValue, maxValue, currentValue;
};

PERSIMMON_NAMESPACE_END
