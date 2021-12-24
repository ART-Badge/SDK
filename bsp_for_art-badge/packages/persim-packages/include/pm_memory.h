#pragma once

#include "pm_internal.h"
#include <stddef.h>

PERSIMMON_NAMESPACE_BEGIN

class RawAllocator
{
public:
    virtual void *malloc(size_t size) const             = 0;
    virtual void *realloc(void *ptr, size_t size) const = 0;
    virtual void free(void *ptr) const                  = 0;
};

class BlockAllocator
{
public:
    enum Type
    {
        Chunk = 0,
        Local,
        Unknow
    };

    template<Type t, class T> class type_allocator
    {
    public:
        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        template<class U> struct rebind
        {
            typedef type_allocator<t, U> other;
        };

        type_allocator() {}
        type_allocator(const type_allocator &) {}
        template<class U> type_allocator(const type_allocator<t, U> &) {}

        pointer allocate(size_type n, const void *hint = 0) { return malloc(n * sizeof(T)); }
        void deallocate(pointer p, size_type) { free(p); }
        void construct(pointer p, const T &value) { ::new (p) T(value); }
        void destroy(pointer p) { p->~T(); }
        pointer address(reference x) { return (pointer)&x; }
        const_pointer address(const_reference x) { return (const_pointer)&x; }
        size_type max_size() const { return size_type(-1) / sizeof(T); }

        template<class U> bool operator==(const type_allocator<t, U> &) { return true; }
        template<class U> bool operator!=(const type_allocator<t, U> &) { return false; }

        static pointer malloc(size_t s) { return pointer(alloc(t)->malloc(s)); }
        static pointer realloc(pointer p, size_t s) { return pointer(alloc(t)->realloc(p, s)); }
        static void free(pointer p) { alloc(t)->free(p); }
    };

    template<Type t> class type_allocator<t, void>
    {
    public:
        typedef void value_type;
        typedef void *pointer;
        typedef const void *const_pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        template<class U> struct rebind
        {
            typedef type_allocator<t, U> other;
        };

        struct object
        {
            void *operator new(size_t size) { return malloc(size); }
            void *operator new[](size_t size) { return malloc(size); }
            void operator delete(void *ptr) { free((void *)ptr); }
            void operator delete[](void *ptr) { free((void *)ptr); }
        };

        template<class U> bool operator==(const type_allocator<t, U> &) { return true; }
        template<class U> bool operator!=(const type_allocator<t, U> &) { return false; }

        static void *malloc(size_t s) { return alloc(t)->malloc(s); }
        static void *realloc(void *p, size_t s) { return alloc(t)->realloc(p, s); }
        static void free(void *p) { alloc(t)->free(p); }
    };

    static void setAllocator(Type type, const RawAllocator *allocator);

private:
    static const RawAllocator *alloc(Type type) { return _allocator[type]; }

private:
    static const RawAllocator *_allocator[Unknow];
};

template<class T = void>
class chunk_allocator : public BlockAllocator::type_allocator<BlockAllocator::Chunk, T>
{
};

template<class T = void>
class local_allocator : public BlockAllocator::type_allocator<BlockAllocator::Local, T>
{
};

inline void *malloc(size_t size)
{
    return chunk_allocator<>::malloc(size);
}

inline void *realloc(void *ptr, size_t size)
{
    return chunk_allocator<>::realloc(ptr, size);
}

inline void free(void *ptr)
{
    return chunk_allocator<>::free(ptr);
}

PERSIMMON_NAMESPACE_END
