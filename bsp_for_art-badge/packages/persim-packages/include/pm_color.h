#pragma once

#include "pm_internal.h"
#include "pm_string.h"
#include "pm_image.h"

PERSIMMON_NAMESPACE_BEGIN

class Gradient;

class Color
{
public:
    Color() : m_argb(0xff000000) {}
    explicit Color(const String &name);
    Color(int r, int g, int b, int a = 255);
    explicit Color(uint32_t value) : m_argb(value) {}
    bool isTransparent() const { return m_argb < 0x01000000; }
    bool isOpaque() const { return m_argb >= 0xff000000; }
    uint32_t argb() const { return m_argb; }
    int alpha() const;
    int red() const;
    int green() const;
    int blue() const;
    String name() const;
    Color blend(int alpha) const;
    Color blend(const Color &color) const;

    Color operator*(int alpha) const { return blend(alpha); }
    Color operator*(const Color &color) const { return blend(color); }

private:
    uint32_t m_argb;
};

class Brush
{
public:
    Brush(Color color = Color()) : m_color(color) {}
    virtual ~Brush();
    Color color() const { return m_color; }
    void setColor(Color color) { m_color = color; }
    void setTexture(const Image &image) { m_image = image; }
    const Image &textureImage() const { return m_image; }

private:
    Color m_color;
    Image m_image;
};

class Pen
{
public:
    enum JoinMode
    {
        BevelJoin = 0,
        MiterJoin = 1,
        RoundJoin = 3
    };
    enum CapMode
    {
        FlatCap   = 0,
        SquareCap = 2,
        RoundCap  = 4
    };

    Pen(Color color = Color(), float size = 1);
    Color color() const { return m_brush.color(); }
    float size() const { return m_size; }
    JoinMode joinMode() const { return JoinMode(m_joinMode); }
    CapMode capMode() const { return CapMode(m_capMode); }
    void setColor(Color color) { m_brush.setColor(color); }
    void setSize(float size) { m_size = size; }
    void setLineJoin(JoinMode mode) { m_joinMode = mode; }
    void setLineCap(CapMode mode) { m_capMode = mode; }
    const Brush &brush() const { return m_brush; }
    void setBrush(const Brush &brush) { m_brush = brush; }

private:
    Brush m_brush;
    float m_size;
    uint32_t m_joinMode : 3;
    uint32_t m_capMode : 3;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Color &);
#endif

PERSIMMON_NAMESPACE_END
