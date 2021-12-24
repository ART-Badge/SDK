/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-11-27     tyustli  first version
 */

#include "rtconfig.h"
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

/******************************* lcd test ******************************/
#ifdef PKG_USING_GUIENGINE
    #include <rtgui/driver.h>
    //#include <blit.h>
#endif

#ifndef PKG_USING_GUIENGINE

struct rt_device_graphic_info
{
    rt_uint8_t  pixel_format;                           /**< graphic format */
    rt_uint8_t  bits_per_pixel;                         /**< bits per pixel */
    rt_uint16_t pitch;                                  /**< bytes per line */

    rt_uint16_t width;                                  /**< width of graphic device */
    rt_uint16_t height;                                 /**< height of graphic device */

    rt_uint8_t *framebuffer;                            /**< frame buffer */
};

enum
{
    /* pixel format define */
    RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
    RTGRAPHIC_PIXEL_FORMAT_GRAY4,
    RTGRAPHIC_PIXEL_FORMAT_GRAY16,
    RTGRAPHIC_PIXEL_FORMAT_RGB332,
    RTGRAPHIC_PIXEL_FORMAT_RGB444,
    RTGRAPHIC_PIXEL_FORMAT_RGB565,
    RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    RTGRAPHIC_PIXEL_FORMAT_RGB666,
    RTGRAPHIC_PIXEL_FORMAT_RGB888,
    RTGRAPHIC_PIXEL_FORMAT_ARGB888,
    RTGRAPHIC_PIXEL_FORMAT_ALPHA,
    RTGRAPHIC_PIXEL_FORMAT_LUT,
    RTGRAPHIC_PIXEL_FORMAT_COLOR,
    /* color format alias */
    RTGRAPHIC_PIXEL_FORMAT_BGR565 = RTGRAPHIC_PIXEL_FORMAT_RGB565P
};
#define RTGRAPHIC_CTRL_RECT_UPDATE      0
#define RTGRAPHIC_CTRL_POWERON          1
#define RTGRAPHIC_CTRL_POWEROFF         2
#define RTGRAPHIC_CTRL_GET_INFO         3
#define RTGRAPHIC_CTRL_SET_MODE         4
#define RTGRAPHIC_CTRL_GET_EXT          5
#define RTGRAPHIC_CTRL_FRAME_UPDATE     6
#define RTGRAPHIC_CTRL_FRAME_SWITCH     7

#define WHITE                   0xffff
#define BLACK                   0x0000
#define BLUE                    0x001f
#define GREEN                   0x07e0
#define RED                     0xf800
#define YELLOW                  0xffe0
#define BROWN                   0xbc40
#endif

#define TEST_LCD_WIDTH          240
#define TEST_LCD_HIGHT          240
#define TEST_LCD_PIXEL_BYTES    2
#define TEST_LCD_BUFFER_SIZE    (LCD_WIDTH * LCD_HEIGHT * LCD_PIXEL_BYTES)
#define TEST_LCD_DEVICE_NAME    "lcd"

void lcd_backlight_test(uint8_t argc, char **argv)
{
#define KEY1_PIN    P2_4
#define KEY2_PIN    P2_5
#define KEY3_PIN    P2_6
#define KEY4_PIN    P2_7

    int i;
    rt_device_t dev;
    int persent = 100;

    int p1_read = 0;
    int p2_read = 0;
    int p3_read = 0;
    int p4_read = 0;

    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT);
    rt_pin_mode(KEY2_PIN, PIN_MODE_INPUT);
    rt_pin_mode(KEY3_PIN, PIN_MODE_INPUT);
    rt_pin_mode(KEY4_PIN, PIN_MODE_INPUT);

    dev = rt_device_find(TEST_LCD_DEVICE_NAME);

    if(dev == RT_NULL)
    {
        rt_kprintf("find %s device failed!\n", TEST_LCD_DEVICE_NAME);
        return;
    }

    if(rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        return;
    }

    for(i = 0; i < 1000; i++)
    {
        p1_read = rt_pin_read(KEY1_PIN);
        p2_read = rt_pin_read(KEY2_PIN);
        p3_read = rt_pin_read(KEY3_PIN);
        p4_read = rt_pin_read(KEY4_PIN);

        if(p1_read == 0)
        {
            if (persent < 100)
            {
                persent += 1;
            }

            p1_read = 1;
        }

        if(p2_read == 0)
        {
            if (persent > 0)
            {
                persent -= 1;
            }

            p2_read = 1;
        }

        rt_device_control(dev, RTGRAPHIC_CTRL_SET_BRIGHTNESS, (void *)persent);

        rt_thread_mdelay(10);
    }

    rt_device_close(dev);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(lcd_backlight_test, lcd backlight control test);
#endif /* RT_USING_FINSH */

/********************************* beep test ****************************/

#define BEEP_PIN                                 H_1
static void beep_test(void)
{
    uint32_t i = 0;

    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);

    for(i = 0; i < 90000; i++)
    {
        rt_pin_write(BEEP_PIN, PIN_HIGH);
        rt_hw_us_delay(50);
        rt_pin_write(BEEP_PIN, PIN_LOW);
        rt_hw_us_delay(5);
    }
}

#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(beep_test, beep test);
#endif /* RT_USING_FINSH */

/********************************* key test ****************************/
#define KEY1_PIN                            P2_4
#define KEY2_PIN                            P2_5
#define KEY3_PIN                            P2_6
#define KEY4_PIN                            P2_7

static void key_test(void)
{
    uint32_t i = 0;
    int p1_read = 0;
    int p2_read = 0;
    int p3_read = 0;
    int p4_read = 0;

    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT);
    rt_pin_mode(KEY2_PIN, PIN_MODE_INPUT);
    rt_pin_mode(KEY3_PIN, PIN_MODE_INPUT);
    rt_pin_mode(KEY4_PIN, PIN_MODE_INPUT);

    for(i = 0; i < 1000; i++)
    {
        p1_read = rt_pin_read(KEY1_PIN);
        p2_read = rt_pin_read(KEY2_PIN);
        p3_read = rt_pin_read(KEY3_PIN);
        p4_read = rt_pin_read(KEY4_PIN);

        rt_thread_mdelay(5);

        if(p1_read == 0)
        {
            rt_kprintf("p1 key down\r\n");
            p1_read = 1;
            beep_test();
        }

        if(p2_read == 0)
        {
            rt_kprintf("p2 key down\r\n");
            p2_read = 1;
            beep_test();
        }

        if(p3_read == 0)
        {
            rt_kprintf("p3 key down\r\n");
            p3_read = 1;
            beep_test();
        }

        if(p4_read == 0)
        {
            rt_kprintf("p4 key down\r\n");
            p4_read = 1;
            beep_test();
        }
    }
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(key_test, key test);
#endif /* RT_USING_FINSH */

#ifdef PKG_USING_MPU6XXX
#include "sensor.h"
#include "sensor_inven_mpu6xxx.h"

int mpu_6xxx_sensor_init(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.type = RT_SENSOR_INTF_I2C;
    cfg.intf.dev_name = "i2c1";
    cfg.intf.user_data = (void *)MPU6XXX_ADDR_DEFAULT;
    cfg.irq_pin.pin  = RT_PIN_NONE;

    rt_hw_mpu6xxx_init("mpu6xxx", &cfg);

    return 0;
}
//INIT_ENV_EXPORT(mpu_6xxx_sensor_init);
#endif /* PKG_USING_MPU6XXX */

static void read_mpu_data_entry(void *parameter)
{
    uint16_t i = 0;

    rt_device_t acce_mpu_dev = RT_NULL, gyro_mpu_dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;

    acce_mpu_dev = rt_device_find("acce_mpu6xxx");

    if (acce_mpu_dev == RT_NULL)
    {
        rt_kprintf("Can't find acce_mpu6xxx_dev!\n");
        return;
    }

    if (rt_device_open(acce_mpu_dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open acce_mpu6xxx_dev failed!\n");
        return;
    }

    gyro_mpu_dev = rt_device_find("gyro_mpu6xxx");

    if (gyro_mpu_dev == RT_NULL)
    {
        rt_kprintf("Can't find gyro_mpu6xxx_dev\n");
        return;
    }

    if (rt_device_open(gyro_mpu_dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open gyro_mpu6xxx_dev failed!\n");
        return;
    }

    for(i = 0; i < 100; i++)
    {
        res = rt_device_read(acce_mpu_dev, 0, &sensor_data, 1);

        if (res != 1)
        {
            rt_kprintf("read acce_mpu6xxx_dev data failed!size is %d\n", res);
            rt_device_close(acce_mpu_dev);
            return;
        }
        else
        {
            rt_kprintf("mpu6050_acce_x is: %d\n", sensor_data.data.acce.x);
            rt_kprintf("mpu6050_acce_y is: %d\n", sensor_data.data.acce.y);
            rt_kprintf("mpu6050_acce_z is: %d\n", sensor_data.data.acce.z);
        }

        res = rt_device_read(gyro_mpu_dev, 0, &sensor_data, 1);

        if (res != 1)
        {
            rt_kprintf("read gyro_mpu_dev data failed!size is %d\n", res);
            rt_device_close(gyro_mpu_dev);
            return;
        }
        else
        {
            rt_kprintf("mpu6050_gyro_x is: %d\n", sensor_data.data.gyro.x);
            rt_kprintf("mpu6050_gyro_y is: %d\n", sensor_data.data.gyro.y);
            rt_kprintf("mpu6050_gyro_z is: %d\n\n", sensor_data.data.gyro.z);
        }

        rt_thread_mdelay(100);
    }
}

void mpu6050_test(void)
{
    rt_thread_t mpu6050_thread;

    mpu6050_thread = rt_thread_create("read_mpu6xxx", read_mpu_data_entry, RT_NULL,
                                      2048, RT_THREAD_PRIORITY_MAX / 2, 20);

    if (mpu6050_thread != RT_NULL)
    {
        rt_thread_startup(mpu6050_thread);
    }
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(mpu6050_test, mpu6050 test);
#endif /* RT_USING_FINSH */

/***************** end of file *********************/
