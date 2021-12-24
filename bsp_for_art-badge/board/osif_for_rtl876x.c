#include <rtthread.h>
#include <stdio.h>
#include <stdint.h>
#include "patch_os.h"
#include "mem_types.h"
#include "os_queue.h"
#include "os_timer.h"
#include "system_rtl876x.h"
#include "rtl876x.h"
#include <rtthread.h>
#include "app_section.h"
#include "osif_for_rtl876x.h"
#include "trace.h"
#include "rtl876x_lib_platform.h"
#include "core_cm4.h"
#include "rtl876x_wdg.h"
#include "flash_device.h"
#include "psram_platform.h"



#define portTICK_PERIOD_MS (1000 / RT_TICK_PER_SECOND)

typedef void (*PendedFunctionOS_t)(uint32_t);

//rt_timer_t timer_numer_array[TIMER_NUMBER_MAX] = {0};
volatile uint32_t pendtick_RTT = 0;

extern uint32_t vector_table_level_two;
extern rt_tick_t rt_tick;
extern void PendSV_Handler(void);
extern void xPortSysTickHandler_RTT(void);
extern rt_base_t rt_hw_interrupt_disable(void);
extern void rt_hw_interrupt_enable(rt_base_t level);
extern rt_mq_t rt_timer_queue;

#define IRQ_NUM (16 + 32)  //16 exceptions + 32 interrupts
IRQ_Fun vector_table_level_one[IRQ_NUM] __attribute__((aligned(0x100)));

extern struct rt_memheap buffer_heap;
extern struct rt_memheap data_heap;

//struct rt_memheap psram_heap;



void common_interrupt_handler(void)
{
    rt_interrupt_enter();
    uint32_t v_num = __get_IPSR();

    IRQ_Fun *pRamVectorTable = (IRQ_Fun *)(vector_table_level_two);

    pRamVectorTable[v_num]();

    rt_interrupt_leave();
}

bool os_systick_handler_RTT(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();

    return true;
}

bool os_delay_RTT(uint32_t ms)
{
    rt_thread_delay((rt_tick_t)((ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS));

    return true;
}

bool os_sys_time_get_RTT(uint64_t *p_time_ms)
{
    *p_time_ms = (rt_tick_get() * portTICK_PERIOD_MS);

    return true;
}

bool os_sys_tick_get_RTT(uint64_t *p_sys_tick)
{
    *p_sys_tick = (uint64_t)rt_tick_get();

    return true;
}

DATA_RAM_FUNCTION bool os_sys_tick_increase_RTT(uint32_t tick_increment,
        rt_tick_t *p_old_tick)  // Change after
{
    rt_base_t level;
    *p_old_tick = rt_tick_get();

    rt_tick_set(rt_tick_get() + tick_increment - 1);

    level = rt_hw_interrupt_disable();
    pendtick_RTT = 1;
    rt_hw_interrupt_enable(level);
    return true;
}

void SetSystemTickInterrupt_RTT(void)
{
    /* Configure SysTick to interrupt at the requested rate. */
    portNVIC_SYSTICK_CTRL_REG = 0UL;
    portNVIC_SYSTICK_LOAD_REG = (configSYSTICK_CLOCK_HZ_RTT / RT_TICK_PER_SECOND) - 1UL;
    portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;
    portNVIC_SYSTICK_CTRL_REG = (portNVIC_SYSTICK_CLK_BIT_RTT | portNVIC_SYSTICK_INT_BIT |
                                 portNVIC_SYSTICK_ENABLE_BIT);
}


bool os_sched_start_RTT(bool *p_result)
{
    rt_base_t level = rt_hw_interrupt_disable();

    rt_tick_set(0);

    SetSystemTickInterrupt_RTT();
    /* Ensure the VFP is enabled - it should be anyway. */
    prvEnableVFP_RTT();


    rt_hw_interrupt_enable(level);

    DBG_DIRECT("USE RTT!!!");
    rt_system_scheduler_start();
    *p_result = true;

    return true;
}


DATA_RAM_FUNCTION bool os_sched_suspend_RTT(bool *p_result)
{
    rt_enter_critical();
    *p_result = true;

    return true;
}

DATA_RAM_FUNCTION bool os_sched_resume_RTT(bool *p_result)
{
    uint32_t uxPendedCounts = pendtick_RTT; /* Non-volatile copy. */
    rt_base_t level;

    rt_exit_critical();
    *p_result = true;

    if (uxPendedCounts > 0)
    {
        do
        {
            rt_tick_increase();

            --uxPendedCounts;
        }
        while (uxPendedCounts > 0);

        level = rt_hw_interrupt_disable();
        pendtick_RTT = 0;
        rt_hw_interrupt_enable(level);
    }

    return true;
}

bool os_sched_is_start_RTT(bool *p_result)
{
    if (rt_thread_self() != NULL)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

void xPortSysTickHandler_RTT(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

bool os_vector_table_update_RTT(void)
{
    IRQ_Fun *vectors = (IRQ_Fun *)vector_table_level_two;

    RamVectorTableUpdate(PendSV_VECTORn, (IRQ_Fun)PendSV_Handler);
    RamVectorTableUpdate(SysTick_VECTORn, (IRQ_Fun)xPortSysTickHandler_RTT);

    #if 1

    for (uint32_t i = 0; i < 16; ++i)
    {
        vector_table_level_one[i] = vectors[i];
    }

    for (uint32_t i = 16; i < IRQ_NUM; ++i)
    {
        vector_table_level_one[i] = common_interrupt_handler;
    }

    //skip the disasm function
    vector_table_level_one[WDG_VECTORn] = vectors[WDG_VECTORn];
    #ifndef _IS_ASIC_
    vector_table_level_one[Peripheral_VECTORn] = vectors[Peripheral_VECTORn];
    #endif

    SCB->VTOR = (uint32_t)vector_table_level_one;
    #endif

    return true;
}


#if 0
static void rt_hw_board_init(void)
{
    /* Set the clock frequency at 100MHz */
    set_system_clock(SYSTEM_90MHZ);
    #ifdef BSP_USING_PSRAM
    /* init psram */
    extern int drv_psram_init(void);
    drv_psram_init();
    #endif
    psram_init();
    psram_try_high_speed(FLASH_MODE_4BIT);
    #if defined(RT_USING_HEAP)
    /* Heap initialization */
    rt_system_heap_init((void *)(OTP->appDataAddr + OTP->appDataSize + 10 * 1024), \
                        (void *)(OTP->appDataAddr + OTP->appDataSize + OTP->heapDataONSize));
    rt_memheap_init(&buffer_heap, "bufferheap", BUFFER_RAM_ADDR, BUFFER_RAM_SIZE);
    rt_memheap_init(&data_heap, "dataheap", (void *)(OTP->appDataAddr + OTP->appDataSize), 10 * 1024);
    rt_memheap_init(&psram_heap, "psram", (void *)(0x6800000), 4 * 1024 * 1024);
    #endif

    #ifdef BSP_USING_UART
    /* USART driver initialization is open by default */
    extern int rt_hw_uart_init(void);
    rt_hw_uart_init();
    #endif

    #ifdef RT_USING_CONSOLE
    /* Set the shell console output device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
    #endif

    #ifdef RT_USING_COMPONENTS_INIT
    /* Board underlying hardware initialization */
    rt_components_board_init();
    #endif
}
#endif

void RTT_idle_hook(void)
{
    TopOfIdleTaskStack = (uint32_t)rt_thread_self()->stack_addr + \
                         (uint32_t)rt_thread_self()->stack_size + \
                         sizeof(rt_uint32_t) - \
                         sizeof(rt_ubase_t);
    TopOfIdleTaskStack  = RT_ALIGN_DOWN((rt_uint32_t)TopOfIdleTaskStack, 8);

    //rt_kprintf("TopOfIdleTaskStack = 0x%x", TopOfIdleTaskStack);
    if (flash_get_flash_exist() && (rt_critical_level() == 0))
    {
        extern void ftl_garbage_collect_in_idle(void);
        ftl_garbage_collect_in_idle();
    }

    if ((OTP->logDisable == 1) ||
            (!OTP->printAllLogBeforeEnterDLPS) ||
            (OTP->printAllLogBeforeEnterDLPS &&
             PPB_IsOutputBufEmpty(pMCU_PPB) && PPB_IsInputBufEmpty(pMCU_PPB)))
    {
        void dlps_processing(void);
        dlps_processing();
    }

    LogUartDMAIdleHook();

    if (OTP->wdgResetInRom)
    {
        WDG_Restart();
    }
}

bool RTT_init()
{
    log_module_trace_set(MODULE_UPPERSTACK,  LEVEL_TRACE, false);
    log_module_trace_set(MODULE_UPPERSTACK,  LEVEL_INFO, false);

    rt_hw_interrupt_disable();

    rt_hw_board_init();

    rt_show_version();

    /* timer system initialization */
    rt_system_timer_init();

    /* scheduler system initialization */
    rt_system_scheduler_init();

    #ifdef RT_USING_SIGNALS
    /* signal system initialization */
    rt_system_signal_init();
    #endif

    /* timer thread initialization */
    rt_system_timer_thread_init();

    /* idle thread initialization */
    //  rt_thread_idle_sethook(RTT_idle_hook);
    rt_thread_idle_init();


    return true;
}

bool os_init_RTT(void)
{
    bool xReturn;
    xReturn = RTT_init();

    if (xReturn != true)
    {
        while (1);  //todo
    }

    return true;
}

bool os_task_create_RTT(void **pp_handle, const char *p_name, void (*p_routine)(void *),
                        void *p_param, uint16_t stack_size, uint16_t priority, bool *p_is_create_success)
{
    rt_thread_t ret;
    *p_is_create_success = false;

    if (pp_handle)
    {
        #ifndef BSP_USING_BLE

        if(!rt_memcmp(p_name, "low_", 4))
        {
            DBG_DIRECT("not start the low statck");
            *p_is_create_success = true;
            return true;
        }

        if(!rt_memcmp(p_name, "Uppe", 4))
        {
            DBG_DIRECT("not start the upper statck");
            *p_is_create_success = true;
            return true;
        }

        #endif
        uint16_t switch_priority = RT_THREAD_PRIORITY_MAX - 1 - priority;
        ret = rt_thread_create((const char *)p_name, p_routine, p_param, stack_size, switch_priority, 2);

        if (ret != RT_NULL)
        {
            *p_is_create_success = true;
            *pp_handle = ret;

            rt_thread_startup(ret);
        }
    }

    return true;
}

bool os_task_delete_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_thread_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_thread_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    ret = rt_thread_delete(obj);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_task_suspend_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_thread_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_thread_t)p_handle;
    ret = rt_thread_suspend(obj);

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    rt_schedule();

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_task_resume_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_thread_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_thread_t)p_handle;
    ret = rt_thread_resume(obj);

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    rt_schedule();

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_task_yield_RTT(bool *p_result)
{
    rt_err_t ret;

    ret = rt_thread_yield();

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_task_handle_get_RTT(void **pp_handle, bool *p_result)
{
    if (pp_handle == NULL)
    {
        *p_result = false;
        return true;
    }

    *pp_handle = (void *)rt_thread_self();
    *p_result = true;

    return true;
}

bool os_task_name_get_RTT(void *p_handle, char **p_task_name, bool *p_result)
{
    rt_thread_t obj;

    if (p_handle != NULL)
    {
        obj = (rt_thread_t)p_handle;
    }
    else
    {
        obj = rt_thread_self();
    }

    *p_task_name = obj->name;

    *p_result = true;

    return true;
}

bool os_task_priority_get_RTT(void *p_handle, uint16_t *p_priority, bool *p_result)
{
    if (p_priority == NULL)
    {
        *p_result = false;
        return true;
    }

    uint16_t priority;
    rt_base_t lock;
    rt_thread_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    if (p_handle != NULL)
    {
        obj = (rt_thread_t)p_handle;
    }
    else
    {
        obj = rt_thread_self();
    }

    priority = obj->current_priority ;
    *p_priority = RT_THREAD_PRIORITY_MAX - 1 - priority;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    *p_result = true;

    return true;
}

bool os_task_priority_set_RTT(void *p_handle, uint16_t priority, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_thread_t obj;
    uint16_t switch_priority = RT_THREAD_PRIORITY_MAX - 1 - priority;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    if (p_handle != NULL)
    {
        obj = (rt_thread_t)p_handle;
    }
    else
    {
        obj = rt_thread_self();
    }

    ret = rt_thread_control(obj, RT_THREAD_CTRL_CHANGE_PRIORITY, &switch_priority);

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    rt_schedule();

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

void vTaskStatusDump_RTT(void)
{
    rt_thread_t cur_thread = rt_thread_self();
    DBG_DIRECT("pxCurrentTCB %p, pxTopOfStack %p,taks_pri:%d",
               cur_thread, cur_thread->sp, RT_THREAD_PRIORITY_MAX - 1 - cur_thread->current_priority);
}

bool os_task_status_dump_RTT(void)
{
    extern void vTaskStatusDump_RTT(void);
    vTaskStatusDump_RTT();

    return true;
}



DATA_RAM_FUNCTION __asm void prvEnableVFP_RTT(void)
{
    PRESERVE8

    /* The FPU enable bits are in the CPACR. */
    ldr.w r0, = 0xE000ED88
                ldr r1, [r0]

                /* Enable CP10 and CP11 coprocessors, then save back. */
                //              orr r1, r1, #( 0xf << 20 )
                orr r1, r1, #0xf00000
                str r1, [r0]
                bx  r14
                nop
}

DATA_RAM_FUNCTION void xPortStartSchedulerInRestore_RTT(void)
{
    /* Make PendSV and SysTick the lowest priority interrupts. */
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;

    /* Start the timer that generates the tick ISR.  Interrupts are disabled
    here already. */
    //vPortSetupTimerInterrupt(); // move to lps_handle_sm_intr()

    /* Ensure the VFP is enabled - it should be anyway. */
    prvEnableVFP_RTT();

    /* Lazy save always. */
    *(portFPCCR) |= portASPEN_AND_LSPEN_BITS;

    __DSB();
    __ISB();
}

/* *INDENT-OFF* */
DATA_RAM_FUNCTION __asm void DLPS_ReturnIdleTask_RTT()
{
//    extern xPortStartSchedulerInRestore_RTT
//    extern rt_thread_idle_entry
//    extern TopOfIdleTaskStack
//    PRESERVE8

//    bl  xPortStartSchedulerInRestore_RTT

//    /* Use the NVIC offset register to locate the stack. */
//    ldr r0, =0xE000ED08
//    ldr r0, [r0]
//    ldr r0, [r0]
//    /* Set the msp back to the start of the stack. */
//    msr msp, r0
//    isb

//    /* Update psp of the idle task */
//    ldr r0, =TopOfIdleTaskStack
//    ldr r0, [r0]
//    msr psp, r0
//    isb

//    /* Switch to the psp stack. */
//    mrs r0, CONTROL
//    orr  r0, r0, #2
//    msr CONTROL, r0
//    isb

//    /* Jump to the entry of the idle task */
//    ldr r0, =rt_thread_idle_entry
//    bx r0

//LOOP_NOP
//    nop
//    B LOOP_NOP
//    ALIGN
}

bool os_task_dlps_return_idle_task_RTT(void)   //change after
{
    //DBG_DIRECT("EXIT LINE = %d",__LINE__);
    extern void DLPS_ReturnIdleTask_RTT(void);
    DLPS_ReturnIdleTask_RTT();

    return true;
}

bool os_lock_RTT(uint32_t *p_flags)
{
    rt_base_t lock;

    lock = rt_hw_interrupt_disable();

    *p_flags = lock;

    return true;
}

bool os_unlock_RTT(uint32_t flags)
{
    rt_base_t lock = flags;

    rt_hw_interrupt_enable(lock);

    return true;
}

bool os_sem_create_RTT(void **pp_handle, uint32_t init_count, uint32_t max_count, bool *p_result)
{
    rt_sem_t ret;

    *p_result = false;

    if (pp_handle)
    {
        ret = rt_sem_create("sem", init_count, RT_IPC_FLAG_FIFO);

        if (ret != RT_NULL)
        {
            *p_result = true;
            *pp_handle = (void *)ret;
        }
        else
        {
            *p_result = false;
        }
    }

    return true;
}

bool os_sem_delete_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_sem_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_sem_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    ret = rt_sem_delete(obj);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_sem_take_RTT(void *p_handle, uint32_t wait_ms, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_sem_t obj;
    rt_tick_t wait_ticks;

    if (p_handle == NULL)
    {
        *p_result = false;
        return true;
    }

    if (rt_interrupt_get_nest() != 0)
    {
        *p_result = false;
        return true;
    }

//    if (rt_critical_level() != 0)
//    {
//        *p_result = false;
//        return true;
//    }

    bool result;
    os_sched_is_start_RTT(&result);
    if (result == false)
    {
        *p_result = true;
        return true;
    }

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_sem_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    if (wait_ms == 0xFFFFFFFFUL)
    {
        ret = rt_sem_take(obj, RT_WAITING_FOREVER);
    }
    else
    {
        wait_ticks = (rt_tick_t)((wait_ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS);
        ret = rt_sem_take(obj, wait_ticks);
    }

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_sem_give_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_sem_t obj;

    if (p_handle == NULL)
    {
        *p_result = false;
        return true;
    }

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_sem_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    ret = rt_sem_release(obj);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_mutex_create_RTT(void **pp_handle, bool *p_result)
{
    rt_mutex_t ret;
    *p_result = false;

    if (pp_handle)
    {
        ret = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);

        if (ret != RT_NULL)
        {
            *p_result = true;
            *pp_handle = (void *)ret;
        }
    }

    return true;
}

bool os_mutex_delete_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_mutex_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_mutex_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    ret = rt_mutex_delete(obj);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_mutex_take_RTT(void *p_handle, uint32_t wait_ms, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_mutex_t obj;
    rt_tick_t wait_ticks;

    if (p_handle == NULL)
    {
        *p_result = false;
        return true;
    }

    if (rt_interrupt_get_nest() != 0)
    {
        *p_result = false;
        return true;
    }

    bool result;
    os_sched_is_start_RTT(&result);
    if (result == false)
    {
        *p_result = true;
        return true;
    }

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_mutex_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    if (wait_ms == 0xFFFFFFFFUL)
    {
        ret = rt_mutex_take(obj, RT_WAITING_FOREVER);
    }
    else
    {
        wait_ticks = (rt_tick_t)((wait_ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS);
        ret = rt_mutex_take(obj, wait_ticks);
    }

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_mutex_give_RTT(void *p_handle, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_mutex_t obj;

    if (p_handle == NULL)
    {
        *p_result = false;
        return true;
    }

    if (rt_interrupt_get_nest() != 0)
    {
        *p_result = false;
        return true;
    }

    bool result;
    os_sched_is_start_RTT(&result);
    if (result == false)
    {
        *p_result = true;
        return true;
    }

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_mutex_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    ret = rt_mutex_release(obj);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_msg_queue_create_intern_RTT(void **pp_handle, uint32_t msg_num, uint32_t msg_size,
                                    const char *p_func, uint32_t file_line, bool *p_result)
{
    rt_mq_t ret;
    *p_result = false;

    if (pp_handle)
    {
        ret = rt_mq_create("queue", msg_size, msg_num, RT_IPC_FLAG_FIFO);

        if (ret != RT_NULL)
        {
            *p_result = true;
            *pp_handle = (void *)ret;
        }
        else
        {
            *p_result = false;
        }
    }

    return true;
}

bool os_msg_queue_delete_intern_RTT(void *p_handle, const char *p_func, uint32_t file_line,
                                    bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_mq_t obj;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_mq_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    ret = rt_mq_delete(obj);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_msg_queue_peek_intern_RTT(void *p_handle, uint32_t *p_msg_num,
                                  const char *p_func, uint32_t file_line, bool *p_result)
{
    rt_mq_t obj;
    rt_base_t lock;

    if (p_handle)
    {
        /* disable interrupt */
        lock = rt_hw_interrupt_disable();

        obj = (rt_mq_t)p_handle;
        *p_msg_num = obj->entry;

        /* enable interrupt */
        rt_hw_interrupt_enable(lock);

        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_msg_send_intern_RTT(void *p_handle, void *p_msg, uint32_t wait_ms,
                            const char *p_func, uint32_t file_line, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_mq_t obj;
    rt_tick_t wait_ticks;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_mq_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    if (wait_ms == 0xFFFFFFFFUL)
    {
        ret = rt_mq_send_wait(obj, p_msg, obj->msg_size, RT_WAITING_FOREVER);
    }
    else
    {
        wait_ticks = (rt_tick_t)((wait_ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS);
        ret = rt_mq_send_wait(obj, p_msg, obj->msg_size, wait_ticks);
    }


    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}

bool os_msg_recv_intern_RTT(void *p_handle, void *p_msg, uint32_t wait_ms,
                            const char *p_func, uint32_t file_line, bool *p_result)
{
    rt_err_t ret;
    rt_base_t lock;
    rt_mq_t obj;
    rt_tick_t wait_ticks;

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    obj = (rt_mq_t)p_handle;

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    if (wait_ms == 0xFFFFFFFFUL)
    {
        ret = rt_mq_recv(obj, p_msg, obj->msg_size, RT_WAITING_FOREVER);
    }
    else
    {
        wait_ticks = (rt_tick_t)((wait_ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS);
        ret = rt_mq_recv(obj, p_msg, obj->msg_size, wait_ticks);
    }

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }

    return true;
}


bool os_mem_peek_RTT(RAM_TYPE ram_type, size_t *p_size)
{

    if (ram_type == 0)
    {
        *p_size =  data_heap.available_size;
    }
    else if (ram_type == 1)
    {
        *p_size =  buffer_heap.available_size;
    }

    return true;
}

bool os_mem_check_heap_usage_RTT(void)
{
    return true;
}

bool os_mem_alloc_intern_RTT(RAM_TYPE ram_type, size_t size,
                             const char *p_func, uint32_t file_line, void **pp)
{
    size_t unused_size;
    os_mem_peek_RTT(ram_type, &unused_size);

    if (OTP->run_in_app == 1)
    {
        *pp = rt_malloc(size);
    }
    else
    {
        if (ram_type == 0)
        {
            *pp = rt_memheap_alloc(&data_heap, size);
        }
        else if (ram_type == 1)
        {
            *pp = rt_memheap_alloc(&buffer_heap, size);
        }
    }

    if (*pp == NULL)
    {
        OSIF_PRINT_ERROR5("os_mem_alloc_intern: %s<%u> ram type %u, required size %u, unused size %u",
                          TRACE_STRING(p_func), file_line, ram_type, size, unused_size);
    }
    return true;
}

bool os_mem_zalloc_intern_RTT(RAM_TYPE ram_type, size_t size,
                              const char *p_func, uint32_t file_line, void **pp)
{
    size_t unused_size;
    os_mem_peek_RTT(ram_type, &unused_size);

    if (OTP->run_in_app == 1)
    {
        *pp = rt_malloc(size);
    }
    else
    {
        if (ram_type == 0)
        {
            *pp = rt_memheap_alloc(&data_heap, size);
        }
        else if (ram_type == 1)
        {
            *pp = rt_memheap_alloc(&buffer_heap, size);
        }
    }

    if (*pp)
    {
        rt_memset(*pp, 0, size);
    }

    if (*pp == NULL)
    {
        OSIF_PRINT_ERROR5("os_mem_zalloc_intern: %s<%u> ram type %u, required size %u, unused size %u",
                          TRACE_STRING(p_func), file_line, ram_type, size, unused_size);
    }

    return true;
}

bool os_mem_aligned_alloc_intern_RTT(RAM_TYPE ram_type, size_t size, uint8_t alignment,
                                     const char *p_func, uint32_t file_line, void **pp)
{
    void *p = NULL;
    void *p_aligned;

    if (alignment == 0)
    {
        alignment = RT_ALIGN_SIZE;
    }

    size_t unused_size;
    os_mem_peek_RTT(ram_type, &unused_size);

    if (OTP->run_in_app == 1)
    {
        *pp = rt_malloc(size + sizeof(void *) + alignment);
    }
    else
    {
        if (ram_type == 0)
        {
            *pp = rt_memheap_alloc(&data_heap, size + sizeof(void *) + alignment);
        }
        else if (ram_type == 1)
        {
            *pp = rt_memheap_alloc(&buffer_heap, size + sizeof(void *) + alignment);
        }
    }

    if (p == NULL)
    {
        OSIF_PRINT_ERROR5("os_mem_aligned_alloc_intern: %s<%u> ram type %u, required size %u, unused size %u",
                          TRACE_STRING(p_func), file_line, ram_type, size, unused_size);
        return true;
    }

    p_aligned = (void *)(((size_t)p + sizeof(void *) + alignment) & ~(alignment - 1));

    rt_memcpy((uint8_t *)p_aligned - sizeof(void *), &p, sizeof(void *));

    *pp = p_aligned;

    return true;
}

bool os_mem_free_RTT(void *p_block)
{
    rt_free(p_block);
    return true;
}

bool os_mem_aligned_free_RTT(void *p_block)
{
    void *p;

    rt_memcpy(&p, (uint8_t *)p_block - sizeof(void *), sizeof(void *));

    rt_free(p);

    return true;
}



struct osif_timer_list
{
    struct rt_timer *timer_handle; //save the rtt handle
    uint32_t osif_timer_id;
    uint32_t timer_number;
    rt_slist_t timer_list;
};
typedef struct osif_timer_list osif_timer_list_t;
static uint8_t timer_number_index;

static rt_slist_t _osif_timer_head = RT_SLIST_OBJECT_INIT(_osif_timer_head);

osif_timer_list_t *osif_timer_get_entry(void *p_handle)
{
    osif_timer_list_t *h_timer;

    struct rt_slist_node *node = NULL;
    rt_base_t lock;
    lock = rt_hw_interrupt_disable();
    rt_slist_for_each(node, &_osif_timer_head)
    {
        h_timer = rt_list_entry(node, struct osif_timer_list, timer_list);
        if (h_timer->timer_handle == p_handle)
        {
            rt_hw_interrupt_enable(lock);
            return h_timer;
        }
    }
    rt_hw_interrupt_enable(lock);
    DBG_DIRECT("%s, couldn't find the handle : %p ", __func__, p_handle);

    return NULL;

}

bool osif_timer_insert_entry(void *p_handle, uint32_t timer_id, bool timer_status, uint8_t timer_number)
{
    osif_timer_list_t *node_timer;
    rt_base_t lock;
    node_timer  = RT_KERNEL_MALLOC(sizeof(osif_timer_list_t));
    if (node_timer == NULL)
    {
        return false;
    }
    node_timer->timer_handle = p_handle;
    node_timer->osif_timer_id = timer_id;
    node_timer->timer_number = timer_number;

    lock = rt_hw_interrupt_disable();
    rt_slist_init(&node_timer->timer_list);
    rt_slist_append(&_osif_timer_head, &node_timer->timer_list);
    rt_hw_interrupt_enable(lock);
    return true;
}

void *osif_timer_remove_entry(void *p_handle)
{
    osif_timer_list_t *h_timer;
    struct rt_slist_node *node = NULL;
    rt_base_t lock;
    lock = rt_hw_interrupt_disable();
    rt_slist_for_each(node, &_osif_timer_head)
    {
        h_timer = rt_list_entry(node, struct osif_timer_list, timer_list);
        if (h_timer->timer_handle == p_handle)
        {
            rt_slist_remove(&_osif_timer_head, node);
            RT_KERNEL_FREE(h_timer);
            rt_hw_interrupt_enable(lock);
            return NULL;
        }
    }
    rt_hw_interrupt_enable(lock);

    DBG_DIRECT("%s, couldn't find the handle : %p ", __func__, p_handle);

    return NULL;

}


bool os_timer_id_get_RTT(void **pp_handle, uint32_t *p_timer_id, bool *p_result)
{
    *p_result = false;
    if (pp_handle == NULL || *pp_handle == NULL)
    {
        return false;
    }

    if (pp_handle && *pp_handle)
    {
        osif_timer_list_t *p_handle_timer;
        p_handle_timer = osif_timer_get_entry(*pp_handle);
        *p_timer_id = p_handle_timer->osif_timer_id;
        *p_result = true;
    }
    return true;
}



bool os_timer_create_RTT(void **pp_handle, const char *p_timer_name, uint32_t timer_id,
                         uint32_t interval_ms, bool reload, void (*p_timer_callback)(), bool *p_result)
{
    rt_timer_t ret = NULL;
    rt_tick_t timer_ticks;
    *p_result = false;

    if (pp_handle == NULL || p_timer_callback == NULL)
    {
        DBG_DIRECT("FAIL!!!! TEST %s  LINE %d, timer_id %d", __func__, __LINE__);
        return false;
    }
    timer_ticks = (rt_tick_t) interval_ms * RT_TICK_PER_SECOND / 1000;

    if (*pp_handle == NULL)
    {

        if (reload == true)
        {
            ret = rt_timer_create(p_timer_name, p_timer_callback, (void *)(*pp_handle), timer_ticks,
                                  RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
        }
        else
        {
            ret = rt_timer_create(p_timer_name, p_timer_callback, (void *)(*pp_handle), timer_ticks,
                                  RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
        }
    }
    if (ret == NULL)
    {
        DBG_DIRECT("FAIL!!!! TEST %s  LINE %d, timer_id %d", __func__, __LINE__);
        return false;
    }
    ret->parameter = ret;
    *pp_handle = ret;

    osif_timer_insert_entry(*pp_handle, timer_id, false, timer_number_index);

    timer_number_index++;
    *p_result = true;
    return true;
}

bool os_timer_start_RTT(void **pp_handle, bool *p_result)
{
    rt_err_t ret;
    *p_result = false;
    if (pp_handle == NULL || *pp_handle == NULL)
    {
        return false;
    }

    rt_timer_queue_cb cb;
    cb.timer  = (rt_timer_t) * pp_handle;
    cb.cb_num = RT_TIMER_CMD_START;
    ret = rt_mq_send(rt_timer_queue, &cb, sizeof(rt_timer_queue_cb));

    if (ret == RT_EOK)
    {
        *p_result = true;
    }


    //DBG_DIRECT("[RTT][TIMER] name = %s;LINE = %d", ((rt_timer_t )*pp_handle)->parent.name,__LINE__);
    return true;
}

bool os_timer_restart_RTT(void **pp_handle, uint32_t interval_ms, bool *p_result)
{
    rt_err_t ret;
    rt_timer_queue_cb cb;
    rt_tick_t timer_ticks;

    *p_result = false;

    timer_ticks = (rt_tick_t)((interval_ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS);

    if (pp_handle && *pp_handle && (timer_ticks > 0))
    {
        cb.timer  = (rt_timer_t) * pp_handle;
        cb.cb_num = RT_TIMER_CMD_RESTART;
        cb.change_tick = (void *)timer_ticks;
        ret = rt_mq_send(rt_timer_queue, &cb, sizeof(rt_timer_queue_cb));

        if (ret == RT_EOK)
        {
            *p_result = true;
        }
    }

    return true;
}

bool os_timer_stop_RTT(void **pp_handle, bool *p_result)
{
    rt_err_t ret;
    rt_timer_queue_cb cb;
    *p_result = false;

    if (pp_handle && *pp_handle)
    {
        cb.timer  = (rt_timer_t) * pp_handle;
        cb.cb_num = RT_TIMER_CMD_STOP;
        ret = rt_mq_send(rt_timer_queue, &cb, sizeof(rt_timer_queue_cb));

        if (ret == RT_EOK)
        {
            *p_result = true;
        }
    }

    return true;
}



bool os_timer_delete_RTT(void **pp_handle, bool *p_result)
{
    rt_err_t ret;
    if (pp_handle == NULL || *pp_handle == NULL)
    {
        DBG_DIRECT("FAIL!!! TEST %s  LINE %d ", __func__, __LINE__);
        return false;
    }

    rt_timer_queue_cb cb;
    cb.timer  = (rt_timer_t) * pp_handle;
    cb.cb_num = RT_TIMER_CMD_DEL;
    ret = rt_mq_send(rt_timer_queue, &cb, sizeof(rt_timer_queue_cb));

    if (ret == RT_EOK)
    {
        *p_result = true;
    }

    osif_timer_remove_entry(*pp_handle);
    *p_result = true;
    return true;
}

bool os_timer_dump_RTT(bool *p_result)
{
    return true;
}

bool os_timer_state_get_RTT(void **pp_handle, uint32_t *p_timer_state, bool *p_result)
{
    rt_base_t lock;
    rt_timer_t obj;

    *p_result = false;

    if (pp_handle && *pp_handle)
    {
        /* disable interrupt */
        lock = rt_hw_interrupt_disable();

        obj = (rt_timer_t)(*pp_handle);
        RT_ASSERT(rt_object_get_type(&obj->parent) == RT_Object_Class_Timer);

        if (obj->parent.flag & RT_TIMER_FLAG_ACTIVATED)
        {
            /*timer is start and run*/
            *(rt_tick_t *)p_timer_state = RT_TIMER_FLAG_ACTIVATED;
        }
        else
        {
            /*timer is stop*/
            *(rt_tick_t *)p_timer_state = RT_TIMER_FLAG_DEACTIVATED;
        }

        /* enable interrupt */
        rt_hw_interrupt_enable(lock);

        *p_result = true;
    }

    return true;
}

bool os_timer_init_RTT(void)
{
    extern rt_mq_t rt_timer_queue;
    extern rt_thread_t rt_timer_task;
    extern void timer_task(void *pa);

    rt_timer_queue = rt_mq_create("timer_queue", sizeof(rt_timer_queue_cb), TIMER_NUMBER_MAX,
                                  RT_IPC_FLAG_FIFO);
    rt_timer_task = rt_thread_create("timer_task_RTT", timer_task, NULL, 512, 0, 2);

    if (rt_timer_task != RT_NULL)
    {
        rt_thread_startup(rt_timer_task);
    }
    return true;
}

bool os_timer_number_get_RTT(void **pp_handle, uint32_t *p_timer_num, bool *p_result)
{
    rt_base_t lock;

    if (pp_handle && *pp_handle)
    {
        osif_timer_list_t *p_handle_timer;
        lock = rt_hw_interrupt_disable();
        p_handle_timer = osif_timer_get_entry(*pp_handle);
        *p_timer_num = p_handle_timer->timer_number;
        rt_hw_interrupt_enable(lock);
        *p_result = true;
        return true;
    }
    else
    {
        *p_timer_num = 0xFF;
        *p_result = false;
    }

    return true;
}

rt_err_t xPendFunctionCall_RTT(PendedFunctionOS_t xFunctionToPend, uint32_t pvParameter1)
{
    rt_timer_queue_cb cb;
    rt_err_t ret;

    cb.pxCallbackFunction = xFunctionToPend;
    cb.pvParameter1 = pvParameter1;
    cb.cb_num = RT_TIMER_EXCUTE_CALLBACK;

    ret = rt_mq_send(rt_timer_queue, &cb, sizeof(rt_timer_queue_cb));

    return ret;
}

bool os_timer_pendcall_RTT(PendedFunctionOS_t xFunctionToPend, uint32_t Parameter1,
                           bool *p_result)
{
    uint32_t ret;

    ret = xPendFunctionCall_RTT(xFunctionToPend, Parameter1);

    if (ret == RT_EOK)
    {
        *p_result = true;
    }
    else
    {
        *p_result = false;
    }
    return true;
}

#include "trace.h"
#include "otp.h"
#include "dlps.h"

rt_mq_t rt_timer_queue;
rt_thread_t rt_timer_task;

static void cmd_proc(rt_timer_queue_cb *cb, uint8_t cmd)
{
    rt_timer_t timer;
    timer = cb->timer;
    rt_base_t lock;

    switch (cmd)
    {
    case RT_TIMER_CMD_START:
        rt_timer_start(timer);
        break;
    case RT_TIMER_CMD_STOP:
        rt_timer_stop(timer);
        break;
    case RT_TIMER_CMD_RESTART:
        lock = rt_hw_interrupt_disable();
        rt_timer_stop(timer);
        rt_timer_control(timer, RT_TIMER_CTRL_SET_TIME, &(cb->change_tick));
        rt_timer_start(timer);
        rt_hw_interrupt_enable(lock);
        break;
    case RT_TIMER_CMD_DEL:
        rt_timer_stop(timer);
        rt_timer_delete(timer);
        break;
    case RT_TIMER_EXCUTE_CALLBACK:
        cb->pxCallbackFunction(cb->pvParameter1);
        break;
    default:
        break;
    }
}

static void timer_cmd_proc(rt_timer_queue_cb *cb)
{
    cmd_proc(cb, cb->cb_num);
}

void timer_task(void *pa)
{
    rt_timer_queue_cb cb_msg;
    while (1)
    {
        if (rt_mq_recv(rt_timer_queue, &cb_msg, sizeof(cb_msg), RT_WAITING_FOREVER) == RT_EOK)
        {
            timer_cmd_proc(&cb_msg);
        }
    }
}



static DATA_RAM_FUNCTION rt_tick_t rt_timer_list_next_timeout_RTT(rt_list_t timer_list[])
{
    char *excluded_handle_RTT[] = {"timer", "timer_task_RTT", "low_stack_task", "dbg_tid_timer", "THERMAL_TIMER", "feature_check_timer", "tidle0"};
    struct rt_timer *timer;
    register rt_base_t level;
    rt_tick_t timeout_tick = RT_TICK_MAX;
    rt_list_t *n;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    if (!rt_list_isempty(&timer_list[RT_TIMER_SKIP_LIST_LEVEL - 1]))
    {
        for (n = timer_list[RT_TIMER_SKIP_LIST_LEVEL - 1].next;
             n != &(timer_list[RT_TIMER_SKIP_LIST_LEVEL - 1]);)
        {
            bool handle_checked = 1;
            timer = rt_list_entry(n, struct rt_timer, row[RT_TIMER_SKIP_LIST_LEVEL - 1]);
            for (uint32_t i = 0; i < sizeof(excluded_handle_RTT) / sizeof(char *); i ++)
            {
                if (rt_strcmp(timer->parent.name, excluded_handle_RTT[i]) == 0)
                {
                    handle_checked = 0;
                    break;
                }
            }
            if (handle_checked)
            {
                timeout_tick = timer->timeout_tick;
                break;
            }
            n = n->next;
        }
    }

    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return timeout_tick;
}


static DATA_RAM_FUNCTION rt_tick_t find_nearest_timeout_tick_RTT(void)
{
    extern rt_list_t rt_soft_timer_list[];
    extern rt_list_t rt_timer_list[];
    rt_tick_t current_tick;
    rt_tick_t next_timeout = rt_timer_list_next_timeout_RTT(rt_soft_timer_list);

    rt_tick_t next_timeout1 = rt_timer_list_next_timeout_RTT(rt_timer_list);

    if (next_timeout1 < next_timeout)
    {
        next_timeout = next_timeout1;
    }

    /* get current tick */
    current_tick = rt_tick_get();

    if ((next_timeout - current_tick) < RT_TICK_MAX / 2)
    {
        /* get the delta timeout tick */
        next_timeout = next_timeout - current_tick;
    }

    return next_timeout;
}



static DATA_RAM_FUNCTION uint32_t DLPS_CheckNextTimeout(void)
{
    extern uint32_t get_valid_systick_current_value_and_update_tick_count(void);
    uint32_t systick_current_value = get_valid_systick_current_value_and_update_tick_count();

    rt_tick_t nearest_timeout_tick = find_nearest_timeout_tick_RTT();

    if (nearest_timeout_tick == RT_TICK_MAX)
    {
        nearest_timeout_tick = 0xFFFFFFFF;
    }
    // xTickCount unit 10msec, wakeup time unit 0.625msec
    if ((nearest_timeout_tick != 0xFFFFFFFF) && (nearest_timeout_tick != 0))
    {
        rt_tick_t wakeup_time_us = nearest_timeout_tick * 10000 - DIVIDE_AND_ROUNDUP((
                SYSTICK_RELOAD_VALUE_RTT - systick_current_value) * 1000000,
                                                                                     configSYSTICK_CLOCK_HZ_RTT) - last_lps_remain_us_get();

        rt_tick_t wakeup_time_slot = wakeup_time_us / 625;

        return (wakeup_time_slot > 0x7D00) ? 0x7D00 : wakeup_time_slot;
    }

    return (uint32_t)nearest_timeout_tick;
}

DATA_RAM_FUNCTION bool os_timer_next_timeout_value_get_RTT(uint32_t *p_value)
{
    *p_value = DLPS_CheckNextTimeout();
    return true;
}

//Call this function in app entry but not in OTA mode
void osif_update_for_rtt(void)
{
    patch_osif_os_init = os_init_RTT;
    patch_osif_os_vector_table_update = os_vector_table_update_RTT;
    patch_osif_os_systick_handler = os_systick_handler_RTT;
    patch_osif_os_delay = os_delay_RTT;
    patch_osif_os_sys_time_get = os_sys_time_get_RTT;
    patch_osif_os_sys_tick_get = os_sys_tick_get_RTT;
    patch_osif_os_sys_tick_increase = os_sys_tick_increase_RTT;
    patch_osif_os_sched_start = os_sched_start_RTT;
    patch_osif_os_sched_suspend = os_sched_suspend_RTT;
    patch_osif_os_sched_resume = os_sched_resume_RTT;
    patch_osif_os_sched_is_start = os_sched_is_start_RTT;

    patch_osif_os_task_create = os_task_create_RTT;
    patch_osif_os_task_delete = os_task_delete_RTT;
    patch_osif_os_task_suspend = os_task_suspend_RTT;
    patch_osif_os_task_resume = os_task_resume_RTT;
    patch_osif_os_task_yield = os_task_yield_RTT;
    patch_osif_os_task_handle_get = os_task_handle_get_RTT;
    patch_osif_os_task_priority_get = os_task_priority_get_RTT;
    patch_osif_os_task_priority_set = os_task_priority_set_RTT;
    patch_osif_os_task_status_dump = os_task_status_dump_RTT;
    patch_osif_os_task_dlps_return_idle_task = os_task_dlps_return_idle_task_RTT;

    patch_osif_os_lock = os_lock_RTT;
    patch_osif_os_unlock = os_unlock_RTT;
    patch_osif_os_sem_create = os_sem_create_RTT;
    patch_osif_os_sem_delete = os_sem_delete_RTT;
    patch_osif_os_sem_take = os_sem_take_RTT;
    patch_osif_os_sem_give = os_sem_give_RTT;
    patch_osif_os_mutex_create = os_mutex_create_RTT;
    patch_osif_os_mutex_delete = os_mutex_delete_RTT;
    patch_osif_os_mutex_take = os_mutex_take_RTT;
    patch_osif_os_mutex_give = os_mutex_give_RTT;

    patch_osif_os_msg_queue_create_intern = os_msg_queue_create_intern_RTT;
    patch_osif_os_msg_queue_delete_intern = os_msg_queue_peek_intern_RTT;
    patch_osif_os_msg_queue_peek_intern = os_msg_queue_peek_intern_RTT;
    patch_osif_os_msg_send_intern = os_msg_send_intern_RTT;
    patch_osif_os_msg_recv_intern = os_msg_recv_intern_RTT;

    patch_osif_os_mem_alloc_intern = os_mem_alloc_intern_RTT;
    patch_osif_os_mem_zalloc_intern = os_mem_zalloc_intern_RTT;
    patch_osif_os_mem_aligned_alloc_intern = os_mem_aligned_alloc_intern_RTT;
    patch_osif_os_mem_free = os_mem_free_RTT;
    patch_osif_os_mem_aligned_free = os_mem_aligned_free_RTT;
    patch_osif_os_mem_peek = os_mem_peek_RTT;
    patch_osif_os_mem_check_heap_usage = os_mem_check_heap_usage_RTT;

    patch_osif_os_timer_init = os_timer_init_RTT;
    patch_osif_os_timer_create =  os_timer_create_RTT;
    patch_osif_os_timer_start = os_timer_start_RTT;
    patch_osif_os_timer_restart = os_timer_restart_RTT;
    patch_osif_os_timer_stop = os_timer_stop_RTT;
    patch_osif_os_timer_delete = os_timer_delete_RTT;
    patch_osif_os_timer_id_get = os_timer_id_get_RTT;
    patch_osif_os_timer_state_get = os_timer_state_get_RTT;
    patch_osif_os_timer_dump = os_timer_dump_RTT;
    patch_osif_os_timer_number_get = os_timer_number_get_RTT;
    patch_osif_os_timer_pendcall = os_timer_pendcall_RTT;
    patch_osif_os_timer_next_timeout_value_get = os_timer_next_timeout_value_get_RTT;

}

USER_CALL_BACK os_patch = osif_update_for_rtt;



