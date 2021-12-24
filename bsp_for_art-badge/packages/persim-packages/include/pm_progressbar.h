/*
 * File      : pm_progressbar.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>
#include <sigslot.h>
#include <image_9patch.h>

namespace Persimmon
{

class ProgressBar : public Widget
{
public:
    enum type
    {
        HORIZONTAL = 1 << 0,
        VERTICAL   = 1 << 1,
    };
    DEFINE_CLASS_ENUM_FLAG_OPERATORS(type);

    ProgressBar();
    ProgressBar(Image *norImg, Image *barImg);
    virtual ~ProgressBar();

    void setDirection(enum type t = HORIZONTAL) { direction = t; }

    void setValueLimits(int min, int max)
    {
        minValue = min;
        maxValue = max;
        if (maxValue <= minValue)
        {
            maxValue = minValue + 1;
        }
    }

    void setValue(int value)
    {
        if (value > maxValue)
            currentValue = maxValue;
        else if (value < minValue)
            currentValue = minValue;
        else
            currentValue = value;
        invalidate();
    }

    int getValue(void) { return currentValue; }

    virtual void render();

private:
    enum type direction;
    int progress, baseProgress;
    int minValue, maxValue, currentValue;
};

} // namespace Persimmon
