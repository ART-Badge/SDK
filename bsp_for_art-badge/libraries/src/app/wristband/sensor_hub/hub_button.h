#ifndef _WRISTBAND_BUTTON_H_
#define _WRISTBAND_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"
#include "trace.h"
#include "app_msg.h"


typedef struct
{
    uint8_t pressing  : 1;
    uint8_t counter   : 7;
} PressingDef_t;

extern PressingDef_t pressingParam;

void button_add_hub_task(void);
void wristband_button_driver_init(void);
void wristband_button_enter_dlps(void);
void wristband_button_exit_dlps(void);
bool wristband_button_allowed_enter_dlps_check(void);
bool wristband_button_system_wakeup_dlps_check(void);
void button_event_handler(T_IO_MSG msg);

#ifdef __cplusplus
}
#endif

#endif /* _WRISTBAND_BUTTON_H_ */

