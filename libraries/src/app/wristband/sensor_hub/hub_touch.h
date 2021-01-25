#ifndef _WRISTBAND_TOUCH_H_
#define _WRISTBAND_TOUCH_H_

#include "rtl876x.h"
#include "trace.h"
#include "app_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

void touch_add_hub_task(void);
void wristband_touch_driver_init(void);
void wristband_touch_enter_dlps(void);
void wristband_touch_exit_dlps(void);
bool wristband_touch_allowed_enter_dlps_check(void);
bool wristband_touch_system_wakeup_dlps_check(void);
void touch_event_handler(T_IO_MSG msg);

#ifdef __cplusplus
}
#endif

#endif /* _WRISTBAND_TOUCH_H_ */

