#pragma once

#include "pm_internal.h"
#include "pm_string.h"
#include "pm_registerlist.h"

PERSIMMON_NAMESPACE_BEGIN

class FontDriver;
struct FontAttribute;

class FontLoader
{
public:
    virtual ~FontLoader() {}
    virtual FontDriver *load(const String &face, const FontAttribute &attr) = 0;

protected:
    FontLoader() : next(0) {}

private:
    FontLoader *next;
    friend class FontLoaderManager;
};

class FontLoaderManager
{
public:
    ~FontLoaderManager();
    void registered(FontLoader *loader);
    // void unregister(FontLoader *loader);
    static FontLoaderManager *instance();

    FontDriver *lookup(const String &family, const FontAttribute &attr);
    FontDriver *loadsimple(const String &family, const FontAttribute &attr);
    FontDriver *load(const String &family, const FontAttribute &attr);

protected:
    FontLoaderManager();

private:
    typedef RegisterList<FontLoader, &FontLoader::next> RegList;
    RegList _list;
    static FontLoaderManager *_instance;
};

PERSIMMON_NAMESPACE_END
