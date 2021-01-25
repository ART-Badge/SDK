/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      hub_task.h
   * @brief     Routines to create App task and handle events & messages
   * @author    howie
   * @date      2019-12-26
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _UI_TASK_H_
#define _UI_TASK_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "stdbool.h"
#include "app_msg.h"


/**  @brief IO type definitions for IO message, may extend as requested */
typedef enum
{
    HUB_MSG_LCD,
    HUB_MSG_GSENSOR,
    HUB_MSG_BATTERY_ADC,
    HUB_MSG_CHARGE,
    HUB_MSG_HRM,
    HUB_MSG_CLOCK,
    HUB_MSG_GPS,
    HUB_MSG_BUTTON,
    HUB_MSG_TOUCH,
    HUB_MSG_UART_DEBUG,
} T_HUB_MSG_TYPE;

/**  @brief IO subtype definitions for @ref T_HUB_MSG_UART_DEBUG type */
typedef enum
{
    UART_DEBUG_MSG_INTERRUPT,
    UART_DEBUG_SIM_TP_INT,
    UART_DEBUG_SIM_TP_RELEASE,
} T_HUB_MSG_UART_DEBUG;

/**  @brief IO subtype definitions for @ref UI_MSG_LCD type */
typedef enum
{
    LCD_MSG_TIMER_MENU,
    LCD_MSG_FRAME_SYNC,
    LCD_MSG_INTERACT,
} T_HUB_MSG_LCD;

/**  @brief IO subtype definitions for @ref UI_MSG_CHARGE type */
typedef enum
{
    CHARGE_MSG_WAKEUP,
} T_HUB_MSG_CHARGE;

/**  @brief IO subtype definitions for @ref UI_MSG_ADC type */
typedef enum
{
    BATTERY_MSG_LOW_POWER,
    BATTERY_MSG_RESTORE,
    BATTERY_MSG_POWER_ON,
    BATTERY_MSG_POWER_OFF,
} T_HUB_MSG_BATTERY_ADC;

/**  @brief IO subtype definitions for @ref UI_MSG_GSENSOR type */
typedef enum
{
    GSENSOR_MSG_WAKEUP,
    GSENSOR_MSG_START,
    GSENSOR_MSG_STOP,
} T_HUB_MSG_GSENSOR;

/**  @brief IO subtype definitions for @ref UI_MSG_HRM type */
typedef enum
{
    HRM_SENSOR_MSG_WAKEUP,
    HRM_SENSOR_MSG_START,
    HRM_SENSOR_MSG_STOP,
} T_HUB_MSG_HRM;

/**  @brief IO subtype definitions for @ref UI_MSG_TOUCH type */
typedef enum
{
    TOUCH_MSG_INT,
    TOUCH_MSG_TIMEOUT,
} T_HUB_MSG_TOUCH;

/**  @brief IO subtype definitions for @ref UI_MSG_BUTTON type */
typedef enum
{
    SHORT_BUTTON_0,
    LONG_BUTTON_0,
    PRESS_BUTTON_0,
    RELEASE_BUTTON_0,
    PRESSING_BUTTON_0,
    SHORT_BUTTON_1,
    LONG_BUTTON_1,
    PRESS_BUTTON_1,
    RELEASE_BUTTON_1,
    SHORT_BUTTON_2,
    LONG_BUTTON_2,
    PRESS_BUTTON_2,
    RELEASE_BUTTON_2,
} T_BUTTON_MSG_TYPE;

typedef void (* pDriverInit)(void);
typedef void (* pExitDLPS)(void);
typedef void (* pEnterDLPS)(void);
typedef bool (* pCheckAllowedDLPS)(void);
typedef bool (* pCheckSystemWakeupDLPS)(void);

typedef struct
{
    pDriverInit init_cb;
    pExitDLPS   enter_dlps_cb;
    pEnterDLPS  exit_dlps_cb;
    pCheckAllowedDLPS allowed_dlps_cb;
    pCheckSystemWakeupDLPS system_wakeup_check_cb;
} T_IO_MODULE_CBS;

#define                WRISTBAND_DRIVER_NUM            0x10
extern uint8_t         wristband_sensor_hub_count;
extern T_IO_MODULE_CBS    *driver_modules[0x10];

extern void *flash_mutex_handle;

/**
 * @brief  Initialize App task
 * @return void
 */
void ui_task_init(void);
void hub_task_handle_msg(T_IO_MSG ui_msg);
bool send_msg_to_ui_task(T_IO_MSG *p_msg, uint32_t line);
void wristband_enter_dlps(void);
void wristband_exit_dlps(void);
bool wristband_enter_dlps_check(void);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */
#endif

