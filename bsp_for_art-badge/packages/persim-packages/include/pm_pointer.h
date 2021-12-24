#pragma once

#include "pm_internal.h"
#include "pm_vector.h"

PERSIMMON_NAMESPACE_BEGIN

class GuardedObject
{
public:
    GuardedObject() : d() {}
    ~GuardedObject();

private:
    struct Guard
    {
        int refcnt;
        GuardedObject *ptr;
        Guard(GuardedObject *v) : refcnt(1), ptr(v) {}
        ~Guard() { PM_CRT_ASSERT(ptr == nullptr); }
        Guard* ref()
        {
            refcnt++;
            return this;
        }
    };
    Guard *d;
    Guard* guardRef();
    template<class> friend class Pointer;
};

template<class T> class Pointer
{
public:
    Pointer() : d() {}
    explicit Pointer(T *p) : d(p ? static_cast<GuardedObject *>(p)->guardRef() : nullptr) {}
    Pointer(const Pointer &p) : d(p ? p.d->ref() : nullptr) {}
    ~Pointer()
    {
        if (d && !--d->refcnt)
            delete d;
    }

    T *data() const { return d ? static_cast<T *>(d->ptr) : nullptr; }
    void assign(const Pointer &ptr)
    {
        if (d && !--d->refcnt)
            delete d;
        d = ptr ? ptr.d->ref() : nullptr;
    }
    void assign(T *ptr) { assign(Pointer(ptr)); }
    Pointer &operator=(const Pointer &rhs)
    {
        assign(rhs);
        return *this;
    }

    operator T *() const { return data(); }
    T &operator*() const { return *data(); }
    T *operator->() const { return data(); }
    operator bool() const { return d && d->ptr; }
    bool operator==(const Pointer &rhs) const { return data() == rhs.data(); }
    bool operator!=(const Pointer &rhs) const { return !(*this == rhs); }
    bool operator<(const Pointer &rhs) const { return data() < rhs.data(); }
    bool operator<=(const Pointer &rhs) const { return !(rhs < *this); }
    bool operator>(const Pointer &rhs) const { return rhs < *this; }
    bool operator>=(const Pointer &rhs) const { return !(*this < rhs); }

private:
    GuardedObject::Guard *d;
    friend class GuardedObject;
};

PERSIMMON_NAMESPACE_END
