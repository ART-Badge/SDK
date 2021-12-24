/*
 * File      : pm_button.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date             Author          Notes
 * 2017-11-05       realthread      the first version
 */

#pragma once

#include <pm_image.h>
#include <pm_widget.h>
#include <pm_sigslot.h>
#include <pm_translator.h>

PERSIMMON_NAMESPACE_BEGIN

class Button : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Button);

    enum PressType
    {
        Down,
        Up,
        Click,
        Press
    };

    Button(Widget *parent = nullptr);
    virtual ~Button();

    bool isDown() const { return m_isDown; }
    String text() const { return m_text; }
    const Image &upImage() const { return m_upImage; }
    const Image &downImage() const { return m_downImage; }

    void setDown(bool state);
    void setText(const String &text);
    void setUpImage(const Image &image);
    void setDownImage(const Image &image);
    void setTriggerInterval(int inteval) { m_triggerInerval = inteval; }
    void setPressable(bool status) { m_pressable = status; }
    void setOpacity(float opacity);
    Signal<PressType> triggered;

protected:
    virtual bool gestureEvent(GestureEvent *event);
    virtual void paintEvent(PaintEvent *event);

private:
    LocalString m_text;
    Image m_upImage, m_downImage;
    uint32_t m_duration, m_triggerInerval;
    bool m_isDown, m_pressable;
    int _opacity;
};

PERSIMMON_NAMESPACE_END
