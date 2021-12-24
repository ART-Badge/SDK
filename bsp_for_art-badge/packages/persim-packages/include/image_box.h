/*
 * File      : image_box.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_image.h>
#include <pm_transformoperations.h>

namespace persim
{

class ImageBox : public Widget
{
public:
    PM_DECLARE_CLASSNAME(ImageBox)

    ImageBox();
    virtual ~ImageBox();

    void setImage(const Image &image);
    const Image &getImage(void);
    float opacity() { return _opacity * (1.f / 255); }
    void setOpacity(float opacity);
    const TransformOperations &transform() { return _transform; }
    void setTransform(const TransformOperations &transform);

    virtual void paintEvent(PaintEvent *event);

    virtual Property property(const char *name);
    virtual Vector<PropertyDescriptor> propertyDescriptors() const;

protected:
    Image image;
    int _opacity;
    TransformOperations _transform;
};

} // namespace persim
