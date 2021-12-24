#pragma once

#include "pm_paintengine.h"

PERSIMMON_NAMESPACE_BEGIN

class PixmapPaintEngine : public PaintEngine
{
public:
    PixmapPaintEngine(Texture *device);
    virtual ~PixmapPaintEngine();
    virtual void drawPoints(const Point *points, int count, const Pen &pen);
    virtual void drawLines(const Line *lines, int count, const Pen &pen);
    virtual void drawRects(const Rect *rects, int count, const Brush &brush);
    virtual void drawPath(const VectorPath &path, const Pen &pen);
    virtual void fillPath(const VectorPath &path, const Brush &brush);
    virtual void drawTexture(const Rect &dr, const Texture &, const Rect &sr, const Brush &br);

protected:
    Texture *device;
};

class PixmapPaintEngineFactory
{
public:
    PixmapPaintEngineFactory() {}
    virtual PaintEngine *factory(Texture *device) const;

private:
    void *operator new(size_t);
    void operator delete(void *);
};

PERSIMMON_NAMESPACE_END
