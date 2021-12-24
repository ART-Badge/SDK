/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_gpio.h
* \brief    The header file of the peripheral GPIO driver.
* \details  This file provides all GPIO firmware functions.
* \author   elliot chen
* \date     2015-05-20
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_GPIO_H_
#define _RTL876X_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    GPIO        GPIO
 *
 * \brief       Manage the GPIO peripheral functions.
 *
 * \ingroup     IO
 */
/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/**
 * \cond        private
 * \defgroup    GPIO_Debounce_Register GPIO Debounce Register
 * \{
 */

/**
 * \brief GPIO Private Defines
 */
#define GPIO_DBCLK_DIV *((volatile uint32_t *)0x40000344UL)

/**
 * \}
 * \endcond
 */

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    GPIO_Exported_Constants     Macro Definitions
 *
 * \ingroup     GPIO
 */

/**
 * \defgroup    GPIO_Number GPIO Number
 * \{
 * \ingroup     GPIO_Exported_Constants
 */
#define GPIO0   0
#define GPIO1   1
#define GPIO2   2
#define GPIO3   3
#define GPIO4   4
#define GPIO5   5
#define GPIO6   6
#define GPIO7   7
#define GPIO8   8
#define GPIO9   9
#define GPIO10  10
#define GPIO11  11
#define GPIO12  12
#define GPIO13  13
#define GPIO14  14
#define GPIO15  15
#define GPIO16  16
#define GPIO17  17
#define GPIO18  18
#define GPIO19  19
#define GPIO20  20
#define GPIO21  21
#define GPIO22  22
#define GPIO23  23
#define GPIO24  24
#define GPIO25  25
#define GPIO26  26
#define GPIO27  27
#define GPIO28  28
#define GPIO29  29
#define GPIO30  30
#define GPIO31  31
/** \} */

/**
 * \defgroup    GPIO_Pins_Define GPIO Pins Define
 * \{
 * \ingroup     GPIO_Exported_Constants
 */
#define GPIO_Pin_0                 ((uint32_t)0x00000001)  /*!< Pin 0 selected   */
#define GPIO_Pin_1                 ((uint32_t)0x00000002)  /*!< Pin 1 selected   */
#define GPIO_Pin_2                 ((uint32_t)0x00000004)  /*!< Pin 2 selected   */
#define GPIO_Pin_3                 ((uint32_t)0x00000008)  /*!< Pin 3 selected   */
#define GPIO_Pin_4                 ((uint32_t)0x00000010)  /*!< Pin 4 selected   */
#define GPIO_Pin_5                 ((uint32_t)0x00000020)  /*!< Pin 5 selected   */
#define GPIO_Pin_6                 ((uint32_t)0x00000040)  /*!< Pin 6 selected   */
#define GPIO_Pin_7                 ((uint32_t)0x00000080)  /*!< Pin 7 selected   */
#define GPIO_Pin_8                 ((uint32_t)0x00000100)  /*!< Pin 8 selected   */
#define GPIO_Pin_9                 ((uint32_t)0x00000200)  /*!< Pin 9 selected   */
#define GPIO_Pin_10                ((uint32_t)0x00000400)  /*!< Pin 10 selected  */
#define GPIO_Pin_11                ((uint32_t)0x00000800)  /*!< Pin 11 selected  */
#define GPIO_Pin_12                ((uint32_t)0x00001000)  /*!< Pin 12 selected  */
#define GPIO_Pin_13                ((uint32_t)0x00002000)  /*!< Pin 13 selected  */
#define GPIO_Pin_14                ((uint32_t)0x00004000)  /*!< Pin 14 selected  */
#define GPIO_Pin_15                ((uint32_t)0x00008000)  /*!< Pin 15 selected  */
#define GPIO_Pin_16                ((uint32_t)0x00010000)  /*!< Pin 16 selected   */
#define GPIO_Pin_17                ((uint32_t)0x00020000)  /*!< Pin 17 selected   */
#define GPIO_Pin_18                ((uint32_t)0x00040000)  /*!< Pin 18 selected   */
#define GPIO_Pin_19                ((uint32_t)0x00080000)  /*!< Pin 19 selected   */
#define GPIO_Pin_20                ((uint32_t)0x00100000)  /*!< Pin 20 selected   */
#define GPIO_Pin_21                ((uint32_t)0x00200000)  /*!< Pin 21 selected   */
#define GPIO_Pin_22                ((uint32_t)0x00400000)  /*!< Pin 22 selected   */
#define GPIO_Pin_23                ((uint32_t)0x00800000)  /*!< Pin 23 selected   */
#define GPIO_Pin_24                ((uint32_t)0x01000000)  /*!< Pin 24 selected   */
#define GPIO_Pin_25                ((uint32_t)0x02000000)  /*!< Pin 25 selected   */
#define GPIO_Pin_26                ((uint32_t)0x04000000)  /*!< Pin 26 selected  */
#define GPIO_Pin_27                ((uint32_t)0x08000000)  /*!< Pin 27 selected  */
#define GPIO_Pin_28                ((uint32_t)0x10000000)  /*!< Pin 28 selected  */
#define GPIO_Pin_29                ((uint32_t)0x20000000)  /*!< Pin 29 selected  */
#define GPIO_Pin_30                ((uint32_t)0x40000000)  /*!< Pin 30 selected  */
#define GPIO_Pin_31                ((uint32_t)0x80000000)  /*!< Pin 31 selected  */
#define GPIO_Pin_All               ((uint32_t)0xFFFFFFFF)  /*!< All pins selected */
/** \} */

#define IS_PIN_NUM(NUM) ((NUM) <= (uint8_t)P4_1)

#define IS_GPIO_PIN(PIN) ((PIN) != (uint32_t)0x00)

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15) || \
                              ((PIN) == GPIO_Pin_16) || \
                              ((PIN) == GPIO_Pin_17) || \
                              ((PIN) == GPIO_Pin_18) || \
                              ((PIN) == GPIO_Pin_19) || \
                              ((PIN) == GPIO_Pin_20) || \
                              ((PIN) == GPIO_Pin_21) || \
                              ((PIN) == GPIO_Pin_22) || \
                              ((PIN) == GPIO_Pin_23) || \
                              ((PIN) == GPIO_Pin_24) || \
                              ((PIN) == GPIO_Pin_25) || \
                              ((PIN) == GPIO_Pin_26) || \
                              ((PIN) == GPIO_Pin_27) || \
                              ((PIN) == GPIO_Pin_28) || \
                              ((PIN) == GPIO_Pin_29) || \
                              ((PIN) == GPIO_Pin_30) || \
                              ((PIN) == GPIO_Pin_31))
/**
 * \brief       Bit_SET and Bit_RESET enumeration
 */
typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))


/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    GPIO_Exported_Types GPIO Exported Types
 *
 * \ingroup     GPIO
 */

/**
 * \brief       GPIO mode enumeration.
 *
 * \ingroup     GPIO_Exported_Types
 */
typedef enum
{
    GPIO_Mode_IN   = 0x00, /**< GPIO Input Mode */
    GPIO_Mode_OUT  = 0x01, /**< GPIO Output Mode */
} GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN)|| ((MODE) == GPIO_Mode_OUT))

/**
 * \brief       Setting interrupt trigger type.
 *
 * \ingroup     GPIO_Exported_Types
 */
typedef enum
{
    GPIO_INT_Trigger_LEVEL = 0x0, /**< This interrupt is level trigger. */
    GPIO_INT_Trigger_EDGE  = 0x1, /**< This interrupt is edge trigger. */
    GPIO_INT_BOTH_EDGE = 0x2,     /**< This interrupt is both edge trigger. */
} GPIOIT_LevelType;

#define IS_GPIOIT_LEVEL_TYPE(TYPE) (((TYPE) == GPIO_INT_Trigger_LEVEL)\
                                    || ((TYPE) == GPIO_INT_Trigger_EDGE)\
                                    || ((TYPE) == GPIO_INT_BOTH_EDGE))

/**
 * \brief       Setting interrupt active mode.
 *
 * \ingroup     GPIO_Exported_Types
 */
typedef enum
{
    GPIO_INT_POLARITY_ACTIVE_LOW  = 0x0, /**< Setting interrupt to low active. */
    GPIO_INT_POLARITY_ACTIVE_HIGH = 0x1, /**< Setting interrupt to high active. */
} GPIOIT_PolarityType;

#define IS_GPIOIT_POLARITY_TYPE(TYPE) (((TYPE) == GPIO_INT_POLARITY_ACTIVE_LOW)\
                                       || ((TYPE) == GPIO_INT_POLARITY_ACTIVE_HIGH))

/**
 * \brief       Enable/Disable interrupt debounce.
 *
 * \ingroup     GPIO_Exported_Types
 */
typedef enum
{
    GPIO_INT_DEBOUNCE_DISABLE = 0x0, /**< Disable interrupt debounce. */
    GPIO_INT_DEBOUNCE_ENABLE  = 0x1, /**< Enable interrupt debounce.  */
} GPIOIT_DebounceType;

#define IS_GPIOIT_DEBOUNCE_TYPE(TYPE) (((TYPE) == GPIO_INT_DEBOUNCE_DISABLE)\
                                       || ((TYPE) == GPIO_INT_DEBOUNCE_ENABLE))

/**
 * \brief       Hardware/Software mode select.
 *
 * \ingroup     GPIO_Exported_Types
 */
typedef enum
{
    GPIO_SOFTWARE_MODE = 0x0, /**< GPIO software mode(default). */
    GPIO_HARDWARE_MODE  = 0x1, /**< GPIO hardware control mode. */
} GPIOControlMode_Typedef;

#define IS_GPIOIT_MODDE(TYPE) (((TYPE) == GPIO_SOFTWARE_MODE)\
                               || ((TYPE) == GPIO_HARDWARE_MODE))

/**
 * \brief       GPIO init structure definition.
 *
 * \ingroup     GPIO_Exported_Types
 */
typedef struct
{
    uint32_t                  GPIO_Pin;         /**< Specifies the GPIO pins to be configured.
                                                        This parameter can be any value of \ref GPIO_Pins_Define */
    GPIOMode_TypeDef
    GPIO_Mode;        /**< Specifies the operating mode for the selected pins. */
    FunctionalState           GPIO_ITCmd;       /**< Enable or disable GPIO interrupt.
                                                        This parameter can be a value of DISABLE or ENABLE. */
    GPIOIT_LevelType          GPIO_ITTrigger;   /**< Interrupt mode is level or edge trigger. */
    GPIOIT_PolarityType       GPIO_ITPolarity;  /**< Interrupt mode is high or low active trigger. */
    GPIOIT_DebounceType       GPIO_ITDebounce;  /**< Enable or disable de-bounce for interrupt. */
    GPIOControlMode_Typedef   GPIO_ControlMode; /**< Specifies the gpio control mode. */
    uint32_t GPIO_DebounceTime;                 /**< Specifies the gpio debounce time(ms). */
} GPIO_InitTypeDef;


/*============================================================================*
 *                         Functions
 *============================================================================*/


/**
 * \defgroup   GPIO_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     GPIO
 */

/**
 * \brief   Deinitializes the GPIO peripheral registers to their default reset values.
 * \param   None.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gpio_init(void)
 * {
 *     GPIO_DeInit();
 * }
 * \endcode
 */
void GPIO_DeInit(void);

/**
 * \brief  Initializes the GPIO peripheral according to the specified
 *         parameters in the GPIO_InitStruct.
 * \param[in] GPIO_InitStruct: Pointer to a GPIO_InitTypeDef structure that
 *            contains the configuration information for the specified GPIO peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gpio_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
 *
 *     GPIO_InitTypeDef GPIO_InitStruct;
 *     GPIO_StructInit(&GPIO_InitStruct);
 *     GPIO_InitStruct.GPIO_Pin        = GPIO_GetPin(P4_0);
 *     GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_IN;
 *     GPIO_InitStruct.GPIO_ITCmd      = ENABLE;
 *     GPIO_InitStruct.GPIO_ITTrigger  = GPIO_INT_Trigger_EDGE;
 *     GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
 *     GPIO_InitStruct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_ENABLE;
 *     GPIO_InitStruct.GPIO_DebounceTime = 10;
 *     GPIO_Init(&GPIO_InitStruct);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = GPIO28_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     GPIO_MaskINTConfig(GPIO_PIN_INPUT, DISABLE);
 *     GPIO_INTConfig(GPIO_PIN_INPUT, ENABLE);
 * }
 * \endcode
 */
void GPIO_Init(GPIO_InitTypeDef *GPIO_InitStruct);

/**
 * \brief    Fills each GPIO_InitStruct member with its default value.
 * \param[in]  GPIO_InitStruct: Pointer to a GPIO_InitTypeDef structure which will
 *             be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gpio_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
 *
 *     GPIO_InitTypeDef GPIO_InitStruct;
 *     GPIO_StructInit(&GPIO_InitStruct);
 *     GPIO_InitStruct.GPIO_Pin        = GPIO_GetPin(P4_0);
 *     GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_IN;
 *     GPIO_InitStruct.GPIO_ITCmd      = ENABLE;
 *     GPIO_InitStruct.GPIO_ITTrigger  = GPIO_INT_Trigger_EDGE;
 *     GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
 *     GPIO_InitStruct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_ENABLE;
 *     GPIO_InitStruct.GPIO_DebounceTime = 10;
 *     GPIO_Init(&GPIO_InitStruct);
 * }
 * \endcode
 */
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);

/**
 * \brief   Enable the specified GPIO pin interrupt.
 * \param[in]  GPIO_Pin: Where x can be 0 or 31.
 * \param[in]  NewState: Enable or disable interrupt
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gpio_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
 *
 *     GPIO_InitTypeDef GPIO_InitStruct;
 *     GPIO_StructInit(&GPIO_InitStruct);
 *     GPIO_InitStruct.GPIO_Pin        = GPIO_GetPin(P4_0);
 *     GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_IN;
 *     GPIO_InitStruct.GPIO_ITCmd      = ENABLE;
 *     GPIO_InitStruct.GPIO_ITTrigger  = GPIO_INT_Trigger_EDGE;
 *     GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
 *     GPIO_InitStruct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_ENABLE;
 *     GPIO_InitStruct.GPIO_DebounceTime = 10;
 *     GPIO_Init(&GPIO_InitStruct);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = GPIO28_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     GPIO_MaskINTConfig(GPIO_PIN_INPUT, DISABLE);
 *     GPIO_INTConfig(GPIO_PIN_INPUT, ENABLE);
 * }
 * \endcode
 */
void GPIO_INTConfig(uint32_t GPIO_Pin, FunctionalState NewState);

/**
 * \brief   Clear the specified GPIO pin interrupt pending bit.
 * \param[in] GPIO_Pin: Where x can be 0 or 31.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     driver_gpio_init();
 * }
 *
 * void GPIO28_Handler(void)
 * {
 *     GPIO_INTConfig(GPIO_PIN_INPUT, DISABLE);
 *     GPIO_MaskINTConfig(GPIO_PIN_INPUT, ENABLE);
 *     //Add user code here.
 *     GPIO_ClearINTPendingBit(GPIO_PIN_INPUT);
 *     GPIO_MaskINTConfig(GPIO_PIN_INPUT, DISABLE);
 *     GPIO_INTConfig(GPIO_PIN_INPUT, ENABLE);
}
 * \endcode
 */
void GPIO_ClearINTPendingBit(uint32_t GPIO_Pin);

/**
 * \brief   Mask the specified GPIO pin interrupt.
 * \param[in] GPIO_Pin: Where x can be 0 or 31.
 * \param[in] NewState: Disable or enable interrupt.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 *
 * void gpio_demo(void)
 * {
 *     driver_gpio_init();
 * }
 *
 * void GPIO28_Handler(void)
 * {
 *     GPIO_INTConfig(GPIO_PIN_INPUT, DISABLE);
 *     GPIO_MaskINTConfig(GPIO_PIN_INPUT, ENABLE);
 *
 *     //Add user code here.
 *
 *     GPIO_ClearINTPendingBit(GPIO_PIN_INPUT);
 *     GPIO_MaskINTConfig(GPIO_PIN_INPUT, DISABLE);
 *     GPIO_INTConfig(GPIO_PIN_INPUT, ENABLE);
 * }
 * \endcode
 */
void GPIO_MaskINTConfig(uint32_t GPIO_Pin, FunctionalState NewState);

/**
 * \brief   Get the GPIO_Pin through the given PAD num.
 * \param[in]  Pin_num: Pad num which can be from P0_0 to H_2, please refer to rtl876x.h "Pin_Number" part.
 * \retval  GPIO_Pin for GPIO initialization.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gpio_init(void)
 * {
 *     uint32_t gpio_pin = GPIO_GetPin(P4_0);
 *     //result: gpio_pin = GPIO_Pin_28
 * }
 * \endcode
 */
uint32_t GPIO_GetPin(uint8_t Pin_num);

/**
 * \brief   Get GPIOx(x is 0~31) value through the given pad.
 * \param[in] Pin_num: Pad num which can be from P0_0 to H_2, please refer to rtl876x.h "Pin_Number" part.
 * \return  GPIOx(x is 0~31) value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     uint8_t gpio_num = GPIO_GetNum(P4_0);
 *     //result: gpio_num = 28
 * }
 * \endcode
 */
uint8_t GPIO_GetNum(uint8_t Pin_num);

/**
 * \brief   Enable GPIO debounce clock.
 * \param[in] NewState: Disable or enable debounce clock.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     GPIO_DBClkCmd(ENABLE);
 * }
 * \endcode
 */
void GPIO_DBClkCmd(FunctionalState NewState);

/**
 * \brief   Read the specified input port pin.
 * \param[in] GPIO_Pin: Specifies the port bit to read.
 *      This parameter can be GPIO_Pin_x where x can be 0~31.
 * \return  The input port pin value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     uint8_t input_bit = GPIO_ReadInputDataBit(GPIO_GetPin(P4_0));
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t GPIO_ReadInputDataBit(uint32_t GPIO_Pin)
{
    uint8_t bitstatus = RESET;
    uint32_t data_in = GPIO->DATAIN;

    /* Check the parameters */
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if (GPIO->DATAIN & GPIO_Pin)
    {
        bitstatus = (uint8_t)SET;
    }

    return bitstatus;
}

/**
 * \brief  Read value of all  GPIO input data port.
 * \param  None.
 * \return GPIO input data port value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     uint32_t input_data = GPIO_ReadInputData();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t GPIO_ReadInputData(void)
{
    return GPIO->DATAIN;
}

/**
 * \brief   Read the specified output port pin.
 * \param[in] GPIO_Pin: Specifies the port bit to read.
 *      This parameter can be GPIO_Pin_x where x can be 0~31.
 * \retval The output port pin value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     uint8_t output_bit = GPIO_ReadOutputDataBit(GPIO_GetPin(P4_0));
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t GPIO_ReadOutputDataBit(uint32_t GPIO_Pin)
{
    uint8_t bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if (GPIO->DATAOUT & GPIO_Pin)
    {
        bitstatus = (uint8_t)SET;
    }

    return bitstatus;
}

/**
 * \brief   Read value of all GPIO output data port.
 * \param   None.
 * \return  GPIO output data port value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     uint32_t output_data = GPIO_ReadOutputData();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t GPIO_ReadOutputData(void)
{
    return ((uint32_t)GPIO->DATAOUT);
}

/**
 * \brief   Sets the selected data port bit.
 * \param[in] GPIO_Pin: Specifies the port bit to be written.
 *      This parameter can be GPIO_Pin_x where x can be 0~31 or GPIO_Pin_All.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     GPIO_SetBits(GPIO_GetPin(P4_0));
 * }
 * \endcode
 */
__STATIC_INLINE void GPIO_SetBits(uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIO->DATAOUT |= GPIO_Pin;
}

/**
 * \brief   Reset the selected data port bit.
 * \param[in] GPIO_Pin: Specifies the port bits to be written.
 *      This parameter can be GPIO_Pin_0 to GPIO_Pin_31 or GPIO_Pin_All.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     GPIO_ResetBits(GPIO_GetPin(P4_0));
 * }
 * \endcode
 */
__STATIC_INLINE void GPIO_ResetBits(uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIO->DATAOUT &= ~(GPIO_Pin);
}

/**
 * \brief  Set or clear the selected data port bit.
 * \param[in] GPIO_Pin: Specifies the port bit to be written.
 *      This parameter can be one of GPIO_Pin_x where x can be 0~31.
 * \param[in] BitVal: specifies the value to be written to the selected bit.
 *      This parameter can be one of the BitAction enum values:
 *     \arg Bit_RESET: To clear the port pin.
 *     \arg Bit_SET: To set the port pin.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     GPIO_WriteBit(GPIO_GetPin(P4_0), Bit_SET);
 * }
 * \endcode
 */
__STATIC_INLINE void GPIO_WriteBit(uint32_t GPIO_Pin, BitAction BitVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_ACTION(BitVal));

    if (BitVal != Bit_RESET)
    {
        GPIO->DATAOUT |= GPIO_Pin;
    }
    else
    {
        GPIO->DATAOUT &= ~(GPIO_Pin);
    }
}

/**
 * \brief   Set or clear  data port.
 * \param[in] PortVal: Specifies the value to be written to the selected bit.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     GPIO_Write(0xFFFFFFFF);
 * }
 * \endcode
 */
__STATIC_INLINE void GPIO_Write(uint32_t PortVal)
{
    GPIO->DATAOUT = PortVal;
}

/**
 * \brief  Get GPIO interrupt status.
 * \param[in] GPIO_Pin: Specifies the port bit to be written.
 *      This parameter can be one of GPIO_Pin_x where x can be 0~31.
 * \retturn The new state of GPIO_IT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     ITStatus int_status = GPIO_GetINTStatus(GPIO_GetPin(P4_0));
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus GPIO_GetINTStatus(uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIO->INTSTATUS & GPIO_Pin) == GPIO_Pin)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
 * \brief  Set debounce time.
 * \param[in] DebounceTime: Specifies interrupt debounce time.
 *      This parameter can be 1ms ~ 64ms.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gpio_demo(void)
 * {
 *     GPIO_Debounce_Time(10);
 * }
 * \endcode
 */
__STATIC_INLINE void GPIO_Debounce_Time(uint32_t DebounceTime)
{
    uint8_t count = 0;

    if (DebounceTime < 1)
    {
        DebounceTime = 1;
    }
    if (DebounceTime > 64)
    {
        DebounceTime = 64;
    }
#ifdef _IS_ASIC_
    //div = 14;//0xd  0b1101<<8
    GPIO_DBCLK_DIV = ((0xd << 8) | (1 << 12));
#else
    //div = 13;//0xc  0b1100<<8
    GPIO_DBCLK_DIV |= ((1 << 11) | (1 << 10) | (1 << 12));
    GPIO_DBCLK_DIV &= (~((1 << 9) | (1 << 8)));
#endif
    count = (244 * DebounceTime) / 100 - 1;
    GPIO_DBCLK_DIV &= (~((0xff << 0)));
    GPIO_DBCLK_DIV = GPIO_DBCLK_DIV + count;
}

/** \} */ /* End of group GPIO_Exported_Functions */


#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_GPIO_H_ */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

