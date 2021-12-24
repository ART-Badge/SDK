/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_keyscan.h
* \brief    The header file of the peripheral KEYSCAN driver.
* \details  This file provides all KEYSCAN firmware functions.
* \author   tifnan_ge
* \date     2015-04-29
* \version  v1.0
* *********************************************************************************************************
*/


#ifndef _RTL876X_KEYSCAN_H_
#define _RTL876X_KEYSCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    KeyScan     KeyScan
 *
 * \brief       Manage the KeyScan peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"


/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    KeyScan_Exported_Types  Init Params Struct
 *
 * \ingroup     KeyScan
 */

/**
 * \brief       keyscan initialize parameters
 *
 * \ingroup     KeyScan_Exported_Types
 */
typedef struct
{
    uint16_t rowSize;           /*!< Specifies Keyscan Row Size.
                                        This parameter can be a value <=8. */
    uint16_t colSize;           /*!< Specifies Keyscan Column Size.
                                        This parameter can be a value <=20. */
    uint32_t detectPeriod;      /*!< Specifies detect period(ms). */
    uint16_t timeout;           /*!< Specifies sw interrupt timeout(s). */
    uint16_t scanInterval;      /*!< Specifies Keyscan release time. */
    uint32_t debounceEn;        /*!< Enable or disable debounce. */
    uint32_t scantimerEn;       /*!< Enable or disable scan timer. */
    uint32_t detecttimerEn;     /*!< Enable or disable detect timer. */
    uint16_t debounceTime;      /*!< Specifies debounce time,debounce time = 31.25us * debounceTime. */
    uint32_t detectMode;        /*!< Specifies Key Detect mode.
                                        This parameter can be a value of \ref Keyscan_Press_Detect_Mode */
    uint32_t fifoOvrCtrl;       /*!< Specifies Keyscan fifo over flow control.
                                        This parameter can be a value of \ref Keyscan_Fifo_Overflow_Control */

    uint16_t maxScanData;       /*!< Specifies max scan data allowable in each scan. */
    uint32_t scanmode;          /*!< Specifies Keyscan mode.
                                        This parameter can be a value of \ref Keyscan_Scan_Mode. */
    uint16_t clockdiv;          /*!< Specifies Keyscan clock divider.system clock/(SCAN_DIV+1)=scan clock. */
    uint8_t delayclk;           /*!< Specifies Keyscan delay clock divider.delay clock= scan clock/(DELAY_DIV+1)*/
    uint16_t fifotriggerlevel;  /*!< Specifies Keyscan fifo threshold to trigger interrupt KEYSCAN_INT_THRESHOLD.*/
    uint8_t debouncecnt;        /*!< Specifies Keyscan Debounce time. */
    uint8_t releasecnt;         /*!< Specifies Keyscan release time. */
    uint8_t keylimit;           /*!< Specifies max scan data allowable in each scan. */
} KEYSCAN_InitTypeDef;


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    KeyScan_Exported_Constants  Marco Definations
 *
 * \ingroup     KeyScan
 */

#define IS_KeyScan_PERIPH(PERIPH) ((PERIPH) == KEYSCAN)

/**
 * \def     Keyscan_Row_Number Keyscan Row Number
 */

#define IS_KEYSCAN_ROW_NUM(ROW) ((ROW) <= 8)

/**
 * \def     Keyscan_Column_Number Keyscan Column Number
 */

#define IS_KEYSCAN_COL_NUM(COL) ((COL) <= 20)

/**
 * \def     Keyscan_Debounce_Time Keyscan Debounce Time
 */
#define IS_KEYSCAN_MAX_SCAN_DATA(DATA_NUM) ((DATA_NUM) <= 26) //0 means no limit

/**
 * \defgroup    Keyscan_Scan_Mode Keyscan scan mode
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */
#define KeyScan_Manual_Scan_Mode        ((uint32_t)(0x0 << 30))
#define KeyScan_Auto_Scan_Mode          ((uint32_t)(0x1 << 30))
/** \} */

#define IS_KEYSCAN_DETECT_MODE(MODE)    (((MODE) == KeyScan_Detect_Mode_Edge) || ((MODE) == KeyScan_Detect_Mode_Level))

/**
 * \defgroup    Keyscan_Fifo_Overflow_Control Keyscan Fifo Overflow Control
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */

#define KeyScan_FIFO_OVR_CTRL_DIS_ALL   ((uint32_t)(0x0 << 28))   //discard all the new scan data when FIFO is full
#define KeyScan_FIFO_OVR_CTRL_DIS_LAST  ((uint32_t)(0x1 << 28))   //discard the oldest scan data when FIFO is full
/** \} */

#define IS_KEYSCAN_FIFO_OVR_CTRL(CTRL)  (((CTRL) == KeyScan_FIFO_OVR_CTRL_DIS_ALL) || ((CTRL) == KeyScan_FIFO_OVR_CTRL_DIS_LAST))

/**
 * \defgroup    Keyscan_Debounce_Config Keyscan Debounce Config
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */
#define KeyScan_Debounce_Enable              ((uint32_t)0x1 << 31)
#define KeyScan_Debounce_Disable             ((uint32_t)0x0 << 31)
/** \} */
#define IS_KEYSCAN_DEBOUNCE_EN(EN) (((EN) == KeyScan_Debounce_Enable) || ((EN) == KeyScan_Debounce_Disable))    //0 means no limit

/**
 * \defgroup    Keyscan_Scan_Interval_En   Keyscan Scan Interval Enable
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */
#define KeyScan_ScanInterval_Enable              ((uint32_t)(0x1 << 30))
#define KeyScan_ScanInterval_Disable             ((uint32_t)(0x0 << 30))
/** \} */
#define IS_KEYSCAN_SCANINTERVAL_EN(EN) (((EN) == KeyScan_ScanInterval_Enable) || ((EN) == KeyScan_ScanInterval_Disable))    //0 means no limit

/**
 * \defgroup    Keyscan_Release_Detect_Timer_En     Keyscan Release Detect Timer Enable
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */
#define KeyScan_Release_Detect_Enable              ((uint32_t)(0x1 << 29))
#define KeyScan_Release_Detect_Disable             ((uint32_t)(0x0 << 29))
/** \} */
#define IS_KEYSCAN_RELEASE_DETECT_EN(EN) (((EN) == KeyScan_Release_Detect_Enable) || ((EN) == KeyScan_Release_Detect_Disable))    //0 means no limit

/**
 * \defgroup    Keyscan_Press_Detect_Mode Keyscan Press Detect Mode
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */

#define KeyScan_Detect_Mode_Edge        ((uint32_t)(0x0 << 29))
#define KeyScan_Detect_Mode_Level       ((uint32_t)(0x1 << 29))
/** \} */

#define IS_KEYSCAN_DETECT_MODE(MODE)    (((MODE) == KeyScan_Detect_Mode_Edge) || ((MODE) == KeyScan_Detect_Mode_Level))

/**
 * \defgroup    Keyscan_Interrupt_Definition    Keyscan Interrupt Definition
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */
#define KEYSCAN_INT_THRESHOLD                    ((uint16_t)(0x1 << 4))
#define KEYSCAN_INT_OVER_READ                    ((uint16_t)(0x1 << 3))
#define KEYSCAN_INT_SCAN_END                     ((uint16_t)(0x1 << 2))
#define KEYSCAN_INT_FIFO_NOT_EMPTY               ((uint16_t)(0x1 << 1))
#define KEYSCAN_INT_ALL_RELEASE                  ((uint16_t)(0x1 << 0))
/** \} */

#define IS_KEYSCAN_CONFIG_IT(IT) ((((IT) & (uint32_t)0xFFF8) == 0x00) && ((IT) != 0x00))

/**
 * \defgroup    Keyscan_Flag_Definition   Keyscan Flag Definition
 * \{
 * \ingroup     KeyScan_Exported_Constants
 */
#define KEYSCAN_FLAG_FIFOLIMIT                       ((uint32_t)(0x1 << 20))
#define KEYSCAN_INT_FLAG_THRESHOLD                   ((uint32_t)(0x1 << 19))
#define KEYSCAN_INT_FLAG_OVER_READ                   ((uint32_t)(0x1 << 18))
#define KEYSCAN_INT_FLAG_SCAN_END                    ((uint32_t)(0x1 << 17))
#define KEYSCAN_INT_FLAG_FIFO_NOT_EMPTY              ((uint32_t)(0x1 << 16))
#define KEYSCAN_INT_FLAG_ALL_RELEASE                 ((uint32_t)(0x1 << 15))
#define KEYSCAN_FLAG_DATAFILTER                      ((uint32_t)(0x1 << 3))
#define KEYSCAN_FLAG_OVR                             ((uint32_t)(0x1 << 2))
#define KEYSCAN_FLAG_FULL                            ((uint32_t)(0x1 << 1))
#define KEYSCAN_FLAG_EMPTY                           ((uint32_t)(0x1 << 0))
/** \} */

#define IS_KEYSCAN_FLAG(FLAG)       ((((FLAG) & (uint32_t)0x01FF) == 0x00) && ((FLAG) != (uint32_t)0x00))
#define IS_KEYSCAN_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0x00C0) == 0x00) && ((FLAG) != (uint32_t)0x00))

/**
 * \def     Keyscan_FIFO_AVALIABLE_MASK
 */

#define STATUS_FIFO_DATA_NUM_MASK           ((uint32_t)(0x3F << 4))


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    Keyscan_Exported_functions Peripheral APIs
 * \{
 * \ingroup     KeyScan
 */

/**
 * \brief  Deinitializes the Keyscan peripheral registers to their default reset values(turn off keyscan clock).
 * \param[in]  KeyScan: selected KeyScan peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_keyscan_init(void)
 * {
 *     KeyScan_DeInit(APBPeriph_KEYSCAN);
 * }
 * \endcode
 */
void KeyScan_DeInit(KEYSCAN_TypeDef *KeyScan);

/**
 * \brief   Initializes the KeyScan peripheral according to the specified
 *          parameters in the KeyScan_InitStruct
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_InitStruct: Pointer to a KEYSCAN_InitTypeDef structure that
 *             contains the configuration information for the specified KeyScan peripheral
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_keyscan_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_KEYSCAN, APBPeriph_KEYSCAN_CLOCK, ENABLE);

 *     KEYSCAN_InitTypeDef KEYSCAN_InitStruct;
 *     KeyScan_StructInit(&KEYSCAN_InitStruct);

 *     KEYSCAN_InitStruct.rowSize  = KEYBOARD_ROW_SIZE;
 *     KEYSCAN_InitStruct.colSize  = KEYBOARD_COLUMN_SIZE;
 *     KEYSCAN_InitStruct.scanmode     = KeyScan_Manual_Scan_Mode;
 *     KEYSCAN_InitStruct.debounceEn   = vDebounce_En;

 *     KeyScan_Init(KEYSCAN, &KEYSCAN_InitStruct);

 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_ClearINTPendingBit(KEYSCAN, KEYSCAN_INT_SCAN_END);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, DISABLE);  // Unmask keyscan interrupt
 *     KeyScan_Cmd(KEYSCAN, ENABLE);
 * }
 * \endcode
 */
void KeyScan_Init(KEYSCAN_TypeDef *KeyScan, KEYSCAN_InitTypeDef *KeyScan_InitStruct);

/**
 * \brief  Fills each Keyscan_InitStruct member with its default value.
 * \param[in]  KeyScan_InitStruct: Pointer to an KEYSCAN_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_keyscan_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_KEYSCAN, APBPeriph_KEYSCAN_CLOCK, ENABLE);

 *     KEYSCAN_InitTypeDef KEYSCAN_InitStruct;
 *     KeyScan_StructInit(&KEYSCAN_InitStruct);

 *     KEYSCAN_InitStruct.rowSize  = KEYBOARD_ROW_SIZE;
 *     KEYSCAN_InitStruct.colSize  = KEYBOARD_COLUMN_SIZE;
 *     KEYSCAN_InitStruct.scanmode     = KeyScan_Manual_Scan_Mode;
 *     KEYSCAN_InitStruct.debounceEn   = vDebounce_En;

 *     KeyScan_Init(KEYSCAN, &KEYSCAN_InitStruct);

 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_ClearINTPendingBit(KEYSCAN, KEYSCAN_INT_SCAN_END);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, DISABLE);  // Unmask keyscan interrupt
 *     KeyScan_Cmd(KEYSCAN, ENABLE);
 * }
 * \endcode
 */
void KeyScan_StructInit(KEYSCAN_InitTypeDef *KeyScan_InitStruct);

/**
 * \brief  Enables or disables the specified KeyScan interrupt.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_IT: Specifies the KeyScan interrupts sources to be enabled or disabled.
 *      This parameter can be any combination of the following values:
 *      \arg KEYSCAN_INT_TIMEOUT: KeyScan timeout interrupt.
 *      \arg KEYSCAN_INT_OVER_THRESHOLD: Kescan FIFO data over threshold interrupt.
 *      \arg KEYSCAN_INT_SCAN_END: KeyScan scan end interrupt.
 * \param[in] newState: New state of the specified KeyScan interrupts.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_TIMEOUT, ENABLE);
 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_TIMEOUT, ENABLE);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_TIMEOUT, DISABLE);
 * }
 * \endcode
 */
void KeyScan_INTConfig(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT,
                       FunctionalState newState);

/**
 * \brief  Mask the specified KeyScan interrupt .
 * \param[in]  KeyScan: selected KeyScan peripheral.
 * \param[in]  KeyScan_IT: Specifies the KeyScan interrupts sources to be enabled or disabled.
 *      This parameter can be any combination of the following values:
 *      \arg KEYSCAN_INT_TIMEOUT: KeyScan timeout interrupt mask.
 *      \arg KEYSCAN_INT_OVER_THRESHOLD: Kescan FIFO data over threshold interrupt mask.
 *      \arg KEYSCAN_INT_SCAN_END: KeyScan scan end interrupt mask.
 * \param[in] newState: New state of the specified KeyScan interrupts mask.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_TIMEOUT, ENABLE);
 * }
 * \endcode
 */
void KeyScan_INTMask(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT,
                     FunctionalState newState);

/**
 * \brief  Read data from keyscan FIFO.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[out] outBuf: Buffer to save data read from KeyScan FIFO.
 * \param[in]  count: Data length to be read.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     uint16_t data[3] = {0};
 *     KeyScan_Read(KEYSCAN, data, 3);
 * }
 * \endcode
 */
void KeyScan_Read(KEYSCAN_TypeDef *KeyScan, uint16_t *outBuf, uint16_t count);

/**
 * \brief   Enable or disable the KeyScan peripheral.
 * \param[in] KeyScan: Selected KeyScan peripheral.
 * \param[in] NewState: New state of the KeyScan peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_Cmd(KEYSCAN, ENABLE);
 * }
 * \endcode
 */
void KeyScan_Cmd(KEYSCAN_TypeDef *KeyScan, FunctionalState NewState);

/**
 * \brief   Set filter data.
 * \param[in] KeyScan: selected KeyScan peripheral.
 * \param[in] data: Config the data to be filtered.
 *      This parameter should not be more than 9 bits.
 * \param[in] NewState: New state of the KeyScan peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_FilterDataConfig(KEYSCAN, 0x01, ENABLE);
 *
 * }
 * \endcode
 */
void KeyScan_FilterDataConfig(KEYSCAN_TypeDef *KeyScan, uint16_t data,
                              FunctionalState NewState);

/**
 * \brief   KeyScan debounce time config.
 * \param[in] KeyScan: selected KeyScan peripheral.
 * \param[in] time: Keyscan hardware debounce time.
 * \param[in] NewState: New state of the KeyScan debounce function.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_debounceConfig(KEYSCAN, 10, ENABLE);
 *
 * }
 * \endcode
 */
__STATIC_INLINE void KeyScan_debounceConfig(KEYSCAN_TypeDef *KeyScan, uint8_t time,
                                            FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    KeyScan->TIMERCR &= ~((0xff << 16) | BIT31);
    KeyScan->TIMERCR |= ((NewState << 31) | time << 16);

}

/**
 * \brief   Get KeyScan FIFO data num.
 * \param[in]  KeyScan: selected KeyScan peripheral.
 * \return  Data length in FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     uint16_t data_len = KeyScan_GetFifoDataNum(KEYSCAN);
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t KeyScan_GetFifoDataNum(KEYSCAN_TypeDef *KeyScan)
{
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    return (uint16_t)((KeyScan->STATUS & STATUS_FIFO_DATA_NUM_MASK) >> 4);
}

/**
 * \brief  Clear the KeyScan interrupt pending bit.
 * \param[in]  KeyScan: selected KeyScan peripheral.
 * \param[in]  KeyScan_IT: specifies the interrupt pending bit to clear.
 *      This parameter can be any combination of the following values:
 *      \arg KEYSCAN_INT_TIMEOUT: KeyScan timeout interrupt mask
 *      \arg KEYSCAN_INT_THRESHOLD
 *      \arg KEYSCAN_INT_OVER_READ
 *      \arg KEYSCAN_INT_SCAN_END
 *      \arg KEYSCAN_INT_FIFO_NOT_EMPTY
 *      \arg KEYSCAN_INT_ALL_RELEASE
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_ClearINTPendingBit(KEYSCAN, KEYSCAN_INT_TIMEOUT);
 * }
 * \endcode
 */
__STATIC_INLINE void KeyScan_ClearINTPendingBit(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_CONFIG_IT(KeyScan_IT));

    KeyScan->INTCLR |= KeyScan_IT;

    return;
}

/**
 * \brief   Clear the specified KeyScan flag.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_FLAG: Specifies the flag to clear.
 *      This parameter can be one of the following values:
 *      \arg KEYSCAN_FLAG_FIFOLIMIT
 *      \arg KEYSCAN_FLAG_DATAFILTER
 *      \arg KEYSCAN_FLAG_OVR
 * \return  None.
 * \note    KEYSCAN_FLAG_FULL and KEYSCAN_FLAG_EMPTY can't be cleared manually.
 *          They are cleared by hardware automatically.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_ClearFlags(KEYSCAN, KEYSCAN_FLAG_FIFOLIMIT);
 * }
 * \endcode
 */
__STATIC_INLINE void KeyScan_ClearFlags(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_FLAG)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_CLEAR_FLAG(KeyScan_FLAG));

    if (KeyScan_FLAG & KEYSCAN_FLAG_FIFOLIMIT)
    {
        KeyScan->INTCLR |= BIT8;
    }
    if (KeyScan_FLAG & KEYSCAN_FLAG_DATAFILTER)
    {
        KeyScan->INTCLR |= BIT7;
    }
    if (KeyScan_FLAG & KEYSCAN_FLAG_OVR)
    {
        KeyScan->INTCLR |= BIT5;
    }

    return;
}

/**
 * \brief   Check whether the specified KeyScan flag is set.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_FLAG: Specifies the flag to check.
 *      This parameter can be one of the following values:
 *      \arg KEYSCAN_FLAG_FIFOLIMIT:
 *      \arg KEYSCAN_FLAG_THRESHOLD:
 *      \arg KEYSCAN_FLAG_OVER_READ:
 *      \arg KEYSCAN_FLAG_SCAN_END:
 *      \arg KEYSCAN_FLAG_FIFO_NOT_EMPTY:
 *      \arg KEYSCAN_FLAG_ALL_RELEASE:
 *      \arg KEYSCAN_FLAG_DATAFILTER:
 *      \arg KEYSCAN_FLAG_OVR:
 *      \arg KEYSCAN_FLAG_FULL:
 *      \arg KEYSCAN_FLAG_EMPTY:
 * \return  The new state of KeyScan_FLAG (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     FlagStatus flag_status = KeyScan_GetFlagState(KEYSCAN, KEYSCAN_FLAG_OVR);
 *
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus KeyScan_GetFlagState(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_FLAG(KeyScan_FLAG));

    if ((KeyScan->STATUS & KeyScan_FLAG) != 0)
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
 * \brief  Read FIFO data.
 * \param[in] KeyScan: Selected KeyScan peripheral.
 * \return Keyscan FIFO data.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     uint16_t data = KeyScan_ReadFifoData(KEYSCAN);
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t KeyScan_ReadFifoData(KEYSCAN_TypeDef *KeyScan)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    return (uint16_t)(KeyScan->FIFODATA);
}

/** \} */ /* End of group KeyScan_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_KEYSCAN_H_ */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/
