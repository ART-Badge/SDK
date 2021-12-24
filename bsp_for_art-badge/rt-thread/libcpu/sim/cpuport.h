/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-18     tyx          first implementation
 */

#ifndef __CPUPORT_H__
#define __CPUPORT_H__

#include <sim.h>

#define SIM_CPU_STACK_MAGIC     0x435055537461636B

typedef struct __sim_cpu_runner_t
{
    sim_thread_t parent;        // parent thread
    sim_sem_t syn;              // thread synchronization signal
    sim_sem_t ack;              // thread response signal
    rt_thread_t self;           // rt_thread_self()
    rt_list_t node;             // append gc list
    sim_time_t stamp;           // time stamp
    sim_flag_t state;
    /* rtthread thread parameter */
    void (*entry)(void* param);
    void* thread_param;
    void (*exit)(void);
}sim_cpu_runner_t;

#endif
