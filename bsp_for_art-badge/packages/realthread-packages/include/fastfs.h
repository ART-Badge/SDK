/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020.12.11     flybreak  first version
 */

#include <rtthread.h>

/*
 * 0. mkfs
 * 1. open file
 * 2. write/read file
 * 3. close file
 *
 * */

/*
 * struct fastfs_root
 * file_num
 * file_list[]
 *
 * */

#define FASTFS_VER          0x0001
#define FILE_NEME_MAX       64
#define FASTFS_CACHE_ENABLE

#ifndef FASTFS_CACHE_SIZE
#define FASTFS_CACHE_SIZE   4096
#endif

#define CACHE_FLAG_RD 0
#define CACHE_FLAG_WR 1

struct fastfs_root
{
    uint32_t magic_word;
    uint32_t version;
    uint32_t block_size;
    uint32_t sector_size;
    uint8_t  root_size;
    uint8_t  reserved1;
    uint16_t dents_num;
    uint8_t  reserved2[4];
};

struct fastfs_cache
{
    uint8_t cache[FASTFS_CACHE_SIZE];
    size_t pos;
    size_t size;
    uint8_t rwflag;
};

struct fastfs_file_entry
{
    uint8_t  type;    /* file type */
    uint8_t  reserved[3];
    char     name[FILE_NEME_MAX];
    uint32_t phy_pos; /* physical position */
    uint32_t size;    /* file size */
    uint32_t ssize;   /* self size */
};

//struct fastfs_dir_entry
//{
//    uint8_t  type;    /* file type */
//    uint8_t  reserved[3];
//    char     name[FILE_NEME_MAX];
//    uint32_t phy_pos; /* physical position */
//    uint32_t size;    /* file size */
//    uint32_t ssize;   /* self size */
//};

struct fastfs_file
{
    uint32_t type:8;    /* file type */
    uint32_t pos:24;
    char     name[FILE_NEME_MAX];
    uint32_t phy_pos; /* physical position */
    uint32_t size;    /* file size */
    void    *data;    /* Specific file system data */
#ifdef FASTFS_CACHE_ENABLE
    struct fastfs_cache *rwcache;
//    struct fastfs_cache *wcache;
#endif
    uint16_t  dents_num;
};

void check_fs(void);




