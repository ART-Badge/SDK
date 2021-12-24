/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file     io_adc.c
* @brief    This file provides demo code of adc continuous mode.
* @details
* @author   yuan
* @date     2018-12-07
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "app_task.h"
#include "io_adc.h"

#define BATT_MAX_VOLTAGE 4200
#define BATT_MIN_VOLTAGE 3300

static uint32_t power_adcValue = 0;
static uint8_t Power_GetBattUsage(void);
/* Globals ------------------------------------------------------------------*/
ADC_Data_TypeDef ADC_Global_Data;

/**
  * @brief  Initialization adc global data.
  * @param  No parameter.
  * @return void
  */
void global_data_adc_init(void)
{
    /* Initialize adc k value! */
    APP_PRINT_INFO0("[io_adc] global_data_adc_init");
    bool adc_k_status = false;
    adc_k_status = ADC_CalibrationInit();

    if (false == adc_k_status)
    {
        APP_PRINT_ERROR0("[io_adc] global_data_adc_init: ADC_CalibrationInit fail!");
    }

    rt_memset(&ADC_Global_Data, 0, sizeof(ADC_Global_Data));
}

/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  No parameter.
  * @return void
*/
void board_adc_init(void)
{
    Pad_Config(ADC_SAMPLE_PIN_0, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}

/**
  * @brief  Initialize ADC peripheral.
  * @param  No parameter.
  * @return void
  */
void driver_adc_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);

    ADC_InitTypeDef ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);

    /* Configure the ADC sampling schedule0 */
    ADC_InitStruct.ADC_SchIndex[0] = EXT_SINGLE_ENDED(ADC_SAMPLE_CHANNEL_0);
    /* Set the bitmap corresponding to schedule0*/
    ADC_InitStruct.ADC_Bitmap = 0x01;

    #if (ADC_DATA_HW_AVERAGE && ADC_DATA_OUTPUT_TO_FIFO)
    APP_PRINT_ERROR0("[io_adc] driver_adc_init: ADC config error !");
    #elif (ADC_DATA_HW_AVERAGE)
    ADC_InitStruct.ADC_DataAvgEn = ADC_DATA_AVERAGE_ENABLE;
    ADC_InitStruct.ADC_DataAvgSel = ADC_DATA_AVERAGE_OF_4;
    #elif (ADC_DATA_OUTPUT_TO_FIFO)
    ADC_InitStruct.ADC_DataWriteToFifo = ADC_DATA_WRITE_TO_FIFO_ENABLE;
    ADC_InitStruct.ADC_FifoThdLevel = 0x0A;
    #endif

    ADC_InitStruct.ADC_PowerAlwaysOnEn = ADC_POWER_ALWAYS_ON_ENABLE;

    #if (ADC_DATA_HW_AVERAGE || ADC_DATA_OUTPUT_TO_FIFO || ADC_MULTI_SAMPLE)
    /* In one shot mode, if it is necessary to sample at regular intervals,
       the hardware timer TIM7 (which needs to be configured separately) can be used to realize ADC timing sampling.
       After enabling the timer, there is no need to manually enable ADC for each sampling.*/
    ADC_InitStruct.ADC_TimerTriggerEn = ADC_TIMER_TRIGGER_ENABLE;
    #endif
    /* Fixed 255 in OneShot mode. */
    ADC_InitStruct.ADC_SampleTime = 255;

    ADC_Init(ADC, &ADC_InitStruct);

    #if (ADC_MODE_DIVIDE_OR_BYPASS == ADC_BYPASS_MODE)
    /* High bypass resistance mode config, please notice that the input voltage of
      adc channel using high bypass mode should not be over 0.9V */
    ADC_BypassCmd(ADC_SAMPLE_CHANNEL_0, ENABLE);
    APP_PRINT_INFO0("[io_adc] driver_adc_init: ADC sample mode is bypass mode !");
    #else
    ADC_BypassCmd(ADC_SAMPLE_CHANNEL_0, DISABLE);
    APP_PRINT_INFO0("[io_adc] driver_adc_init: ADC sample mode is divide mode !");
    #endif

    #if (!ADC_DATA_OUTPUT_TO_FIFO)
    ADC_INTConfig(ADC, ADC_INT_ONE_SHOT_DONE, ENABLE);
    #else
    ADC_INTConfig(ADC, ADC_INT_FIFO_THD, ENABLE);
    #endif

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    /* When ADC is enabled, sampling will be done quickly and interruption will occur.
       After initialization, ADC can be enabled when sampling is needed.*/
    //    ADC_Cmd(ADC, ADC_ONE_SHOT_MODE, ENABLE);
}

/**
  * @brief  Initialize timer peripheral for adc one shot mode.
  * @param  No parameter.
  * @return void
*/
void driver_adc_timer_init(void)
{
    /* Only timer7 can be used as a hardware timer for timing sampling of ADC one shot mode. */
    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    TIM_StructInit(&TIM_InitStruct);

    TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
    /* Timing settings for timer see IO TIM demo. */
    TIM_InitStruct.TIM_Period = ADC_ONE_SHOT_SAMPLE_PERIOD - 1; //sampling once 1 second
    TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
    TIM_InitStruct.TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_40;
    TIM_TimeBaseInit(TIM7, &TIM_InitStruct);

    TIM_ClearINT(TIM7);
    TIM_INTConfig(TIM7, ENABLE);
    //    TIM_Cmd(TIM7, ENABLE);
}

/**
  * @brief  Calculate adc sample voltage.
  * @param  No parameter.
  * @return void
  */
static void io_adc_voltage_calculate(T_IO_MSG *io_adc_msg)
{
    ADC_Data_TypeDef *p_buf = io_adc_msg->u.buf;
    uint8_t sample_data_len = 0;

    sample_data_len = p_buf->RawDataLen;

    for (uint8_t i = 0; i < sample_data_len; i++)
    {
        power_adcValue = p_buf->RawData[i];
        power_adcValue = (p_buf->RawData[i] & 0x3FFC) >> 2;
        uint16_t sample_data_decimal = (p_buf->RawData[i] & 0x3);
//        rt_kprintf("power_adcValue: %d, %d\n", power_adcValue, sample_data_decimal);
		
		float sample_voltage = 0;
        ADC_ErrorStatus error_status = NO_ERROR;
        sample_voltage = ADC_GetVoltage(DIVIDE_SINGLE_MODE, (int32_t)power_adcValue, &error_status);
    }

    rt_memset(&ADC_Global_Data, 0, sizeof(ADC_Global_Data));
}

void adc_demo_driver_init(void)
{
    driver_adc_init();
    #if (ADC_DATA_HW_AVERAGE || ADC_DATA_OUTPUT_TO_FIFO || ADC_MULTI_SAMPLE)
    driver_adc_timer_init();
    #endif
}

void adc_sample_start(void)
{
    #if (ADC_DATA_OUTPUT_TO_FIFO)
    ADC_ClearFifo(ADC);
    #endif
    /* Enable adc sample */
    ADC_Cmd(ADC, ADC_ONE_SHOT_MODE, ENABLE);

    #if (ADC_DATA_HW_AVERAGE || ADC_DATA_OUTPUT_TO_FIFO || ADC_MULTI_SAMPLE)
    TIM_Cmd(TIM7, ENABLE);
    #endif
}

/**
  * @brief  Handle adc data function.
  * @param  No parameter.
  * @return void
  */
void io_handle_adc_msg(T_IO_MSG *io_adc_msg)
{
//    power_adcValue = io_adc_voltage_calculate(io_adc_msg);
//    Power_GetBattUsage();
}

/**
  * @brief  ADC interrupt handler function.
  * @param  No parameter.
  * @return void
  */
void ADC_Handler(void)
{
    if (ADC_GetINTStatus(ADC, ADC_INT_ONE_SHOT_DONE) == SET)
    {
        ADC_ClearINTPendingBit(ADC, ADC_INT_ONE_SHOT_DONE);

        uint16_t sample_data = 0;
        sample_data = ADC_ReadRawData(ADC, ADC_Schedule_Index_0);

        T_IO_MSG int_adc_msg;
        int_adc_msg.type = IO_MSG_TYPE_ADC;
        int_adc_msg.subtype = 0;
        ADC_Global_Data.RawDataLen = 1;
        ADC_Global_Data.RawData[0] = sample_data;
        int_adc_msg.u.buf = (void *)(&ADC_Global_Data);
        io_adc_voltage_calculate(&int_adc_msg);

        extern void hws_battery_level_update(rt_uint8_t level);
        // dcm 发送百分比数据
        hws_battery_level_update(Power_GetBattUsage());

        APP_PRINT_ERROR0("[io_adc] ADC_Handler: Send int_adc_msg failed!");
        //Add user code here!

        TIM_Cmd(TIM7, DISABLE);
        ADC_ClearINTPendingBit(ADC, ADC_INT_ONE_SHOT_DONE);
        return;
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    const long dividend = out_max - out_min;
    const long divisor = in_max - in_min;
    const long delta = x - in_min;

    if (divisor == 0)
    {
        rt_kprintf("Invalid map input range, min == max");
        return -1;
    }

    return (delta * dividend + (divisor / 2)) / divisor + out_min;
}

// 660-3.63	925-4.235
static uint8_t Power_GetBattUsage(void)
{
    int voltage = map(
                      power_adcValue,
                      0, 4095,
                      0, 3300);

    voltage *= 5.6;

    __LimitValue(voltage, BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE);

    int usage = map(
                    voltage,
                    BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE,
                    0, 100);

    rt_kprintf("voltage3 present is: %d%\n", usage);

    return usage;
}

void dcm_voltage_change_fun(void)
{
    global_data_adc_init();
    board_adc_init();
    adc_demo_driver_init();
    adc_sample_start();
}

MSH_CMD_EXPORT(dcm_voltage_change_fun, dcm_voltage_change_fun);