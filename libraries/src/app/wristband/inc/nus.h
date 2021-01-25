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
#ifndef _SIMPLE_BLE_SERVICE_H_
#define _SIMPLE_BLE_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_server.h>

#define GATT_NUS_TX_7F01_INDEX              0x02
#define GATT_NUS_RX_7F02_INDEX              0x05
#define GATT_NUS_RX_7F02_CCCD_INDEX         0x06
#define GATT_NUS_FITNESS_7F03_INDEX         0x08
#define GATT_NUS_MASS_7F04_INDEX            0x0B

typedef enum
{
    nus_service_proto_rx_state = 0,//01
    nus_service_proto_tx_state,//2
    nus_service_sportdata_tx_state,//3
    nus_service_bigdata_rx_state,//04
    nus_service_aduio_tx_state,//5
} nus_service_state;

T_SERVER_ID nus_add_service(void *p_func);

#ifdef __cplusplus
}
#endif

#endif /* _SIMPLE_BLE_SERVICE_H_ */
