/****************************************************************************************************//**
 * @file     trace_hardfault.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 ****************************************************************************************************/

#ifndef TRACE_HARDFAULT_H
#define TRACE_HARDFAULT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "trace_common.h"
#include "trace_port_config.h"

typedef struct
{
    uint32_t stacked_r0;
    uint32_t stacked_r1;
    uint32_t stacked_r2;
    uint32_t stacked_r3;
    uint32_t stacked_r4;
    uint32_t stacked_r5;
    uint32_t stacked_r6;
    uint32_t stacked_r7;
    uint32_t stacked_r8;
    uint32_t stacked_r9;
    uint32_t stacked_r10;
    uint32_t stacked_r11;
    uint32_t stacked_r12;
    uint32_t stacked_lr;
    uint32_t stacked_pc;
    uint32_t stacked_psr;
    uint32_t old_sp;
    uint32_t msp;
    uint32_t cfsr;
    uint32_t bus_fault_address;
    uint32_t memmanage_fault_address;

    uint32_t PRIMASK;
    uint32_t BASEPRI;
    uint32_t EXC_RETURN;

    uint32_t HFSR;
    uint32_t DFSR;
    uint32_t AFSR;
} T_HARDFAULT_RECORD;

/******** magic pattern for hardfault record ********/
#define HARDFAULT_PATTERN_BEG           (0xbeefface)
#define HARDFAULT_PATTERN_END           (0xbabeface)
#define HARDFAULT_PATTERN_MID           (0xdcabface)

extern bool patch_hardfault_save_to_flash_init(void);

/************Dump info:  *********
*********Part0: beg pattern*******************
4:                  PATTERN_BEG
*********Part1: hardfault register record****
sizeof(*record):    hardfault reg info
4:                  PATTERN_MID
*********Part2: Current task stack****
1024:               current task stack
4:                  PATTERN_MID
16:                 bottom of current task stack
4:                  PATTERN_MID
*********Part3: specific task stack****
task stack size0:   task stack0
4:                  PATTERN_MID
16:                 bottom of task stack0
4:                  PATTERN_MID
task stack size1:   task stack1
4:                  PATTERN_MID
16:                 bottom of task stack1
4:                  PATTERN_MID
.......
task stack size n:   task stack n
4:                  PATTERN_MID
16:                 bottom of task stack n
4:                  PATTERN_MID
**********Part4: end pattern*******************
4:                  PATTERN_END
*********************************/

/**
 * extern void *app_task_handle;
 * TCB_t *pxTimerTCB =(TCB_t *)(*(uint32_t *)0x00200e5c);
 * TCB_t *pxLowerTCB =(TCB_t *)(*(uint32_t *)0x0020176c);
 * TCB_t *pxUpperTCB =(TCB_t *)(*(uint32_t *)0x00201494);
 * TCB_t *pxAppTCB =(TCB_t *)app_task_handle;
*/
/**
 * @brief  Initializes trace_hardfault save to flash including critical task stack
 * @param  dump_task_stack_num: number of task
 * @return if it is successfully initialized , return true

 * For example
   hardfault_save_to_flash_init(2, (uint32_t *)0x0020176c, 1024, &app_task_handle, 1024);
   @param  2: dump two task stack
   @param  (uint32_t *)0x0020176c: lowstack task handler address
   @param  1024: dump task stack size
   @param  (uint32_t *)0x00201494: upperstack task handler address
   @param  1024: dump task stack size
   @note if need dump app task, must call this api after app task created!!!
 */
void hardfault_save_to_flash_init(uint32_t dump_task_stack_num, ...);


#ifdef __cplusplus
}
#endif

#endif

