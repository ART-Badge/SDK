#include <rtthread.h>
#include "sc7a20.h"
#include "rtdevice.h"

#define DBG_TAG "sc7a20"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define _GET_BUS_DEVICE(sc7a20_device_t)                            \
                ((struct rt_i2c_bus_device *)sc7a20_device_t->bus)
#define _GET_BUS_ADDR(sc7a20_device_t)                              \
                (sc7a20_device_t->i2c_addr)

static rt_err_t sc7a20_write_reg(sc7a20_device_t *dev, rt_uint8_t reg , rt_uint8_t data);
static rt_err_t sc7a20_read_reg(sc7a20_device_t *dev, rt_uint8_t reg, rt_uint8_t *buf,rt_uint8_t len);

sc7a20_device_t *sc7a20_init(const char *dev_name, rt_uint8_t i2c_addr)
{
    sc7a20_device_t* dev = RT_NULL;
    sc7a20_reg_t reg = {0xff};
    rt_bool_t ret = RT_EOK;
	rt_uint8_t chipid_reg = WHO_AM_I_ADDR;
    rt_uint8_t chipid = 0xff;

    RT_ASSERT(dev_name);

    dev = rt_calloc(1, sizeof(sc7a20_device_t));
    if(dev == RT_NULL)
    {
        LOG_E("Can't allocate memory for mpu6xxx device on '%s' ", dev_name);
        goto __exit;
    }

    dev->bus = rt_device_find(dev_name);
    if (dev->bus == RT_NULL)
    {
        LOG_E("Can't find device:'%s'", dev_name);
        goto __exit;
    }

    if (dev->bus->type == RT_Device_Class_I2CBUS)
    {
        if (i2c_addr != RT_NULL)
        {
            dev->i2c_addr = i2c_addr;
            // sc7a20_read_reg(dev, chipid_reg, &chipid, 1);
            // LOG_I("who_am_i: '0x%x'", chipid);
            sc7a20_read_reg(dev, chipid_reg, &reg.who_am_i, 1);
            LOG_I("chip ID, ID: '0x%x'", reg.who_am_i);
        }
        else
        {
            dev->i2c_addr = 0x19;
            if (sc7a20_read_reg(dev, chipid_reg, &chipid, 1) != RT_EOK)
            {
                dev->i2c_addr = 0x18;
                if (sc7a20_read_reg(dev, chipid_reg, &chipid, 1) != RT_EOK)
                {
                    LOG_E("Can't find device at '%s'!", dev_name);
                    goto __exit;
                }
            }
            LOG_D("Device i2c address is:'0x%x'!", dev->i2c_addr);
        }
    }   

    if(reg.who_am_i == 0x11)
    {
        dev->id = reg.who_am_i;
        LOG_I("chip ID, ID: '0x%x'", reg.who_am_i);
    }
    else
    {
        LOG_I("Incorrect chip ID, ID: '0x%x'", reg.who_am_i);
    }

/*      初始化参数设置      */
    if (ret == RT_EOK)
    {
        LOG_I("Device init succeed!");
        rt_uint8_t ctrlreg1 = 0x97;
        rt_uint8_t ctrlreg2 = 0xFF;
        rt_uint8_t ctrlreg1_addr = 0x20;
        rt_uint8_t ctrlreg2_addr = 0xff;

        sc7a20_write_reg(dev, ctrlreg1_addr, ctrlreg1);
        sc7a20_write_reg(dev, ctrlreg2_addr, ctrlreg2);

    }
    else
    {
        LOG_W("Error in device initialization!");
    }
    
    return dev;

__exit:
    if (dev != RT_NULL)
    {
        rt_free(dev);
    }
    return RT_NULL;
}

void sc7a20_deinit(sc7a20_device_t* dev)
{
    RT_ASSERT(dev);

    rt_free(dev);
}

static rt_err_t sc7a20_write_reg(sc7a20_device_t *dev, rt_uint8_t reg , rt_uint8_t data)
{
    // struct rt_i2c_msg msgs[2];
    rt_uint8_t value[2];
	rt_bool_t res;
    
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[2] = {reg, data};

    RT_ASSERT(_GET_BUS_DEVICE(dev));
    
	// value[0] = reg;
	// value[1] = data;
		
	// msgs[0].addr = _GET_BUS_ADDR(dev);
	// msgs[0].flags = RT_I2C_WR;
	// msgs[0].buf = buf;
	// msgs[0].len = 2;

    msgs.addr  = _GET_BUS_ADDR(dev);    /* slave address */
    msgs.flags = RT_I2C_WR;             /* write flag */
    msgs.buf   = buf;                   /* Send data pointer */
    msgs.len   = 2;

	res = rt_i2c_transfer(((struct rt_i2c_bus_device *)dev->bus), &msgs, 1);
	if ( res == 1)
	{
		return RT_EOK;
	}
	else
	{
		LOG_D("sc7a20_write_regs fail\n");
        return -RT_ERROR;
	}
}

static rt_err_t sc7a20_read_reg(sc7a20_device_t *dev, rt_uint8_t reg, rt_uint8_t *buf,rt_uint8_t len)
{
    struct rt_i2c_msg msgs[2];
	rt_bool_t res;

    RT_ASSERT(_GET_BUS_DEVICE(dev));

		
	msgs[0].addr = _GET_BUS_ADDR(dev);
	msgs[0].flags = RT_I2C_WR;
	msgs[0].buf = &reg;
	msgs[0].len = 1;

	msgs[1].addr = _GET_BUS_ADDR(dev);		
	msgs[1].flags = RT_I2C_RD;
	msgs[1].buf = buf;
	msgs[1].len = len;

	res = rt_i2c_transfer(((struct rt_i2c_bus_device *)dev->bus), msgs, 2);
	if ( res == 2)
    {
        return RT_EOK;
	}
	else
    {
		LOG_D("sc7a20_read_reg fail\n");
        return -RT_ERROR;
	}
}

static rt_err_t sc7a20_get_accel_raw(sc7a20_device_t *dev, sc7a20_3axes_t *accel)
{
    rt_uint8_t buffer[6];
    rt_err_t res;

    res = sc7a20_read_reg(dev, 0x29, buffer, 1);
    if (res != RT_EOK)
    {
        return res;
    }

    accel->x = ((rt_uint16_t)buffer[0] << 8) + buffer[1];
    accel->y = ((rt_uint16_t)buffer[2] << 8) + buffer[3];
    accel->z = ((rt_uint16_t)buffer[4] << 8) + buffer[5];

    return RT_EOK;
}

rt_err_t sc7a20_get_accel(sc7a20_device_t *dev, sc7a20_3axes_t *accel)
{
    sc7a20_3axes_t tmp;
    rt_uint16_t sen;
    rt_err_t res;

    res = sc7a20_get_accel_raw(dev, &tmp);
    if (res != RT_EOK)
    {
        return res;
    }

    // sen = MPU6XXX_ACCEL_SEN >> dev->config.accel_range;

    // accel->x = (rt_int32_t)tmp.x * 1000 / sen;
    // accel->y = (rt_int32_t)tmp.y * 1000 / sen;
    // accel->z = (rt_int32_t)tmp.z * 1000 / sen;

    accel->x = (rt_int32_t)tmp.x;
    accel->y = (rt_int32_t)tmp.y;
    accel->z = (rt_int32_t)tmp.z;

    return RT_EOK;
}



/////
static rt_err_t test_read_reg(void)
{
    struct rt_i2c_msg msgs[2];
    rt_uint8_t buf = 0x0f;
	rt_bool_t res;
    struct rt_i2c_bus_device *dev = RT_NULL;
    // RT_ASSERT(_GET_BUS_DEVICE(dev));
    dev = rt_i2c_bus_device_find("i2c1");
		
	msgs[0].addr = 0x19;
	msgs[0].flags = RT_I2C_WR;
	msgs[0].buf = &buf;
	msgs[0].len = 1;

	msgs[1].addr = 0x19;		
	// msgs[1].flags = RT_I2C_RD|RT_I2C_NO_READ_ACK;
    msgs[1].flags = RT_I2C_RD;
	msgs[1].buf = &buf;
	msgs[1].len = 1;

	res = rt_i2c_transfer(dev, msgs, 2);
	rt_kprintf("msggggggg: 0x%x\n", buf);
	if ( res == 2)
    {
        return RT_EOK;
	}
	else
    {
		LOG_D("sc7a20_read_reg fail\n");
        return -RT_ERROR;
	}
}
MSH_CMD_EXPORT(test_read_reg, test_read_reg);