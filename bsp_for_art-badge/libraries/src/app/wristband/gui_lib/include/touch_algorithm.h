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
#ifndef _TOUCH_ALGORITHM__
#define _TOUCH_ALGORITHM__

#include "stdint.h"

#define TOUCHPAD_RESOLUTION_WIDTH 240
#define TOUCHPAD_RESOLUTION_HEIGHT 240

typedef enum
{
    TOUCH_HOLD,
    TOUCH_HOLD_Y,
    TOUCH_SHORT,
    TOUCH_LONG,
    TOUCH_ORIGIN_FROM_X,
    TOUCH_ORIGIN_FROM_Y,
    TOUCH_LEFT_SLIDE,
    TOUCH_RIGHT_SLIDE,
    TOUCH_UP_SLIDE,
    TOUCH_DOWN_SLIDE,
    TOUCH_INVALIDE,
} GESTURE_T;

typedef enum
{
    SHORT_TOUCH_BUTTON,
    LONG_TOUCH_BUTTON,
} TOUCH_BUTTON_T;

typedef enum
{
    TOUCH_SPEED_SLOW,
    TOUCH_SPEED_FAST,
} TOUCH_SPEED_T;



typedef struct
{
    int16_t x;
    int16_t y;
    int16_t counter;
    uint16_t pressFlag;
    int32_t rtc_counter;
} RtkTouchRawData_t;

typedef struct
{
    GESTURE_T type;
    TOUCH_SPEED_T speed;
    int16_t deltaX;
    int16_t deltaY;
    int16_t x;
    int16_t y;
} RtkTouchInfo_t;


typedef void (* touch_algorithm_func)(RtkTouchInfo_t *);


void RtkCallbackReg(touch_algorithm_func func);
void RtkTouchProcess(RtkTouchRawData_t *TouchData);

#endif //_TOUCH_ALGORITHM__


