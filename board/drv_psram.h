/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-20     tyustli      first version
 * 2021-01-23     WillianChan  modify the size of psram to be initialized
 */

#ifndef __DRV_PSRAM_H
#define __DRV_PSRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#define PSRAM_BEGIN_ADDR    0x69ce000
#define PSRAM_SIZE          0x0032000   /* 200KB, the rest is initialized in smodule */

#ifdef __cplusplus
}
#endif

#endif /* __DRV_PSRAM_H */
