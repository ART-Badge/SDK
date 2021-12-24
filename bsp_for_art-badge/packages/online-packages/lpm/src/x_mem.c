/*
 * File      : x_mem.c
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Electronic Technology Co.,Ltd.
 * All Rights Reserved.
 *
 * This file is part of the RT-Thread MoM Electric Meter system.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-12      xxx          the first version
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <rtthread.h>

#include "x_mem.h"


#define X_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define X_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define HEAP_MAGIC 0x1ea0
#define ASSERT(EX)

#define X_MEM_TAB_CNT   (8)
#define X_MEM_TAB_MAX   (8)

static void list_init(list_t *l)
{
    l->next = l->prev = l;
}

static void list_insert_before(list_t *l, list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

static void list_insert_after(list_t *l, list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

static void list_remove(list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

static struct x_mem_tab *get_mem_tab(x_mem_t x_mem, struct heap_mem *mem)
{
    struct x_mem_tab *tab;
    uint32_t begin, end;

    list_for_each_entry(tab, &x_mem->tab_head, node)
    {
        begin = (uint32_t)tab + sizeof(struct x_mem_tab);
        end = begin + sizeof(struct heap_mem) * X_MEM_TAB_CNT;
        if ((uint32_t)mem >= begin &&
            (uint32_t)mem <= end &&
            ((uint32_t)mem - begin) % sizeof(struct heap_mem) == 0)
        {
            return tab;
        }
    }
    return NULL;
}

static struct heap_mem *get_mem_obj(x_mem_t x_mem)
{
    int i;
    struct x_mem_tab *tab, *new_tab;

    list_for_each_entry(tab, &x_mem->tab_head, node)
    {
        if (X_MEM_TAB_CNT - tab->used > 0)
        {
            for (i = 0; i < X_MEM_TAB_CNT; i++)
            {
                if (!(tab->tab[i].used & OBJ_USED_FLAG))
                {
                    tab->tab[i].used = OBJ_USED_FLAG;
                    list_init(&tab->tab[i].list);
                    tab->tab[i].len = 0;
                    tab->used ++;
                    return &tab->tab[i];
                }
            }
        }
    }
    new_tab = calloc(1, sizeof(struct x_mem_tab) + sizeof(struct heap_mem) * X_MEM_TAB_CNT);
    if (new_tab == NULL)
    {
        return NULL;
    }
    new_tab->used = 0;
    list_init(&new_tab->node);
    list_insert_after(&x_mem->tab_head, &new_tab->node);
    return get_mem_obj(x_mem);
}

static void put_mem_obj(x_mem_t x_mem, struct heap_mem *mem)
{
    struct x_mem_tab *tab, *tmp;
    int f_cnt;

    tab = get_mem_tab(x_mem, mem);
    if (tab == NULL)
    {
        return;
    }
    memset(mem, 0, sizeof(struct heap_mem));
    tab->used --;

    f_cnt = 0;
    list_for_each_entry_safe(tab, tmp, &x_mem->tab_head, node)
    {
        if (f_cnt >= X_MEM_TAB_MAX && tab->used == 0)
        {
            list_remove(&tab->node);
            free(tab);
        }
        else
        {
            f_cnt += X_MEM_TAB_CNT - tab->used;
        }
    }
}

x_mem_t x_create(void *begin_addr, void *end_addr, size_t align_size)
{
    uint32_t begin_align = X_ALIGN((uint32_t)begin_addr, align_size);
    uint32_t end_align   = X_ALIGN_DOWN((uint32_t)end_addr, align_size);
    x_mem_t x_mem;
    struct heap_mem *mem;

    x_mem = calloc(1, sizeof(struct x_mem));
    if (x_mem == NULL)
    {
        return NULL;
    }

    /* calculate the aligned memory size */
    x_mem->align_size = align_size;
    x_mem->mem_size_aligned = end_align - begin_align;
    x_mem->heap_ptr = (uint8_t *)begin_align;
    list_init(&x_mem->head);
    list_init(&x_mem->tab_head);

    mem = get_mem_obj(x_mem);
    if (mem == NULL)
    {
        free(x_mem);
        return NULL;
    }

    mem->len = x_mem->mem_size_aligned;
    list_init(&mem->list);
    list_insert_after(&x_mem->head, &mem->list);

    return x_mem;
}

void x_destroy(x_mem_t x_mem)
{
    struct x_mem_tab *mem_tab, *tmp;

    if (x_mem ==  NULL)
    {
        return;
    }

    list_for_each_entry_safe(mem_tab, tmp, &x_mem->tab_head, node)
    {
        list_remove(&mem_tab->node);
        free(mem_tab);
    }

    free(x_mem);
}

void *_x_malloc(x_mem_t x_mem, unsigned int size,uint8_t flag)
{
    struct heap_mem *mem_obj, *mem_new;
    size_t ptr = 0;

    if (size == 0)
    {
        size = x_mem->align_size;
    }

    /* to determine whether the written data is out of bounds */
    /* alignment size */
    size = X_ALIGN(size, x_mem->align_size);

    if (size > x_mem->mem_size_aligned)
    {
        return NULL;
    }

    list_for_each_entry(mem_obj, &x_mem->head, list)
    {
        if (!(mem_obj->used & MEM_USED_FLAG))
        {
            if (mem_obj->len > size)
            {
                mem_new = get_mem_obj(x_mem);
                if (mem_new == NULL)
                {
                    return NULL;
                }
                mem_new->len = size;
                mem_new->used |= MEM_USED_FLAG;
                mem_new->reserved = flag;
                list_init(&mem_new->list);

                mem_obj->len = mem_obj->len - size;
                list_insert_before(&mem_obj->list, &mem_new->list);

                x_mem->used_mem += size;
                if (x_mem->used_mem > x_mem->max_mem)
                {
                    x_mem->max_mem = x_mem->used_mem;
                }

                return (void *)(ptr + x_mem->heap_ptr);
            }
            else if (mem_obj->len == size)
            {
                mem_obj->used |= MEM_USED_FLAG;

                x_mem->used_mem += size;
                if (x_mem->used_mem > x_mem->max_mem)
                {
                    x_mem->max_mem = x_mem->used_mem;
                }

                return (void *)(ptr + x_mem->heap_ptr);
            }
        }
        ptr += mem_obj->len;
    }

    return NULL;
}

void *x_malloc_flag(x_mem_t x_mem, unsigned int size, uint8_t flag)
{
    return _x_malloc(x_mem,size,flag);
}

void *x_malloc(x_mem_t x_mem, unsigned int size)
{
    return  _x_malloc(x_mem, size, 0);
}

void *x_realloc(x_mem_t x_mem, void *mem, unsigned int newsize)
{
    size_t len, ptr;
    struct heap_mem *mem_obj, *tmp;
    void *nmem;

    ptr = (uint8_t *)mem - x_mem->heap_ptr;
    if (ptr > x_mem->mem_size_aligned)
    {
        return NULL;
    }

    /* alignment size */
    newsize = X_ALIGN(newsize, x_mem->align_size);
    if (newsize > x_mem->mem_size_aligned)
    {
        return NULL;
    }
    else if (newsize == 0)
    {
        x_free(x_mem, mem);
        return NULL;
    }

    /* allocate a new memory block */
    if (mem == NULL)
        return x_malloc(x_mem, newsize);

    len = 0;
    list_for_each_entry_safe(mem_obj, tmp, &x_mem->head, list)
    {
        if (ptr == len)
        {
            if (mem_obj->used & MEM_USED_FLAG)
            {
                if (&tmp->list != &x_mem->head && (!(tmp->used & MEM_USED_FLAG)))
                {
                    if (tmp->len + mem_obj->len > newsize)
                    {
                        tmp->len = newsize - mem_obj->len;
                        mem_obj->len = newsize;
                        return mem;
                    }
                    else if (tmp->len + mem_obj->len == newsize)
                    {
                        mem_obj->len = newsize;
                        list_remove(&tmp->list);
                        put_mem_obj(x_mem, tmp);
                        return mem;
                    }
                    else
                    {
                        /* expand memory */
                        nmem = x_malloc(x_mem, newsize);
                        if (nmem != NULL) /* check memory */
                        {
                            memcpy(nmem, mem, mem_obj->len);
                            x_free(x_mem, mem);
                        }
                        return mem;
                    }
                }
            }
            else
            {
                return NULL;
            }
        }
        else if (ptr < len)
        {
            return NULL;
        }
        len += mem_obj->len;
    }
    return NULL;
}

void *x_calloc(x_mem_t x_mem, unsigned int count, unsigned int size)
{
    void *p;

    /* allocate 'count' objects of size 'size' */
    p = x_malloc(x_mem, count * size);

    /* zero the memory */
    if (p)
        memset(p, 0, count * size);

    return p;
}

/**
 * This function will release the previously allocated memory block by
 * x_malloc. The released memory block is taken back to system heap.
 *
 * @param rmem the address of memory which will be released
 */
void *x_free(x_mem_t x_mem, void *mem)
{
    struct heap_mem *mem_obj, *mem_last, *tmp;
    size_t ptr, len;

    if (mem == NULL)
        return NULL;

    ptr = (uint8_t *)mem - x_mem->heap_ptr;
    if (ptr > x_mem->mem_size_aligned)
    {
        //failed. return mem
        return mem;
    }

    len = 0;
    mem_last = NULL;
    list_for_each_entry_safe(mem_obj, tmp, &x_mem->head, list)
    {
        if (ptr == len)
        {
            if (mem_obj->used & MEM_USED_FLAG)
            {
                
                mem_obj->used &= ~MEM_USED_FLAG;
                x_mem->used_mem -= mem_obj->len;

                if (mem_last && (!(mem_last->used & MEM_USED_FLAG)))
                {
                    mem_last->len += mem_obj->len;
                    list_remove(&mem_obj->list);
                    put_mem_obj(x_mem, mem_obj);
                    mem_obj = mem_last;
                }

                if (&tmp->list != &x_mem->head && (!(tmp->used & MEM_USED_FLAG)))
                {
                    mem_obj->len += tmp->len;
                    list_remove(&tmp->list);
                    put_mem_obj(x_mem, tmp);
                }
                return NULL;
            }
            else
            {
                //failed. return mem
                return mem;
            }
        }
        else if (ptr < len)
        {
            //failed. return mem
            return mem;
        }
        len += mem_obj->len;
        mem_last = mem_obj;
    }
    //failed. return mem
    return mem;
}

uint32_t x_size(x_mem_t x_mem, void *mem)
{
    struct heap_mem *mem_obj;
    size_t ptr, len;

    if (mem == NULL)
        return 0;

    ptr = (uint8_t *)mem - x_mem->heap_ptr;
    if (ptr > x_mem->mem_size_aligned)
    {
        //failed. return null
        return 0;
    }

    len = 0;

    list_for_each_entry(mem_obj, &x_mem->head, list)
    {
        if(ptr == len)
        {
            return mem_obj->len;
        }
        len += mem_obj->len;
    }
    return 0;
}

void x_info(x_mem_t x_mem, unsigned int *total, unsigned int *used, unsigned int *max_used)
{
    if (total != NULL)
        *total = x_mem->mem_size_aligned;
    if (used  != NULL)
        *used = x_mem->used_mem;
    if (max_used != NULL)
        *max_used = x_mem->max_mem;
}

void x_dump(x_mem_t x_mem)
{
    struct heap_mem *mem_obj;
    uint8_t *p = x_mem->heap_ptr;
    struct x_mem_tab *tab;

    printf("\ntotal memory: %d\n", x_mem->mem_size_aligned);
    printf("used memory : %d\n", x_mem->used_mem);
    printf("maximum allocated memory: %d\n", x_mem->max_mem);

    printf("\n--memory item information --\n");
    list_for_each_entry(mem_obj, &x_mem->head, list)
    {
        printf("[%04x - ", (uint32_t)p);
        if (mem_obj->len < 1024)
            printf("%05d", mem_obj->len);
        else if (mem_obj->len < 1024 * 1024)
            printf("%04dK", mem_obj->len / 1024);
        else
            printf("%04dM", mem_obj->len / (1024 * 1024));

        if (mem_obj->used & MEM_USED_FLAG)
        {
            printf("] X\n");
        }
        else
        {
            printf("]\n");
        }
        p += mem_obj->len;
    }

    list_for_each_entry(tab, &x_mem->tab_head, node)
    {
        printf("tab[%p] used:%d\n", tab, tab->used);
    }
}
