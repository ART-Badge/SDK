#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

class ListHelper
{
public:
    ListHelper() {}
    virtual void *&next(void *object) const = 0;
    virtual void *&prev(void *object) const = 0;
};

template<typename T> class ListHelperDefault : public ListHelper
{
public:
    ListHelperDefault() {}
    virtual void *&next(void *object) const { return reinterpret_cast<T *>(object)->_listnext; }
    virtual void *&prev(void *object) const { return reinterpret_cast<T *>(object)->_listprev; }
};

class ListCore
{
public:
    ListCore(const ListHelper *helper);
    bool empty() const { return list == nullptr; }
    void *back() const;
    void *front() const { return list; }
    void push_back(void *node);
    void push_front(void *node);
    void pop_back();
    void pop_front();
    void insert(void *prev, void *node);
    void remove(void *node);
    void *prev(void *node);
    void *next(void *node);

private:
    const ListHelper *helper;
    void *list;
};

template<typename T> class List : public ListCore
{
public:
    List(const ListHelper *helper) : ListCore(helper) {}
    T *back() const { return reinterpret_cast<T *>(ListCore::back()); }
    T *front() const { return reinterpret_cast<T *>(ListCore::front()); }
    void push_back(T *node) { ListCore::push_back(node); }
    void push_front(T *node) { ListCore::push_front(node); }
    void insert(T *prev, T *node) { ListCore::insert(prev, node); }
    void remove(T *node) { ListCore::remove(node); }
};

PERSIMMON_NAMESPACE_END
