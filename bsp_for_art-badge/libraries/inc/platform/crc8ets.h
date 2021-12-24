/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    crc8ets.h
  * @brief   CRC/FCS routines based on ETSI TS 101 369 V6.3.0 (1999-03) /
  *          GSM 07.10 version 6.3.0 Release 1997, B.3.5 Reversed CRC table.
  *          Generator polynomial: x**8 + x**2 + x + 1
  * @author  gordon
  * @date    2015-07-13
  * @version v1.0
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

#if ! defined (__CRC8ETS_H)
#define        __CRC8ETS_H


#if ! defined (__FLAGS_H)
#include <bt_flags.h>
#endif

/*
#if ! defined (__BASETYPE_H)
#include <basetype.h>
#endif
*/
#include <stdint.h>

/** @defgroup CRC_FCS CRC Implementation
  * @brief CRC implementation for specified polynomial
  * @{
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup CRC_FCS_Exported_Functions CRC Implementation Exported Functions
    * @brief
    * @{
    */

uint8_t crc8EtsGen(uint8_t *p, uint16_t len);
uint8_t crc8EtsCheck(uint8_t *p, uint16_t len, uint8_t rfcs);
/** @} */ /* End of group CRC_FCS_Exported_Functions */

/** @} */ /* End of group CRC_FCS */

#endif /**< ! defined (__CRC8ETS_H) */

