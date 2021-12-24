/*
 */

#ifndef __OSIF_FOR_876X_H__
#define __OSIF_FOR_876X_H__

/* include rtconfig header to import configuration */
#include <rtconfig.h>

#include "rtdef.h"
#include "rtl876x.h"
#include "mem_config.h"
#include "os_timer.h"
#include "otp.h"
#include <stdint.h>
#include "trace.h"
#include "app_section.h"

#ifdef __cplusplus
extern "C" {
#endif


enum
{
    RT_TIMER_CMD_CB = 0u,
    RT_TIMER_CMD_START,
    RT_TIMER_CMD_STOP,
    RT_TIMER_CMD_RESTART,
    RT_TIMER_CMD_DEL,
    RT_TIMER_EXCUTE_CALLBACK
};

typedef struct
{
    rt_timer_t timer;
    uint8_t    cb_num;
    void *change_tick;
    PendedFunctionOS_t  pxCallbackFunction;
    uint32_t pvParameter1;

} rt_timer_queue_cb;


#define CLOCK_20MHZ                             20000000
#define CLOCK_40MHZ                             40000000
#define CLOCK_90MHZ                             90000000

#define configCPU_CLOCK_HZ_RTT                      ( CLOCK_40MHZ )   // V6 platform
#define configSYSTICK_CLOCK_HZ_RTT                 ( (OTP->systick_clk_src == CORE_CLOCK) ? configCPU_CLOCK_HZ_RTT : OTP->systick_ext_clk_freq )
#define portNVIC_SYSTICK_CLK_BIT_RTT               ( OTP->systick_clk_src << 2UL )

#define SYSTICK_RELOAD_VALUE_RTT                    ((configSYSTICK_CLOCK_HZ_RTT/ RT_TICK_PER_SECOND) - 1UL)
#define DIVIDE_AND_ROUND(dividend, divisor)          ((dividend + (divisor >> 1)) / divisor)
#define DIVIDE_AND_ROUNDUP(dividend, divisor)        ((dividend + (divisor - 1)) / divisor)

#define BUFFER_RAM_ADDR                                     (void *)(0x00280800)
#define BUFFER_RAM_SIZE                                     (OTP->heapBufferONSize)

#define DATA_RAM_ADDR                                       (void *)(OTP->appDataAddr + OTP->appDataSize)
#define DATA_RAM_SIZE                                       (OTP->heapDataONSize)

#define TIMER_NUMBER_MAX 32

#define portNVIC_SYSTICK_CTRL_REG           ( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG           ( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG  ( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI2_REG                ( * ( ( volatile uint32_t * ) 0xe000ed20 ) )
/* ...then bits in the registers. */
#define portNVIC_SYSTICK_INT_BIT            ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT         ( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT     ( 1UL << 16UL )
#define portNVIC_PENDSVCLEAR_BIT            ( 1UL << 27UL )
#define portNVIC_PEND_SYSTICK_CLEAR_BIT     ( 1UL << 25UL )

//#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configKERNEL_INTERRUPT_PRIORITY         ( 0x07 << (8 - 3) )
#define portNVIC_PENDSV_PRI                 ( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define portNVIC_SYSTICK_PRI                ( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

/* Constants required to manipulate the VFP. */
#define portFPCCR                   ( ( volatile uint32_t * ) 0xe000ef34 ) /* Floating point context control register. */
#define portASPEN_AND_LSPEN_BITS    ( 0x3UL << 30UL )

void prvEnableVFP_RTT(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* RT-Thread definitions for C++ */
namespace rtthread
{

enum TICK_WAIT
{
    WAIT_NONE = 0,
    WAIT_FOREVER = -1,
};

}

#endif /* end of __cplusplus */

#endif
