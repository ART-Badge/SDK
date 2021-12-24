#pragma once

#include "pm_internal.h"
#include "pm_shareddata.h"
#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

class FontDriver;

class Font
{
public:
    Font();
    Font(const String &family, float size = 12, int weight = 400, bool italic = false);
    Font(const Font &font);
    bool isNull() const;
    ~Font();

    const String family() const;
    float pixelSize() const;
    float pointSize() const;

    void setFamily(const String &family);
    void setPixelSize(float size);
    void setPointSize(float size);

    static Font fromDefault();

private:
    void updateAttrs(const struct FontAttribute &attr, int req);

private:
    SharedDataRef<FontDriver> m_ref;
    friend class FontMetrics;
    friend class FontDrawer;
    friend class TextLayout;
    friend class VectorPath;
};

PERSIMMON_NAMESPACE_END
