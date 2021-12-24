#ifndef __JS_ANIMATOR_H
#define __JS_ANIMATOR_H

#include "pm_widgetanimator.h"
#include "js_utility.h"

PERSIMMON_NAMESPACE_BEGIN

class JsInterpolator : public AbstractInterpolator
{
public:
    JsInterpolator(const JsValue &value);
    virtual ~JsInterpolator();
    virtual float interpolation(float input);

    static AbstractInterpolator *parseInterpolator(const String &name);
    static AbstractInterpolator *parseInterpolator(const JsValue &value);

private:
    JsValue m_function;
};

class JsAnimator : public JsNativeClassT<JsAnimator>
{
public:
    JsAnimator();
    virtual ~JsAnimator();

    bool isPlay() const { return m_group.isPlay(); }
    void step();
    void start();
    void reverse();
    void stop() { m_group.stop(); }
    void end() { m_group.end(); }
    void setInterpolator(const JsValue &value);
    void setDuration(int xms) { m_duration = xms; }
    void playFrame(int xms) { m_group.playFrame(int(xms)); }
    void setRepeat(int count) { m_group.setRepeat(count); }
    void setCallback(const JsValue &func) { m_callback = func; }
    void setWidget(Widget *widget) { m_widget = widget; }
    void pushKeyFrames(const JsValue &keyframes, unsigned int duration, unsigned int start);
    void pushEndFrame(const String &prop, const JsValue &end, const JsValue &timeline = JsValue());
    void setEaseType(const String &name);
    void setDirection(const String &name);
    Widget *widget() const { return m_widget; }
    AnimatorGroup &group() { return m_group; }
    const AnimatorGroup &group() const { return m_group; }

private:
    void pushAnimator(WidgetAnimator *anim, unsigned int duration, unsigned int start);
    AbstractInterpolator *createInterpolator();
    void onStop();
    struct Timeline
    {
        WidgetAnimator *anim;
        unsigned int duration;
        unsigned int start;
    };

private:
    Widget *m_widget;
    AnimatorGroup m_group;
    Vector<Timeline> m_anims;
    JsValue m_callback;
    int m_duration;
    String m_interpolatorName;
    JsValue m_interpolatorFunc;
    PropertyAnimator::Direction m_direction;
    PropertyAnimator::EaseType m_easeType;
};

PERSIMMON_NAMESPACE_END

#endif
