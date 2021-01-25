/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_nvic.h
* \brief    The header file of NVIC  driver.
* \details  This file provides all NVIC firmware functions.
* \author   elliot chen
* \date     2015-05-19
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_NVIC_H_
#define _RTL876X_NVIC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    NVIC        NVIC
 * \brief       Manage the NVIC peripheral functions.
 * \ingroup     IO
 */

/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"

/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    NVIC_Exported_Types Init Params Struct
 * \ingroup     NVIC
 */

/**
 * \brief       NVIC init structure definition
 * \ingroup     NVIC_Exported_Types
 */

typedef struct
{
    IRQn_Type NVIC_IRQChannel;              /*!< Specifies the IRQ channel to be enabled or disabled.
                                                    This parameter can be a value of \ref IRQn_Type in rtl876x.h. */
    uint32_t NVIC_IRQChannelPriority;       /*!< Specifies the priority for the IRQ channel.
                                                    This parameter can be a value between 0 and x as described in the table.*/
    FunctionalState
    NVIC_IRQChannelCmd;     /*!< Enabled or disabled the IRQ channel defined in NVIC_IRQChannel.*/
} NVIC_InitTypeDef;


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    NVIC_Exported_Functions Peripheral APIs
 * \ingroup     NVIC
 * \{
 */

/**
 * \brief   Initializes the NVIC peripheral according to the specified
 *          parameters in NVIC_InitStruct.
 * \param[in]   NVIC_InitStruct: Pointer to a NVIC_InitTypeDef structure that contains
 *              the configuration information for the specified NVIC peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void nvic_config(void)
 * {
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);
 * }
 * \endcode
 */
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct);

/** \} */ /* End of group NVIC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_NVIC_H_ */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

