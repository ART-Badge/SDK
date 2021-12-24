/*
 * File      : pm_multi_textbox.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <vector>
#include <pm_sigslot.h>
#include <pm_translator.h>
#include <pm_layout.h>

namespace persim
{

class MultiTextBox : public Widget
{
public:
    PM_DECLARE_CLASSNAME(MultiTextBox)

    MultiTextBox();
    MultiTextBox(const Rect &rect);
    MultiTextBox(Font &font, const Rect &rect);
    virtual ~MultiTextBox();

    void addText(const String &text);
    String getText(void);
    void setLineAlign(int align);
    void setAutomatic(bool status);
    void clearText(void);
    void setRowHeight(int value);

    virtual void paintEvent(PaintEvent *event);

    Signal<int> clicked;

private:
    void layout();

private:
    LocalString _plainText;
    int rowHeight;
    bool _automatic;
};

} // namespace persim
