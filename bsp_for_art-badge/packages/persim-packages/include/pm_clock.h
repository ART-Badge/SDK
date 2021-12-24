/*
 * File      : pm_clock.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-29     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>
#include <pm_widget.h>
#include <pm_transform.h>
#include <pm_pixmap.h>

namespace persim
{

template<class> class ValueAnimator;

class Clock : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Clock)

    Clock();
    virtual ~Clock();

    void enableMillisecond(bool enabled);
    void setTime(int time);
    void setHourImage(const Image &img);
    void setMinuteImage(const Image &img);
    void setSecondImage(const Image &img);
    void setCenterImage(const Image &img);

    void setHourAnchor(const PointF &anchor);
    void setMinuteAnchor(const PointF &anchor);
    void setSecondAnchor(const PointF &anchor);

protected:
    virtual void paintEvent(PaintEvent *event);

private:
    Image m_centerImage;
    Pixmap m_hourImage, m_minuteImage, m_secondImage;
    PointF m_hourAnchor, m_minuteAnchor, m_secondAnchor;
    uint32_t m_timestamp;
    bool m_useMsec;
};

} // namespace persim
