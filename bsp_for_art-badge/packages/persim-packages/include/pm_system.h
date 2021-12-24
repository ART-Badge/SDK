#pragma once

#include "pm_internal.h"

#define pmsys() PersimSystem::instance()

PERSIMMON_NAMESPACE_BEGIN

class Texture;
class PaintEngine;
class PaintDevice;
class Painter;
class PixmapPaintEngineFactory;

class PersimSystem
{
public:
    PersimSystem();
    virtual ~PersimSystem();
    PaintDevice *screenDevice() const { return m_screen; }
    Painter *contextPainter() const { return m_painter; }
    PaintEngine *newPixmapPaintEngine(Texture *device) const;

    void setScreenDevice(PaintDevice *device);
    void setContextPainter(Painter *painter);
    void setPixmapPaintEngineFactory(const PixmapPaintEngineFactory *factory);

    static PersimSystem *instance() { return _instance; }

private:
    static PersimSystem *_instance;
    PaintDevice *m_screen;
    Painter *m_painter;
    const PixmapPaintEngineFactory *m_ppeFactory;
};

PERSIMMON_NAMESPACE_END
