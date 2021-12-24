/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_if8080.h
* \brief    The header file of the peripheral 8080 parallel interface driver.
* \details  This file provides all 8080 firmware functions.
* \author   elliot chen
* \date     2019-10-10
* \version  v2.1.0
* *********************************************************************************************************
*/


#ifndef _RTL876x_IF8080_H_
#define _RTL876x_IF8080_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    IF8080      IF8080
 *
 * \brief       Manage the IF8080 peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/*============================================================================*
 *                         Types
 *============================================================================*/

/** \defgroup   IF8080_Exported_Types   Init Params Struct
 *
 * \ingroup     IF8080
 */

/**
 * \brief       IF8080 initialize parameters.
 *
 * \ingroup     IF8080_Exported_Types
 */
typedef struct
{
    uint32_t IF8080_ClockDiv;               /*!< Specifies the IF8080 clock speed. */
    uint32_t IF8080_Mode;                   /*!< Specifies the IF8080 operation mode. */
    uint32_t IF8080_AutoModeDirection;      /*!< Specifies the IF8080 read or write operation. */
    uint32_t IF8080_InitGuardTimeCmd;       /*!< Specifies the init guard time function. */
    uint32_t IF8080_InitGuardTime;          /*!< Specifies the guard time. This parameter is 0~3T of divider clock. */
    uint32_t IF8080_CmdGuardTimeCmd;        /*!< Specifies the command guard time function. */
    uint32_t IF8080_CmdGuardTime;           /*!< Specifies the guard time. This parameter is 0~3T of divider clock. */
    uint32_t IF8080_GuardTimeCmd;           /*!< Specifies the guard time function. The guard time only avaliable for hardware continuously wtite mode*/
    uint32_t IF8080_GuardTime;              /*!< Specifies the guard time. This parameter is 0~3T of divider clock. */
    uint32_t IF8080_WRDelay;                /*!< Specifies the delay time for WR. This parameter is half or one bus clock cycle. */
    uint32_t IF8080_8BitSwap;               /*!< Specifies the FIFO data format. */
    uint32_t IF8080_16BitSwap;              /*!< Specifies the FIFO data format. */
    uint32_t IF8080_TxThr;                  /*!< Specifies the TX FIFO threshold value. This value can be from 0 to 16. */
    uint32_t IF8080_TxDMACmd;               /*!< Specifies the TX DMA status in auto mode. */
    uint32_t IF8080_VsyncCmd;               /*!< Specifies the Vsync signal. */
    uint32_t IF8080_VsyncPolarity;          /*!< Specifies the Vsync trigger polarity. */
} IF8080_InitTypeDef;

/*============================================================================*
 *                         Registers Definitions
 *============================================================================*/

/* Peripheral: IF8080 */
/* Description: IF8080 register defines */

/* Register: CTRL0 -------------------------------------------------------*/
/* Description: Control register 0. Offset: 0x00. Address: 0x40024800. */

/* CTRL0[31]: IF8080_AUTO_WRITE_START. 0x1: Start. 0x0: Stop. */
#define IF8080_AUTO_WRITE_START_POS        (31)
#define IF8080_AUTO_WRITE_START_MSK        ((uint32_t)0x1 << IF8080_AUTO_WRITE_START_POS)
#define IF8080_AUTO_WRITE_START_CLR        (~IF8080_AUTO_WRITE_START_MSK)
/* CTRL0[30]: IF8080_AUTO_READ_START. 0x1: Start. 0x0: Stop. */
#define IF8080_AUTO_READ_START_POS         (30)
#define IF8080_AUTO_READ_START_MSK         ((uint32_t)0x1 << IF8080_AUTO_READ_START_POS)
#define IF8080_AUTO_READ_START_CLR         (~IF8080_AUTO_READ_START_MSK)
/* CTRL0[23:20]: IF8080_INIT_GUARD_TIME. */
#define IF8080_INIT_GUARD_TIME_POS         (20)
#define IF8080_INIT_GUARD_TIME_MSK         (0xF << IF8080_INIT_GUARD_TIME_POS)
#define IF8080_INIT_GUARD_TIME_CLR         (~IF8080_INIT_GUARD_TIME_MSK)
/* CTRL0[19]: IF8080_BYPASS_INIT_GUARD_TIME. 0x1: bypass. 0x0: not bypass. */
#define IF8080_BYPASS_INIT_GUARD_TIME_POS  (19)
#define IF8080_BYPASS_INIT_GUARD_TIME_MSK  (0x1 << IF8080_BYPASS_INIT_GUARD_TIME_POS)
#define IF8080_BYPASS_INIT_GUARD_TIME_CLR  (~IF8080_BYPASS_INIT_GUARD_TIME_MSK)
/* CTRL0[18:17]: IF8080_CMD_GUARD_TIME. */
#define IF8080_CMD_GUARD_TIME_POS          (17)
#define IF8080_CMD_GUARD_TIME_MSK          (0x3 << IF8080_CMD_GUARD_TIME_POS)
#define IF8080_CMD_GUARD_TIME_CLR          (~IF8080_CMD_GUARD_TIME_MSK)
/* CTRL0[16]: IF8080_BYPASS_CMD_GUARD_TIME. 0x1: bypass. 0x0: not bypass. */
#define IF8080_BYPASS_CMD_GUARD_TIME_POS   (16)
#define IF8080_BYPASS_CMD_GUARD_TIME_MSK   (0x1 << IF8080_BYPASS_CMD_GUARD_TIME_POS)
#define IF8080_BYPASS_CMD_GUARD_TIME_CLR   (~IF8080_BYPASS_CMD_GUARD_TIME_MSK)
/* CTRL0[15:14]: IF8080_GUARD_TIME. */
#define IF8080_GUARD_TIME_POS              (14)
#define IF8080_GUARD_TIME_MSK              (0x3 << IF8080_GUARD_TIME_POS)
#define IF8080_GUARD_TIME_CLR              (~IF8080_GUARD_TIME_MSK)
/* CTRL0[11]: IF8080_BYPASS_GUARD_TIME. 0x1: bypass. 0x0: not bypass. */
#define IF8080_BYPASS_GUARD_TIME_POS       (11)
#define IF8080_BYPASS_GUARD_TIME_MSK       (0x1 << IF8080_BYPASS_GUARD_TIME_POS)
#define IF8080_BYPASS_GUARD_TIME_CLR       (~IF8080_BYPASS_GUARD_TIME_MSK)
/* CTRL0[9]: IF8080_8_BIT_SWAP. */
#define IF8080_8_BIT_SWAP_POS              (9)
#define IF8080_8_BIT_SWAP_MSK              (0x1 << IF8080_8_BIT_SWAP_POS)
#define IF8080_8_BIT_SWAP_CLR              (~IF8080_8_BIT_SWAP_MSK)
/* CTRL0[8]: IF8080_16_BIT_SWAP. */
#define IF8080_16_BIT_SWAP_POS             (8)
#define IF8080_16_BIT_SWAP_MSK             (0x1 << IF8080_16_BIT_SWAP_POS)
#define IF8080_16_BIT_SWAP_CLR             (~IF8080_16_BIT_SWAP_MSK)
/* CTRL0[7:4]: IF8080_SPEED_SEL. */
#define IF8080_SPEED_SEL_POS               (4)
#define IF8080_SPEED_SEL_MSK               (0xF << IF8080_SPEED_SEL_POS)
#define IF8080_SPEED_SEL_CLR               (~IF8080_SPEED_SEL_MSK)
/* CTRL0[3]: IF8080_VSYNC_POLARITY. */
#define IF8080_VSYNC_POLARITY_POS          (3)
#define IF8080_VSYNC_POLARITY_MSK          (0x1 << IF8080_VSYNC_POLARITY_POS)
#define IF8080_VSYNC_POLARITY_CLR          (~IF8080_VSYNC_POLARITY_MSK)
/* CTRL0[2]: IF8080_VSYNC_START_EN. */
#define IF8080_VSYNC_START_EN_POS          (2)
#define IF8080_VSYNC_START_EN_MSK          (0x1 << IF8080_VSYNC_START_EN_POS)
#define IF8080_VSYNC_START_EN_CLR          (~IF8080_VSYNC_START_EN_MSK)
/* CTRL0[1]: IF8080_AUTO_MODE_RW_SEL. */
#define IF8080_AUTO_MODE_RW_SEL_POS        (1)
#define IF8080_AUTO_MODE_RW_SEL_MSK        (0x1 << IF8080_AUTO_MODE_RW_SEL_POS)
#define IF8080_AUTO_MODE_RW_SEL_CLR        (~IF8080_AUTO_MODE_RW_SEL_MSK)
/* CTRL0[0]: IF8080_MODE_SEL. */
#define IF8080_MODE_SEL_POS                (0)
#define IF8080_MODE_SEL_MSK                (0x1 << IF8080_MODE_SEL_POS)
#define IF8080_MODE_SEL_CLR                (~IF8080_MODE_SEL_MSK)

/* Register: CTRL1 -------------------------------------------------------*/
/* Description: Control registe 1. Offset: 0x04. Address: 0x40024804. */

/* CTRL1[7:3]: IF8080_TX_DMA_FIFO_THR. Fixed to 0x08. */
#define IF8080_TX_DMA_FIFO_THR_POS         (3)
#define IF8080_TX_DMA_FIFO_THR_MSK         (0x1F << IF8080_TX_DMA_FIFO_THR_POS)
#define IF8080_TX_DMA_FIFO_THR             (0x08 << IF8080_TX_DMA_FIFO_THR_POS)
#define IF8080_TX_DMA_FIFO_THR_CLR         (~IF8080_TX_DMA_FIFO_THR_MSK)
/* CTRL1[0]: IF8080_TX_DMA_EN. */
#define IF8080_TX_DMA_EN_POS               (0)
#define IF8080_TX_DMA_EN_MSK               (0x1 << IF8080_TX_DMA_EN_POS)
#define IF8080_TX_DMA_EN_CLR               (~IF8080_TX_DMA_EN_MSK)

/* Register: IMR ---------------------------------------------------------*/
/* Description: IMR. Offset: 0x08. Address: 0x40024808. */

/* IMR[11]: IF8080_MASK_VSYNC_INT. */
#define IF8080_MASK_VSYNC_INT_POS              (11)
#define IF8080_MASK_VSYNC_INT_MSK              (0x1 << IF8080_MASK_VSYNC_INT_POS)
#define IF8080_MASK_VSYNC_INT_CLR              (~IF8080_MASK_VSYNC_INT_MSK)
/* IMR[10]: IF8080_MASK_RX_AUTO_DONE_INT. */
#define IF8080_MASK_RX_AUTO_DONE_INT_POS       (10)
#define IF8080_MASK_RX_AUTO_DONE_INT_MSK       (0x1 << IF8080_MASK_RX_AUTO_DONE_INT_POS)
#define IF8080_MASK_RX_AUTO_DONE_INT_CLR       (~IF8080_MASK_RX_AUTO_DONE_INT_MSK)
/* IMR[9]: IF8080_MASK_RX_FIFO_OVERFLOW_INT. */
#define IF8080_MASK_RX_FIFO_OVERFLOW_INT_POS   (9)
#define IF8080_MASK_RX_FIFO_OVERFLOW_INT_MSK   (0x1 << IF8080_MASK_RX_FIFO_OVERFLOW_INT_POS)
#define IF8080_MASK_RX_FIFO_OVERFLOW_INT_CLR   (~IF8080_MASK_RX_FIFO_OVERFLOW_INT_MSK)
/* IMR[8]: IF8080_MASK_TX_AUTO_DONE_INT. */
#define IF8080_MASK_TX_AUTO_DONE_INT_POS       (8)
#define IF8080_MASK_TX_AUTO_DONE_INT_MSK       (0x1 << IF8080_MASK_TX_AUTO_DONE_INT_POS)
#define IF8080_MASK_TX_AUTO_DONE_INT_CLR       (~IF8080_MASK_TX_AUTO_DONE_INT_MSK)
/* IMR[7]: IF8080_MASK_TX_FIFO_EMPTY_INT. */
#define IF8080_MASK_TX_FIFO_EMPTY_INT_POS      (7)
#define IF8080_MASK_TX_FIFO_EMPTY_INT_MSK      (0x1 << IF8080_MASK_TX_FIFO_EMPTY_INT_POS)
#define IF8080_MASK_TX_FIFO_EMPTY_INT_CLR      (~IF8080_MASK_TX_FIFO_EMPTY_INT_MSK)
/* IMR[6]: IF8080_MASK_TX_FIFO_OVERFLOW_INT. */
#define IF8080_MASK_TX_FIFO_OVERFLOW_INT_POS   (6)
#define IF8080_MASK_TX_FIFO_OVERFLOW_INT_MSK   (0x1 << IF8080_MASK_TX_FIFO_OVERFLOW_INT_POS)
#define IF8080_MASK_TX_FIFO_OVERFLOW_INT_CLR   (~IF8080_MASK_TX_FIFO_OVERFLOW_INT_MSK)
/* IMR[5]: IF8080_MASK_TX_FIFO_THR_INT. */
#define IF8080_MASK_TX_FIFO_THR_INT_POS        (5)
#define IF8080_MASK_TX_FIFO_THR_INT_MSK        (0x1 << IF8080_MASK_TX_FIFO_THR_INT_POS)
#define IF8080_MASKTX_FIFO_THR_INT_CLR         (~IF8080_MASK_TX_FIFO_THR_INT_MSK)
/* IMR[4:0]: IF8080_TX_FIFO_THRESHOLD. */
#define IF8080_TX_FIFO_THRESHOLD_POS           (0)
#define IF8080_TX_FIFO_THRESHOLD_MSK           (0x1F << IF8080_TX_FIFO_THRESHOLD_POS)
#define IF8080_TX_FIFO_THRESHOLD_CLR           (~IF8080_TX_FIFO_THRESHOLD_MSK)

/* Register: SR ---------------------------------------------------------*/
/* Description: SR. Offset: 0x0C. Address: 0x4002480C. */

/* SR[21]: IF8080_VSYNC_INT_FLAG. */
#define IF8080_VSYNC_INT_FLAG_POS          (21)
#define IF8080_VSYNC_INT_FLAG_MSK          (0x1 << IF8080_VSYNC_INT_FLAG_POS)
#define IF8080_VSYNC_INT_FLAG_CLR          (~IF8080_VSYNC_INT_FLAG_MSK)
/* SR[20]: IF8080_RX_FIFO_EMPTY_FLAG. */
#define IF8080_RX_FIFO_EMPTY_FLAG_POS      (20)
#define IF8080_RX_FIFO_EMPTY_FLAG_MSK      (0x1 << IF8080_RX_FIFO_EMPTY_FLAG_POS)
#define IF8080_RX_FIFO_EMPTY_FLAG_CLR      (~IF8080_RX_FIFO_EMPTY_FLAG_MSK)
/* SR[19]: IF8080_RX_FIFO_FULL_FLAG. */
#define IF8080_RX_FIFO_FULL_FLAG_POS       (19)
#define IF8080_RX_FIFO_FULL_FLAG_MSK       (0x1 << IF8080_RX_FIFO_FULL_FLAG_POS)
#define IF8080_RX_FIFO_FULL_FLAG_CLR       (~IF8080_RX_FIFO_FULL_FLAG_MSK)
/* SR[18]: IF8080_RX_AUTO_DONE_INT. */
#define IF8080_RX_AUTO_DONE_INT_POS        (18)
#define IF8080_RX_AUTO_DONE_INT_MSK        (0x1 << IF8080_RX_AUTO_DONE_INT_POS)
#define IF8080_RX_AUTO_DONE_INT_CLR        (~IF8080_RX_AUTO_DONE_INT_MSK)
/* SR[17]: IF8080_RX_FIFO_OVERFLOW_INT. */
#define IF8080_RX_FIFO_OVERFLOW_INT_POS    (17)
#define IF8080_RX_FIFO_OVERFLOW_INT_MSK    (0x1 << IF8080_RX_FIFO_OVERFLOW_INT_POS)
#define IF8080_RX_FIFO_OVERFLOW_INT_CLR    (~IF8080_RX_FIFO_OVERFLOW_INT_MSK)
/* SR[16:12]: IF8080_RX_FIFO_OFFSET_FLAG. */
#define IF8080_RX_FIFO_OFFSET_FLAG_POS     (12)
#define IF8080_RX_FIFO_OFFSET_FLAG_MSK     (0x1F << IF8080_RX_FIFO_OFFSET_FLAG_POS)
#define IF8080_RX_FIFO_OFFSET_FLAG_CLR     (~IF8080_RX_FIFO_OFFSET_FLAG_MSK)
/* SR[11]: IF8080_TX_AUTO_DONE_INT. */
#define IF8080_TX_AUTO_DONE_INT_POS        (11)
#define IF8080_TX_AUTO_DONE_INT_MSK        (0x1 << IF8080_TX_AUTO_DONE_INT_POS)
#define IF8080_TX_AUTO_DONE_INT_CLR        (~IF8080_TX_AUTO_DONE_INT_MSK)
/* SR[10:6]: IF8080_TX_FIFO_OFFSET_FLAG. */
#define IF8080_TX_FIFO_OFFSET_FLAG_POS     (6)
#define IF8080_TX_FIFO_OFFSET_FLAG_MSK     (0x1F << IF8080_TX_FIFO_OFFSET_FLAG_POS)
#define IF8080_TX_FIFO_OFFSET_FLAG_CLR     (~IF8080_TX_FIFO_OFFSET_FLAG_MSK)
/* SR[5]: IF8080_TX_FIFO_EMPTY_FLAG. */
#define IF8080_TX_FIFO_EMPTY_FLAG_POS      (5)
#define IF8080_TX_FIFO_EMPTY_FLAG_MSK      (0x1 << IF8080_TX_FIFO_EMPTY_FLAG_POS)
#define IF8080_TX_FIFO_EMPTY_FLAG_CLR      (~IF8080_TX_FIFO_EMPTY_FLAG_MSK)
/* SR[4]: IF8080_TX_FIFO_FULL_FLAG. */
#define IF8080_TX_FIFO_FULL_FLAG_POS       (4)
#define IF8080_TX_FIFO_FULL_FLAG_MSK       (0x1 << IF8080_TX_FIFO_FULL_FLAG_POS)
#define IF8080_TX_FIFO_FULL_FLAG_CLR       (~IF8080_TX_FIFO_FULL_FLAG_MSK)
/* SR[3]: IF8080_TX_FIFO_EMPTY_INT. */
#define IF8080_TX_FIFO_EMPTY_INT_POS       (3)
#define IF8080_TX_FIFO_EMPTY_INT_MSK       (0x1 << IF8080_TX_FIFO_EMPTY_INT_POS)
#define IF8080_TX_FIFO_EMPTY_INT_CLR       (~IF8080_TX_FIFO_EMPTY_INT_MSK)
/* SR[2]: IF8080_TX_FIFO_OVERFLOW_INT. */
#define IF8080_TX_FIFO_OVERFLOW_INT_POS    (2)
#define IF8080_TX_FIFO_OVERFLOW_INT_MSK    (0x1 << IF8080_TX_FIFO_OVERFLOW_INT_POS)
#define IF8080_TX_FIFO_OVERFLOW_INT_CLR    (~IF8080_TX_FIFO_OVERFLOW_INT_MSK)
/* SR[1]: IF8080_TX_FIFO_THR_INT. */
#define IF8080_TX_FIFO_THR_INT_POS         (1)
#define IF8080_TX_FIFO_THR_INT_MSK         (0x1 << IF8080_TX_FIFO_THR_INT_POS)
#define IF8080_TX_FIFO_THR_INT_CLR         (~IF8080_TX_FIFO_THR_INT_MSK)

/* Register: ICR ---------------------------------------------------------*/
/* Description: Interrupt Clear register. Offset: 0x10. Address: 0x40024810. */

/* CFG[9]: IF8080_CLR_VSYNC_INT. */
#define IF8080_CLR_VSYNC_INT_POS           (9)
#define IF8080_CLR_VSYNC_INT_MSK           (0x1 << IF8080_CLR_VSYNC_INT_POS)
#define IF8080_CLR_VSYNC_INT_CLR           (~IF8080_CLR_VSYNC_INT_MSK)
/* CFG[8]: IF8080_CLR_RX_AUTO_DONE_INT. */
#define IF8080_CLR_RX_AUTO_DONE_INT_POS    (8)
#define IF8080_CLR_RX_AUTO_DONE_INT_MSK    (0x1 << IF8080_CLR_RX_AUTO_DONE_INT_POS)
#define IF8080_CLR_RX_AUTO_DONE_INT_CLR    (~IF8080_CLR_RX_AUTO_DONE_INT_MSK)
/* CFG[7]: IF8080_CLR_RF_OF_INT. */
#define IF8080_CLR_RF_OF_INT_POS           (7)
#define IF8080_CLR_RF_OF_INT_MSK           (0x1 << IF8080_CLR_RF_OF_INT_POS)
#define IF8080_CLR_RF_OF_INT_CLR           (~IF8080_CLR_RF_OF_INT_MSK)
/* CFG[6]: IF8080_CLR_RX_OUTPUT_CNT_INT. */
#define IF8080_CLR_RX_OUTPUT_CNT_POS       (6)
#define IF8080_CLR_RX_OUTPUT_CNT_MSK       (0x1 << IF8080_CLR_RX_OUTPUT_CNT_POS)
#define IF8080_CLR_RX_OUTPUT_CNT_CLR       (~IF8080_CLR_RX_OUTPUT_CNT_MSK)
/* CFG[5]: IF8080_CLR_TX_AUTO_DONE_INT. */
#define IF8080_CLR_TX_AUTO_DONE_INT_POS    (5)
#define IF8080_CLR_TX_AUTO_DONE_INT_MSK    (0x1 << IF8080_CLR_TX_AUTO_DONE_INT_POS)
#define IF8080_CLR_TX_AUTO_DONE_INT_CLR    (~IF8080_CLR_TX_AUTO_DONE_INT_MSK)
/* CFG[4]: IF8080_CLR_TF_EMPTY_INT. */
#define IF8080_CLR_TF_EMPTY_INT_POS        (4)
#define IF8080_CLR_TF_EMPTY_INT_MSK        (0x1 << IF8080_CLR_TF_EMPTY_INT_POS)
#define IF8080_CLR_TF_EMPTY_INT_CLR        (~IF8080_CLR_TF_EMPTY_INT_MSK)
/* CFG[3]: IF8080_CLR_TF_OF_INT. */
#define IF8080_CLR_TF_OF_INT_POS           (3)
#define IF8080_CLR_TF_OF_INT_MSK           (0x1 << IF8080_CLR_TF_OF_INT_POS)
#define IF8080_CLR_TF_OF_INT_CLR           (~IF8080_CLR_TF_OF_INT_MSK)
/* CFG[2]: IF8080_CLR_TF_THR_INT. */
#define IF8080_CLR_TF_THR_INT_POS          (2)
#define IF8080_CLR_TF_THR_INT_MSK          (0x1 << IF8080_CLR_TF_THR_INT_POS)
#define IF8080_CLR_TF_THR_INT_CLR          (~IF8080_CLR_TF_THR_INT_MSK)
/* CFG[1]: IF8080_CLR_TX_OUTPUT_CNT. */
#define IF8080_CLR_TX_OUTPUT_CNT_POS       (1)
#define IF8080_CLR_TX_OUTPUT_CNT_MSK       (0x1 << IF8080_CLR_TX_OUTPUT_CNT_POS)
#define IF8080_CLR_TX_OUTPUT_CNT_CLR       (~IF8080_CLR_TX_OUTPUT_CNT_MSK)
/* CFG[0]: IF8080_CLR_FIFO. */
#define IF8080_CLR_FIFO_POS                (0)
#define IF8080_CLR_FIFO_MSK                (0x1 << IF8080_CLR_FIFO_POS)
#define IF8080_CLR_FIFO_CLR                (~IF8080_CLR_FIFO_MSK)

/* Register: CFG ---------------------------------------------------------*/
/* Description: Configure register. Offset: 0x14. Address: 0x40024814. */

/* CFG[31:30]: IF8080_WR_TRIGGER_DELAY_CTRL. */
#define IF8080_WR_TRIGGER_DELAY_CTRL_POS   (30)
#define IF8080_WR_TRIGGER_DELAY_CTRL_MSK   (0x3 << IF8080_WR_TRIGGER_DELAY_CTRL_POS)
#define IF8080_WR_TRIGGER_DELAY_CTRL_CLR   (~IF8080_WR_TRIGGER_DELAY_CTRL_MSK)
/* CFG[11]: IF8080_RD_TRIGGER_CTRL. */
#define IF8080_RD_TRIGGER_CTRL_POS         (11)
#define IF8080_RD_TRIGGER_CTRL_MSK         (0x1 << IF8080_RD_TRIGGER_CTRL_POS)
#define IF8080_RD_TRIGGER_CTRL_CLR         (~IF8080_RD_TRIGGER_CTRL_MSK)
/* CFG[10]: IF8080_WR_TRIGGER_CTRL. */
#define IF8080_WR_TRIGGER_CTRL_POS         (10)
#define IF8080_WR_TRIGGER_CTRL_MSK         (0x1 << IF8080_WR_TRIGGER_CTRL_POS)
#define IF8080_WR_TRIGGER_CTRL_CLR         (~IF8080_WR_TRIGGER_CTRL_MSK)
/* CFG[9]: IF8080_CMD_DATA_CTRL. */
#define IF8080_CMD_DATA_CTRL_POS           (9)
#define IF8080_CMD_DATA_CTRL_MSK           (0x1 << IF8080_CMD_DATA_CTRL_POS)
#define IF8080_CMD_DATA_CTRL_CLR           (~IF8080_CMD_DATA_CTRL_MSK)
/* CFG[8]: IF8080_MANUAL_CS_CTRL. */
#define IF8080_MANUAL_CS_CTRL_POS          (8)
#define IF8080_MANUAL_CS_CTRL_MSK          (0x1 << IF8080_MANUAL_CS_CTRL_POS)
#define IF8080_MANUAL_CS_CTRL_CLR          (~IF8080_MANUAL_CS_CTRL_MSK)
/* CFG[3:0]: IF8080_AUTO_MODE_CMD_NUM. */
#define IF8080_AUTO_MODE_CMD_NUM_POS       (0)
#define IF8080_AUTO_MODE_CMD_NUM_MSK       (0xF << IF8080_AUTO_MODE_CMD_NUM_POS)
#define IF8080_AUTO_MODE_CMD_NUM_CLR       (~IF8080_AUTO_MODE_CMD_NUM_MSK)

/* Register: RXDATA -----------------------------------------------------*/
/* Description: RXDATA. Offset: 0x20. Address: 0x40024820. */

/* RXDATA[8]: IF8080_RX_DATA_VALID. */
#define IF8080_RX_DATA_VALID_POS           (8)
#define IF8080_RX_DATA_VALID_MSK           (0x01 << IF8080_RX_DATA_VALID_POS)
#define IF8080_RX_DATA_VALID_CLR           (~IF8080_RX_DATA_VALID_MSK)
/* RXDATA[7:0]: IF8080_RX_DATA_. */
#define IF8080_RX_DATA_POS                 (0)
#define IF8080_RX_DATA_MSK                 (0xFF << IF8080_RX_DATA_POS)
#define IF8080_RX_DATA_CLR                 (~IF8080_RX_DATA_MSK)

/* Register: TX_LEN -----------------------------------------------------*/
/* Description: TX_LEN. Offset: 0x24. Address: 0x40024824. */

/* TX_LEN[32:0]: IF8080_OUTPUT_BYTE_NUM. */
#define IF8080_OUTPUT_BYTE_NUM_POS         (0)
#define IF8080_OUTPUT_BYTE_NUM_MSK         (0xFFFFFFFF << IF8080_OUTPUT_BYTE_NUM_POS)
#define IF8080_OUTPUT_BYTE_NUM_CLR         (~IF8080_OUTPUT_BYTE_NUM_MSK)

/* Register: TX_CNT ----------------------------------------------------*/
/* Description: TX_CNT. Offset: 0x28. Address: 0x40024828. */

/* TX_CNT[32:0]: IF8080_OUTPUT_CNT. */
#define IF8080_OUTPUT_CNT_POS              (0)
#define IF8080_OUTPUT_CNT_MSK              (0xFFFFFFFF << IF8080_OUTPUT_CNT_POS)
#define IF8080_OUTPUT_CNT_CLR              (~IF8080_OUTPUT_CNT_MSK)

/* Register: RX_LEN -----------------------------------------------------*/
/* Description: RX_LEN. Offset: 0x30. Address: 0x40024830. */

/* RX_LEN[32:0]: IF8080_INPUT_BYTE_NUM. */
#define IF8080_INPUT_BYTE_NUM_POS          (0)
#define IF8080_INPUT_BYTE_NUM_MSK          (0xFFFFFFFF << IF8080_INPUT_BYTE_NUM_POS)
#define IF8080_INPUT_BYTE_NUM_CLR          (~IF8080_INPUT_BYTE_NUM_MSK)

/* Register: RX_CNT ----------------------------------------------------*/
/* Description: RX_CNT. Offset: 0x34. Address: 0x40024834. */

/* RX_CNT[32:0]: IF8080_INPUT_CNT. */
#define IF8080_INPUT_CNT_POS               (0)
#define IF8080_INPUT_CNT_MSK               (0xFFFFFFFF << IF8080_INPUT_CNT_POS)
#define IF8080_INPUT_CNT_CLR               (~IF8080_INPUT_CNT_MSK)

/* Register: CR ----------------------------------------------------*/
/* Description: CR. Offset: 0x94. Address: 0x40024894. */

/* CR[31]: IF8080_GDMA_G1_WP. */
#define IF8080_GDMA_G1_WP_POS              (31)
#define IF8080_GDMA_G1_WP_MSK              ((uint32_t)0x1 << IF8080_GDMA_G1_WP_POS)
#define IF8080_GDMA_G1_WP_CLR              (~IF8080_GDMA_G1_WP_MSK)
/* CR[30]: IF8080_GDMA_G2_WP. */
#define IF8080_GDMA_G2_WP_POS              (30)
#define IF8080_GDMA_G2_WP_MSK              (0x1 << IF8080_GDMA_G2_WP_POS)
#define IF8080_GDMA_G2_WP_CLR              (~IF8080_GDMA_G2_WP_MSK)
/* CR[29]: IF8080_GDMA_LOAD_CNT_CLR. */
#define IF8080_GDMA_LOAD_CNT_CLR_POS       (29)
#define IF8080_GDMA_LOAD_CNT_CLR_MSK       (0x1 << IF8080_GDMA_LOAD_CNT_CLR_POS)
#define IF8080_GDMA_LOAD_CNT_CLR_CLR       (~IF8080_GDMA_LOAD_CNT_CLR_MSK)
/* CR[21:11]: IF8080_GDMA_LOOP_NUM. */
#define IF8080_GDMA_LOOP_NUM_POS           (11)
#define IF8080_GDMA_LOOP_NUM_MSK           (0x7FF << IF8080_GDMA_LOOP_NUM_POS)
#define IF8080_GDMA_LOOP_NUM_CLR           (~IF8080_GDMA_LOOP_NUM_MSK)
/* CR[10:0]: IF8080_GDMA_LOAD_CNT. */
#define IF8080_GDMA_LOAD_CNT_POS           (0)
#define IF8080_GDMA_LOAD_CNT_MSK           (0x7FF << IF8080_GDMA_LOAD_CNT_POS)
#define IF8080_GDMA_LOAD_CNT_CLR           (~IF8080_GDMA_LOAD_CNT_MSK)

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    IF8080_Exported_Constants Macro Definitions
 *
 * \ingroup     IF8080
 */

/**
 * \defgroup    IF8080_Pin_Group IF8080 Pin Group
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_PinGroup_DISABLE                        ((uint32_t)0x00)
#define IF8080_PinGroup_1                              ((uint32_t)0x02 << 28)
#define IF8080_PinGroup_2                              ((uint32_t)0x01 << 28)
/** \} */

#define IS_IF8080_PINGROUP_IDX(IDX) (((IDX) == IF8080_PinGroup_DISABLE) || ((IDX) == IF8080_PinGroup_1) || \
                                     ((IDX) == IF8080_PinGroup_2))

/**
 * \defgroup    IF8080_ClockDiv IF8080 clock div
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_CLOCK_DIV_2                             (((uint32_t)0x1 << IF8080_SPEED_SEL_POS))
#define IF8080_CLOCK_DIV_3                             (((uint32_t)0x2 << IF8080_SPEED_SEL_POS))
#define IF8080_CLOCK_DIV_4                             (((uint32_t)0x3 << IF8080_SPEED_SEL_POS))
#define IF8080_CLOCK_DIV_5                             (((uint32_t)0x4 << IF8080_SPEED_SEL_POS))
#define IF8080_CLOCK_DIV_6                             (((uint32_t)0x5 << IF8080_SPEED_SEL_POS))
#define IF8080_CLOCK_DIV_7                             (((uint32_t)0x6 << IF8080_SPEED_SEL_POS))
#define IF8080_CLOCK_DIV_8                             (((uint32_t)0x7 << IF8080_SPEED_SEL_POS))
/** \} */

#define IS_IF8080_CLOCK_DIV(DIV) (((DIV) == IF8080_CLOCK_DIV_2) || ((DIV) == IF8080_CLOCK_DIV_3) || \
                                  ((DIV) == IF8080_CLOCK_DIV_4) || ((DIV) == IF8080_CLOCK_DIV_5) || \
                                  ((DIV) == IF8080_CLOCK_DIV_6) || ((DIV) == IF8080_CLOCK_DIV_7) || \
                                  ((DIV) == IF8080_CLOCK_DIV_8))

/**
 * \defgroup    IF8080_Mode IF8080 Mode
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_MODE_AUTO                               (0)
#define IF8080_MODE_MANUAL                             (IF8080_MODE_SEL_MSK)
/** \} */

#define IS_IF8080_MODE(MODE) (((MODE) == IF8080_MODE_AUTO) || ((MODE) == IF8080_MODE_MANUAL))

/**
 * \defgroup    IF8080_Auto_Mode_Direction IF8080 Auto Mode direction
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_Auto_Mode_Direction_READ                (IF8080_AUTO_MODE_RW_SEL_MSK)
#define IF8080_Auto_Mode_Direction_WRITE               (0)
/** \} */

#define IS_IF8080_Auto_Mode_DIR(DIR) (((DIR) == IF8080_Auto_Mode_Direction_READ) || ((DIR) == IF8080_Auto_Mode_Direction_WRITE))

/**
 * \defgroup    IF8080_InitGuardTime_Cmd IF8080 Init Guard Time Cmd
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_INIT_GUARD_TIME_ENABLE                   (0)
#define IF8080_INIT_GUARD_TIME_DISABLE                  (IF8080_BYPASS_INIT_GUARD_TIME_MSK)
/** \} */

#define IS_IF8080_INIT_GUARD_TIME_CMD(CMD) (((CMD) == IF8080_INIT_GUARD_TIME_ENABLE) || ((CMD) == IF8080_INIT_GUARD_TIME_DISABLE))

/**
 * \defgroup    IF8080_InitGuardTime IF8080 Init Guard Time
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_INIT_GUARD_TIME_1T                       (0)
#define IF8080_INIT_GUARD_TIME_2T                       (((uint32_t)0x1 << IF8080_INIT_GUARD_TIME_POS))
#define IF8080_INIT_GUARD_TIME_3T                       (((uint32_t)0x2 << IF8080_INIT_GUARD_TIME_POS))
#define IF8080_INIT_GUARD_TIME_4T                       (((uint32_t)0x3 << IF8080_INIT_GUARD_TIME_POS))
/** \} */

#define IS_IF8080_INIT_GUARD_TIME(TIME) (((TIME) == IF8080_INIT_GUARD_TIME_1T) || ((TIME) == IF8080_INIT_GUARD_TIME_2T) || \
                                         ((TIME) == IF8080_INIT_GUARD_TIME_3T) || ((TIME) == IF8080_INIT_GUARD_TIME_4T))

/**
 * \defgroup    IF8080_CmdGuardTime_Cmd IF8080 Cmd Guard Time Cmd
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_CMD_GUARD_TIME_ENABLE                   (0)
#define IF8080_CMD_GUARD_TIME_DISABLE                  (IF8080_BYPASS_CMD_GUARD_TIME_MSK)
/** \} */

#define IS_IF8080_CMD_GUARD_TIME_CMD(CMD) (((CMD) == IF8080_CMD_GUARD_TIME_ENABLE) || ((CMD) == IF8080_CMD_GUARD_TIME_DISABLE))

/**
 * \defgroup    IF8080_CmdGuardTime IF8080 Cmd Guard Time
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_CMD_GUARD_TIME_1T                       (0)
#define IF8080_CMD_GUARD_TIME_2T                       (((uint32_t)0x1 << IF8080_CMD_GUARD_TIME_POS))
#define IF8080_CMD_GUARD_TIME_3T                       (((uint32_t)0x2 << IF8080_CMD_GUARD_TIME_POS))
#define IF8080_CMD_GUARD_TIME_4T                       (IF8080_CMD_GUARD_TIME_MSK)
/** \} */

#define IS_IF8080_CMD_GUARD_TIME(TIME) (((TIME) == IF8080_CMD_GUARD_TIME_1T) || ((TIME) == IF8080_CMD_GUARD_TIME_2T) || \
                                        ((TIME) == IF8080_CMD_GUARD_TIME_3T) || ((TIME) == IF8080_CMD_GUARD_TIME_4T))

/**
 * \defgroup   IF8080_GuardTime_Cmd IF8080 Guard Time Cmd
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_GUARD_TIME_ENABLE                       (0)
#define IF8080_GUARD_TIME_DISABLE                      (IF8080_BYPASS_GUARD_TIME_MSK)
/** \} */

#define IS_IF8080_GUARD_TIME_CMD(CMD) (((CMD) == IF8080_GUARD_TIME_ENABLE) || ((CMD) == IF8080_GUARD_TIME_DISABLE))

/**
 * \defgroup    IF8080_GuardTime IF8080 Guard Time
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_GUARD_TIME_1T                           (0)
#define IF8080_GUARD_TIME_2T                           (((uint32_t)0x1 << IF8080_GUARD_TIME_POS))
#define IF8080_GUARD_TIME_3T                           (((uint32_t)0x2 << IF8080_GUARD_TIME_POS))
#define IF8080_GUARD_TIME_4T                           (IF8080_GUARD_TIME_MSK)
/** \} */

#define IS_IF8080_GUARD_TIME(TIME) (((TIME) == IF8080_GUARD_TIME_1T) || ((TIME) == IF8080_GUARD_TIME_2T) || \
                                    ((TIME) == IF8080_GUARD_TIME_3T) || ((TIME) == IF8080_GUARD_TIME_4T))

/**
 * \defgroup    IF8080_WRDelay IF8080 WR Delay
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_WR_DELAY_TIME_HALF_BUS_CLK_CYCLE        (((uint32_t)0x1 << IF8080_WR_TRIGGER_DELAY_CTRL_POS))
#define IF8080_WR_DELAY_TIME_ONE_BUS_CLK_CYCLE         (((uint32_t)0x2 << IF8080_WR_TRIGGER_DELAY_CTRL_POS))
/** \} */

#define IS_IF8080_WR_DELAY_TIME(TIME) (((TIME) == IF8080_WR_DELAY_TIME_HALF_BUS_CLK_CYCLE) || \
                                       ((TIME) == IF8080_WR_DELAY_TIME_ONE_BUS_CLK_CYCLE))

/**
 * \defgroup    IF8080_8BitSwap IF8080 8-bit Swap
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_8BitSwap_DISABLE                        (0)
#define IF8080_8BitSwap_ENABLE                         (IF8080_8_BIT_SWAP_MSK)
/** \} */

#define IS_IF8080_8BitSwap_CMD(CMD) (((CMD) == IF8080_8BitSwap_DISABLE) || ((CMD) == IF8080_8BitSwap_ENABLE))

/**
 * \defgroup    IF8080_16BitSwap IF8080 16-bit Swap
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_16BitSwap_DISABLE                       (0)
#define IF8080_16BitSwap_ENABLE                        (IF8080_16_BIT_SWAP_MSK)
/** \} */

#define IS_IF8080_16BitSwap_CMD(CMD) (((CMD) == IF8080_16BitSwap_DISABLE) || ((CMD) == IF8080_16BitSwap_ENABLE))

/**
 * \defgroup    IF8080_TxDMACmd IF8080 Tx DMA Cmd
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_TX_DMA_ENABLE                           (IF8080_TX_DMA_EN_MSK)
#define IF8080_TX_DMA_DISABLE                          (0)
/** \} */

#define IS_IF8080_TX_DMA_CMD(CMD) (((CMD) == IF8080_TX_DMA_ENABLE) || ((CMD) == IF8080_TX_DMA_DISABLE))

/**
 * \defgroup    IF8080_VsyncCmd IF8080 Vsync Cmd
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_VSYNC_ENABLE                            (IF8080_VSYNC_START_EN_MSK)
#define IF8080_VSYNC_DISABLE                           (0)
/** \} */

#define IS_IF8080_VSYNC_CMD(CMD) (((CMD) == IF8080_VSYNC_ENABLE) || ((CMD) == IF8080_VSYNC_DISABLE))

/**
 * \defgroup    IF8080_VsyncPolarity IF8080 Vsync Polarity
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_VSYNC_POLARITY_RISING                   (0)
#define IF8080_VSYNC_POLARITY_FALLING                  (IF8080_VSYNC_POLARITY_MSK)
/** \} */

#define IS_IF8080_VSYNC_POLARITY(POLARITY) (((POLARITY) == IF8080_VSYNC_POLARITY_RISING) || \
                                            ((POLARITY) == IF8080_VSYNC_POLARITY_FALLING))

/**
 * \defgroup    IF8080_Flag_Definition IF8080 Flag Definition
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_FLAG_RF_EMPTY                           (IF8080_RX_FIFO_EMPTY_FLAG_MSK)
#define IF8080_FLAG_RF_FULL                            (IF8080_RX_FIFO_FULL_FLAG_MSK)
#define IF8080_FLAG_TF_EMPTY                           (IF8080_TX_FIFO_EMPTY_FLAG_MSK)
#define IF8080_FLAG_TF_FULL                            (IF8080_TX_FIFO_FULL_FLAG_MSK)
/** \} */

#define IS_IF8080_FLAG_CONFIG(CONFIG)   (((CONFIG) == IF8080_FLAG_RF_EMPTY) || \
                                         ((CONFIG) == IF8080_FLAG_RF_FULL)  || \
                                         ((CONFIG) == IF8080_FLAG_TF_EMPTY) || \
                                         ((CONFIG) == IF8080_FLAG_TF_FULL))

/**
 * \defgroup    IF8080_Interrupts_Mask_Definition IF8080 Interrupts Mask Definition
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_INT_VSYNC_MSK                           (IF8080_MASK_VSYNC_INT_MSK)
#define IF8080_INT_RX_AUTO_DONE_MSK                    (IF8080_MASK_RX_AUTO_DONE_INT_MSK)
#define IF8080_INT_RF_OF_MSK                           (IF8080_MASK_RX_FIFO_OVERFLOW_INT_MSK)
#define IF8080_INT_TX_AUTO_DONE_MSK                    (IF8080_MASK_TX_AUTO_DONE_INT_MSK)
#define IF8080_INT_TF_EMPTY_MSK                        (IF8080_MASK_TX_FIFO_EMPTY_INT_MSK)
#define IF8080_INT_TF_OF_MSK                           (IF8080_MASK_TX_FIFO_OVERFLOW_INT_MSK)
#define IF8080_INT_TF_LEVEL_MSK                        (IF8080_MASK_TX_FIFO_THR_INT_MSK)
/** \} */

#define IS_IF8080_INT_MSK_CONFIG(CONFIG)   (((CONFIG) == IF8080_INT_VSYNC_MSK)            || \
                                            ((CONFIG) == IF8080_INT_RX_AUTO_DONE_MSK)     || \
                                            ((CONFIG) == IF8080_INT_RF_OF_MSK)            || \
                                            ((CONFIG) == IF8080_INT_TX_AUTO_DONE_MSK)     || \
                                            ((CONFIG) == IF8080_INT_TF_EMPTY_MSK)         || \
                                            ((CONFIG) == IF8080_INT_TF_OF_MSK)            || \
                                            ((CONFIG) == IF8080_INT_TF_LEVEL_MSK))

/**
 * \defgroup    IF8080_Interrupts_Definition IF8080 Interrupts Definition
 * \{
 * \ingroup     IF8080_Exported_Constants
 */

#define IF8080_INT_SR_VSYNC                            (IF8080_VSYNC_INT_FLAG_MSK)
#define IF8080_INT_SR_RX_AUTO_DONE                     (IF8080_RX_AUTO_DONE_INT_MSK)
#define IF8080_INT_SR_RF_OF                            (IF8080_RX_FIFO_OVERFLOW_INT_MSK)
#define IF8080_INT_SR_TX_AUTO_DONE                     (IF8080_TX_AUTO_DONE_INT_MSK)
#define IF8080_INT_SR_TF_EMPTY                         (IF8080_TX_FIFO_EMPTY_INT_MSK)
#define IF8080_INT_SR_TF_OF                            (IF8080_TX_FIFO_OVERFLOW_INT_MSK)
#define IF8080_INT_SR_TF_LEVEL                         (IF8080_TX_FIFO_THR_INT_MSK)
/** \} */

#define IS_IF8080_INT_CONFIG(CONFIG)       (((CONFIG) == IF8080_INT_SR_VSYNC)         || \
                                            ((CONFIG) == IF8080_INT_SR_RX_AUTO_DONE)  || \
                                            ((CONFIG) == IF8080_INT_SR_RF_OF)         || \
                                            ((CONFIG) == IF8080_INT_SR_TX_AUTO_DONE)  || \
                                            ((CONFIG) == IF8080_INT_SR_TF_EMPTY)      || \
                                            ((CONFIG) == IF8080_INT_SR_TF_OF)         || \
                                            ((CONFIG) == IF8080_INT_SR_TF_LEVEL))

/**
 * \defgroup    IF8080_Interrupts_Clear_Status IF8080 Interrupts Clear Status
 * \{
 * \ingroup     IF8080_Exported_Constants
 */
#define IF8080_INT_RX_AUTO_DONE_CLR                    (IF8080_CLR_RX_AUTO_DONE_INT_MSK)
#define IF8080_INT_RF_OF_CLR                           (IF8080_CLR_RF_OF_INT_MSK)
#define IF8080_INT_TX_AUTO_DONE_CLR                    (IF8080_CLR_TX_AUTO_DONE_INT_MSK)
#define IF8080_INT_TF_EMPTY_CLR                        (IF8080_CLR_TF_EMPTY_INT_MSK)
#define IF8080_INT_TF_OF_CLR                           (IF8080_CLR_TF_OF_INT_MSK)
#define IF8080_INT_TF_LEVEL_CLR                        (IF8080_CLR_TF_THR_INT_MSK)
/** \} */

#define IS_IF8080_INT_CLEAR(INT)           (((INT) == IF8080_INT_RX_AUTO_DONE_CLR) || \
                                            ((INT) == IF8080_INT_RF_OF_CLR) || \
                                            ((INT) == IF8080_INT_TX_AUTO_DONE_CLR) || \
                                            ((INT) == IF8080_INT_TF_EMPTY_CLR) || \
                                            ((INT) == IF8080_INT_TF_OF_CLR) || \
                                            ((INT) == IF8080_INT_TF_LEVEL_CLR))

/** \} */ /* End of group IF8080_Exported_Constants */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    IF8080_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     IF8080
 */

/**
 * \brief   Deinitializes the IF8080 peripheral registers to their default values.
 * \param   None.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_if8080_init(void)
 * {
 *     IF8080_DeInit();
 * }
 * \endcode
 */
void IF8080_DeInit(void);

/**
 * \brief   Select the IF8080 output pin group.
 * \param[in] IF8080_PinGroupType:
 *      This parameter can be one of the following values:
 *      \arg IF8080_PinGroup_DISABLE : disable IF8080 interface
 *      \arg IF8080_PinGroup_1 : CS(P3_3), RD(P3_2), DCX(P3_4), WR(P3_5),
 *           D0(P0_2), D1(P0_4), D2(P1_3), D3(P1_4), D4(P4_0), D5(P4_1), D6(P4_2), D7(P4_3)
 *      \arg IF8080_PinGroup_2 : CS(P3_3), DCX(P3_4), WR(P3_2), RD(P2_0)
 *           D0(P3_5), D1(P0_1), D2(P0_2), D3(P0_4), D4(P4_0), D5(P4_1), D6(P4_2), D7(P4_3)
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_if8080_init(void)
 * {
 *     IF8080_PinGroupConfig(IF8080_PinGroup_1);
 * }
 * \endcode
 */
void IF8080_PinGroupConfig(uint32_t IF8080_PinGroupType);

/**
 * \brief   Initializes the IF8080 peripheral according to the specified
 *          parameters in the IF8080_InitStruct
 * \param[in] IF8080_InitStruct: Pointer to a IF8080_InitTypeDef structure that
 *            contains the configuration information for the specified IF8080 peripheral
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_if8080_init(void)
 * {
 *     //close clock
 *     IF8080_DeInit();
 *     RCC_PeriphClockCmd(APBPeriph_IF8080, APBPeriph_IF8080_CLOCK, ENABLE);
 *
 *     IF8080_PinGroupConfig(IF8080_PinGroup_1);
 *
 *     IF8080_InitTypeDef IF8080_InitStruct;
 *     IF8080_StructInit(&IF8080_InitStruct);
 *
 *     IF8080_InitStruct.IF8080_ClockDiv          = IF8080_CLOCK_DIV_5;
 *     IF8080_InitStruct.IF8080_Mode              = IF8080_MODE_MANUAL;
 *     IF8080_InitStruct.IF8080_AutoModeDirection = IF8080_Auto_Mode_Direction_WRITE;
 *     IF8080_InitStruct.IF8080_GuardTimeCmd      = IF8080_GUARD_TIME_DISABLE;
 *     IF8080_InitStruct.IF8080_GuardTime         = IF8080_GUARD_TIME_2T;
 *     IF8080_InitStruct.IF8080_8BitSwap          = IF8080_8BitSwap_DISABLE;
 *     IF8080_InitStruct.IF8080_16BitSwap         = IF8080_16BitSwap_DISABLE;
 *     IF8080_InitStruct.IF8080_TxThr             = 10;
 *     IF8080_InitStruct.IF8080_TxDMACmd          = IF8080_TX_DMA_DISABLE;
 *     IF8080_InitStruct.IF8080_VsyncCmd          = IF8080_VSYNC_DISABLE;
 *     IF8080_InitStruct.IF8080_VsyncPolarity     = IF8080_VSYNC_POLARITY_FALLING;
 *     IF8080_Init(&IF8080_InitStruct);
 * }
 * \endcode
 */
void IF8080_Init(IF8080_InitTypeDef *IF8080_InitStruct);

/**
 * \brief  Fills each IF8080_InitStruct member with its default value.
 * \param[in] IF8080_InitStruct: Pointer to an IF8080_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_if8080_init(void)
 * {
 *     IF8080_DeInit();
 *     RCC_PeriphClockCmd(APBPeriph_IF8080, APBPeriph_IF8080_CLOCK, ENABLE);
 *
 *     IF8080_PinGroupConfig(IF8080_PinGroup_1);
 *
 *     IF8080_InitTypeDef IF8080_InitStruct;
 *     IF8080_StructInit(&IF8080_InitStruct);
 *
 *     IF8080_InitStruct.IF8080_ClockDiv          = IF8080_CLOCK_DIV_5;
 *     IF8080_InitStruct.IF8080_Mode              = IF8080_MODE_MANUAL;
 *     IF8080_InitStruct.IF8080_AutoModeDirection = IF8080_Auto_Mode_Direction_WRITE;
 *     IF8080_InitStruct.IF8080_GuardTimeCmd      = IF8080_GUARD_TIME_DISABLE;
 *     IF8080_InitStruct.IF8080_GuardTime         = IF8080_GUARD_TIME_2T;
 *     IF8080_InitStruct.IF8080_8BitSwap          = IF8080_8BitSwap_DISABLE;
 *     IF8080_InitStruct.IF8080_16BitSwap         = IF8080_16BitSwap_DISABLE;
 *     IF8080_InitStruct.IF8080_TxThr             = 10;
 *     IF8080_InitStruct.IF8080_TxDMACmd          = IF8080_TX_DMA_DISABLE;
 *     IF8080_InitStruct.IF8080_VsyncCmd          = IF8080_VSYNC_DISABLE;
 *     IF8080_InitStruct.IF8080_VsyncPolarity     = IF8080_VSYNC_POLARITY_FALLING;
 *     IF8080_Init(&IF8080_InitStruct);
 * }
 * \endcode
 */
void IF8080_StructInit(IF8080_InitTypeDef *IF8080_InitStruct);

/**
 * \brief  Enable or disable the selected IF8080 operation in auto mode.
 * \param[in]  lcd_dir: The IF8080 read or write operation.
 * \param[in]  NewState: New state of the operation mode.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_AutoModeCmd(IF8080_Auto_Mode_Direction_WRITE, ENABLE);
 * }
 * \endcode
 */
void IF8080_AutoModeCmd(uint32_t IF8080_Direction, FunctionalState NewState);

/**
 * \brief  Send command in manual mode.
 * \param[in] cmd: Command which to be sent.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint8_t cmd =0x01;//Command which to be sent.
 *     IF8080_SendCommand(cmd);
 * }
 * \endcode
 */
void IF8080_SendCommand(uint8_t cmd);

/**
 * \brief  Send data in manual mode.
 * \param[in] pBuf: Buffer address to be sent.
 * \param[in] len: Data length.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint8_t data[10] ={0};//Data to be sent.
 *     IF8080_SendCommand(data, 10);
 * }
 * \endcode
 */
void IF8080_SendData(uint8_t *pBuf, uint32_t len);

/**
 * \brief  Receive data in manual mode.
 * \param[in] pBuf: Buffer address to be received.
 * \param[in] len: Data length.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint8_t data[10] ={0};//Data buffer.
 *     IF8080_ReceiveData(data, 10);
 * }
 * \endcode
 */
void IF8080_ReceiveData(uint8_t *pBuf, uint32_t len);

/**
 * \brief  Send command and data buffer in manual mode.
 * \param[in]  cmd: Command which to be sent.
 * \param[in]  pBuf: Buffer address to be sent.
 * \param[in]  len: Data length.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint8_t cmd =0x01;//Command which to be sent.
 *     uint8_t data[10] ={0};//Data to be sent.
 *     IF8080_Write(cmd, data, 10);
 * }
 * \endcode
 */
void IF8080_Write(uint8_t cmd, uint8_t *pBuf, uint32_t len);

/**
 * \brief   Send command and read data buffer in manual mode.
 * \param[in]  cmd: Command which to be sent.
 * \param[in]  pBuf: Buffer address to be sent.
 * \param[in]  len:  Data length.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint8_t cmd =0x01;//Command which to be sent.
 *     uint8_t data[10] ={0};//Data buffer.
 *     IF8080_Read(cmd, data, 10);
 * }
 * \endcode
 */
void IF8080_Read(uint8_t cmd, uint8_t *pBuf, uint32_t len);

/**
 * \brief  Configure command sequences in auto mode.
 * \param[in]  pCmdBuf: Buffer address which store command sequence.
 * \param[in]  len: Command length.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint8_t cmd[10] ={0};//Command buffer.
 *     IF8080_SetCmdSequence(cmd, 10);
 * }
 * \endcode
 */
FlagStatus IF8080_SetCmdSequence(uint8_t *pCmdBuf, uint8_t len);

/**
 * \brief  Mask or unmask the specified IF8080 interrupts.
 * \param[in]  IF8080_INT_MSK: Specifies the IF8080 interrupts sources to be mask or unmask.
 *      This parameter can be the following values:
 *     \arg IF8080_INT_TF_EMPTY_MSK: Mask TX FIFO empty interrupt.
 *     \arg IF8080_INT_TF_OF_MSK: Mask TX FIFO overflow interrupt.
 *     \arg IF8080_INT_TF_LEVEL_MSK: Mask TX FIFO threshold interrupt.
 * \param[in]  NewState: New state of the specified IF8080 interrupts.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_MaskINTConfig(IF8080_INT_TF_EMPTY_MSK, ENABLE);
 * }
 * \endcode
 */
void IF8080_MaskINTConfig(uint32_t IF8080_INT_MSK, FunctionalState NewState);

/**
 * \brief  Get the specified IF8080 interrupt status.
 * \param[in]  IF8080_INT: The specified IF8080 interrupts.
 *      This parameter can be one of the following values:
 *      \arg IF8080_INT_SR_AUTO_DONE: Auto done interrupt.
 *      \arg IF8080_INT_SR_TF_EMPTY: TX FIFO empty interrupt.
 *      \arg IF8080_INT_SR_TF_OF: TX FIFO overflow interrupt.
 *      \arg IF8080_INT_SR_TF_LEVEL: TX FIFO threshold interrupt.
 * \retval The new state of IF8080_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     ITStatus int_status = IF8080_GetINTStatus(IF8080_INT_SR_TF_EMPTY);
 * }
 * \endcode
 */
ITStatus IF8080_GetINTStatus(uint32_t IF8080_INT);

/**
 * \brief  Get the specified IF8080 flag status.
 * \param[in]  IF8080_INT: the specified IF8080 flag.
 *      This parameter can be one of the following values:
 *     \arg IF8080_FLAG_TF_EMPTY:  FIFO empty flag.
 *     \arg IF8080_FLAG_TF_FULL:  FIFO full flag.
 * \retval The new state of IF8080_FLAG (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     FlagStatus flag_status = IF8080_GetFlagStatus(IF8080_FLAG_TF_EMPTY);
 * }
 * \endcode
 */
FlagStatus IF8080_GetFlagStatus(uint32_t IF8080_FLAG);

/**
 * \brief  Configure IF8080 multi-block LLI parameters.
 * \param[in]  IF8080_LLIGroup1: Pointer to a IF8080_GDMALLITypeDef structure that
 *             contains the group 1 configuration information for the If8080 GDMA peripheral.
 * \param[in]  IF8080_LLIGroup2: Pointer to a IF8080_GDMALLITypeDef structure that
 *             contains the group 2 configuration information for the If8080 GDMA peripheral.
 * \param[in]  IF8080_LLP_Group1Offset: Pointer to a IF8080_GDMALLIOFTTypeDef structure that
 *             contains the group 1 offset configuration information for the GDMA transmission.
 * \param[in]  IF8080_LLP_Group2Offset: Pointer to a IF8080_GDMALLIOFTTypeDef structure that
 *             contains the group 2 offset configuration information for the GDMA transmission.
 * \param[in]  LLI_loop_num: Number of LLI.
 * \param[in]  addr: Address of last LLI.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * \endcode
 */
void IF8080_GDMALLIConfig(IF8080_GDMALLITypeDef *IF8080_LLPGroup1,
                          IF8080_GDMALLITypeDef *IF8080_LLPGroup2,
                          IF8080_GDMALLIOFTTypeDef *IF8080_LLPGroup1Offset,
                          IF8080_GDMALLIOFTTypeDef *IF8080_LLPGroup2Offset,
                          uint16_t LLI_loop_num,
                          uint32_t last_LLI_addr);

/**
 * \brief  Dynamic switch IF8080 operation mode.
 * \param[in]  mode: Selected IF8080 operation mode.
 *      This parameter can be the following values:
 *     \arg IF8080_MODE_AUTO: Automation mode.
 *     \arg IF8080_MODE_MANUAL: Manual mode.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_if8080_init(void)
 * {
 *     IF8080_SwitchMode(IF8080_MODE_MANUAL);
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_SwitchMode(uint32_t mode)
{
    IF8080->CTRL0 &= IF8080_MODE_SEL_CLR;
    IF8080->CTRL0 |= mode;
}

/**
 * \brief  Enable or disable GDMA for IF8080 transmission.
 * \param[in]  NewState: New state of GDMA.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_GDMACmd(ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_GDMACmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        IF8080->CTRL1 |= IF8080_TX_DMA_EN_MSK;
    }
    else
    {
        IF8080->IMR &= IF8080_TX_DMA_EN_CLR;
    }
}

/**
 * \brief  Set CS singal in manual mode.
 * \param  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_SetCS();
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_SetCS(void)
{
    IF8080->CFG |= IF8080_MANUAL_CS_CTRL_MSK;
}

/**
 * \brief  Reset CS singal in manual mode.
 * \param  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_ResetCS();
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_ResetCS(void)
{
    IF8080->CFG &= IF8080_MANUAL_CS_CTRL_CLR;
}

/**
 * \brief   Clear the IF8080 interrupt pending bits.
 * \param[in] IF8080_INT: specifies the interrupt pending bit to clear.
 *      This parameter can be any combination of the following values:
 *      \arg IF8080_INT_RX_AUTO_DONE_CLR: Clear RX auto done interrupt.
 *      \arg IF8080_INT_RF_OF_CLR: Clear RX FIFO overflow interrupt.
 *      \arg IF8080_INT_TX_AUTO_DONE_CLR: Clear TX auto done interrupt.
 *      \arg IF8080_INT_TF_EMPTY_CLR: Clear TX FIFO empty interrupt.
 *      \arg IF8080_INT_TF_OF_CLR: Clear TX FIFO overflow interrupt.
 *      \arg IF8080_INT_TF_LEVEL_CLR: Clear TX FIFO threshold interrupt.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_ClearINTPendingBit(IF8080_INT_RX_AUTO_DONE_CLR);
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_ClearINTPendingBit(uint32_t IF8080_CLEAR_INT)
{
    /* Check the parameters */
    assert_param(IS_IF8080_INT_CLEAR(IF8080_CLEAR_INT));

    IF8080->ICR = IF8080_CLEAR_INT;
}

/**
 * \brief   Set IF8080 output data length whose unit is byte.
 * \param[in]   len : Length of read data which can be 0 to 0x1FFFF.
 * \return  Value set in.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t data_len = IF8080_SetTxDataLen(10);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_SetTxDataLen(uint32_t len)
{
    return IF8080->TX_LEN = len & IF8080_OUTPUT_BYTE_NUM_MSK;
}

/**
 * \brief  Get IF8080 output data length whose unit is byte.
 * \param  None.
 * \return Length of read data which can be 0 to 0xFFFFFFFF.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t data_len = IF8080_GetTxDataLen();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_GetTxDataLen(void)
{
    return (IF8080->TX_LEN & IF8080_OUTPUT_BYTE_NUM_MSK);
}

/**
 * \brief  Get IF8080 output data counter whose unit is byte.
 * \param[in]  None.
 * \return Length of counter which can be 0 to 0xFFFFFFFF.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t tx_counter = IF8080_GetTxCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_GetTxCounter(void)
{
    return (IF8080->TX_CNT & IF8080_OUTPUT_CNT_MSK);
}

/**
 * \brief   Set IF8080 input data length whose unit is byte.
 * \param[in]  len : Length of read data which can be 0 to 0x1FFFF.
 * \return  Value set in.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t data_len = IF8080_SetRxDataLen(10);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_SetRxDataLen(uint32_t len)
{
    return IF8080->RX_LEN = len & IF8080_INPUT_BYTE_NUM_MSK;
}

/**
 * \brief  Get IF8080 input data length whose unit is byte.
 * \param[in]  None.
 * \return Length of read data which can be 0 to 0xFFFFFFFF.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t data_len = IF8080_GetRxDataLen();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_GetRxDataLen(void)
{
    return (IF8080->RX_LEN & IF8080_INPUT_BYTE_NUM_MSK);
}

/**
 * \brief  Get IF8080 input data counter whose unit is byte.
 * \param  None.
 * \return Length of counter which can be 0 to 0xFFFFFFFF.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t rx_counter = IF8080_GetRxCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_GetRxCounter(void)
{
    return (IF8080->RX_CNT & IF8080_INPUT_CNT_MSK);
}

/**
 * \brief  Clear IF8080 output data counter value.
 * \param  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_ClearTxCounter();
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_ClearTxCounter(void)
{
    IF8080->ICR = IF8080_CLR_TX_OUTPUT_CNT_MSK;
}

/**
 * \brief  Clear IF8080 input data counter value.
 * \param  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_ClearRxCounter();
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_ClearRxCounter(void)
{
    IF8080->ICR = IF8080_CLR_RX_OUTPUT_CNT_MSK;
}

/**
 * \brief  Write IF8080 FIFO in auto mode.
 * \param[in]  data: FIFO data.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t data = 0x01020304
 *     IF8080_WriteFIFO(data);
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_WriteFIFO(uint32_t data)
{
    IF8080->FIFO = data;
}

/**
 * \brief  Read IF8080 FIFO in auto mode.
 * \param[in]  data: FIFO data.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     uint32_t data = IF8080_ReadFIFO();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t IF8080_ReadFIFO(void)
{
    return IF8080->FIFO;
}

/**
 * \brief  Clear IF8080 output data counter value.
 * \param  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_ClearFIFO();
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_ClearFIFO(void)
{
    IF8080->ICR = IF8080_CLR_FIFO_MSK;
}

/**
 * \brief  Enable or disable Vsync start function.
 * \param[in]  NewState: new state of the Vsync function.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void if8080_demo(void)
 * {
 *     IF8080_VsyncCmd(ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void IF8080_VsyncCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        IF8080->CTRL0 |= IF8080_VSYNC_START_EN_MSK;
    }
    else
    {
        IF8080->CTRL0 &= IF8080_VSYNC_START_EN_CLR;
    }
}

/** \} */ /*End of group IF8080_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876x_IF8080_H_ */


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

