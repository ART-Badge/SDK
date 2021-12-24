#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

template<class T, T *T::*Next = &T::next> class RegisterList
{
public:
    RegisterList() : _list(), _tail() {}
    ~RegisterList()
    {
        for (iterator it = begin(); it != end();) delete &(*it++);
    }

    void push_back(T *node)
    {
        PM_ASSERT(node);
        if (!_list)
            _list = node;
        if (_tail)
            _tail->*Next = node;
        _tail       = node;
        node->*Next = nullptr;
    }

    class iterator
    {
    public:
        iterator(T *node = nullptr) : _node(node) {}
        T &operator*() { return *_node; }
        T *operator->() { return _node; }
        iterator &operator++()
        {
            _node = _node->*Next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            _node        = _node->*Next;
            return tmp;
        }
        bool operator==(const iterator &rhs) const { return _node == rhs._node; }
        bool operator!=(const iterator &rhs) const { return !(*this == rhs); }

    private:
        T *_node;
    };

    iterator begin() const { return _list; }
    iterator end() const { return iterator(); }

private:
    T *_list, *_tail;
};

PERSIMMON_NAMESPACE_END
