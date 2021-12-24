#define DEBUG_TEST_BOOT_APP_ENABLE                                              0
#define DEBUG_TEST_OTA_APP_ENABLE                                               0
#define DEBUG_TEST_DLPS_APP_ENABLE                                              0
#define DEBUG_TEST_WDG_APP_ENABLE                                               0

#if (DEBUG_TEST_BOOT_APP_ENABLE == 1)
void xTimerTestBootDebugCallback(void *pxTimer);
void test_boot_debug_swtimer_init(void);
#endif

#if (DEBUG_TEST_DLPS_APP_ENABLE == 1)
#define DLPS_WAKEUP_TASKDELAY_EVENT                                             0
#define DLPS_WAKEUP_BT_EVENT                                                    0
#define DLPS_WAKEUP_RTC_EVENT                                                   0
#define DLPS_WAKEUP_SWTIMER_EVENT                                               0

void xTimerTestDLPSDebugCallback(void *pxTimer);
void xTimerTestDLPSWakeupCallback(void *pxTimer);
void test_DLPS_debug_swtimer_init(void);
void test_DLPS_wakeup_swtimer_init(void);
#endif

#if (DEBUG_TEST_OTA_APP_ENABLE == 1)
#define OTA_BUFFER_CHECK_EN                                                     0
#define OTA_AES_FLAG_EN                                                         0
#endif

