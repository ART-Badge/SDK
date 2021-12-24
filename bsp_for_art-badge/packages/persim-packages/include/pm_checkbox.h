/*
 * File      : pm_checkbox.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <vector>
#include <pm_widget.h>
#include <pm_image.h>
#include "pm_sigslot.h"

namespace persim
{

class Checkbox : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Checkbox)

    Checkbox();
    virtual ~Checkbox();

    bool isSelected(void) { return selected; }

    void setSelected(bool status)
    {
        selected = status;
        update();
    }

    void setSelectedImg(const Image &image) { selectedImg = image; }

    const Image &getSelectedImg(void) { return selectedImg; }

    void setNormalImg(const Image &image) { normalImg = image; }

    const Image &getNormalImg(void) { return normalImg; }

    Signal<bool> changed;

    virtual void paintEvent(PaintEvent *event);

protected:
    virtual bool gestureEvent(GestureEvent *event);

private:
    Image selectedImg, normalImg;
    bool selected, autoRefresh;
};

} // namespace persim
