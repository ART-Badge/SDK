#pragma once

#include "pm_texture.h"
#include "pm_shareddata.h"
#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

class VectorPath;

// designed for horizontal layout only!!!
struct GlyphMetrics
{
    // Q26.6 Fixed Format
    int16_t left, top;
    int16_t height, xadvance;
};

struct FontAttribute
{
    uint32_t style : 2;
    uint32_t underline : 1;
    uint32_t overline : 1;
    uint32_t weight : 8;
    uint32_t size : 16;
    FontAttribute();
};

class GlyphBitmap : public Texture
{
public:
    GlyphBitmap();
    virtual ~GlyphBitmap();
    void resize(int width, int height, int pixfmt = PixelFormat::A8);
    static void move(GlyphBitmap &dst, GlyphBitmap &src);
    GlyphMetrics metrics;

private:
    GlyphBitmap(const GlyphBitmap &);    // non-copyable
    void operator=(const GlyphBitmap &); // non-copyable
    friend class FontDriver;
};

class FontDriver : public SharedData
{
public:
    enum FontType
    {
        Vector = 1,
        Bitmap = 2
    };
    enum Style
    {
        NormalStyle,
        ItalicStyle,
        ObliqueStyle
    };
    enum Weight
    {
        Thin       = 1, // 100
        ExtraLight = 2, // 200
        Light      = 3, // 300
        Regular    = 4, // 400
        Medium     = 5, // 500
        DemiBold   = 6, // 600
        Bold       = 7, // 700
        ExtraBold  = 8, // 800
        Black      = 9  // 900
    };
    enum AttributeRequest
    {
        SizeRequest      = 0x01,
        WeightRequest    = 0x02,
        StyleRequest     = 0x04,
        UnderlineRequest = 0x08,
        OverlineRequest  = 0x10,
        AllAttrsRequest  = 0xffff
    };

    virtual ~FontDriver();
    int type() { return m_type; }
    const String &facename() const { return m_facename; }
    uint32_t hashcode() const { return m_hashcode; }
    const FontAttribute &attributes() const { return m_attributes; }

    virtual FontDriver *copy() const                             = 0;
    virtual bool bitmapOf(uint32_t code, GlyphBitmap *bitmap)    = 0;
    virtual bool metricsOf(uint32_t code, GlyphMetrics *metrics) = 0;
    virtual bool outlineOf(uint32_t code, VectorPath *outline);

    void request(const FontAttribute &attr, int req);
    bool operator==(const FontDriver &other) const;

protected:
    FontDriver(const String &facename, int type);
    virtual void requestHandler(int req) = 0;
    static void move(FontDriver &dst, const FontDriver &src);

private:
    FontDriver(const FontDriver &);
    FontDriver &operator              =(const FontDriver &);
    virtual intptr_t _classid() const = 0;

private:
    uint32_t m_type : 2;
    uint32_t m_hashcode : 30;
    intptr_t m_rsid;
    String m_facename;
    FontAttribute m_attributes;
    void *_hashnext; // for hash-map cache
    friend class FontDriverRegistry;
    template<class> friend class HashMapHelperDefault;
};

template<class T> class FontDriverT : public FontDriver
{
protected:
    FontDriverT(const String &facename, int type) : FontDriver(facename, type) {}

protected:
    static intptr_t classid()
    {
        static const char _static_id = 0;
        return intptr_t(&_static_id);
    }

private:
    virtual intptr_t _classid() const { return classid(); }
};

inline bool operator==(const FontAttribute &a, const FontAttribute &b)
{
    return a.style == b.style && a.overline == b.overline && a.underline == b.underline
        && a.weight == b.weight && a.size == b.size;
}

inline bool operator!=(const FontAttribute &a, const FontAttribute &b)
{
    return !(a == b);
}

PERSIMMON_NAMESPACE_END
