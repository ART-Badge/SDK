#ifndef __PMEM_H__
#define __PMEM_H__

#include <rtthread.h>

struct pmem_tab
{
    rt_uint32_t blk_size;
    rt_uint32_t blk_cnt;
    rt_uint32_t min_mem;
};
// typedef struct pmem_tab * pmem_tab_t;

struct pmem_pool
{
    rt_uint32_t min_mem;    /* Less than this value will 
                               not be allocated from this memory pool */
    struct rt_mempool mp;
};

struct pmem
{
    struct rt_memheap heap;
    void *heap_start;
    void *pool_start;
    rt_size_t mp_cnt;
    struct pmem_pool pmp[];
};
typedef struct pmem *pmem_t;

pmem_t pmem_init(const char          *name,
               void                  *start,
               rt_size_t             size,
               const struct pmem_tab *tab,
               rt_size_t             tab_num);
void *pmem_alloc(pmem_t pmem, rt_size_t size);
void *pmem_realloc(pmem_t pmem, void *ptr, rt_size_t rsize);
void pmem_free(pmem_t pmem, void *ptr);
rt_size_t pmem_size(pmem_t pmem, void *ptr);
void pmem_deinit(pmem_t pmem);
#endif
