/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "trace_config.h"
#include "system_trace.h"
#include "os_timer.h"
#include "otp.h"
#include "trace.h"

extern void *xTimerQueue;
extern void *io_queue_handle;
extern void *evt_queue_handle;

extern BOOL_PATCH_FUNC patch_vTaskSwitchContext;
extern void *xTimerQueue;

#if (TRACE_HEAP_EN == 1)
HEAP_INFO heap_info[TRACE_HEAP_TYPE_NUM];

void print_heap_info(void)
{
    for (uint32_t i = 0; i < TRACE_HEAP_TYPE_NUM; ++i)
    {
        OS_PRINT_TRACE4("heap type: %i, total size: %d, remain size: %d, minumum ever free size: %d",
                        i, heap_info[i].total_size, heap_info[i].curr_remain_size, heap_info[i].minimum_ever_free_size);

        OS_PRINT_TRACE1("free list: free block %d.", heap_info[i].free_size_list.number);
        for (uint32_t j = 0; j < heap_info[i].free_size_list.number; ++j)
        {
            OS_PRINT_TRACE3("heap type: %i,free block: %d, free size: %d", i,
                            heap_info[i].free_size_list.number, heap_info[i].free_size_list.size[j]);
        }
    }
}
#endif

#if (TRACE_STACK_EN == 1)
STACK_INFO stack_info;

void print_stack_info(void)
{
    OS_PRINT_TRACE1("task stack: task number %d", stack_info.task_number);
    for (uint32_t i = 0; i < stack_info.task_number; ++i)
    {
        OS_PRINT_TRACE3("task id: %d, name: %s, minimum ever remain size %d bytes",
                        stack_info.task_stack_info[i].task_id, TRACE_STRING(stack_info.task_stack_info[i].task_name),
                        stack_info.task_stack_info[i].minimum_ever_remain_size);
    }
    OS_PRINT_TRACE1("main stack: minimum ever remain size %d bytes",
                    stack_info.main_stack_minimum_ever_remain_size);
}
#endif

#if (TRACE_TIMER_EN == 1)
TIMER_INFO timer_info;

void print_timer_info(void)
{
    OS_PRINT_TRACE3("timer: total %d, current used %d, minimum ever remain %d",
                    OTP->timerMaxNumber, timer_info.curr_used_num, timer_info.minimum_ever_remain_num);
}
#endif

#if (TRACE_QUEUE_EN == 1)
extern uint32_t queue_handle_num0;
QUEUE_INFO queue_info[MAX_QUEUE_OBJECTS_NUM];
void print_queue_info(void)
{
    for (uint32_t i = 0; i < queue_handle_num0; i++)
    {
        OS_PRINT_TRACE4("queue %d: capacity_num %d, curr_num_in_queue %d, maximum_ever_num_in_queue %d",
                        i, queue_info[i].capacity_num, queue_info[i].curr_num_in_queue,
                        queue_info[i].maximum_ever_num_in_queue);
    }
}
#endif

void trace_timer_callback(void *p_timer)
{
#if (TRACE_HEAP_EN == 1)
    trace_heap();
    print_heap_info();
#endif

#if (TRACE_STACK_EN == 1)
    trace_task_stack();
    print_stack_info();
#endif

#if (TRACE_TIMER_EN == 1)
    trace_timer();
    print_timer_info();
#endif
#if (TRACE_QUEUE_EN == 1)
    trace_queue();
    print_queue_info();
#endif
}

void system_trace_init(void)
{
#if (TRACE_HEAP_EN == 1 || TRACE_STACK_EN == 1 || TRACE_TIMER_EN == 1)
    void *p_trace_timer = NULL;
    if (os_timer_create(&p_trace_timer, "trace_timer",  1, \
                        TRACE_PERIOD_TIME, true, trace_timer_callback))
    {
        os_timer_start(&p_trace_timer);
    }
#endif

#if (TRACE_TIMER_EN == 1)
    trace_timer_init();
#endif

#if (DEBUG_DLPS_ERROR_EN == 1)
    if (!trace_dlps_init(10000, 60000, 0))
    {
        OS_PRINT_ERROR0("trace dlps init fail!");
    }
#endif

#if (DEBUG_TASK_HANG_EN == 1)
    trace_task_hang_init(2, &xTimerQueue, 12, &evt_queue_handle, 10);
#endif

#if (TRACE_QUEUE_EN == 1)
    trace_queue_init(2, &xTimerQueue, &evt_queue_handle);
#endif

#if (TRACE_HARDFAULT == 1)
    /**
     * extern void *app_task_handle;
     * TCB_t *pxTimerTCB =(TCB_t *)(*(uint32_t *)xTimerTaskHandle);
     * TCB_t *pxLowerTCB =(TCB_t *)(*(uint32_t *)low_task_handle);
     * TCB_t *pxUpperTCB =(TCB_t *)(*(uint32_t *)upperstack_handle);
     * TCB_t *pxAppTCB =(TCB_t *)app_task_handle;
    */
    /**
     * @brief  Initializes trace_hardfault save to flash including critical task stack
     * @param  dump_task_stack_num: number of task
     * @return if it is successfully initialized , return true

     * For example
       hardfault_save_to_flash_init(2, (uint32_t *)low_task_handle, 1024, &app_task_handle, 1024);
       @param  2: dump two task stack
       @param  (uint32_t *)low_task_handle: lowstack task handler address
       @param  1024: dump task stack size
       @param  app_task_handle: app task handler value
       @param  1024: dump task stack size
       @note if need dump app task, must call this api after app task created!!!
           Upperstack handler address is different when used different upperstack img!
     */
    extern void *low_task_handle;
    extern void *xTimerTaskHandle;
    extern void *xIdleTaskHandle;
    extern void *app_task_handle;
    hardfault_save_to_flash_init(2, (uint32_t *)low_task_handle, 1024, &app_task_handle, 1024);
#endif
}


