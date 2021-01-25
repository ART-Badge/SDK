/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _PATCH_OS_H_
#define _PATCH_OS_H_
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*BOOL_PATCH_FUNC)();

/** @brief LogMissedCounter records the number of log missed.
    If the value is greater then 0, please reduce logs or speed up log uart/DMA.
    */
typedef struct
{
    uint8_t *pInputBuffer;
    uint8_t *pOutputBuffer;
    uint16_t InputBufferSize;
    uint16_t OutputBufferSize;
    uint32_t LogMissedCounter;
    uint32_t MaxBufferSize;
} PingpongBuffer;

/* OSIF patch function */
extern BOOL_PATCH_FUNC patch_osif_os_systick_handler;
extern BOOL_PATCH_FUNC patch_osif_os_delay;
extern BOOL_PATCH_FUNC patch_osif_os_sys_time_get;
extern BOOL_PATCH_FUNC patch_osif_os_sys_tick_get;
extern BOOL_PATCH_FUNC patch_osif_os_sys_tick_increase;
extern BOOL_PATCH_FUNC patch_osif_os_sched_start;
extern BOOL_PATCH_FUNC patch_osif_os_sched_stop;
extern BOOL_PATCH_FUNC patch_osif_os_sched_suspend;
extern BOOL_PATCH_FUNC patch_osif_os_sched_resume;
extern BOOL_PATCH_FUNC patch_osif_os_sched_is_start;
extern BOOL_PATCH_FUNC patch_osif_os_vector_table_update;
extern BOOL_PATCH_FUNC patch_osif_os_init;

extern BOOL_PATCH_FUNC patch_osif_os_task_create;
extern BOOL_PATCH_FUNC patch_osif_os_task_delete;
extern BOOL_PATCH_FUNC patch_osif_os_task_suspend;
extern BOOL_PATCH_FUNC patch_osif_os_task_resume;
extern BOOL_PATCH_FUNC patch_osif_os_task_yield;
extern BOOL_PATCH_FUNC patch_osif_os_task_handle_get;
extern BOOL_PATCH_FUNC patch_osif_os_task_priority_get;
extern BOOL_PATCH_FUNC patch_osif_os_task_priority_set;

extern BOOL_PATCH_FUNC patch_osif_os_task_status_dump;
extern BOOL_PATCH_FUNC patch_osif_os_task_dlps_return_idle_task;

extern BOOL_PATCH_FUNC patch_osif_os_lock;
extern BOOL_PATCH_FUNC patch_osif_os_unlock;
extern BOOL_PATCH_FUNC patch_osif_os_sem_create;
extern BOOL_PATCH_FUNC patch_osif_os_sem_delete;
extern BOOL_PATCH_FUNC patch_osif_os_sem_take;
extern BOOL_PATCH_FUNC patch_osif_os_sem_give;
extern BOOL_PATCH_FUNC patch_osif_os_mutex_create;
extern BOOL_PATCH_FUNC patch_osif_os_mutex_delete;
extern BOOL_PATCH_FUNC patch_osif_os_mutex_take;
extern BOOL_PATCH_FUNC patch_osif_os_mutex_give;

extern BOOL_PATCH_FUNC patch_osif_os_msg_queue_create_intern;
extern BOOL_PATCH_FUNC patch_osif_os_msg_queue_delete_intern;
extern BOOL_PATCH_FUNC patch_osif_os_msg_queue_peek_intern;
extern BOOL_PATCH_FUNC patch_osif_os_msg_send_intern;
extern BOOL_PATCH_FUNC patch_osif_os_msg_recv_intern;
extern BOOL_PATCH_FUNC patch_osif_os_msg_peek_intern;

extern BOOL_PATCH_FUNC patch_osif_os_mem_alloc_intern;
extern BOOL_PATCH_FUNC patch_osif_os_mem_zalloc_intern;
extern BOOL_PATCH_FUNC patch_osif_os_mem_aligned_alloc_intern;
extern BOOL_PATCH_FUNC patch_osif_os_mem_free;
extern BOOL_PATCH_FUNC patch_osif_os_mem_aligned_free;
extern BOOL_PATCH_FUNC patch_osif_os_mem_peek;
extern BOOL_PATCH_FUNC patch_osif_os_mem_check_heap_usage;

extern BOOL_PATCH_FUNC patch_osif_os_pool_create_intern;
extern BOOL_PATCH_FUNC patch_osif_os_pool_extend_intern;
extern BOOL_PATCH_FUNC patch_osif_os_pool_delete_intern;
extern BOOL_PATCH_FUNC patch_osif_os_pool_dump;
extern BOOL_PATCH_FUNC patch_osif_os_buffer_get_intern;
extern BOOL_PATCH_FUNC patch_osif_os_buffer_put_intern;

extern BOOL_PATCH_FUNC patch_osif_os_queue_in;
extern BOOL_PATCH_FUNC patch_osif_os_queue_out;
extern BOOL_PATCH_FUNC patch_osif_os_queue_insert;
extern BOOL_PATCH_FUNC patch_osif_os_queue_delete;

extern BOOL_PATCH_FUNC patch_osif_os_timer_id_get;
extern BOOL_PATCH_FUNC patch_osif_os_timer_create;
extern BOOL_PATCH_FUNC patch_osif_os_timer_start;
extern BOOL_PATCH_FUNC patch_osif_os_timer_restart;
extern BOOL_PATCH_FUNC patch_osif_os_timer_stop;
extern BOOL_PATCH_FUNC patch_osif_os_timer_delete;
extern BOOL_PATCH_FUNC patch_osif_os_timer_dump;
extern BOOL_PATCH_FUNC patch_osif_os_timer_state_get;
extern BOOL_PATCH_FUNC patch_osif_os_timer_init;
extern BOOL_PATCH_FUNC patch_osif_os_timer_number_get;
extern BOOL_PATCH_FUNC patch_osif_os_timer_pendcall;
extern BOOL_PATCH_FUNC patch_osif_os_timer_next_timeout_value_get;


extern BOOL_PATCH_FUNC patch_vApplicationMallocFailedHook;
extern uint32_t TopOfIdleTaskStack;
extern PingpongBuffer *pMCU_PPB;
extern void LogUartDMAIdleHook(void);
static inline bool PPB_IsOutputBufEmpty(PingpongBuffer *pPPB)
{
    return (pPPB->OutputBufferSize == 0);
}

static inline bool PPB_IsInputBufEmpty(PingpongBuffer *pPPB)
{
    return (pPPB->InputBufferSize == 0);
}

/* OSIF patch function added by SDK*/
extern BOOL_PATCH_FUNC patch_osif_os_task_name_get;

#ifdef __cplusplus
}
#endif

#endif /* _PATCH_OS_H_ */
