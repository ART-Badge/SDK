/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-08     tyx          first implementation
 */

#include <rthw.h>
#include <rtthread.h>
#include <assert.h>
#include <cpuport.h>

#define THREAD_GC_TIMEOUT           (1000)

#define THREAD_GC_FLAG_INIT         (1)
#define THREAD_GC_FLAG_READY        (2)
#define THREAD_GC_FLAG_RUNING       (3)
#define THREAD_GC_FLAG_EXIT         (4)
#define THREAD_GC_FLAG_CORPSE       (5)

static rt_list_t gc_list = RT_LIST_OBJECT_INIT(gc_list);

static rt_thread_t thread_find(void* pEntry, void* pParam, rt_uint8_t* pStackAddr)
{
    struct rt_object* object = RT_NULL;
    struct rt_list_node* node = RT_NULL;
    struct rt_object_information* information = RT_NULL;
    rt_thread_t thread;
    rt_base_t level;

    information = rt_object_get_information(RT_Object_Class_Thread);
    /* parameter check */
    if (information == RT_NULL) return RT_NULL;
    /* enter critical */
    level = rt_hw_interrupt_disable();
    /* try to find object */
    rt_list_for_each(node, &(information->object_list))
    {
        thread = (rt_thread_t)rt_list_entry(node, struct rt_object, list);
        if (thread->entry == pEntry &&
            thread->parameter == pParam &&
            pStackAddr == (rt_uint8_t*)((char*)thread->stack_addr + thread->stack_size - sizeof(rt_ubase_t)))
        {
            /* leave critical */
            rt_hw_interrupt_enable(level);
            return thread;
        }
    }
    /* leave critical */
    rt_hw_interrupt_enable(level);
    return RT_NULL;
}

static int thread_run(sim_pointer_t parameter)
{
    sim_cpu_runner_t* runner = (sim_cpu_runner_t*)parameter;
    sim_char_t name[RT_NAME_MAX + 1] = { 0 };
    // check parameter
    assert(runner != NULL);
    // set thread name
    rt_strncpy(name, runner->self->name, RT_NAME_MAX);
    sim_thread_setname(&runner->parent, name);
    // send ack
    runner->state = THREAD_GC_FLAG_READY;
    sim_sem_release(&runner->ack);
    // wait sync
    sim_sem_take(&runner->syn, SIM_WAITING_FOREVER);
    assert(runner->state == THREAD_GC_FLAG_RUNING);
    // run current thread
    runner->entry(runner->thread_param);
    // set exit flag
    runner->state = THREAD_GC_FLAG_EXIT;
    runner->stamp = sim_mtime();
    //call rtthread exit function
    if (runner->exit)
        runner->exit();
    /* wait rt_hw_idle_hook exit thread.*/
    return 0;
}

rt_uint8_t *rt_hw_stack_init(void *pEntry,void *pParam, rt_uint8_t *pStackAddr,void *pExit)
{
    sim_cpu_runner_t* runner;
    rt_thread_t thread;
    rt_base_t level;
    sim_err_t err;

    thread = thread_find(pEntry, pParam, pStackAddr);
    RT_ASSERT(thread != RT_NULL);
    // get stack top
    pStackAddr = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)pStackAddr - sizeof(sim_cpu_runner_t), 8);
    // create runner object
    runner = sim_malloc(sizeof(*runner));
    RT_ASSERT(runner != NULL);
    *((sim_cpu_runner_t**)pStackAddr) = runner;
    // init rtthread param
    rt_memset(runner, 0, sizeof(*runner));
    runner->entry = pEntry;
    runner->thread_param = pParam;
    runner->exit = pExit;
    // init runner object
    runner->self = thread;
    runner->stamp = sim_mtime();
    runner->state = THREAD_GC_FLAG_INIT;
    //append gc list
    rt_list_init(&runner->node);
    level = rt_hw_interrupt_disable();
    rt_list_insert_after(&gc_list, &runner->node);
    rt_hw_interrupt_enable(level);
    //init sync sem
    err = sim_sem_init(&runner->syn, 0);
    RT_ASSERT(err == SIM_OK);
    err = sim_sem_init(&runner->ack, 0);
    RT_ASSERT(err == SIM_OK);
    err = sim_thread_init(&runner->parent, thread_run, runner, 0);
    RT_ASSERT(err == SIM_OK);
    //wait ready
    sim_sem_take(&runner->ack, SIM_WAITING_FOREVER);
    RT_ASSERT(runner->state == THREAD_GC_FLAG_READY);
    //suspend
    sim_thread_suspend(&runner->parent);
    sim_sem_release(&runner->syn);
    runner->state = THREAD_GC_FLAG_RUNING;
    return (rt_uint8_t *)pStackAddr;
}

void rt_hw_idle_hook(void)
{
    struct rt_thread* thread;
    struct rt_list_node* node, * bk_node;
    struct rt_list_node rm_list;
    struct rt_object_information* information;
    sim_cpu_runner_t* runner;
    rt_base_t level;
    sim_time_t stamp = sim_mtime();

    rt_list_init(&rm_list);
    /* clean thread handle */
    level = rt_hw_interrupt_disable();
    /* Scan zombie threads */
    information = rt_object_get_information(RT_Object_Class_Thread);
    if (information != RT_NULL)
    {
        /* mark all thread */
        rt_list_for_each_safe(node, bk_node, &(information->object_list))
        {
            thread = rt_list_entry(node, struct rt_thread, list);
            runner = *((sim_cpu_runner_t**)thread->sp);
            //update time stamp
            runner->stamp = stamp;
        }
        /* Check whether the thread is in the gc list  */
        rt_list_for_each_safe(node, bk_node, &gc_list)
        {
            runner = rt_list_entry(node, sim_cpu_runner_t, node);
            /* It has been removed from the RT-thread list for some time. Clean up the thread */
            if (stamp - runner->stamp > THREAD_GC_TIMEOUT && runner->state == THREAD_GC_FLAG_RUNING)
            {
                //set THREAD_GC_FLAG_CORPSE flag
                runner->state = THREAD_GC_FLAG_CORPSE;
            }
        }
    }
    /* check gc list */
    rt_list_for_each_safe(node, bk_node, &gc_list)
    {
        runner = rt_list_entry(node, sim_cpu_runner_t, node);
        if (stamp - runner->stamp > THREAD_GC_TIMEOUT)
        {
            // append remove list
            rt_list_remove(&runner->node);
            rt_list_insert_after(&rm_list, &runner->node);
        }
    }
    rt_hw_interrupt_enable(level);
    //gc
    while (rt_list_isempty(&rm_list) == RT_FALSE)
    {
        runner = rt_list_first_entry(&rm_list, sim_cpu_runner_t, node);
        rt_list_remove(&runner->node);
        if (runner->state == THREAD_GC_FLAG_EXIT)
        {
            sim_sem_deinit(&runner->ack);
            sim_sem_deinit(&runner->syn);
            sim_thread_deinit(&runner->parent);
            sim_free(runner);
        }
        else if (runner->state == THREAD_GC_FLAG_EXIT)
        {
            sim_sem_deinit(&runner->ack);
            sim_sem_deinit(&runner->syn);
            sim_thread_deinit(&runner->parent);
            sim_free(runner);
        }
        else
        {
            RT_ASSERT(0);
        }
    }
    RT_ASSERT(THREAD_GC_TIMEOUT >= 100);
    /* idle */
    sim_msleep(THREAD_GC_TIMEOUT / 100);
}

void rt_hw_cpu_shutdown(void)
{
    sim_shutdown();
}
