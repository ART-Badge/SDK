/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      app_task.h
   * @brief     Routines to create App task and handle events & messages
   * @author    jane
   * @date      2017-06-02
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#include "app_msg.h"
#include "stdbool.h"

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_WRISTBNAD type */
typedef enum
{
    APP_MSG_BWPS_TX_VALUE,
    APP_MSG_RTC_TIMEROUT_WALL_CLOCK,
    APP_MSG_SENSOR_WAKE_UP,
    APP_MSG_LED_TWINKLE,
    APP_MSG_MOTOR_VIBRATE,
    APP_MSG_CHARGER_STATE,
    APP_MSG_RTC_LOW_BATTERY_RESTORE,
    APP_MSG_RTC_ALARM,
    APP_MSG_POWER_OFF,
    APP_MSG_POWER_ON,
    APP_MSG_HRS_EVENT,
    APP_MSG_SENSOR_MOTION_INTERRUPT,
    APP_MSG_UART_CMD_DEBUG,
    APP_MSG_HRS_TIMEOUT_HANDLE,
    APP_MSG_UPDATE_CONPARA,
    APP_MSG_REPORT_BUTTON,
    APP_MSG_UART_GPS,
    APP_MSG_UART_DEBUG_RX,
    APP_MSG_WAS_RX_VALUE,
    APP_MSG_WAS_ENABLE_CCCD,
    APP_MSG_VOICE_DMA_RX,
    APP_MSG_TOUCH_GES,
    APP_MSG_TOUCH_INT,
    APP_MSG_TOUCH_TIMEOUT,
    APP_MSG_TOUCH_HANDLE,
    APP_MSG_MENU_TIMER,
    APP_MSG_LCD_SYNC,
    APP_MSG_ANCS_DISCOVERY,
} T_APP_TASK_SUB_MSG;

/**
 * @brief  Initialize App task
 * @return void
 */
void app_task_init(void);
bool app_send_msg_to_apptask(T_IO_MSG *p_msg);

#endif

