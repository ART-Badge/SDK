#pragma once

#include "pm_internal.h"
#include "pm_color.h"
#include "pm_rect.h"
#include "pm_transform.h"
#include "pm_font.h"

PERSIMMON_NAMESPACE_BEGIN

class Image;
class Texture;
class PaintDevice;
class VectorPath;
class Widget;

class Painter
{
public:
    class Helper;

    explicit Painter(Widget *widget);
    explicit Painter(PaintDevice *device);
    Painter(PaintDevice *device, const Rect &clip);
    ~Painter();
    int opacity() const;
    const Pen &pen() const { return m_pen; }
    const Brush &brush() const { return m_brush; }
    const Font &font() const { return m_font; }
    float lineHeight() const;
    LayoutDirection layoutDirection();
    void setOpacity(int opacity);
    void setOpacityF(float opacity) { setOpacity(int(opacity * 255)); }
    void setPen(const Pen &pen) { m_pen = pen; }
    void setBrush(const Brush &brush) { m_brush = brush; }
    void setFont(const Font &font) { m_font = font; }
    void setTransform(const Transform &transform);
    void setLineHeight(float lineHeight);
    void setLayoutDirection(LayoutDirection direction);
    void drawPoint(const Point &point);
    void drawPoint(const Point &point, const Pen &pen);
    void drawLine(const Point &p1, const Point &p2);
    void drawLine(const Point &p1, const Point &p2, const Pen &pen);
    void drawLine(const Line &line);
    void drawLine(const Line &line, const Pen &pen);
    void fillRect(const Rect &rect);
    void fillRect(const Rect &rect, const Brush &brush);
    void drawRect(const Rect &rect);
    void drawRect(const Rect &rect, const Pen &pen);
    void drawTexture(const Rect &r, const Texture &texture);
    void drawTexture(const Rect &dstRect, const Texture &texture, const Rect &srcRect);
    void drawTexture(const Point &point, const Texture &texture, const Rect &srcRect);
    void drawTexture(const Point &point, const Texture &texture);
    void drawImage(const Rect &r, const Image &image);
    void drawImage(const Rect &dstRect, const Image &image, const Rect &srcRect);
    void drawImage(const Point &point, const Image &image, const Rect &srcRect);
    void drawImage(const Point &point, const Image &image);
    void drawPath(const VectorPath &path);
    void drawPath(const VectorPath &path, const Pen &pen);
    void fillPath(const VectorPath &path);
    void fillPath(const VectorPath &path, const Brush &brush);
    void drawText(const Rect &r, const String &text, int flags = 0, const Rect &box = Rect());

private:
    void initialization();

private:
    class PaintEngine *engine;
    class PaintDevice *m_device;
    Pen m_pen;
    Brush m_brush;
    Font m_font;
    Transform m_transform;
    uint32_t m_lineHeight : 30;
    uint32_t m_layoutDir : 2;
};

PERSIMMON_NAMESPACE_END
