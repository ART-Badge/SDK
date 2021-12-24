#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

class MemoryPoolBase
{
public:
    MemoryPoolBase(size_t elmSize, size_t blockSize = 256);
    ~MemoryPoolBase();
    void *alloc();
    void free(void *ptr);

private:
    struct Block
    {
        struct Block *prev;
        uintptr_t *free;
    };
    void newBlock();
    void *blockAlloc(Block *b);

private:
    Block *blocks;
    const size_t esize, bsize;
};

template<typename T> class MemoryPool : public MemoryPoolBase
{
public:
    MemoryPool(size_t blocks = 256) : MemoryPoolBase(sizeof(T), blocks) {}
    T *alloc() { return reinterpret_cast<T *>(MemoryPoolBase::alloc()); }
    void free(T *ptr) { MemoryPoolBase::free(ptr); }
};

class MemoryAllocPoolBase
{
public:
    MemoryAllocPoolBase(size_t elmSize, size_t blockSize = 256);
    ~MemoryAllocPoolBase();
    void *alloc();
    void clear();

private:
    void newBlock();

private:
    struct Block
    {
        struct Block *next;
    };
    Block *current, *head;
    const size_t esize, bsize;
    size_t used;
};

template<typename T> class MemoryAllocPool : public MemoryAllocPoolBase
{
public:
    MemoryAllocPool(size_t blocks = 256) : MemoryAllocPoolBase(sizeof(T), blocks) {}
    T *alloc() { return reinterpret_cast<T *>(MemoryAllocPoolBase::alloc()); }
};

PERSIMMON_NAMESPACE_END
