/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file
  * @brief
  * @details
  * @author
  * @date
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _WRISTBAND_CHARGER_H_
#define _WRISTBAND_CHARGER_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include "board.h"
#include "hub_task.h"
/**************************************************************************
* Charging state defination
***************************************************************************/
typedef enum
{
    NoCharge = 0,               /* Show not connect to plugs*/
    InCharging,        /* Device connect to the plugs*/
    ChargingComplete,      /* Show charging complete & still connect to plugs*/
} T_CHARGE_STATUS;



void charge_add_hub_task(void);
void wristband_charge_driver_init(void);
void wristband_charge_exit_dlps(void);
void wristband_charge_enter_dlps(void);
bool wristband_charge_allowed_enter_dlps_check(void);
bool wristband_charge_system_wakeup_dlps_check(void);
void charge_event_handler(T_IO_MSG msg);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */


#endif



