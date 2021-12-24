/*
 * File      : pm_dotIndicator.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>

PERSIMMON_NAMESPACE_BEGIN

class DotIndicator : public Widget
{
public:
    PM_DECLARE_CLASSNAME(DotIndicator)

    DotIndicator();
    DotIndicator(const Image &norImg, const Image &selImg, int num, int gap);
    virtual ~DotIndicator();

    void setNorImage(const Image &nor);
    void setSelImage(const Image &sel);
    void setDirection(Direction t = Direction::Horizontal);
    void setSelNum(int num);
    void setTotalNum(int num);
    void setGap(int gap);

    int getSelNum(void);

protected:
    virtual void paintEvent(PaintEvent *event);

private:
    Direction _direction;
    Image _norImage, _selImage;
    int totalNum, selNum, gap;
};

PERSIMMON_NAMESPACE_END
