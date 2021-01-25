#ifndef __JS_ANIMATOR_H
#define __JS_ANIMATOR_H

#include <pm_animator.h>
#include "js_util.h"

namespace Persimmon
{

class AbstractAnimatorFactory
{
public:
    AbstractAnimatorFactory() {}
    virtual ~AbstractAnimatorFactory() {}
    virtual PropertyAnimator* create(Widget *widget) = 0;
};

class JsInterpolator : public AbstractInterpolator
{
public:
    JsInterpolator(js_value_t value);
    virtual ~JsInterpolator();
    virtual float interpolation(float input);

    static AbstractInterpolator* parseInterpolator(const char *name);
    static AbstractInterpolator* parseInterpolator(js_value_t value);
    static const char* cacheName(const char *name);

private:
    js_value_t m_function;
};

class JsAnimator
{
public:
    JsAnimator();
    ~JsAnimator();

    bool isPlay() const
    {
        return m_group->isPlay();
    }
    void step();
    void start();
    void reverse();
    void stop()
    {
        m_group->stop();
    }
    void setInterpolator(js_value_t value);
    void setDuration(int xms)
    {
        m_duration = xms;
    }
    void setInterval(int xms)
    {
        m_interval = xms;
    }
    void setRepeat(int count)
    {
        m_group->setRepeat(count);
    }
    void setCallback(js_value_t func)
    {
        m_callback = js_acquire_value(func);
    }
    void setWidget(Widget *widget)
    {
        m_widget = widget;
    }
    void addAnimator(AbstractAnimatorFactory *factor);

    void setEaseType(const char *name);

    void setDirection(const char *name);

private:
    AbstractInterpolator* createInterpolator();
    void onStop()
    {
        js_util_lock();
        if (js_value_is_function(m_callback))
        {
            js_call_func_obj(m_callback, m_callback, NULL, 0);
        }
        js_util_unlock();
    }

private:
    Widget *m_widget;
    AnimatorGroup *m_group;
    std::vector<PropertyAnimator *> m_anims;
    void (*m_setValue)(AbstractAnimator *, js_value_t, bool);
    js_value_t m_callback;
    int m_duration, m_interval;
    const char *m_interpolatorName;
    js_value_t m_interpolatorFunc;
    PropertyAnimator::Direction m_direction;
    PropertyAnimator::EaseType m_easeType;
};

};

#endif
