/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_trng.h
* @brief
* @details
* @author    eason li
* @date      2019-05-06
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef __RTL876X_TRNG_H
#define __RTL876X_TRNG_H
// *INDENT-OFF*
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "rtl876x.h"

/**
  * @brief referenc to IO_MODULE (sheet: SoC-VENDOR_REG)
  */
typedef struct
{
    union /* 0x00 */
    {
        __IO uint32_t CTRL;
        struct
        {
            __IO uint32_t rng_srst: 1; /* RNG Software Reset, High Active. It will reset all digital except programmable registers, except Calibration Degital. */
            __IO uint32_t clk_hf_sel: 1; /* Selection of High Speed Clock: 0: clk_hfosc_rng, 1: clk_hfosc_rng/2 */
            __IO uint32_t corrector_bypass: 1; /* Random Bias Corrector will be bypassed if corrector_bypass = 1 */
            __IO uint32_t rsvd0: 2;
            __IO uint32_t corrector_imode: 1; /* Random Bias Corrector sample rate, 0: 4 valid TRNG, 1: 2 valid TRNG (But corrector_bypass is the highest priority) */
            __IO uint32_t dbg0_sel: 5; /* The Debug Signal Group 0 Selection. */
            __IO uint32_t dbg1_sel: 5; /* The Debug Signal Group 1 Selection. */
            __IO uint32_t lfsr_bypass_1: 1; /* LFSR_1 will be bypassed if lfsr_bypass_1 = 1. */
            __IO uint32_t lfsr_mode: 1;
            __IO uint32_t st_en: 1; /* Self test enable */
            __IO uint32_t mode: 1; /* 0: TRNG -> BIAS -> LFSR -> self-test -> buf; 1: TRNG -> BIAS -> self-test -> LFSR -> buf */
            __IO uint32_t rsvd1: 12;
        } CTRL_field;
    };

    __IO uint32_t rsvd0[6]; // 0x04 - 0x18

    union /* 0x1C */
    {
        __IO uint32_t RNG_return0;
        struct
        {
            __I  uint32_t out_ready:        1; /* The Random Number for Realtek is Ready to Read. */
            __IO uint32_t rsvd0:            31;
        } return0_field;
    };

    __I  uint32_t valid_bit_num; /* 0x20, The Bit Number of Realtek Random Number is Generated after reset. It will be wrapped around if reach the top. */
    __I  uint32_t drop_num; /* 0x24, The Bit Number of Realtek Random Number is Dropped after reset. It will be wrapped around if reach the top. */
    __I  uint32_t random_out; /* 0x28, Random Number for Realtek. It will return all zeros if random number is not ready. */
    __IO uint32_t lfsr_poly_lsb; /* 0x2C, RNG LFSR_1 Polynomial LSB 32-bit from register (for Realtek) */
    __IO uint32_t lfsr_poly_msb; /* 0x30, RNG LFSR_1 Polynomial MSB 32-bit from register (for Realtek) */
    __IO uint32_t rsvd1; /* 0x34 */

    union /* 0x38 */
    {
        __IO uint32_t RNG_POWER;
        struct
        {
            __IO uint32_t gating_en:        1; /* Enable power gating function. */
            __IO uint32_t rsvd:             31;
        } power_field;
    };

    union /* 0x3C */
    {
        __IO uint32_t RNG_st;
        struct
        {
            __IO uint32_t thr_done_rep: 8; /* Done threshold for repetition count test */
            __IO uint32_t thr_err_rep: 8; /* Error threshold for repetition count test */
            __IO uint32_t window_size_adap2: 2;
            __IO uint32_t window_size_adap1: 2;
            __IO uint32_t compare_unit_adap2: 1; /* Compare unit for adaptive test 2, 0 : bit mode, 1 : byte mode */
            __IO uint32_t compare_unit_adap1: 1; /* Compare unit for adaptive test 1, 0 : bit mode, 1 : byte mode */
            __IO uint32_t compare_unit_rep: 1; /* Compare unit for repetition test, 0 : bit mode, 1 : byte mode */
            __IO uint32_t rsvd: 1;
            __IO uint32_t thr_done_adap2: 4; /* Done threshold for adaptive proportion test (pass times), Default : pass four times, done_adap2 will be asserted */
            __IO uint32_t thr_done_adap1: 4; /* Done threshold for adaptive proportion test (pass times), Default : pass four times, done_adap1 will be asserted */
        } st_field;
    };

    union /* 0x40 */
    {
        __IO uint32_t RNG_st_return;
        struct
        {
            __IO uint32_t err_adap2: 1; /* Error will be asserted when adaptive proportion test detects error */
            __IO uint32_t err_adap1: 1; /* Error will be asserted when adaptive proportion test detects error */
            __IO uint32_t err_rep: 1; /* Error will be asserted when repetition count test detects error */
            __IO uint32_t err: 1; /* Error will be asserted when self test detects error */
            __I  uint32_t done_adap2: 1; /* Adaptive proportion test 2 is ready */
            __I  uint32_t done_adap1: 1; /* Adaptive proportion test 1 is ready */
            __I  uint32_t done_rep: 1; /* Repetition count test is ready */
            __I  uint32_t done: 1; /* Repetition count test is ready */
            __IO uint32_t rsvd: 24;
        } st_return_field;
    };

    __IO uint32_t rsvd2[4]; /* 0x44 - 0x50 */

    union /* 0x54 */
    {
        __IO uint32_t RNG_interrupt_st;
        struct
        {
            __O  uint32_t write_data: 1; /* 1 to set, 0 to clear bit with 1. */
            __IO uint32_t rsvd0: 1;
            __IO uint32_t parity: 1; /* interrupt for buffer parity check */
            __IO uint32_t rsvd1: 29;
        } interrupt_st_field;
    };

    __IO uint32_t rsvd3[2]; /* 0x58 - 0x5C */

    union /* 0x60 */
    {
        __IO uint32_t RNG_st_thr_err;
        struct
        {
            __IO uint32_t adap2: 16; /* Error threshold for adaptive proportion test, Bit-mode: Max = 3/4 * window size, Byte-mode: Max = 1/4 * window size */
            __IO uint32_t adap1: 16; /* Error threshold for adaptive proportion test, Bit-mode: Max = 3/4 * window size, Byte-mode: Max = 1/4 * window size */
        } st_thr_err_field;
    };

    union /* 0x64 */
    {
        __IO uint32_t RNG_interrupt_enable;
        struct
        {
            __O  uint32_t write_data: 1; /* 1 to set, 0 to clear bit with 1. */
            __IO uint32_t rsvd0: 1;
            __IO uint32_t parity: 1; /* set 1 to enable interrupt */
            __IO uint32_t rsvd1: 29;
        } interrupt_enable_field;
    };
} TRNG_TypeDef;

#define TRNG_BASE   0x40006200UL
#define TRNG        ((TRNG_TypeDef *) TRNG_BASE)

extern void (*init_true_random_generator)(void);
extern uint32_t (*get_true_random_number)(void);

#ifdef __cplusplus
}
#endif
#endif /*__RTL876X_TRNG_H*/

