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

#ifdef PKG_USING_VIRTUAL_SENSOR_ACCE

#include "sensor.h"
#include <stdlib.h>

#define DBG_TAG    "v_acce"
#ifdef PKG_USING_VIRTUAL_SENSOR_DBG
    #define DBG_LVL    DBG_LOG
#else
    #define DBG_LVL    DBG_INFO
#endif
#include <rtdbg.h>

enum SENS_ACCE_ID
{
    SENS_ACCE_01 = 0, //Accelerometer
    SENS_ACCE_MAX,
};

#define SENS_BUS_NAME                       "sens_bus"
#define SENS_ACCE_01_SENSOR_ID              (RT_SENSOR_CLASS_ACCE + 0x10)

struct sens_acce
{
    char* dev_name;
    rt_uint8_t sens_id;
};

static struct sens_acce sens_acce_tbl[SENS_ACCE_MAX] =
{
    {V_SENS_ACCE_DEV_NAME,            0x00 }, /* Accelerometer */
};

static struct rt_sensor_info acce_info_tbl[SENS_ACCE_MAX] =
{
    {RT_SENSOR_CLASS_ACCE,  RT_SENSOR_VENDOR_BOSCH,   RT_NULL,    RT_SENSOR_UNIT_MG,      RT_SENSOR_INTF_SPI,     2048,   -2048,   1 },
};

static rt_uint8_t sensor_get_id(rt_uint8_t sens_index)
{
    rt_uint8_t chip_id = 0x00;

    switch (sens_index)
    {
    case SENS_ACCE_01:
        chip_id = SENS_ACCE_01_SENSOR_ID;
        break;
    default:
        break;
    }

    return chip_id;
}

static int sensor_init(rt_uint8_t index)
{
    sens_acce_tbl[index].sens_id = sensor_get_id(index);

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

static rt_size_t acce_sensor_fetch_data(struct rt_sensor_device* sensor, void* buf, rt_size_t len)
{
    struct rt_sensor_data* data = buf;
    rt_int16_t max_range = 0;

    max_range = acce_info_tbl[SENS_ACCE_01].range_max - acce_info_tbl[SENS_ACCE_01].range_min;
    data->type = RT_SENSOR_CLASS_TVOC;
    data->data.acce.x = rand() % max_range + acce_info_tbl[SENS_ACCE_01].range_min;
    data->data.acce.y = rand() % max_range + acce_info_tbl[SENS_ACCE_01].range_min;
    data->data.acce.z = rand() % max_range + acce_info_tbl[SENS_ACCE_01].range_min;
    data->timestamp = rt_sensor_get_ts();
    LOG_D("%s:%d,%d,%d", __func__, data->data.acce.x,
        data->data.acce.y, data->data.acce.z);
    return RT_EOK;
}

static rt_err_t acce_sensor_control(struct rt_sensor_device* sensor, int cmd, void* args)
{
    rt_err_t result = RT_EOK;

    switch (cmd)
    {
    case RT_SENSOR_CTRL_GET_ID:
        *(rt_uint8_t*)args = sens_acce_tbl[SENS_ACCE_01].sens_id;
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
    {acce_sensor_fetch_data, acce_sensor_control},
};

int rt_vd_sens_acce_init(void)
{
    rt_int8_t result;
    rt_uint8_t index = 0;
    rt_sensor_t sensor_dat = RT_NULL;
    struct rt_sensor_config cfg;

    cfg.intf.dev_name = SENS_BUS_NAME;
    cfg.intf.user_data = RT_NULL;
    cfg.irq_pin.pin = RT_PIN_NONE;

    for (index = 0; index < SENS_ACCE_MAX; index++)
    {
        _sensor_create(&cfg.intf, index);
        sensor_dat = rt_calloc(1, sizeof(struct rt_sensor_device));
        if (sensor_dat == RT_NULL)
        {
            LOG_E("rt_hw_sens_init:rt_calloc err!");
            return -RT_ERROR;
        }

        sensor_dat->info.type = acce_info_tbl[index].type;
        sensor_dat->info.vendor = acce_info_tbl[index].vendor;
        sensor_dat->info.model = acce_info_tbl[index].model;
        sensor_dat->info.unit = acce_info_tbl[index].unit;
        sensor_dat->info.intf_type = acce_info_tbl[index].intf_type;
        sensor_dat->info.range_max = acce_info_tbl[index].range_max;
        sensor_dat->info.range_min = acce_info_tbl[index].range_min;
        sensor_dat->info.period_min = acce_info_tbl[index].period_min;

        rt_memcpy(&sensor_dat->config, &cfg, sizeof(struct rt_sensor_config));
        sensor_dat->ops = &sensor_ops[index];

        result = rt_hw_sensor_register(sensor_dat, sens_acce_tbl[index].dev_name, RT_DEVICE_FLAG_RDWR, RT_NULL);
        if (result != RT_EOK)
        {
            LOG_E("device register err code: %d", result);
            rt_free(sensor_dat);
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

INIT_DEVICE_EXPORT(rt_vd_sens_acce_init);

#endif

