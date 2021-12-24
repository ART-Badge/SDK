/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-29     tyx          first implementation
 */


#include <rtthread.h>
#include <cpuport.h>
#include <sim.h>

static rt_ubase_t rt_interrupt_from_thread, rt_interrupt_to_thread;
static rt_ubase_t rt_thread_switch_interrupt_flag;

rt_base_t rt_hw_interrupt_disable(void)
{
    return (rt_base_t)sim_interrupt_disable();
}

void rt_hw_interrupt_enable(rt_base_t level)
{
    sim_interrupt_enable((long)level);
}

void rt_hw_context_switch_to(rt_ubase_t to)
{
    // init thread switch state
    rt_interrupt_to_thread = to;
    rt_interrupt_from_thread = 0;
    rt_thread_switch_interrupt_flag = 1;
    // start system tick
    sim_systick_init(RT_TICK_PER_SECOND);
    // trigger schedule
    sim_interrupt_schedule_trigger();
    // start interrupt server
    sim_interrupt_server_run();
    //stop system tick
    sim_systick_deinit();
}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to)
{
    if (rt_thread_switch_interrupt_flag != 1)
    {
        rt_thread_switch_interrupt_flag = 1;
        // set rt_interrupt_from_thread
        rt_interrupt_from_thread = from;
    }
    // set rt_interrupt_to_thread
    rt_interrupt_to_thread = to;
    sim_interrupt_schedule_trigger();
}

void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to)
{
    if (rt_thread_switch_interrupt_flag != 1)
    {
        rt_thread_switch_interrupt_flag = 1;
        // set rt_interrupt_from_thread
        rt_interrupt_from_thread = from;
    }
    // set rt_interrupt_to_thread
    rt_interrupt_to_thread = to;
    sim_interrupt_schedule_trigger();
}

void sim_interrupt_schedule_handler(void)
{
    sim_cpu_runner_t* from, * to;

    if (rt_interrupt_from_thread == rt_interrupt_to_thread||
        rt_thread_switch_interrupt_flag == 0)
    {
        return;
    }
    if (rt_interrupt_from_thread)
    {
        from = ((sim_cpu_runner_t **)(*((rt_ubase_t *)rt_interrupt_from_thread)))[0];
        sim_thread_suspend(&from->parent);
    }
    if (rt_interrupt_to_thread)
    {
        to = ((sim_cpu_runner_t**)(*((rt_ubase_t*)rt_interrupt_to_thread)))[0];
        sim_thread_resume(&to->parent);
    }
    rt_thread_switch_interrupt_flag = 0;
}

void sim_interrupt_context_load(sim_handle_t context[64])
{
    sim_cpu_runner_t* current_runner;

    context[0] = (sim_handle_t)rt_hw_interrupt_disable();
    if (rt_interrupt_to_thread)
    {
        current_runner = ((sim_cpu_runner_t**)(*((rt_ubase_t*)rt_interrupt_to_thread)))[0];
        context[1] = (sim_handle_t)current_runner;
        sim_thread_suspend(&current_runner->parent);
    }
    else
    {
        context[1] = (sim_handle_t)SIM_NULL;
    }
}

void sim_interrupt_context_store(sim_handle_t context[64])
{
    sim_cpu_runner_t* current_runner;

    current_runner = (sim_cpu_runner_t*)context[1];
    if (current_runner)
    {
        sim_thread_resume(&current_runner->parent);
    }
    rt_hw_interrupt_enable((rt_base_t)context[0]);
}
