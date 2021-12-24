/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-03     RTT       the first version
 */
#ifndef _BS_CFG_H_
#define _BS_CFG_H_

/* define the medium storage */
extern struct bs_medium fal_app;
extern struct bs_medium fal_old;
extern struct bs_medium fal_new;
extern struct bs_medium fal_args;
extern struct bs_medium fal_diff;
extern struct bs_medium fal_recovery;

#define BS_MEDIUM_TABLE     \
{                           \
    &fal_app,               \
    &fal_old,               \
    &fal_new,               \
    &fal_args,              \
    &fal_diff,              \
    &fal_recovery,          \
}

/* define the extractor */
extern struct bs_extract zlib_extractor;

#define BS_EXTRACTOR_TABLE  \
{                           \
    &zlib_extractor,        \
}

#endif /* _BS_CFG_H_ */
