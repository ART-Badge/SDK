/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-21     stackRyan    first version
 */

#include "rtconfig.h"

#ifdef VDEVICE_USING_TOUCH

#include <rtthread.h>
#include <rtdevice.h>
#include "string.h"
#include <touch.h>

#define DBG_TAG    VDEVICE_TOUCH_NAME
#define DBG_LVL    DBG_LOG
#include <rtdbg.h>

#define TOUCH_WIDTH     240     //resolution ratio config X
#define TOUCH_HEIGHT    240     //resolution ratio config Y
#define TOUCH_POINT_NUM     1   //touchpoint number

static struct rt_touch_device touch_device;

static rt_size_t device_touch_readpoint(struct rt_touch_device *touch, void *buf, rt_size_t touch_num)
{
    struct rt_touch_data touch_data = {0};
    /*make a fake touch data and event*/
    touch_data.event = RT_TOUCH_EVENT_UP;
    touch_data.x_coordinate = 100;
    touch_data.y_coordinate = 100;

    rt_memcpy(buf, &touch_data, sizeof(struct rt_touch_data));

    LOG_D("_touch_readpoint");
    return touch_num;
}

static rt_err_t device_touch_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    RT_ASSERT(touch != RT_NULL);

    switch (cmd)
    {
    case RT_TOUCH_CTRL_GET_ID:
    {
        *(rt_uint8_t *)arg = 0x5a;
        LOG_D("RT_TOUCH_CTRL_GET_ID: %d", *(rt_uint8_t *)arg);
        break;
    }

    case RT_TOUCH_CTRL_GET_INFO:
    {
        *(rt_uint8_t *)arg = 0xa5;
        LOG_D("RT_TOUCH_CTRL_GET_INFO: %d", *(rt_uint8_t *)arg);
        break;
    }

    case RT_TOUCH_CTRL_SET_MODE:
    {
        /* */
        rt_uint8_t mode = *(rt_uint8_t *)arg;
        LOG_D("RT_TOUCH_CTRL_SET_MODE: %d", mode);
        break;
    }

    case RT_TOUCH_CTRL_SET_X_RANGE:
    {
        touch_device.info.range_x = *(rt_uint32_t *)arg;
        LOG_D("RT_TOUCH_CTRL_SET_X_RANGE: %d", touch_device.info.type);
        break;
    }

    case RT_TOUCH_CTRL_SET_Y_RANGE:
    {
        touch_device.info.range_x = *(rt_uint32_t *)arg;
        LOG_D("RT_TOUCH_CTRL_SET_Y_RANGE: %d", touch_device.info.type);
        break;
    }
    case RT_TOUCH_CTRL_SET_X_TO_Y:
        LOG_D("RT_TOUCH_CTRL_SET_X_TO_Y");
        break;
    case RT_TOUCH_CTRL_DISABLE_INT:
        LOG_D("RT_TOUCH_CTRL_DISABLE_INT");
        break;
    case RT_TOUCH_CTRL_ENABLE_INT:
        LOG_D("RT_TOUCH_CTRL_ENABLE_INT");
        break;
    case RT_TOUCH_CTRL_POWER_ON:
        LOG_D("RT_TOUCH_CTRL_POWER_ON");
        break;
    case RT_TOUCH_CTRL_POWER_OFF:
        LOG_D("RT_TOUCH_CTRL_POWER_OFF");
        break;
    case RT_TOUCH_CTRL_GET_STATUS:
        LOG_D("RT_TOUCH_CTRL_GET_STATUS");
        break;

    default:
        break;
    }

    return RT_EOK;
}

static struct rt_touch_ops touch_ops =
{
    .touch_readpoint = device_touch_readpoint,
    .touch_control = device_touch_control,
};

static int rt_hw_touch_init(void)
{
    rt_err_t result = RT_EOK;
    /* touch infomation */
    touch_device.info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device.info.vendor = RT_TOUCH_VENDOR_UNKNOWN;
    touch_device.info.point_num = TOUCH_POINT_NUM;
    touch_device.info.range_x = TOUCH_WIDTH;
    touch_device.info.range_y = TOUCH_HEIGHT;
    touch_device.config.user_data = RT_NULL;
    touch_device.ops = &touch_ops;

#ifdef RT_TOUCH_USING_PIN
    touch_device.config.irq_pin.pin = RT_PIN_NONE;
    touch_device.config.irq_pin.mode = PIN_MODE_INPUT;
#endif /* RT_TOUCH_USING_PIN */

    result = rt_hw_touch_register(&touch_device, VDEVICE_TOUCH_NAME, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    LOG_D("rt_hw_touch_init succeed!");
    return result;
}
INIT_DEVICE_EXPORT(rt_hw_touch_init);

#endif /* VDEVICE_USING_TOUCH */
