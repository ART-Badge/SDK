/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-07-29     RTT       the first version
 */
#ifndef _BSDIFF_LIB_H_
#define _BSDIFF_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bs_def.h"

/* bsdiff args position index */
typedef enum bs_step
{
    BS_STEP_CLEAR,
    BS_STEP_VERIFY_DIFF,
    BS_STEP_VERIFY_OLD_APP,
    BS_STEP_BACKUP_OLD_APP,
    BS_STEP_VERIFY_BACKUP_APP,
    BS_STEP_ERASE_OLD_APP,
    BS_STEP_PATCH_OLD_APP,
    BS_STEP_VERIFY_NEW_APP,
    BS_STEP_MAX,
} bs_step_t;

/* bsdiff args position index */
typedef enum bs_restore_step
{
    BS_RESTORE_STEP_CLEAR,
    BS_RESTORE_STEP_VERIFY_DIFF,
    BS_RESTORE_STEP_VERIFY_BACKUP_APP,
    BS_RESTORE_STEP_REVERT_BACKUP_APP,
    BS_RESTORE_STEP_REVERT_FACTORY_APP,
    BS_RESTORE_STEP_MAX,
} bs_restore_step_t;

/* bsdiff upgrade status */
typedef enum bs_update_status
{
    BS_UPDATE_STATUS_FINISH = 0,
    BS_UPDATE_STATUS_START  = 1,
    BS_UPDATE_STATUS_MAX    = 2,
} bs_update_status_t;

/* bsdiff upgrade status */
typedef enum bs_update_result
{
    BS_UPDATE_RESULT_CLEAR  = 0,
    BS_UPDATE_RESULT_SUCCESS= 1,
    BS_UPDATE_RESULT_FAILED = 2,
    BS_UPDATE_RESULT_MAX    = 3,
} bs_update_result_t;

/* bsdiff args position index */
typedef enum bs_args_index
{
    BS_ARGS_UPDATE_START    = 0,
    BS_ARGS_UPDATE_RESULT   = 1,
    BS_ARGS_UPDATE_STEP     = 2,
    BS_ARGS_MAX,
} bs_args_index_t;

/* bsdiff args struct */
typedef struct bs_args
{
    uint8_t update_start;
    uint8_t update_result;
    uint8_t update_step;
} bs_args_t;

/* bsdiff progress type */
typedef enum bs_proc_type
{
    BS_UPGRADE_PROCESS = 0,
    BS_RESTORE_PROCESS = 1,
} bs_proc_t;

/* bsdiff progress callback function */
typedef void (*bs_progress)(bs_proc_t proc, int step, int result, uint32_t percentage);
/* bsdiff upfdate result callback function */
typedef void (*bs_result)(bs_update_result_t result);

/*
 * Bsdiff library all user interface
 */

/* Step 1: Use this interface to initialize the bsdiff library */
int bsdiff_init(void);

/*
 * Bsdiff library bootloader interface
 */
#ifdef BSDIFF_LIB_USING_IN_BOOTLOADER
/* Step 2: Execute the processing function */
int bsdiff_process(bs_progress callback);
/* Step 3: If an exception occurs, you can recover through this interface */
int bsdiff_recovery(void);
#endif /* BSDIFF_LIB_USING_IN_BOOTLOADER */

/*
 * Bsdiff library application interface
 */
#ifdef BSDIFF_LIB_USING_IN_APP
/* Step 2: If firmware upgrade is required, call this interface to notify the bootloader */
int bsdiff_notify_update(void);
/* Step 3: Execute upgrade completion processing function */
int bsdiff_updated(bs_result callback);

/* Others: You can use this interface to notify the bootloader to restore the factory */
int bsdiff_notify_factory(void);
#endif /* BSDIFF_LIB_USING_IN_APP */

#endif /* _BSDIFF_LIB_H_ */
