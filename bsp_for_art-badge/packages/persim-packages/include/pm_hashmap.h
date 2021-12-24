#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

class HashMapHelper
{
public:
    HashMapHelper() {}
    virtual uint32_t hash(void *object) const = 0;
    virtual void *&next(void *object) const   = 0;
};

class HashMapFinder
{
public:
    HashMapFinder() {}
    virtual bool match(const void *object) const = 0;
    uint32_t hashcode;
};

template<typename T> class HashMapHelperDefault : public HashMapHelper
{
public:
    HashMapHelperDefault() {}
    virtual uint32_t hash(void *object) const { return reinterpret_cast<T *>(object)->hashcode(); }
    virtual void *&next(void *object) const { return reinterpret_cast<T *>(object)->_hashnext; }
};

class HashMapCore
{
public:
    class iterator
    {
    public:
        void *data();
        iterator &operator++();
        bool hasNext() const { return slot < end; }

    private:
        const HashMapHelper *helper;
        void **slot, *prev;
        void **end;
        friend HashMapCore;
    };

    HashMapCore(const HashMapHelper *helper);
    ~HashMapCore();
    float loadFactorLimit() const;
    void setLoadFactorLimit(float limit);
    inline bool empty() const { return !size(); }
    inline size_t size() const { return _size; }
    void reduce();
    bool insert(void *node);
    bool remove(void *node);
    void *remove(const HashMapFinder &finder);
    iterator remove(iterator it);
    void *find(const HashMapFinder &finder) const;
    iterator begin() const;

private:
    void rehash(size_t size);
    bool insert_p(void *node);

private:
    const HashMapHelper *helper;
    size_t _slotcnt, _size, _loadLimit;
    void **slots;
};

template<typename T> class HashMap : public HashMapCore
{
public:
    class iterator : public HashMapCore::iterator
    {
    public:
        iterator() {}
        iterator(const HashMapCore::iterator &iter) { HashMapCore::iterator::operator=(iter); }
        T *data() { return reinterpret_cast<T *>(HashMapCore::iterator::data()); }
        inline T &operator*() { return *data(); }
        inline T *operator->() { return data(); }
    };
    HashMap(const HashMapHelper *helper) : HashMapCore(helper) {}
    bool insert(T *node) { return HashMapCore::insert(node); }
    bool remove(T *node) { return HashMapCore::remove(node); }
    T *remove(const HashMapFinder &finder)
    {
        return reinterpret_cast<T *>(HashMapCore::remove(finder));
    }
    iterator remove(const iterator &it) { return HashMapCore::remove(it); }
    T *find(const HashMapFinder &finder) const
    {
        return reinterpret_cast<T *>(HashMapCore::find(finder));
    }
    iterator begin() const { return HashMapCore::begin(); }
};

PERSIMMON_NAMESPACE_END
