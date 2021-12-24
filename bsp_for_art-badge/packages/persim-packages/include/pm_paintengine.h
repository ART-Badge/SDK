#pragma once

#include "pm_internal.h"
#include "pm_color.h"
#include "pm_rect.h"
#include "pm_font.h"

PERSIMMON_NAMESPACE_BEGIN

class Pen;
class Brush;
class Texture;
class Transform;
class VectorPath;
class Font;

class PaintEngine
{
public:
    virtual ~PaintEngine() {}
    virtual void drawPoints(const Point *points, int count, const Pen &pen)                  = 0;
    virtual void drawLines(const Line *lines, int count, const Pen &pen)                     = 0;
    virtual void drawRects(const Rect *rects, int count, const Brush &brush)                 = 0;
    virtual void drawPath(const VectorPath &path, const Pen &pen)                            = 0;
    virtual void fillPath(const VectorPath &path, const Brush &brush)                        = 0;
    virtual void drawTexture(const Rect &dr, const Texture &, const Rect &sr, const Brush &) = 0;

    const Rect &clip() const { return _clip; }
    const Point &origin() const { return _origin; }
    const Transform *transform() const { return _transform; }
    int opacity() const { return _opacity; }

protected:
    PaintEngine();

private:
    Rect _clip;                  // clip area
    Point _origin;               // paint origin
    const Transform *_transform; // transform matrix
    int _opacity;                // constant alpha
    friend class Painter;
};

PERSIMMON_NAMESPACE_END
