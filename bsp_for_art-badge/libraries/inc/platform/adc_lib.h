/**
*****************************************************************************************
*     Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file      adc_lib.h
* @brief     Header file.
* @author    yuan
* @date      2020-08-18
* @version   v1.1.0
**************************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2020 Realtek Semiconductor Corporation</center></h2>
**************************************************************************************
*/
#ifndef __ADC_K_LIB_H
#define __ADC_K_LIB_H

#include <stdbool.h>
#include <stdint.h>

#define SOC_VERSION_CHECK               1

#define ADC_K_VERSION_N2                1
#define ADC_K_VERSION_N1                1
#define ADC_K_VERSION_0                 1
#define ADC_K_VERSION_P1                1

#define USE_ADC_DIVIDE_SINGLE_MODE      1
#define USE_ADC_BYPASS_SINGLE_MODE      1
#define USE_ADC_DIVIDE_DIFF_MODE        1
#define USE_ADC_BYPASS_DIFF_MODE        1

typedef enum
{
    DIVIDE_SINGLE_MODE = 1,
    BYPASS_SINGLE_MODE = 2,
    DIVIDE_DIFFERENTIAL_MODE = 3,
    BYPASS_DIFFERENTIAL_MODE = 4,
} ADC_SampleMode;

typedef enum
{
    NO_ERROR = 0,
    PARAMETER_ERROR = -1,
    RAM_DATA_ERROR = -2,
    NO_CALIBRATION = -3,
    VERSION_ERROR = -4,
} ADC_ErrorStatus;

extern int8_t ADC_K_Version;
extern uint16_t ADC_Resistance_Value;

bool ADC_CalibrationInit(void);
float ADC_GetVoltage(const ADC_SampleMode vSampleMode, int32_t vSampleData,
                     ADC_ErrorStatus *pErrorStatus);
uint16_t ADC_GetResistance(void);




#endif
