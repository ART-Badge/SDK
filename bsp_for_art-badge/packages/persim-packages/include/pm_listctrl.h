/*
 * File      : pm_listctrl.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <vector>
#include <pm_widget.h>
#include <pm_image.h>
#include <pm_sigslot.h>
#include "encoder/pm_encoder.h"

namespace persim
{

class AbstractBar;
template<class> class ValueAnimator;
class AbstractInterpolator;
class ListInterpolator;

class ListCtrl : public Widget
{
public:
    PM_DECLARE_CLASSNAME(ListCtrl)

    enum MoveType
    {
        DropStart,
        DropFirst,
        DropLast,
        MoveChange
    };

    ListCtrl();
    virtual ~ListCtrl();

    void setScrollBar(AbstractBar *bar);
    void setScrollbarPosition(int p);

    void setDirection(Direction direction);
    void setItemGap(int gap)
    {
        if (gap < 0)
            return;

        itemGap       = gap;
        firstPosition = 0;
        lastPosition  = 0;
    }

    void setAcceleration(unsigned int value) { acceleration = value > 100 ? value : 100; }

    void movePosition(int position);
    void moveToPosition(int position, bool anim = false);
    void fixItemExtent(bool force = false);
    void setItemAlign(int align);
    void setItemWidth(int width);
    void setItemHeight(int height);
    void selectIndex(int index);
    int focusIndex() const;
    void setAutoFade(bool isAuto);
    void setAutoStep(bool isAuto);
    Signal<MoveType, int> bindSignal;

protected:
    virtual bool event(Event *event);
    virtual bool gestureEvent(GestureEvent *event);
    virtual bool encoderEvent(EncoderEvent* event);
    virtual void scrolled(int x, int y) {}
    int totalLength() const;

private:
    void fixItemExtent(Widget *widget, bool force = false);
    bool startAnimation(int speed = 0);
    void updateScrollBar();
    void onAnimRun(int value);
    void onAnimFinish();
    ListInterpolator *m_interpolator;

private:
    int firstPosition, lastPosition;
    int itemGap;
    Direction direction;
    ValueAnimator<int> *m_anim;
    int animMovePitch;
    unsigned int acceleration;
    int scrollbarPosition;
    bool isTop, isBottom, showScrollbar, inGesture;
    AbstractBar *m_scrollBar;
    int m_itemWidth, m_itemHeight;
    int m_itemAlign;
    int m_oldIndex;
    bool m_inFixItems;
    bool m_isAutoFade, m_isAutoStep;
};

} // namespace persim
