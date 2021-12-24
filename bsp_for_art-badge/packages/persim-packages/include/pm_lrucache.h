#pragma once

#include "pm_internal.h"
#include "pm_hashmap.h"
#include "pm_list.h"

PERSIMMON_NAMESPACE_BEGIN

class LRUCacheHelper
{
public:
    LRUCacheHelper() {}
    virtual bool match(const void *object) const = 0;
    virtual void revoke(void *object) const      = 0;
    virtual size_t size(void *object) const      = 0;
};

class LRUCacheCore
{
public:
    LRUCacheCore(const HashMapHelper *hhlp, const ListHelper *lhlp);
    size_t count() const;
    size_t size() const;
    float loadFactorLimit() const;
    void setLoadFactorLimit(float limit);
    void sizeSizeLimit(size_t limit);
    void *find(const HashMapFinder &finder);
    void insert(void *node, const LRUCacheHelper &helper);
    void remove(void *node, const LRUCacheHelper &helper);
    void flush(const LRUCacheHelper &filter);
    void flush(size_t size, const LRUCacheHelper &filter);

private:
    HashMapCore map;
    ListCore list;
    size_t _size, _sizelimit;
};

template<typename T> class LRUCache : public LRUCacheCore
{
public:
    LRUCache(const HashMapHelper *hhlp, const ListHelper *lhlp) : LRUCacheCore(hhlp, lhlp) {}
    T *find(const HashMapFinder &finder)
    {
        return reinterpret_cast<T *>(LRUCacheCore::find(finder));
    }
    void insert(T *node, const LRUCacheHelper &helper) { LRUCacheCore::insert(node, helper); }
};

PERSIMMON_NAMESPACE_END
