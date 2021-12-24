/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020.12.11     wanghaijing  first version
 */

#include <rtthread.h>

#define LPM_NAME_MAX        8
#define LPM_SUPER_BLK_NUM   (1024 * 8) // 8K
#define LPM_VER             "V0.1.2"

struct lpm
{
    rt_slist_t device_list; /* 物理设备链表 */
};

/* 分区数据结构 */
struct lpm_partition /* 分区结构 */
{
    char name[LPM_NAME_MAX];
    uint32_t size;  //分配了多少个block
    uint32_t offset;  //相对于存储设备是第几个block
    rt_slist_t list;
    struct lpm_dev *dev; /* 所属存储设备 */
};

/* 物理存储设备数据结构*/
/* 支持三种底层存储设备，RTT Block 设备、MTD Nand 设备、通用存储设备：struct lpm_dev */
struct lpm_dev /* 底层存储设备结构 */
{
    char name[LPM_NAME_MAX];
    rt_uint32_t sector_count; /**< count of sectors */
    rt_uint32_t bytes_per_sector; /**< number of bytes per sector */
    rt_uint32_t block_size; /**< number of bytes to erase one block */
    rt_uint32_t phy_start_addr; /**< physical start address */
    rt_slist_t dev_list;
    rt_slist_t part_list;
    void *mem_ptr;
    struct lpm_dev_ops *ops;
};
struct lpm_dev_ops /* 操作接口，待定 */
{
    int (*init)(struct lpm_dev *dev);
    int (*erase)(struct lpm_dev *dev, uint32_t block_no, size_t num);
    int (*read)(struct lpm_dev *dev, uint32_t sector_no, uint8_t *buf, size_t size);
    int (*write)(struct lpm_dev *dev, uint32_t sector_no, const uint8_t *buf, size_t size);
    int (*control)(struct lpm_dev *dev, int cmd, void *arg);
};

struct rt_lpm_device
{
    struct lpm_dev lpm_dev;
    rt_device_t rt_device;
};
/* 初始化 */
int lpm_init(void);

/* 创建分区，最大创建 4G 的分区大小 */
struct lpm_partition *lpm_partition_create(const char *dev_name, const char *name, uint32_t size);
int lpm_partition_delete(const char *dev_name, const char *name);
int lpm_partition_delete_all(const char *dev_name);

/* 创建匿名分区 */
int lpm_anonymity_create(const char *dev_name, uint32_t size, struct lpm_partition *lpm);
/* 删除匿名分区 */
int lpm_anonymity_delete(struct lpm_partition *lpm_par);

/* >4G 寻址空间，blk 的概念更清晰 */
struct lpm_partition * lpm_partition_find(const char *dev_name, const char *name);
int lpm_partition_read(const struct lpm_partition *part, uint32_t sector_no, uint8_t *buf, size_t size);
int lpm_partition_write(const struct lpm_partition *part, uint32_t sector_no, const uint8_t *buf, size_t size);
int lpm_partition_erase(const struct lpm_partition *part, uint32_t block_no, size_t num);
int lpm_partition_erase_all(const struct lpm_partition *part);
int lpm_partition_delete_self(const struct lpm_partition *part);

/* 将分区注册为块设备 */
struct rt_device *lpm_blk_device_create(const char *dev_name, const char *parition_name);

/* info */
int lpm_dump_partition(void);
int lpm_dump_dev(void);
char* lpm_get_version(struct lpm_dev *dev);
uint32_t lpm_partition_get_addr(const struct lpm_partition *part);
uint32_t lpm_free(const char *dev_name);

/* >4G 寻址空间，blk 的概念更清晰 */
struct lpm_dev *lpm_dev_find(const char *dev_name);
int lpm_dev_read(struct lpm_dev *dev, uint32_t sector_no, uint8_t *buf, size_t size);
int lpm_dev_write(struct lpm_dev *dev, uint32_t sector_no, const uint8_t *buf, size_t size);
int lpm_dev_erase(const struct lpm_dev *dev, uint32_t block_no, size_t num);
int lpm_dev_erase_all(const struct lpm_dev *dev);
int lpm_dev_control(struct lpm_dev *dev, int cmd, void *arg);

/* 底层接口 */
int lpm_dev_append(struct lpm_dev *dev);
int lpm_dev_blk_append(struct rt_device *dev);
int lpm_dev_nand_append(struct rt_device *dev);

int lpm_part_info_load(struct lpm_dev *dev);

uint32_t lpm_part_alloc(struct lpm_dev *dev, size_t size,uint8_t flag);
int lpm_part_free(struct lpm_dev *dev, uint32_t addr);
