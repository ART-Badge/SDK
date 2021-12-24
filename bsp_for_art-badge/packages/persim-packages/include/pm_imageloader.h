#pragma once

#include "pm_internal.h"
#include "pm_image.h"
#include "pm_iodevice.h"
#include "pm_registerlist.h"
#include <map>

PERSIMMON_NAMESPACE_BEGIN

class ImageLoader
{
public:
    ImageLoader(const char *type);
    virtual ~ImageLoader();
    virtual bool load(Image::Data &data, IODevice *dev, bool withPixel) = 0;

protected:
    enum BlockMode
    {
        AutoFreePixels,
        DontFreePixels
    };
    void setSize(Image::Data &data, int width, int height);
    void setPixelsData(Image::Data &data, uint8_t *pixels, int pitch,
                       BlockMode mode = AutoFreePixels);
    void setLutData(Image::Data &data, uint32_t *lut, int size);
    void setPixelFormat(Image::Data &data, int pixfmt);

private:
    ImageLoader *next;
    const char *tname;
    friend class ImageLoaderManager;
};

class ImageLoaderManager
{
public:
    ~ImageLoaderManager();
    void registered(ImageLoader *loader);
    void unregister(const String &type);
    bool loadMetrics(Image::Data &data, IODevice *dev);
    bool load(Image::Data &data, IODevice *dev);
    static ImageLoaderManager *instance();

protected:
    ImageLoaderManager();

private:
    bool loadMode(Image::Data &data, IODevice *dev, bool withPixel);
    typedef RegisterList<ImageLoader, &ImageLoader::next> reglist;
    reglist m_list;
    static ImageLoaderManager *_instance;
};

PERSIMMON_NAMESPACE_END
