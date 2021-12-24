/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-02-27     bernard      fix the re-work issue.
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_HEAP

static void _delayed_work_timeout_handler(void *parameter);

rt_inline rt_err_t _workqueue_work_completion(struct rt_workqueue *queue)
{
    rt_err_t result;

    rt_enter_critical();
    while (1)
    {
        /* try to take condition semaphore */
        result = rt_sem_trytake(&(queue->sem));
        if (result == -RT_ETIMEOUT)
        {
            /* it's timeout, release this semaphore */
            rt_sem_release(&(queue->sem));
        }
        else if (result == RT_EOK)
        {
            /* keep the sem value = 0 */
            result = RT_EOK;
            break;
        }
        else
        {
            result = -RT_ERROR;
            break;
        }
    }
    rt_exit_critical();

    return result;
}

static void _workqueue_thread_entry(void *parameter)
{
    rt_base_t level;
    struct rt_work *work;
    struct rt_workqueue *queue;

    queue = (struct rt_workqueue *) parameter;
    RT_ASSERT(queue != RT_NULL);

    while (1)
    {
        level = rt_hw_interrupt_disable();
        if (rt_list_isempty(&(queue->work_list)))
        {
            /* no software timer exist, suspend self. */
            rt_thread_suspend(rt_thread_self());
            rt_hw_interrupt_enable(level);
            rt_schedule();
            continue;
        }

        /* we have work to do with. */
        work = rt_list_entry(queue->work_list.next, struct rt_work, list);
        rt_list_remove(&(work->list));
        queue->work_current = work;
        work->flags &= ~RT_WORK_STATE_PENDING;
        work->workqueue = RT_NULL;
        rt_hw_interrupt_enable(level);

        /* do work */
        work->work_func(work, work->work_data);
        /* clean current work */
        queue->work_current = RT_NULL;

        /* ack work completion */
        _workqueue_work_completion(queue);
    }
}

static rt_err_t _workqueue_submit_work(struct rt_workqueue *queue,
        struct rt_work *work, rt_tick_t ticks)
{
    rt_base_t level;
    rt_err_t err;

    level = rt_hw_interrupt_disable();
    /* remove list */
    rt_list_remove(&(work->list));
    work->flags &= ~RT_WORK_STATE_PENDING;
    /*  */
    if (ticks == 0)
    {
        if (queue->work_current != work)
        {
            rt_list_insert_after(queue->work_list.prev, &(work->list));
            work->flags |= RT_WORK_STATE_PENDING;
            work->workqueue = queue;
            err = RT_EOK;
        }
        else
        {
            err = -RT_EBUSY;
        }

        /* whether the workqueue is doing work */
        if (queue->work_current == RT_NULL &&
            ((queue->work_thread->stat & RT_THREAD_STAT_MASK) == RT_THREAD_SUSPEND))
        {
            /* resume work thread */
            rt_thread_resume(queue->work_thread);
            rt_hw_interrupt_enable(level);
            rt_schedule();
        }
        else
        {
            rt_hw_interrupt_enable(level);
        }
        return err;
    }
    else if (ticks < RT_TICK_MAX / 2)
    {
        /* Timer started */
        if (work->flags & RT_WORK_STATE_SUBMITTING)
        {
            rt_timer_stop(&work->timer);
            rt_timer_control(&work->timer, RT_TIMER_CTRL_SET_TIME, &ticks);
        }
        else
        {
            rt_timer_init(&(work->timer), "work", _delayed_work_timeout_handler,
                        work, ticks, RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
            work->flags |= RT_WORK_STATE_SUBMITTING;
        }
        work->workqueue = queue;
        /* insert delay work list */
        rt_list_insert_after(queue->delayed_list.prev, &(work->list));
        rt_hw_interrupt_enable(level);
        rt_timer_start(&(work->timer));
        return RT_EOK;
    }
    rt_hw_interrupt_enable(level);
    return -RT_ERROR;
}

static rt_err_t _workqueue_cancel_work(struct rt_workqueue *queue, struct rt_work *work)
{
    rt_base_t level;
    rt_err_t err;

    level = rt_hw_interrupt_disable();
    rt_list_remove(&(work->list));
    work->flags &= ~RT_WORK_STATE_PENDING;
    /* Timer started */
    if (work->flags & RT_WORK_STATE_SUBMITTING)
    {
        rt_timer_stop(&(work->timer));
        rt_timer_detach(&(work->timer));
        work->flags &= ~RT_WORK_STATE_SUBMITTING;
    }
    err = queue->work_current != work ? RT_EOK : -RT_EBUSY;
    work->workqueue = RT_NULL;
    rt_hw_interrupt_enable(level);
    return err;
}

static void _delayed_work_timeout_handler(void *parameter)
{
    struct rt_work *work;
    struct rt_workqueue *queue;
    rt_base_t level;

    work = (struct rt_work *)parameter;
    queue = work->workqueue;
    RT_ASSERT(queue != RT_NULL);

    level = rt_hw_interrupt_disable();
    rt_timer_detach(&(work->timer));
    work->flags &= ~RT_WORK_STATE_SUBMITTING;
    /* remove delay list */
    rt_list_remove(&(work->list));
    /* insert work queue */
    if (queue->work_current != work)
    {
        rt_list_insert_after(queue->work_list.prev, &(work->list));
        work->flags |= RT_WORK_STATE_PENDING;
    }
    /* whether the workqueue is doing work */
    if (queue->work_current == RT_NULL &&
        ((queue->work_thread->stat & RT_THREAD_STAT_MASK) == RT_THREAD_SUSPEND))
    {
        /* resume work thread */
        rt_thread_resume(queue->work_thread);
        rt_hw_interrupt_enable(level);
        rt_schedule();
    }
    else
    {
        rt_hw_interrupt_enable(level);
    }
}

struct rt_workqueue *rt_workqueue_create(const char *name, rt_uint16_t stack_size, rt_uint8_t priority)
{
    struct rt_workqueue *queue = RT_NULL;

    queue = (struct rt_workqueue *)RT_KERNEL_MALLOC(sizeof(struct rt_workqueue));
    if (queue != RT_NULL)
    {
        /* initialize work list */
        rt_list_init(&(queue->work_list));
        rt_list_init(&(queue->delayed_list));
        queue->work_current = RT_NULL;
        rt_sem_init(&(queue->sem), "wqueue", 0, RT_IPC_FLAG_FIFO);

        /* create the work thread */
        queue->work_thread = rt_thread_create(name, _workqueue_thread_entry, queue, stack_size, priority, 10);
        if (queue->work_thread == RT_NULL)
        {
            RT_KERNEL_FREE(queue);
            return RT_NULL;
        }

        rt_thread_startup(queue->work_thread);
    }

    return queue;
}

rt_err_t rt_workqueue_destroy(struct rt_workqueue *queue)
{
    RT_ASSERT(queue != RT_NULL);

    rt_workqueue_cancel_all_work(queue);
    rt_thread_delete(queue->work_thread);
    rt_sem_detach(&(queue->sem));
    RT_KERNEL_FREE(queue);

    return RT_EOK;
}

rt_err_t rt_workqueue_dowork(struct rt_workqueue *queue, struct rt_work *work)
{
    RT_ASSERT(queue != RT_NULL);
    RT_ASSERT(work != RT_NULL);

    return _workqueue_submit_work(queue, work, 0);
}

rt_err_t rt_workqueue_submit_work(struct rt_workqueue *queue, struct rt_work *work, rt_tick_t time)
{
    RT_ASSERT(queue != RT_NULL);
    RT_ASSERT(work != RT_NULL);

    return _workqueue_submit_work(queue, work, time);
}

rt_err_t rt_workqueue_critical_work(struct rt_workqueue *queue, struct rt_work *work)
{
    rt_base_t level;
    RT_ASSERT(queue != RT_NULL);
    RT_ASSERT(work != RT_NULL);

    level = rt_hw_interrupt_disable();
    /* NOTE: the work MUST be initialized firstly */
    rt_list_remove(&(work->list));
    rt_list_insert_after(&queue->work_list, &(work->list));
    /* whether the workqueue is doing work */
    if (queue->work_current == RT_NULL &&
        ((queue->work_thread->stat & RT_THREAD_STAT_MASK) == RT_THREAD_SUSPEND))
    {
        /* resume work thread */
        rt_thread_resume(queue->work_thread);
        rt_hw_interrupt_enable(level);
        rt_schedule();
    }
    else
    {
        rt_hw_interrupt_enable(level);
    }

    return RT_EOK;
}

rt_err_t rt_workqueue_cancel_work(struct rt_workqueue *queue, struct rt_work *work)
{
    RT_ASSERT(work != RT_NULL);
    RT_ASSERT(queue != RT_NULL);
    return _workqueue_cancel_work(queue, work);
}

rt_err_t rt_workqueue_cancel_work_sync(struct rt_workqueue *queue, struct rt_work *work)
{
    RT_ASSERT(queue != RT_NULL);
    RT_ASSERT(work != RT_NULL);

    if (queue->work_current == work) /* it's current work in the queue */
    {
        /* wait for work completion */
        rt_sem_take(&(queue->sem), RT_WAITING_FOREVER);
    }
    else
    {
        _workqueue_cancel_work(queue, work);
    }

    return RT_EOK;
}

rt_err_t rt_workqueue_cancel_all_work(struct rt_workqueue *queue)
{
    struct rt_work *work;

    RT_ASSERT(queue != RT_NULL);

    /* cancel work */
    rt_enter_critical();
    while (rt_list_isempty(&queue->work_list) == RT_FALSE)
    {
        work = rt_list_first_entry(&queue->work_list, struct rt_work, list);
        _workqueue_cancel_work(queue, work);
    }
    /* cancel delay work */
    while (rt_list_isempty(&queue->delayed_list) == RT_FALSE)
    {
        work = rt_list_first_entry(&queue->delayed_list, struct rt_work, list);
        _workqueue_cancel_work(queue, work);
    }
    rt_exit_critical();

    return RT_EOK;
}

void rt_delayed_work_init(struct rt_delayed_work *work, void (*work_func)(struct rt_work *work,
                          void *work_data), void *work_data)
{
    rt_work_init(&work->work, work_func, work_data);
}

#ifdef RT_USING_SYSTEM_WORKQUEUE
static struct rt_workqueue *sys_workq;

rt_err_t rt_work_submit(struct rt_work *work, rt_tick_t time)
{
    return rt_workqueue_submit_work(sys_workq, work, time);
}

rt_err_t rt_work_cancel(struct rt_work *work)
{
    return rt_workqueue_cancel_work(sys_workq, work);
}

int rt_work_sys_workqueue_init(void)
{
    if (sys_workq != RT_NULL)
        return RT_EOK;

    sys_workq = rt_workqueue_create("sys_work", RT_SYSTEM_WORKQUEUE_STACKSIZE,
                                    RT_SYSTEM_WORKQUEUE_PRIORITY);
    RT_ASSERT(sys_workq != RT_NULL);

    return RT_EOK;
}
INIT_PREV_EXPORT(rt_work_sys_workqueue_init);
#endif
#endif
