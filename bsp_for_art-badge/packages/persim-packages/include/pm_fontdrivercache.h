#pragma once

#include "pm_internal.h"
#include "pm_hashmap.h"
#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

struct FontAttribute;
class FontDriver;

class FontDriverRegistry
{
public:
    ~FontDriverRegistry();
    FontDriver *lookup(const String &face, const FontAttribute &attr);
    void insert(FontDriver *driver);
    void remove(FontDriver *driver);

    class FamilyIterator
    {
    public:
        bool hasNext() const { return _it.hasNext(); }
        FontDriver *next();

    private:
        void skip();
        intptr_t _tid;
        String _family;
        HashMap<FontDriver>::iterator _it;
        friend class FontDriverRegistry;
    };
    FamilyIterator listFamily(const String &family, intptr_t type) const;

    static uint16_t hashcode(const String &face, const FontAttribute &attr);
    static FontDriverRegistry *instance();

protected:
    FontDriverRegistry();

private:
    static FontDriverRegistry *_instance;
    HashMap<FontDriver> _map;
};

PERSIMMON_NAMESPACE_END
