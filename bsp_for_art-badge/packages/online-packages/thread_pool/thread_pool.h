/*
 * This file is part of the EasyDataManager Library.
 *
 * Copyright (C) 2013-2019 by Armink <armink.ztl@gmail.com>
 *
 * Function: a thread pool base on RT-Thread
 * Created on: 2013-11-14
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <rtthread.h>
#include <string.h>

#define THREAD_POOL_VER              "4.0.1"

#define THREAD_POOL_THREADS_INIT_TIME     30    /**< threads initialize average waiting time */
#define THREAD_POOL_JOB_DEFAULT_PRIORITY  10    /**< thread poll job's priority in rt-thread */
#define THREAD_POOL_JOB_TICK               5    /**< thread poll job's time slice in rt-thread */
#define THREAD_POOL_NAME_MAX     RT_NAME_MAX    /**< thread poll max name length */

/* thread pool error code */
typedef enum {
    THREAD_POOL_NO_ERR,                 /**< no error */
    THREAD_POOL_ALREADY_SHUTDOWN_ERR,   /**< thread pool already shutdown */
    THREAD_POOL_MEM_FULL_ERR,           /**< memory full */
} thread_pool_err;

/* a task queue which run in thread pool */
typedef struct _thread_pool_task {
    void (*process)(void *arg);  /**< task callback function */
    void *arg;                   /**< task callback function's arguments */
    struct _thread_pool_task *next;
} thread_pool_task, *thread_pool_task_t;

/* thread pool struct */
typedef struct _thread_pool {
    char name[THREAD_POOL_NAME_MAX + 1];/**< the name of ThreadPool, the end of name is include '\0' */
    thread_pool_task_t queue_head;      /**< task queue which place all waiting task */
    rt_mutex_t user_lock;               /**< a synchronized lock provided to user */
    rt_mutex_t queue_lock;              /**< task queue mutex lock */
    rt_sem_t queue_ready;               /**< a semaphore which for task queue ready */
    rt_uint8_t is_shutdown;                /**< shutdown state,if shutdown the value will equal TRUE  */
    rt_thread_t* thread_id;             /**< thread queue which in thread pool */
    rt_uint8_t max_thread_num;             /**< the thread max number in thread pool */
    rt_uint8_t cur_wait_thread_num;        /**< the current waiting thread number in thread pool */
    /**
     * This function will add a task to thread pool.
     *
     * @param pool the ThreadPool pointer
     * @param process task function pointer
     * @param arg task function arguments
     *
     * @return error code
     */
    thread_pool_err (*add_task)(struct _thread_pool* const pool,
            void (*process)(void *arg), void *arg);
    /**
     * This function will delete all task.
     *
     * @param pool
     *
     * @return error code
     */
    thread_pool_err (*del_all)(struct _thread_pool* const pool);
    /**
     * This function will destroy thread pool.
     *
     * @param pool the ThreadPool pointer
     *
     * @return error code
     */
    thread_pool_err (*destroy)(struct _thread_pool* pool);
    /**
     * This function will lock the synchronized lock.
     *
     * @param pool the ThreadPool pointer
     *
     */
    void (*lock)(struct _thread_pool* pool);
    /**
     * This function will unlock the synchronized lock.
     *
     * @param pool the ThreadPool pointer
     *
     */
    void (*unlock)(struct _thread_pool* pool);
} thread_pool, *thread_pool_t;

thread_pool_err init_thread_pool(thread_pool_t const pool, const char* name, rt_uint8_t max_thread_num,
        rt_uint32_t thread_stack_size);

#endif /* THREAD_POOL_H_ */
