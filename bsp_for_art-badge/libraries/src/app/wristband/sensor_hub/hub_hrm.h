
#ifndef __WRISTBAND_HRM_H__
#define __WRISTBAND_HRM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "board.h"
#include "app_msg.h"


void hrm_sensor_add_hub_task(void);
void wristband_hrm_sensor_driver_init(void);
void wristband_hrm_sensor_device_init(void);
void wristband_hrm_sensor_enter_dlps(void);
void wristband_hrm_sensor_exit_dlps(void);
bool wristband_hrm_sensor_allowed_enter_dlps_check(void);
void hrs_event_handler(T_IO_MSG msg);


#ifdef __cplusplus
}
#endif

#endif //__WRISTBAND_HRM_H__


