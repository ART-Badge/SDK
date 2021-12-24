/*
 * File      : x_mem.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Electronic Technology Co.,Ltd.
 * All Rights Reserved.
 *
 * This file is part of the RT-Thread MoM Electric Meter system.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-12      xxx          the first version
 */

#ifndef __X_MEM_H__
#define __X_MEM_H__

#include <stdint.h>

#define ALIGN_SIZE      4
#define X_MEM_BOUNDARY  0xbd

#define MEM_USED_FLAG   (0x1 << 0)
#define OBJ_USED_FLAG   (0x1 << 4)

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define list_entry(node, type, member) \
    container_of(node, type, member)

#define list_for_each_entry(pos, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member), \
         n = list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))

struct list_node
{
    struct list_node *next;                          /**< point to next node. */
    struct list_node *prev;                          /**< point to prev node. */
};
typedef struct list_node list_t;                     /**< Type for lists. */

struct x_mem
{
    uint8_t *heap_ptr;
    size_t align_size;
    size_t mem_size_aligned;
    size_t used_mem, max_mem;
    struct list_node tab_head;
    list_t head;
};
typedef struct x_mem * x_mem_t;
struct heap_mem
{
    list_t list;
    size_t len;
    uint16_t used;
    uint16_t reserved;
};

struct x_mem_tab
{
    struct list_node node;
    int32_t used;
    struct heap_mem tab[];
};
x_mem_t x_create(void *begin_addr, void *end_addr, size_t align_size);
void x_destroy(x_mem_t x_mem);
void *x_malloc(x_mem_t x_mem, unsigned int size);
void *x_calloc(x_mem_t x_mem, unsigned int count, unsigned int size);
void *x_realloc(x_mem_t x_mem, void *rmem, unsigned int newsize);
void *x_free(x_mem_t x_mem, void *mem);
void x_info(x_mem_t x_mem, unsigned int *total, unsigned int *used, unsigned int *max_used);
void x_dump(x_mem_t x_mem);
uint32_t x_size(x_mem_t x_mem, void *mem);
void *x_malloc_flag(x_mem_t x_mem, unsigned int size, uint8_t flag);
#endif
