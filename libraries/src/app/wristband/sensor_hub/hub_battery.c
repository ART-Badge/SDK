/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file        wrsitband_battery.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "trace.h"
#include "board.h"
#include "string.h"
#include "gap_adv.h"
#include "health_algo.h"
#include "hub_battery.h"
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#include "rtl876x_nvic.h"
#include "rtl876x_adc.h"
#include "bas.h"
#include "wristband_global_data.h"
#include "hub_gsensor.h"
#include "wristband_gap.h"
#include "adc_lib.h"
#include "rtk_gsa.h"
#include "hub_task.h"

#define VOLTAGE_ROW_NUM 15
static uint8_t adc_ft_flag = 0;
static bool battery_has_installed = false;
bool allowed_battery_enter_dlps = true;

static const T_IO_MODULE_CBS battery_cbs =
{
    wristband_battery_driver_init,
    wristband_battery_enter_dlps,
    wristband_battery_exit_dlps,
    wristband_battery_allowed_enter_dlps_check,

};

void battery_add_hub_task(void)
{
    wristband_battery_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&battery_cbs;
    battery_has_installed = true;
}

void wristband_battery_driver_init(void)
{
    Pad_Config(BATTERY_CHECK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pinmux_Config(BATTERY_CHECK, IDLE_MODE);
    /* turn on ADC clock */
    ADC_DeInit(ADC);
    RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);

    /* ADC init */
    ADC_InitTypeDef adcInitStruct;
    ADC_StructInit(&adcInitStruct);
    for (uint8_t i = 0; i < 16; i++)
    {
        adcInitStruct.ADC_SchIndex[i]   = EXT_SINGLE_ENDED(BATTERY_CHECK_INDEX);
    }
    adcInitStruct.ADC_Bitmap            = 0xffff;
    adcInitStruct.ADC_SampleTime        =
        14;//convert time = 0.5us sample time = 1.5us-->(n + 1)*1/10M , total 500K
    ADC_Init(ADC, &adcInitStruct);

    ADC_INTConfig(ADC, ADC_INT_ONE_SHOT_DONE, ENABLE);
    adc_ft_flag = ADC_CalibrationInit();
    RtkWristbandSys.battery_level_value = wristband_get_battery_level();
}

void wristband_battery_enter_dlps(void)
{
    Pad_Config(BATTERY_CHECK, PAD_SW_MODE, PAD_NOT_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}
void wristband_battery_exit_dlps(void)
{
    Pad_Config(BATTERY_CHECK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}
bool wristband_battery_allowed_enter_dlps_check(void)
{
    return allowed_battery_enter_dlps;
}

/*
    100%----4.20V
    90%-----4.06V
    80%-----3.98V
    70%-----3.92V
    60%-----3.87V
    50%-----3.82V
    40%-----3.79V
    30%-----3.77V
    20%-----3.74V
    10%-----3.68V
    5%------3.45V
    0%------3.00V
 */

static uint16_t battery_voltage_table[VOLTAGE_ROW_NUM][2] =
{
#if 1
    {4160, 100},
    {4050, 90},
    {3947, 80},
    {3860, 70},
    {3800, 60},
    {3765, 50},
    {3730, 40},
    {3690, 30},
    {3640, 20},
    {3600, 12}, //low power protect level
    {3580, 10}, //TODO
    {3550, 7},  //TODO
    {3530, 4},  //TODO
    {3500, 2},  //TODO
    {3200, 0},  //TODO
#else
    {3100, 100},
    {3100, 100},
    {3100, 100},
    {3100, 100},
    {3000, 100},
    {3000, 100},
    {3000, 100},
    {3000, 100},
    {2900, 100},
    {2900, 100}, //low power protect level
    {2900, 100}, //TODO
    {2900, 100},  //TODO
    {2500, 100},  //TODO
    {2500, 100},  //TODO
    {0, 100},  //TODO
#endif
};
#if 1
static uint16_t battery_voltage_incharing_table[VOLTAGE_ROW_NUM][2] =
{
    {4160, 100},
    {4094, 90},
    {4044, 80},
    {4005, 70},
    {3972, 60},
    {3942, 50},
    {3914, 40},
    {3891, 30},
    {3857, 20},
    {3807, 12}, //low power protect level
    {3794, 10}, //TODO
    {3766, 7},  //TODO
    {3684, 4},  //TODO
    {3571, 2},  //TODO
    {3406, 0},  //TODO
};
#endif


/**
* @brief    start update newest battery level process
* @param    No parameter.
* @return   void
*/
uint8_t wristband_get_battery_level(void)
{
    if (battery_has_installed == false)
    {
        return 0;
    }
    //enable ADC read and wait for ADC data ready interrupt
    ADC_BypassCmd(BATTERY_CHECK_INDEX, ENABLE);
    ADC_Cmd(ADC, ADC_ONE_SHOT_MODE, ENABLE);
    /* read ADC convert result*/
    while (ADC_GetINTStatus(ADC, ADC_INT_ONE_SHOT_DONE) != SET);
    uint16_t data = ADC_ReadRawData(ADC, 0);//Howie todo change the second paramter to 15
    ADC_Cmd(ADC, ADC_ONE_SHOT_MODE, DISABLE);   //disable for manual power mode
    ADC_ClearINTPendingBit(ADC, ADC_INT_ONE_SHOT_DONE);

    if (adc_ft_flag != 0)
    {
        float direct_voltage = 0;
        ADC_ErrorStatus error_status = NO_ERROR;
        direct_voltage = ADC_GetVoltage(BYPASS_SINGLE_MODE, data, &error_status);//todo by howie
//        direct_voltage = 1000;
//        uint32_t battery_value = direct_voltage;
//        battery_value = (float)1220 / 220 * direct_voltage;

        RtkWristbandSys.battery_vol_value = direct_voltage;
        //APP_PRINT_INFO2("adc raw value = 0x%x; battery voltage = %d mv;", data, battery_value);
    }
    else
    {
        uint16_t voltage = 0.2511 * data - 32.7247;//y = kx + b, R1 = 100K, R2 = 30k
        voltage = (float)1220 / 220 * voltage;
        RtkWristbandSys.battery_vol_value = voltage;
        //APP_PRINT_INFO2("adc raw value = 0x%x; battery voltage = %d mv;", data, voltage);
    }
    /* comment this for release version*/
    RtkWristbandSys.battery_vol_value = 4200;
    uint8_t tmp_level = 0;

    if (RtkWristbandSys.charger_status == InCharging)
    {
        for (uint8_t i = 0; i < VOLTAGE_ROW_NUM; i++)
        {
            if (RtkWristbandSys.battery_vol_value >= battery_voltage_incharing_table[i][0])
            {
                tmp_level = battery_voltage_table[i][1];
                break;
            }
        }
    }
    else if (RtkWristbandSys.charger_status == NoCharge)
    {
        for (uint8_t i = 0; i < VOLTAGE_ROW_NUM; i++)
        {
            if (RtkWristbandSys.battery_vol_value >= battery_voltage_table[i][0])
            {
                tmp_level = battery_voltage_table[i][1];
                break;
            }
        }
    }

    bas_set_parameter(BAS_PARAM_BATTERY_LEVEL, 1, &tmp_level);
    return tmp_level;
}
/**
* @brief      protect battery when in low voltage level.
* @param      No parameter.
* @return     void
*/
void low_battery_protect(void)
{
    RtkWristbandSys.flag_field.low_power_flag = true;
    RtkWristbandSys.current_stationary_time = 0;
    if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
    {
        APP_PRINT_INFO0("Wristband PowerOff, cut off link..\n");
        le_disconnect(RtkWristbandSys.wristband_conn_id);//todo
    }
    if (RtkWristbandSys.gap_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
    {
        APP_PRINT_INFO0("Wristband PowerOff, stop advertising..\n");
        le_adv_stop();//todo
    }
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        T_IO_MSG gsensor_msg;
        gsensor_msg.type = HUB_MSG_GSENSOR;
        gsensor_msg.subtype = GSENSOR_MSG_STOP;
        send_msg_to_ui_task(&gsensor_msg, __LINE__);
    }
}
/**
* @brief     restore to normal mode when battery level rise up
* @param     No parameter.
* @return    void
*/
void low_battery_restore(void)
{
    APP_PRINT_INFO0("Wristband PowerOn,start advertising..\n");
    RtkWristbandSys.flag_field.low_power_flag = false;
    RtkWristbandSys.current_stationary_time = 0;
    wristband_start_adv();
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        T_IO_MSG gsensor_msg;
        gsensor_msg.type = HUB_MSG_GSENSOR;
        gsensor_msg.subtype = GSENSOR_MSG_START;
        send_msg_to_ui_task(&gsensor_msg, __LINE__);
    }
}
/**
* @brief     wristband_power_off
* @param     No parameter.
* @return    void
*/
void wristband_power_off(void)
{
    RtkWristbandSys.flag_field.power_off_flag = true;
    RtkWristbandSys.current_stationary_time = 0;
    if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
    {
        //cut off link
        APP_PRINT_INFO0("Wristband PowerOff, cut off link..\n");
        le_disconnect(RtkWristbandSys.wristband_conn_id);
    }
    if (RtkWristbandSys.gap_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
    {
        //stop advertising
        APP_PRINT_INFO0("Wristband PowerOff, stop advertising..\n");
        le_adv_stop();
    }
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        T_IO_MSG gsensor_msg;
        gsensor_msg.type = HUB_MSG_GSENSOR;
        gsensor_msg.subtype = GSENSOR_MSG_STOP;
        send_msg_to_ui_task(&gsensor_msg, __LINE__);
    }
}
/**
* @brief     wristband_power_on
* @param     No parameter.
* @return    void
*/
void wristband_power_on(void)
{
    APP_PRINT_INFO0("Wristband PowerOn,start advertising..\n");
    RtkWristbandSys.flag_field.power_off_flag = false;
    RtkWristbandSys.current_stationary_time = 0;
    wristband_start_adv();
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        T_IO_MSG gsensor_msg;
        gsensor_msg.type = HUB_MSG_GSENSOR;
        gsensor_msg.subtype = GSENSOR_MSG_START;
        send_msg_to_ui_task(&gsensor_msg, __LINE__);
    }
}

void battery_event_handler(T_IO_MSG msg)
{
    uint8_t value = msg.subtype;
    switch (value)
    {
    case BATTERY_MSG_LOW_POWER:
        {
            break;
        }
    case BATTERY_MSG_RESTORE:
        {
            break;
        }
    default:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            break;
        }
    }
}

