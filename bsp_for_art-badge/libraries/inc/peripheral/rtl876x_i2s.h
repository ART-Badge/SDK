/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_i2s.h
* \brief    The header file of the peripheral I2S driver.
* \details  This file provides all I2S firmware functions.
* \author   elliot chen
* \date     2017-12-06
* \version  v1.0
* *********************************************************************************************************
*/


#ifndef _RTL876x_I2S_H
#define _RTL876x_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    I2S         I2S
 *
 * \brief       Manage the I2S peripheral functions.
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

/**
 * \defgroup    I2S_Exported_Types Inti Params Struct
 *
 * \ingroup     I2S
 */

/**
 * \brief       I2S initialize parameters.
 *
 * \ingroup     I2S_Exported_Types
 */
typedef struct
{
    uint32_t I2S_ClockSource;       /*!< Specifies the I2S clock source.
                                            This parameter can be a value of \ref I2S_Clock_Source*/
    uint32_t I2S_BClockMi;          /*!< Specifies the BLCK clock speed. BCLK = 40MHz*(I2S_BClockNi/I2S_BClockMi).
                                            This parameter must range from 1 to 0xffff */
    uint32_t I2S_BClockNi;          /*!< Specifies the BLCK clock speed.
                                            This parameter must range from 1 to 0x7FFF */
    uint32_t I2S_DeviceMode;        /*!< Specifies the I2S device mode.
                                            This parameter can be a value of \ref I2S_device_mode*/
    uint32_t I2S_ChannelType;       /*!< Specifies the channel type used for the I2S communication.
                                            This parameter can be a value of \ref I2S_Channel_Type */
    uint32_t I2S_TxChSequence;      /*!< Specifies the transmission channel seqence used for the I2S communication.
                                            This parameter can be a value of \ref I2S_Tx_Ch_Sequence*/
    uint32_t I2S_RxChSequence;      /*!< Specifies the receiving channel seqence used for the I2S communication.
                                            This parameter can be a value of \ref I2S_Rx_Ch_Sequence*/
    uint32_t I2S_DataFormat;        /*!< Specifies the I2S Data format mode.
                                            This parameter can be a value of \ref I2S_Format_Mode*/
    uint32_t I2S_TxBitSequence;     /*!< Specifies the I2S Data bits sequences.
                                            This parameter can be a value of \ref I2S_Tx_Bit_Sequence*/
    uint32_t I2S_RxBitSequence;     /*!< Specifies the I2S Data bits sequences.
                                            This parameter can be a value of \ref I2S_Rx_Bit_Sequence*/
    uint32_t I2S_DataWidth;         /*!< Specifies the I2S Data width.
                                            This parameter can be a value of \ref I2S_Data_Width */
    uint32_t I2S_MCLKOutput;        /*!< Specifies the I2S MCLK output freqency.
                                            This parameter can be a value of \ref I2S_MCLK_Output */
    uint32_t I2S_DMACmd;            /*!< Specifies the I2S DMA control.
                                            This parameter can be a value of \ref FunctionalState */
    uint32_t I2S_TxWaterlevel;      /*!< Specifies the dma watermark level in transmit mode.
                                            This parameter must range from 1 to 63 */
    uint32_t I2S_RxWaterlevel;      /*!< Specifies the dma watermark level in receive mode.
                                            This parameter must range from 1 to 63 */
} I2S_InitTypeDef;


/*============================================================================*
 *                         Register Defines
 *============================================================================*/

/* Peripheral: I2S */
/* Description: I2S register defines */

/* Register: CTRL0 -------------------------------------------------------*/
/* Description: Control register 0. Offset: 0x04. */

/* CTRL0[30]: I2S_MCLK_SEL. */
#define I2S_MCLK_SEL_POS                (30)
#define I2S_MCLK_SEL_MSK                (0x1 << I2S_MCLK_SEL_POS)
#define I2S_MCLK_SEL_CLR                (~I2S_MCLK_SEL_MSK)
/* CTRL0[29:28]: I2S_RX_CH_SEQ. */
#define I2S_RX_CH_SEQ_POS               (28)
#define I2S_RX_CH_SEQ_MSK               (0x1 << I2S_RX_CH_SEQ_POS)
#define I2S_RX_CH_SEQ_CLR               (~I2S_RX_CH_SEQ_MSK)
/* CTRL0[25]: I2S_START_RX. */
#define I2S_START_RX_POS                (25)
#define I2S_START_RX_MSK                (0x1 << I2S_START_RX_POS)
#define I2S_START_RX_CLR                (~I2S_START_RX_MSK)
/* CTRL0[24]: I2S_RX_DISABLE. */
#define I2S_RX_DISABLE_POS              (24)
#define I2S_RX_DISABLE_MSK              (0x1 << I2S_RX_DISABLE_POS)
#define I2S_RX_DISABLE_CLR              (~I2S_RX_DISABLE_MSK)
/* CTRL0[23]: I2S_RX_LSB_FIRST. */
#define I2S_RX_LSB_FIRST_POS            (23)
#define I2S_RX_LSB_FIRST_MSK            (0x1 << I2S_RX_LSB_FIRST_POS)
#define I2S_RX_LSB_FIRST_CLR            (~I2S_RX_LSB_FIRST_MSK)
/* CTRL0[22]: I2S_TX_LSB_FIRST. */
#define I2S_TX_LSB_FIRST_POS            (22)
#define I2S_TX_LSB_FIRST_MSK            (0x1 << I2S_TX_LSB_FIRST_POS)
#define I2S_TX_LSB_FIRST_CLR            (~I2S_TX_LSB_FIRST_MSK)
/* CTRL0[21:20]: I2S_TX_CH_SEQ. */
#define I2S_TX_CH_SEQ_POS               (20)
#define I2S_TX_CH_SEQ_MSK               (0x1 << I2S_TX_CH_SEQ_POS)
#define I2S_TX_CH_SEQ_CLR               (~I2S_TX_CH_SEQ_MSK)
/* CTRL0[17]: I2S_START_TX. */
#define I2S_START_TX_POS                (17)
#define I2S_START_TX_MSK                (0x1 << I2S_START_TX_POS)
#define I2S_START_TX_CLR                (~I2S_START_TX_MSK)
/* CTRL0[16]: I2S_TX_DISABLE. */
#define I2S_TX_DISABLE_POS              (16)
#define I2S_TX_DISABLE_MSK              (0x1 << I2S_TX_DISABLE_POS)
#define I2S_TX_DISABLE_CLR              (~I2S_TX_DISABLE_MSK)
/* CTRL0[13:12]: I2S_DATA_WIDTH. */
#define I2S_DATA_WIDTH_POS              (12)
#define I2S_DATA_WIDTH_MSK              (0x3 << I2S_DATA_WIDTH_POS)
#define I2S_DATA_WIDTH_CLR              (~I2S_DATA_WIDTH_MSK)
/* CTRL0[11]: I2S_CHANNEL_TYPE. */
#define I2S_CHANNEL_TYPE_POS            (11)
#define I2S_CHANNEL_TYPE_MSK            (0x1 << I2S_CHANNEL_TYPE_POS)
#define I2S_CHANNEL_TYPE_CLR            (~I2S_CHANNEL_TYPE_MSK)
/* CTRL0[9:8]: I2S_DATA_FORMAT. */
#define I2S_DATA_FORMAT_POS             (8)
#define I2S_DATA_FORMAT_MSK             (0x3 << I2S_DATA_FORMAT_POS)
#define I2S_DATA_FORMAT_CLR             (~I2S_DATA_FORMAT_MSK)
/* CTRL0[7]: I2S_CTRL_MODE. */
#define I2S_CTRL_MODE_POS              (7)
#define I2S_CTRL_MODE_MSK              (0x1 << I2S_CTRL_MODE_POS)
#define I2S_CTRL_MODE_CLR              (~I2S_CTRL_MODE_MSK)
/* CTRL0[4]: I2S_SLAVE_DATA_SEL. */
#define I2S_SLAVE_DATA_SEL_POS          (4)
#define I2S_SLAVE_DATA_SEL_MSK          (0x1 << I2S_SLAVE_DATA_SEL_POS)
#define I2S_SLAVE_DATA_SEL_CLR          (~I2S_SLAVE_DATA_SEL_MSK)
/* CTRL0[3]: I2S_SLAVE_CLK_SEL. */
#define I2S_SLAVE_CLK_SEL_POS           (3)
#define I2S_SLAVE_CLK_SEL_MSK           (0x1 << I2S_SLAVE_CLK_SEL_POS)
#define I2S_SLAVE_CLK_SEL_CLR           (~I2S_SLAVE_CLK_SEL_MSK)
/* CTRL0[0]: I2S_RESET. */
#define I2S_RESET_POS                   (0)
#define I2S_RESET_MSK                   (0x1 << I2S_RESET_POS)
#define I2S_RESET_CLR                   (~I2S_RESET_MSK)

/* Register: CTRL1 -------------------------------------------------------*/
/* Description: Control register 1. Offset: 0x08. */

/* CTRL1[30]: I2S_TX_IDLE. */
#define I2S_TX_IDLE_POS                 (30)
#define I2S_TX_IDLE_MSK                 (0x1 << I2S_TX_IDLE_POS)
#define I2S_TX_IDLE_CLR                 (~I2S_TX_IDLE_MSK)
/* CTRL1[29]: I2S_RF_EMPTY. */
#define I2S_RF_EMPTY_POS                (29)
#define I2S_RF_EMPTY_MSK                (0x1 << I2S_RF_EMPTY_POS)
#define I2S_RF_EMPTY_CLR                (~I2S_RF_EMPTY_MSK)
/* CTRL1[28]: I2S_TF_EMPTY. */
#define I2S_TF_EMPTY_POS                (28)
#define I2S_TF_EMPTY_MSK                (0x1 << I2S_TF_EMPTY_POS)
#define I2S_TF_EMPTY_CLR                (~I2S_TF_EMPTY_MSK)
/* CTRL1[27]: I2S_RX_FULL. */
#define I2S_RX_FULL_POS                 (27)
#define I2S_RX_FULL_MSK                 (0x1 << I2S_RX_FULL_POS)
#define I2S_RX_FULL_CLR                 (~I2S_RX_FULL_MSK)
/* CTRL1[26]: I2S_TX_FULL. */
#define I2S_TX_FULL_POS                 (26)
#define I2S_TX_FULL_MSK                 (0x1 << I2S_TX_FULL_POS)
#define I2S_TX_FULL_CLR                 (~I2S_TX_FULL_MSK)
/* CTRL1[25]: I2S_READY_TO_RX. */
#define I2S_READY_TO_RX_POS             (25)
#define I2S_READY_TO_RX_MSK             (0x1 << I2S_READY_TO_RX_POS)
#define I2S_READY_TO_RX_CLR             (~I2S_READY_TO_RX_MSK)
/* CTRL1[24]: I2S_READY_TO_TX. */
#define I2S_READY_TO_TX_POS             (24)
#define I2S_READY_TO_TX_MSK             (0x1 << I2S_READY_TO_TX_POS)
#define I2S_READY_TO_TX_CLR             (~I2S_READY_TO_TX_MSK)

/* CTRL1[23]: I2S_RX_SNK_LR_SWAP. */
#define I2S_RX_SNK_LR_SWAP_POS          (23)
#define I2S_RX_SNK_LR_SWAP_MSK          (0x1 << I2S_RX_SNK_LR_SWAP_POS)
#define I2S_RX_SNK_LR_SWAP_CLR          (~I2S_RX_SNK_LR_SWAP_MSK)
/* CTRL1[22]: I2S_RX_SNK_BYTE_SWAP. */
#define I2S_RX_SNK_BYTE_SWAP_POS        (22)
#define I2S_RX_SNK_BYTE_SWAP_MSK        (0x1 << I2S_RX_SNK_BYTE_SWAP_POS)
#define I2S_RX_SNK_BYTE_SWAP_CLR        (~I2S_RX_SNK_BYTE_SWAP_MSK)
/* CTRL1[21]: I2S_TX_SRC_LR_SWAP. */
#define I2S_TX_SRC_LR_SWAP_POS          (21)
#define I2S_TX_SRC_LR_SWAP_MSK          (0x1 << I2S_TX_SRC_LR_SWAP_POS)
#define I2S_TX_SRC_LR_SWAP_CLR          (~I2S_TX_SRC_LR_SWAP_MSK)
/* CTRL1[20]: I2S_TX_SRC_BYTE_SWAP. */
#define I2S_TX_SRC_BYTE_SWAP_POS        (20)
#define I2S_TX_SRC_BYTE_SWAP_MSK        (0x1 << I2S_TX_SRC_BYTE_SWAP_POS)
#define I2S_TX_SRC_BYTE_SWAP_CLR        (~I2S_TX_SRC_BYTE_SWAP_MSK)
/* CTRL1[17]: I2S_CLK_128FS. */
#define I2S_CLK_128FS_POS               (17)
#define I2S_CLK_128FS_MSK               (0x1 << I2S_CLK_128FS_POS)
#define I2S_CLK_128FS_CLR               (~I2S_CLK_128FS_MSK)
/* CTRL1[16]: I2S_CLK_40M. */
#define I2S_CLK_128_FS_POS              (16)
#define I2S_CLK_128_FS_MSK              (0x1 << I2S_CLK_128_FS_POS)
#define I2S_CLK_128_FS_CLR              (~I2S_CLK_128_FS_MSK)
/* CTRL1[13]: I2S_CLR_RX_ERR_CNT. */
#define I2S_CLR_RX_ERR_CNT_POS          (13)
#define I2S_CLR_RX_ERR_CNT_MSK          (0x1 << I2S_CLR_RX_ERR_CNT_POS)
#define I2S_CLR_RX_ERR_CNT_CLR          (~I2S_CLR_RX_ERR_CNT_MSK)
/* CTRL1[12]: I2S_CLR_TX_ERR_CNT. */
#define I2S_CLR_TX_ERR_CNT_POS          (12)
#define I2S_CLR_TX_ERR_CNT_MSK          (0x1 << I2S_CLR_TX_ERR_CNT_POS)
#define I2S_CLR_TX_ERR_CNT_CLR          (~I2S_CLR_TX_ERR_CNT_MSK)
/* CTRL1[7:0]: I2S_FRAME_SYNC_OFFSET. */
#define I2S_FRAME_SYNC_OFFSET_POS       (0)
#define I2S_FRAME_SYNC_OFFSET_MSK       (0xFF << I2S_FRAME_SYNC_OFFSET_POS)
#define I2S_FRAME_SYNC_OFFSET_CLR       (~I2S_FRAME_SYNC_OFFSET_MSK)
#define I2S_FRAME_SYNC_OFFSET_DEFAULT   (0x81 << I2S_FRAME_SYNC_OFFSET_POS)

/* Register: DSP_INT_CR -------------------------------------------------------*/
/* Description:Interrupt clearregister. Offset: 0x0C. */

/* DSP_INT_CR[1]: I2S_RX_CLEAR_INT. */
#define I2S_RX_CLEAR_INT_POS            (1)
#define I2S_RX_CLEAR_INT_MSK            (0x1 << I2S_RX_CLEAR_INT_POS)
#define I2S_RX_CLEAR_INT_CLR            (~I2S_RX_CLEAR_INT_MSK)
/* DSP_INT_CR[0]: I2S_TX_CLEAR_INT. */
#define I2S_TX_CLEAR_INT_POS            (0)
#define I2S_TX_CLEAR_INT_MSK            (0x1 << I2S_TX_CLEAR_INT_POS)
#define I2S_TX_CLEAR_INT_CLR            (~I2S_TX_CLEAR_INT_MSK)

/* Register: FIFO_SR -------------------------------------------------------*/
/* Description: FIFO status register. Offset: 0x14. */

/* FIFO_SR[13:8]: I2S_RX_FIFO_DEPTH_CNT. */
#define I2S_RX_FIFO_DEPTH_CNT_POS       (8)
#define I2S_RX_FIFO_DEPTH_CNT_MSK       (0x3F << I2S_RX_FIFO_DEPTH_CNT_POS)
#define I2S_RX_FIFO_DEPTH_CNT_CLR       (~I2S_RX_FIFO_DEPTH_CNT_MSK)
/* FIFO_SR[5:0]: I2S_TX_FIFO_DEPTH_CNT. */
#define I2S_TX_FIFO_DEPTH_CNT_POS       (0)
#define I2S_TX_FIFO_DEPTH_CNT_MSK       (0x3F << I2S_TX_FIFO_DEPTH_CNT_POS)
#define I2S_TX_FIFO_DEPTH_CNT_CLR       (~I2S_TX_FIFO_DEPTH_CNT_MSK)

/* Register: ERROR_CNT_SR -------------------------------------------------------*/
/* Description: Error counter status register. Offset: 0x18. */

/* ERROR_CNT_SR[15:8]: I2S_RX_ERR_CNT. */
#define I2S_RX_ERR_CNT_POS              (8)
#define I2S_RX_ERR_CNT_MSK              (0xFF << I2S_RX_ERR_CNT_POS)
#define I2S_RX_ERR_CNT_CLR              (~I2S_RX_ERR_CNT_MSK)
/* ERROR_CNT_SR[7:0]: I2S_TX_ERR_CNT. */
#define I2S_TX_ERR_CNT_POS              (0)
#define I2S_TX_ERR_CNT_MSK              (0xFF << I2S_TX_ERR_CNT_POS)
#define I2S_TX_ERR_CNT_CLR              (~I2S_TX_ERR_CNT_MSK)

/* Register: BCLK_DIV -------------------------------------------------------*/
/* Description: BCLK divider register. Offset: 0x1C. */

/* BCLK_DIV[31]: I2S_MI_NI_UPDATE. */
#define I2S_MI_NI_UPDATE_POS        (31)
#define I2S_MI_NI_UPDATE_MSK        ((uint32_t)0x1 << I2S_MI_NI_UPDATE_POS)
#define I2S_MI_NI_UPDATE_CLR        (~I2S_MI_NI_UPDATE_MSK)
/* BCLK_DIV[30:16]: I2S_BCLK_NI. */
#define I2S_BCLK_NI_POS             (16)
#define I2S_BCLK_NI_MSK             (0x7FFF << I2S_BCLK_NI_POS)
#define I2S_BCLK_NI_CLR             (~I2S_BCLK_NI_MSK)
/* BCLK_DIV[15:0]: I2S_BCLK_MI. */
#define I2S_BCLK_MI_POS             (0)
#define I2S_BCLK_MI_MSK             (0xFFFF << I2S_BCLK_MI_POS)
#define I2S_BCLK_MI_CLR             (~I2S_BCLK_MI_MSK)

/* Register: DMA_TRDLR -------------------------------------------------------*/
/* Description: DMA transmit receive data level register. Offset: 0x20. */

/* DMA_TRDLR[5:0]: I2S_TX_DMA_BURST_SIZE. */
#define I2S_TX_DMA_BURST_SIZE_POS   (0)
#define I2S_TX_DMA_BURST_SIZE_MSK   (0x3F << I2S_TX_DMA_BURST_SIZE_POS)
#define I2S_TX_DMA_BURST_SIZE_CLR   (~I2S_TX_DMA_BURST_SIZE_MSK)
/* DMA_TRDLR[13:8]: I2S_RX_DMA_BURST_SIZE. */
#define I2S_RX_DMA_BURST_SIZE_POS   (8)
#define I2S_RX_DMA_BURST_SIZE_MSK   (0x3F << I2S_RX_DMA_BURST_SIZE_POS)
#define I2S_RX_DMA_BURST_SIZE_CLR   (~I2S_RX_DMA_BURST_SIZE_MSK)

/* Register: PERIPH_AUDIO_CLK_224 -------------------------------------------------------*/
/* Description: peri on register. address: 0x40000224. */
#define PERIPH_AUDIO_CLK_224        0x40000224
/* PERIPH_AUDIO_CLK_224[3]: I2S0_EXT_CODEC. */
#define I2S0_EXT_CODEC_POS         (3)
#define I2S0_EXT_CODEC_MSK         (0x1 << I2S0_EXT_CODEC_POS)
#define I2S0_EXT_CODEC_CLR         (~I2S0_EXT_CODEC_MSK)

/* Register: PERIPH_AUDIO_CLK_228 -------------------------------------------------------*/
/* Description: peri on register. address: 0x40000228. */
#define PERIPH_AUDIO_CLK_228        0x40000228
/* PERIPH_AUDIO_CLK_228[4]: I2S_MCLK_OUTPUT. */
#define I2S_MCLK_OUTPUT_POS         (4)
#define I2S_MCLK_OUTPUT_MSK         (0x1 << I2S_MCLK_OUTPUT_POS)
#define I2S_MCLK_OUTPUT_CLR         (~I2S_MCLK_OUTPUT_MSK)


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    I2S_Exported_Constants  Macro Defines
 *
 * \ingroup     I2S
 */

#define IS_I2S_ALL_PERIPH(PERIPH) (((PERIPH) == I2S0) || \
                                   ((PERIPH) == I2S1))

/**
 * \defgroup    I2S_Clock_Source I2S Clock Source
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_CLK_40M                                 (I2S_CLK_128_FS_MSK)
#define I2S_CLK_128fs                               (I2S_CLK_128FS_MSK)
#define I2S_CLK_256fs                               ((uint32_t)(0x00))
/** \} */

#define IS_I2S_CLK_SOURCE(CLK) (((CLK) == I2S_CLK_40M) || \
                                ((CLK) == I2S_CLK_128fs) || \
                                ((CLK) == I2S_CLK_256fs))

/**
 * \defgroup    I2S_Device_Mode I2S Device Mode
 * \{
 * \ingroup     I2S_Exported_Constants
 */

#define I2S_DeviceMode_Master                       ((uint32_t)0x00)
#define I2S_DeviceMode_Slave                        (I2S_SLAVE_DATA_SEL_MSK )//| I2S_SLAVE_CLK_SEL_MSK)
/** \} */

#define IS_I2S_DEVICE_MODE(DEVICE)      (((DEVICE) == I2S_DeviceMode_Master) || ((DEVICE) == I2S_DeviceMode_Slave))

/**
 * \defgroup    I2S_Mode I2S Mode
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_MODE_TX                                 (I2S_START_TX_MSK)
#define I2S_MODE_RX                                 (I2S_START_RX_MSK)
/** \} */

#define IS_I2S_MODE(MODE)               (((MODE) == I2S_MODE_TX) || ((MODE) == I2S_MODE_RX))

/**
 * \defgroup    I2S_Channel_Type I2S Channel Type
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_Channel_Mono                            (I2S_CHANNEL_TYPE_MSK)
#define I2S_Channel_stereo                          ((uint32_t)(0x0))
/** \} */

#define IS_I2S_CHANNEL_TYPE(TYPE)       (((TYPE) == I2S_Channel_Mono) || ((TYPE) == I2S_Channel_stereo))

/**
 * \defgroup    I2S_Tx_Ch_Sequence I2S Transmission Channel Sequence
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_TX_CH_L_R                           (0)
#define I2S_TX_CH_R_L                           ((uint32_t)0x01 << I2S_TX_CH_SEQ_POS)
#define I2S_TX_CH_L_L                           ((uint32_t)0x02 << I2S_TX_CH_SEQ_POS)
#define I2S_TX_CH_R_R                           ((uint32_t)0x03 << I2S_TX_CH_SEQ_POS)
/** \} */

#define IS_I2S_TX_CH_SEQ(SEQ)       (((SEQ) == I2S_TX_CH_L_R) || ((SEQ) == I2S_TX_CH_R_L) || \
                                     ((SEQ) == I2S_TX_CH_L_L) || ((SEQ) == I2S_TX_CH_R_R))

/**
 * \defgroup    I2S_Rx_Ch_Sequence I2S Receiving Channel Sequence
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_RX_CH_L_R                           (0)
#define I2S_RX_CH_R_L                           ((uint32_t)0x01 << I2S_RX_CH_SEQ_POS)
#define I2S_RX_CH_L_L                           ((uint32_t)0x02 << I2S_RX_CH_SEQ_POS)
#define I2S_RX_CH_R_R                           ((uint32_t)0x03 << I2S_RX_CH_SEQ_POS)
/** \} */

#define IS_I2S_RX_CH_SEQ(SEQ)       (((SEQ) == I2S_RX_CH_L_R) || ((SEQ) == I2S_RX_CH_R_L) || \
                                     ((SEQ) == I2S_RX_CH_L_L) || ((SEQ) == I2S_RX_CH_R_R))

/**
 * \defgroup    I2S_Format_Mode I2S Format Mode
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_Mode                                    ((uint32_t)(0x00 << I2S_DATA_FORMAT_POS))
#define Left_Justified_Mode                         ((uint32_t)(0x01 << I2S_DATA_FORMAT_POS))
#define PCM_Mode_A                                  ((uint32_t)(0x02 << I2S_DATA_FORMAT_POS))
#define PCM_Mode_B                                  ((uint32_t)(0x03 << I2S_DATA_FORMAT_POS))
/** \} */

#define IS_I2S_DATA_FORMAT(FORMAT)      (((FORMAT) == I2S_Mode) || ((FORMAT) == Left_Justified_Mode) || \
                                         ((FORMAT) == PCM_Mode_A) || ((FORMAT) == PCM_Mode_B))

/**
 * \defgroup    I2S_Tx_Bit_Sequence I2S Tx Bit Sequence
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_TX_MSB_First                            ((uint32_t)0x0)
#define I2S_TX_LSB_First                            (I2S_TX_LSB_FIRST_MSK)
/** \} */

#define IS_I2S_TX_BIT_SEQ(SEQ)      (((SEQ) == I2S_TX_MSB_First) || ((SEQ) == I2S_TX_LSB_First))

/**
 * \defgroup    I2S_Rx_Bit_Sequence I2S Rx Bit Sequence
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_RX_MSB_First                            ((uint32_t)0x0)
#define I2S_RX_LSB_First                            (I2S_RX_LSB_FIRST_MSK)
/** \} */

#define IS_I2S_RX_BIT_SEQ(SEQ)      (((SEQ) == I2S_RX_MSB_First) || ((SEQ) == I2S_RX_LSB_First))

/**
 * \defgroup    I2S_Data_Width I2S Data Width
 * \{
 * \ingroup     I2S_Exported_Constants
 */

#define I2S_Width_16Bits                            ((uint32_t)(0x00 << I2S_DATA_WIDTH_POS))
#define I2S_Width_24Bits                            ((uint32_t)(0x02 << I2S_DATA_WIDTH_POS))
#define I2S_Width_8Bits                             ((uint32_t)(0x03 << I2S_DATA_WIDTH_POS))
/** \} */

#define IS_I2S_DATA_WIDTH(WIDTH)        (((WIDTH) == I2S_Width_16Bits) || \
                                         ((WIDTH) == I2S_Width_24Bits) || \
                                         ((WIDTH) == I2S_Width_8Bits))

/**
 * \defgroup    I2S_MCLK_Output I2S MCLK Output
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_MCLK_128fs                              ((uint32_t)0x0)
#define I2S_MCLK_256fs                              (I2S_MCLK_SEL_MSK)
/** \} */

#define IS_I2S_MCLK_OUTPUT_TYPE(TYPE)       (((TYPE) == I2S_MCLK_128fs) || ((TYPE) == I2S_MCLK_256fs))

/**
 * \defgroup    I2S_DMA_Cmd I2S DMA Cmd
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_DMA_ENABLE                              ((uint32_t)0x0)
#define I2S_DMA_DISABLE                             (I2S_CTRL_MODE_MSK)
/** \} */

#define IS_I2S_DMA_CMD(CMD)       (((CMD) == I2S_DMA_ENABLE) || ((CMD) == I2S_DMA_DISABLE))

/**
 * \defgroup    I2S_Interrupt_Definition   I2S Interrupt Definition
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_INT_TX_IDLE                             (I2S_TX_IDLE_MSK)
#define I2S_INT_RF_EMPTY                            (I2S_RF_EMPTY_MSK)
#define I2S_INT_TF_EMPTY                            (I2S_TF_EMPTY_MSK)
#define I2S_INT_RF_FULL                             (I2S_RX_FULL_MSK)
#define I2S_INT_TF_FULL                             (I2S_TX_FULL_MSK)
#define I2S_INT_RX_READY                            (I2S_READY_TO_RX_MSK)
#define I2S_INT_TX_READY                            (I2S_READY_TO_TX_MSK)
/** \} */

#define IS_I2S_INT_CONFIG(INT)          (((INT) == I2S_INT_TX_IDLE) || ((INT) == I2S_INT_RF_EMPTY) || \
                                         ((INT) == I2S_INT_TF_EMPTY) || ((INT) == I2S_INT_RF_FULL) || \
                                         ((INT) == I2S_INT_TF_FULL) || ((INT) == I2S_INT_RX_READY) || \
                                         ((INT) == I2S_INT_TX_READY) )

/**
 * \defgroup   I2S_Clear_Interrupt_Definition     I2S Clear Interrupt Definition
 * \{
 * \ingroup     I2S_Exported_Constants
 */
#define I2S_CLEAR_INT_RX_READY                      (I2S_RX_CLEAR_INT_MSK)
#define I2S_CLEAR_INT_TX_READY                      (I2S_TX_CLEAR_INT_MSK)
/** \} */

#define IS_I2S_CLEAR_INT(CLEAR)          (((CLEAR) == I2S_CLEAR_INT_RX_READY) || \
                                          ((CLEAR) == I2S_CLEAR_INT_TX_READY) )


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    I2S_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     I2S
 */

/**
 * \brief   Deinitializes the I2S peripheral registers to their default values.
 * \param   None.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_i2s_init(void)
 * {
 *     I2S_DeInit(I2S0);
 * }
 * \endcode
 */
void I2S_DeInit(I2S_TypeDef *I2Sx);

/**
 * \brief   Initializes the I2S peripheral according to the specified
 *          parameters in the I2S_InitStruct
 * \param[in] I2Sx: Selected I2S peripheral.
 * \param[in] I2S_InitStruct: Pointer to a I2S_InitTypeDef structure that
 *            contains the configuration information for the specified I2S peripheral
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_i2s_init(void)
 * {
 *     RCC_PeriphClockCmd(APB_I2S, APB_I2S_CLOCK, ENABLE);
 *
 *     I2S_InitTypeDef I2S_InitStruct;
 *
 *     I2S_StructInit(&I2S_InitStruct);
 *     I2S_InitStruct.I2S_ClockSource      = I2S_CLK_40M;
 *     I2S_InitStruct.I2S_BClockMi         = 0x271;
 *     I2S_InitStruct.I2S_BClockNi         = 0x10;
 *     I2S_InitStruct.I2S_DeviceMode       = I2S_DeviceMode_Master;
 *     I2S_InitStruct.I2S_ChannelType      = I2S_Channel_stereo;
 *     I2S_InitStruct.I2S_DataWidth        = I2S_Width_16Bits;
 *     I2S_InitStruct.I2S_DataFormat       = I2S_Mode;
 *     I2S_InitStruct.I2S_DMACmd           = I2S_DMA_DISABLE;
 *     I2S_Init(I2S0, &I2S_InitStruct);
 *     I2S_Cmd(I2S0, I2S_MODE_TX, ENABLE);
 * }
 * \endcode
 */
void I2S_Init(I2S_TypeDef *I2Sx, I2S_InitTypeDef *I2S_InitStruct);

/**
 * \brief   Fills each I2S_InitStruct member with its default value.
 * \param[in] I2S_InitStruct: Pointer to an I2S_InitTypeDef structure which will be initialized.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_i2s_init(void)
 * {
 *     RCC_PeriphClockCmd(APB_I2S, APB_I2S_CLOCK, ENABLE);
 *
 *     I2S_InitTypeDef I2S_InitStruct;
 *
 *     I2S_StructInit(&I2S_InitStruct);
 *     I2S_InitStruct.I2S_ClockSource      = I2S_CLK_40M;
 *     I2S_InitStruct.I2S_BClockMi         = 0x271;
 *     I2S_InitStruct.I2S_BClockNi         = 0x10;
 *     I2S_InitStruct.I2S_DeviceMode       = I2S_DeviceMode_Master;
 *     I2S_InitStruct.I2S_ChannelType      = I2S_Channel_stereo;
 *     I2S_InitStruct.I2S_DataWidth        = I2S_Width_16Bits;
 *     I2S_InitStruct.I2S_DataFormat       = I2S_Mode;
 *     I2S_InitStruct.I2S_DMACmd           = I2S_DMA_DISABLE;
 *     I2S_Init(I2S0, &I2S_InitStruct);
 *     I2S_Cmd(I2S0, I2S_MODE_TX, ENABLE);
 * }
 * \endcode
 */
void I2S_StructInit(I2S_InitTypeDef *I2S_InitStruct);

/**
 * \brief   Enable or disable the selected I2S mode.
 * \param[in] I2Sx: Selected I2S peripheral.
 * \param[in] mode: Selected I2S operation mode.
 *      This parameter can be the following values:
 *      \arg I2S_MODE_TX: Transmission mode.
 *      \arg I2S_MODE_RX: Receiving mode.
 * \param[in] NewState: New state of the operation mode.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_i2s_init(void)
 * {
 *     RCC_PeriphClockCmd(APB_I2S, APB_I2S_CLOCK, ENABLE);
 *
 *     I2S_InitTypeDef I2S_InitStruct;
 *
 *     I2S_StructInit(&I2S_InitStruct);
 *     I2S_InitStruct.I2S_ClockSource      = I2S_CLK_40M;
 *     I2S_InitStruct.I2S_BClockMi         = 0x271;
 *     I2S_InitStruct.I2S_BClockNi         = 0x10;
 *     I2S_InitStruct.I2S_DeviceMode       = I2S_DeviceMode_Master;
 *     I2S_InitStruct.I2S_ChannelType      = I2S_Channel_stereo;
 *     I2S_InitStruct.I2S_DataWidth        = I2S_Width_16Bits;
 *     I2S_InitStruct.I2S_DataFormat       = I2S_Mode;
 *     I2S_InitStruct.I2S_DMACmd           = I2S_DMA_DISABLE;
 *     I2S_Init(I2S_NUM, &I2S_InitStruct);
 *     I2S_Cmd(I2S_NUM, I2S_MODE_TX, ENABLE);
 * }
 * \endcode
 */
void I2S_Cmd(I2S_TypeDef *I2Sx, uint32_t mode, FunctionalState NewState);

/**
 * \brief   Enable or disable the specified I2S interrupt source.
 * \param[in] I2S_INT: Specifies the I2S interrupt source to be enable or disable.
 *      This parameter can be the following values:
 *      \arg I2S_INT_TX_IDLE: Transmit idle interrupt source.
 *      \arg I2S_INT_RF_EMPTY: Receive FIFO empty interrupt source.
 *      \arg I2S_INT_TF_EMPTY: Transmit FIFO empty interrupt source.
 *      \arg I2S_INT_RF_FULL: Receive FIFO full interrupt source.
 *      \arg I2S_INT_TF_FULL: Transmit FIFO full interrupt source.
 *      \arg I2S_INT_RX_READY: Ready to receive interrupt source.
 *      \arg I2S_INT_TX_READY: Ready to transmit interrupt source.
 * \param[in]  NewState: New state of the specified I2S interrupt.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_INTConfig(I2S0, I2S_INT_TF_EMPTY, ENABLE);
 * }
 * \endcode
 */
void I2S_INTConfig(I2S_TypeDef *I2Sx, uint32_t I2S_INT, FunctionalState newState);

/**
 * \brief   Get the specified I2S interrupt status.
 * \param[in] I2S_INT: the specified I2S interrupt.
 *      This parameter can be one of the following values:
 *      \arg I2S_INT_TX_IDLE: Transmit idle interrupt.
 *      \arg I2S_INT_RF_EMPTY: Receive FIFO empty interrupt.
 *      \arg I2S_INT_TF_EMPTY: Transmit FIFO empty interrupt.
 *      \arg I2S_INT_RF_FULL: Receive FIFO full interrupt.
 *      \arg I2S_INT_TF_FULL: Transmit FIFO full interrupt.
 *      \arg I2S_INT_RX_READY: Ready to receive interrupt.
 *      \arg I2S_INT_TX_READY: Ready to transmit interrupt.
 * \retval The new state of I2S_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     ITStatus int_status = I2S_GetINTStatus(I2S0, I2S_INT_TF_EMPTY);
 * }
 * \endcode
 */
ITStatus I2S_GetINTStatus(I2S_TypeDef *I2Sx, uint32_t I2S_INT);

/**
 * \brief  Clear the I2S interrupt pending bit.
 * \param[in] I2S_CLEAR_INT: Specifies the interrupt pending bit to clear.
 *      This parameter can be any combination of the following values:
 *      \arg I2S_CLEAR_INT_RX_READY: Clear ready to receive interrupt.
 *      \arg I2S_CLEAR_INT_TX_READY: Clear ready to transmit interrupt.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_ClearINTPendingBit(I2S0, I2S_CLEAR_INT_RX_READY);
 * }
 * \endcode
 */
void I2S_ClearINTPendingBit(I2S_TypeDef *I2Sx, uint32_t I2S_CLEAR_INT);

/**
 * \brief   Transmits a data through the SPIx/I2Sx peripheral.
 * \param[in] I2Sx: To select the I2Sx peripheral, x can be 0 or 1.
 * \param[in] Data: Data to be transmitted.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_SendData(I2S0, 0x02);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_SendData(I2S_TypeDef *I2Sx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    I2Sx->TX_DR = Data;
}

/**
 * \brief  Received data by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \return Return the most recent received data.
 * \
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     uint32_t data = I2S_ReceiveData(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t I2S_ReceiveData(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    return I2Sx->RX_DR;
}

/**
 * \brief   Get transmit FIFO free length by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \retval  Return the transmit FIFO free length.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     uint8_t data_len = I2S_GetTxFIFOFreeLen(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t I2S_GetTxFIFOFreeLen(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    return ((I2Sx->FIFO_SR & I2S_TX_FIFO_DEPTH_CNT_MSK) >> I2S_TX_FIFO_DEPTH_CNT_POS);
}

/**
 * \brief   Get receive FIFO data length by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \return  The data length of the receive FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     uint8_t data_len = I2S_GetRxFIFOLen(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t I2S_GetRxFIFOLen(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    return ((I2Sx->FIFO_SR & I2S_RX_FIFO_DEPTH_CNT_MSK) >> I2S_RX_FIFO_DEPTH_CNT_POS);
}

/**
 * \brief   Get the send error counter value by the I2Sx peripheral.
 * \param[in]  I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \return  The send error counter value .
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     uint8_t conter = I2S_GetTxErrCnt(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t I2S_GetTxErrCnt(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    return ((I2Sx->ERROR_CNT_SR & I2S_TX_ERR_CNT_MSK) >> I2S_TX_ERR_CNT_POS);
}

/**
 * \brief  Get the reception error counter value by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \retval  The reception error counter value .
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     uint8_t conter = I2S_GetRxErrCnt(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t I2S_GetRxErrCnt(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    return ((I2Sx->ERROR_CNT_SR & I2S_RX_ERR_CNT_MSK) >> I2S_RX_ERR_CNT_POS);
}

/**
 * \brief   Swap audio data bytes sequence which sent by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] NewState: New state of the bytes sequence.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_SwapBytesForSend(I2S0, ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_SwapBytesForSend(I2S_TypeDef *I2Sx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (NewState == ENABLE)
    {
        I2Sx->CTRL1 |= I2S_TX_SRC_BYTE_SWAP_MSK;
    }
    else
    {
        I2Sx->CTRL1 &= I2S_TX_SRC_BYTE_SWAP_CLR;
    }
}

/**
 * \brief   Swap audio data bytes sequence which read by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] NewState: New state of the bytes sequence.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_SwapBytesForRead(I2S0, ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_SwapBytesForRead(I2S_TypeDef *I2Sx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (NewState == ENABLE)
    {
        I2Sx->CTRL1 |= I2S_RX_SNK_BYTE_SWAP_MSK;
    }
    else
    {
        I2Sx->CTRL1 &= I2S_RX_SNK_BYTE_SWAP_CLR;
    }
}

/**
 * \brief  Swap audio channel data which sent by the I2Sx peripheral..
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] NewState: New state of the left and right channel data sequence.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_SwapLRChDataForSend(I2S0, ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_SwapLRChDataForSend(I2S_TypeDef *I2Sx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (NewState == ENABLE)
    {
        I2Sx->CTRL1 |= I2S_TX_SRC_LR_SWAP_MSK;
    }
    else
    {
        I2Sx->CTRL1 &= I2S_TX_SRC_LR_SWAP_CLR;
    }
}

/**
 * \brief   Swap audio channel data which read by the I2Sx peripheral.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] NewState: New state of the left and right channel data sequence.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_SwapLRChDataForRead(I2S0, ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_SwapLRChDataForRead(I2S_TypeDef *I2Sx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (NewState == ENABLE)
    {
        I2Sx->CTRL1 |= I2S_RX_SNK_LR_SWAP_MSK;
    }
    else
    {
        I2Sx->CTRL1 &= I2S_RX_SNK_LR_SWAP_CLR;
    }
}

/**
 * \brief   MCLK output selection which can be from I2S0 or I2S1.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] NewState: New state of MCLK output.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_MCLKOutputSelectCmd(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_MCLKOutputSelectCmd(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (I2Sx == I2S0)
    {
        *((volatile uint32_t *)PERIPH_AUDIO_CLK_228) |= I2S_MCLK_OUTPUT_MSK;
    }
    else
    {
        if (I2Sx == I2S1)
        {
            *((volatile uint32_t *)PERIPH_AUDIO_CLK_228) &= I2S_MCLK_OUTPUT_CLR;
        }
    }
}

/**
 * \brief   I2S0 communication selection which can be from intrnal codec or external codec.
 * \param[in] NewState: new state of I2S0 communication selection.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S0_WithExtCodecCmd(ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S0_WithExtCodecCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        *((volatile uint32_t *)PERIPH_AUDIO_CLK_224) |= I2S0_EXT_CODEC_MSK;
    }
    else
    {
        *((volatile uint32_t *)PERIPH_AUDIO_CLK_224) &= I2S0_EXT_CODEC_CLR;
    }
}

/**
 * \brief   Config BClk clock.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] I2S_BClockMi: Mi parameter.
 * \param[in] I2S_BClockNi: Ni parameter.
 * \return  Execution status.
 * \retval  SET: Success.
 * \retval  RESET: Failure.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     FlagStatus status = I2S_UpdateBClk(I2S0, 0x271, 0x10);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus I2S_UpdateBClk(I2S_TypeDef *I2Sx, uint16_t I2S_BClockMi,
                                          uint16_t I2S_BClockNi)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (!(I2Sx->BCLK_DIV & I2S_MI_NI_UPDATE_MSK))
    {
        I2Sx->BCLK_DIV = I2S_BClockMi | (I2S_BCLK_NI_MSK & (I2S_BClockNi << I2S_BCLK_NI_POS)) |
                         I2S_MI_NI_UPDATE_MSK;
        return SET;
    }

    return RESET;
}

/**
 * \brief   Get BClk clock status.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \return  Execution status.
 * \retval  SET: BLCK is updating.
 * \retval  RESET: BLCK update is done.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     FlagStatus status = I2S_GetBClkStatus(I2S0);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus I2S_GetBClkStatus(I2S_TypeDef *I2Sx)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));

    if (I2Sx->BCLK_DIV & I2S_MI_NI_UPDATE_MSK)
    {
        return SET;
    }

    return RESET;
}

/**
 * \brief   Config channel type.
 * \param[in] I2Sx: To select I2Sx peripheral, where x can be: 0 or 1.
 * \param[in] I2S_ChannelType: The channel type used for the I2S communication.
 *      This parameter can be one of the following values:
 *      \arg I2S_Channel_Mono: Mono channel.
 *      \arg I2S_Channel_stereo: Stereo channel.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void i2s_demo(void)
 * {
 *     I2S_ConfigChannelType(I2S0, I2S_Channel_Mono);
 * }
 * \endcode
 */
__STATIC_INLINE void I2S_ConfigChannelType(I2S_TypeDef *I2Sx, uint32_t I2S_ChannelType)
{
    I2Sx->CTRL0 &= I2S_CHANNEL_TYPE_CLR;
    I2Sx->CTRL0 |= I2S_ChannelType;
}

/** \} */ /*End of group I2S_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876x_I2S_H_ */




/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/

