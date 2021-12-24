/*
 * File      : pm_switch.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>
#include <pm_sigslot.h>
#include <pm_animator.h>

namespace persim
{

class Switch : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Switch)

    Switch();
    virtual ~Switch();

    void setOnImage(const Image &image);
    void setOffImage(const Image &image);
    void setSliderImage(const Image &image);

    void setStatus(bool on, bool doAnim = true);
    bool getStatus(void);
    void statusChange(void);

    void enabled(void);
    void disabled(void);
    bool isEnable(void);

    void setText(const String &text);
    const String &getText();

    Signal<bool> changed;

    virtual void paintEvent(PaintEvent *event);

protected:
    virtual bool gestureEvent(GestureEvent *event);
    void updateProgress(int progress);

private:
    Image offImage, onImage, sliderImage;
    bool on, _enabled;
    String _text;
    int sliderPos;
    ValueAnimator<int> _anim;
};

} // namespace persim
