#pragma once

#include <pm_widget.h>

PERSIMMON_NAMESPACE_BEGIN

template<class> class ValueAnimator;

class AbstractScrollArea : public Widget
{
public:
    explicit AbstractScrollArea(Widget *parent = nullptr);
    virtual ~AbstractScrollArea();

protected:
    virtual bool gestureEvent(GestureEvent *event);

private:
    int m_offset, m_length;
};

PERSIMMON_NAMESPACE_END
