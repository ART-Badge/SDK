/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-19     zhangsz      add virtual sensor device
 */

#include <rtthread.h>

#ifdef PKG_USING_VIRTUAL_SENSOR_FORCE

#include "sensor.h"
#include <stdlib.h>

#define DBG_TAG    "v_force"
#ifdef PKG_USING_VIRTUAL_SENSOR_DBG
    #define DBG_LVL    DBG_LOG
#else
    #define DBG_LVL    DBG_INFO
#endif
#include <rtdbg.h>

enum SENS_FORCE_ID
{
    SENS_FORCE_01 = 0, //Force
    SENS_FORCE_MAX,
};

#define SENS_BUS_NAME                   "sens_bus"
#define SENS_FORCE_01_SENSOR_ID         (RT_SENSOR_CLASS_FORCE + 0x10)

struct sens_force
{
    char* dev_name;
    rt_uint8_t sens_id;
};

static struct sens_force sens_force_tbl[SENS_FORCE_MAX] =
{
    {V_SENS_FORCE_DEV_NAME,               0x00 }, /* Force */
};

static struct rt_sensor_info force_info_tbl[SENS_FORCE_MAX] =
{
    {RT_SENSOR_CLASS_FORCE,  RT_SENSOR_VENDOR_STM,   RT_NULL,    RT_SENSOR_UNIT_MN,      RT_SENSOR_INTF_SPI,     255,   0,   1 },
};

static rt_uint8_t sensor_get_id(rt_uint8_t sens_index)
{
    rt_uint8_t chip_id = 0x00;

    switch (sens_index)
    {
    case SENS_FORCE_01:
        chip_id = SENS_FORCE_01_SENSOR_ID;
        break;
    default:
        break;
    }

    return chip_id;
}

static int sensor_init(rt_uint8_t index)
{
    sens_force_tbl[index].sens_id = sensor_get_id(index);

    return RT_EOK;
}

static void* _sensor_create(struct rt_sensor_intf* intf, rt_uint8_t index)
{
    if (sensor_init(index) != RT_EOK)
    {
        LOG_E("%s:error!", __func__);
    }

    return 0;
}

static rt_err_t _sensor_set_odr(rt_sensor_t sensor, rt_uint16_t odr)
{
    LOG_D("%s:odr=%d", __func__, odr);
    return RT_EOK;
}
static rt_err_t _sensor_set_range(rt_sensor_t sensor, rt_uint16_t range)
{
    LOG_D("%s:range=%d", __func__, range);
    return RT_EOK;
}

static rt_err_t _sensor_set_power(rt_sensor_t sensor, rt_uint8_t power)
{
    rt_int8_t rslt = 0;
    LOG_D("%s:power=%d", __func__, power);
    return rslt;
}

static rt_size_t force_sensor_fetch_data(struct rt_sensor_device* sensor, void* buf, rt_size_t len)
{
    struct rt_sensor_data* data = buf;
    rt_int16_t max_range = 0;

    max_range = force_info_tbl[SENS_FORCE_01].range_max - force_info_tbl[SENS_FORCE_01].range_min;
    data->type = RT_SENSOR_CLASS_FORCE;
    data->data.force = rand() % max_range + force_info_tbl[SENS_FORCE_01].range_min;
    data->timestamp = rt_sensor_get_ts();
    LOG_D("%s:%d", __func__, data->data.force);

    return RT_EOK;
}

static rt_err_t force_sensor_control(struct rt_sensor_device* sensor, int cmd, void* args)
{
    rt_err_t result = RT_EOK;

    switch (cmd)
    {
    case RT_SENSOR_CTRL_GET_ID:
        *(rt_uint8_t*)args = sens_force_tbl[SENS_FORCE_01].sens_id;
        break;
    case RT_SENSOR_CTRL_SET_ODR:
        result = _sensor_set_odr(sensor, (rt_uint32_t)args & 0xffff);
        break;
    case RT_SENSOR_CTRL_SET_RANGE:
        result = _sensor_set_range(sensor, (rt_uint32_t)args);
        break;
    case RT_SENSOR_CTRL_SET_POWER:
        result = _sensor_set_power(sensor, (rt_uint32_t)args & 0xff);
        break;
    case RT_SENSOR_CTRL_SELF_TEST:
        /* TODO */
        result = -RT_EINVAL;
        break;
    default:
        return -RT_EINVAL;
    }
    return result;
}

static struct rt_sensor_ops sensor_ops[] =
{
    {force_sensor_fetch_data, force_sensor_control},
};

int rt_vd_sens_force_init(void)
{
    rt_int8_t result;
    rt_uint8_t index = 0;
    rt_sensor_t sensor_dat = RT_NULL;
    struct rt_sensor_config cfg;

    cfg.intf.dev_name = SENS_BUS_NAME;
    cfg.intf.user_data = RT_NULL;
    cfg.irq_pin.pin = RT_PIN_NONE;

    for (index = 0; index < SENS_FORCE_MAX; index++)
    {
        _sensor_create(&cfg.intf, index);
        sensor_dat = rt_calloc(1, sizeof(struct rt_sensor_device));
        if (sensor_dat == RT_NULL)
        {
            LOG_E("rt_hw_sens_init:rt_calloc err!");
            return -RT_ERROR;
        }

        sensor_dat->info.type = force_info_tbl[index].type;
        sensor_dat->info.vendor = force_info_tbl[index].vendor;
        sensor_dat->info.model = force_info_tbl[index].model;
        sensor_dat->info.unit = force_info_tbl[index].unit;
        sensor_dat->info.intf_type = force_info_tbl[index].intf_type;
        sensor_dat->info.range_max = force_info_tbl[index].range_max;
        sensor_dat->info.range_min = force_info_tbl[index].range_min;
        sensor_dat->info.period_min = force_info_tbl[index].period_min;

        rt_memcpy(&sensor_dat->config, &cfg, sizeof(struct rt_sensor_config));
        sensor_dat->ops = &sensor_ops[index];

        result = rt_hw_sensor_register(sensor_dat, sens_force_tbl[index].dev_name, RT_DEVICE_FLAG_RDWR, RT_NULL);
        if (result != RT_EOK)
        {
            LOG_E("device register err code: %d", result);
            rt_free(sensor_dat);
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

INIT_DEVICE_EXPORT(rt_vd_sens_force_init);

#endif

