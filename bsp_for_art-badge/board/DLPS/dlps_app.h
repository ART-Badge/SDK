#ifndef __DLPS_APP_H
#define __DLPS_APP_H

#include "rtl876x_pinmux.h"
#include "rtl876x_io_dlps.h"
#include "dlps.h"


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
#define USE_GPIO_DLPS               0//1
#define USE_I2C0_DLPS               0
#define USE_I2C1_DLPS               0
#define USE_IR_DLPS                 0
#define USE_KEYSCAN_DLPS            0
#define USE_PWM0_DLPS               0
#define USE_PWM1_DLPS               0
#define USE_PWM2_DLPS               0
#define USE_PWM3_DLPS               0
#define USE_QDECODER_DLPS           0
#define USE_RTC_DLPS                0//1
#define USE_SPI0_DLPS               0
#define USE_SPI1_DLPS               0
#define USE_SPI2W_DLPS              0
#define USE_TIM_DLPS                0//1
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

#define DLPS_EN                                     1

#define DEBUG_TEST_DLPS_APP_ENABLE                                              1

#if (DEBUG_TEST_DLPS_APP_ENABLE == 1)
#define DLPS_WAKEUP_TASKDELAY_EVENT                                             0
#define DLPS_WAKEUP_BT_EVENT                                                    0
#define DLPS_WAKEUP_RTC_EVENT                                                   0
#define DLPS_WAKEUP_SWTIMER_EVENT                                               0
#endif

#define KEY_IRQ               GPIO0_IRQn
#define KEY                   P0_3 //KEY2 EVB QFN48/QFN40

void dlps_cpu_init(void);

#endif