/*
 * File      : pm_animator.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-01     realthread   the first version
 */

#ifndef __PM_ANIMATOR_H__
#define __PM_ANIMATOR_H__

#include <pm_rect.h>
#include <pm_color.h>
#include <pm_sigslot.h>
#include <pm_vector.h>
#include <pm_timer.h>
#include <pm_variant.h>
#include <pm_transformoperations.h>

namespace persim
{

class Widget;
class Timer;
class SyncTimer;

class AbstractInterpolator
{
public:
    AbstractInterpolator() {}
    virtual ~AbstractInterpolator() {}
    virtual float interpolation(float input) = 0;
};

/* produce uniform animation */
class LinearInterpolator : public AbstractInterpolator
{
public:
    virtual float interpolation(float input);
};

/* produce accelerate animation */
class AccelerateInterpolator : public AbstractInterpolator
{
public:
    AccelerateInterpolator(float acce = 1.0f);
    virtual float interpolation(float input);

private:
    float m_acce;
};

/* produce slow down animation */
class NegExpInterpolator : public AbstractInterpolator
{
public:
    virtual float interpolation(float input);
};

/* produce bounce animation */
class BounceInterpolator : public AbstractInterpolator
{
public:
    virtual float interpolation(float x);

private:
    float bounce(float t);
};

class AnticipateInterpolator : public AbstractInterpolator
{
public:
    AnticipateInterpolator(float time = 1.0f);
    virtual float interpolation(float x);

private:
    float m_time;
};

template<typename T> class ValueEvaluator
{
public:
    T evaluate(float progress, const T &start, const T &end)
    {
        return start + (end - start) * progress;
    }
};

template<> class ValueEvaluator<Size>
{
public:
    virtual Size evaluate(float progress, const Size &start, const Size &end);
};

template<> class ValueEvaluator<Rect>
{
public:
    Rect evaluate(float progress, const Rect &start, const Rect &end);
};

template<> class ValueEvaluator<Color>
{
public:
    Color evaluate(float progress, const Color &start, const Color &end);
};

template<> class ValueEvaluator<Variant>
{
public:
    Variant evaluate(float progress, const Variant &start, const Variant &end)
    {
        const TransformOperations &a(start.to<TransformOperations>());
        const TransformOperations &b(end.to<TransformOperations>());
        return Variant(b.blend(a, progress));
    }
};

class AbstractAnimator
{
    friend class AnimatorReducer;
    friend class ParallelAnimator;
    friend class AnimatorGroup;

public:
    AbstractAnimator();
    virtual ~AbstractAnimator();
    void setDuration(int xms);
    void setRepeat(int times);
    void setReloadInterval(int xms);
    Signal<void> *finishedSignal();
    Signal<void> *startSignal();
    void start();
    bool stop();
    bool isPlay() const { return m_isPlay; }
    bool isOdd() const { return m_isOdd; }
    void setOdd(bool isOdd);

protected:
    virtual void end();
    virtual void playFrame(int index, int total);

private:
    void onTimeout();

private:
    unsigned int m_repeatCount, m_repeat;
    unsigned int m_reloadInterval, m_duration;
    SyncTimer *m_timer;
    bool m_repeatFlag;
    Signal<void> m_finished;
    Signal<void> m_firstFrame;
    unsigned long m_tick, m_tickStart, m_tickEnd;
    bool m_isOdd, m_isPlay;
};

class PropertyAnimator : public AbstractAnimator
{
public:
    enum EaseType
    {
        EaseIn,
        EaseOut,
        EaseInOut
    };
    enum Direction
    {
        Normal,
        Reverse,
        Alternate,
        AlternateReverse
    };
    PropertyAnimator();
    virtual ~PropertyAnimator();
    AbstractInterpolator *interpolator() const { return m_interpolator; }
    void setInterpolator(AbstractInterpolator *interpolator);
    void setEaseType(EaseType type) { m_easeType = type; }
    void setDirection(Direction direction) { m_direction = direction; }
    EaseType easeType() const { return m_easeType; }
    Direction direction() const { return m_direction; }

protected:
    virtual void playFrame(int index, int total);
    virtual void playFrame(float progress) = 0;
    virtual void initialization() {}

private:
    float interpolation(float input);

private:
    AbstractInterpolator *m_interpolator;
    EaseType m_easeType;
    Direction m_direction;
};

template<typename T> class PropertySettingBase
{
public:
    virtual ~PropertySettingBase() {}
    virtual void setProperty(const T &value) = 0;
};

template<class T1, typename T2> class PropertySetting : public PropertySettingBase<T2>
{
public:
    PropertySetting(T1 *obj, void (T1::*func)(const T2 &)) : m_obj(obj), m_func1(func), m_func2() {}
    PropertySetting(T1 *obj, void (T1::*func)(T2)) : m_obj(obj), m_func1(), m_func2(func) {}

    virtual void setProperty(const T2 &value)
    {
        if (m_func1)
            (m_obj->*m_func1)(value);
        else
            (m_obj->*m_func2)(value);
    }

private:
    T1 *m_obj;
    void (T1::*m_func1)(const T2 &);
    void (T1::*m_func2)(T2);
};

template<typename T> class ValueAnimator : public PropertyAnimator
{
public:
    template<class T1> ValueAnimator(T1 *obj, void (T1::*func)(const T &)) : PropertyAnimator()
    {
        PropertySetting<T1, T> *prop = new PropertySetting<T1, T>(obj, func);
        m_propSet                    = prop;
    }
    template<class T1> ValueAnimator(T1 *obj, void (T1::*func)(T)) : PropertyAnimator()
    {
        PropertySetting<T1, T> *prop = new PropertySetting<T1, T>(obj, func);
        m_propSet                    = prop;
    }
    virtual ~ValueAnimator() { delete m_propSet; }
    T startValue() const { return m_startValue; }
    T endValue() const { return m_endValue; }
    void setStartValue(const T &value) { m_startValue = value; }
    void setEndValue(const T &value) { m_endValue = value; }
    void setValueLimits(const T &start, const T &end)
    {
        m_startValue = start;
        m_endValue   = end;
    }

protected:
    virtual bool isRelative() const { return m_relative; }
    using PropertyAnimator::playFrame;
    virtual void playFrame(float progress)
    {
        T value = m_evaluator.evaluate(progress, m_startValue, m_endValue);
        m_propSet->setProperty(value);
    }

private:
    T m_startValue, m_endValue;
    PropertySettingBase<T> *m_propSet;
    ValueEvaluator<T> m_evaluator;
    bool m_relative;
};

class ParallelAnimator : public AbstractAnimator
{
    friend class AnimatorGroup;

public:
    ParallelAnimator();
    virtual ~ParallelAnimator();
    void addAnimator(AbstractAnimator *anim);
    virtual void playFrame(int index, int total);

protected:
    virtual void end();

private:
    Vector<AbstractAnimator *> m_anims;
};

class AnimatorGroup
{
public:
    AnimatorGroup();
    virtual ~AnimatorGroup();
    void addAnimator(AbstractAnimator *anim, unsigned int duration = 0, unsigned int start = 0);
    void addAnimator(AnimatorGroup *group, unsigned int duration = 0, unsigned int start = 0);
    void setDuration(int ms);
    void setRepeat(int count);
    void setSequence(bool sequence);
    void start(bool reset = true);
    void reverse(bool reset = true);
    void stop();
    void end();
    Signal<void> *startedSignal();
    Signal<void> *finishedSignal();
    Signal<void> *animatorStartedSignal(int index = -1);
    Signal<void> *animatorFinishedSignal(int index = -1);

    bool isPlay() const { return m_isPlay; }
    void playFrame(int index);
    void playFrame(float progress);

private:
    struct AnimatorNode
    {
        enum StateType
        {
            Unstarted = 0,
            Started,
            Finished,
            Reserve = 0x0F,
        };
        bool isGroup;
        AbstractAnimator *anim;
        AnimatorGroup *group;
        int duration, start;
        Signal<void> *started, *finished;
        StateType state;
    };

    void playFrame(struct AnimatorNode &node, int index, int total);
    void playFrame(int index, int total);
    void onTimeout();
    void resetAnimatorState();
    void sequenceTimeline();

private:
    Vector<AnimatorNode> m_anims;
    bool m_sequence, m_isPlay, m_reverse;
    int m_repeat, m_playCount;
    Signal<void> m_started, m_finished;

    SyncTimer m_timer;
    unsigned int m_duration, m_firstTime, m_currentTime;
};

}; // namespace persim

#endif
