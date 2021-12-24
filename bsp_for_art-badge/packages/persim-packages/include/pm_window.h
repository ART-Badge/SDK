/*
 * File      : pm_window.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_sigslot.h>

namespace persim
{

class Window : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Window)

    /* create a main window */
    Window();
    virtual ~Window();
    void show();
    Signal<void> signalShow;

protected:
    virtual bool event(Event *event);
    virtual void paintEvent(PaintEvent *event);

private:
    void tickEvent(Event *event);
    bool SHOW_FIRST;
};

} // namespace persim
