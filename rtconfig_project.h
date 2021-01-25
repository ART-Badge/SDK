#ifndef RTCONFIG_PROJECT_H__
#define RTCONFIG_PROJECT_H__
#include <stdint.h>

#define GUI_DISP_FPS

#define PKG_USING_VGL
#define GUI_DISP_WATERMARK

#define GUIENGINE_USING_RES
#include <api/js_filerw.h>
#define JS_READ_FILE(filename, script) js_filerw_read(filename, script)
#define PKG_PERSIMMON_USING_LOCAL

#define TIMER_NUMBER_MAX  32

/* change rgb565 big/little-endian*/
#define RTGUI_RGB565_CHANGE_ENDIAN

/* psram as main memory heap for rt_malloc */
//#define PSRAM_AS_MAIN_HEAP

/* reimplement memory manage */
extern struct rt_memheap psram_heap;

#ifdef PSRAM_AS_MAIN_HEAP
#define PSRAM_ALLOC(...)            rt_malloc
#define PSRAM_REALLOC(...)          rt_realloc
#define PSRAM_FREE                  rt_free
#else
#define PSRAM_ALLOC(...)            rt_memheap_alloc(&psram_heap, __VA_ARGS__)
#define PSRAM_REALLOC(...)          rt_memheap_realloc(&psram_heap,__VA_ARGS__)
#define PSRAM_FREE                  rt_memheap_free
#endif

#define RT_KERNEL_MALLOC                        PSRAM_ALLOC
#define RT_KERNEL_REALLOC                       PSRAM_REALLOC
#define RT_KERNEL_FREE                          PSRAM_FREE

#define RT_JS_MALLOC                            PSRAM_ALLOC
#define RT_JS_REALLOC                           PSRAM_REALLOC
#define RT_JS_FREE                              PSRAM_FREE


/* lib pkgs */
#define PKG_USING_PERSIMMON_LIB
#define PKG_USING_PERSIMWEAR_JS_FWK_LIB
#define PKG_USING_REALTHREAD_PACKAGES_LIB

/* Privated Packages of RealThread */

#define PKG_USING_PERSIMMON_SRC
#define PKG_USING_PERSIMMON_SRC_LATEST_VERSION
#define PKG_PERSIMMON_USING_DEMO_NONE
#define PKG_PERSIMMON_DEMO_SEL "NONE"
#define PKG_USING_JS_PERSIMMON
#define PKG_USING_JS_PERSIMMON_LATEST_VERSION
#define PKG_JS_PERSIMMON_THREAD_STACK_SIZE 29
#define PKG_JS_PERSIMMON_ENABLE_LOGO
#define PKG_JS_PERSIMMON_LOGO_SHOW_TIME_MS 500
#define PKG_JS_PERSIMMON_ENABLE_DEFAULT_LOGO
#define PKG_JS_PERSIMMON_WAITING_READY_SEM

/* Network Utilities */

#define PKG_USING_MCF
#define MCF_PKT_MAX_SIZE 4096
#define MCF_USING_ACK
#define MCF_ACK_TIMOUT 500
#define MCF_USING_REQ_EXEC
#define MCF_REQ_THREAD_POOL_SIZE 1
#define MCF_REQ_THREAD_POOL_STACK_SIZE 4096
#define MCF_USING_LINK_UART
#define MCF_LINK_UART_CRC
#define PKG_USING_MCF_LATEST_VERSION
#define PKG_USING_URPC
#define URPC_USING_CJSON
#define URPC_USING_SAMPLES
#define PKG_USING_URPC_LATEST_VERSION
#define PKG_USING_BSAL

/* Role support */

#define PKG_BSAL_ROLE_PERIPHERAL
#define PKG_BSAL_ROLE_BROADCASTER
#define PKG_BSAL_SAMPLE_BAS_BLUFI_COMBINE

/* Profile support */

#define PKG_BSAL_SRV_BAS
#define PKG_BSAL_SRV_BLUFI
#define PKG_BSAL_RTK_STACK
#define PKG_BSAL_STACK_NAME "realtek"
#define PKG_USING_BSAL_LATEST_VERSION
#define PKG_USING_DCM
#define DCM_USING_VALUETYPE
#define DCM_USING_STORAGE
#define PKG_USING_DCM_LATEST_VERSION
#define PKG_USING_EMQ
#define PKG_USING_EMQ_LATEST_VERSION
#define PKG_USING_CFGM
#define CFGM_STORAGE_POOL_NAME "cfgmpool"
#define CFGM_MSH_CMD_ENABLE
#define PKG_USING_CFGM_LATEST_VERSION

#define JSFW_USING_SENSOR
#define SENS_ACCE_DEV_NAME "acce_acc"
#define SENS_GYRO_DEV_NAME "gyro_gyr"
#define SENS_MAG_DEV_NAME "mag_magn"
#define SENS_TEMP_DEV_NAME "temp_tem"
#define SENS_HUMI_DEV_NAME "humi_hum"
#define SENS_BARO_DEV_NAME "baro_bar"
#define SENS_LIGHT_DEV_NAME "li_light"
#define SENS_PROX_DEV_NAME "pr_proxi"
#define SENS_HR_DEV_NAME "hr_heart"
#define SENS_TVOC_DEV_NAME "tvoc_tvo"
#define SENS_NOISE_DEV_NAME "noi_nois"
#define SENS_STEP_DEV_NAME "step_ste"
#define SENS_FORCE_DEV_NAME "forc_for"
#define SENS_SPO2_DEV_NAME "spo2_sp2"
#define JSFW_USING_SENSOR_CMD

#define PKG_USING_UDBD

#endif
