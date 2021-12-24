/*
 * File      : pm_pixmapanimator.h
 * COPYRIGHT (C) 2012-2021, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include "pm_pixmap.h"
#include "pm_transformoperations.h"

PERSIMMON_NAMESPACE_BEGIN

class Variant;
class AnimatorGroup;

#ifdef PIXMAP_ANIM_SNAPSHOT_PIXEL_FORMAT_RGB565
#define PIXMAP_ANIM_SNAPSHOT_PIXEL_FORMAT   PixelFormat::RGB565
#else
#define PIXMAP_ANIM_SNAPSHOT_PIXEL_FORMAT   PixelFormat::ARGB8888
#endif /* PIXMAP_ANIM_SNAPSHOT_PIXEL_FORMAT_RGB565 */

class PixmapAnimator : public Widget
{
public:
    PM_DECLARE_CLASSNAME(PixmapAnimator);

    PixmapAnimator();
    virtual ~PixmapAnimator();

    void paintWidget(PixmapAnimator *w          = nullptr,
                     PixelFormat::Format format = PIXMAP_ANIM_SNAPSHOT_PIXEL_FORMAT);
    void paintScreen(PixelFormat::Format format = PIXMAP_ANIM_SNAPSHOT_PIXEL_FORMAT);
    float opacity() const { return _opacity; }
    void setOpacity(float opacity);
    float scaleX() const { return _scaleX; }
    float scaleY() const { return _scaleY; }
    void setScale(PointF scale);
    PointF center() const { return _center; }
    void setCenter(PointF center);
    const TransformOperations &transform() const { return _transform; }
    void setTransform(const Variant &tr);
    void setTransform(const TransformOperations &transform);

    virtual Property property(const char *name);
    virtual Vector<PropertyDescriptor> propertyDescriptors() const;

    void drawSnapshot(bool draw = true);
    void releaseSnapshot(void);
    void setAnimatorGroup(AnimatorGroup *anim = nullptr);

protected:
    virtual void paintEvent(PaintEvent *event);
    Pixmap _pixmap;
    bool _drawSnapshot;

private:
    float _opacity, _scaleX, _scaleY;
    PointF _center;
    TransformOperations _transform;
    AnimatorGroup *_animGroup;
};

PERSIMMON_NAMESPACE_END
