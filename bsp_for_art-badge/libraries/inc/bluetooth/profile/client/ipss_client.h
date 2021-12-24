/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ipss_client.c
  * @brief    Source file for client of internet protocol support service.
  * @details  Global data and function implement.
  * @author   Jeff_Zheng
  * @date     2017-12-05
  * @version  v1.0
  * *************************************************************************************
  */


#ifndef _IPSS_CLIENT_H_
#define _IPSS_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

/** @defgroup IPSS_CLIENT Internet Protocol Support Service Client
  * @brief Internet Protocol Support Service Client
  * @details

    IPSS client is used to discover Internet Protocol Support Service, this file could be used if application need to find IPSS.

    Application shall register IPSS client when initialization through @ref ipss_add_client function.

    Application can start discovery IPSS service through @ref ipss_find_srv_dcl function.

  * @{
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup IPSS_CLIENT_Exported_Functions Internet Protocol Support Service Client Exported Functions
  * @brief
  * @{
  */


/**
  * @brief add IPS service client.
  *
  * @param[in] p_func  pointer of app callback function called by client.
  * @return client id.
  */
uint8_t ipss_add_client(void *p_func);

/**
  * @brief discover IPS service by service uuid .
  *
  * @param[in] conn_id  connection id.
  * @return Operation result.
  * @retval true Operation success.
  * @retval false Operation failure.
  */
bool ipss_find_srv_dcl(uint8_t conn_id);

/** @} End of IPSS_CLIENT_Exported_Functions */

/** @} End of IPSS_CLIENT */

#ifdef  __cplusplus
}
#endif

#endif

