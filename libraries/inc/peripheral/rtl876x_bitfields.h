/* Copyright (c) 2015 Realtek Semiconductor. All Rights Reserved.
 *
 *  The information contained herein is property of Realtek Semiconductor.
 *  Terms and conditions of usage are described in detail in Realtek
 *  SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 *   Licensees are granted free, non-transferable use of the information. NO
 *   WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 *   the file.
 *
 */
#ifndef __RTL876X_BITS_H
#define __RTL876X_BITS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <core_cm4.h>

/* Peripheral: KEYSCAN */
/* Description: Key Scan. */


/* Register: RTL_KEYSCAN_CR0 */
/* Description: Control register 0. */

/* Bit 31 : Keyscan clock enable */
#define KEYSCAN_CR0_CLKEN_Pos (31UL) /*!< Position of clock enable field. */
#define KEYSCAN_CR0_CLKEN_Msk (0x1UL << KEYSCAN_CR0_CLKEN_Pos) /*!< Bit mask of clock enable field. */
#define KEYSCAN_CR0_CLKEN_Disabled (0UL) /*!< Clock disabled. */
#define KEYSCAN_CR0_CLKEN_Enabled (1UL) /*!< Clock enabled. */

/* Bit 8 : Column selection */
#define KEYSCAN_CR0_COLSEL_Pos (8UL) /*!< Position of column selection field. */
#define KEYSCAN_CR0_COLSEL_Msk (0xFFFFFUL << KEYSCAN_CR0_COLSEL_Pos) /*!< Bit mask of column selection field. */

/* Bit 5 : Row number */
#define KEYSCAN_CR0_ROWNUM_Pos (5UL) /*!< Position of row number field. */
#define KEYSCAN_CR0_ROWNUM_Msk (0x07UL << KEYSCAN_CR0_ROWNUM_Pos) /*!< Bit mask of row number field. */


/* Bit 0 : Column number */
#define KEYSCAN_CR0_COLNUM_Pos (0UL) /*!< Position of column number field. */
#define KEYSCAN_CR0_COLNUM_Msk (0x1FUL << KEYSCAN_CR0_COLNUM_Pos) /*!< Bit mask of column number field. */

/* Register: RTL_KEYSCAN_CR1 */
/* Description: Control register 1. */

/* Bit 27 : Post guard time for column*/
#define KEYSCAN_CR1_POSTGUARD_Pos (27UL) /*!< Position of post guard time field. */
#define KEYSCAN_CR1_POSTGUARD_Msk (0x07UL << KEYSCAN_CR1_POSTGUARD_Pos) /*!< Bit mask of post guard time field. */

/* Bit 24 : Pre guard time for column */
#define KEYSCAN_CR1_PREGUARD_Pos (24UL) /*!< Position pre guard time field. */
#define KEYSCAN_CR1_PREGUARD_Msk (0x07UL << KEYSCAN_CR1_PREGUARD_Pos) /*!< Bit mask of pre guard time field. */

/* Bit 16 : Row selection */
#define KEYSCAN_CR1_ROWSEL_Pos (16UL) /*!< Position of  row selection field. */
#define KEYSCAN_CR1_ROWSEL_Msk (0xFFUL << KEYSCAN_CR1_ROWSEL_Pos) /*!< Bit mask of row selection field. */

/* Bit 8 : Detection period */
#define KEYSCAN_CR1_DETECTPERIOD_Pos (8UL) /*!< Position of detection period field. */
#define KEYSCAN_CR1_DETECTPERIOD_Msk (0x07UL << KEYSCAN_CR1_DETECTPERIOD_Pos) /*!< Bit mask of detection period field. */
#define KEYSCAN_CR1_DETECTPERIOD_Disabled (0UL) /*!< Detection disabled. */
#define KEYSCAN_CR1_DETECTPERIOD_Enabled (1UL) /*!< Detection enabled. */

/* Bit 7 : Debouncing enable */
#define KEYSCAN_CR1_DEBOUNCEN_Pos (7UL) /*!< Position of debouncing enable field. */
#define KEYSCAN_CR1_DEBOUNCEN_Msk (0x01UL << KEYSCAN_CR1_DEBOUNCEN_Pos) /*!< Bit mask of debouncing enable field. */
#define KEYSCAN_CR1_DEBOUNCEN_Disabled (0UL) /*!< Debouncing disabled. */
#define KEYSCAN_CR1_DEBOUNCEN_Enabled (1UL) /*!< Debouncing enabled. */

/* Bit 2 : Timeout period */
#define KEYSCAN_CR1_TOPERIOD_Pos (2UL) /*!< Position of timeout period field. */
#define KEYSCAN_CR1_TOPERIOD_Msk (0x03UL << KEYSCAN_CR1_TOPERIOD_Pos) /*!< Bit mask of timeout period field. */

/* Bit 0 : Wait period */
#define KEYSCAN_CR1_WAITPERIOD_Pos (0UL) /*!< Position of wait period field. */
#define KEYSCAN_CR1_WAITPERIOD_Msk (0x03UL << KEYSCAN_CR1_WAITPERIOD_Pos) /*!< Bit mask of wait period field. */


/* Register: RTL_KEYSCAN_CR2 */
/* Description: Control register 2. */

/* Bit 27 : Program debouncing time enable */
#define KEYSCAN_CR2_DEBEN_Pos (27UL) /*!< Position of program debouncing time enable field. */
#define KEYSCAN_CR2_DEBEN_Msk (0x1UL << KEYSCAN_CR2_DEBEN_Pos) /*!< Bit mask of program debouncing time enable  field. */
#define KEYSCAN_CR2_DEBEN_Disabled (0UL) /*!< Clock disabled. */
#define KEYSCAN_CR2_DEBEN_Enabled (1UL) /*!< Clock enabled. */

/* Bit 16 : Debouncing time*/
#define KEYSCAN_CR2_DEBTIME_Pos (16UL) /*!< Position of debouncing time field. */
#define KEYSCAN_CR2_DEBTIME_Msk (0x3FFUL << KEYSCAN_CR2_DEBTIME_Pos) /*!< Bit mask of debouncing time field. */

/* Bit 15 : FIFO clear */
#define KEYSCAN_CR2_FIFOCLEAR_Pos (15UL) /*!< Position of FIFO clear field. */
#define KEYSCAN_CR2_FIFOCLEAR_Msk (0x1UL << KEYSCAN_CR2_FIFOCLEAR_Pos) /*!< Bit mask of FIFO clear field. */

/* Bit 14 : Force scan fsm idle */
#define KEYSCAN_CR2_FSMIDLE_Pos (14UL) /*!< Position of force scan fsm idle field. */
#define KEYSCAN_CR2_FSMIDLE_Msk (0x1UL << KEYSCAN_CR2_FSMIDLE_Pos) /*!< Bit mask of force scan fsm idle field. */

/* Bit 13 : FIFO flag clear1 */
#define KEYSCAN_CR2_FIFOFLAGCLR1_Pos (13UL) /*!< Position of FIFO flag clear1 field. */
#define KEYSCAN_CR2_FIFOFLAGCLR1_Msk (0x1UL << KEYSCAN_CR2_FIFOFLAGCLR1_Pos) /*!< Bit mask of FIFO flag clear1 field. */

/* Bit 12 : FIFO flag clear2 */
#define KEYSCAN_CR2_FIFOFLAGCLR2_Pos (12UL) /*!< Position of FIFO flag clear2 field. */
#define KEYSCAN_CR2_FIFOFLAGCLR2_Msk (0x1UL << KEYSCAN_CR2_FIFOFLAGCLR2_Pos) /*!< Bit mask of FIFO flag clear2 field. */

/* Bit 7 : Key scan trigger type */
#define KEYSCAN_CR2_TRIGTYPE_Pos (7UL) /*!< Position of ey scan trigger type field. */
#define KEYSCAN_CR2_TRIGTYPE_Msk (0x1UL << KEYSCAN_CR2_TRIGTYPE_Pos) /*!< Bit mask of key scan trigger type field. */
#define KEYSCAN_CR2_TRIGTYPE_EDGE (0UL) /*!< Edge trigger */
#define KEYSCAN_CR2_TRIGTYPE_LEVEL (1UL) /*!< Level trigger */

/* Bit 6 : FIFO overflow control */
#define KEYSCAN_CR2_FIFOOVCTL_Pos (6UL) /*!< Position of FIFO overflow control field. */
#define KEYSCAN_CR2_FIFOOVCTL_Msk (0x1UL << KEYSCAN_CR2_FIFOOVCTL_Pos) /*!< Bit mask of FIFO overflow control field. */
#define KEYSCAN_CR2_FIFOOVCTL_DISCARDNEW (0UL) /*!< Discard all the new scan data when FIFO is full. */
#define KEYSCAN_CR2_FIFOOVCTL_DISCARDOLD (1UL) /*!< Discard the last scan data when FIFO is full. */

/* Bit 0 : Scan key limit */
#define KEYSCAN_CR2_KEYLIMIT_Pos (0UL) /*!< Position of scan key limit field. */
#define KEYSCAN_CR2_KEYLIMIT_Msk (0x1FUL << KEYSCAN_CR2_KEYLIMIT_Pos) /*!< Bit mask of scan key limit field. */


/* Register: RTL_KEYSCAN_INTCR */
/* Description: Interrupt Control Register */

/* Bit 15 : Interrupt mask */
#define KEYSCAN_INTCR_INTMSK_Pos (15UL) /*!< Position of interrupt mask field. */
#define KEYSCAN_INTCR_INTMSK_Msk (0x1UL << KEYSCAN_INTCR_INTMSK_Pos) /*!< Bit mask of interrupt mask field. */

/* Bit 14 : Int3 clear */
#define KEYSCAN_INTCR_INT3CLR_Pos (14UL) /*!< Position of Int3 clear field. */
#define KEYSCAN_INTCR_INT3CLR_Msk (0x1UL << KEYSCAN_INTCR_INT3CLR_Pos) /*!< Bit mask of Int3 clear field. */

/* Bit 13 : Int2 clear */
#define KEYSCAN_INTCR_INT2CLR_Pos (13UL) /*!< Position of Int2 clear field. */
#define KEYSCAN_INTCR_INT2CLR_Msk (0x1UL << KEYSCAN_INTCR_INT2CLR_Pos) /*!< Bit mask of Int2 clear field. */

/* Bit 12 : Int1 clear */
#define KEYSCAN_INTCR_INT1CLR_Pos (12UL) /*!< Position of Int1 clear field. */
#define KEYSCAN_INTCR_INT1CLR_Msk (0x1UL << KEYSCAN_INTCR_INT1CLR_Pos) /*!< Bit mask of Int1 clear field. */

/* Bit 11 : Interrupt control mode */
#define KEYSCAN_INTCR_INTCTLMODE_Pos (11UL) /*!< Position of interrupt control mode field. */
#define KEYSCAN_INTCR_INTCTLMODE_Msk (0x1UL << KEYSCAN_INTCR_INTCTLMODE_Pos) /*!< Bit mask of interrupt control mode field. */
#define KEYSCAN_INTCR_INTCTLMODE_HW (0UL) /*!< HW control interrupt mode */
#define KEYSCAN_INTCR_INTCTLMODE_SW (1UL) /*!< SW control interrupt mode */

/* Bit 10 : Enable/Disable interrupt in SW control mode */
#define KEYSCAN_INTCR_SWINTEN_Pos (10UL) /*!< Position of Enable/Disable interrupt in SW control mode field. */
#define KEYSCAN_INTCR_SWINTEN_Msk (0x1UL << KEYSCAN_INTCR_SWINTEN_Pos) /*!< Bit mask of Enable/Disable interrupt in SW control mode field. */

/* Bit 8 : Interrupt timeout period */
#define KEYSCAN_INTCR_INTTOPERIOD_Pos (8UL) /*!< Position of Interrupt timeout period field. */
#define KEYSCAN_INTCR_INTTOPERIOD_Msk (0x3UL << KEYSCAN_INTCR_INTTOPERIOD_Pos) /*!< Bit mask of Interrupt timeout period field. */

/* Bit 4 : Interrupt threshold */
#define KEYSCAN_INTCR_INTTHRESHOLD_Pos (4UL) /*!< Position of ey Interrupt threshold field. */
#define KEYSCAN_INTCR_INTTHRESHOLD_Msk (0x07UL << KEYSCAN_INTCR_INTTHRESHOLD_Pos) /*!< Bit mask of key Interrupt threshold field. */

/* Bit 2 : Int3 enable */
#define KEYSCAN_INTCR_INT3EN_Pos (2UL) /*!< Position of Int3 enable field. */
#define KEYSCAN_INTCR_INT3EN_Msk (0x1UL << KEYSCAN_INTCR_INT3EN_Pos) /*!< Bit mask of Int3 enable field. */

/* Bit 1 : Int2 enable */
#define KEYSCAN_INTCR_INT2EN_Pos (1UL) /*!< Position of Int2 enable field. */
#define KEYSCAN_INTCR_INT2EN_Msk (0x1UL << KEYSCAN_INTCR_INT2EN_Pos) /*!< Bit mask of Int2 enable field. */

/* Bit 0 : Int1 enable */
#define KEYSCAN_INTCR_INT1EN_Pos (0UL) /*!< Position of Int1 enable field. */
#define KEYSCAN_INTCR_INT1EN_Msk (0x1UL << KEYSCAN_INTCR_INT1EN_Pos) /*!< Bit mask of Int1 enable field. */


/* Register: RTL_KEYSCAN_FIFODATA */
/* Description: . */

/* Bit 5 : Row index */
#define KEYSCAN_FIFODATA_ROWINDEX_Pos (5UL) /*!< Position of row index field. */
#define KEYSCAN_FIFODATA_ROWINDEX_Msk (0x1FUL << KEYSCAN_FIFODATA_ROWINDEX_Pos) /*!< Bit mask of row index field. */

/* Bit 0 : Column index */
#define KEYSCAN_FIFODATA_COLINDEX_Pos (0UL) /*!< Position of column index field. */
#define KEYSCAN_FIFODATA_COLINDEX_Msk (0x7UL << KEYSCAN_FIFODATA_COLINDEX_Pos) /*!< Bit mask of column index field. */


/* Register: RTL_KEYSCAN_STATUS */
/* Description: . */

/* Bit 8 : FIFO offset */
#define KEYSCAN_STATUS_FIFOOFFSET_Pos (8UL) /*!< Position of FIFO offset field. */
#define KEYSCAN_STATUS_FIFOOFFSET_Msk (0x1FUL << KEYSCAN_STATUS_FIFOOFFSET_Pos) /*!< Bit mask of FIFO offset field. */

/* Bit 7 : FIFO reject flag */
#define KEYSCAN_STATUS_FIFOREJECT_Pos (7UL) /*!< Position of FIFO reject flag. */
#define KEYSCAN_STATUS_FIFOREJECT_Msk (0x1UL << KEYSCAN_STATUS_FIFOREJECT_Pos) /*!< Bit mask of FIFO reject flag. */

/* Bit 6 : FIFO overflow flag */
#define KEYSCAN_STATUS_FIFOOVERFLOW_Pos (5UL) /*!< Position of FIFO overflow flag field. */
#define KEYSCAN_STATUS_FIFOOVERFLOW_Msk (0x1UL << KEYSCAN_STATUS_FIFOOVERFLOW_Pos) /*!< Bit mask of FIFO overflow flag field. */

/* Bit 5 : FIFO full flag */
#define KEYSCAN_STATUS_FIFOFULL_Pos (5UL) /*!< Position of FIFO full flag field. */
#define KEYSCAN_STATUS_FIFOFULL_Msk (0x1UL << KEYSCAN_STATUS_FIFOFULL_Pos) /*!< Bit mask of FIFO full flag field. */

/* Bit 4 : FIFO empty flag */
#define KEYSCAN_STATUS_FIFOEMPTY_Pos (4UL) /*!< Position of FIFO empty flag field. */
#define KEYSCAN_STATUS_FIFOEMPTY_Msk (0x1UL << KEYSCAN_STATUS_FIFOEMPTY_Pos) /*!< Bit mask of FIFO empty flag field. */

/* Bit 0 : Current state */
#define KEYSCAN_STATUS_CURSTATE_Pos (0UL) /*!< Position of Current state field. */
#define KEYSCAN_STATUS_CURSTATE_Msk (0x7UL << KEYSCAN_STATUS_CURSTATE_Pos) /*!< Bit mask of Current state field. */


/****************************************************************************************************************/
/* Peripheral: System Block Control */
/* Description: System Block Control. */

/* Register: REG_SYS_CLK_SEL */
/* Description: REG_SYS_CLK_SEL. */

/* Bit 31 : r_SYS_CKE_SPLL. Clock enable of 40MHz and divider */
#define SYSBLK_SYSCLK_CLKEN_SPLL_Pos (31UL) /*!< Position of  */
#define SYSBLK_SYSCLK_CLKEN_SPLL_Msk (0x1UL << SYSBLK_SYSCLK_CLKEN_SPLL_Pos) /*!< Bit mask of  */

/* Bit 30 : r_SYS_40M_SEL. 0: from 40M RCOSC 1: from PLL */
#define SYSBLK_SYSCLK_SRCSEL_Pos (30UL) /*!< Position of . */
#define SYSBLK_SYSCLK_SRCSEL_Msk (0x1UL << SYSBLK_SYSCLK_SRCSEL_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_SYS_CKSL_CPU. CPU clock select 0: 40MHz 1: 20MHz 2: 10MHz 3: 5MHz 4: 53MHz 5: 26.5MHz Others: reserved */
#define SYSBLK_SYSCLK_CPUCLK_Pos (0UL) /*!< Position of  */
#define SYSBLK_SYSCLK_CPUCLK_Msk (0x7UL << SYSBLK_SYSCLK_CPUCLK_Pos) /*!< Bit mask of  */


/* Register: REG_SOC_FUNC_EN */
/* Description: REG_SOC_FUNC_EN. */

/* Bit 16 : BIT_SOC_GTIMER_EN. 1: enable GTIMER IP and GTIMER register */
#define SYSBLK_GTIMER_EN_Pos (16UL) /*!< Position of  */
#define SYSBLK_GTIMER_EN_Msk (0x1UL << SYSBLK_GTIMER_EN_Pos) /*!< Bit mask of  */

/* Bit 13 : BIT_SOC_GDMA0_EN. 1: Enable GDMA block */
#define SYSBLK_GDMA0_EN_Pos (13UL) /*!< Position of . */
#define SYSBLK_GDMA0_EN_Msk (0x1UL << SYSBLK_GDMA0_EN_Pos) /*!< Bit mask of  */

/* Bit 12 : BIT_SOC_LOG_UART_EN. 1: enable log UART; 0: disable log UART */
#define SYSBLK_LOGUART_EN_Pos (12UL) /*!< Position of . */
#define SYSBLK_LOGUART_EN_Msk (0x1UL << SYSBLK_LOGUART_EN_Pos) /*!< Bit mask of  */

/* Bit 5 : BIT_SOC_eFLASH_EN. 1. enable eflash controller */
#define SYSBLK_EFLASH_EN_Pos (5UL) /*!< Position of . */
#define SYSBLK_EFLASH_EN_Msk (0x1UL << SYSBLK_EFLASH_EN_Pos) /*!< Bit mask of  */

/* Bit 4 : BIT_SOC_FLASH_EN. 1: enaable flash controller */
#define SYSBLK_EXTFLASH_EN_Pos (4UL) /*!< Position of . */
#define SYSBLK_EXTFLASH_EN_Msk (0x1UL << SYSBLK_EXTFLASH_EN_Pos) /*!< Bit mask of  */

/* Bit 2 : BIT_SOC_BTBUS_EN. 1: enable BT function */
#define SYSBLK_BTBUS_EN_Pos (2UL) /*!< Position of . */
#define SYSBLK_BTBUS_EN_Msk (0x1UL << SYSBLK_BTBUS_EN_Pos) /*!< Bit mask of  */


/* Register: REG_SOC_PERI_FUNC0_EN */
/* Description: REG_SOC_PERI_FUNC0_EN. */

/* Bit 24 : BIT_PERI_SPI2W_EN. 1: enable 2-wire/3-wire SPI interface */
#define SYSBLK_SPI2W_EN_Pos (24UL) /*!< Position of  */
#define SYSBLK_SPI2W_EN_Msk (0x1UL << SYSBLK_SPI2W_EN_Pos) /*!< Bit mask of  */

/* Bit 19 : BIT_PERI_KEYSCAN_EN. 1: enable KEYSCAN interface */
#define SYSBLK_KEYSCAN_EN_Pos (19UL) /*!< Position of . */
#define SYSBLK_KEYSCAN_EN_Msk (0x1UL << SYSBLK_KEYSCAN_EN_Pos) /*!< Bit mask of  */

/* Bit 18 : BIT_PERI_QDECODE_EN. 1: enable QDECODE interface */
#define SYSBLK_QDECODE_EN_Pos (18UL) /*!< Position of . */
#define SYSBLK_QDECODE_EN_Msk (0x1UL << SYSBLK_QDECODE_EN_Pos) /*!< Bit mask of  */

/* Bit 17 : BIT_PERI_I2C1_EN. 1: enable I2C1 interface */
#define SYSBLK_I2C1_EN_Pos (17UL) /*!< Position of . */
#define SYSBLK_I2C1_EN_Msk (0x1UL << SYSBLK_I2C1_EN_Pos) /*!< Bit mask of  */

/* Bit 16 : BIT_PERI_I2C0_EN. 1: enable I2C0 interface */
#define SYSBLK_I2C0_EN_Pos (16UL) /*!< Position of . */
#define SYSBLK_I2C0_EN_Msk (0x1UL << SYSBLK_I2C0_EN_Pos) /*!< Bit mask of  */

/* Bit 10 : BIT_PERI_IR_EN. 1: enable IR interface */
#define SYSBLK_IR_EN_Pos (10UL) /*!< Position of . */
#define SYSBLK_IR_EN_Msk (0x1UL << SYSBLK_IR_EN_Pos) /*!< Bit mask of  */

/* Bit 9 : BIT_PERI_SPI1_EN. 1: enable SPI1 interface */
#define SYSBLK_SPI1_EN_Pos (9UL) /*!< Position of . */
#define SYSBLK_SPI1_EN_Msk (0x1UL << SYSBLK_SPI1_EN_Pos) /*!< Bit mask of  */

/* Bit 8 : BIT_PERI_SPI0_EN. 1: enable SPI0 interface */
#define SYSBLK_SPI0_EN_Pos (8UL) /*!< Position of . */
#define SYSBLK_SPI0_EN_Msk (0x1UL << SYSBLK_SPI0_EN_Pos) /*!< Bit mask of  */

/* Bit 1 : BIT_PERI_UART1_EN. 1; enable UART1 interface ( DATA Uart1) */
#define SYSBLK_UART1_EN_Pos (1UL) /*!< Position of . */
#define SYSBLK_UART1_EN_Msk (0x1UL << SYSBLK_UART1_EN_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_PERI_UART0_EN. 1; enable UART0 interface ( DATA Uart) */
#define SYSBLK_UART0_EN_Pos (0UL) /*!< Position of . */
#define SYSBLK_UART0_EN_Msk (0x1UL << SYSBLK_UART0_EN_Pos) /*!< Bit mask of  */


/* Register: REG_SOC_PERI_FUNC1_EN */
/* Description: REG_SOC_PERI_FUNC1_EN. */

/* Bit 8 : BIT_PERI_GPIO_EN. 1: enable DW GPIO */
#define SYSBLK_GPIO_EN_Pos (8UL) /*!< Position of  */
#define SYSBLK_GPIO_EN_Msk (0x1UL << SYSBLK_GPIO_EN_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_PERI_ADC_EN. 1: enable ADC */
#define SYSBLK_ADC_EN_Pos (0UL) /*!< Position of . */
#define SYSBLK_ADC_EN_Msk (0x1UL << SYSBLK_ADC_EN_Pos) /*!< Bit mask of  */


/* Register: REG_PESOC_CLK_CTRL */
/* Description: REG_PESOC_CLK_CTRL. */

/* Bit 25 : BIT_SOC_SLPCK_GPIO_EN */
#define SYSBLK_SLPCK_GPIO_EN_Pos (25UL) /*!< Position of  */
#define SYSBLK_SLPCK_GPIO_EN_Msk (0x1UL << SYSBLK_SLPCK_GPIO_EN_Pos) /*!< Bit mask of  */

/* Bit 24 : BIT_SOC_ACTCK_GPIO_EN */
#define SYSBLK_ACTCK_GPIO_EN_Pos (24UL) /*!< Position of  */
#define SYSBLK_ACTCK_GPIO_EN_Msk (0x1UL << SYSBLK_ACTCK_GPIO_EN_Pos) /*!< Bit mask of  */

/* Bit 17 : BIT_SOC_SLPCK_GDMA0_EN */
#define SYSBLK_SLPCK_GDMA0_EN_Pos (17UL) /*!< Position of  */
#define SYSBLK_SLPCK_GDMA0_EN_Msk (0x1UL << SYSBLK_SLPCK_GDMA0_EN_Pos) /*!< Bit mask of  */

/* Bit 16 : BIT_SOC_ACTCK_GDMA0_EN */
#define SYSBLK_ACTCK_GDMA0_EN_Pos (16UL) /*!< Position of  */
#define SYSBLK_ACTCK_GDMA0_EN_Msk (0x1UL << SYSBLK_ACTCK_GDMA0_EN_Pos) /*!< Bit mask of  */

/* Bit 15 : BIT_SOC_SLPCK_TIMER_EN */
#define SYSBLK_SLPCK_TIMER_EN_Pos (15UL) /*!< Position of  */
#define SYSBLK_SLPCK_TIMER_EN_Msk (0x1UL << SYSBLK_SLPCK_TIMER_EN_Pos) /*!< Bit mask of  */

/* Bit 14 : BIT_SOC_ACTCK_TIMER_EN */
#define SYSBLK_ACTCK_TIMER_EN_Pos (14UL) /*!< Position of  */
#define SYSBLK_ACTCK_TIMER_EN_Msk (0x1UL << SYSBLK_ACTCK_TIMER_EN_Pos) /*!< Bit mask of  */

/* Bit 13 : BIT_SOC_SLPCK_LOG_UART_EN */
#define SYSBLK_SLPCK_LOGUART_EN_Pos (13UL) /*!< Position of  */
#define SYSBLK_SLPCK_LOGUART_EN_Msk (0x1UL << SYSBLK_SLPCK_LOGUART_EN_Pos) /*!< Bit mask of  */

/* Bit 12 : BIT_SOC_ACTCK_LOG_UART_EN */
#define SYSBLK_ACTCK_LOGUART_EN_Pos (12UL) /*!< Position of  */
#define SYSBLK_ACTCK_LOGUART_EN_Msk (0x1UL << SYSBLK_ACTCK_LOGUART_EN_Pos) /*!< Bit mask of  */

/* Bit 11 : BIT_SOC_SLPCK_UART1_DATA_EN */
#define SYSBLK_SLPCK_UART1DATA_EN_Pos (11UL) /*!< Position of  */
#define SYSBLK_SLPCK_UART1DATA_EN_Msk (0x1UL << SYSBLK_SLPCK_UART1DATA_EN_Pos) /*!< Bit mask of  */

/* Bit 10 : BIT_SOC_ACTCK_UART1_DATA_EN */
#define SYSBLK_ACTCK_UART1DATA_EN_Pos (10UL) /*!< Position of  */
#define SYSBLK_ACTCK_UART1DATA_EN_Msk (0x1UL << SYSBLK_ACTCK_UART1DATA_EN_Pos) /*!< Bit mask of  */

/* Bit 9 : BIT_SOC_SLPCK_FLASH_EN */
#define SYSBLK_SLPCK_FLASH_EN_Pos (9UL) /*!< Position of  */
#define SYSBLK_SLPCK_FLASH_EN_Msk (0x1UL << SYSBLK_SLPCK_FLASH_EN_Pos) /*!< Bit mask of  */

/* Bit 8 : BIT_SOC_ACTCK_FLASH_EN */
#define SYSBLK_ACTCK_FLASH_EN_Pos (8UL) /*!< Position of  */
#define SYSBLK_ACTCK_FLASH_EN_Msk (0x1UL << SYSBLK_ACTCK_FLASH_EN_Pos) /*!< Bit mask of  */

/* Bit 7 : BIT_SOC_SLPCK_VENDOR_REG_EN */
#define SYSBLK_SLPCK_VENDORREG_EN_Pos (7UL) /*!< Position of  */
#define SYSBLK_SLPCK_VENDORREG_EN_Msk (0x1UL << SYSBLK_SLPCK_VENDORREG_EN_Pos) /*!< Bit mask of  */

/* Bit 6 : BIT_SOC_ACTCK_VENDOR_REG_EN */
#define SYSBLK_ACTCK_VENDORREG_EN_Pos (6UL) /*!< Position of  */
#define SYSBLK_ACTCK_VENDORREG_EN_Msk (0x1UL << SYSBLK_ACTCK_VENDORREG_EN_Pos) /*!< Bit mask of  */

/* Bit 3 : BIT_CKE_EFC. Embedded flash controller clock enable */
#define SYSBLK_EFLASH_CLK_EN_Pos (3UL) /*!< Position of  */
#define SYSBLK_EFLASH_CLK_Msk (0x1UL << SYSBLK_EFLASH_CLK_EN_Pos) /*!< Bit mask of  */

/* Bit 1 : BIT_CKE_CORDIC. CORDIC clock enable */
#define SYSBLK_CODEC_CLK_EN_Pos (1UL) /*!< Position of  */
#define SYSBLK_CODEC_CLK_EN_Msk (0x1UL << SYSBLK_CODEC_CLK_EN_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_CKE_HWSPI. HWSPI clock enable */
#define SYSBLK_HWSPI_CLK_EN_Pos (0UL) /*!< Position of  */
#define SYSBLK_HWSPI_CLK_EN_Msk (0x1UL << SYSBLK_HWSPI_CLK_EN_Pos) /*!< Bit mask of  */
#define SYSBLK_ADC_EN_Msk (0x1UL << SYSBLK_ADC_EN_Pos) /*!< Bit mask of  */


/* Register: REG_PESOC_PERI_CLK_CTRL0 */
/* Description: REG_PESOC_PERI_CLK_CTRL0. */

/* Bit 21 : BIT_SOC_SLPCK_IR_EN. 1: IR clock enable when CPU sleep command */
#define SYSBLK_SLPCK_IR_EN_Pos (21UL) /*!< Position of  */
#define SYSBLK_SLPCK_IR_EN_Msk (0x1UL << SYSBLK_SLPCK_IR_EN_Pos) /*!< Bit mask of  */

/* Bit 20 : BIT_SOC_ACTCK_IR_EN */
#define SYSBLK_ACTCK_IR_EN_Pos (20UL) /*!< Position of  */
#define SYSBLK_ACTCK_IR_EN_Msk (0x1UL << SYSBLK_ACTCK_IR_EN_Pos) /*!< Bit mask of  */

/* Bit 19 : BIT_SOC_SLPCK_SPI1_EN */
#define SYSBLK_SLPCK_SPI1_EN_Pos (19UL) /*!< Position of  */
#define SYSBLK_SLPCK_SPI1_EN_Msk (0x1UL << SYSBLK_SLPCK_SPI1_EN_Pos) /*!< Bit mask of  */

/* Bit 18 : BIT_SOC_ACTCK_SPI1_EN */
#define SYSBLK_ACTCK_SPI1_EN_Pos (18UL) /*!< Position of  */
#define SYSBLK_ACTCK_SPI1_EN_Msk (0x1UL << SYSBLK_ACTCK_SPI1_EN_Pos) /*!< Bit mask of  */

/* Bit 17 : BIT_SOC_SLPCK_SPI0_EN */
#define SYSBLK_SLPCK_SPI0_EN_Pos (17UL) /*!< Position of  */
#define SYSBLK_SLPCK_SPI0_EN_Msk (0x1UL << SYSBLK_SLPCK_SPI0_EN_Pos) /*!< Bit mask of  */

/* Bit 16 : BIT_SOC_ACTCK_SPI0_EN */
#define SYSBLK_ACTCK_SPI0_EN_Pos (16UL) /*!< Position of  */
#define SYSBLK_ACTCK_SPI0_EN_Msk (0x1UL << SYSBLK_ACTCK_SPI0_EN_Pos) /*!< Bit mask of  */

/* Bit 3 : BIT_SOC_SLPCK_UART1_HCI_EN */
#define SYSBLK_SLPCK_UART1HCI_EN_Pos (3UL) /*!< Position of  */
#define SYSBLK_SLPCK_UART1HCI_EN_Msk (0x1UL << SYSBLK_SLPCK_UART1HCI_EN_Pos) /*!< Bit mask of  */

/* Bit 2 : BIT_SOC_ACTCK_UART1_HCI_EN */
#define SYSBLK_ACTCK_UART1HCI_EN_Pos (2UL) /*!< Position of  */
#define SYSBLK_ACTCK_UART1HCI_EN_Msk (0x1UL << SYSBLK_ACTCK_UART1HCI_EN_Pos) /*!< Bit mask of  */

/* Bit 1 : BIT_SOC_SLPCK_UART0_DATA_EN */
#define SYSBLK_SLPCK_UART0DATA_EN_Pos (1UL) /*!< Position of  */
#define SYSBLK_SLPCK_UART0DATA_EN_Msk (0x1UL << SYSBLK_SLPCK_UART0DATA_EN_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_SOC_ACTCK_UART0_DATA_EN */
#define SYSBLK_ACTCK_UART0DATA_EN_Pos (0UL) /*!< Position of  */
#define SYSBLK_ACTCK_UART0DATA_EN_Msk (0x1UL << SYSBLK_ACTCK_UART0DATA_EN_Pos) /*!< Bit mask of  */


/* Register: REG_PESOC_PERI_CLK_CTRL1 */
/* Description: REG_PESOC_PERI_CLK_CTRL1. */

/* Bit 31 : r_CODEC_CLK_10M_EN. Codec 10MHz clock source enable */
#define SYSBLK_CODEC_CLK10M_EN_Pos (31UL) /*!< Position of  */
#define SYSBLK_CODEC_CLK10M_EN_Msk (0x1UL << SYSBLK_CODEC_CLK10M_EN_Pos) /*!< Bit mask of  */

/* Bit 30 : r_CODEC_CLK_40M_EN */
#define SYSBLK_CODEC_CLK40M_EN_Pos (30UL) /*!< Position of  */
#define SYSBLK_CODEC_CLK40M_EN_Msk (0x1UL << SYSBLK_CODEC_CLK40M_EN_Pos) /*!< Bit mask of  */

/* Bit 25 : BIT_SOC_SLPCK_ADC_EN */
#define SYSBLK_SLPCK_ADC_EN_Pos (25UL) /*!< Position of  */
#define SYSBLK_SLPCK_ADC_EN_Msk (0x1UL << SYSBLK_SLPCK_ADC_EN_Pos) /*!< Bit mask of  */

/* Bit 24 : BIT_SOC_ACTCK_ADC_EN */
#define SYSBLK_ACTCK_ADC_EN_Pos (24UL) /*!< Position of  */
#define SYSBLK_ACTCK_ADC_EN_Msk (0x1UL << SYSBLK_ACTCK_ADC_EN_Pos) /*!< Bit mask of  */

/* Bit 17 : BIT_SOC_SLPCK_SPI_3WIRE_EN */
#define SYSBLK_SLPCK_SPI2WIRE_EN_Pos (17UL) /*!< Position of  */
#define SYSBLK_SLPCK_SPI2WIRE_EN_Msk (0x1UL << SYSBLK_SLPCK_SPI2WIRE_EN_Pos) /*!< Bit mask of  */

/* Bit 16 : BIT_SOC_ACTCK_SPI_3WIRE_EN */
#define SYSBLK_ACTCK_SPI2WIRE_EN_Pos (16UL) /*!< Position of  */
#define SYSBLK_ACTCK_SPI2WIRE_EN_Msk (0x1UL << SYSBLK_ACTCK_SPI2WIRE_EN_Pos) /*!< Bit mask of  */

/* Bit 7 : BIT_SOC_SLPCK_KEYSCAN_EN */
#define SYSBLK_SLPCK_KEYSCAN_EN_Pos (7UL) /*!< Position of  */
#define SYSBLK_SLPCK_KEYSCAN_EN_Msk (0x1UL << SYSBLK_SLPCK_KEYSCAN_EN_Pos) /*!< Bit mask of  */

/* Bit 6 : BIT_SOC_ACTCK_KEYSCAN_EN */
#define SYSBLK_ACTCK_KEYSCAN_EN_Pos (6UL) /*!< Position of  */
#define SYSBLK_ACTCK_KEYSCAN_EN_Msk (0x1UL << SYSBLK_ACTCK_KEYSCAN_EN_Pos) /*!< Bit mask of  */

/* Bit 5 : BIT_SOC_SLPCK_QDECODE_EN */
#define SYSBLK_SLPCK_QDECODE_EN_Pos (5UL) /*!< Position of  */
#define SYSBLK_SLPCK_QDECODE_EN_Msk (0x1UL << SYSBLK_SLPCK_QDECODE_EN_Pos) /*!< Bit mask of  */

/* Bit 4 : BIT_SOC_ACTCK_QDECODE_EN */
#define SYSBLK_ACTCK_QDECODE_EN_Pos (4UL) /*!< Position of  */
#define SYSBLK_ACTCK_QDECODE_EN_Msk (0x1UL << SYSBLK_ACTCK_QDECODE_EN_Pos) /*!< Bit mask of  */

/* Bit 3 : BIT_SOC_SLPCK_I2C1_EN */
#define SYSBLK_SLPCK_I2C1_EN_Pos (3UL) /*!< Position of  */
#define SYSBLK_SLPCK_I2C1_EN_Msk (0x1UL << SYSBLK_SLPCK_I2C1_EN_Pos) /*!< Bit mask of  */

/* Bit 2 : BIT_SOC_ACTCK_I2C1_EN */
#define SYSBLK_ACTCK_I2C1_EN_Pos (2UL) /*!< Position of  */
#define SYSBLK_ACTCK_I2C1_EN_Msk (0x1UL << SYSBLK_ACTCK_I2C1_EN_Pos) /*!< Bit mask of  */

/* Bit 1 : BIT_SOC_SLPCK_I2C0_EN */
#define SYSBLK_SLPCK_I2C0_EN_Pos (1UL) /*!< Position of  */
#define SYSBLK_SLPCK_I2C0_EN_Msk (0x1UL << SYSBLK_SLPCK_I2C0_EN_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_SOC_ACTCK_I2C0_EN */
#define SYSBLK_ACTCK_I2C0_EN_Pos (0UL) /*!< Position of  */
#define SYSBLK_ACTCK_I2C0_EN_Msk (0x1UL << SYSBLK_ACTCK_I2C0_EN_Pos) /*!< Bit mask of  */


/* Register: REG_PESOC_COM_CLK_CTRL1 */
/* Description: REG_PESOC_COM_CLK_CTRL1. */

/* Bit 1 : BIT_SOC_SLPCK_BTBUS_EN. 1: BTBUS clock enable when CPU sleep mode */
#define SYSBLK_SLPCK_BTBUS_EN_Pos (1UL) /*!< Position of  */
#define SYSBLK_SLPCK_BTBUS_EN_Msk (0x1UL << SYSBLK_SLPCK_BTBUS_EN_Pos) /*!< Bit mask of  */

/* Bit 0 : BIT_SOC_ACTCK_BTBUS_EN. 1: BTBUS clock enable in CPU run mode */
#define SYSBLK_ACTCK_BTBUS_EN_Pos (0UL) /*!< Position of  */
#define SYSBLK_ACTCK_BTBUS_EN_Msk (0x1UL << SYSBLK_ACTCK_BTBUS_EN_Pos) /*!< Bit mask of  */


/****************************************************************************************************************/
/* Peripheral: Quad Decoder */
/* Description: Quad Decoder */



#ifdef __cplusplus
}
#endif

#endif

