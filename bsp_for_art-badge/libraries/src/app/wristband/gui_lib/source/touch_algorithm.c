/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     touch_algorithm.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/
#include "touch_algorithm.h"
#include "string.h"
#include "trace.h"
#include "os_timer.h"
#include "os_sched.h"
#include "wristband_gui.h"
#include "stdlib.h"
#include "gui_core.h"

#define ENABLE_VERTICAL_SLIDE 1

#define TOUCH_DATA_CACHE_LEN  6
#define TOUCH_VALID_AREA_CNT  5

#define TOUCH_SLIDE_THR 30
#define QUICK_SLIDE_THR 10000
#define QUICK_SLIDE_X_SPEED_THR 20
#define SAME_POINT_THR 3
#define LONG_TOUCH_PERIOD 200
#define Y_START_MIN    8
#define Y_START_MAX    215
#define HALF_SCREEN_SIZE    120

//static RtkTouchRawData_t cache[TOUCH_DATA_CACHE_LEN];
static RtkTouchRawData_t x_min_record = {0};
static RtkTouchRawData_t y_min_record = {0};
static RtkTouchRawData_t x_max_record = {0};
static RtkTouchRawData_t y_max_record = {0};
static RtkTouchRawData_t first_record = {0};
//static RtkTouchRawData_t last_record = {0};
//static uint32_t pressed_time = 0;
//static uint32_t released_time = 0;
static bool touch_start = true;

static RtkTouchInfo_t temp;
static RtkTouchInfo_t *info = &temp;

//static int32_t speed_x = 0;

static touch_algorithm_func touch_func = NULL;


void RtkCallbackReg(touch_algorithm_func func)
{
    touch_func = func;
}


void RtkTouchProcess(RtkTouchRawData_t *TouchData)
{
#if ENABLE_TOUCH_DEBUG1
    APP_PRINT_INFO5("x= %d;y=%d;count=%d;RTC counter = %d;press_flag= %d", \
                    TouchData->x, TouchData->y, TouchData->counter, TouchData->rtc_counter, \
                    TouchData->pressFlag);
#endif
    if ((touch_func == NULL) && ((TouchData->counter) < 2))
    {
        return;
    }

    if (touch_start)
    {
        x_min_record = *TouchData;
        x_max_record = *TouchData;
        y_min_record = *TouchData;
        y_max_record = *TouchData;
        first_record = *TouchData;
        info->type = TOUCH_INVALIDE;
        touch_start = false;
    }
    else
    {
        if (TouchData->x < x_min_record.x)
        {
            x_min_record = *TouchData;
        }
        if (TouchData->y < x_min_record.y)
        {
            y_min_record = *TouchData;
        }
        if (TouchData->x > x_max_record.x)
        {
            x_max_record = *TouchData;
        }
        if (TouchData->y > x_max_record.y)
        {
            y_max_record = *TouchData;
        }
    }


    info->deltaX = TouchData->x - first_record.x;
    info->deltaY = TouchData->y - first_record.y;
    info->x = first_record.x;
    info->y = first_record.y;
    if (TouchData->pressFlag == true)
    {
        if (
            ((x_max_record.x - x_min_record.x) >= SAME_POINT_THR) || \
            ((y_max_record.y - y_min_record.y) >= SAME_POINT_THR)
        )
        {
            if (TouchData->counter >= 15)
            {
                if (info->type == TOUCH_INVALIDE)
                {
                    if (abs(info->deltaX) >= abs(info->deltaY))
                    {
                        info->type = TOUCH_HOLD;
                    }
                    else
                    {
                        info->type = TOUCH_HOLD_Y;
                    }
                }

                touch_func(info);
            }

        }

        if (TouchData->counter == LONG_TOUCH_PERIOD)
        {
            if (
                ((x_max_record.x - x_min_record.x) < SAME_POINT_THR) && \
                ((y_max_record.y - y_min_record.y) < SAME_POINT_THR)
            )
            {
                for (uint8_t i = 0; i < pCurrentMenu->current_max_icon; i++)
                {
                    if (pCurrentMenu->pWidgetList[i].touch_capability == true)
                    {
                        if ((pCurrentMenu->pWidgetList[i].x < TouchData->x) && \
                            ((pCurrentMenu->pWidgetList[i].x + pCurrentMenu->pWidgetList[i].width) > TouchData->x) && \
                            (pCurrentMenu->pWidgetList[i].y < TouchData->y) && \
                            ((pCurrentMenu->pWidgetList[i].y + pCurrentMenu->pWidgetList[i].hight) > TouchData->y))
                        {
                            pCurrentMenu->pWidgetList[i].widget_touch_func(LONG_TOUCH_BUTTON);
                            break;
                        }
                    }
                }
            }
        }

    }
    else
    {
        if (
            ((x_max_record.x - x_min_record.x) < SAME_POINT_THR) && \
            ((y_max_record.y - y_min_record.y) < SAME_POINT_THR)
        )
        {
            if (TouchData->counter < 200)
            {
                for (int8_t i = (pCurrentMenu->current_max_icon - 1); i >= 0; i--)
                {
                    if (pCurrentMenu->pWidgetList[i].touch_capability == true)
                    {
                        if ((pCurrentMenu->pWidgetList[i].x < TouchData->x) && \
                            ((pCurrentMenu->pWidgetList[i].x + pCurrentMenu->pWidgetList[i].width) > TouchData->x) && \
                            (pCurrentMenu->pWidgetList[i].y < TouchData->y) && \
                            ((pCurrentMenu->pWidgetList[i].y + pCurrentMenu->pWidgetList[i].hight) > TouchData->y))
                        {
                            //APP_PRINT_INFO0("[Touch Test]-->short button test!!]");
                            pCurrentMenu->pWidgetList[i].widget_touch_func(SHORT_TOUCH_BUTTON);
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            if (TouchData->counter < 15)
            {
                if (abs(info->deltaX) >= abs(info->deltaY))
                {
                    if (TouchData->x < first_record.x)
                    {
                        info->type = TOUCH_LEFT_SLIDE;
                    }
                    else
                    {
                        info->type = TOUCH_RIGHT_SLIDE;
                    }
                }
                else
                {
                    if (TouchData->y < first_record.y)
                    {
                        info->type = TOUCH_DOWN_SLIDE;
                    }
                    else
                    {
                        info->type = TOUCH_UP_SLIDE;
                    }
                }
            }
            else
            {
                if (info->type == TOUCH_HOLD)
                {
                    if (abs(info->deltaX) > HALF_SCREEN_SIZE)
                    {
                        if (TouchData->x < first_record.x)
                        {
                            info->type = TOUCH_LEFT_SLIDE;
                        }
                        else
                        {
                            info->type = TOUCH_RIGHT_SLIDE;
                        }
                    }
                    else
                    {
                        info->type = TOUCH_ORIGIN_FROM_X;
                    }
                }
                else if (info->type == TOUCH_HOLD_Y)
                {
                    if (abs(info->deltaY) > HALF_SCREEN_SIZE)
                    {
                        if (TouchData->y < first_record.y)
                        {
                            info->type = TOUCH_DOWN_SLIDE;
                        }
                        else
                        {
                            info->type = TOUCH_UP_SLIDE;
                        }
                    }
                    else
                    {
                        info->type = TOUCH_ORIGIN_FROM_Y;
                    }
                }

            }

//            APP_PRINT_INFO4("algorithm info->type = %d, info->detalX = %d, info->speed = %d, y_min_record.y = %d",
//                            \
//                            info->type, info->deltaX, info->speed, y_min_record.y);
            touch_func(info);
        }
        touch_start = true;
    }
}



