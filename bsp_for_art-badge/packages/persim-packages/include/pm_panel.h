/*
 * File      : pm_panel.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>
#include <pm_layout.h>

PERSIMMON_NAMESPACE_BEGIN

class Layout;

class Panel : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Panel)

    Panel(Widget *parent = nullptr);
    virtual ~Panel();

    void setLayout(AxialLayout *layout);

protected:
    virtual bool event(Event *event);
    virtual bool touchEvent(TouchEvent *event) { return true; }
    virtual bool gestureEvent(GestureEvent *event) { return true; }

private:
    AxialLayout *m_layout;
};

PERSIMMON_NAMESPACE_END
