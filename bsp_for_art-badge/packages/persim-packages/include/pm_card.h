#pragma once

#include <pm_widget.h>
#include <pm_animator.h>
#include <pm_scrollbar.h>
#include "encoder/pm_encoder.h"

PERSIMMON_NAMESPACE_BEGIN

class AbstractSwitcher;
class AbstractScrollComputer;

class Card : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Card)

    typedef int (*BounceFunc)(int value, int maximum);

    enum CardEvent
    {
        Unchanged,
        Changed,
        DropFirst,
        DropLast,
        TouchStart,
        TouchRelease,
        UnknowEvent
    };

    Card(Widget *parent = nullptr, Direction direction = Direction::Horizontal);
    virtual ~Card();
    Direction direction() const { return m_direction; }
    void setDirection(Direction direction);
    void setBounceFunc(BounceFunc func);
    int itemIndex() const { return m_index; }
    void setItemIndex(int index);
    void rollToIndex(int index);
    void rollForward();
    void rollBackward();
    ValueAnimator<int> *animator() { return &m_anim; }
    const ValueAnimator<int> *animator() const { return &m_anim; }
    void setScrollBar(class AbstractBar *bar);
    void setAutoFade(bool isAuto);
    void setSwitchBuffer(bool enabled);
    void setArcEnable(bool enabled);

    virtual void paintEvent(PaintEvent *event);

    Signal<CardEvent, int> onchange;

protected:
    virtual bool event(Event *event);
    virtual bool gestureEvent(GestureEvent *event);
    virtual void changeEvent(CardEvent event);
    virtual bool encoderEvent(EncoderEvent* event);

private:
    void updateChild(ChildEvent *event);
    void onAnimFinished();
    void emitDropFirst();
    void emitDropLast();
    void emitTouchStart();
    void emitTouchRelease();
    void resetDropFlags();
    void updateProgress(int progress);

private:
    int m_index, m_start, m_offset, m_touchOffset;
    uint32_t m_dropFisrt : 1;
    uint32_t m_dropLast : 1;
    uint32_t m_inTouch : 1;
    uint32_t m_isAutoFade : 1;
    uint32_t m_isArc : 1;
    Direction m_direction;
    AbstractSwitcher *switcher;
    const AbstractScrollComputer *scrollComputer;
    ValueAnimator<int> m_anim;
    BounceFunc bounce;
    class AbstractBar *m_scrollBar;
    friend AbstractScrollComputer;
};

PERSIMMON_NAMESPACE_END
