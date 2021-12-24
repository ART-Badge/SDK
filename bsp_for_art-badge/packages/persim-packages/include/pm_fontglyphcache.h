#pragma once

#include "pm_internal.h"
#include "pm_lrucache.h"

PERSIMMON_NAMESPACE_BEGIN

class FontDriver;
class GlyphBitmap;

class FontGlyphCache
{
public:
    ~FontGlyphCache();
    void setSizeLimit(size_t limit);
    const GlyphBitmap *lookup(FontDriver *drv, uint32_t code);
    void flush(const FontDriver *drv);
    void flush();
    static FontGlyphCache *instance();

protected:
    FontGlyphCache(size_t limit);

private:
    class Node;
    class Filter;
    LRUCache<Node> lru;
    static FontGlyphCache *_instance;
    friend class FontGlyphCachePrivate;
};

PERSIMMON_NAMESPACE_END
