/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020.12.11     wanghaijing  first version
 */

#include "lpm.h"

#define DBG_TAG               "lpm"
#define DBG_LVL               DBG_INFO
#include <rtdbg.h>

extern struct lpm lpm;

#include "x_mem.h"

const rt_uint8_t auchCRCHi[] = { 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

const rt_uint8_t auchCRCLo[] = { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5,
        0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
        0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1,
        0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F,
        0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
        0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2,
        0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9,
        0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
        0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90,
        0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D,
        0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86,
        0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };

/* partition magic word */
#define LPM_MAGIC_WORD 0x45503130
#define LPM_PART_FULLNAME  1
#define LPM_PART_ANONYMITY 0
#define LPM_SUPER_PAGE0    0
#define LPM_SUPER_PAGE1    1

static rt_bool_t  infopart = LPM_SUPER_PAGE0;

struct lpm_part_info
{
    char name[LPM_NAME_MAX];
    uint32_t size;
    uint32_t block_no;
};

struct lpm_super_block
{
    uint32_t magic_word1;
    uint32_t magic_word2;
    char lpm_ver[LPM_NAME_MAX];
    uint32_t part_num;
    uint32_t status;
    uint32_t reserved1;
    uint32_t reserved2;
    struct lpm_part_info part_info[];
};

uint32_t lpm_part_alloc(struct lpm_dev *dev, size_t size, uint8_t flag)
{
    RT_ASSERT(dev);

    return (uint32_t) x_malloc_flag(dev->mem_ptr, size, flag);
}

int lpm_part_free(struct lpm_dev *dev, uint32_t addr)
{
    RT_ASSERT(dev);

    return (uint32_t) x_free(dev->mem_ptr, (void*) addr);
}

uint32_t lpm_free(const char *dev_name)
{
    struct lpm_dev *dev;
    unsigned int total, used;

    dev = lpm_dev_find(dev_name);

    if(dev == RT_NULL)
    {
        LOG_W("dev [%s] not finded!", dev_name);
        return 0;
    }

    x_info(dev->mem_ptr, &total, &used, RT_NULL);

    return total - used;
}

void lpm_part_dump(struct lpm_dev *dev)
{
    RT_ASSERT(dev);

    x_dump(dev->mem_ptr);
}

static uint16_t lpm_create_crc(uint8_t *lpm_sup, uint32_t len,uint16_t crc)
{
    rt_uint32_t Index;
    uint16_t result = 0;

    rt_uint8_t CRCHi =  (crc >> 8) & 0xFF;
    rt_uint8_t CRCLo =  crc & 0xFF;

    while (len--)
    {
        Index = CRCHi ^ *(lpm_sup++);
        CRCHi = CRCLo ^ auchCRCHi[Index];
        CRCLo = auchCRCLo[Index];
    }

    result |= CRCHi << 8;
    result |= CRCLo;

    return result;
}

static rt_err_t lpm_check_crc(struct lpm_dev *dev, uint32_t sup_blk_num)
{
    struct lpm_super_block *lpm_sup;
    uint16_t crc = 0xFFFF;
    uint32_t part_num;
    uint32_t len;

    RT_ASSERT(dev);
    if(!(sup_blk_num != 0  || sup_blk_num != 1))
        return -RT_ERROR;

    lpm_sup = rt_malloc(dev->block_size);
    if (lpm_sup == RT_NULL)
    {
        return -RT_ERROR;
    }

    /* reads the first blk of the superblock */
    dev->ops->read(dev, sup_blk_num * (LPM_SUPER_BLK_NUM / dev->block_size), (uint8_t*)lpm_sup, 1);
    /* Check if the maximum number of records supported by the superblock has been exceeded */
    if(lpm_sup->part_num > LPM_SUPER_BLK_NUM / sizeof(struct lpm_part_info) - 3)
    {
        return -RT_ERROR;
    }
    part_num = lpm_sup->part_num;
    /* The total length of CRC data is not included */
    uint32_t totle_size_no_crc = sizeof(struct lpm_super_block) + sizeof(struct lpm_part_info) * part_num;
    uint32_t blk_num;
    /* Save the blk in the superblock */
    blk_num = totle_size_no_crc / dev->block_size;
    /* Check whether the BLK number exceeds the maximum number */
    if(blk_num > LPM_SUPER_BLK_NUM / dev->block_size)
    {
        return -RT_ERROR;
    }

    for (int i = 0; i < blk_num; i++) {
        rt_memset(lpm_sup, 0, dev->block_size);
        dev->ops->read(dev, (sup_blk_num * (LPM_SUPER_BLK_NUM / dev->block_size)) + i, (uint8_t*)lpm_sup, 1);

        crc = lpm_create_crc((uint8_t*)lpm_sup, dev->block_size,crc);
    }

    len = totle_size_no_crc % dev->block_size;
    rt_memset(lpm_sup, 0, dev->block_size);
    dev->ops->read(dev, (sup_blk_num * (LPM_SUPER_BLK_NUM / dev->block_size)) + blk_num, (uint8_t*)lpm_sup, 1);
    if(len == 0)
    {
        /* only crc, veify crc */
        if(crc == *((uint16_t*)lpm_sup))
        {
            rt_free(lpm_sup);
            return RT_EOK;
        }
        else
        {
            rt_free(lpm_sup);
            return -RT_ERROR;
        }

    }
    else
    {
        crc = lpm_create_crc((uint8_t*)lpm_sup, len, crc);
        /* veify crc */
        if(crc == *((uint16_t*)((uint8_t*)(lpm_sup)+len)))
        {
            rt_free(lpm_sup);
            return RT_EOK;
        }
        else
        {
            rt_free(lpm_sup);
            return -RT_ERROR;
        }
    }
}

static int lpm_superblk_read(struct lpm_dev *dev, uint32_t blk_num,uint8_t *buf)
{
    uint32_t num_cur;

    num_cur = (LPM_SUPER_BLK_NUM / (dev->block_size)) * infopart + blk_num;

    return dev->ops->read(dev, num_cur, buf, 1);
}

/* 从存储设备上加载分区信息 */
int lpm_part_info_load(struct lpm_dev *dev)
{
    struct lpm_super_block *lpm_sup, *lpm_sup0, *lpm_sup1;
    struct lpm_partition *lpm_par;
    struct lpm_part_info *ptr_part;
    uint8_t res = 0;
    uint32_t blk_num = 0;
    uint32_t part_num = 0;

    RT_ASSERT(dev);

    lpm_sup0 = rt_malloc(dev->block_size);
    if (lpm_sup0 == RT_NULL)
    {
        return -RT_ERROR;
    }

    dev->ops->read(dev, 0, (uint8_t*)lpm_sup0, 1);

    if ((lpm_sup0->magic_word1 == LPM_MAGIC_WORD) && (lpm_sup0->magic_word2 == LPM_MAGIC_WORD))
    {
        if(lpm_check_crc(dev, 0) != RT_EOK)
        {
            LOG_W("lpm_sup1 crc check failed!");
            rt_free(lpm_sup0);

            res |= 1 << 0; //bit0 置1 给一个 sup0 错误标志位
        }
    }
    else
    {
        rt_free(lpm_sup0);

        res |= 1 << 0; //bit0 置1 给一个 sup0 错误标志位
    }

    lpm_sup1 = rt_malloc(dev->block_size);
    if (lpm_sup1 == RT_NULL)
    {
        res |= 1 << 1; //bit1 置1 给一个 sup2 错误标志位
    }
    else
    {
        dev->ops->read(dev, LPM_SUPER_BLK_NUM / dev->block_size, (uint8_t*)lpm_sup1, 1);
        if ((lpm_sup1->magic_word1 == LPM_MAGIC_WORD) && (lpm_sup1->magic_word2 == LPM_MAGIC_WORD))
        {
            if(lpm_check_crc(dev, 1) != RT_EOK)
            {
                LOG_W("lpm_sup2 crc check failed!");
                rt_free(lpm_sup1);

                res |= 1 << 1; //bit1 置1 给一个 sup2 错误标志位
            }
        }
        else
        {
            rt_free(lpm_sup1);

            res |= 1 << 1; //bit1 置1 给一个 sup2 错误标志位
        }
    }

    if(res == 3)//bit0:1 都为1 就说明，超级块0和1 都没有记录分区信息
    {
        return -RT_ERROR;
    }
    else if(res == 0x01 ){ // 超级块0 出现错误
        lpm_sup = lpm_sup1;

        infopart = LPM_SUPER_PAGE1; //当前信息储存在 超级块1，下一次就要写在 超级块0
    }
    else if(res == 0x02){ // 超级块2 出现错误
        lpm_sup = lpm_sup0;

        infopart = LPM_SUPER_PAGE0; //当前信息储存在 超级块0，下一次就要写在 超级块1
    }
    else {
       if(lpm_sup1->status >= lpm_sup0->status)  //status 是 uint32，目前每次保存加 1，溢出情况的处理还未实现
       {
           lpm_sup = lpm_sup1;

           rt_free(lpm_sup0);

           infopart = LPM_SUPER_PAGE1; //当前信息储存在 超级块1，下一次就要写在 超级块0
       }
       else {
           lpm_sup = lpm_sup0;

           rt_free(lpm_sup1);

           infopart = LPM_SUPER_PAGE0; //当前信息储存在 超级块0，下一次就要写在 超级块1
       }
    }

    if (lpm_sup->part_num == 0)
    {
        LOG_W("Not found LPM partition, Please create first LPM partition.");
    }
    else
    {
        int pos = (LPM_SUPER_BLK_NUM * 2) / dev->block_size; // pos 从 2 个超级块之后开始计算
        part_num = lpm_sup->part_num;
        ptr_part = lpm_sup->part_info;

        for (int i = 0; i < part_num; i++)
        {
            lpm_par = rt_malloc(sizeof(struct lpm_partition));
            if (lpm_par == RT_NULL)
            {
                rt_free(lpm_sup);

                return -RT_ERROR;
            }

            rt_strncpy(lpm_par->name, ptr_part->name, RT_NAME_MAX);
            lpm_par->offset = ptr_part->block_no;
            lpm_par->size = ptr_part->size;
            lpm_par->dev = dev;

            if(lpm_par->name[0] != 0)
            {
                if(pos != lpm_par->offset) // 如果偏移不是存储设备的地址，则申请空间临时占住这部分空间
                {
                    lpm_part_alloc(dev,lpm_par->offset - pos,LPM_PART_FULLNAME);
                    pos = lpm_par->offset;
                }
                lpm_part_alloc(dev,lpm_par->size,LPM_PART_FULLNAME);
                pos += lpm_par->size;

                rt_slist_append(&(dev->part_list), &(lpm_par->list));// 有名分区插入链表
            }
            else
            {
                if(pos != lpm_par->offset)// 如果偏移不是存储设备的地址，则申请空间临时占住这部分空间
                {
                    lpm_part_alloc(dev,lpm_par->offset - pos, LPM_PART_ANONYMITY);
                    pos = lpm_par->offset;
                }
                lpm_part_alloc(dev,lpm_par->size, LPM_PART_ANONYMITY);
                pos += lpm_par->size;

                rt_free(lpm_par);// 如果是匿名分区，那么就不需要插入到链表这里申请的内存就可以释放了
            }

            if((uint32_t)ptr_part + sizeof(struct lpm_part_info)- (uint32_t)lpm_sup >= dev->block_size)
            {
                blk_num++;
                lpm_superblk_read(dev, blk_num, (uint8_t*)lpm_sup);

                ptr_part = (struct lpm_part_info *)lpm_sup; // 指针移动回 起始地址

                continue;
            }
            ptr_part++; // 每一次成的加载完成数据之后，指针向后移动
        }

        pos = (LPM_SUPER_BLK_NUM * 2) / dev->block_size;

        blk_num = 0;
        lpm_superblk_read(dev, blk_num, (uint8_t*)lpm_sup);
        ptr_part = lpm_sup->part_info;
        for (int i = 0; i < part_num; i++)
        {
            if(pos != ptr_part->block_no)
            {
                lpm_part_free(dev,pos); // 释放临时占住的空间
                pos = ptr_part->block_no;
            }
            pos += ptr_part->size;

            if((uint32_t)ptr_part + sizeof(struct lpm_part_info)- (uint32_t)lpm_sup >= dev->block_size)
            {
                blk_num++;
                lpm_superblk_read(dev, blk_num, (uint8_t*)lpm_sup);

                ptr_part = (struct lpm_part_info *)lpm_sup;

                continue;
            }
            ptr_part++;
        }
    }

    rt_free(lpm_sup);

    return RT_EOK;
}

static int lpm_superblk_write(struct lpm_dev *dev, uint32_t blk_num,uint8_t *buf)
{
    uint32_t num_cur;
    uint8_t info_part = 1;

    if (infopart) {
        info_part = LPM_SUPER_PAGE0;
    }

    num_cur = (LPM_SUPER_BLK_NUM / (dev->block_size)) * info_part + blk_num;

    return dev->ops->write(dev, num_cur, buf, 1);
}


/* 保存分区信息到存储设备上 */
int lpm_part_info_save(struct lpm_dev *dev)
{
    rt_slist_t *node = RT_NULL;
    struct lpm_partition *lpm_par;
    struct lpm_super_block *lpm_sup;
    struct lpm_part_info *ptr_part;
    uint32_t i = 0;
    size_t ptr = 0;
    uint32_t blk_num = 0;
    uint16_t crc = 0xFFFF;
    uint8_t crc_flag = RT_TRUE;

    RT_ASSERT(dev);

    lpm_sup = rt_malloc(dev->block_size);
    if (lpm_sup == RT_NULL)
    {
        return -RT_ERROR;
    }

    /* Count the number of partitions */
    struct heap_mem *mem_obj;
    list_for_each_entry(mem_obj, &((x_mem_t)dev->mem_ptr)->head, list)
    {
        if(i == 0)
        {
            i++;
            ptr += mem_obj->len;
            continue;
        }
         if(mem_obj->used & MEM_USED_FLAG)
         {
             i++;
             ptr += mem_obj->len;
         }
         else {
            continue;
        }
    }

    uint32_t part_num = i - 1;

    if(dev->ops->read(dev, (LPM_SUPER_BLK_NUM / (dev->block_size)) * infopart, (uint8_t*) lpm_sup, 1) < 0)
    {
        return -RT_ERROR;
    }

    /* 更新头部状态  */
    lpm_sup->magic_word1 = LPM_MAGIC_WORD;
    lpm_sup->magic_word2 = LPM_MAGIC_WORD;
    rt_memcpy(lpm_sup->lpm_ver,LPM_VER,LPM_NAME_MAX);
    lpm_sup->part_num = part_num; // 更新超级块的 分区数量
    lpm_sup->status += 1; // 更新 status 状态
    lpm_sup->reserved1 = 0;
    lpm_sup->reserved2 = 0;

    ptr_part = lpm_sup->part_info;
    node = RT_NULL;
    ptr = 0;
    i = 0;

    /* 遍历 x_mem 的空间使用情况，来查找出那些地方存放了分区信息 */
    list_for_each_entry(mem_obj, &((x_mem_t)dev->mem_ptr)->head, list)
    {
        if(i == 0)
        {
            i++;
            ptr += mem_obj->len;
            continue;
        }

        /* 只有被使用的空间里面才保存了分区信息 */
        if(mem_obj->used & MEM_USED_FLAG)
        {
            ptr_part->size = mem_obj->len;
            ptr_part->block_no = ptr;
            ptr_part->name[0] = 0;
            if (mem_obj->reserved == LPM_PART_FULLNAME) // 有名字的分区
            {
                rt_slist_for_each(node, &(dev->part_list))
                {
                    lpm_par = rt_slist_entry(node, struct lpm_partition, list);
                    if (lpm_par != RT_NULL)
                    {
                        if(ptr_part->block_no == lpm_par->offset)
                        {
                            rt_strncpy(ptr_part->name, lpm_par->name, RT_NAME_MAX);
                        }
                    }
                }
            }

            /* 判断一个 blk 是够已经写满了 */
            if((uint32_t)ptr_part + sizeof(struct lpm_part_info)- (uint32_t)lpm_sup >= dev->block_size)
            {
                crc = lpm_create_crc((uint8_t*)lpm_sup, dev->block_size, crc); // 计算 CRC

                lpm_superblk_write(dev, blk_num, (uint8_t *)lpm_sup);

                ptr_part = (struct lpm_part_info *)lpm_sup;

                blk_num ++ ;
                if((LPM_SUPER_BLK_NUM / dev->block_size) ==  blk_num)
                {
                    /* The maximum number of partitions that can be stored was exceeded */
                    return -RT_ERROR;
                }
                rt_memset(lpm_sup, 0, sizeof(dev->block_size));
            }
            ptr_part ++;
        }
        ptr += mem_obj->len;
    }

    /* 如果还有一个 blk 没有被写满 */
    if((uint32_t)ptr_part - (uint32_t)lpm_sup != 0)
    {
        crc = lpm_create_crc((uint8_t*)lpm_sup, (uint32_t)ptr_part - (uint32_t)lpm_sup, crc);// 计算 CRC

        *((uint16_t*)ptr_part) = crc; // ptr_part 的地址就是已经偏移的地址，直接写入 CRC 的值
        lpm_superblk_write(dev, blk_num, (uint8_t *)lpm_sup);

        crc_flag = RT_FALSE;// 设置一个标志位，这个标志位用来表示 CRC 已经写入了，后面就不需要在写
    }

    /* 写 crc 到新的blk */
    if (crc_flag) { // 如果执行到这里还未写入 CRC ，则需要在下一个块来写入 CRC
        rt_memset(lpm_sup, 0, sizeof(dev->block_size));
        *((uint16_t*) lpm_sup) = crc;

        lpm_superblk_write(dev, blk_num, (uint8_t *)lpm_sup);
    }

    rt_free(lpm_sup);

    // 切换块
    if (infopart) {
        infopart = LPM_SUPER_PAGE0;
    }else {
        infopart = LPM_SUPER_PAGE1;
    }

    return RT_EOK;
}

struct lpm_partition *lpm_partition_create(const char *dev_name, const char *name, uint32_t size)
{
    struct lpm_dev *lpm_p;
    struct lpm_partition *lpm_par;
    struct lpm_partition *lpm_par_temp;
    uint32_t offset;

    if(name[0] != 0 && lpm_partition_find(dev_name, name) != RT_NULL)
    {
        LOG_W("The partition cannot be created,because the %s already exists.",name);
        return RT_NULL;
    }

    /*find lpm device*/
    lpm_p = lpm_dev_find(dev_name);
    if (lpm_p == RT_NULL)
    {
        LOG_W("not find dev_name!", dev_name);
        return RT_NULL;
    }

    lpm_par = rt_malloc(sizeof(struct lpm_partition));
    if (lpm_par == RT_NULL)
    {
        return RT_NULL;
    }

    rt_strncpy(lpm_par->name, name, RT_NAME_MAX);

    offset = lpm_part_alloc(lpm_p, size, LPM_PART_FULLNAME);

    if( offset == RT_NULL)
    {
        rt_free(lpm_par);
        return RT_NULL;
    }

    lpm_par->offset = offset;
    lpm_par->size = size;
    lpm_par->dev = lpm_p;

    rt_slist_init(&(lpm_par->list));

    rt_slist_t *dev_node = RT_NULL;
    rt_slist_t *last_node = &(lpm_par->list);

    if(rt_slist_isempty(&(lpm_p->part_list)))
    {
        rt_slist_insert(&(lpm_p->part_list), &(lpm_par->list));
    }
    else
    {
        rt_slist_for_each(dev_node, &(lpm_p->part_list))
        {
            lpm_par_temp = rt_slist_entry(dev_node, struct lpm_partition, list);

            if(lpm_par->offset > lpm_par_temp->offset)
            {
                last_node = dev_node;
                if(last_node->next == RT_NULL)
                {
                    rt_slist_insert(last_node, &(lpm_par->list));
                    break;
                }
                continue;
            }
            else
            {
                rt_slist_insert(last_node, &(lpm_par->list));
                break;
            }
        }
    }

    lpm_part_info_save(lpm_p);

    return lpm_par;
}

int lpm_anonymity_create(const char *dev_name, uint32_t size, struct lpm_partition *lpm_par)
{
    struct lpm_dev *lpm_p;
    uint32_t offset;

    /*find lpm device*/
    lpm_p = lpm_dev_find(dev_name);
    if (lpm_p == RT_NULL)
    {
        LOG_W("not find dev_name!", dev_name);
        return -RT_ERROR;
    }

    offset = lpm_part_alloc(lpm_p, size, LPM_PART_ANONYMITY);
    if(offset == RT_NULL){
        LOG_W("alloc size %d space failed!", size);
        return -RT_ERROR;
    }

    LOG_D("offset size 0x %x, %d",offset, size);

    lpm_par->name[0] = 0;
    lpm_par->dev = lpm_p;
    lpm_par->offset = offset;
    lpm_par->size = size;

    lpm_part_info_save(lpm_p);

    return RT_EOK;
}

int lpm_anonymity_delete(struct lpm_partition *lpm_par)
{
    struct lpm_dev *lpm_p = lpm_par->dev;

    if (lpm_p == RT_NULL)
    {
        LOG_W("not find dev_name!");
        return -RT_ERROR;
    }

    lpm_part_free(lpm_p, lpm_par->offset);

    lpm_part_info_save(lpm_p);

    return RT_EOK;
}

int lpm_partition_delete(const char *dev_name, const char *name)
{
    struct lpm_dev *lpm_p;
    struct lpm_partition *lpm_par;
    rt_slist_t *par_node = RT_NULL;

    /*find lpm device*/
    lpm_p = lpm_dev_find(dev_name);
    if (lpm_p == RT_NULL)
    {
        LOG_W("not find dev_name!", dev_name);
        return -RT_ERROR;
    }

    rt_slist_for_each(par_node, &(lpm_p->part_list))
    {
        lpm_par = rt_slist_entry(par_node, struct lpm_partition, list);

        if (lpm_par != RT_NULL)
        {
            if (rt_strncmp(lpm_par->name, name, LPM_NAME_MAX) == 0)
            {
                LOG_D("delete lpm_dev name is %s  partition name is %s.", lpm_p->name, lpm_par->name);
                lpm_part_free(lpm_p, lpm_par->offset);
                rt_slist_remove(&(lpm_p->part_list), &(lpm_par->list));

                lpm_part_info_save(lpm_p);
                rt_free(lpm_par);

                return RT_EOK;
            }
        }
        else
        {
            return -RT_ERROR;
        }

    }

    return RT_EOK;
}

int lpm_partition_delete_self(const struct lpm_partition *part)
{
    struct lpm_dev *lpm_p = part->dev;
    struct lpm_partition *lpm_par;
    rt_slist_t *par_node = RT_NULL;

    rt_slist_for_each(par_node, &(lpm_p->part_list))
    {
        lpm_par = rt_slist_entry(par_node, struct lpm_partition, list);

        if (lpm_par != RT_NULL)
        {
            if (lpm_par->offset == part->offset)
            {
                LOG_D("delete lpm_dev name is %s  partition offset is %d.", lpm_p->name, lpm_par->offset);
                lpm_part_free(lpm_p, lpm_par->offset);
                rt_slist_remove(&(lpm_p->part_list), &(lpm_par->list));

                lpm_part_info_save(lpm_p);
                rt_free(lpm_par);

                return RT_EOK;
            }
        }
        else
        {
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

int lpm_partition_delete_all(const char *dev_name)
{
    struct lpm_dev *lpm_p;
    struct lpm_partition *lpm_par;

    rt_slist_t *dev_node = RT_NULL;
    rt_slist_t *par_node = RT_NULL;

    /*find lpm device*/
    lpm_p = lpm_dev_find(dev_name);
    if (lpm_p == RT_NULL)
    {
        LOG_W("not find dev_name!", dev_name);
        return -RT_ERROR;
    }

    x_destroy((x_mem_t)(lpm_p->mem_ptr));

    x_mem_t lpm_mem;

    lpm_mem = x_create((void *) 0, (void *) lpm_p->sector_count, 1); //todo 这里是因为 block 的大小与 sector 一致
    if (lpm_mem == RT_NULL)
    {
        return -RT_ERROR;
    }

    lpm_p->mem_ptr = lpm_mem;

    lpm_part_alloc(lpm_p, 2*(LPM_SUPER_BLK_NUM) / (lpm_p->block_size), LPM_PART_ANONYMITY);

    rt_slist_for_each(dev_node, &(lpm.device_list))
    {
        lpm_p = rt_slist_entry(dev_node, struct lpm_dev, dev_list);

        if (lpm_p != RT_NULL)
        {
            if (rt_strncmp(lpm_p->name, dev_name, LPM_NAME_MAX) == 0)
            {
                rt_slist_for_each(par_node, &(lpm_p->part_list))
                {
                    lpm_par = rt_slist_entry(par_node, struct lpm_partition, list);

                    if (lpm_par != RT_NULL)
                    {
                        LOG_D("delete lpm_dev name is %s  partition name is %s.", lpm_p->name, lpm_par->name);

                        rt_slist_remove(&(lpm_p->part_list), &(lpm_par->list));

                        rt_free(lpm_par);
                    }
                    else
                    {
                        return -RT_ERROR;
                    }
                }
            }
        }
        else
        {
            return -RT_ERROR;
        }
        lpm_part_info_save(lpm_p);
    }

    return RT_EOK;
}

/* >4G 寻址空间，blk 的概念更清晰 */
struct lpm_partition *lpm_partition_find(const char *dev_name, const char *name)
{
    struct lpm_dev *lpm_p;
    struct lpm_partition *lpm_par;
    rt_slist_t *node = RT_NULL;

    /*find lpm device*/
    lpm_p = lpm_dev_find(dev_name);
    if (lpm_p == RT_NULL)
    {
        LOG_W("not find dev_name %s!", dev_name);
        return RT_NULL;
    }

    rt_slist_for_each(node, &(lpm_p->part_list))
    {
        lpm_par = rt_slist_entry(node, struct lpm_partition, list);
        if (lpm_par != RT_NULL)
        {
            if (rt_strncmp(lpm_par->name, name, LPM_NAME_MAX) == 0)
            {
                return lpm_par;
            }
        }
    }

    return RT_NULL;
}

int lpm_partition_read(const struct lpm_partition *part, uint32_t sector_no, uint8_t *buf, size_t size)
{
    //用户传递进来的是 sector_num ，但是 part->offset 的偏移量是 block,需要计算  block 与 sector 的比例关系
    uint32_t cur_offset;
    uint32_t i;

    RT_ASSERT(part);
    RT_ASSERT(part->dev);
    RT_ASSERT(part->dev->ops);
    RT_ASSERT(part->dev->ops->read);

    //计算当前 block 设备 block 与 sector 的关系
    i = ((part->dev->block_size) / (part->dev->bytes_per_sector));

    cur_offset = (part->offset) * i + sector_no;

    return part->dev->ops->read(part->dev, cur_offset, buf, size); //传递进去单位也是 sector
}

int lpm_partition_write(const struct lpm_partition *part, uint32_t sector_no, const uint8_t *buf, size_t size)
{
    uint32_t cur_offset;
    uint32_t i;

    RT_ASSERT(part);
    RT_ASSERT(part->dev);
    RT_ASSERT(part->dev->ops);
    RT_ASSERT(part->dev->ops->write);

    i = (part->dev->block_size) / (part->dev->bytes_per_sector);

    cur_offset = (part->offset) * i + sector_no;

    return part->dev->ops->write(part->dev, cur_offset, buf, size);
}

int lpm_partition_erase(const struct lpm_partition *part, uint32_t block_no, size_t num)
{
    uint32_t cur_offset;
    uint32_t i;

    RT_ASSERT(part);
    RT_ASSERT(part->dev);
    RT_ASSERT(part->dev->ops);
    RT_ASSERT(part->dev->ops->erase);

    i = (part->dev->block_size) / (part->dev->bytes_per_sector);

    cur_offset = (part->offset + block_no) * i;

    return part->dev->ops->erase(part->dev, cur_offset, num * i);
}

int lpm_partition_erase_all(const struct lpm_partition *part)
{
    uint32_t cur_offset;
    uint32_t i;

    i = (part->dev->block_size) / (part->dev->bytes_per_sector);

    cur_offset = (part->offset) * i;

    return part->dev->ops->erase(part->dev, cur_offset, (part->size) * i);
}

int lpm_dump_partition(void)
{
    struct lpm_dev *lpm_p;
    struct lpm_partition *lpm_par;
    rt_slist_t *dev_node = RT_NULL;
    rt_slist_t *par_node = RT_NULL;

    rt_kprintf("==================== LPM partition table ============================\r\n");
    rt_kprintf("|  dev_name  |  par_name  |  block_size  |   offset   |   length   |\r\n");
    rt_kprintf("---------------------------------------------------------------------\r\n");

    rt_slist_for_each(dev_node, &(lpm.device_list))
    {
        lpm_p = rt_slist_entry(dev_node, struct lpm_dev, dev_list);

        if (lpm_p != RT_NULL)
        {
            rt_slist_for_each(par_node, &(lpm_p->part_list))
            {
                lpm_par = rt_slist_entry(par_node, struct lpm_partition, list);
                if (lpm_par != RT_NULL)
                {
                    rt_kprintf("|  %-*.*s  |  %-*.*s  |  0x%08lx  | 0x%08lx | 0x%08lx |\r\n", RT_NAME_MAX, RT_NAME_MAX, lpm_p->name, \
                            LPM_NAME_MAX, LPM_NAME_MAX, lpm_par->name, lpm_p->block_size, lpm_par->offset, lpm_par->size);
                }
                else
                {
                    return -RT_ERROR;
                }
            }
        }
        else
        {
            return -RT_ERROR;
        }
    }
    rt_kprintf("=====================================================================\r\n");
    return RT_EOK;
}

char* lpm_get_version(struct lpm_dev *dev)
{
    struct lpm_super_block *lpm_sup, *lpm_sup1, *lpm_sup2;
    static char version[LPM_NAME_MAX];
    uint8_t res = 0;

    RT_ASSERT(dev);

    lpm_sup1 = rt_malloc(dev->block_size);
    if (lpm_sup1 == RT_NULL)
    {
        return RT_NULL;
    }

    dev->ops->read(dev, 0, (uint8_t*)lpm_sup1, 1);

    if ((lpm_sup1->magic_word1 == LPM_MAGIC_WORD) && (lpm_sup1->magic_word2 == LPM_MAGIC_WORD))
    {
        if(lpm_check_crc(dev, 0) != RT_EOK)
        {
            rt_kprintf("lpm_sup1 crc check failed! \r\n");
            rt_free(lpm_sup1);

            res |= 1 << 0;
        }
    }
    else
    {
        rt_free(lpm_sup1);

        res |= 1 << 0;
    }

    lpm_sup2 = rt_malloc(dev->block_size);
    if (lpm_sup2 == RT_NULL)
    {
        res |= 1 << 1;
    }
    else
    {
        dev->ops->read(dev, 1, (uint8_t*)lpm_sup2, 1);

        if ((lpm_sup2->magic_word1 == LPM_MAGIC_WORD) && (lpm_sup2->magic_word2 == LPM_MAGIC_WORD))
        {
            if(lpm_check_crc(dev, 1) != RT_EOK)
            {
                rt_kprintf("lpm_sup2 crc check failed! \r\n");
                rt_free(lpm_sup2);

                res |= 1 << 1;
            }
        }
        else
        {
            rt_free(lpm_sup2);

            res |= 1 << 1;
        }
    }

    if(res == 3)
    {
        return RT_NULL;
    }
    else if(res == 0x01 ){
        lpm_sup = lpm_sup2;
    }
    else if(res == 0x02){
        lpm_sup = lpm_sup1;
    }
    else {
       if(lpm_sup2->status >= lpm_sup1->status)
       {
           lpm_sup = lpm_sup2;

           rt_free(lpm_sup1);
       }
       else {
           lpm_sup = lpm_sup1;

           rt_free(lpm_sup2);
       }
    }

    rt_memcpy(version, lpm_sup->lpm_ver, LPM_NAME_MAX);

    rt_free(lpm_sup);

    return version;
}

uint32_t lpm_partition_get_addr(const struct lpm_partition *part)
{
    uint32_t offset, addr;

    RT_ASSERT(part);

    offset = part->offset;

    addr = part->dev->bytes_per_sector;

    return offset * addr;
}
