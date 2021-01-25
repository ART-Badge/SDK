#include <k_api.h>
#include "dlps.h"
#include <los_api.h>
#include <trace.h>
#include "rtl876x_nvic.h"
#include "rtl876x_rtc.h"
#include "os_msg.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_sync.h"
#include "os_sched.h"
#include "os_mem.h"
#include "otp.h"
#include "rtl876x_wdg.h"

#include "patch_os.h"
#include "test_os.h"
#include <rtthread.h>

void *test_task_os1;
void *test_task_os2;
void *test_timer_os;
void *test_queue;
void *test_sem_sync;
void *test_sem_mutex;
void *test_mutex;

uint32_t k = 0;
uint32_t k1 = 0, k2 = 0;
uint32_t task2_suspend_count = 0;
uint32_t queue_num;
uint64_t prev_task1_tick, prev_task2_tick;
uint64_t cur_task1_tick, cur_task2_tick;

void *task2_handle = NULL;
uint16_t task1_priority;
uint16_t task2_priority;
uint32_t test1_recv = 0;

uint32_t timer_id = 0;
uint32_t timer_num;
uint32_t timer_status;
uint32_t timer_state;
uint64_t prev_timer_tick;
uint64_t cur_timer_tick;

uint32_t *array_malloc;
uint32_t prev_mem_size;
uint32_t cur_mem_size;

#define Test_Timer_flag     1

#if (TEST_OS == 1 && USE_OS  == 0)
bool os_sched_start_freertos(bool *p_result)
{

    void test_function(void);
    test_function();

    *p_result = false;

    DBG_DIRECT("USE FreeRTOS");

    return false;
}

void osif_update_for_freertos()
{
    patch_osif_os_sched_start = os_sched_start_freertos;
}

USER_CALL_BACK os_patch = osif_update_for_freertos;
#endif

void test_pend_function(uint32_t pend_para1)
{
    if (pend_para1 == 9)
    {
        DBG_DIRECT("Timer pend success");
    }
    else
    {
        DBG_DIRECT("Timer pend error, para1 = %d", pend_para1);
    }
}

#if (TEST_DLPS == 1)
void test_timer_change(void *timer, void *args)
{
    k++;
    cur_timer_tick = os_sys_tick_get();
    DBG_DIRECT("#list: timer count = %d, tick = %lld", k, cur_timer_tick);

    if (k == 4)
    {
        if ((cur_timer_tick - prev_timer_tick) != 600)
        {
            DBG_DIRECT("#list: DLPS Timer tick error, %d", cur_timer_tick - prev_timer_tick);
        }
        else
        {
            DBG_DIRECT("#list: DLPS Timer tick success");
        }
    }

    prev_timer_tick = cur_timer_tick;
}

void test1(void *arg)
{
    while (true)
    {
        cur_task1_tick = os_sys_tick_get();
        DBG_DIRECT("#list: Task1 tick = %lld", cur_task1_tick);

        if (k == 2)
        {
            if ((cur_task1_tick - prev_task1_tick) != 200)
            {
                DBG_DIRECT("#list: DLPS Task1 tick error, %d", cur_task1_tick - prev_task1_tick);
            }
            else
            {
                DBG_DIRECT("#list: DLPS Task1 tick success");
            }
        }
        prev_task1_tick = cur_task1_tick;
        os_delay(2 * 1000);
    }
}

void test2(void *arg)
{
    while (true)
    {
        cur_task2_tick = os_sys_tick_get();
        DBG_DIRECT("#list: Task2 tick = %lld", cur_task2_tick);

        if (k == 3)
        {
            if ((cur_task2_tick - prev_task2_tick) != 300)
            {
                DBG_DIRECT("#list: DLPS Task2 tick error, %d", cur_task2_tick - prev_task2_tick);
            }
            else
            {
                DBG_DIRECT("#list: DLPS Task2 tick success");
            }
        }
        prev_task2_tick = cur_task2_tick;
        os_delay(3 * 1000);
    }
}

#else

#if Test_Timer_flag
void test_timer_change(void *timer, void *args)
{
    k++;
    cur_timer_tick = os_sys_tick_get();

    DBG_DIRECT("timer count = %d, tick = %lld", k, cur_timer_tick);

    if (k == 3)
    {
        os_timer_id_get(&test_timer_os, &timer_id);
        if (timer_id != 10)
        {
            DBG_DIRECT("Timer id1 error, %d", timer_id);
        }
        else
        {
            DBG_DIRECT("Timer id1 success");
        }
    }

    if (k == 4)
    {
        os_timer_state_get(&test_timer_os, &timer_status);
        if (timer_status != 1)
        {
            DBG_DIRECT("Timer state error, %d", timer_status);
        }
        else
        {
            DBG_DIRECT("Timer state success");
        }
    }

    if (k == 5)
    {
        os_timer_number_get(&test_timer_os, &timer_num);
        if (timer_num != 3)
        {
            DBG_DIRECT("Timer number1 error, %d", timer_num);
        }
        else
        {
            DBG_DIRECT("Timer number1 success");
        }
    }

    if (k == 8)
    {
        if ((cur_timer_tick - prev_timer_tick) != 600)
        {
            DBG_DIRECT("#list: Timer tick1 error, %d", cur_timer_tick - prev_timer_tick);
        }
        else
        {
            DBG_DIRECT("#list: Timer tick1 success");
        }
        os_timer_restart(&test_timer_os, 2 * 1000);
    }

    if (k == 10)
    {
        if ((cur_timer_tick - prev_timer_tick) != 200)
        {
            DBG_DIRECT("#list: Timer tick2 error, %d", cur_timer_tick - prev_timer_tick);
        }
        else
        {
            DBG_DIRECT("#list: Timer tick2 success");
        }
    }

    if (k == 11)
    {
        os_timer_id_get(&test_timer_os, &timer_id);
        if (timer_id != 10)
        {
            DBG_DIRECT("Timer id2 error, %d", timer_id);
        }
        else
        {
            DBG_DIRECT("Timer id2 success");
        }
    }

    if (k == 12)
    {
        os_timer_number_get(&test_timer_os, &timer_num);
        if (timer_num != 3)
        {
            DBG_DIRECT("Timer number2 error, %d", timer_num);
        }
        else
        {
            DBG_DIRECT("Timer number2 success");
        }
    }

    prev_timer_tick = cur_timer_tick;
    DBG_DIRECT("#list: record perv_timer_change time");

    os_timer_start(&test_timer_os);
}
#else
void test_timer_change(void *timer, void *args)
{
    k++;
    cur_timer_tick = os_sys_tick_get();

    DBG_DIRECT("timer count = %d, tick = %lld", k, cur_timer_tick);

    if (k == 3)
    {
        os_timer_id_get(&test_timer_os, &timer_id);
        if (timer_id != 10)
        {
            DBG_DIRECT("Timer id1 error, %d", timer_id);
        }
        else
        {
            DBG_DIRECT("Timer id1 success");
        }
    }

    if (k == 4)
    {
        os_timer_state_get(&test_timer_os, &timer_status);
        if (timer_status != 1)
        {
            DBG_DIRECT("Timer state error, %d", timer_status);
        }
        else
        {
            DBG_DIRECT("Timer state success");
        }
    }

    if (k == 5)
    {
        os_timer_number_get(&test_timer_os, &timer_num);
        if (timer_num != 3)
        {
            DBG_DIRECT("Timer number1 error, %d", timer_num);
        }
        else
        {
            DBG_DIRECT("Timer number1 success");
        }
    }

    if (k == 8)
    {
        if ((cur_timer_tick - prev_timer_tick) != 600)
        {
            DBG_DIRECT("#list: Timer tick1 error, %d", cur_timer_tick - prev_timer_tick);
        }
        else
        {
            DBG_DIRECT("#list: Timer tick1 success");
        }
        os_timer_restart(&test_timer_os, 2 * 1000);
    }

    if (k == 10)
    {
        if ((cur_timer_tick - prev_timer_tick) != 200)
        {
            DBG_DIRECT("#list: Timer tick2 error, %d", cur_timer_tick - prev_timer_tick);
        }
        else
        {
            DBG_DIRECT("#list: Timer tick2 success");
        }
    }

    if (k == 11)
    {
        os_timer_id_get(&test_timer_os, &timer_id);
        if (timer_id != 10)
        {
            DBG_DIRECT("Timer id2 error, %d", timer_id);
        }
        else
        {
            DBG_DIRECT("Timer id2 success");
        }
    }

    if (k == 12)
    {
        os_timer_number_get(&test_timer_os, &timer_num);
        if (timer_num != 3)
        {
            DBG_DIRECT("Timer number2 error, %d", timer_num);
        }
        else
        {
            DBG_DIRECT("Timer number2 success");
        }
    }

    prev_timer_tick = cur_timer_tick;
    DBG_DIRECT("#list: record perv_timer_change time");
}
#endif

void test1(void *arg)
{
    while (true)
    {
        switch (k)
        {
        case 1:
            if (k1 == 0)
            {
                os_task_priority_set(test_task_os2, 3);
                k1++;
            }
            os_delay(2 * 1000);
            break;
        case 2:
            if (k1 == 1)
            {
                os_task_suspend(test_task_os1);
                k1 ++;

                if (task2_suspend_count >= 3)
                {
                    DBG_DIRECT("Task1 resume success");
                }
                else
                {
                    DBG_DIRECT("Task1 resume error, %d", task2_suspend_count);
                }
            }
            break;
        case 3:
            os_msg_recv(test_queue, &test1_recv, 0xFFFFFFFF);
//            os_msg_send(test_queue, &k2, 0xFFFFFFFF);
            break;
        case 4:
//            DBG_DIRECT("TASK1 K1 = %d", k1);
            if (k1 == 2)
            {
                k1++;
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
            }

            os_delay(2 * 1000);
            break;
        case 5:
            if (true == os_sem_take(test_sem_sync, 0xFFFFFFFF))
            {
                DBG_DIRECT("Task1 sem sync success");
            }
            break;
        case 6:
            if (k1 == 3)
            {
                k1++;
                if (test_sem_mutex != NULL)
                {
                    os_sem_take(test_sem_mutex, 0xFFFFFFFF);
                }
                os_delay(1 * 1000);
                DBG_DIRECT("#Order:Task1 SEM Mutex get");
                os_sem_give(test_sem_mutex);
                os_delay(1 * 1000);
            }
            break;
        case 7:
            if (k1 == 4)
            {
                k1++;
                if (test_mutex != NULL)
                {
                    os_mutex_take(test_mutex, 0xFFFFFFFF);
                }
                os_delay(1 * 1000);
                DBG_DIRECT("#Order:Task1 Mutex get");
                os_mutex_give(test_mutex);
                os_delay(1 * 1000);
            }
            break;
        case 9:
            if (k1 == 5)
            {
                k1++;
                os_timer_pendcall((PendedFunctionOS_t)test_pend_function, 9, 0);
            }
            break;
        case 13:
            if (k1 == 6)
            {
                k1++;
//                DBG_DIRECT("data %d, buf %d", os_mem_peek(RAM_TYPE_DATA_ON), os_mem_peek(RAM_TYPE_BUFFER_ON));
                prev_mem_size = os_mem_peek(RAM_TYPE_DATA_ON) + os_mem_peek(RAM_TYPE_BUFFER_ON);
                array_malloc = (uint32_t *)os_mem_alloc(RAM_TYPE_DATA_ON, 20);
//                DBG_DIRECT("data %d, buf %d", os_mem_peek(RAM_TYPE_DATA_ON), os_mem_peek(RAM_TYPE_BUFFER_ON));
                cur_mem_size = os_mem_peek(RAM_TYPE_DATA_ON) + os_mem_peek(RAM_TYPE_BUFFER_ON);
                if ((prev_mem_size - cur_mem_size) < 20)
                {
                    DBG_DIRECT("memory malloc peek error, prev = %d, cur = %d", prev_mem_size, cur_mem_size);
                }
                else
                {
                    DBG_DIRECT("memory malloc peek success");
                }
                os_mem_free(array_malloc);
//                DBG_DIRECT("data %d, buf %d", os_mem_peek(RAM_TYPE_DATA_ON), os_mem_peek(RAM_TYPE_BUFFER_ON));
                cur_mem_size = os_mem_peek(RAM_TYPE_DATA_ON) + os_mem_peek(RAM_TYPE_BUFFER_ON);
                if (prev_mem_size != cur_mem_size)
                {
                    DBG_DIRECT("memory free peek error, prev = %d, cur = %d", prev_mem_size, cur_mem_size);
                }
                else
                {
                    DBG_DIRECT("memory free peek success");
                }
            }
        }
    }
}

void test2(void *arg)
{
    while (true)
    {
        switch (k)
        {
        case 1:
            if (k2 == 0)
            {
                k2++;
                os_task_handle_get(&task2_handle);
                if (task2_handle != test_task_os2)
                {
                    DBG_DIRECT("Task handle get error");
                }
                else
                {
                    DBG_DIRECT("Task handle get success");
                }
                os_task_priority_get(task2_handle, &task2_priority);
                if (task2_priority != 3)
                {
                    DBG_DIRECT("Task priority set error");
                }
                else
                {
                    DBG_DIRECT("Task priority set success");
                }
                os_task_priority_set(test_task_os2, 1);
            }
            os_delay(3 * 1000);
            break;
        case 2:
            task2_suspend_count++;
            os_delay(1 * 1000);
            if (task2_suspend_count == 3)
            {
                os_task_resume(test_task_os1);
            }
            break;
        case 3:
            if (k2 == 1)
            {
                k2++;

                os_msg_queue_peek(test_queue, &queue_num);
                if (queue_num != 0)
                {
                    DBG_DIRECT("queue peek error %d", queue_num);
                }
                else
                {
                    DBG_DIRECT("queue peek success");
                }

//                os_msg_recv(test_queue, &test1_recv, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);
                os_msg_send(test_queue, &k2, 0);

                os_msg_queue_peek(test_queue, &queue_num);
                if (queue_num != 0)
                {
                    DBG_DIRECT("queue recv error %d", queue_num);
                }
                else
                {
                    DBG_DIRECT("queue recv success");
                }

            }
            break;
        case 4:
            if (k2 == 2)
            {
                k2++;
                os_msg_send(test_queue, &k2, 0);
                os_msg_queue_peek(test_queue, &queue_num);
                if (queue_num != 5)
                {
                    DBG_DIRECT("queue send and peek error %d", queue_num);
                }
                else
                {
                    DBG_DIRECT("queue send and peek success");
                }
                os_msg_recv(test_queue, &test1_recv, 0xFFFFFFFF);
                os_msg_recv(test_queue, &test1_recv, 0xFFFFFFFF);
                os_msg_recv(test_queue, &test1_recv, 0xFFFFFFFF);
                os_msg_recv(test_queue, &test1_recv, 0xFFFFFFFF);
                os_msg_recv(test_queue, &test1_recv, 0xFFFFFFFF);
                os_msg_queue_peek(test_queue, &queue_num);
                if (queue_num != 0)
                {
                    DBG_DIRECT("queue recv and peek error %d", queue_num);
                }
                else
                {
                    DBG_DIRECT("queue recv and peek success");
                }
            }
            break;
        case 5:
            if (k2 == 3)
            {
                k2++;
                os_sem_give(test_sem_sync);
            }
            break;
        case 6:
            if (k2 == 4)
            {
                k2++;
                os_sem_give(test_sem_sync);
                if (test_sem_mutex != NULL)
                {
                    os_sem_take(test_sem_mutex, 0xFFFFFFFF);
                }
                DBG_DIRECT("#Order:Task2 SEM Mutex get");
                os_sem_give(test_sem_mutex);
            }
            break;
        case 7:
            if (k2 == 5)
            {
                k2++;
                if (test_mutex != NULL)
                {
                    os_mutex_take(test_mutex, 0xFFFFFFFF);
                }
                DBG_DIRECT("#Order:Task2 Mutex get");
                os_mutex_give(test_mutex);
            }
            break;
        }
    }
}
#endif

#if 0
void test_timer_callback(void *timer_handle)
{
    bool ret;
    k++;
    cur_timer_tick = os_sys_tick_get();
    DBG_DIRECT("timer count = %d, tick = %lld", k, cur_timer_tick);

    ret = os_timer_number_get(&timer_handle, &timer_num);
    if (ret == false)
    {
        DBG_DIRECT("timer delete error");
    }
    else
    {
        DBG_DIRECT("timer delete success");
    }
    DBG_DIRECT("TIMER num %d", timer_num);
}
void test_function(void)
{
    bool ret;
    ret = os_timer_create(&test_timer_os, "test_time", 10, 1 * 1000, false, test_timer_callback);
    if (ret == false)
    {
        DBG_DIRECT("timer create error");
    }
    else
    {
        DBG_DIRECT("timer create success, timer handle = %x", test_timer_os);
    }

    ret = os_timer_start(&test_timer_os);
    if (ret == false)
    {
        DBG_DIRECT("timer start error");
    }
    else
    {
        DBG_DIRECT("timer start success");
    }
}
#else
void test_function(void)
{
    bool ret;

#if Test_Timer_flag
    ret = os_timer_create(&test_timer_os, "test_time", 10, 6 * 1000, false, test_timer_change);
#else
    ret = os_timer_create(&test_timer_os, "test_time", 10, 6 * 1000, true, test_timer_change);
#endif

    if (ret == false)
    {
        DBG_DIRECT("timer create error");
    }
    else
    {
        DBG_DIRECT("timer create success, timer handle = %x", test_timer_os);
    }

    ret = os_timer_start(&test_timer_os);
    if (ret == false)
    {
        DBG_DIRECT("timer start error");
    }
    else
    {
        DBG_DIRECT("timer start success");
    }

//    extern void rt_timer_dump(rt_list_t timer_heads[]);
//    extern rt_list_t rt_soft_timer_list[RT_TIMER_SKIP_LIST_LEVEL];
//    extern struct rt_thread timer_thread;
//    rt_timer_dump(rt_soft_timer_list);


    ret = os_task_create(&test_task_os1, "write_task1", test1, NULL, 400, 2);
    if (ret == false)
    {
        DBG_DIRECT("task create error");
    }
    else
    {
        DBG_DIRECT("task create success, task handle = %x", test_task_os1);
    }

    ret = os_task_create(&test_task_os2, "write_task2", test2, NULL, 400, 1);
    if (ret == false)
    {
        DBG_DIRECT("task create error");
    }
    else
    {
        DBG_DIRECT("task create success, task handle = %x", test_task_os2);
    }

    ret = os_msg_queue_create(&test_queue, 5, sizeof(uint32_t));
    if (ret == false)
    {
        DBG_DIRECT("queue create error");
    }
    else
    {
        DBG_DIRECT("queue create success, queue handle = %x", test_queue);
    }

    ret = os_sem_create(&test_sem_sync, 0, 1);
    if (ret == false)
    {
        DBG_DIRECT("sem sync create error");
    }
    else
    {
        DBG_DIRECT("sem sync create success, sem handle = %x", test_sem_sync);
    }

    ret = os_sem_create(&test_sem_mutex, 1, 1);
    if (ret == false)
    {
        DBG_DIRECT("sem mutex create error");
    }
    else
    {
        DBG_DIRECT("sem mutex create success, sem handle = %x", test_sem_mutex);
    }

    ret = os_mutex_create(&test_mutex);
    if (ret == false)
    {
        DBG_DIRECT("mutex create error");
    }
    else
    {
        DBG_DIRECT("mutex create success, mutex handle = %x", test_mutex);
    }

#if (TEST_DLPS != 1)
    lps_mode_set(LPM_ACTIVE_MODE);
#endif
}
#endif
