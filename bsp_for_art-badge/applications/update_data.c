/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-06-29     supperthomas  first version
 */
#include <rtthread.h>
#include <dcm.h>
#include <dcmtype.h>
#include <dcm_strg.h>
#include <rtdevice.h>
#include "emq_hub.h"
#include "emq_buf.h"

#define HWS_DCM_POOL_NAME "hws"
#define HWS_DCM_POOL_MAX_NUM 64
const char *dev_beep_name = "pwm2"; //beep device name
const int beep_channel = 0x01;  // default pwm_channel

int dcm_cache_save(struct dcm_pool *pool, const char *key);

static struct dcm_pool *get_hws_dcm(void)
{
    struct dcm_pool *p_dcm_pool = dcm_pool_find(HWS_DCM_POOL_NAME);

    if (p_dcm_pool == RT_NULL)
    {
        rt_kprintf("%s:err!\n", __func__);
    }

    return p_dcm_pool;
}

static void hws_dcm_update_int32(const char *key, rt_int32_t dat)
{
    dt_value_t dcm_v;
    struct dcm_pool *p_dcm_pool = get_hws_dcm();

    if (p_dcm_pool != RT_NULL)
    {
        dt_value_init(&dcm_v);
        dt_pack_int32(&dcm_v, dat);
        dcm_value_set(p_dcm_pool, key, &dcm_v);
    }
}

static rt_err_t hwsSetBeepOn(void)
{
    rt_err_t ret = RT_EOK;
    struct rt_device_pwm *device = RT_NULL;

    device = (struct rt_device_pwm *)rt_device_find(dev_beep_name);

    if (device == RT_NULL)
    {
        rt_kprintf("%s:device find error!", __func__);
        return -RT_ERROR;
    }

    ret = rt_pwm_enable(device, beep_channel);
    return ret;
}

static rt_err_t hwsSetBeepOff(void)
{
    rt_err_t ret = RT_EOK;
    struct rt_device_pwm *device = RT_NULL;

    device = (struct rt_device_pwm *)rt_device_find(dev_beep_name);

    if (device == RT_NULL)
    {
        rt_kprintf("%s:device find error!", __func__);
        return -RT_ERROR;
    }

    ret = rt_pwm_disable(device, beep_channel);
    return ret;
}

static void hws_dcm_update_string(const char *key, const char *buf)
{
    dt_value_t dcm_v;
    struct dcm_pool *p_dcm_pool = get_hws_dcm();

    if (p_dcm_pool != RT_NULL)
    {
        dt_value_init(&dcm_v);
        dt_pack_string(&dcm_v, buf);
        dcm_value_set(p_dcm_pool, key, &dcm_v);
    }
}

static void hws_dcm_update_json_string(const char *key, const char *buf)
{
    dt_value_t dcm_v;
    struct dcm_pool *p_dcm_pool = get_hws_dcm();

    if (p_dcm_pool != RT_NULL)
    {
        dt_value_init(&dcm_v);
        dt_pack_json(&dcm_v, buf);
        dcm_value_set(p_dcm_pool, key, &dcm_v);
    }
}

static rt_int32_t hws_dcm_get_int(const char *key)
{
    const dt_value_t *val = RT_NULL;
    rt_uint16_t type = 0;

    struct dcm_pool *pHWDcmPool = get_hws_dcm();

    if (pHWDcmPool != RT_NULL)
    {
        val = dcm_value_get(pHWDcmPool, key);
        type = dt_get_type(val);

        if (type == DT_TYPE_INT32)
        {
            return dt_unpack_int32(val);
        }
    }

    return 0;
}


struct work_param
{
    struct rt_work work;
    char *key;
    char *pool;
    char *v;
    rt_bool_t is_save;
};

static void do_wrok(struct rt_work *work, void *param)
{
    struct dcm_pool *pool;
    struct work_param *p = param;
    dt_value_t dv;

    pool = dcm_pool_find(p->pool);
    dt_value_init(&dv);
    dt_pack_string(&dv, p->v);
    dcm_value_set(pool, p->key, &dv);

    if(p->is_save == RT_TRUE)
    {
        dcm_cache_del_and_save(pool, p->key);
        dcm_value_set_and_save(pool, p->key, &dv);
    }

//    dcm_cache_save(pool, p->key);
    rt_free(p->key);
    rt_free(p->pool);
    rt_free(p->v);
    rt_free(p);
    rt_thread_mdelay(50);
}

static void push_workqueue(const char *pool, const char *key, const char *v, uint8_t is_save)
{
    struct rt_work *work;
    struct work_param *p;

    p = rt_malloc(sizeof(struct work_param));

    if (p)
    {
        p->key = strdup(key);
        p->pool = strdup(pool);
        p->is_save = is_save;
        p->v = strdup(v);

        if (!p->pool || !p->key)
        {
            rt_free(p->key);
            rt_free(p->pool);
//            rt_free(p->is_save);
            rt_free(p->v);
            rt_free(p);
            return;
        }

        rt_work_init(&p->work, do_wrok, p);
        rt_work_submit(&p->work, 0);
    }
}

static void ui_data_update_save(const char *key, const char *v, rt_bool_t is_save)
{
    push_workqueue("system", key, v, is_save);
}

void ui_name_update(const char *name)
{
    ui_data_update_save("m.p.id.name", name, (rt_bool_t)RT_TRUE);
}

void ui_page_update(const char *name)
{
    ui_data_update_save("m.p.id.tick", name, (rt_bool_t)RT_FALSE);
}

void ui_ble_mac_update(const char *mac)
{
    ui_data_update_save("ble.mac", mac, (rt_bool_t)RT_TRUE);
}

void ui_day_weather_update(const char *day_weather)
{
    ui_data_update_save("m.p.id.day_weather", day_weather, (rt_bool_t)RT_FALSE);
}

void ui_degree_update(const char *min_degree)
{
    ui_data_update_save("m.p.id.degree", min_degree, (rt_bool_t)RT_FALSE);
}

void ui_metting1_update(const char *data)
{
    ui_data_update_save("m.p.id.metting1", data, (rt_bool_t)RT_TRUE);
}

void ui_metting2_update(const char *data)
{
    ui_data_update_save("m.p.id.metting2", data, (rt_bool_t)RT_TRUE);
}

void ui_metting3_update(const char *data)
{
    ui_data_update_save("m.p.id.metting3", data, (rt_bool_t)RT_TRUE);
}

void ui_metting4_update(const char *data)
{
    ui_data_update_save("m.p.id.metting4", data, (rt_bool_t)RT_TRUE);
}

void ui_metting5_update(const char *data)
{
    ui_data_update_save("m.p.id.metting5", data, (rt_bool_t)RT_TRUE);
}

void ui_metting6_update(const char *data)
{
    ui_data_update_save("m.p.id.metting6", data, (rt_bool_t)RT_TRUE);
}

void ui_metting7_update(const char *data)
{
    ui_data_update_save("m.p.id.metting7", data, (rt_bool_t)RT_TRUE);
}

void ui_metting8_update(const char *data)
{
    ui_data_update_save("m.p.id.metting8", data, (rt_bool_t)RT_TRUE);
}

void ui_metting9_update(const char *data)
{
    ui_data_update_save("m.p.id.metting9", data, (rt_bool_t)RT_TRUE);
}

void metting_save_update(const char *data)
{
    ui_data_update_save("m.p.id.save", data, (rt_bool_t)RT_TRUE);
}

void btn_fb_save_update(const char *data)
{
    ui_data_update_save("m.p.id.btn", data, (rt_bool_t)RT_TRUE);
}