#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

class SharedData
{
public:
    enum SharedType
    {
        Writable  = 0,
        Immutable = 1,
        Static    = 3
    };
    SharedData(SharedType type = Writable);
    virtual ~SharedData();

    void ref();
    bool deref();
    bool isShared() const;
    bool isWritable() const;
    bool isStatic() const;
    int refcnt() const;
    virtual SharedData *copy() const = 0;

protected:
    void setWritable(bool status);

private:
    // bit [31:30]: type
    // bit [29: 0]: refcnt
    uint32_t m_refcnt;
};

class SharedDataRefBase
{
public:
    bool detach();
    void swap(SharedDataRefBase &ref);

    bool operator==(const SharedDataRefBase &ref) const { return m_data == ref.m_data; }
    bool operator!=(const SharedDataRefBase &ref) const { return m_data != ref.m_data; }
    bool operator<(const SharedDataRefBase &ref) const { return m_data < ref.m_data; }
    bool operator<=(const SharedDataRefBase &ref) const { return m_data <= ref.m_data; }
    bool operator>(const SharedDataRefBase &ref) const { return m_data > ref.m_data; }
    bool operator>=(const SharedDataRefBase &ref) const { return m_data >= ref.m_data; }

protected:
    SharedDataRefBase(SharedData *data);
    SharedDataRefBase(const SharedDataRefBase &ref);
    ~SharedDataRefBase();

    SharedData *data() const { return m_data; }
    void assign(const SharedDataRefBase &ref);
    operator bool() const { return m_data != nullptr; }

private:
    SharedData *m_data;
};

template<class T> class SharedDataRef : public SharedDataRefBase
{
public:
    SharedDataRef(T *data = nullptr) : SharedDataRefBase(static_cast<SharedData *>(data)) {}
    SharedDataRef(const SharedDataRef &ref) : SharedDataRefBase(ref) {}
    T *data() const { return static_cast<T *>(SharedDataRefBase::data()); }
    T *operator->() const { return static_cast<T *>(SharedDataRefBase::data()); }
    T &operator*() const { return *operator->(); }
    SharedDataRef &operator=(const SharedDataRef &ref)
    {
        assign(ref);
        return *this;
    }
    using SharedDataRefBase::operator bool;
};

template<typename T> class ReferenceValue
{
    struct Data
    {
        int _refcnt;
        T _value;
        Data(const T &v) : _refcnt(1), _value(v) {}
    };

public:
    explicit ReferenceValue(const T &v = T()) : _d(new Data(v)) { PM_ASSERT(_d); }
    ReferenceValue(const ReferenceValue &ref) : _d(ref._d) { ++_d->_refcnt; }
    ~ReferenceValue()
    {
        if (!--_d->_refcnt)
            delete _d;
    }
    ReferenceValue &operator=(const T &v)
    {
        _d->_value = v;
        return *this;
    }
    T &data() { return _d->_value; }
    const T &data() const { return _d->_value; }
    T &operator*() { return data(); }
    const T &operator*() const { return data(); }
    T *operator->() { return &data(); }
    const T *operator->() const { return &data(); }

private:
    ReferenceValue &operator=(const ReferenceValue &ref);
    Data *_d;
};

template<typename T> class ReferencePointer : public ReferenceValue<T *>
{
public:
    explicit ReferencePointer(T *p = nullptr) : ReferenceValue<T *>(p) {}
    T *data() { return ReferenceValue<T *>::data(); }
    const T *data() const { return ReferenceValue<T *>::data(); }
    T &operator*() { return *data(); }
    const T &operator*() const { return *data(); }
    T *operator->() { return data(); }
    const T *operator->() const { return data(); }
    operator bool() const { return data(); }
    ReferencePointer &operator=(T *p)
    {
        ReferenceValue<T *>::operator=(p);
        return *this;
    }
};

PERSIMMON_NAMESPACE_END
