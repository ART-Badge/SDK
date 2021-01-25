/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-09-01     tyustli  first version
 */

#include "rtconfig.h"
#include <simulator_touch.h>

#if defined(PKG_USING_GUIENGINE) && defined(BSP_USING_SIMULATOR_TOUCH)
#include "board.h"
#include <rtgui/driver.h>
#include <rtgui/rtgui_server.h>

void right_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_server_post_touch(TEST_TOUCH_POINT_RIGHT + i * 15, TEST_TOUCH_POINT_RIGHT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }
    rtgui_server_post_touch(TEST_TOUCH_POINT_RIGHT + i * 15, TEST_TOUCH_POINT_RIGHT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(right_touch, soft right touch move);
#endif /* RT_USING_FINSH */

void left_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_server_post_touch(TEST_TOUCH_POINT_LEFT - i * 15, TEST_TOUCH_POINT_LEFT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }
    rtgui_server_post_touch(TEST_TOUCH_POINT_LEFT - i * 15, TEST_TOUCH_POINT_LEFT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(left_touch, soft left touch move);
#endif /* RT_USING_FINSH */

void up_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_server_post_touch(TEST_TOUCH_POINT_UP, TEST_TOUCH_POINT_UP - i * 15, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }
    rtgui_server_post_touch(TEST_TOUCH_POINT_UP, TEST_TOUCH_POINT_UP - i * 15, TOUCH_UP);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(up_touch, soft up touch move);
#endif /* RT_USING_FINSH */

void down_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_server_post_touch(TEST_TOUCH_POINT_DOWN, TEST_TOUCH_POINT_DOWN + i * 15, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_server_post_touch(TEST_TOUCH_POINT_DOWN, TEST_TOUCH_POINT_DOWN + i * 15, TOUCH_UP);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(down_touch, soft down touch move);
#endif /* RT_USING_FINSH */

void in_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_server_post_touch(TEST_TOUCH_POINT, TEST_TOUCH_POINT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }
    rtgui_server_post_touch(TEST_TOUCH_POINT, TEST_TOUCH_POINT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(in_touch, soft in touch move);
#endif /* RT_USING_FINSH */

void long_press_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM * 170; i++)
    {
        rtgui_server_post_touch(TEST_TOUCH_POINT, TEST_TOUCH_POINT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }
    rtgui_server_post_touch(TEST_TOUCH_POINT, TEST_TOUCH_POINT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(long_press_touch, soft long press touch move);
#endif /* RT_USING_FINSH */

#endif /* PKG_USING_GUIENGINE && BSP_USING_SIMULATOR_TOUCH */
