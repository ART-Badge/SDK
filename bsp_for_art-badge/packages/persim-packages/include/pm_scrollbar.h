/*
 * File      : pm_scrollbar.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */
#ifndef __PM_SCROLLBAR_H
#define __PM_SCROLLBAR_H

#include <pm_widget.h>

namespace persim
{

template<class> class ValueAnimator;

class AbstractBar : public Widget
{
public:
    PM_DECLARE_CLASSNAME(AbstractBar)

    AbstractBar();
    virtual ~AbstractBar();

    int value() const { return m_value; }
    int pageStep() const { return m_pageStep; }
    int minimum() const { return m_minimum; }
    int maximum() const { return m_maximum; }
    int range() const { return m_maximum - m_minimum; }

    void setValue(int value);
    void setPageStep(int step);
    void setMinimum(int value);
    void setMaximum(int value);
    void setRange(int minimum, int maximum);
    void setBarBackground(const Color &color);
    void fadeInOut(bool visiable);
    void setFadeTime(int time);

protected:
    Color barForeground() const;
    Color barBackground() const;
    int barAlpha() const { return m_alpha; }

private:
    void setAlpha(int alpha);

private:
    bool m_visiabled;
    int m_value, m_pageStep;
    int m_minimum, m_maximum;
    int m_alpha;
    unsigned int fadeTime;
    Color m_barBackground, m_barForeground;
    ValueAnimator<int> *m_anim;
};

class ScrollBar : public AbstractBar
{
public:
    PM_DECLARE_CLASSNAME(ScrollBar)
    ScrollBar();
    virtual ~ScrollBar();

    void setDirection(Direction direction);
    Direction direction() const { return m_direction; }
    void setLineWidth(float lineWidth);
    void drawBar(Painter *p);
    void drawBgBar(Painter *p);
    virtual void paintEvent(PaintEvent *event);

private:
    int _lineWidth;
    Direction m_direction;
};

}; // namespace persim

#endif
