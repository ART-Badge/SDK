/*
 * File      : animated_image.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-22     realthread   the first version
 */

#pragma once

#include <image_box.h>
#include <pm_timer.h>
#include <pm_sigslot.h>
#include <pm_vector.h>

PERSIMMON_NAMESPACE_BEGIN

class AnimatedImage : public ImageBox
{
public:
    PM_DECLARE_CLASSNAME(AnimatedImage)

    AnimatedImage();
    virtual ~AnimatedImage();

    void empty();
    void addImage(const String &path);
    void addImage(const Vector<String> &path);
    void startIndex(uint32_t index);
    void endIndex(uint32_t index);
    void startAnimation();
    void stopAnimation(bool end = false);
    void pauseAnimation();
    void setLoop(bool loop);
    bool isLoop();
    void setInterval(uint32_t ms);
    void setIndex(uint32_t i);
    uint32_t index(void) const;
    virtual void paintEvent(PaintEvent *event);
    Signal<void> end;

protected:
    void timeout();

private:
    void updateImage();

private:
    SyncTimer *_timer;
    Vector<Image> _image;
    bool _loop;
    uint32_t _start_ms, _start_index, _end_index, _current_index;
    uint32_t _interval;
};

PERSIMMON_NAMESPACE_END
