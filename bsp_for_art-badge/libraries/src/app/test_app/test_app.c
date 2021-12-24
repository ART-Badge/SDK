#include "test_app.h"
#include "os_sched.h"
#include "os_timer.h"
#include <trace.h>
#include "rtl876x_io_dlps.h"
#include "app_msg.h"
#include "app_task.h"
#include "gap_msg.h"
#include "system_rtl876x.h"
#include "dfu_flash.h"
#include "otp.h"

extern uint32_t lps_wakeup_count_get(void);

#if (DEBUG_TEST_OTA_APP_ENABLE == 1 && SUPPORT_SILENT_OTA == 1)

#include <string.h>
//#include "dfu_int.h"
#include "rtl876x_wdg.h"
#include "Patch_platform.h"
#include "patch_header_check.h"

#if (TEST_OS == 1 && USE_OS  == 0)
bool os_sched_start_freertos(bool *p_result)
{
    DBG_DIRECT("USE FreeRTOS");
    *p_result = false;
    return false;
}

void osif_update_for_freertos()
{
    patch_osif_os_sched_start = os_sched_start_freertos;
}

USER_CALL_BACK os_patch = osif_update_for_freertos;
#endif



void set_dfu_config()
{
#if (OTA_AES_FLAG_EN == 0)
    OTP->ota_with_encryption_data = 0x0;
#else
    OTP->ota_with_encryption_data = 0x1;
#endif

}

//USER_CALL_BACK app_pre_main_cb = set_dfu_config;

#endif

#if (DEBUG_TEST_BOOT_APP_ENABLE == 1)

void *xTimerTestBootDebug; //debug ota test timer handle

void xTimerTestBootDebugCallback(void *pxTimer)
{
    uint32_t wakeupCount = lps_wakeup_count_get();
    DBG_DIRECT("wakeupCount = %d", wakeupCount);
    if (wakeupCount > 0)
    {
        DBG_DIRECT("BOOT_Test_Pass");
    }

    lps_mode_set(LPM_ACTIVE_MODE);

}

void test_boot_debug_swtimer_init(void)
{
    bool retval = os_timer_create(&xTimerTestBootDebug, "xTimerTestBootDebug",  1, \
                                  15 * 1000/*15s*/, true, xTimerTestBootDebugCallback);
    if (retval)
    {
        os_timer_start(&xTimerTestBootDebug);
    }
    else
    {
        DBG_DIRECT("boot test timer init failed");
    }
}

#endif

#if (DEBUG_TEST_DLPS_APP_ENABLE == 1)

void *xTimerTestDLPSDebug; //debug ota test timer handle
void *xTimerWakeupEvent; //debug ota test timer handle

void xTimerTestDLPSDebugCallback(void *pxTimer)
{
    uint32_t wakeupCount = lps_wakeup_count_get();
    DBG_DIRECT("DLPS Count = %d", wakeupCount);
    if (wakeupCount >= 550)
    {
#if DLPS_WAKEUP_TASKDELAY_EVENT == 1
        DBG_DIRECT("TASKDELAY_DLPS_TEST_PASS");
#endif

#if DLPS_WAKEUP_BT_EVENT == 1
        DBG_DIRECT("BT_EVENT_DLPS_TEST_PASS");
#endif

#if DLPS_WAKEUP_RTC_EVENT == 1
        DBG_DIRECT("RTC_EVENT_DLPS_TEST_PASS");
#endif

#if DLPS_WAKEUP_SWTIMER_EVENT == 1
        DBG_DIRECT("SWTIMER_EVENT_DLPS_TEST_PASS");
#endif
    }

    // lps_mode_set(LPM_ACTIVE_MODE);
}

#if (DLPS_WAKEUP_SWTIMER_EVENT == 1)
void xTimerTestDLPSWakeupCallback(void *pxTimer)
{
//      uint32_t wakeupCount = lps_wakeup_count_get();
//      DBG_DIRECT("DLPS Count = %d", wakeupCount);
}
#endif

#if (DLPS_WAKEUP_BT_EVENT == 1)
void xTimerTestDLPSWakeupCallback(void *pxTimer)
{
//              DBG_DIRECT("Advertising start");
    T_IO_MSG bee_io_msg = {0};
    bee_io_msg.type = IO_MSG_TYPE_BT_STATUS;
    bee_io_msg.subtype = GAP_MSG_LE_DEV_STATE_CHANGE;
    if (false == app_send_msg_to_apptask(&bee_io_msg))
    {
        APP_PRINT_ERROR0("[WDG] send IO_MSG_TYPE_BT_STATUS message failed!");
    }
}
#endif

void test_DLPS_debug_swtimer_init(void)
{
    bool retval = os_timer_create(&xTimerTestDLPSDebug, "xTimerDLPSDebug",  0, \
                                  1 * 60 * 1000/*1min*/, false, xTimerTestDLPSDebugCallback);

    // RTC_DeInit();

    if (retval)
    {
        os_timer_start(&xTimerTestDLPSDebug);
    }
}

#if (DLPS_WAKEUP_SWTIMER_EVENT == 1 || DLPS_WAKEUP_BT_EVENT == 1)
void test_DLPS_wakeup_swtimer_init(void)
{
    bool retval = os_timer_create(&xTimerWakeupEvent, "xTimerWakeupEvent",  1, \
                                  100/*100ms*/, true, xTimerTestDLPSWakeupCallback);

    if (retval)
    {
        os_timer_start(&xTimerWakeupEvent);
    }
}
#endif

#endif


