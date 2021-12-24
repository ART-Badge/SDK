/*
 * File      : pm_transitionsanimator.h
 * COPYRIGHT (C) 2012-2021, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-07-23     realthread   the first version
 */
#pragma once

#include <pm_widget.h>
#include <pm_animator.h>
#include <jsvm/jsvm.h>

USING_PERSIMMON_NAMESPACE

typedef JsValue (*JsTransitionsUpdate)(const JsValue &data);

class TransitionsAnimator
{
public:
    class Animator
    {
    public:
        Animator() {}
        virtual ~Animator() {}
        virtual void insertAnimation(AnimatorGroup *group, Widget *widget, Widget *face = nullptr) = 0;
    };

    TransitionsAnimator() : _duration(0), _exchangeFgBg(false), _enterAnim(nullptr), _exitAnim(nullptr){};
    virtual ~TransitionsAnimator()
    {
        if (_enterAnim)
            delete _enterAnim;
        if (_exitAnim)
            delete _exitAnim;
    };

    virtual AnimatorGroup *enter(Widget *fg, Widget *bg)
    {
        if (_enterAnim || _exitAnim)
        {
            AnimatorGroup *animator = new AnimatorGroup();
            if (animator)
            {
                if (_enterAnim)
                {
                    if (_exchangeFgBg)
                    {
                        _enterAnim->insertAnimation(animator, fg, bg);
                    }
                    else
                    {
                        _enterAnim->insertAnimation(animator, fg);
                    }
                }
                if (_exitAnim)
                {
                    if (_exchangeFgBg)
                    {
                        _exitAnim->insertAnimation(animator, bg, fg);
                    }
                    else
                    {
                        _exitAnim->insertAnimation(animator, bg);
                    }
                }
                animator->setDuration(_duration ? _duration : 300); // duration defaut 300 ms
                return animator;
            }
        }
        return nullptr;
    }

    virtual AnimatorGroup *exit(Widget *fg, Widget *bg)
    {
        if (_enterAnim || _exitAnim)
        {
            AnimatorGroup *animator = new AnimatorGroup();
            if (animator)
            {
                if (_enterAnim)
                {
                    if (_exchangeFgBg)
                    {
                        _enterAnim->insertAnimation(animator, bg, fg);
                    }
                    else
                    {
                        _enterAnim->insertAnimation(animator, bg);
                    }
                }
                if (_exitAnim)
                {
                    if (_exchangeFgBg)
                    {
                        _exitAnim->insertAnimation(animator, fg, bg);
                    }
                    else
                    {
                        _exitAnim->insertAnimation(animator, fg);
                    }
                }
                animator->setDuration(_duration ? _duration : 300); // duration defaut 300 ms
                return animator;
            }
        }
        return nullptr;
    }

    void setDuration(int ms) { _duration = ms; }
    int duration(void) { return _duration; }
    void enableExchange(bool enable) { _exchangeFgBg = enable; }
    void setEnterAnimator(Animator *animator) { _enterAnim = animator; }
    void setExitAnimator(Animator *animator) { _exitAnim = animator; }

private:
    int _duration;
    bool _exchangeFgBg;
    Animator *_enterAnim, *_exitAnim;
};

class TransitionsAnimation
{
public:
    static TransitionsAnimation *instance();
    void setAppDefault(TransitionsAnimator *animator);
    void setPageDefault(TransitionsAnimator *animator);
    void setAnimator(TransitionsAnimator *animator);
    TransitionsAnimator *appAnimator(void);
    TransitionsAnimator *pageAnimator(void);
    void setTransitionsUpdate(const JsTransitionsUpdate value);
    JsValue callTransitionsUpdate(const JsValue &data);

private:
    TransitionsAnimation();
    virtual ~TransitionsAnimation();
    static TransitionsAnimation *_instance;
    bool _hasAnimator;
    TransitionsAnimator *_appDefault, *_pageDefault, *_animator;
    JsTransitionsUpdate _transitionsUpdate;
};
