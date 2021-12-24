#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

#define BSP_DIR "."
#define RTT_DIR "./rt-thread"
#define PKGS_DIR "./package-indexs"

/* RT-Thread Kernel */

#define RT_NAME_MAX 8
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 512
#define RT_USING_TIMER_SOFT
#define RT_TIMER_THREAD_PRIO 4
#define RT_TIMER_THREAD_STACK_SIZE 1024

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
#define RT_USING_MEMHEAP_AS_HEAP
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 256
#define RT_CONSOLE_DEVICE_NAME "segger"
#define RT_VER_NUM 0x40003
#define ARCH_ARM
#define RT_USING_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT

/* C++ features */

#define RT_USING_CPLUSPLUS

/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_USING_MSH_ONLY
#define FINSH_ARG_MAX 10

/* Device virtual file system */

#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define DFS_FILESYSTEMS_MAX 5
#define DFS_FILESYSTEM_TYPES_MAX 5
#define DFS_FD_MAX 64
#define RT_USING_DFS_ELMFAT

/* elm-chan's FatFs, Generic FAT Filesystem Module */

#define RT_DFS_ELM_CODE_PAGE 437
#define RT_DFS_ELM_WORD_ACCESS
#define RT_DFS_ELM_USE_LFN_3
#define RT_DFS_ELM_USE_LFN 3
#define RT_DFS_ELM_LFN_UNICODE_0
#define RT_DFS_ELM_LFN_UNICODE 0
#define RT_DFS_ELM_MAX_LFN 255
#define RT_DFS_ELM_DRIVES 2
#define RT_DFS_ELM_MAX_SECTOR_SIZE 4096
#define RT_DFS_ELM_REENTRANT
#define RT_USING_DFS_DEVFS
#define RT_USING_DFS_ROMFS

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 512
#define RT_USING_SYSTEM_WORKQUEUE
#define RT_SYSTEM_WORKQUEUE_STACKSIZE 2048
#define RT_SYSTEM_WORKQUEUE_PRIORITY 23
#define RT_USING_SERIAL
#define RT_SERIAL_RB_BUFSZ 256
#define RT_USING_I2C
#define RT_USING_I2C_BITOPS
#define RT_USING_PIN
#define RT_USING_PWM
#define RT_USING_MTD_NOR
#define RT_USING_PM
#define PM_TICKLESS_THRESHOLD_TIME 2
#define RT_USING_RTC
#define RT_USING_ALARM
#define RT_USING_SOFT_RTC
#define RT_USING_SENSOR
#define RT_USING_SENSOR_CMD
#define RT_USING_TOUCH

/* Using USB */


/* POSIX layer and C standard library */

#define RT_USING_LIBC
#define RT_USING_PTHREADS
#define PTHREAD_NUM_MAX 8
#define RT_USING_POSIX
#define RT_LIBC_FIXED_TIMEZONE 8

/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */


/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */

#define RT_USING_ULOG
#define ULOG_OUTPUT_LVL_I
#define ULOG_OUTPUT_LVL 6
#define ULOG_USING_ISR_LOG
#define ULOG_ASSERT_ENABLE
#define ULOG_LINE_BUF_SIZE 128

/* log format */

#define ULOG_USING_COLOR
#define ULOG_OUTPUT_TIME
#define ULOG_OUTPUT_LEVEL
#define ULOG_OUTPUT_TAG
#define ULOG_BACKEND_USING_CONSOLE

/* RT-Thread online packages */

/* IoT - internet of things */

#define PKG_USING_CJSON
#define PKG_USING_CJSON_V102
#define PKG_USING_EZXML
#define PKG_USING_EZXML_LATEST_VERSION

/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */

#define PKG_USING_JERRYSCRIPT
#define PKG_USING_JERRYSCRIPT_LATEST_VERSION
#define PKG_JMEM_HEAP_SIZE 256
#define PKG_JERRY_ENABLE_ERROR_MESSAGES
#define PKG_JERRY_ENABLE_LOGGING
#define PKG_JMEM_STATS
#define CONFIG_DISABLE_REGEXP_BUILTIN
#define PKG_CONFIG_DISABLE_ES2015

/* multimedia packages */


/* tools packages */

#define PKG_USING_SYSTEMVIEW
#define PKG_SYSVIEW_APP_NAME "RT-Thread Trace"
#define PKG_SYSVIEW_DEVICE_NAME "Cortex-M4"
#define PKG_SYSVIEW_TIMESTAMP_FREQ 1000000
#define PKG_SYSVIEW_CPU_FREQ 1000000
#define PKG_SYSVIEW_RAM_BASE 0x20000000
#define PKG_SYSVIEW_EVENTID_OFFSET 32
#define PKG_SYSVIEW_USE_CYCCNT_TIMESTAMP
#define PKG_SYSVIEW_SYSDESC0 "I#15=SysTick"
#define PKG_SYSVIEW_SYSDESC1 ""
#define PKG_SYSVIEW_SYSDESC2 ""

/* Segger RTT configuration */

#define PKG_SEGGER_RTT_MAX_NUM_UP_BUFFERS 3
#define PKG_SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 3
#define PKG_SEGGER_RTT_BUFFER_SIZE_UP 1024
#define PKG_SEGGER_RTT_BUFFER_SIZE_DOWN 16
#define PKG_SEGGER_RTT_PRINTF_BUFFER_SIZE 64
#define PKG_SEGGER_RTT_AS_SERIAL_DEVICE
#define PKG_SERIAL_DEVICE_NAME "segger"
#define PKG_SEGGER_RTT_MODE_ENABLE_NO_BLOCK_SKIP
#define PKG_SEGGER_RTT_MAX_INTERRUPT_PRIORITY 0x20

/* SystemView buffer configuration */

#define PKG_SEGGER_SYSVIEW_RTT_BUFFER_SIZE 1024
#define PKG_SEGGER_SYSVIEW_RTT_CHANNEL 1
#define PKG_SEGGER_SYSVIEW_USE_STATIC_BUFFER

/* SystemView Id configuration */

#define PKG_SEGGER_SYSVIEW_ID_BASE 0x10000000
#define PKG_SEGGER_SYSVIEW_ID_SHIFT 2
#define PKG_USING_SYSTEMVIEW_LATEST_VERSION
#define PKG_USING_VCONSOLE
#define PKG_USING_VCONSOLE_LATEST_VERSION

/* system packages */

/* acceleration: Assembly language or algorithmic acceleration packages */


/* Micrium: Micrium software products porting for RT-Thread */

#define PKG_USING_GUIENGINE
#define PKG_USING_GUIENGINE_LATEST_VERSION
#define PKG_USING_RGB888_PIXEL_BITS_24
#define PKG_USING_RGB888_PIXEL_BITS 24
#define GUIENGINE_CMD_STRING_MAX 16
#define GUIENGINE_USING_TTF
#define GUIENGINE_TTF_UTF8
#define GUIENGINE_USING_FONT16
#define GUIENGINE_USING_FONT12
#define GUIENGINE_USING_PNG
#define GUIENGINE_IMAGE_LODEPNG
#define GUIENGINE_USING_HDC
#define GUIENGINE_IMAGE_BMP
#define GUIENGINE_IMAGE_CONTAINER
#define GUIENGINE_NAME_MAX 16
#define PKG_USING_FAL
#define FAL_DEBUG_CONFIG
#define FAL_DEBUG 1
#define FAL_PART_HAS_TABLE_CFG
#define PKG_USING_FAL_V00500
#define PKG_FAL_VER_NUM 0x00500
#define PKG_USING_FLASHDB
#define FDB_USING_KVDB
#define FDB_USING_FAL_MODE
#define FDB_WRITE_GRAN_1BIT
#define FDB_WRITE_GRAN 1
#define FDB_USING_FILE_POSIX_MODE
#define FDB_DEBUG_ENABLE
#define PKG_USING_FLASHDB_LATEST_VERSION
#define PKG_FLASHDB_VER_NUM 0x99999
#define PKG_USING_THREAD_POOL
#define PKG_USING_THREAD_POOL_LATEST_VERSION

/* peripheral libraries and drivers */

#define PKG_USING_SENSORS_DRIVERS

/* AI packages */


/* miscellaneous packages */

#define PKG_USING_FASTLZ
#define PKG_USING_FASTLZ_V101
#define PKG_USING_FLEXIBLE_BUTTON
#define PKG_USING_FLEXIBLE_BUTTON_V100

/* samples: kernel and components samples */


/* entertainment: terminal games and other interesting software packages */


/* Hardware Drivers Config */

#define SOC_RTL_8762D

/* Onboard Peripheral Drivers */

/* On-chip Peripheral Drivers */

#define BSP_USING_UART
#define BSP_USING_UART0
#define BSP_USING_LCD
#define BSP_USING_PSRAM
#define BSP_USING_BLE
#define BSP_USING_REALTEK_BLE
#define BSP_USING_FLASH
#define BSP_USING_PIN
#define BSP_USING_I2C
#define BSP_USING_I2C1
#define BSP_USING_TOUCHPAD
#define BSP_USING_SIMULATOR_TOUCH
#define BSP_USING_BEEP
#define BSP_USING_ADC
#define BSP_USING_DLPS
#define BSP_USING_PWM
#define BSP_USING_PWM2
#define BSP_USING_PWM2_CH1

/* Board extended module Drivers */

#include "rtconfig_project.h"

#endif
