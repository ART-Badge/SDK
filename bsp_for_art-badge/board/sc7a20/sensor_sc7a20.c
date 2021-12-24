#include <rtthread.h>
#include "sensor_sc7a20.h"

#define DBG_TAG "sensor.sc7a20"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define FIFO_LEN_MAX 16
#define ADDR_PING_HIGH

typedef struct rt_sensor_intf rt_sensor_intf_t;
typedef struct rt_sensor_ops rt_sensor_ops_t;

static rt_size_t sc7a20_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len);
static rt_err_t sc7a20_control(struct rt_sensor_device *sensor, int cmd, void *args);
static rt_sensor_ops_t sensor_ops =
{
    .fetch_data     =   sc7a20_fetch_data,
    .control        =   sc7a20_control,
};

//rt_sensor_t sc7a20 =
//{
//    parent;
//
//    info;
//    config;
//
//    data_buf;
//    data_len;
//
//    ops;
//
//    module;
//
//    irq_handle;
//};

// static init_mode()
// {
//     rt_uint8_t ctrlreg1 = 0x97;
//     rt_uint8_t ctrlreg2 = 0xFF;

//     sc7a20_write_reg();

// }

static rt_err_t _sc7a20_set_range(rt_sensor_t sensor, rt_int32_t range)
{
    if (sensor->info.type == RT_SENSOR_CLASS_GYRO)
    {
        return RT_EOK;
    }

    return RT_ERROR;
}

static rt_err_t _sc7a20_set_mode(rt_sensor_t sensor, rt_uint8_t mode)
{
    if (mode == RT_SENSOR_MODE_POLLING)
    {
        LOG_D("set mode to POLLING");
    }
    else if (mode == RT_SENSOR_MODE_FIFO)
    {
        LOG_D("set mode to FIFO");  //settings for fifo
    }
    else
    {
        LOG_D("Unsupported mode, code is %d", mode);
        return -RT_ERROR;
    }
    return RT_EOK;
}

static rt_err_t _sc7a20_set_power(rt_sensor_t sensor, rt_uint8_t power)
{
    switch (power)
    {
    case RT_SENSOR_POWER_DOWN:
        break;
    case RT_SENSOR_POWER_NORMAL:
        break;
    case RT_SENSOR_POWER_LOW:
        // can change osrs_p, osrs_t, odr ?
        break;
    case RT_SENSOR_POWER_HIGH:
        break;
    default:
        break;
    }
    return RT_EOK;
}

static rt_size_t _sc7a20_fifo_get_acce_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    unsigned int fifodepth;
    static signed short sc7a20_xyz[3][16];      //传感器x,y,z坐标值
    struct rt_sensor_data *data = buf;

//    SL_SC7A20_Watch_Algo_Exe(0);                //保证算法执行到位，这里实测需执行
//    fifodepth = SL_SC7A20_GET_DATA(sc7a20_xyz[0],sc7a20_xyz[1],sc7a20_xyz[2]);  //获取x y z坐标值
    fifodepth = fifodepth < FIFO_LEN_MAX ? fifodepth : FIFO_LEN_MAX;        //fifo 深度限幅
    for(unsigned int i = 0; i < fifodepth; i++)
    {
        data[i].type = RT_SENSOR_CLASS_ACCE;
        data[i].data.acce.x = sc7a20_xyz[0][i];
        data[i].data.acce.y = sc7a20_xyz[1][i];
        data[i].data.acce.z = sc7a20_xyz[2][i];
        data[i].timestamp = rt_sensor_get_ts();
    }
    return fifodepth;
}

static rt_size_t _sc7a20_polling_get_data(rt_sensor_t sensor, struct rt_sensor_data *data)
{
        sc7a20_3axes_t acce;
        if (sc7a20_get_accel(((sc7a20_device_t *)sensor->parent.user_data), &acce) != RT_EOK)
        {
            return 0;
        }

        data->type = RT_SENSOR_CLASS_ACCE;
        data->data.acce.x = acce.x;
        data->data.acce.y = acce.y;
        data->data.acce.z = acce.z;
        data->timestamp = rt_sensor_get_ts();

        return 1;
}

static rt_size_t sc7a20_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    RT_ASSERT(buf);
    if (sensor->info.type == RT_SENSOR_CLASS_ACCE)
    {
        if (sensor->config.mode == RT_SENSOR_MODE_POLLING)  //TODO:使用基于IRQ的FIFO
        {
            // return _sc7a20_fifo_get_acce_data(sensor, buf, len);
            return _sc7a20_polling_get_data(sensor, buf);
        }
    }

    return RT_EOK;
}

static rt_err_t sc7a20_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    switch (cmd)
    {
    // case RT_SENSOR_CTRL_GET_ID:
    //     *(uint8_t *)args = sc7a20_dev->id;
        // break;
    case RT_SENSOR_CTRL_SET_RANGE:
        result = _sc7a20_set_range(sensor, (rt_int32_t)args);
        break;
    case RT_SENSOR_CTRL_SET_ODR:
        result = -RT_EINVAL;
        break;
    case RT_SENSOR_CTRL_SET_MODE:
        result = _sc7a20_set_mode(sensor, (rt_uint32_t)args & 0xff);
        break;
    case RT_SENSOR_CTRL_SET_POWER:
        result = _sc7a20_set_power(sensor, (rt_uint32_t)args & 0xff);
        break;
    // case RT_SENSOR_CTRL_SELF_TEST:
    //     result = _sc7a20_self_test(sensor, *((rt_uint8_t *)args));
    //     break;
    default:
        return -RT_ERROR;
    }
    return result;
}

static sc7a20_device_t  *_sc7a20_init(rt_sensor_intf_t *intf)
{
#ifdef ADDR_PING_HIGH
    rt_uint8_t  i2c_addr = (rt_uint32_t)(intf->user_data) & 0x19;
    LOG_I("i2c_addr : 0x%x", i2c_addr);
#elif  ADDR_PING_LOW
    rt_uint8_t  i2c_addr = (rt_uint32_t)(intf->user_data) & 0x18;
    LOG_I("i2c_addr : 0x%x", i2c_addr);
#endif
    return sc7a20_init(intf->dev_name, i2c_addr);
}

int rt_hw_sc7a20_init(const char *name, struct rt_sensor_config *cfg)
{
    rt_err_t result = RT_EOK;
    sc7a20_device_t *sc7a20_dev_temp;
    rt_sensor_t acce_3x = RT_NULL;

    sc7a20_dev_temp = _sc7a20_init(&cfg->intf);
    if (sc7a20_dev_temp == RT_NULL)
    {
        LOG_E("_sc7a20 init err!");
        goto __exit;
    }

    acce_3x = rt_calloc(1, sizeof(struct rt_sensor_device));
    if (acce_3x == RT_NULL)
        goto __exit;

    acce_3x->info.vendor     =   RT_SENSOR_VENDOR_UNKNOWN;
    acce_3x->info.type       =   RT_SENSOR_CLASS_ACCE;
    acce_3x->info.model      =   "sc7a20_acce";
    acce_3x->info.unit       =   RT_SENSOR_UNIT_MG;
    acce_3x->info.intf_type  =   RT_SENSOR_INTF_I2C;
    acce_3x->info.range_max  =   16000;
    acce_3x->info.range_min  =   2000;
    acce_3x->info.period_min =   100;                                                                                //NOTE
    acce_3x->info.fifo_max   =   FIFO_LEN_MAX;                                                                                 //NOTE

    rt_memcpy(&acce_3x->config, cfg, sizeof(struct rt_sensor_config));
    acce_3x->ops = &sensor_ops;

    result = rt_hw_sensor_register(acce_3x, name, RT_DEVICE_FLAG_RDONLY | RT_DEVICE_FLAG_FIFO_RX, RT_NULL);
    if (result != RT_EOK)
    {
        LOG_E("device register err code: %d", result);
        goto acc__exit;    
    }

    acce_3x->parent.user_data = sc7a20_dev_temp;

acc__exit:
    if(acce_3x)
        rt_free(acce_3x);

__exit:
    if (sc7a20_dev_temp)
        sc7a20_deinit(sc7a20_dev_temp);
    return result = -RT_ERROR;
}

static int rt_hw_sc7a20_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name = "i2c1";//"i2c5"; 		/* i2c bus */
    cfg.intf.user_data = (void *)0x19;	/* i2c slave addr */
    cfg.irq_pin.pin = RT_PIN_NONE;  //TODO:使用基于pin IRQ的FIFO
    rt_hw_sc7a20_init("sc7a20", &cfg);/* sc7a20 */
    return RT_EOK;
}
MSH_CMD_EXPORT(rt_hw_sc7a20_port, rt_hw_sc7a20_port);


/*
 * 程序清单：这是一个 传感器 设备使用例程
 * 例程导出了 sensor_sample 命令到控制终端
 * 命令调用格式：sensor_sample dev_name
 * 命令解释：命令第二个参数是要使用的传感器设备名称
 * 程序功能：打开对应的传感器，然后连续读取 5 次数据并打印出来。
*/

#include "sensor.h"

static void sensor_show_data(rt_size_t num, rt_sensor_t sensor, struct rt_sensor_data *sensor_data)
{
    switch (sensor->info.type)
    {
    case RT_SENSOR_CLASS_ACCE:
        rt_kprintf("num:%3d, x:%5d, y:%5d, z:%5d, timestamp:%5d\n", num, sensor_data->data.acce.x, sensor_data->data.acce.y, sensor_data->data.acce.z, sensor_data->timestamp);
        break;
    default:
        break;
    }
}

static void sensor_sample(int argc, char **argv)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data data;
    rt_size_t res, i;

    /* 查找系统中的传感器设备 */
    dev = rt_device_find(argv[1]);
    if (dev == RT_NULL)
    {
        rt_kprintf("Can't find device:%s\n", argv[1]);
        return;
    }

    /* 以轮询模式打开传感器设备 */
    if (rt_device_open(dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return;
    }

    for (i = 0; i < 5; i++)
    {
        /* 从传感器读取一个数据 */
        res = rt_device_read(dev, 0, &data, 1);
        if (res != 1)
        {
            rt_kprintf("read data failed!size is %d", res);
        }
        else
        {
            sensor_show_data(i, (rt_sensor_t)dev, &data);
        }
        rt_thread_mdelay(100);
    }
    /* 关闭传感器设备 */
    rt_device_close(dev);
}
MSH_CMD_EXPORT(sensor_sample, sensor device sample);
