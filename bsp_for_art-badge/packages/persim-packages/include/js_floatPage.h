/*
 * File      : js_floatPage.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2021, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-16     realthread   first version
 */
#pragma once
#include "jsvm/jsvm.h"
#include <pm_vector.h>
#include "js_page.h"
#include <pm_panel.h>

class TransitionsAnimator;

class FloatPage
{
public:
    typedef std::pair<JsPage *, int> FloatPageItem;
    typedef persim::Vector<FloatPageItem> FloatPageList;

    static FloatPage *instance();
    void show(JsPage *page, JsValue value, int index = -1);
    bool hide(JsPage *page);
    bool hide(const String &name);
    bool isTransparent(void);
    void tickEvent(Event *event);
    int count(void);
    const FloatPageList &floatPageList(void);
    JsValue floatPageObject(const String &name = String());

protected:
    FloatPage();
    ~FloatPage();
    void fixShow(bool lifecycle = true, bool visible = true, bool doAnim = false);

private:
    static FloatPage *_instance;
    FloatPageList _floatPage;
    persim::Panel *_floatPanel;
    bool _transparent;

    /* animator */
    TransitionsAnimator *_animator;
    AnimatorGroup *_animGroup, *_animGroupDel;
    JsPage *_animFg;
    PixmapAnimator *_animBg;

    /* Animation */
    void doAnimShow(JsPage *page, PixmapAnimator *animBg);
    void doAnimHide(JsPage *page, PixmapAnimator *animBg);

    void onAnimShowFinished();
    void onAnimHideFinished();
};
