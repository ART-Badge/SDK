#pragma once

#include "pm_internal.h"
#include <memory>
#include <iterator>
#include <algorithm>

PERSIMMON_NAMESPACE_BEGIN

template<typename T, class _Alloc = std::allocator<T> > class Vector : private _Alloc
{
public:
    typedef T value_type;
    typedef _Alloc allocator_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef typename _Alloc::size_type size_type;
    typedef typename _Alloc::difference_type difference_type;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit Vector(const allocator_type &alloc = allocator_type())
        : allocator_type(alloc), _first(0), _last(0), _end(0)
    {
    }
    explicit Vector(size_type n, const_reference v = value_type(),
                    const allocator_type &alloc = allocator_type())
        : allocator_type(alloc)
    {
        initialize(n, v);
    }
    template<class InputIterator>
    Vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type())
        : allocator_type(alloc), _first(0), _last(0), _end(0)
    {
        insert(begin(), first, last);
    }
    Vector(const Vector &vec) : allocator_type(vec)
    {
        _first = allocator_type::allocate(vec.size());
        _last  = std::uninitialized_copy(vec._first, vec._last, _first);
        _end   = _last;
    }

    ~Vector()
    {
        destroy(_first, _last);
        deallocate();
    }

    Vector &operator=(const Vector &vec)
    {
        if (&vec != this)
            assign(vec.begin(), vec.end());
        return *this;
    }

    iterator begin() { return _first; }
    iterator end() { return _last; }
    const_iterator begin() const { return _first; }
    const_iterator end() const { return _last; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }
    size_type size() const { return size_type(_last - _first); }
    size_type capacity() const { return size_type(_end - _first); }
    bool empty() const { return _first == _last; }
    reference operator[](size_type n) { return _first[n]; }
    const_reference operator[](size_type n) const { return _first[n]; }
    reference at(size_type n) { return _first[n]; }
    const_reference at(size_type n) const { return _first[n]; }
    reference front() { return *_first; }
    const_reference front() const { return *_first; }
    reference back() { return *(_last - 1); }
    const_reference back() const { return *(_last - 1); }
    pointer data() { return _first; }
    const_pointer data() const { return _first; }

    void push_back(const_reference value)
    {
        if (_last != _end)
            allocator_type::construct(_last++, value);
        else
            insert(end(), value);
    }
    void pop_back() { allocator_type::destroy(--_last); }
    iterator erase(iterator first, iterator last)
    {
        iterator i = std::copy(last, _last, first);
        destroy(i, _last);
        _last = _last - (last - first);
        return first;
    }
    iterator erase(iterator position)
    {
        if (position + 1 != end())
            std::copy(position + 1, end(), position);
        allocator_type::destroy(--_last);
        return position;
    }
    void clear() { erase(begin(), end()); }

    template<class InputIterator> void assign(InputIterator first, InputIterator last)
    {
        clear();
        insert(begin(), first, last);
    }
    void assign(size_type n, const_reference value)
    {
        clear();
        insert(begin(), n, value);
    }

    void swap(Vector &vec)
    {
        std::swap(_first, vec._first);
        std::swap(_last, vec._last);
        std::swap(_end, vec._end);
    }

    void insert(iterator position, size_type n, const_reference value)
    {
        if (n != 0)
        {
            if (capacity() >= size() + n)
            {
                const size_type tails = end() - position;
                if (tails > n)
                {
                    std::uninitialized_copy(_last - n, _last, _last);
                    std::copy_backward(position, end() - n, _last);
                    std::fill(position, position + n, value);
                }
                else
                {
                    std::uninitialized_fill_n(_last, n - tails, value);
                    std::uninitialized_copy(position, end(), position + n);
                    std::fill(position, end(), value);
                }
                _last += n;
            }
            else
            {
                const size_type len = size() + pmax(size(), n);
                pointer first       = allocator_type::allocate(len);
                pointer last        = std::uninitialized_copy(begin(), position, first);
                std::uninitialized_fill_n(last, n, value);
                last = std::uninitialized_copy(position, end(), last + n);
                destroy(_first, _last);
                deallocate();
                _first = first;
                _last  = last;
                _end   = first + len;
            }
        }
    }

    template<class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last)
    {
        if (first != last)
        {
            size_type n = std::distance(first, last);
            if (capacity() >= size() + n)
            {
                const size_type tails = end() - position;
                if (tails > n)
                {
                    std::uninitialized_copy(_last - n, _last, _last);
                    std::copy_backward(position, end() - n, _last);
                    std::copy(first, last, position);
                }
                else
                {
                    InputIterator it(first);
                    std::advance(it, tails);
                    std::uninitialized_copy(it, last, _last);
                    std::uninitialized_copy(position, end(), position + n);
                    std::copy(first, it, position);
                }
                _last += n;
            }
            else
            {
                const size_type len = size() + pmax(size(), n);
                pointer first_      = allocator_type::allocate(len);
                pointer last_       = std::uninitialized_copy(begin(), position, first_);
                last_               = std::uninitialized_copy(first, first + n, last_);
                last_               = std::uninitialized_copy(position, end(), last_);
                destroy(_first, _last);
                deallocate();
                _first = first_;
                _last  = last_;
                _end   = first_ + len;
            }
        }
    }

    iterator insert(iterator position, const_reference value)
    {
        if (_last != _end)
        {
            if (position == end())
            {
                allocator_type::construct(_last++, value);
            }
            else
            {
                allocator_type::construct(_last, *(_last - 1));
                std::copy_backward(position, end() - 1, _last);
                ++_last;
                *position = value;
            }
        }
        else
        {
            const size_type len = size() ? size() * 2 : 1;
            pointer first       = allocator_type::allocate(len);
            pointer last        = std::uninitialized_copy(begin(), position, first);
            allocator_type::construct(last, value);
            last     = std::uninitialized_copy(position, end(), last + 1);
            position = last;
            destroy(_first, _last);
            deallocate();
            _first = first;
            _last  = last;
            _end   = _first + len;
        }
        return position;
    }

    void resize(size_type newsize, const_reference value = value_type())
    {
        if (newsize < size())
            erase(begin() + newsize, end());
        else
            insert(end(), newsize - size(), value);
    }

    void reserve(size_type n)
    {
        if (n > capacity())
        {
            pointer first = allocator_type::allocate(n);
            pointer last  = std::uninitialized_copy(_first, _last, first);
            destroy(_first, _last);
            deallocate();
            _first = first;
            _last  = last;
            _end   = _first + n;
        }
    }

private:
    void initialize(size_type n, const_reference value)
    {
        _first = alloc_and_fill(n, value);
        _last  = _first + n;
        _end   = _last;
    }
    pointer alloc_and_fill(size_type n, const_reference v)
    {
        pointer data = allocator_type::allocate(n);
        std::uninitialized_fill_n(data, n, v);
        return data;
    }
    void deallocate()
    {
        if (_first)
            allocator_type::deallocate(_first, _end - _first);
    }
    void destroy(pointer first, pointer last)
    {
        for (; first < last; ++first) allocator_type::destroy(first);
    }

    allocator_type get_allocator() const { return *this; }

private:
    pointer _first, _last, _end;
};

template<class T, class Alloc>
bool operator==(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, class Alloc>
bool operator!=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template<class T, class Alloc>
bool operator<(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator<=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return !(rhs < lhs);
}

template<class T, class Alloc>
bool operator>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return rhs < lhs;
}

template<class T, class Alloc>
bool operator>=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return !(lhs < rhs);
}

PERSIMMON_NAMESPACE_END
