/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     simple_ble_service.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _APS_H_
#define _APS_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_server.h>

#define GATT_APS_COMM_4B02_INDEX              0x02
#define GATT_APS_COMM_4B02_CCCD_INDEX              0x03


T_SERVER_ID aps_add_service(void *p_func);

#ifdef __cplusplus
}
#endif

#endif /* _APS_H_ */
