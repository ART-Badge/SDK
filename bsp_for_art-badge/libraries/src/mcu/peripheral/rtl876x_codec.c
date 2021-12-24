/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_codec.c
* @brief    This file provides all the CODEC firmware functions.
* @details
* @author   elliot chen
* @date     2019-10-25
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_codec.h"
#include "rtl876x_pinmux.h"
#include "platform_utils.h"

static bool power_mode_switched = false;
static uint8_t power_mode_reg_value;

/**
  * @brief  Initialize the CODEC analog registers in AON area.
  * @param  None.
  * @retval None
  */
static void CODEC_AnalogCircuitInit(void)
{
    uint8_t reg_value = 0;

    /* Enable ADC 1V1 power cut */
    reg_value = btaon_fast_read_safe(0x110);
    btaon_fast_write(0x110, reg_value | 0x01);

    /* Delay 10us */
    platform_delay_us(10);

    reg_value = btaon_fast_read_safe(0x113);
    btaon_fast_write(0x113, reg_value | 0x01);
    /* Enable ADC 1V8 LDO */
    reg_value = btaon_fast_read_safe(0x110);
    btaon_fast_write(0x110, reg_value | 0x04);

    /* Delay 2ms */
    platform_delay_ms(2);
    /* Disable ADC 1V8 LDO current limit */
    reg_value = btaon_fast_read_safe(0x113);
    btaon_fast_write(0x113, reg_value & (~(0x01)));
}

/**
  * @brief  Deinitialize the CODEC analog registers in AON area.
  * @param  None.
  * @retval None
  */
static void CODEC_AnalogCircuitDeInit(void)
{
    uint8_t reg_value = 0;

    /* Disable  1V1 power cut */
    reg_value = btaon_fast_read_safe(0x110);
    btaon_fast_write(0x110, reg_value & (~0x01));
}

/**
  * @brief  Deinitializes the CODEC peripheral registers to their default reset values(turn off CODEC clock).
  * @param  CODECx: selected CODEC peripheral.
  * @retval None
  */
void CODEC_DeInit(CODEC_TypeDef *CODECx)
{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));

    if (power_mode_switched == true)
    {
        /* restore power mode */
        power_mode_switched = false;
        btaon_fast_write_safe(0x26, power_mode_reg_value);
    }

    RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, DISABLE);

    CODEC_AnalogCircuitDeInit();
}

/**
  * @brief Initializes the CODEC peripheral according to the specified
  *   parameters in the CODEC_InitStruct
  * @param  CODECx: selected CODEC peripheral.
  * @param  CODEC_InitStruct: pointer to a CODEC_InitTypeDef structure that
  *   contains the configuration information for the specified CODEC peripheral
  * @retval None
  */
void CODEC_Init(CODEC_TypeDef *CODECx, CODEC_InitTypeDef *CODEC_InitStruct)
{
    /* Check the parameters */
    /* MIC initialization parameters for input */
    assert_param(IS_CODEC_PERIPH(CODECx));
    assert_param(IS_SAMPLE_RATE(CODEC_InitStruct->CODEC_SampleRate));
    assert_param(IS_CODEC_I2S_DATA_FORMAT(CODEC_InitStruct->CODEC_I2SFormat));
    assert_param(IS_CODEC_I2S_DATA_WIDTH(CODEC_InitStruct->CODEC_I2SDataWidth));
    assert_param(IS_MICBIAS_CONFIG(CODEC_InitStruct->CODEC_MicBIAS));
    assert_param(IS_MICBST_MODE(CODEC_InitStruct->CODEC_MicBstMode));
    assert_param(IS_MICBST_GAIN(CODEC_InitStruct->CODEC_MicBstGain));
    assert_param(IS_CODEC_DMIC_CLOCK(CODEC_InitStruct->CODEC_DmicClock));
    /* MIC channel 0 initialization parameters */
    assert_param(IS_CODEC_CH0_MIC_MUTE(CODEC_InitStruct->CODEC_Ch0Mute));
    assert_param(IS_CODEC_CH0_MIC_TYPE(CODEC_InitStruct->CODEC_Ch0MicType));
    assert_param(IS_DMIC_CH0_LATCH_EDGE(CODEC_InitStruct->CODEC_Ch0DmicDataLatch));
    assert_param(IS_AD_GAIN(CODEC_InitStruct->CODEC_Ch0AdGain));
    assert_param(IS_CH0_BOOST_GAIN(CODEC_InitStruct->CODEC_Ch0BoostGain));
    assert_param(IS_CH0_ADC_ZERO_DET_TIMEOUT(CODEC_InitStruct->CODEC_Ch0ZeroDetTimeout));
    /* MIC channel 1 initialization parameters */
    assert_param(IS_CODEC_CH1_MIC_MUTE(CODEC_InitStruct->CODEC_Ch1Mute));
    assert_param(IS_CODEC_CH1_MIC_TYPE(CODEC_InitStruct->CODEC_Ch1MicType));
    assert_param(IS_DMIC_CH1_LATCH_EDGE(CODEC_InitStruct->CODEC_Ch1DmicDataLatch));
    assert_param(IS_AD_GAIN(CODEC_InitStruct->CODEC_Ch1AdGain));
    assert_param(IS_CH1_BOOST_GAIN(CODEC_InitStruct->CODEC_Ch1BoostGain));
    assert_param(IS_CH1_ADC_ZERO_DET_TIMEOUT(CODEC_InitStruct->CODEC_Ch1ZeroDetTimeout));
    /* PDM initialization parameters */
    assert_param(IS_PDM_Clock_EDGE(CODEC_InitStruct->CODEC_PDMClockEdge));
    assert_param(IS_PDM_CH(CODEC_InitStruct->CODEC_PDMChannel));
    assert_param(IS_DAC_MUTE(CODEC_InitStruct->CODEC_DaMute));
    assert_param(IS_DA_GAIN(CODEC_InitStruct->CODEC_DaGain));
    assert_param(IS_DAC_ZERO_DET_TIMEOUT(CODEC_InitStruct->CODEC_DacZeroDetTimeout));

    power_mode_reg_value = btaon_fast_read_safe(0x26);
    if (power_mode_reg_value & 0x08)
    {
        /* switch power mode */
        power_mode_switched = true;
        btaon_fast_write_safe(0x26, (power_mode_reg_value & (~0x08)));
    }

    /* Enable audio IP*/
    CODECx->CR0 = CODEC_AUDIO_IP_EN_MSK;

    /* Clock configuration */
    CODECx->CLK_CR1 = CODEC_AD_ANA_CLK_EN_MSK | \
                      CODEC_AD_FIFO_CLK_EN_MSK;
    if (CODEC_InitStruct->CODEC_Ch0Mute == CODEC_CH0_UNMUTE)
    {
        CODECx->CLK_CR1 |= CODEC_AD_FILTER_CH0_CLK_MSK | \
                           CODEC_AD_CH0_CLK_MSK;
    }
    if (CODEC_InitStruct->CODEC_Ch1Mute == CODEC_CH1_UNMUTE)
    {
        CODECx->CLK_CR1 |= CODEC_AD_FILTER_CH1_CLK_MSK | \
                           CODEC_AD_CH1_CLK_MSK;
    }
    if (CODEC_InitStruct->CODEC_DaMute != DAC_Mute)
    {
        CODECx->CLK_CR1 |= CODEC_DA_FIFO_CLK_EN_MSK | \
                           CODEC_PDM_CLK_EN_MSK     | \
                           CODEC_DA_CLK_EN_MSK;
    }

    /* DMIC clock configuration */
    if (((CODEC_InitStruct->CODEC_Ch0Mute   == CODEC_CH0_UNMUTE) && \
         (CODEC_InitStruct->CODEC_Ch0MicType == CODEC_CH0_DMIC))  || \
        ((CODEC_InitStruct->CODEC_Ch1Mute   == CODEC_CH1_UNMUTE) && \
         (CODEC_InitStruct->CODEC_Ch1MicType == CODEC_CH1_DMIC)))
    {
        CODECx->CLK_CR2 = CODEC_DMIC1_CLK_EN_MSK | \
                          CODEC_InitStruct->CODEC_DmicClock;
    }
    CODECx->CLK_CR3 = CODEC_InitStruct->CODEC_SampleRate;

    /* Analog initialization */
    if (((CODEC_InitStruct->CODEC_Ch0Mute   == CODEC_CH0_UNMUTE) && \
         (CODEC_InitStruct->CODEC_Ch0MicType == CODEC_CH0_AMIC))  || \
        ((CODEC_InitStruct->CODEC_Ch1Mute   == CODEC_CH1_UNMUTE) && \
         (CODEC_InitStruct->CODEC_Ch1MicType == CODEC_CH1_AMIC)))
    {
        /* Analog initialization in AON register */
        CODEC_AnalogCircuitInit();

        if (CODEC_InitStruct->CODEC_MicBstMode == MICBST_Mode_Single)
        {
            uint8_t reg_value = 0;
            /* Enable ADC 1V1 power cut */
            reg_value = btaon_fast_read_safe(0x113);
            btaon_fast_write(0x113, reg_value | 0x02);
        }

        /* Enable AD/DA clock and ADC analog power */
        CODEC_ANA->ANA_CR0 = CODEC_ADC_ANA_POW_MSK  | \
                             CODEC_DTSDM_CLK_EN_MSK | \
                             CODEC_DAC_ADDACK_POW_MSK;

        CODEC_ANA->ANA_CR1 = CODEC_VREF_SEL_DEFALUT_MSK         | \
                             CODEC_MICBST_POW_MSK               | \
                             CODEC_MICBST_MUTE_MIC_MSK          | \
                             CODEC_InitStruct->CODEC_MicBIAS    | \
                             CODEC_InitStruct->CODEC_MicBstMode | \
                             CODEC_MICBST_VREF_POW_MSK          | \
                             CODEC_InitStruct->CODEC_MicBstGain | \
                             CODEC_MICBIAS_POW_MSK              | \
                             CODEC_MICBIAS_ENCHX_MSK;

        CODEC_ANA->ANA_CR2 = CODEC_MICBIAS_POWSHDT_DEFALUT_MSK  | \
                             CODEC_MICBIAS_OCSEL_DEFALUT_MSK    | \
                             CODEC_MICBIAS_COUNT_DEFALUT_MSK;
        /* Clock enable */
        CODECx->ANA_CR1 = CODEC_CKX_MICBIAS_EN_MSK;
    }

    /* Configure I2S parameters */
    CODECx->I2S_CTRL = CODEC_InitStruct->CODEC_I2SChSequence | \
                       CODEC_InitStruct->CODEC_I2SDataWidth  | \
                       CODEC_InitStruct->CODEC_I2SFormat     | \
                       CODEC_AUDIO_RST_N_MSK;

    /* Configuer mic channel 0 parameters */
    CODECx->ADC0_CTRL0 = CODEC_InitStruct->CODEC_Ch0MicType         | \
                         CODEC_InitStruct->CODEC_Ch0Mute            | \
                         CODEC_InitStruct->CODEC_Ch0MicType         | \
                         CODEC_InitStruct->CODEC_Ch0DmicDataLatch   | \
                         CODEC_InitStruct->CODEC_Ch0BoostGain       | \
                         CODEC_InitStruct->CODEC_Ch0ZeroDetTimeout  | \
                         CODEC_ADC0_CTRL0_REG_DEFAULT_MSK;
    CODECx->ADC0_CTRL1 = CODEC_InitStruct->CODEC_Ch0AdGain;

    /* Configuer mic channel 1 parameters */
    CODECx->ADC1_CTRL0 = CODEC_InitStruct->CODEC_Ch1MicType         | \
                         CODEC_InitStruct->CODEC_Ch1Mute            | \
                         CODEC_InitStruct->CODEC_Ch1MicType         | \
                         CODEC_InitStruct->CODEC_Ch1DmicDataLatch   | \
                         CODEC_InitStruct->CODEC_Ch1BoostGain       | \
                         CODEC_InitStruct->CODEC_Ch1ZeroDetTimeout  | \
                         CODEC_ADC0_CTRL1_REG_DEFAULT_MSK;
    CODECx->ADC1_CTRL1 = CODEC_InitStruct->CODEC_Ch1AdGain;

    /* Configure PDM parametes */
    CODECx->DAC_CTRL1 = CODEC_InitStruct->CODEC_PDMClockEdge        | \
                        CODEC_InitStruct->CODEC_PDMChannel          | \
                        CODEC_InitStruct->CODEC_DaMute              | \
                        CODEC_InitStruct->CODEC_DacZeroDetTimeout   | \
                        CODEC_DAC_ZDET_DEFAULT_MSK;
    CODECx->DAC_CTRL0 =  CODEC_InitStruct->CODEC_DaGain;
}

/**
  * @brief  Fills each CODEC_InitStruct member with its default value.
  * @param  CODEC_InitStruct: pointer to an CODEC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void CODEC_StructInit(CODEC_InitTypeDef *CODEC_InitStruct)
{
    /* Basic parameters section */
    CODEC_InitStruct->CODEC_SampleRate          = SAMPLE_RATE_16KHz;
    CODEC_InitStruct->CODEC_I2SFormat           = CODEC_I2S_DataFormat_I2S;
    CODEC_InitStruct->CODEC_I2SDataWidth        = CODEC_I2S_DataWidth_16Bits;
    CODEC_InitStruct->CODEC_I2SChSequence       = CODEC_I2S_CH_L_L;
    CODEC_InitStruct->CODEC_MicBIAS             = MICBIAS_VOLTAGE_1_8;
    CODEC_InitStruct->CODEC_MicBstMode          = MICBST_Mode_Single;
    CODEC_InitStruct->CODEC_MicBstGain          = MICBST_Gain_20dB;
    CODEC_InitStruct->CODEC_DmicClock           = DMIC_Clock_2500KHz;
    /* MIC channel 0 initialization parameters section */
    CODEC_InitStruct->CODEC_Ch0Mute             = CODEC_CH0_UNMUTE;
    CODEC_InitStruct->CODEC_Ch0MicType          = CODEC_CH0_AMIC;
    CODEC_InitStruct->CODEC_Ch0DmicDataLatch    = DMIC_Ch0_Rising_Latch;
    CODEC_InitStruct->CODEC_Ch0AdGain           = 0x2F;
    CODEC_InitStruct->CODEC_Ch0BoostGain        = Ch0_Boost_Gain_0dB;
    CODEC_InitStruct->CODEC_Ch0ZeroDetTimeout   = Ch0_ADC_Zero_DetTimeout_1024_32_Sample;
    /* MIC channel 1 initialization parameters section */
    CODEC_InitStruct->CODEC_Ch1Mute             = CODEC_CH1_MUTE;
    CODEC_InitStruct->CODEC_Ch1MicType          = CODEC_CH1_DMIC;
    CODEC_InitStruct->CODEC_Ch1DmicDataLatch    = DMIC_Ch1_Falling_Latch;
    CODEC_InitStruct->CODEC_Ch1AdGain           = 0x2F;
    CODEC_InitStruct->CODEC_Ch1BoostGain        = Ch1_Boost_Gain_0dB;
    CODEC_InitStruct->CODEC_Ch1ZeroDetTimeout   = Ch1_ADC_Zero_DetTimeout_1024_32_Sample;
    /* PDM initialization parameters section */
    CODEC_InitStruct->CODEC_PDMClockEdge        = PDM_Clock_Rising_Edge;
    CODEC_InitStruct->CODEC_PDMChannel          = PDM_CH_L_L;
    CODEC_InitStruct->CODEC_DaMute              = DAC_Mute;
    CODEC_InitStruct->CODEC_DaGain              = 0xFF;
    CODEC_InitStruct->CODEC_DacZeroDetTimeout   = DAC_Zero_DetTimeout_1024_16_Sample;
}

/**
  * @brief Initializes the CODEC EQ module according to the specified
  *   parameters in the CODEC_EQInitStruct
  * @param  CODEC_EQx: the selected CODEC EQ channel.
  *   which can be CODEC_CH0_EQ1~CODEC_CH0_EQ5, or CODEC_CH1_EQ1~CODEC_CH1_EQ5.
  * @param  CODEC_EQInitStruct: pointer to a CODEC_EQInitTypeDef structure that
  *   contains the configuration information for the specified CODEC EQ channel
  * @retval None
  */
void CODEC_EQInit(CODEC_EQTypeDef *CODEC_EQx, CODEC_EQInitTypeDef *CODEC_EQInitStruct)
{
    uint32_t bit_pos = 0;

    CODEC_EQx->EQ_H0 = CODEC_EQInitStruct->CODEC_EQCoefH0;
    CODEC_EQx->EQ_B1 = CODEC_EQInitStruct->CODEC_EQCoefB1;
    CODEC_EQx->EQ_B2 = CODEC_EQInitStruct->CODEC_EQCoefB2;
    CODEC_EQx->EQ_A1 = CODEC_EQInitStruct->CODEC_EQCoefA1;
    CODEC_EQx->EQ_A2 = CODEC_EQInitStruct->CODEC_EQCoefA2;

    if (((uint32_t)CODEC_EQx) <= CODEC_CH0_EQ5_REG_BASE)
    {
        bit_pos = ((uint32_t)CODEC_EQx - CODEC_CH0_EQ1_REG_BASE) / 20;
        if (CODEC_EQInitStruct->CODEC_EQChCmd != EQ_CH_DISABLE)
        {
            CODEC_CH0_CR |= EQ_CH_ENABLE << bit_pos;
        }
        else
        {
            CODEC_CH0_CR &= ~(EQ_CH_ENABLE << bit_pos);
        }
    }
    else
    {
        bit_pos = ((uint32_t)CODEC_EQx - CODEC_CH1_EQ1_REG_BASE) / 20;
        if (CODEC_EQInitStruct->CODEC_EQChCmd != EQ_CH_DISABLE)
        {
            CODEC_CH1_CR |= EQ_CH_ENABLE << bit_pos;
        }
        else
        {
            CODEC_CH1_CR &= ~(EQ_CH_ENABLE << bit_pos);
        }
    }
}

/**
  * @brief  Fills each CODEC_EQInitStruct member with its default value.
  * @param  CODEC_EQInitStruct: pointer to an CODEC_EQInitTypeDef structure which will be initialized.
  * @retval None
  */
void CODEC_EQStructInit(CODEC_EQInitTypeDef *CODEC_EQInitStruct)
{
    CODEC_EQInitStruct->CODEC_EQChCmd   = EQ_CH_DISABLE;
    /*!< The following all parameters can be 0 to 0x7FFFF, whose physical meaning represents a range of -8 to 7.99 */
    CODEC_EQInitStruct->CODEC_EQCoefH0  = 0;
    CODEC_EQInitStruct->CODEC_EQCoefB1  = 0;
    CODEC_EQInitStruct->CODEC_EQCoefB2  = 0;
    CODEC_EQInitStruct->CODEC_EQCoefA1  = 0;
    CODEC_EQInitStruct->CODEC_EQCoefA2  = 0;
}

/**
  * @brief  Enable or disable mic_bias output.
  * @param  CODECx: selected CODEC peripheral.
  * @param  NewState: new state of MICBIAS.
  *   This parameter can be: ENABLE or DISABLE.
  * @return none.
  */
void CODEC_MICBIASCmd(CODEC_TypeDef *CODECx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        if (!(CODEC_ANA->ANA_CR1 & CODEC_MICBIAS_POW_MSK))
        {
            /* Analog initialization in AON register */
            CODEC_AnalogCircuitInit();

            /* MICBIAS power on */
            CODEC_ANA->ANA_CR1 |= CODEC_MICBIAS_ENCHX_MSK       | \
                                  CODEC_MICBST_VREF_POW_MSK     | \
                                  CODEC_MICBIAS_POW_MSK;

            /* Clock enable */
            CODECx->ANA_CR1 = CODEC_CKX_MICBIAS_EN_MSK;
        }
    }
    else
    {
        CODEC_ANA->ANA_CR1 &= CODEC_MICBIAS_POW_CLR;
    }
}

/******************* (C) COPYRIGHT 2019 Realtek Semiconductor Corporation *****END OF FILE****/

