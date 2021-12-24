#pragma once

#include "pm_internal.h"
#include <iterator>
#include <cstring>

PERSIMMON_NAMESPACE_BEGIN

class unicode_iterator
{
public:
    unicode_iterator(const char *ptr = nullptr) : byte(reinterpret_cast<const uint8_t *>(ptr)) {}
    uint32_t operator*() const;
    unicode_iterator &operator++();
    unicode_iterator operator++(int);
    bool operator==(const unicode_iterator &it) const { return byte == it.byte; }
    bool operator!=(const unicode_iterator &it) const { return !(*this == it); }
    const char *base() const { return (const char *)byte; }

private:
    const uint8_t *byte;
};

class String
{
    class StringData
    {
    public:
        int refcnt;
        size_t size, capacity;
        inline char *data() { return reinterpret_cast<char *>(this) + sizeof(StringData); }
        inline const char *data() const
        {
            return reinterpret_cast<const char *>(this) + sizeof(StringData);
        }
        inline char *end() { return data() + size; }
        inline const char *end() const { return data() + size; }
        inline void ref() { refcnt++; }
        inline bool deref() { return --refcnt; }
        StringData *copy() const;
        static StringData *alloc(int size);
        static void free(StringData *data);
    };

public:
    typedef const char *const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    enum CaseSensitivity
    {
        CaseSensitive,
        CaseInsensitive
    };

    String() : _d(nullptr) {}
    String(const char *s) { initialization(s, s ? std::strlen(s) : 0); }
    String(const char *s, size_t size) { initialization(s, size); }
    String(const const_iterator &begin, const const_iterator &end)
    {
        initialization(begin, end - begin);
    }
    explicit String(size_t size) { initialization(nullptr, size); }
    String(const String &string);
    ~String();

    // properties getter
    inline bool empty() const { return !size(); }
    inline size_t length() const { return size(); }
    inline size_t size() const { return _d ? _d->size : 0; }
    inline size_t capacity() const { return _d ? _d->capacity : 0; }
    inline const char *data() const { return _d ? _d->data() : ""; }
    inline char *data() { return detach() ? _d->data() : nullptr; }
    inline const char *c_str() const { return data(); }
    uint32_t hashcode() const;

    // modify operations
    void clear();
    void push_back(int c);
    void pop_back();
    void push_unicode(uint32_t code);
    void append(int c) { push_back(c); }
    void append(const char *str, size_t size);
    void append(const char *str);
    void append(const_iterator first, const_iterator last);
    void append(const String &str);
    void insert(size_t pos, const char *str, size_t size);
    void insert(size_t pos, const_iterator first, const_iterator last);
    void insert(size_t pos, const String &str);
    void remove(const_iterator first, const_iterator last);
    void resize(size_t size);
    void reserve(size_t size);

    // iterator
    inline const_iterator begin() const { return data(); }
    inline const_iterator end() const { return data() + size(); }
    inline const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    inline const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    unicode_iterator ubegin() const { return unicode_iterator(begin()); }
    unicode_iterator uend() const { return unicode_iterator(end()); }

    // match & search algorithms
    const_iterator find(int ch, CaseSensitivity sensitive = CaseSensitive) const;
    const_iterator find(const char *substr, CaseSensitivity sensitive = CaseSensitive) const;
    const_iterator find(const String &substr, CaseSensitivity sensitive = CaseSensitive) const;
    const_iterator findLast(int ch, CaseSensitivity sensitive = CaseSensitive) const;
    const_iterator findLast(const char *substr, CaseSensitivity sensitive = CaseSensitive) const;
    const_iterator findLast(const String &substr, CaseSensitivity sensitive = CaseSensitive) const;
    bool match(const_iterator first, const_iterator last, size_t offset = 0,
               CaseSensitivity sensitive = CaseSensitive) const;
    bool match(const char *substr, size_t offset = 0,
               CaseSensitivity sensitive = CaseSensitive) const
    {
        return match(substr, substr + std::strlen(substr), offset, sensitive);
    }
    bool match(const String &substr, size_t offset = 0,
               CaseSensitivity sensitive = CaseSensitive) const
    {
        return match(substr.begin(), substr.end(), offset, sensitive);
    }
    bool endWith(const_iterator first, const_iterator last,
                 CaseSensitivity sensitive = CaseSensitive) const
    {
        return match(first, last, size() - (last - first), sensitive);
    }
    bool endWith(const char *substr, CaseSensitivity sensitive = CaseSensitive) const
    {
        const size_t sublen = std::strlen(substr);
        return match(substr, substr + sublen, size() - sublen, sensitive);
    }
    bool endWith(const String &substr, CaseSensitivity sensitive = CaseSensitive) const
    {
        return match(substr, size() - substr.size(), sensitive);
    }

    // substrings
    String substr(const_iterator first, const_iterator last) const;
    String substr(size_t position, size_t len = size_t(-1)) const
    {
        const const_iterator first = begin() + position;
        return substr(first, len == size_t(-1) ? end() : first + len);
    }
    String substr(const const_iterator &position) const { return substr(position, end()); }
    String left(size_t position) const { return substr(begin(), begin() + position); }
    String left(const const_iterator &position) const { return substr(begin(), position); }
    String right(size_t position) const { return substr(position); }
    String right(const const_iterator &position) const { return substr(position); }

    // operators
    char operator[](size_t position) const { return data()[position]; }
    String operator+(const String &str) const;
    String operator+(const char *str) const;
    String &operator+=(int c)
    {
        append(c);
        return *this;
    }
    String &operator+=(const String &s)
    {
        append(s);
        return *this;
    }
    String &operator+=(const char *s)
    {
        append(s);
        return *this;
    }
    String &operator<<(char c) { return *this += c; }
    String &operator<<(const String &s) { return *this += s; }
    String &operator<<(const char *s) { return *this += s; }
    // move assign
    void assign(const String &str);
    void assign(const char *str);
    void assign(const const_iterator &begin, const const_iterator &end);
    String &operator=(const String &str)
    {
        assign(str);
        return *this;
    }
    String &operator=(const char *str)
    {
        assign(str);
        return *this;
    }

private:
    void initialization(const char *data, size_t size);
    bool detach();
    bool exchange(size_t size);
    bool expand(size_t incr);

private:
    StringData *_d;
    friend String operator+(const char *, const String &);
};

String operator+(const char *cstr, const String &str);

// BUG: If a null character is stored in the string, the following
// comparison functions may not get the correct result.

// equal to
bool operator==(const String &left, const String &right);

inline bool operator==(const String &left, const char *right)
{
    return !std::strcmp(left.begin(), right);
}

inline bool operator==(const char *left, const String &right)
{
    return right == left;
}

// not equal to
inline bool operator!=(const String &left, const String &right)
{
    return !(left == right);
}

inline bool operator!=(const String &left, const char *right)
{
    return !(left == right);
}

inline bool operator!=(const char *left, const String &right)
{
    return !(left == right);
}

// less than to
inline bool operator<(const String &left, const String &right)
{
    size_t size = pmin(left.size(), right.size()) + 1;
    return std::memcmp(left.data(), right.data(), size) < 0;
}

inline bool operator<(const String &left, const char *right)
{
    return std::strcmp(left.data(), right) < 0;
}

inline bool operator<(const char *left, const String &right)
{
    return std::strcmp(left, right.data()) < 0;
}

// less or equal than to
inline bool operator<=(const String &left, const String &right)
{
    return !(right < left);
}

inline bool operator<=(const String &left, const char *right)
{
    return !(right < left);
}

inline bool operator<=(const char *left, const String &right)
{
    return !(right < left);
}

// greater or equal to
inline bool operator>=(const String &left, const String &right)
{
    return !(left < right);
}

inline bool operator>=(const String &left, const char *right)
{
    return !(left < right);
}

inline bool operator>=(const char *left, const String &right)
{
    return !(left < right);
}

// greater to
inline bool operator>(const String &left, const String &right)
{
    return !(left <= right);
}

inline bool operator>(const String &left, const char *right)
{
    return !(left <= right);
}

inline bool operator>(const char *left, const String &right)
{
    return !(left <= right);
}

PERSIMMON_NAMESPACE_END
