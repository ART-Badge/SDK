/*
* File      : main_win.h
* COPYRIGHT (C) 2012-2017, Shanghai Real-Thread Technology Co., Ltd
*
* Change Logs:
* Date           Author       Notes
* 2017-12-22     realthread   the first version
* 2018-02-28     realthread   the ui demo for 1024 X 600
*/

#pragma once

#include <pm_window.h>
#include <pm_timer.h>
#include <pm_label.h>
#include <pm_button.h>

using namespace Persimmon;

class MainWin : public Window
{
public:
    MainWin(const char *title);

    void onBtn(int num);
    virtual void pageEmpty(void);

private:
    Label *label;
    Button *btn;
};

extern "C" {
    void rtgui_main_app(void);
}
