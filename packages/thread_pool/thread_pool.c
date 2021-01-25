/*
 * This file is part of the EasyDataManager Library.
 *
 * Copyright (C) 2013-2019 by Armink <armink.ztl@gmail.com>
 *
 * Function: a thread pool base on RT-Thread
 * Created on: 2013-11-14
 */

#include <thread_pool.h>

#define DBG_SECTION_NAME               "thread_pool"
#define DBG_LEVEL                      DBG_WARNING
#include <rtdbg.h>

#ifdef PKG_USING_THREAD_POOL

static thread_pool_err add_task(thread_pool_t const pool, void (*process)(void *arg), void *arg);
static thread_pool_err destroy(thread_pool_t pool);
static void thread_job(void* arg);
static void sync_lock(thread_pool_t pool);
static void sync_unlock(thread_pool_t pool);
static thread_pool_err del_all(thread_pool_t const pool);

/**
 * This function will initialize the thread pool.
 *
 * @param pool the thread_pool pointer
 * @param name the thread_pool name
 * @param max_thread_num the max thread number in this thread_pool
 * @param thread_stack_size the thread stack size in this thread_pool
 *
 * @return error code
 */
thread_pool_err init_thread_pool(thread_pool_t const pool, const char* name, rt_uint8_t max_thread_num,
        rt_uint32_t thread_stack_size) {
    thread_pool_err errorCode = THREAD_POOL_NO_ERR;
    char job_name[THREAD_POOL_NAME_MAX] = { 0 };
    rt_uint8_t i;

    RT_ASSERT(name);

    strncpy(pool->name, name, THREAD_POOL_NAME_MAX);
    strncpy(job_name, name, THREAD_POOL_NAME_MAX);

    pool->queue_lock = rt_mutex_create("tp_qlock", RT_IPC_FLAG_FIFO);
    RT_ASSERT(pool->queue_lock != NULL);
    pool->user_lock = rt_mutex_create("tp_ulock", RT_IPC_FLAG_FIFO);
    RT_ASSERT(pool->user_lock != NULL);
    pool->queue_ready = rt_sem_create("tp_qready", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(pool->queue_ready != NULL);
    pool->queue_head = NULL;
    pool->max_thread_num = max_thread_num;
    pool->cur_wait_thread_num = 0;
    pool->is_shutdown = RT_FALSE;
    pool->add_task = add_task;
    pool->del_all = del_all;
    pool->destroy = destroy;
    pool->lock = sync_lock;
    pool->unlock = sync_unlock;
    pool->thread_id = (rt_thread_t*) rt_malloc(max_thread_num * sizeof(rt_thread_t));
    RT_ASSERT(pool->thread_id != NULL);
    for (i = 0; i < max_thread_num; i++) {
        rt_snprintf(job_name, THREAD_POOL_NAME_MAX, "%s_%d", name, i);
        pool->thread_id[i] = rt_thread_create(job_name, thread_job, pool, thread_stack_size,
                THREAD_POOL_JOB_DEFAULT_PRIORITY, THREAD_POOL_JOB_TICK);
        RT_ASSERT(pool->thread_id[i] != NULL);
        rt_thread_startup(pool->thread_id[i]);
        LOG_D("create thread success.Current total thread number is %d", i + 1);
        rt_thread_delay(THREAD_POOL_THREADS_INIT_TIME);
    }
    LOG_D("initialize thread pool success!");

    return errorCode;
}

/**
 * This function will add a task to thread pool.
 *
 * @param pool the thread_pool pointer
 * @param process task function pointer
 * @param arg task function arguments
 *
 * @return error code
 */
static thread_pool_err add_task(thread_pool_t const pool, void (*process)(void *arg), void *arg) {
    thread_pool_err error_code = THREAD_POOL_NO_ERR;
    thread_pool_task_t member = NULL;
    thread_pool_task_t newtask = (thread_pool_task_t) rt_malloc(sizeof(thread_pool_task));
    if (!newtask) {
        LOG_W("Memory full!");
        return THREAD_POOL_MEM_FULL_ERR;
    }
    newtask->process = process;
    newtask->arg = arg;
    newtask->next = NULL;
    /* lock thread pool */
    rt_mutex_take(pool->queue_lock, RT_WAITING_FOREVER);
    member = pool->queue_head;
    /* task queue is NULL */
    if (member == NULL) {
        pool->queue_head = newtask;
    } else {
        /* look up for queue tail */
        while (member->next != NULL) {
            member = member->next;
        }
        member->next = newtask;
    }
    /* add current waiting thread number */
    pool->cur_wait_thread_num++;
    rt_mutex_release(pool->queue_lock);
    /* wake up a waiting thread to process task */
    rt_sem_release(pool->queue_ready);
    LOG_D("add a task to task queue success.");
    return error_code;
}

/**
 * This function will delete all wait task.
 *
 * @param pool the thread_pool pointer
 *
 * @return error code
 */
static thread_pool_err del_all(thread_pool_t const pool) {
    thread_pool_err error = THREAD_POOL_NO_ERR;

    rt_mutex_take(pool->queue_lock, RT_WAITING_FOREVER);
    /* delete all task in queue */
    for (;;) {
        if (pool->queue_head != NULL) {
            rt_free(pool->queue_head);
            pool->queue_head = pool->queue_head->next;
            pool->cur_wait_thread_num--;
        } else {
            break;
        }
    }
    rt_sem_control(pool->queue_ready, RT_IPC_CMD_RESET, NULL);
    LOG_D("delete all wait task success");
    rt_mutex_release(pool->queue_lock);
    return error;
}

/**
 * This function will destroy thread pool.
 *
 * @param pool the thread_pool pointer
 *
 * @return error code
 */
static thread_pool_err destroy(thread_pool_t pool) {
    thread_pool_err error = THREAD_POOL_NO_ERR;
    thread_pool_task_t head = NULL;
    rt_uint8_t i;
    if (pool->is_shutdown) {/* thread already shutdown */
        error = THREAD_POOL_ALREADY_SHUTDOWN_ERR;
    }
    if (error == THREAD_POOL_NO_ERR) {
        pool->is_shutdown = RT_TRUE;
        /* wait all thread exit */
        for (i = 0; i < pool->max_thread_num; i++) {
            rt_thread_delete(pool->thread_id[i]);
        }
        /* wake up all thread from broadcast */
        /* delete mutex and semaphore then all waiting thread will wake up */
        rt_mutex_delete(pool->queue_lock);
        rt_sem_delete(pool->queue_ready);
        /* release memory */
        rt_free(pool->thread_id);
        pool->thread_id = NULL;
        /* destroy task queue */
        while (pool->queue_head != NULL) {
            head = pool->queue_head;
            pool->queue_head = pool->queue_head->next;
            rt_free(head);
        }
        /* destroy mutex */
        rt_mutex_delete(pool->user_lock);
        pool = NULL;
        LOG_D("Thread pool destroy success");
    }
    return error;
}

/**
 * This function is thread job.
 *
 * @param arg the thread job arguments
 *
 */
static void thread_job(void* arg) {
    thread_pool_t pool = NULL;
    thread_pool_task_t task = NULL;
    while (1) {
        pool = (thread_pool_t) arg;
        /* lock thread pool */
        rt_mutex_take(pool->queue_lock, RT_WAITING_FOREVER);
        /* If waiting thread number is 0 ,and thread is not shutdown.
         * The thread will block.
         * Before thread block the queueLock will unlock.
         * After thread wake up ,the queueLock will relock.*/
        while (pool->cur_wait_thread_num == 0 && !pool->is_shutdown) {
            /* ququeReady is NULL,the thread will block */
            if (pool->queue_ready->value == 0) {
                rt_mutex_release(pool->queue_lock);
                rt_sem_take(pool->queue_ready, RT_WAITING_FOREVER);
                rt_mutex_take(pool->queue_lock, RT_WAITING_FOREVER);
            } else {/* ququeReady is not NULL,the ququeReady semaphore will decrease */
                rt_sem_take(pool->queue_ready, RT_WAITING_FOREVER);
            }
        }
        if (pool->is_shutdown) { /* thread pool will shutdown */
            rt_mutex_release(pool->queue_lock);
            return;
        }
        RT_ASSERT(pool->cur_wait_thread_num != 0);
        RT_ASSERT(pool->queue_head != NULL);
        /* load task to thread job */
        pool->cur_wait_thread_num--;
        task = pool->queue_head;
        pool->queue_head = task->next;
        rt_mutex_release(pool->queue_lock);
        /* run task */
        (*(task->process))(task->arg);
        /* release memory */
        rt_free(task);
        task = NULL;
    }
}

/**
 * This function will lock the synchronized lock.
 *
 * @param pool the thread_pool pointer
 *
 */
static void sync_lock(thread_pool_t pool) {
    rt_mutex_take(pool->user_lock, RT_WAITING_FOREVER);
}

/**
 * This function will unlock the synchronized lock.
 *
 * @param pool the thread_pool pointer
 *
 */
static void sync_unlock(thread_pool_t pool) {
    rt_mutex_release(pool->user_lock);
}

#endif /* PKG_USING_THREAD_POOL */
