/*
 * File      : prc_recovery.c
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2021, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-14     realthread   first version
 */

#ifndef __PRC_UPGRADE_H__
#define __PRC_UPGRADE_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* prc progress callback function */
typedef void (*prc_progress)(int percentage);

/**
 * Description: prc progress callback function registration
 * @Parameter : prc_progress progress callback function
 * @Return: == 0 register success
 *          != 0 register failed
 * @Others: null
 */

int prc_progress_register(prc_progress callback);
/**
 * @description prc upgrade processing function
 * @param old_prc old prc file path
 * @param diff diff file path
 * @return = 0 successful
           < 0 failed
 * @others NULL
 */
int prc_upgrade(const char *old_prc, const char *diff);
    
#ifdef __cplusplus
}
#endif

#endif /* __PRC_UPGRADE_H__ */
