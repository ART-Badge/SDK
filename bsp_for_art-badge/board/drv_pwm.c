/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     BalanceTWK   first version
 */

#include <board.h>
#ifdef BSP_USING_PWM
//#define DRV_DEBUG
#define LOG_TAG             "drv.pwm"
#include <drv_log.h>

#include "rtl876x_tim.h"

/** Define pwm output pin.
  * P0_1 is connected to LED0 on the EVB.
  */
#define PWM_OUT_PIN             H_1 /**<GPIO26   32K_XI   */
#define PWM_OUT_PIN_PINMUX      timer_pwm2
/* Config PWM_PERIOD and PWM_DUTY_CYCLE */
#define PWM_PERIOD              1000 //uint:us
#define PWM_DUTY_CYCLE          50      //uint:percent
/* PWM_HIGH_COUNT =  */
#define PWM_HIGH_COUNT          (((PWM_PERIOD)*((PWM_DUTY_CYCLE*40)/100))-1)    //PWM CLOCK = 40000000
#define PWM_LOW_COUNT           (((PWM_PERIOD)*(((100-PWM_DUTY_CYCLE)*40)/100))-1)
/** Config PWM_DEAD_ZONE_TIME. Max 8ms.
  * When dead time is set, PWM2_P and PWM2_N will
  * delay one dead time longer than PWM2 at high output level.
  * When the dead_zone time is longer than the high-level time of PWM,
  * PWM_P will always output low-level.
  * When the dead_zone time is longer than the low-level time of PWM,
  * PWM_N will always output low-level.
  */
#define PWM_DEAD_ZONE_TIME      1 //uint:ms
#define PWM_DEAD_ZONE_SIZE      ((PWM_DEAD_ZONE_TIME)*32-1)


struct rtl876x_pwm
{
    struct rt_device_pwm pwm_device;
    TIM_TypeDef* hw_tim;
    rt_uint32_t RTL876X_Pin_Number;
    rt_uint32_t Pin_Function_Number;
    char *name;
};

static rt_err_t drv_pwm_control(struct rt_device_pwm *device, int cmd, void *arg);
static struct rt_pwm_ops drv_ops =
{
    drv_pwm_control
};

static rt_err_t drv_pwm_enable(struct rtl876x_pwm *pwm, rt_bool_t enable)
{
    if (!enable)
    {
        TIM_Cmd(pwm->hw_tim, DISABLE);
    }
    else
    {
        TIM_Cmd(pwm->hw_tim, ENABLE);
    }

    return RT_EOK;
}

static rt_err_t drv_rtl876x_pwm_get(struct rtl876x_pwm *pwm, struct rt_pwm_configuration *configuration)
{

    return RT_EOK;
}

static rt_err_t drv_rtl876x_pwm_set(struct rtl876x_pwm *pwm, struct rt_pwm_configuration *configuration)
{
    rt_uint32_t high_count, low_count;

    /* !!!!! configuration->period: uint:us */
    high_count = (((configuration->period) * (((configuration->pulse) * 40) / 100)) - 1); /* PWM CLOCK = 40000000 */
    low_count = (((configuration->period) * (((100 - (configuration->pulse)) * 40) / 100)) - 1);

    TIM_Cmd(pwm->hw_tim, DISABLE);
    TIM_PWMChangeFreqAndDuty(pwm->hw_tim, high_count, low_count);
    TIM_Cmd(pwm->hw_tim, ENABLE);

    Pad_Config(pwm->RTL876X_Pin_Number, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);

    return RT_EOK;
}

static rt_err_t drv_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)arg;
    struct rtl876x_pwm *pwm;

    RT_ASSERT(device != RT_NULL);
    pwm = (struct rtl876x_pwm*)(device->parent.user_data);

    switch (cmd)
    {
        case PWM_CMD_ENABLE:
            return drv_pwm_enable(pwm, RT_TRUE);

        case PWM_CMD_DISABLE:
            return drv_pwm_enable(pwm, RT_FALSE);

        case PWM_CMD_SET:
            return drv_rtl876x_pwm_set(pwm, configuration);

        case PWM_CMD_GET:
            return drv_rtl876x_pwm_get(pwm, configuration);

        default:
            return RT_EINVAL;
    }
}

static rt_err_t rtl876x_hw_pwm_init(struct rtl876x_pwm *device)
{
    rt_err_t result = RT_EOK;
    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    RT_ASSERT(device != RT_NULL);

    Pad_Config(device->RTL876X_Pin_Number, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pinmux_Config(device->RTL876X_Pin_Number, device->Pin_Function_Number);

    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);

    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_PWM_En   = PWM_ENABLE;
    TIM_InitStruct.TIM_Mode     = TIM_Mode_UserDefine;
    TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
    TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
    TIM_InitStruct.PWM_Deazone_Size     = PWM_DEAD_ZONE_SIZE;
    TIM_InitStruct.PWMDeadZone_En       = DEADZONE_ENABLE;  //enable to use pwn p/n output
    TIM_TimeBaseInit(device->hw_tim, &TIM_InitStruct);

    TIM_Cmd(device->hw_tim, DISABLE);

    return result;
}

static struct rtl876x_pwm rtl876x_pwm_obj_1;

static int rtl876x_pwm_init(void)
{
    int result = RT_EOK;

    rtl876x_pwm_obj_1.hw_tim = TIM2;
    rtl876x_pwm_obj_1.name = "pwm2";
    rtl876x_pwm_obj_1.RTL876X_Pin_Number = H_1; /**<GPIO26   32K_XI   */
    rtl876x_pwm_obj_1.Pin_Function_Number = timer_pwm2;

    if (rtl876x_hw_pwm_init(&rtl876x_pwm_obj_1) != RT_EOK)
    {
        LOG_E("%s init failed", rtl876x_pwm_obj_1.name);
        result = -RT_ERROR;
        goto __exit;
    }
    else
    {
        LOG_D("%s init success", rtl876x_pwm_obj_1.name);

        /* register pwm device */
        if (rt_device_pwm_register(&rtl876x_pwm_obj_1.pwm_device, rtl876x_pwm_obj_1.name, &drv_ops, &rtl876x_pwm_obj_1) == RT_EOK)
        {
            LOG_D("%s register success", rtl876x_pwm_obj_1.name);
        }
        else
        {
            LOG_E("%s register failed", rtl876x_pwm_obj_1.name);
            result = -RT_ERROR;
        }
    }

__exit:
    return result;
}
INIT_BOARD_EXPORT(rtl876x_pwm_init);

#endif /* RT_USING_PWM */
