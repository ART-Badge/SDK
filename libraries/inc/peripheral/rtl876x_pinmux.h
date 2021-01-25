/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_pinmux.h
* \brief    The header file of PAD and PINMUX driver.
* \details  This file provides all PAD and PINMUX firmware functions.
* \author   Chuanguo Xue
* \date     2015-3-27
* \version  v0.1
* *********************************************************************************************************
*/


#ifndef _RTL876X_PINMUX_H_
#define _RTL876X_PINMUX_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    PINMUX      PINMUX
 *
 * \brief       Manage the PINMUX peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    PINMUX_Exported_Constants   PINMUX Exported Constants
 * \{
 * \ingroup     PINMUX
 */

/**
 * \cond
 * \brief       Internal Macros
 * \{
 */

#define  TOTAL_PIN_NUM              39

/* Pad Functions */
#define Output_Val                  (BIT0)
#define Output_En                   (BIT1)
#define Pull_En                     (BIT2)
#define Pull_Direction              (BIT3)
#define Pull_Resistance             (BIT4)
#define WakeUp_En                   (BIT5)
#define WKPOL                       (BIT6)

#define SHDN                        (BIT0)
#define Pin_Mode                    (BIT1)
#define Pin_Debounce                (BIT2)

/**
 * \}
 * \endcond
 */

/**
 * \defgroup    Pin_Function_Number Pin Function Number
 * \ingroup     PINMUX_Exported_Constants
 * \{
 */
#define IDLE_MODE              0
#define HCI_UART_TX            1
#define HCI_UART_RX            2
#define HCI_UART_CTS           3
#define HCI_UART_RTS           4
#define I2C0_CLK               5
#define I2C0_DAT               6
#define I2C1_CLK               7
#define I2C1_DAT               8
#define PWM2_P                 9
#define PWM2_N                 10
#define PWM3_P                 11
#define PWM3_N                 12
#define timer_pwm0             13
#define timer_pwm1             14
#define timer_pwm2             15
#define timer_pwm3             16
#define timer_pwm4             17
#define timer_pwm5             18
#define timer_pwm6             19
#define timer_pwm7             20
#define qdec_phase_a_x         21
#define qdec_phase_b_x         22
#define qdec_phase_a_y         23
#define qdec_phase_b_y         24
#define qdec_phase_a_z         25
#define qdec_phase_b_z         26
#define UART2_TX               27
#define UART2_RX               28
#define UART1_TX               29
#define UART1_RX               30
#define UART1_CTS              31
#define UART1_RTS              32
#define IRDA_TX                33
#define IRDA_RX                34
#define UART0_TX               35
#define UART0_RX               36
#define UART0_CTS              37
#define UART0_RTS              38
#define SPI1_SS_N_0_MASTER     39
#define SPI1_SS_N_1_MASTER     40
#define SPI1_SS_N_2_MASTER     41
#define SPI1_CLK_MASTER        42
#define SPI1_MO_MASTER         43
#define SPI1_MI_MASTER         44
#define SPI0_SS_N_0_SLAVE      45
#define SPI0_CLK_SLAVE         46
#define SPI0_SO_SLAVE          47
#define SPI0_SI_SLAVE          48
#define SPI0_SS_N_0_MASTER     49
#define SPI0_CLK_MASTER        50
#define SPI0_MO_MASTER         51
#define SPI0_MI_MASTER         52
#define SPI2W_DATA             53
#define SPI2W_CLK              54
#define SPI2W_CS               55
#define SWD_CLK                56
#define SWD_DIO                57
#define KEY_COL_0              58
#define KEY_COL_1              59
#define KEY_COL_2              60
#define KEY_COL_3              61
#define KEY_COL_4              62
#define KEY_COL_5              63
#define KEY_COL_6              64
#define KEY_COL_7              65
#define KEY_COL_8              66
#define KEY_COL_9              67
#define KEY_COL_10             68
#define KEY_COL_11             69
#define KEY_COL_12             70
#define KEY_COL_13             71
#define KEY_COL_14             72
#define KEY_COL_15             73
#define KEY_COL_16             74
#define KEY_COL_17             75
#define KEY_COL_18             76
#define KEY_COL_19             77
#define KEY_ROW_0              78
#define KEY_ROW_1              79
#define KEY_ROW_2              80
#define KEY_ROW_3              81
#define KEY_ROW_4              82
#define KEY_ROW_5              83
#define KEY_ROW_6              84
#define KEY_ROW_7              85
#define KEY_ROW_8              86
#define KEY_ROW_9              87
#define KEY_ROW_10             88
#define KEY_ROW_11             89
#define DWGPIO                 90
#define LRC_SPORT1             91
#define BCLK_SPORT1            92
#define ADCDAT_SPORT1          93
#define DACDAT_SPORT1          94
#define DIGI_DEBUG             95
#define DMIC1_CLK              96
#define DMIC1_DAT              97
#define LRC_I_CODEC_SLAVE      98
#define BCLK_I_CODEC_SLAVE     99
#define SDI_CODEC_SLAVE        100
#define SDO_CODEC_SLAVE        101
#define LRC_I_PCM              102
#define BCLK_I_PCM             103
#define UART2_CTS              104
#define UART2_RTS              105
#define BT_COEX_I_0            106
#define BT_COEX_I_1            107
#define BT_COEX_I_2            108
#define BT_COEX_I_3            109
#define BT_COEX_O_0            110
#define BT_COEX_O_1            111
#define BT_COEX_O_2            112
#define BT_COEX_O_3            113
#define PTA_I2C_CLK_SLAVE      114
#define PTA_I2C_DAT_SLAVE      115
#define PTA_I2C_INT_OUT        116
#define DSP_GPIO_OUT           117
#define DSP_JTCK               118
#define DSP_JTDI               119
#define DSP_JTDO               120
#define DSP_JTMS               121
#define DSP_JTRST              122
#define LRC_SPORT0             123
#define BCLK_SPORT0            124
#define ADCDAT_SPORT0          125
#define DACDAT_SPORT0          126
#define MCLK                   127
/** \} */

/**
 * \enum        PAD_WakeUp_Polarity_Value   PAD WakeUp Polarity
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_WAKEUP_POL_VAL
{
    PAD_WAKEUP_POL_HIGH,
    PAD_WAKEUP_POL_LOW
} PAD_WAKEUP_POL_VAL;

/**
 * \enum        PAD_PULL_CONFIG_VAL PAD Pull Config
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_PULL_CONFIG_VAL
{
    PAD_WEAK_PULL,
    PAD_STRONG_PULL
} PAD_PULL_VAL;

/**
 * \enum        PAD_Pull_Mode PAD Pull Mode
 * \ingroup     PINMUX_Exported_Constants
 */
typedef enum _PAD_Pull_Mode
{
    PAD_PULL_UP,
    PAD_PULL_DOWN,
    PAD_PULL_NONE,
} PAD_Pull_Mode;

/**
 * \enum        PAD_Output_Config PAD Output Config
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_OUTPUT_ENABLE_Mode
{
    PAD_OUT_DISABLE,
    PAD_OUT_ENABLE
} PAD_OUTPUT_ENABLE_Mode;

/**
 * \enum        PAD_Output_Value    PAD Output Value
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_OUTPUT_VAL
{
    PAD_OUT_LOW,
    PAD_OUT_HIGH
} PAD_OUTPUT_VAL;

/**
 * \enum        PAD_WakeUp_Debounce_En  PAD WakeUp Debounce enable
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_WAKEUP_DEBOUNCE_EN
{
    PAD_WK_DEBOUNCE_DISABLE,
    PAD_WK_DEBOUNCE_ENABLE
} PAD_WAKEUP_DEBOUNCE_EN;

/**
 * \enum        PAD_Mode PAD Mode
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_Mode
{
    PAD_SW_MODE,
    PAD_PINMUX_MODE
} PAD_Mode;

/**
 * \enum        PAD_Power_Mode PAD Power Mode
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_PWR_Mode
{
    PAD_NOT_PWRON,
    PAD_IS_PWRON = 1
} PAD_PWR_Mode;

/**
 * \enum        PAD_DRIVING_CURRENT_CONT PAD Driving Current control value
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_DRIVING_CURRENT_CONT
{
    DRIVING_CURRENT_DISABLE,
    DRIVING_CURRENT_E1,
    DRIVING_CURRENT_E2,
    DRIVING_CURRENT_E3,
    DRIVING_CURRENT_E4,
    DRIVING_CURRENT_E5
} PAD_DRIVING_CURRENT_VAL;

/**
 * \enum        PAD_DRIVING_CURRENT_E2_LEVEL PAD Driving Current E2 Level
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_DRIVING_CURRENT_E2_LEVEL
{
    LEVEL_E2_0,
    LEVEL_E2_1
} PAD_DRIVING_CURRENT_E2_LEVEL;

/**
 * \enum        PAD_DRIVING_CURRENT_E3_LEVEL PAD Driving Current E3 Level
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_DRIVING_CURRENT_E3_LEVEL
{
    LEVEL_E3_0,
    LEVEL_E3_1
} PAD_DRIVING_CURRENT_E3_LEVEL;

/**
 * \enum        PAD_FUNCTION_CONFIG_VALUE PAD Function config value
 *
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_FUNCTION_CONFIG_VALUE
{
    AON_GPIO,
    LED0,
    LED1,
    LED2,
    CLK_REQ,
} PAD_FUNCTION_CONFIG_VAL;

/**
 * \enum        PAD_LDO_Type PAD LDO Type
 * \ingroup     PINMUX_Exported_Constants
 */

typedef enum _PAD_LDO_TYPE
{
    PAD_LDOAUX1,
    PAD_LDOAUX2
} PAD_LDO_TYPE;


/** \} */ /** End of group PINMUX_Exported_Constants */

/**
 * \brief
 */
extern const uint8_t digi_debug_pin[32];

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    PINMUX_Exported_Functions Peripheral APIs
 * \ingroup     PINMUX
 * \{
 */

/**
 * \brief   Configure or reset all pins to idle mode.
 * \param   None.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)//XXX represents the name of the peripheral to be configured.
 * {
 *     Pinmux_Reset();
 * }
 * \endcode
 */
void Pinmux_Reset(void);

/**
 * \brief   Configure the specified pin to idle mode.
 * \param   Pin_Num: Pin number to be configured.
 *      This parameter can be one of the following values:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pinmux_Deinit(P2_2);
 * }
 * \endcode
 */
void Pinmux_Deinit(uint8_t Pin_Num);

/**
 * \brief   Config the usage function of the selected pin.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter can be the following:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2
 * \param[in] Pin_Func: Use function of pin.
 *      This parameter This parameter refers to \ref Pin_Function_Number
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_uart_init(void)
 * {
 *     Pad_Config(P2_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
 *     Pad_Config(P2_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);

 *     Pinmux_Config(P2_0, UART0_TX);
 *     Pinmux_Config(P2_1, UART0_RX);
 * }
 * \endcode
 */
void Pinmux_Config(uint8_t Pin_Num, uint8_t Pin_Func);

/**
 * \brief   Configure the relevant operation mode,
 *          peripheral circuit and output level value in software mode of the specified pin
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter can be the following:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2
 * \param[in] AON_PAD_Mode: Use software mode or pinmux mode.
 *      This parameter can be one of the following values:
 *      \arg PAD_SW_MODE: Use software mode.
 *      \arg PAD_PINMUX_MODE: Use pinmux mode.
 * \param[in] AON_PAD_PwrOn: Config power of pad.
 *      This parameter can be one of the following values:
 *      \arg PAD_NOT_PWRON: Shutdown power of pad.
 *      \arg PAD_IS_PWRON: Enable power of pad.
 * \param[in] AON_PAD_Pull: config pad pull mode.
 *      This parameter can be one of the following values:
 *      \arg PAD_PULL_NONE: No pull.
 *      \arg PAD_PULL_UP: Pull this pin up.
 *      \arg PAD_PULL_DOWN: Pull this pin down.
 * \param[in] AON_PAD_E: Config pad out put function.
 *      This parameter can be one of the following values:
 *      \arg PAD_OUT_DISABLE: Disable pin output.
 *      \arg PAD_OUT_ENABLE: Enable pad output.
 * \param[in] AON_PAD_O: Config pin output level.
 *      This parameter can be one of the following values:
 *      \arg PAD_OUT_LOW: Pad output low.
 *      \arg PAD_OUT_HIGH: Pad output high.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_adc_init(void)
 * {
 *     Pad_Config(P2_0, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
 *     Pad_Config(P2_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
 * }
 * \endcode
 */
void Pad_Config(uint8_t Pin_Num,
                PAD_Mode AON_PAD_Mode,
                PAD_PWR_Mode AON_PAD_PwrOn,
                PAD_Pull_Mode AON_PAD_Pull,
                PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                PAD_OUTPUT_VAL AON_PAD_O);

/**
 * \brief   Enable the function of the wake-up system of the specified pin.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter can be the following:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2
 * \param[in] Polarity: Polarity of wake-up system.
 *      This parameter can be the following:
 *      \arg PAD_WAKEUP_POL_HIGH: Use high level wakeup.
 *      \arg PAD_WAKEUP_POL_LOW: Use low level wakeup.
 * \param[in] DebounceEn: Enable delay function.
 *      \arg PAD_WK_DEBOUNCE_DISABLE: Disable delay function.
 *      \arg PAD_WK_DEBOUNCE_ENABLE: Enable delay function.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 * //IO enter dlps call back function.
 * void io_uart_dlps_enter(void)
 * {
 *     // Switch pad to software mode
 *     Pad_ControlSelectValue(P2_0, PAD_SW_MODE);//tx pin
 *     Pad_ControlSelectValue(P2_1, PAD_SW_MODE);//rx pin
 *
 *     System_WakeUpPinEnable(P2_1, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
 * }
 * \endcode
 */
void System_WakeUpPinEnable(uint8_t Pin_Num, uint8_t Polarity, uint8_t DebounceEn);

/**
 * \brief   Disable the function of the wake-up system of the specified pin.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter can be the following:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 * #define UART_RX_PIN                P4_1
 *
 * //System interrupt handler function, for wakeup pin.
 * void System_Handler(void)
 * {
 *     if (System_WakeUpInterruptValue(UART_RX_PIN) == SET)
 *     {
 *         Pad_ClearWakeupINTPendingBit(UART_RX_PIN);
 *         System_WakeUpPinDisable(UART_RX_PIN);
 *         //Add user code here.
 *     }
 * }
 * \endcode
 */
void System_WakeUpPinDisable(uint8_t Pin_Num);

/**
 * \brief   Get pin interrupt status.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter can be the following:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2
 * \return Interrupt status.
 * \retval 1: Pin wake up system.
 * \retval 0: The pin does not wake up the system.
 *
 * <b>Example usage</b>
 * \code{.c}
 * #define UART_RX_PIN                P4_1
 *
 * //System interrupt handler function, for wakeup pin.
 * void System_Handler(void)
 * {
 *     if (System_WakeUpInterruptValue(UART_RX_PIN) == SET)
 *     {
 *         Pad_ClearWakeupINTPendingBit(UART_RX_PIN);
 *         System_WakeUpPinDisable(UART_RX_PIN);
 *         //Add user code here.
 *     }
 * }
 * \endcode
 */
uint8_t System_WakeUpInterruptValue(uint8_t Pin_Num);

/**
 * \brief   Get pin interrupt status, function is the same as \see system_WakeUpInterruptValue.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter can be the following:
 *      \arg P0_0~P0_7, P1_0~P1_7, P2_0~P2_7, P3_0~P3_6, P4_0~P4_3, H0~H2
 * \return Interrupt status.
 * \retval 1: Pin wake up system.
 * \retval 0: The pin does not wake up the system.
 */
uint8_t Pad_WakeupInterruptValue(uint8_t Pin_Num);

/**
 * \brief   Enable or disable pad output mode.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to H_2, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: This parameter sets whether the pin outputs the level in software mode.
 *      This parameter can be enumerated PAD_OUTPUT_ENABLE_Mod of the values:
 *      \arg PAD_OUT_DISABLE: Disable output.
 *      \arg PAD_OUT_ENABLE: Enable output.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void pad_demo(void)
 * {
 *     Pad_OutputEnableValue(P2_0, PAD_OUT_ENABLE);
 * }
 * \endcode
 */
void Pad_OutputEnableValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Enable or disable pad pull-up / pull-down resistance function.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to PH_0, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: This parameter enable or disable the the pad pin pull-up/pull-down function.
 *      This parameter can be the following:
 *      \arg DISABLE: Disable pad pull-up / pull-down function.
 *      \arg ENABLE: Enable  pad pull-up / pull-down function.
 *
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_PullEnableValue(P2_0, ENABLE);
 * }
 * \endcode
 */
void Pad_PullEnableValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Pad pull-up/pull-down resistance function selection.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to H_2, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: This parameter sets whether the pin pull-up or pull-down.
 *      This parameter can be the following:
 *      \arg 0: Config pad pull-up function.
 *      \arg 1: Config  pad pull-down function.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_PullUpOrDownValue(P2_0, 1);
 * }
 * \endcode
 */
void Pad_PullUpOrDownValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Configure the strength of pull-up/pull-down resistance.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: This parameter sets the strength of pull-up/pull-down resistance.
 *      This parameter can be the following:
 *      \arg PAD_WEAK_PULL: Resistance weak pull.
 *      \arg PAD_STRONG_PULL: Resistance strong pull.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_PullConfigValue(P2_0, PAD_STRONG_PULL);
 * }
 * \endcode
 */
void Pad_PullConfigValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Set pin mode.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to H_2, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: This parameter sets the pin mode.
 *      This parameter can be the following:
 *      \arg PAD_SW_MODE: Software mode.
 *      \arg PAD_PINMUX_MODE: Pinmux mode.
 *
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_ControlSelectValue(P2_0, PAD_SW_MODE);
 * }
 * \endcode
 */
void Pad_ControlSelectValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Configure pad output level.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: Config pin output level.
 *      This parameter can be one of the following values:
 *      \arg PAD_OUT_LOW: Pad output low.
 *      \arg PAD_OUT_HIGH: Pad output high.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_OutputControlValue(P2_0, PAD_OUT_HIGH);
 * }
 * \endcode
 */
void Pad_OutputControlValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Enable the function of the wake-up system of the specified pin.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: Enable delay function.
 *      \arg 0:Disable wake-up system function.
 *      \arg 1:Enable wake-up system function.
 * \param[in]
 *
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_WakeupEnableValue(P2_0, 1);
 * }
 * \endcode
 */
void Pad_WakeupEnableValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Set polarity of wake-up system.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] Polarity: Polarity of wake-up system.
 *      This parameter can be the following:
 *      \arg PAD_WAKEUP_POL_LOW:Use low level wakeup.
 *      \arg PAD_WAKEUP_POL_HIGH: Use high level wakeup.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_WakeupPolarityValue(P2_0, PAD_WAKEUP_POL_HIGH);
 * }
 * \endcode
 */
void Pad_WakeupPolarityValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Config pin delay function.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: Enable delay function.
 *      \arg 0:Disable delay function.
 *      \arg 1:Enable delay function.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_WKDebounceConfig(P2_0, 1);
 * }
 * \endcode
 */
void Pad_WKDebounceConfig(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Clear the interrupt pendign bit of the specified pin
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_ClearWakeupINTPendingBit(P2_0);
 * }
 * \endcode
 */
void Pad_ClearWakeupINTPendingBit(uint8_t Pin_Num);

/**
 * \brief   Clear all wake up pin interrupt pending bit.
 * \param[in] Pin_Num: pin number.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_ClearAllWakeupINT();
 * }
 * \endcode
 */
void Pad_ClearAllWakeupINT(void);

/**
 * \brief   Set pin power mode.
 * \param[in] Pin_Num: Pin number to be configured.
 *      This parameter is from P0_0 to P4_1, please refer to rtl876x.h "RTL876X_Pin_Number" part.
 * \param[in] value: This parameter sets the power supply mode of the pin,
 *                   and the value is enumeration PAD_PWR_Mode One of the values.
 *      \arg PAD_NOT_PWRON: Power off.
 *      \arg PAD_IS_PWRON: Power on.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     Pad_PowerOrShutDownValue(P2_0, PAD_NOT_PWRON);
 * }
 * \endcode
 */
void Pad_PowerOrShutDownValue(uint8_t Pin_Num, uint8_t value);

/**
 * \brief   Config wak-up system debounce time.
 * \param[in] time: Debounce time,1ms~64ms.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_xxx_init(void)
 * {
 *     System_WakeUpDebounceTime(10);
 * }
 * \endcode
 */
void System_WakeUpDebounceTime(uint8_t time);

/**
 * \brief   8080 pinmux control.
 * \param[in] value: LCD 8080 pinmux control.
 *      \arg 0x0: disable LCD interface.
 *      \arg 0x1: enable LCD1 interface (8080_D0: P0_2).
 *      \arg 0x2/0x3: enable LCD2 interface (8080_D0: P3_5).
 * \param[in] VSYNC:1:Disable 0:Enable.
 * \param[in] RD:1:Disable 0:Enable.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_if8080_init(void)
 * {
 *     Pinmux_8080Control(1, 0, 0);
 * }
 * \endcode
 */
void Pinmux_8080Control(uint32_t value, uint8_t VSYNC, uint8_t RD);

/**
 * \brief   Spic0 master enable.
 * \param[in] value: 0:Disable 1:Enable.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void io_demo(void)
 * {
 *     Spic0_control(1);
 * }
 * \endcode
 */
void Spic0_control(uint8_t value);

/**
 * \brief   Spic1 master enable.
 * \param[in] value: 0:Disable 1:Enable.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void io_demo(void)
 * {
 *     Spic1_control(1);
 * }
 * \endcode
 */
void Spic1_control(uint8_t value);


/** \} */ /* End of group PINMUX_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_PINMUX_H_ */

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/

