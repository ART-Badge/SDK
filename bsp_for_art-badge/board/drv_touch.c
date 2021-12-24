/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-14     tyustli   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include <touch.h>

#ifdef PKG_USING_GUIENGINE
    #include <rtgui/driver.h>
#endif

#define DBG_TAG              "drv.touch"
#ifdef DRV_DEBUG
    #define DBG_LVL               DBG_LOG
#else
    #define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

#define TOUCH_WIDTH                  480
#define TOUCH_HEIGHT                 272
#define TOUCH_DEV_NAME "gt"
#define TOUCH_THREAD_STACK_SIZE      1024
#define TOUCH_THREAD_PRIORITY        15
#define TOUCH_THREAD_TIMESLICE       10

#define TOUCH_RST_PIN           P0_2

#define TOUCH_INT_PIN           P0_4
#define GPIO_INT_IRQN           GPIO4_IRQn
#define GPIO_INT_INPUT          GPIO_GetPin(TOUCH_INT_PIN)
#define GPIO_Input_Handler      GPIO4_Handler

static rt_sem_t     gt9147_sem = RT_NULL;

/**
  * @brief  Initialize GPIO peripheral.
  * @param  No parameter.
  * @return void
  */
static void int_driver_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin        = GPIO_INT_INPUT;
    GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_ITCmd      = ENABLE;
    GPIO_InitStruct.GPIO_ITTrigger  = GPIO_INT_Trigger_EDGE;
    GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_HIGH;
    GPIO_InitStruct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_ENABLE;
    GPIO_InitStruct.GPIO_DebounceTime = 10;/* unit:ms , can be 1~64 ms */
    GPIO_Init(&GPIO_InitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GPIO_INT_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_MaskINTConfig(GPIO_INT_INPUT, DISABLE);
    GPIO_INTConfig(GPIO_INT_INPUT, ENABLE);
}

void GPIO_Input_Handler(void *param)
{
    rt_interrupt_enter();

    GPIO_INTConfig(GPIO_INT_INPUT, DISABLE);
    GPIO_MaskINTConfig(GPIO_INT_INPUT, ENABLE);
    rt_sem_release(gt9147_sem);
    GPIO_ClearINTPendingBit(GPIO_INT_INPUT);
    GPIO_MaskINTConfig(GPIO_INT_INPUT, DISABLE);
    GPIO_INTConfig(GPIO_INT_INPUT, ENABLE);

    rt_interrupt_leave();
}

static void touch_entry(void *parameter)
{
    rt_device_t dev = (rt_device_t) parameter;
    struct rt_touch_info info = {0};
    struct rt_touch_data read_data = {0};
    rt_bool_t touch_state = RT_FALSE;

    rt_device_control(dev, RT_TOUCH_CTRL_GET_INFO, &info);

    while (1)
    {
        rt_sem_take(gt9147_sem, RT_WAITING_FOREVER);
        rt_device_read(dev, 0, &read_data, 1);

        if (read_data.event == RT_TOUCH_EVENT_DOWN || read_data.event == RT_TOUCH_EVENT_MOVE)
        {
            rtgui_send_touch(TOUCH_WIDTH - read_data.x_coordinate, TOUCH_HEIGHT - read_data.y_coordinate, 1);
            touch_state = RT_TRUE;
        }
        else if (read_data.event == RT_TOUCH_EVENT_UP && touch_state == RT_TRUE)
        {
            rtgui_send_touch(TOUCH_WIDTH - read_data.x_coordinate, TOUCH_HEIGHT - read_data.y_coordinate, 0);

            /* clean touch up state */
            touch_state = RT_FALSE;
        }

        rt_thread_mdelay(20);
    }
}

static int drv_gt9147_init(void)
{
//    struct rt_touch_config config;
//    rt_uint8_t rst;

//#ifdef BSP_USING_I2C1
//    rst = TOUCH_RST_PIN;
//    config.dev_name = "i2c1";
//    config.irq_pin.pin  = TOUCH_INT_PIN;
//    config.user_data = &rst;

//    extern int rt_hw_gt9147_init(const char *name, struct rt_touch_config * cfg);
//    rt_hw_gt9147_init(TOUCH_DEV_NAME, &config);
//#endif

//    return 0;
}

int drv_touch_hw_init(void)
{
    void *id = RT_NULL;
    rt_device_t  dev = RT_NULL;
    rt_thread_t touch_tid = RT_NULL;
    int x = TOUCH_WIDTH, y = TOUCH_HEIGHT;

    /* initialize gt9147 */
    drv_gt9147_init();

    /* init int */
    int_driver_gpio_init();

    dev = rt_device_find(TOUCH_DEV_NAME);

    if (dev == RT_NULL)
    {
        rt_kprintf("can't find device:%s", TOUCH_DEV_NAME);
        return -1;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -1;
    }

    id = rt_malloc(sizeof(rt_uint8_t) * 8);
    rt_device_control(dev, RT_TOUCH_CTRL_GET_ID, id);
    rt_uint8_t * read_id = (rt_uint8_t *)id;
    rt_kprintf("id = %d %d %d %d\r\n", read_id[0] - '0', read_id[1] - '0', read_id[2] - '0', read_id[3] - '0');

    rt_device_control(dev, RT_TOUCH_CTRL_SET_X_RANGE, &x);  /* if possible you can set your x y coordinate*/
    rt_device_control(dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);
    rt_device_control(dev, RT_TOUCH_CTRL_GET_INFO, id);
    rt_kprintf("range_x = %d\r\n", (*(struct rt_touch_info*)id).range_x);
    rt_kprintf("range_y = %d\r\n", (*(struct rt_touch_info*)id).range_y);
    rt_free(id);

    rt_device_control(dev, RT_TOUCH_CTRL_SET_X_TO_Y, RT_NULL);

    gt9147_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);

    if (gt9147_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }

    touch_tid = rt_thread_create("touch",
                                 touch_entry,
                                 (void *)dev,
                                 TOUCH_THREAD_STACK_SIZE,
                                 TOUCH_THREAD_PRIORITY,
                                 TOUCH_THREAD_TIMESLICE);

    if (touch_tid != RT_NULL)
    {
        rt_thread_startup(touch_tid);
    }

    return 0;
}

