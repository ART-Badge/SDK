#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

template<typename T> class DoublyListHelperDefault
{
public:
    inline T *&next(T *node) const { return node->_listnext; }
    inline T *&prev(T *node) const { return node->_listprev; }
};

template<typename T, class Helper = DoublyListHelperDefault<T> > class DoublyList
{
public:
    class iterator
    {
    public:
        iterator(T *node) : _ptr(node) {}
        T &operator*() const { return *_ptr; }
        T *operator->() const { return _ptr; }
        iterator &operator++()
        {
            _ptr = Helper().next(_ptr);
            return *this;
        }
        iterator operator++(int)
        {
            T *p = _ptr;
            _ptr = Helper().next(_ptr);
            return p;
        }
        iterator &operator--()
        {
            _ptr = Helper().prev(_ptr);
            return *this;
        }
        iterator operator--(int)
        {
            T *p = _ptr;
            _ptr = Helper().prev(_ptr);
            return p;
        }
        bool operator==(const iterator &node) const { return _ptr == node._ptr; }
        bool operator!=(const iterator &node) const { return !(*this == node); }
        T *data() const { return _ptr; }

    private:
        T *_ptr;
    };

    DoublyList(const Helper &helper = Helper()) : _first(nullptr), _last(nullptr) {}
    bool empty() const { return _first == nullptr; }
    T *front() const { return _first; }
    T *back() const { return _last; }
    void push_back(T *node) { insert(end(), node); }
    void push_front(T *node) { insert(begin(), node); }
    void pop_back()
    {
        PM_ASSERT(_last);
        remove(_last);
    }
    void pop_front()
    {
        PM_ASSERT(_first);
        remove(_first);
    }
    void insert(const iterator &position, T *node)
    {
        PM_ASSERT(node != nullptr);
        T *prev, *next = position.data();
        if (next == _first)
        { // insert to first node
            prev   = nullptr;
            _first = node;
        }
        if (next == nullptr)
        { // insert to last node
            prev  = _last;
            _last = node;
        }
        else
        {
            prev = Helper().prev(next);
        }
        Helper().next(node) = next;
        Helper().prev(node) = prev;
        if (prev)
            Helper().next(prev) = node;
        if (next)
            Helper().prev(next) = node;
    }
    void remove(const iterator &position)
    {
        T *node = position.data();
        PM_ASSERT(node != nullptr);
        PM_ASSERT(_first || _last);
        T *prev = Helper().prev(node);
        T *next = Helper().next(node);
        if (prev)
            Helper().next(prev) = next;
        if (next)
            Helper().prev(next) = prev;
        if (node == _first)
            _first = next;
        if (node == _last)
            _last = prev;
    }
    iterator erase(const iterator &position)
    {
        iterator next = position.data();
        PM_ASSERT(position.data() != nullptr);
        ++next;
        remove(position);
        return next;
    }
    iterator begin() const { return iterator(front()); }
    iterator end() const { return iterator(nullptr); }
    // iterator rbegin() const { return iterator(nullptr); }
    // iterator rend() const { return iterator(back()); }

private:
    T *_first, *_last;
};

PERSIMMON_NAMESPACE_END
