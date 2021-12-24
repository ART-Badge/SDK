/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-05     tyustli      the first version
 */
#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtthread.h>
#include <board.h>

extern const struct fal_flash_dev realtek_nor_flash;
extern struct fal_flash_dev sram0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &realtek_nor_flash,                                           	 \
    &sram0,                                                          \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */

#define FAL_PART_TABLE                                                                                                    \
{                                                                                                                         \
    {FAL_PART_MAGIC_WROD,        "rootfs",    "nor_flash",                                  0,      9 * 1024 * 1024, 0},  \
    {FAL_PART_MAGIC_WROD,        "user",      "nor_flash",                    9 * 1024 * 1024,      2 * 1024 * 1024, 0},  \
    {FAL_PART_MAGIC_WROD,        "fatfs",     "nor_flash",                    10 * 1024 * 1024,      3 * 1024 * 1024, 0},  \
    {FAL_PART_MAGIC_WROD,        "cfgmpool",  "sram0",                                     0,             20 * 1024, 0},  \
}
#endif /* FAL_PART_HAS_TABLE_CFG */
#endif /* _FAL_CFG_H_ */
