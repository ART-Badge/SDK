/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      board.h
* @brief     header file of Keypad demo.
* @details
* @author    tifnan_ge
* @date      2015-06-26
* @version   v0.1
* *********************************************************************************************************
*/


#ifndef _BOARD_H_
#define _BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x_pinmux.h"

/** @defgroup IO Driver Config
  * @note user must config it firstly!! Do not change macro names!!
  * @{
  */

/* if use user define dlps enter/dlps exit callback function */
#define USE_USER_DEFINE_DLPS_ENTER_CB       1
#define USE_USER_DEFINE_DLPS_EXIT_CB        1

/* if use any peripherals below, #define it 1 */
#define USE_ADC_DLPS                0
#define USE_DMIC_DLPS               0
#define USE_GPIO_DLPS               1
#define USE_I2C0_DLPS               0
#define USE_I2C1_DLPS               0
#define USE_IR_DLPS                 0
#define USE_KEYSCAN_DLPS            0
#define USE_PWM0_DLPS               0
#define USE_PWM1_DLPS               0
#define USE_PWM2_DLPS               0
#define USE_PWM3_DLPS               0
#define USE_QDECODER_DLPS           0
#define USE_RTC_DLPS                1
#define USE_SPI0_DLPS               0
#define USE_SPI1_DLPS               0
#define USE_SPI2W_DLPS              0
#define USE_TIM_DLPS                1
#define USE_UART0_DLPS              0
#define USE_UART1_DLPS              0
#define USE_LOG_UART2_DLPS          0

/* do not modify USE_IO_DRIVER_DLPS macro */
#define USE_IO_DRIVER_DLPS          (USE_ADC_DLPS | USE_DMIC_DLPS | USE_GPIO_DLPS\
                                     | USE_I2C0_DLPS | USE_I2C1_DLPS | USE_IR_DLPS\
                                     | USE_KEYSCAN_DLPS| USE_PWM0_DLPS | USE_PWM1_DLPS\
                                     | USE_PWM2_DLPS | USE_PWM3_DLPS | USE_QDECODER_DLPS\
                                     | USE_RTC_DLPS | USE_SPI0_DLPS | USE_SPI1_DLPS\
                                     | USE_SPI2W_DLPS | USE_TIM_DLPS | USE_UART0_DLPS\
                                     | USE_UART1_DLPS | USE_LOG_UART2_DLPS\
                                     | USE_USER_DEFINE_DLPS_ENTER_CB\
                                     | USE_USER_DEFINE_DLPS_EXIT_CB)

#define DLPS_EN                                     0

/*============================================================================*
 *                                OTA Config
 *============================================================================*/
#define SUPPORT_NORMAL_OTA                          1
#define ENABLE_FLASH_TRY_4BIT_MODE                  0
#define ENABLE_AUTO_BANK_SWITCH                     0 //for qc test
#define ENABLE_BANK_SWITCH_COPY_APP_DATA            1
#define ENABLE_SLAVE_REQUEST_UPDATE_COON_PARA       0

extern uint8_t g_ota_mode;
#define DFU_TEMP_BUFFER_SIZE                        2048
#define DFU_BUFFER_CHECK_ENABLE                     0x1//(g_ota_mode & 0x1)


#if (ENABLE_SLAVE_REQUEST_UPDATE_COON_PARA ==1)
/*interval 1.25ms/step,    supervision_timeout 10ms/step*/
#define CONNECT_INTERVAL_MIN                        0x6  /* (7.5ms) */
#define CONNECT_INTERVAL_MAX                        0x6  /* (7.5ms) */
#define CONNECT_LATENCY                             0x0
#define SUPERVISION_TIMEOUT                         1000 /* (10s) */
#endif
/****************************************************/

#define EVB_8762CJ           0
#define EVB_8762CK           0
#define EVB_8762CJF          1

#if EVB_8762CJ
#define TP0                   P1_2
#else
#define TP0                   P0_0
#endif

#define KEY                   P2_4 //KEY2 EVB QFN48/QFN40
#define KEY_IRQ               GPIO20_IRQn
#define KEY_INT_Handle        GPIO20_Handler

#ifdef __cplusplus
}
#endif

#endif  /* _BOARD_H_ */

