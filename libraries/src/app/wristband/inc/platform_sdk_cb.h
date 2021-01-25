#ifndef __PLATFORM_SDK_CB_H__
#define __PLATFORM_SDK_CB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x_wdg.h"
#include "dfu_api.h"


void wristband_app_pre_main_cb(void);
//bool wristband_app_user_dfu_cb(DFU_PROGRESS_STATUS status, uint32_t length);
bool wristband_wdg_reboot_callback(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason);
#ifdef __cplusplus
}
#endif



#endif

