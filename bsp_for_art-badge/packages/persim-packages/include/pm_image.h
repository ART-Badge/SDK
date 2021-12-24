#pragma once

#include "pm_texture.h"
#include "pm_shareddata.h"
#include "pm_iodevice.h"

PERSIMMON_NAMESPACE_BEGIN

class Image
{
public:
    class Data;
    class Texture;

    Image();
    explicit Image(const String &path);
    explicit Image(IODevice *dev);
    Image(const Image &image);
    virtual ~Image();

    bool isEmpty() const { return size().isEmpty(); }
    int width() const { return size().width(); }
    int height() const { return size().height(); }
    Size size() const;
    Rect rect() const { return Rect(Point(), size()); }
    inline Texture texture() const;
    void prepare() const;
    Image &operator=(const Image &image);

private:
    SharedDataRef<Data> m_ref;
    friend class ImageUnitTestSuite;
};

class Image::Texture : public persim::Texture
{
public:
    Texture(const Image &image);
    Texture(const Texture &texture);
    virtual ~Texture();
    const Image &image() const { return m_image; }

    Texture &operator=(const Texture &rhs);

private:
    Image m_image;
};

inline Image::Texture Image::texture() const
{
    return Texture(*this);
}

PERSIMMON_NAMESPACE_END
