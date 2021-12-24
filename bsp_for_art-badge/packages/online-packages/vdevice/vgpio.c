/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-21     stackRyan    first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#define DBG_TAG VDEVICE_PIN_NAME
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#ifdef VDEVICE_USING_PIN

static void _pin_mode(struct rt_device *device, rt_base_t pin, rt_base_t mode)
{
    /* Todo:set pin mode */
    LOG_D("pin_mode: pin_%d,mode_%d", pin, mode);
}

static void _pin_write(struct rt_device *device, rt_base_t pin, rt_base_t value)
{
    /* Todo:set pin low or high */
    LOG_D("pin_write: pin_%d,value_%d", pin, value);
}

static int _pin_read(struct rt_device *device, rt_base_t pin)
{
    /* Todo:get pin status and return status value */
    LOG_D("pin_read: pin_%d,value_0");
    return 0;
}

static rt_err_t _pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                                rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    rt_err_t ret = RT_EOK;

    /* Todo:attach hdr to pin ISR */
    LOG_D("pin_attach_irq: pin_%d,mode_%d", pin, mode);

    return ret;
}

static rt_err_t _pin_detach_irq(struct rt_device *device, rt_int32_t pin)
{
    rt_err_t ret = RT_EOK;

    /* Todo:detach hdr from pin ISR */
    LOG_D("pin_detach_irq: pin_%d", pin);

    return ret;
}

static rt_err_t _pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint32_t enabled)
{
    rt_err_t ret = RT_EOK;

    /* Todo:enable pin ISR */
    LOG_D("pin_irq_enable: pin_%d,Enabled:%d", pin, enabled);

    return ret;
}

const static struct rt_pin_ops _pin_ops =
{
    _pin_mode,
    _pin_write,
    _pin_read,

    _pin_attach_irq,
    _pin_detach_irq,
    _pin_irq_enable
};

int rt_hw_pin_init(void)
{
    rt_err_t ret = RT_EOK;

    ret = rt_device_pin_register(VDEVICE_PIN_NAME, &_pin_ops, RT_NULL);
    LOG_D("rt_hw_pin_init succeed!");
    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_pin_init);

#endif /* PKG_USING_PIN */
