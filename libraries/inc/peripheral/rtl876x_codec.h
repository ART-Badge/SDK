/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_codec.h
* \brief    The header file of the peripheral CODEC driver.
* \details  This file provides all CODEC firmware functions.
* \author   elliot chen
* \date     2019-10-25
* \version  v2.1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_CODEC_H_
#define _RTL876X_CODEC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    CODEC       CODEC
 *
 * \brief       Manage the CODEC peripheral functions.
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
 * \defgroup    CODEC_Exported_Types    Init Params Struct
 *
 * \ingroup     CODEC
 */

/**
 * \brief       CODEC initialize parameters.
 *
 * \ingroup     CODEC_Exported_Types
 */
typedef struct
{
    /* Basic parameters section */
    uint32_t CODEC_SampleRate;          /*!< Specifies the sample rate. */
    uint32_t CODEC_DmicClock;           /*!< Specifies the dmic clock. */
    uint32_t CODEC_I2SFormat;           /*!< Specifies the I2S Tx/Rx format of codec port. */
    uint32_t CODEC_I2SDataWidth;        /*!< Specifies the I2S Tx/Rx data width of codec port. */
    uint32_t CODEC_I2SChSequence;       /*!< Specifies the I2S Tx/Rx channel sequence. */
    uint32_t CODEC_MicBIAS;             /*!< Specifies the MICBIAS voltage. */
    uint32_t CODEC_MicBstGain;          /*!< Specifies the MICBST gain. */
    uint32_t CODEC_MicBstMode;          /*!< Specifies the MICBST mode. */

    /* MIC channel 0 initialization parameters section */
    uint32_t CODEC_Ch0Mute;             /*!< Specifies the channel 0 dmic mute status. */
    uint32_t CODEC_Ch0MicType;          /*!< Specifies the channel 0 mic type, which can be dmic or amic. */
    uint32_t CODEC_Ch0DmicDataLatch;    /*!< Specifies the channel 0 dmic data latch type. */
    uint32_t CODEC_Ch0AdGain;           /*!< Specifies the channel 0 ADC digital volume. */
    uint32_t CODEC_Ch0BoostGain;        /*!< Specifies the channel 0 boost gain. */
    uint32_t CODEC_Ch0ZeroDetTimeout;   /*!< Specifies the channel 0 zero detection timeout mode control. */

    /* MIC channel 1 initialization parameters section */
    uint32_t CODEC_Ch1Mute;             /*!< Specifies the channel 1 dmic mute status. */
    uint32_t CODEC_Ch1MicType;          /*!< Specifies the channel 1 mic type, which can be dmic or amic. */
    uint32_t CODEC_Ch1DmicDataLatch;    /*!< Specifies the channel 1 dmic data latch type. */
    uint32_t CODEC_Ch1AdGain;           /*!< Specifies the channel 1 ADC digital volume. */
    uint32_t CODEC_Ch1BoostGain;        /*!< Specifies the channel 1 boost gain. */
    uint32_t CODEC_Ch1ZeroDetTimeout;   /*!< Specifies the channel 1 zero detection timeout mode control. */

    /* PDM initialization parameters section */
    uint32_t CODEC_PDMClockEdge;        /*!< Specifies the DAC clock edge. */
    uint32_t CODEC_PDMChannel;          /*!< Specifies the DAC channel type. */
    uint32_t CODEC_DaMute;              /*!< Specifies the DAC mute status. */
    uint32_t CODEC_DaGain;              /*!< Specifies the DAC gain control. */
    uint32_t CODEC_DacZeroDetTimeout;   /*!< Specifies the DAC zero detection timeout mode control. */
} CODEC_InitTypeDef;

/**
 * \brief       CODEC EQ part initialize parameters.
 *
 * \ingroup     CODEC_Exported_Types
 */
typedef struct
{
    uint32_t CODEC_EQChCmd;             /*!< Specifies the EQ channel status */
    uint32_t CODEC_EQCoefH0;            /*!< Specifies the EQ coef.h0. This value can be 0 to 0x7FFFF,
                                        whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefB1;            /*!< Specifies the EQ coef.b1. This value can be 0 to 0x7FFFF,
                                        whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefB2;            /*!< Specifies the EQ coef.b2. This value can be 0 to 0x7FFFF,
                                        whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefA1;            /*!< Specifies the EQ coef.a1. This value can be 0 to 0x7FFFF,
                                        whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefA2;            /*!< Specifies the EQ coef.a2. This value can be 0 to 0x7FFFF,
                                        whose physical meaning represents a range of-8 to 7.99 */
} CODEC_EQInitTypeDef;

/*============================================================================*
 *                         Register Defines
 *============================================================================*/

/* Peripheral: CODEC */
/* Description: CODEC register defines */

/* Register: ANA_CR0 ----------------------------------------------------------*/
/* Description: Control register 0. Offset: 0x00. Address: 0x40022000UL. */

/* ANA_CR0[10]: ADC_ANA_POW. 0x1: Powen on. 0x0: Power down. */
#define CODEC_ADC_ANA_POW_POS               (10)
#define CODEC_ADC_ANA_POW_MSK               (0x1 << CODEC_ADC_ANA_POW_POS)
#define CODEC_ADC_ANA_POW_CLR               (~CODEC_ADC_ANA_POW_MSK)
/* ANA_CR0[9]: DTSDM_CLK_EN. 0x1: Powen on. 0x0: Power down. */
#define CODEC_DTSDM_CLK_EN_POS              (9)
#define CODEC_DTSDM_CLK_EN_MSK              (0x1 << CODEC_DTSDM_CLK_EN_POS)
#define CODEC_DTSDM_CLK_EN_CLR              (~CODEC_DTSDM_CLK_EN_MSK)
/* ANA_CR0[0]: DAC_ADDACK_POW. 0x1: Powen on. 0x0: Power down. */
#define CODEC_DAC_ADDACK_POW_POS            (0)
#define CODEC_DAC_ADDACK_POW_MSK            (0x1 << CODEC_DAC_ADDACK_POW_POS)
#define CODEC_DAC_ADDACK_POW_CLR            (~CODEC_DAC_ADDACK_POW_MSK)

/* Register: ANA_CR1 ----------------------------------------------------------*/
/* Description: Control register 1. Offset: 0x04. Address: 0x40022004UL. */

/* ANA_CR1[27:26]: VREF_SEL. */
#define CODEC_VREF_SEL_POS                  (26)
#define CODEC_VREF_SEL_MSK                  (0x3 << CODEC_VREF_SEL_POS)
#define CODEC_VREF_SEL_CLR                  (~CODEC_VREF_SEL_MSK)
#define CODEC_VREF_SEL_DEFALUT_MSK          (0x2 << CODEC_VREF_SEL_POS)
/* ANA_CR1[25:24]: MICBST_POW. */
#define CODEC_MICBST_POW_POS                (24)
#define CODEC_MICBST_POW_MSK                (0x3 << CODEC_MICBST_POW_POS)
#define CODEC_MICBST_POW_CLR                (~CODEC_MICBST_POW_MSK)
/* ANA_CR1[21:20]: MICBST_MUTE. */
#define CODEC_MICBST_MUTE_POS               (20)
#define CODEC_MICBST_MUTE_MSK               (0x3 << CODEC_MICBST_MUTE_POS)
#define CODEC_MICBST_MUTE_CLR               (~CODEC_MICBST_MUTE_MSK)
#define CODEC_MICBST_MUTE_MIC_MSK           (0x2 << CODEC_MICBST_MUTE_POS)
/* ANA_CR1[17:16]: MICBST_GAIN. */
#define CODEC_MICBST_GAIN_POS               (16)
#define CODEC_MICBST_GAIN_MSK               (0x3 << CODEC_MICBST_GAIN_POS)
#define CODEC_MICBST_GAIN_CLR               (~CODEC_MICBST_GAIN_MSK)
/* ANA_CR1[15]: MICBST_VREF_POW. */
#define CODEC_MICBST_VREF_POW_POS           (15)
#define CODEC_MICBST_VREF_POW_MSK           (0x1 << CODEC_MICBST_VREF_POW_POS)
#define CODEC_MICBST_VREF_POW_CLR           (~CODEC_MICBST_VREF_POW_MSK)
/* ANA_CR1[14]: MICBST_ENDFL. */
#define CODEC_MICBST_ENDFL_POS              (14)
#define CODEC_MICBST_ENDFL_MSK              (0x1 << CODEC_MICBST_ENDFL_POS)
#define CODEC_MICBST_ENDFL_CLR              (~CODEC_MICBST_ENDFL_MSK)
/* ANA_CR1[13:11]: MICBIAS_VSET. */
#define CODEC_MICBIAS_VSET_POS              (11)
#define CODEC_MICBIAS_VSET_MSK              (0x7 << CODEC_MICBIAS_VSET_POS)
#define CODEC_MICBIAS_VSET_CLR              (~CODEC_MICBIAS_VSET_MSK)
/* ANA_CR1[10]: MICBIAS_POW. 0x1: Powen on. 0x0: Power down. */
#define CODEC_MICBIAS_POW_POS               (10)
#define CODEC_MICBIAS_POW_MSK               (0x1 << CODEC_MICBIAS_POW_POS)
#define CODEC_MICBIAS_POW_CLR               (~CODEC_MICBIAS_POW_MSK)
/* ANA_CR1[9]: MICBIAS_ENCHX. */
#define CODEC_MICBIAS_ENCHX_POS             (9)
#define CODEC_MICBIAS_ENCHX_MSK             (0x1 << CODEC_MICBIAS_ENCHX_POS)
#define CODEC_MICBIAS_ENCHX_CLR             (~CODEC_MICBIAS_ENCHX_MSK)

/* Register: ANA_CR2 ----------------------------------------------------------*/
/* Description: Control register 2. Offset: 0x08. Address: 0x40022008UL. */

/* ANA_CR2[14]: MICBIAS_POWSHDT. */
#define CODEC_MICBIAS_POWSHDT_POS           (14)
#define CODEC_MICBIAS_POWSHDT_MSK           (0x1 << CODEC_MICBIAS_POWSHDT_POS)
#define CODEC_MICBIAS_POWSHDT_CLR           (~CODEC_MICBIAS_POWSHDT_MSK)
#define CODEC_MICBIAS_POWSHDT_DEFALUT_MSK   (0)
/* ANA_CR2[13:12]: MICBIAS_OCSEL. */
#define CODEC_MICBIAS_OCSEL_POS             (12)
#define CODEC_MICBIAS_OCSEL_MSK             (0x3 << CODEC_MICBIAS_OCSEL_POS)
#define CODEC_MICBIAS_OCSEL_CLR             (~CODEC_MICBIAS_OCSEL_MSK)
#define CODEC_MICBIAS_OCSEL_DEFALUT_MSK     (0x1 << CODEC_MICBIAS_OCSEL_POS)
/* ANA_CR2[11:8]: MICBIAS_COUNT. */
#define CODEC_MICBIAS_COUNT_POS             (8)
#define CODEC_MICBIAS_COUNT_MSK             (0xF << CODEC_MICBIAS_COUNT_POS)
#define CODEC_MICBIAS_COUNT_CLR             (~CODEC_MICBIAS_COUNT_MSK)
#define CODEC_MICBIAS_COUNT_DEFALUT_MSK     (0x1 << CODEC_MICBIAS_COUNT_POS)

/* Register: CR0 --------------------------------------------------------------*/
/* Description: audio Control register. Offset: 0x100. Address: 0x4002_2100UL. */

/* CR0[0]: AUDIO_IP_EN. */
#define CODEC_AUDIO_IP_EN_POS               (0)
#define CODEC_AUDIO_IP_EN_MSK               (0x1 << CODEC_AUDIO_IP_EN_POS)
#define CODEC_AUDIO_IP_EN_CLR               (~CODEC_AUDIO_IP_EN_MSK)

/* Register: ANA_CR1 -----------------------------------------------------------*/
/* Description: analog Control register. Offset: 0x104. Address: 0x4002_2104UL. */

/* ANA_CR1[3]: CKX_MICBIAS_EN. */
#define CODEC_CKX_MICBIAS_EN_POS           (3)
#define CODEC_CKX_MICBIAS_EN_MSK           (0x1 << CODEC_CKX_MICBIAS_EN_POS)
#define CODEC_CKX_MICBIAS_EN_CLR           (~CODEC_CKX_MICBIAS_EN_MSK)

/* Register: CLK_CR1 ----------------------------------------------------------*/
/* Description: clock Control register. Offset: 0x108. Address: 0x4002_2108UL. */

/* CLK_CR1[13]: AD_FILTER_CH1_CLK. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_AD_FILTER_CH1_CLK_POS         (13)
#define CODEC_AD_FILTER_CH1_CLK_MSK         (0x1 << CODEC_AD_FILTER_CH1_CLK_POS)
#define CODEC_AD_FILTER_CH1_CLK_CLR         (~CODEC_AD_FILTER_CH1_CLK_MSK)
/* CLK_CR1[12]: AD_FILTER_CH0_CLK. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_AD_FILTER_CH0_CLK_POS         (12)
#define CODEC_AD_FILTER_CH0_CLK_MSK         (0x1 << CODEC_AD_FILTER_CH0_CLK_POS)
#define CODEC_AD_FILTER_CH0_CLK_CLR         (~CODEC_AD_FILTER_CH0_CLK_MSK)
/* CLK_CR1[11]: AD_ANA_CLK_EN. 0x1: Enable ADC analog clock. 0x0: Disable ADC analog clock. */
#define CODEC_AD_ANA_CLK_EN_POS             (11)
#define CODEC_AD_ANA_CLK_EN_MSK             (0x1 << CODEC_AD_ANA_CLK_EN_POS)
#define CODEC_AD_ANA_CLK_EN_CLR             (~CODEC_AD_ANA_CLK_EN_MSK)
/* CLK_CR1[10]: AD_FIFO_CLK_EN. 0x1: Enable ADC FIFO clock. 0x0: Disable ADC FIFO clock. */
#define CODEC_AD_FIFO_CLK_EN_POS            (10)
#define CODEC_AD_FIFO_CLK_EN_MSK            (0x1 << CODEC_AD_FIFO_CLK_EN_POS)
#define CODEC_AD_FIFO_CLK_EN_CLR            (~CODEC_AD_FIFO_CLK_EN_MSK)
/* CLK_CR1[7]: AD_CH1_CLK. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_AD_CH1_CLK_POS                (7)
#define CODEC_AD_CH1_CLK_MSK                (0x1 << CODEC_AD_CH1_CLK_POS)
#define CODEC_AD_CH1_CLK_CLR                (~CODEC_AD_CH1_CLK_MSK)
/* CLK_CR1[6]: AD_CH0_CLK. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_AD_CH0_CLK_POS                (6)
#define CODEC_AD_CH0_CLK_MSK                (0x1 << CODEC_AD_CH0_CLK_POS)
#define CODEC_AD_CH0_CLK_CLR                (~CODEC_AD_CH0_CLK_MSK)
/* CLK_CR1[5]: DA_FIFO_CLK_EN. 0x1: Enable DAC FIFO clock. 0x0: Disable DAC FIFO clock. */
#define CODEC_DA_FIFO_CLK_EN_POS            (5)
#define CODEC_DA_FIFO_CLK_EN_MSK            (0x1 << CODEC_DA_FIFO_CLK_EN_POS)
#define CODEC_DA_FIFO_CLK_EN_CLR            (~CODEC_DA_FIFO_CLK_EN_MSK)
/* CLK_CR1[2]: PDM_CLK_EN. 0x1: Enable PDM clock. 0x0: Disable PDM clock. */
#define CODEC_PDM_CLK_EN_POS                (2)
#define CODEC_PDM_CLK_EN_MSK                (0x1 << CODEC_PDM_CLK_EN_POS)
#define CODEC_PDM_CLK_EN_CLR                (~CODEC_PDM_CLK_EN_MSK)
/* CLK_CR1[0]: DA_CLK_EN. 0x1: Enable DAC FIFO clock. 0x0: Disable DAC FIFO clock. */
#define CODEC_DA_CLK_EN_POS                 (0)
#define CODEC_DA_CLK_EN_MSK                 (0x1 << CODEC_DA_CLK_EN_POS)
#define CODEC_DA_CLK_EN_CLR                 (~CODEC_DA_CLK_EN_MSK)

/* Register: CLK_CR2 ----------------------------------------------------------*/
/* Description: clock Control register. Offset: 0x10C. Address: 0x4002_210CUL. */

/* CLK_CR2[3]: DMIC1_CLK_EN. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_DMIC1_CLK_EN_POS              (3)
#define CODEC_DMIC1_CLK_EN_MSK              (0x1 << CODEC_DMIC1_CLK_EN_POS)
#define CODEC_DMIC1_CLK_EN_CLR              (~CODEC_DMIC1_CLK_EN_MSK)
/* CLK_CR2[2:0]: DMIC1_CLK_SEL. */
#define CODEC_DMIC1_CLK_SEL_POS             (0)
#define CODEC_DMIC1_CLK_SEL_MSK             (0x7 << CODEC_DMIC1_CLK_SEL_POS)
#define CODEC_DMIC1_CLK_SEL_CLR             (~CODEC_DMIC1_CLK_SEL_MSK)

/* Register: CLK_CR3 ----------------------------------------------------------*/
/* Description: clock Control register. Offset: 0x110. Address: 0x4002_2110UL. */

/* CLK_CR3[3:0]: SAMPLE_RATE. */
#define CODEC_SAMPLE_RATE_POS               (0)
#define CODEC_SAMPLE_RATE_MSK               (0xF << CODEC_SAMPLE_RATE_POS)
#define CODEC_SAMPLE_RATE_CLR               (~CODEC_SAMPLE_RATE_MSK)

/* Register: I2S_CTRL -------------------------------------------------------*/
/* Description: I2S Control register. Offset: 0x11C. Address: 0x4002_211CUL. */

/* I2S_CTRL[19:18]: I2S_RX_CH. */
#define CODEC_I2S_RX_CH_POS                 (18)
#define CODEC_I2S_RX_CH_MSK                 (0x3 << CODEC_I2S_RX_CH_POS)
#define CODEC_I2S_RX_CH_CLR                 (~CODEC_I2S_RX_CH_MSK)
/* I2S_CTRL[17:16]: I2S_TX_CH. */
#define CODEC_I2S_TX_CH_POS                 (16)
#define CODEC_I2S_TX_CH_MSK                 (0x3 << CODEC_I2S_TX_CH_POS)
#define CODEC_I2S_TX_CH_CLR                 (~CODEC_I2S_TX_CH_MSK)
/* I2S_CTRL[15:14]: I2S_RX_DATA_LEN. */
#define CODEC_I2S_RX_DATA_LEN_POS           (14)
#define CODEC_I2S_RX_DATA_LEN_MSK           (0x3 << CODEC_I2S_RX_DATA_LEN_POS)
#define CODEC_I2S_RX_DATA_LEN_CLR           (~CODEC_I2S_RX_DATA_LEN_MSK)
/* I2S_CTRL[13:12]: I2S_TX_DATA_LEN. */
#define CODEC_I2S_TX_DATA_LEN_POS           (12)
#define CODEC_I2S_TX_DATA_LEN_MSK           (0x3 << CODEC_I2S_TX_DATA_LEN_POS)
#define CODEC_I2S_TX_DATA_LEN_CLR           (~CODEC_I2S_TX_DATA_LEN_MSK)
/* I2S_CTRL[11:10]: I2S_RX_DATA_FORMAT. */
#define CODEC_I2S_RX_DATA_FORMAT_POS        (10)
#define CODEC_I2S_RX_DATA_FORMAT_MSK        (0x3 << CODEC_I2S_RX_DATA_FORMAT_POS)
#define CODEC_I2S_RX_DATA_FORMAT_CLR        (~CODEC_I2S_RX_DATA_FORMAT_MSK)
/* I2S_CTRL[9:8]: I2S_TX_DATA_FORMAT. */
#define CODEC_I2S_TX_DATA_FORMAT_POS        (8)
#define CODEC_I2S_TX_DATA_FORMAT_MSK        (0x3 << CODEC_I2S_TX_DATA_FORMAT_POS)
#define CODEC_I2S_TX_DATA_FORMAT_CLR        (~CODEC_I2S_TX_DATA_FORMAT_MSK)
/* I2S_CTRL[0]: AUDIO_RST_N. 0x1: Powen on. 0x0: Power down. */
#define CODEC_AUDIO_RST_N_POS               (0)
#define CODEC_AUDIO_RST_N_MSK               (0x1 << CODEC_AUDIO_RST_N_POS)
#define CODEC_AUDIO_RST_N_CLR               (~CODEC_AUDIO_RST_N_MSK)

/* Register: ADC0_CTRL0 -------------------------------------------------------*/
/* Description: ADC0 Control register. Offset: 0x120. Address: 0x4002_2120UL. */

/* ADC0_CTRL0[15]: AMIC_DMIC_CH0_SEL. 0x1: ADC path. 0x0: DMIC path. */
#define CODEC_AMIC_DMIC_CH0_SEL_POS         (15)
#define CODEC_AMIC_DMIC_CH0_SEL_MSK         (0x1 << CODEC_AMIC_DMIC_CH0_SEL_POS)
#define CODEC_AMIC_DMIC_CH0_SEL_CLR         (~CODEC_AMIC_DMIC_CH0_SEL_MSK)\
    /* ADC0_CTRL0[14:13]: ADC_CH0_BOOST_GAIN. */
#define CODEC_ADC_CH0_BOOST_GAIN_POS        (13)
#define CODEC_ADC_CH0_BOOST_GAIN_MSK        (0x3 << CODEC_ADC_CH0_BOOST_GAIN_POS)
#define CODEC_ADC_CH0_BOOST_GAIN_CLR        (~CODEC_ADC_CH0_BOOST_GAIN_MSK)
/* ADC0_CTRL0[12]: ADC_CH0_AD_MUTE. */
#define CODEC_ADC_CH0_AD_MUTE_POS           (12)
#define CODEC_ADC_CH0_AD_MUTE_MSK           (0x1 << CODEC_ADC_CH0_AD_MUTE_POS)
#define CODEC_ADC_CH0_AD_MUTE_CLR           (~CODEC_ADC_CH0_AD_MUTE_MSK)
/* ADC0_CTRL0[11:10]: ADC_CH0_ZDET_TOUT. */
#define CODEC_ADC_CH0_ZDET_TOUT_POS         (10)
#define CODEC_ADC_CH0_ZDET_TOUT_MSK         (0x3 << CODEC_ADC_CH0_ZDET_TOUT_POS)
#define CODEC_ADC_CH0_ZDET_TOUT_CLR         (~CODEC_ADC_CH0_ZDET_TOUT_MSK)
/* ADC0_CTRL0[9:8]: ADC_CH0_ZDET_FUN. 0x1: ADC path. 0x0: DMIC path. */
#define CODEC_ADC_CH0_ZDET_FUN_POS          (8)
#define CODEC_ADC_CH0_ZDET_FUN_MSK          (0x3 << CODEC_ADC_CH0_ZDET_FUN_POS)
#define CODEC_ADC_CH0_ZDET_FUN_CLR          (~CODEC_ADC_CH0_ZDET_FUN_MSK)
#define CODEC_ADC_CH0_ZDET_FUN_DEFAULT_MSK  (0x1 << CODEC_ADC_CH0_ZDET_FUN_POS)

/* ADC0_CTRL0[1:0]: DMIC_CH0_RI_FA_SEL. 0x1: Falling latching. 0x0: Rising latching. */
#define CODEC_DMIC_CH0_RI_FA_SEL_POS        (0)
#define CODEC_DMIC_CH0_RI_FA_SEL_MSK        (0x3 << CODEC_DMIC_CH0_RI_FA_SEL_POS)
#define CODEC_DMIC_CH0_RI_FA_SEL_CLR        (~CODEC_DMIC_CH0_RI_FA_SEL_MSK)
#define CODEC_ADC0_CTRL0_REG_DEFAULT_MSK    ((0x7 << 17) | (0x01 << 8) | (0x03 <<4))

/* Register: ADC1_CTRL0 -------------------------------------------------------*/
/* Description: ADC1 Control register. Offset: 0x128. Address: 0x4002_2128UL. */

/* ADC1_CTRL0[15]: AMIC_DMIC_CH1_SEL. 0x1: ADC path. 0x0: DMIC path. */
#define CODEC_AMIC_DMIC_CH1_SEL_POS         (15)
#define CODEC_AMIC_DMIC_CH1_SEL_MSK         (0x1 << CODEC_AMIC_DMIC_CH1_SEL_POS)
#define CODEC_AMIC_DMIC_CH1_SEL_CLR         (~CODEC_AMIC_DMIC_CH1_SEL_MSK)
/* ADC1_CTRL0[14:13]: ADC_CH1_BOOST_GAIN. */
#define CODEC_ADC_CH1_BOOST_GAIN_POS        (13)
#define CODEC_ADC_CH1_BOOST_GAIN_MSK        (0x3 << CODEC_ADC_CH1_BOOST_GAIN_POS)
#define CODEC_ADC_CH1_BOOST_GAIN_CLR        (~CODEC_ADC_CH1_BOOST_GAIN_MSK)
/* ADC1_CTRL0[12]: ADC_CH1_AD_MUTE. */
#define CODEC_ADC_CH1_AD_MUTE_POS           (12)
#define CODEC_ADC_CH1_AD_MUTE_MSK           (0x1 << CODEC_ADC_CH1_AD_MUTE_POS)
#define CODEC_ADC_CH1_AD_MUTE_CLR           (~CODEC_ADC_CH1_AD_MUTE_MSK)
/* ADC1_CTRL0[11:10]: ADC_CH1_ZDET_TOUT. */
#define CODEC_ADC_CH1_ZDET_TOUT_POS         (10)
#define CODEC_ADC_CH1_ZDET_TOUT_MSK         (0x3 << CODEC_ADC_CH1_ZDET_TOUT_POS)
#define CODEC_ADC_CH1_ZDET_TOUT_CLR         (~CODEC_ADC_CH1_ZDET_TOUT_MSK)
/* ADC1_CTRL0[1:0]: DMIC_CH1_RI_FA_SEL. 0x1: Falling latching. 0x0: Rising latching. */
#define CODEC_DMIC_CH1_RI_FA_SEL_POS        (0)
#define CODEC_DMIC_CH1_RI_FA_SEL_MSK        (0x3 << CODEC_DMIC_CH1_RI_FA_SEL_POS)
#define CODEC_DMIC_CH1_RI_FA_SEL_CLR        (~CODEC_DMIC_CH1_RI_FA_SEL_MSK)
#define CODEC_ADC0_CTRL1_REG_DEFAULT_MSK    ((0x7 << 17) | (0x01 << 8) | (0x03 <<4))

/* Register: DAC_CTRL1 -------------------------------------------------------*/
/* Description: DAC Control register. Offset: 0x134. Address: 0x4002_2134UL. */

/* DAC_CTRL1[10]: PDM_CLOCK_SEL. */
#define CODEC_PDM_CLOCK_SEL_POS             (10)
#define CODEC_PDM_CLOCK_SEL_MSK             (0x1 << CODEC_PDM_CLOCK_SEL_POS)
#define CODEC_PDM_CLOCK_SEL_CLR             (~CODEC_PDM_CLOCK_SEL_MSK)
/* DAC_CTRL1[9:8]: PDM_CH_SEL. */
#define CODEC_PDM_CH_SEL_POS                (8)
#define CODEC_PDM_CH_SEL_MSK                (0x3 << CODEC_PDM_CH_SEL_POS)
#define CODEC_PDM_CH_SEL_CLR                (~CODEC_PDM_CH_SEL_MSK)
/* DAC_CTRL1[4]: DAC_MUTE. */
#define CODEC_DAC_MUTE_POS                  (4)
#define CODEC_DAC_MUTE_MSK                  (0x1 << CODEC_DAC_MUTE_POS)
#define CODEC_DAC_MUTE_CLR                  (~CODEC_DAC_MUTE_MSK)
/* DAC_CTRL1[3:2]: DAC_ZDET_TOUT. */
#define CODEC_DAC_ZDET_TOUT_POS             (2)
#define CODEC_DAC_ZDET_TOUT_MSK             (0x3 << CODEC_DAC_ZDET_TOUT_POS)
#define CODEC_DAC_ZDET_TOUT_CLR             (~CODEC_DAC_ZDET_TOUT_MSK)
/* DAC_CTRL1[1:0]: DAC_ZDET_FUN. */
#define CODEC_DAC_ZDET_FUN_POS              (0)
#define CODEC_DAC_ZDET_FUN_MSK              (0x3 << CODEC_DAC_ZDET_FUN_POS)
#define CODEC_DAC_ZDET_FUN_CLR              (~CODEC_DAC_ZDET_FUN_MSK)
#define CODEC_DAC_ZDET_DEFAULT_MSK          (0x1 << CODEC_DAC_ZDET_FUN_POS)

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    CODEC_Exported_Constants    Macro Definitions
 *
 * \ingroup     CODEC
 */

#define IS_CODEC_PERIPH(PERIPH) ((PERIPH) == CODEC)

/**
 * \defgroup    Sample_Rate_Definition  Sample Rate Definition
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define SAMPLE_RATE_48KHz                           ((uint32_t)(0x00))
#define SAMPLE_RATE_96KHz                           ((uint32_t)(0x01 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_32KHz                           ((uint32_t)(0x03 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_16KHz                           ((uint32_t)(0x05 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_8KHz                            ((uint32_t)(0x07 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_44100Hz                         ((uint32_t)(0x08 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_88200Hz                         ((uint32_t)(0x09 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_24KHz                           ((uint32_t)(0x0A << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_12KHz                           ((uint32_t)(0x0B << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_22050Hz                         ((uint32_t)(0x0C << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_11025Hz                         ((uint32_t)(0x0D << CODEC_SAMPLE_RATE_POS))
/** \} */

#define IS_SAMPLE_RATE(RATE) (((RATE) == SAMPLE_RATE_48KHz)   || ((RATE) == SAMPLE_RATE_96KHz)   || \
                              ((RATE) == SAMPLE_RATE_32KHz)   || ((RATE) == SAMPLE_RATE_16KHz)   || \
                              ((RATE) == SAMPLE_RATE_8KHz)    || ((RATE) == SAMPLE_RATE_44100Hz) || \
                              ((RATE) == SAMPLE_RATE_88200Hz) || ((RATE) == SAMPLE_RATE_24KHz)   || \
                              ((RATE) == SAMPLE_RATE_12KHz)   || ((RATE) == SAMPLE_RATE_22050Hz) || \
                              ((RATE) == SAMPLE_RATE_11025Hz))

/**
 * \defgroup    I2S_Data_Format I2S Data Format
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_I2S_DataFormat_I2S                    ((uint32_t)(0x00))
#define CODEC_I2S_DataFormat_LeftJustified          ((uint32_t)((0x01 << CODEC_I2S_RX_DATA_FORMAT_POS) | \
                                                                (0x01 << CODEC_I2S_TX_DATA_FORMAT_POS)))
#define CODEC_I2S_DataFormat_PCM_A                  ((uint32_t)((0x02 << CODEC_I2S_RX_DATA_FORMAT_POS) | \
                                                                (0x02 << CODEC_I2S_TX_DATA_FORMAT_POS)))
#define CODEC_I2S_DataFormat_PCM_B                  ((uint32_t)((0x03 << CODEC_I2S_RX_DATA_FORMAT_POS) | \
                                                                (0x03 << CODEC_I2S_TX_DATA_FORMAT_POS)))
/** \} */

#define IS_CODEC_I2S_DATA_FORMAT(FORMAT) (((FORMAT) == CODEC_I2S_DataFormat_I2S) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_LeftJustified) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_PCM_A) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_PCM_B))

/**
 * \defgroup    I2S_Data_Width I2S Data Width
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_I2S_DataWidth_16Bits                  ((uint32_t)(0x00))
#define CODEC_I2S_DataWidth_24Bits                  ((uint32_t)((0x02 << CODEC_I2S_RX_DATA_LEN_POS) | \
                                                                (0x02 << CODEC_I2S_TX_DATA_LEN_POS)))
#define CODEC_I2S_DataWidth_8Bits                   ((uint32_t)((0x03 << CODEC_I2S_RX_DATA_LEN_POS) | \
                                                                (0x03 << CODEC_I2S_TX_DATA_LEN_POS)))
/** \} */
#define IS_CODEC_I2S_DATA_WIDTH(WIDTH) (((WIDTH) == CODEC_I2S_DataWidth_16Bits) || \
                                        ((WIDTH) == CODEC_I2S_DataWidth_24Bits) || \
                                        ((WIDTH) == CODEC_I2S_DataWidth_8Bits))

/**
 * \defgroup    CODEC_I2S_CH CODEC I2S CH
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_I2S_CH_L_R                            ((uint32_t)(0x00))
#define CODEC_I2S_CH_R_L                            ((uint32_t)((0x01 << CODEC_I2S_TX_CH_POS) | \
                                                                (0x01 << CODEC_I2S_RX_CH_POS)))
#define CODEC_I2S_CH_L_L                            ((uint32_t)((0x02 << CODEC_I2S_TX_CH_POS) | \
                                                                (0x02 << CODEC_I2S_RX_CH_POS)))
#define CODEC_I2S_CH_R_R                            ((uint32_t)((0x03 << CODEC_I2S_TX_CH_POS) | \
                                                                (0x03 << CODEC_I2S_RX_CH_POS)))
/** \} */

#define IS_CODEC_I2S_CH(CH)             (((CH) == CODEC_I2S_CH_L_R) || \
                                         ((CH) == CODEC_I2S_CH_R_L) || \
                                         ((CH) == CODEC_I2S_CH_L_L) || \
                                         ((CH) == CODEC_I2S_CH_R_R))

/**
 * \defgroup    CODEC_MIC_BIAS_Config CODEC MIC BIAS Config
 * \{
 * \ingroup     CODEC_Exported_Constants
 */

#define MICBIAS_VOLTAGE_1_507                       ((uint32_t)(0x00))
#define MICBIAS_VOLTAGE_1_62                        ((uint32_t)(0x01 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_1_705                       ((uint32_t)(0x02 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_1_8                         ((uint32_t)(0x03 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_1_906                       ((uint32_t)(0x04 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_2_025                       ((uint32_t)(0x05 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_2_16                        ((uint32_t)(0x06 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_2_314                       ((uint32_t)(0x07 << CODEC_MICBIAS_VSET_POS))
/** \} */

#define IS_MICBIAS_CONFIG(CONFIG) (((CONFIG) == MICBIAS_VOLTAGE_1_507) || ((CONFIG) == MICBIAS_VOLTAGE_1_62) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_1_705) || ((CONFIG) == MICBIAS_VOLTAGE_1_8) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_1_906) || ((CONFIG) == MICBIAS_VOLTAGE_2_025) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_2_16) || ((CONFIG) == MICBIAS_VOLTAGE_2_314))

/**
 * \defgroup    MICBST_Mode MICBST Mode
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define MICBST_Mode_Single                          ((uint32_t)(0x0))
#define MICBST_Mode_Differential                    ((uint32_t)CODEC_MICBST_ENDFL_MSK)
/** \} */
#define IS_MICBST_MODE(MODE)            (((MODE) == MICBST_Mode_Single) || \
                                         ((MODE) == MICBST_Mode_Differential))

/**
 * \defgroup    MICBST_Gain MICBST Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define MICBST_Gain_0dB                             ((uint32_t)(0x0))
#define MICBST_Gain_20dB                            ((uint32_t)(0x1 << CODEC_MICBST_GAIN_POS))
#define MICBST_Gain_30dB                            ((uint32_t)(0x2 << CODEC_MICBST_GAIN_POS))
#define MICBST_Gain_40dB                            ((uint32_t)(0x3 << CODEC_MICBST_GAIN_POS))
/** \} */

#define IS_MICBST_GAIN(GAIN)            (((GAIN) == MICBST_Gain_0dB) || \
                                         ((GAIN) == MICBST_Gain_20dB) || \
                                         ((GAIN) == MICBST_Gain_30dB) || \
                                         ((GAIN) == MICBST_Gain_40dB))

/**
 * \defgroup    DMIC_Clock_Type_Definition DMIC Clock Type Definition
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define DMIC_Clock_5MHz                             ((uint32_t)(0x0))
#define DMIC_Clock_2500KHz                          ((uint32_t)(0x01 << CODEC_DMIC1_CLK_SEL_POS))
#define DMIC_Clock_1250KHz                          ((uint32_t)(0x02 << CODEC_DMIC1_CLK_SEL_POS))
#define DMIC_Clock_625KHz                           ((uint32_t)(0x03 << CODEC_DMIC1_CLK_SEL_POS))
#define DMIC_Clock_312500Hz                         ((uint32_t)(0x04 << CODEC_DMIC1_CLK_SEL_POS))
/** \} */

#define IS_CODEC_DMIC_CLOCK(CLOCK) (((CLOCK) == DMIC_Clock_5MHz) || ((CLOCK) == DMIC_Clock_2500KHz) || \
                                    ((CLOCK) == DMIC_Clock_1250KHz) || ((CLOCK) == DMIC_Clock_625KHz) || \
                                    ((CLOCK) == DMIC_Clock_312500Hz))

/**
 * \defgroup    CODEC_Ch0_Mic_Type CODEC Ch0 Mic Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */

#define CODEC_CH0_MUTE                              (CODEC_ADC_CH0_AD_MUTE_MSK)
#define CODEC_CH0_UNMUTE                            (0)
/** \} */

#define IS_CODEC_CH0_MIC_MUTE(MUTE) (((MUTE) == CODEC_CH0_MUTE) || ((MUTE) == CODEC_CH0_UNMUTE))

/**
 * \defgroup    CODEC_Ch0_Mic_Type CODEC Ch0 Mic Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_CH0_AMIC                              (CODEC_AMIC_DMIC_CH0_SEL_MSK)
#define CODEC_CH0_DMIC                              (0)
/** \} */

#define IS_CODEC_CH0_MIC_TYPE(TYPE) (((TYPE) == CODEC_CH0_AMIC) || ((TYPE) == CODEC_CH0_DMIC))

/**
 * \defgroup    DMIC_Data_Latch_Edge DMIC Data Latch Edge
 * \{
 * \ingroup     CODEC_Exported_Constants
 */

#define DMIC_Ch0_Rising_Latch                        ((uint32_t)(0x00))
#define DMIC_Ch0_Falling_Latch                       ((uint32_t)(0x01 << CODEC_DMIC_CH0_RI_FA_SEL_POS))
/** \} */

#define IS_DMIC_CH0_LATCH_EDGE(EDGE) (((EDGE) == DMIC_Ch0_Rising_Latch) || ((EDGE) == DMIC_Ch0_Falling_Latch))

/**
 * \def         CODEC_AdGain Codec Ad Gain
 */

#define IS_AD_GAIN(GAIN)            (((GAIN)<= 0x7F))

/**
 * \defgroup    Ch0_Boost_Gain Ch0 Boost Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define Ch0_Boost_Gain_0dB                           ((uint32_t)(0x0))
#define Ch0_Boost_Gain_12dB                          ((uint32_t)(0x1 << CODEC_ADC_CH0_BOOST_GAIN_POS))
#define Ch0_Boost_Gain_24dB                          ((uint32_t)(0x2 << CODEC_ADC_CH0_BOOST_GAIN_POS))
#define Ch0_Boost_Gain_36dB                          ((uint32_t)(0x3 << CODEC_ADC_CH0_BOOST_GAIN_POS))
/** \} */

#define IS_CH0_BOOST_GAIN(GAIN)          (((GAIN) == Ch0_Boost_Gain_0dB) || \
                                          ((GAIN) == Ch0_Boost_Gain_12dB) || \
                                          ((GAIN) == Ch0_Boost_Gain_24dB) || \
                                          ((GAIN) == Ch0_Boost_Gain_36dB))

/**
 * \defgroup    Ch0_ADC_Zero_Detection_Timeout Ch0 ADC Zero Detection Timeout
 * \{
 * \ingroup     CODEC_Exported_Constants
 */

#define Ch0_ADC_Zero_DetTimeout_1024_16_Sample       ((uint32_t)(0x00))
#define Ch0_ADC_Zero_DetTimeout_1024_32_Sample       ((uint32_t)(0x01 << CODEC_ADC_CH0_ZDET_TOUT_POS))
#define Ch0_ADC_Zero_DetTimeout_1024_64_Sample       ((uint32_t)(0x02 << CODEC_ADC_CH0_ZDET_TOUT_POS))
#define Ch0_ADC_Zero_DetTimeout_64_Sample            ((uint32_t)(0x03 << CODEC_ADC_CH0_ZDET_TOUT_POS))
/** \} */

#define IS_CH0_ADC_ZERO_DET_TIMEOUT(TIMEOUT) (((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_1024_16_Sample) || \
                                              ((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_1024_32_Sample) || \
                                              ((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_1024_64_Sample) || \
                                              ((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_64_Sample))

/**
 * \defgroup    CODEC_Ch1_Mic_Type CODEC Ch1 Mic Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_CH1_MUTE                               (CODEC_ADC_CH1_AD_MUTE_MSK)
#define CODEC_CH1_UNMUTE                             (0)
/** \} */

#define IS_CODEC_CH1_MIC_MUTE(MUTE) (((MUTE) == CODEC_CH1_MUTE) || ((MUTE) == CODEC_CH1_UNMUTE))

/**
 * \defgroup    CODEC_Ch1_Mic_Type CODEC Ch1 Mic Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_CH1_AMIC                               (CODEC_AMIC_DMIC_CH1_SEL_MSK)
#define CODEC_CH1_DMIC                               (0)
/** \} */

#define IS_CODEC_CH1_MIC_TYPE(TYPE) (((TYPE) == CODEC_CH1_AMIC) || ((TYPE) == CODEC_CH1_DMIC))

/**
 * \defgroup    DMIC_Data_Latch_Edge DMIC Data Latch Edge
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define DMIC_Ch1_Rising_Latch                        ((uint32_t)(0x00))
#define DMIC_Ch1_Falling_Latch                       ((uint32_t)(0x01 << CODEC_DMIC_CH1_RI_FA_SEL_POS))
/** \} */

#define IS_DMIC_CH1_LATCH_EDGE(EDGE) (((EDGE) == DMIC_Ch1_Rising_Latch) || ((EDGE) == DMIC_Ch1_Falling_Latch))

/**
 * \defgroup    Ch1_Boost_Gain Ch1 Boost Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define Ch1_Boost_Gain_0dB                           ((uint32_t)(0x0))
#define Ch1_Boost_Gain_12dB                          ((uint32_t)(0x1 << CODEC_ADC_CH1_BOOST_GAIN_POS))
#define Ch1_Boost_Gain_24dB                          ((uint32_t)(0x2 << CODEC_ADC_CH1_BOOST_GAIN_POS))
#define Ch1_Boost_Gain_36dB                          ((uint32_t)(0x3 << CODEC_ADC_CH1_BOOST_GAIN_POS))
/** \} */

#define IS_CH1_BOOST_GAIN(GAIN)          (((GAIN) == Ch1_Boost_Gain_0dB) || \
                                          ((GAIN) == Ch1_Boost_Gain_12dB) || \
                                          ((GAIN) == Ch1_Boost_Gain_24dB) || \
                                          ((GAIN) == Ch1_Boost_Gain_36dB))

/**
 * \defgroup    Ch1_ADC_Zero_Detection_Timeout Ch1 ADC Zero Detection Timeout
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define Ch1_ADC_Zero_DetTimeout_1024_16_Sample      ((uint32_t)(0x00))
#define Ch1_ADC_Zero_DetTimeout_1024_32_Sample      ((uint32_t)(0x01 << CODEC_ADC_CH1_ZDET_TOUT_POS))
#define Ch1_ADC_Zero_DetTimeout_1024_64_Sample      ((uint32_t)(0x02 << CODEC_ADC_CH1_ZDET_TOUT_POS))
#define Ch1_ADC_Zero_DetTimeout_64_Sample           ((uint32_t)(0x03 << CODEC_ADC_CH1_ZDET_TOUT_POS))
/** \} */

#define IS_CH1_ADC_ZERO_DET_TIMEOUT(TIMEOUT) (((TIMEOUT) == Ch1_ADC_Zero_DetTimeout_1024_16_Sample) || \
                                              ((TIMEOUT) == Ch1_ADC_Zero_DetTimeout_1024_32_Sample) || \
                                              ((TIMEOUT) == Ch1_ADC_Zero_DetTimeout_1024_64_Sample) || \
                                              ((TIMEOUT) == Ch1_ADC_Zero_DetTimeout_64_Sample))

/**
 * \defgroup    PDM_Clock_Edge PDM Clock Edge
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define PDM_Clock_Rising_Edge                       ((uint32_t)(0x00))
#define PDM_Clock_Falling_Edge                      ((uint32_t)CODEC_PDM_CLOCK_SEL_MSK)
/** \} */

#define IS_PDM_Clock_EDGE(EDGE) (((EDGE) == PDM_Clock_Rising_Edge) || ((EDGE) == PDM_Clock_Falling_Edge))

/**
 * \defgroup    PDM_Channel PDM channel
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define PDM_CH_L_L                                  (0)
#define PDM_CH_L_R                                  ((uint32_t)0x01 << CODEC_PDM_CH_SEL_POS)
#define PDM_CH_R_L                                  ((uint32_t)0x02 << CODEC_PDM_CH_SEL_POS)
#define PDM_CH_R_R                                  ((uint32_t)0x03 << CODEC_PDM_CH_SEL_POS)
/** \} */

#define IS_PDM_CH(CH)               (((CH) == PDM_CH_L_L) || ((CH) == PDM_CH_L_R) || \
                                     ((CH) == PDM_CH_R_L) || ((CH) == PDM_CH_R_R))

/**
 * \defgroup    DAC_Mute_Control DAC Mute Control
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define DAC_UuMute                                  ((uint32_t)(0x0))
#define DAC_Mute                                    (CODEC_DAC_MUTE_MSK)
/** \} */

#define IS_DAC_MUTE(MUTE)               (((MUTE) == DAC_UuMute) || ((MUTE) == DAC_Mute))

/**
 * \def     CODEC_Da_Gain Codec Da Gain
 *
 */
#define IS_DA_GAIN(GAIN)            (((GAIN)<= 0xFF))

/**
 * \defgroup    DAC_Zero_Detection_Timeout DAC Zero Detection Timeout
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define DAC_Zero_DetTimeout_1024_16_Sample          ((uint32_t)(0x00 << CODEC_DAC_ZDET_TOUT_POS))
#define DAC_Zero_DetTimeout_1024_32_Sample          ((uint32_t)(0x01 << CODEC_DAC_ZDET_TOUT_POS))
#define DAC_Zero_DetTimeout_1024_64_Sample          ((uint32_t)(0x02 << CODEC_DAC_ZDET_TOUT_POS))
#define DAC_Zero_DetTimeout_256_Sample              ((uint32_t)(0x03 << CODEC_DAC_ZDET_TOUT_POS))
/** \} */

#define IS_DAC_ZERO_DET_TIMEOUT(TIMEOUT) (((TIMEOUT) == DAC_Zero_DetTimeout_1024_16_Sample) || \
                                          ((TIMEOUT) == DAC_Zero_DetTimeout_1024_32_Sample) || \
                                          ((TIMEOUT) == DAC_Zero_DetTimeout_1024_64_Sample) || \
                                          ((TIMEOUT) == DAC_Zero_DetTimeout_256_Sample))

/**
 * \defgroup    EQ_CH_Cmd EQ Channel Cmd
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define EQ_CH_ENABLE                            ((uint32_t)(0x01))
#define EQ_CH_DISABLE                           ((uint32_t)(0x00))
/** \} */
#define IS_EQ_CH_STATUS(STATUS) (((STATUS) == EQ_CH_ENABLE) || \
                                 ((STATUS) == EQ_CH_DISABLE))


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    CODEC_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     CODEC
 */

/**
 * \brief   Deinitializes the CODEC peripheral registers to their default reset values(turn off CODEC clock).
 * \param[in] CODECx: CODEC peripheral selected .
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     CODEC_DeInit(CODEC);
 * }
 * \endcode
 */
void CODEC_DeInit(CODEC_TypeDef *CODECx);

/**
 * \brief   Initializes the CODEC peripheral according to the specified
 *          parameters in the CODEC_InitStruct.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] CODEC_InitStruct: Pointer to a CODEC_InitTypeDef structure that
 *            contains the configuration information for the specified CODEC peripheral
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 */
void CODEC_Init(CODEC_TypeDef *CODECx, CODEC_InitTypeDef *CODEC_InitStruct);

/**
 * \brief   Fills each CODEC_InitStruct member with its default value.
 * \param[in] CODEC_InitStruct: Pointer to a CODEC_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 */
void CODEC_StructInit(CODEC_InitTypeDef *CODEC_InitStruct);

/**
 * \brief   Initializes the CODEC EQ module according to the specified
 *          parameters in the CODEC_EQInitStruct.
 * \param[in] CODEC_EQx: CODEC EQ channel selected.
 * \param[in] CODEC_EQInitStruct: Pointer to a CODEC_EQInitTypeDef structure that
 *            contains the configuration information for the specified CODEC EQ channel.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_eq_init(void)
 * {
 *     CODEC_EQInitTypeDef  CODEC_EQInitStruct;
 *     CODEC_StructInit(&CODEC_EQInitStruct);
 *     CODEC_EQInitStruct. CODEC_EQChCmd    = EQ_CH_Cmd_ENABLE;
 *     CODEC_EQInitStruct.CODEC_EQCoefH0    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB2    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA2    = 0xFF;
 *     CODEC_EQInit(CODEC_EQ1, CODEC_EQInitStruct);
 * }
 * \endcode
 */
void CODEC_EQInit(CODEC_EQTypeDef *CODEC_EQx, CODEC_EQInitTypeDef *CODEC_EQInitStruct);

/**
 * \brief  Fills each CODEC_EQInitStruct member with its default value.
 * \param[in] CODEC_EQInitStruct: Pointer to a CODEC_EQInitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_eq_init(void)
 * {
 *     CODEC_EQInitTypeDef  CODEC_EQInitStruct;
 *     CODEC_StructInit(&CODEC_EQInitStruct);
 *     CODEC_EQInitStruct. CODEC_EQChCmd    = EQ_CH_Cmd_ENABLE;
 *     CODEC_EQInitStruct.CODEC_EQCoefH0    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB2    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA2    = 0xFF;
 *     CODEC_EQInit(CODEC_EQ1, CODEC_EQInitStruct);
 * }
 * \endcode
 */
void CODEC_EQStructInit(CODEC_EQInitTypeDef *CODEC_EQInitStruct);

/**
 * \brief  Enable or disable mic_bias output.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] NewState: New state of MICBIAS.
 *      This parameter can be: ENABLE or DISABLE.
 * \return none.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_MICBIASCmd(CODEC, ENABLE);
 * }
 * \endcode
 */
void CODEC_MICBIASCmd(CODEC_TypeDef *CODECx, FunctionalState NewState);

/**
 * \brief  Reset CODEC.
 * \param[in] CODECx: CODEC peripheral selected.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_Reset(CODEC);
 * }
 * \endcode
 */
__STATIC_INLINE void CODEC_Reset(CODEC_TypeDef *CODECx)
{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));

    /* CODEC IP reset */
    CODECx->CR0 &= CODEC_AUDIO_IP_EN_CLR;;
    CODECx->CR0 = CODEC_AUDIO_IP_EN_MSK;;
}

/**
 * \brief   Config MIC BIAS Vref voltage.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] data: New value of MIC BIAS.
 *   This parameter can be one of the following values:
 *     \arg MICBIAS_VOLTAGE_1_507: Vref voltage is 1.507V.
 *     \arg MICBIAS_VOLTAGE_1_62:  Vref voltage is 1.62V.
 *     \arg MICBIAS_VOLTAGE_1_705: Vref voltage is 1.705V.
 *     \arg MICBIAS_VOLTAGE_1_8: Vref voltage is 1.8V.
 *     \arg MICBIAS_VOLTAGE_1_906: Vref voltage is 1.906V.
 *     \arg MICBIAS_VOLTAGE_2_025:  Vref voltage is 2.025V.
 *     \arg MICBIAS_VOLTAGE_2_16: Vref voltage is 2.16V.
 *     \arg MICBIAS_VOLTAGE_2_314: Vref voltage is 2.314V.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_SetMICBIAS(CODEC, MICBIAS_VOLTAGE_1_507);
 * }
 * \endcode
 */
__STATIC_INLINE void CODEC_SetMICBIAS(CODEC_TypeDef *CODECx,  uint16_t data)

{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));
    assert_param(IS_MICBIAS_CONFIG(data));

    CODEC_ANA->ANA_CR1 &= CODEC_MICBIAS_POW_CLR;
    CODEC_ANA->ANA_CR1 &= CODEC_MICBIAS_VSET_CLR;
    CODEC_ANA->ANA_CR1 |= data;
}

/** \} */ /* End of group CODEC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_CODEC_H_ */


/******************* (C) COPYRIGHT 2019 Realtek Semiconductor *****END OF FILE****/

