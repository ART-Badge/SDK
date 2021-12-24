/*
 * File      : js_toast.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2021, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-08     realthread   first version
 */
#pragma once
#include "jsvm/jsvm.h"
#include <pm_vector.h>
#include "pm_widget.h"
#include "pm_timer.h"
#include <pm_animator.h>
#include <pm_translator.h>
#include "pm_pixmap.h"

USING_PERSIMMON_NAMESPACE

class ToastTextBox : public Widget
{
public:
    PM_DECLARE_CLASSNAME(ToastTextBox)

    ToastTextBox();
    virtual ~ToastTextBox();
    void enableRound(bool round = true);
    void setText(const String &text, int minWidth, int minHeight);
    String getText(void);
    void setRowHeight(int value);
    void setMargin(int h, int v);
    void setMarginR(int r);
    virtual void renderbg();
    virtual void paintEvent(PaintEvent *event);

    void setOpacity(float opacity);
    void animPrepare(void);
    void animRelease(void);

private:
    LocalString _plainText;
    bool _round;
    int rowHeight, hMargin, vMargin, marginR;
    Pixmap _pixmap;
    int _opacity;
};

class ToastData
{
public:
    persim::String title;
    int duration, width, minWidth, minHeight, hMargin, vMargin, marginR, rowHeight;
    bool mask, round;
    persim::Color fgColor, bgColor, maskColor;
    persim::String fontFamily;
    int fontSize;
    ToastData();
    ~ToastData();
};

class Toast
{
public:
    static Toast *instance();
    void insert(ToastData &data);
    void handler(bool showLatest = false);
    void hide(void);

protected:
    Toast();
    ~Toast();
    void durationTimeout(void);

    void enterEnd(void);
    void exitEnd(void);
    bool animEnter(ToastTextBox *toast);
    bool animExit(ToastTextBox *toast);
    void animEnd(void);

private:
    static Toast *_instance;
    persim::Vector<ToastData> _toastData;
    bool _isShow;
    persim::Widget *_toast;
    persim::Timer _timer;
    ToastTextBox *_toastBox;
    persim::AnimatorGroup *_animator;
};
