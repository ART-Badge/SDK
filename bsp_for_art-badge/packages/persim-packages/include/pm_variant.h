#pragma once

#include <pm_internal.h>

PERSIMMON_NAMESPACE_BEGIN

class Variant
{
    enum
    {
        stack_cache_size = 32
    };
    struct AdapterBase
    {
        typedef void (*modifier)(AdapterBase *, void *);
        AdapterBase(modifier m = Variant::modify) : refcnt(1), modify(m) {}
        int refcnt;
        const modifier modify;
    };
    template<typename T> struct Adapter : public AdapterBase
    {
        T value;
        Adapter(const T &v) : AdapterBase(modify), value(v) {}
        static void modify(AdapterBase *p, void *newptr) { delete (Adapter<T> *)p; }
    };
    static void modify(AdapterBase *, void *);
    AdapterBase *_d;
    uint8_t _cache[stack_cache_size];

public:
    typedef AdapterBase::modifier Type;
    Variant() : _d() {}
    template<typename T> explicit Variant(const T &v) : _d(new Adapter<T>(v)) {}
    Variant(const Variant &rhs);
    ~Variant();
    bool isNull() const { return !_d || _d->modify == modify; }
    template<typename T> bool is() const { return _d && _d->modify == &Adapter<T>::modify; }
    template<typename T> const T &to(const T &def = T()) const;
    Type type() const { return _d ? _d->modify : Variant::modify; };
    template<typename T> static Variant::Type type() { return Adapter<T>::modify; }
    bool isNull() { return type() == Variant::modify; }
    Variant &operator=(const Variant &rhs);
};

template<typename T> const T &Variant::to(const T &def) const
{
    if (is<T>())
        return static_cast<Adapter<T> *>(_d)->value;
    return def;
}

PERSIMMON_NAMESPACE_END
