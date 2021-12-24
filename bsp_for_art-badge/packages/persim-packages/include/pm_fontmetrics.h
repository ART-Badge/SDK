#pragma once

#include "pm_rect.h"
#include "pm_font.h"

PERSIMMON_NAMESPACE_BEGIN

class FontMetrics
{
public:
    FontMetrics(const Font &font);
    Rect boundingRect(const String &text, int flags, int width, int height, float lineHeight = 0);
    int width(const String &text, int length = -1);
    String elidedText(const String &text, int width) const;

    static float pointToPixel(float point);
    static float pixelToPoint(float pixel);

private:
    Font m_font;
};

PERSIMMON_NAMESPACE_END
