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
#include <pm_container.h>
#include <pm_image.h>
#include "sigslot.h"

namespace Persimmon
{

class Checkbox : public Widget
{
public:
    Checkbox();
    Checkbox(const Rect& rect);
    virtual ~Checkbox();

    bool isSelected(void)
    {
        return selected;
    }

    void enableSelected(bool sel = true)
    {
        selected = sel;
        invalidate();
    }

    void setAutoRefresh(bool au)
    {
        autoRefresh = au;
    }

    void setSelectedImg(Image *image)
    {
        if (selectedImg)
            delete selectedImg;

        selectedImg = NULL;

        if (image)
            selectedImg = image;
    }

    Image *getSelectedImg(void)
    {
        return selectedImg;
    }

    void setNormalImg(Image *image)
    {
        if (normalImg)
            delete normalImg;

        normalImg = NULL;

        if (image)
            normalImg = image;
    }

    Image *getNormalImg(void)
    {
        return normalImg;
    }

    Signal<int, bool> changed;

    virtual bool handleGestureEvent(struct rtgui_event_gesture *, const struct rtgui_gesture *);

    virtual void render(struct rtgui_dc* dc, const Point &dcPoint = Point(),
                        const Rect &srcRect = Rect(),
                        RenderFlag flags = DrawNormal);

private:
    Image *selectedImg, *normalImg;
    bool selected, autoRefresh;
};

}
