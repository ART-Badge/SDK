#ifndef __BATTERY_MONITOR_H__
#define __BATTERY_MONITOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"
#include "app_msg.h"

/**
* @brief  init battery monitor--ADC
*
*
* @param   void
* @retval    void
*/

#define LOW_POWER_BATTERY_LEVEL         12
#define RESTORE_POWER_BATTERY_LEVEL     20 //20


void battery_add_hub_task(void);
void wristband_battery_driver_init(void);
void wristband_battery_enter_dlps(void);
void wristband_battery_exit_dlps(void);
bool wristband_battery_allowed_enter_dlps_check(void);
void wristband_power_off(void);

uint8_t wristband_get_battery_level(void);
void battery_event_handler(T_IO_MSG msg);



#ifdef __cplusplus
}
#endif

#endif
