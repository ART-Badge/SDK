#ifndef RTCONFIG_PROJECT_H__
#define RTCONFIG_PROJECT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define GUI_DISP_FPS
#define GUI_DISP_FPS_CPU

/* use hardware vsync */
#define PKG_JS_PERSIMMON_WAITING_READY_SEM

/* change rgb565 big/little-endian*/
#define RTGUI_RGB565_CHANGE_ENDIAN

/* persim use bsp config */
#define PKG_PERSIMMON_CUSTOM_BSP_CONFIG

#define GUIENGINE_USING_RES
#define PKG_PERSIMMON_USING_LOCAL

/* use smodule*/
#define PKG_USING_SMODULE
#define PKG_SMODULE_USING_MATH

/* reimplement memory manage */
extern struct rt_memheap psram_heap;
extern struct rt_memheap pm_heap;

#define PSRAM_ALLOC(...)            rt_memheap_alloc(&psram_heap, __VA_ARGS__)
#define PSRAM_REALLOC(...)          rt_memheap_realloc(&psram_heap,__VA_ARGS__)
#define PSRAM_FREE                  rt_memheap_free

#define RT_KERNEL_MALLOC                        PSRAM_ALLOC
#define RT_KERNEL_REALLOC                       PSRAM_REALLOC
#define RT_KERNEL_FREE                          PSRAM_FREE

#define PM_ALLOC(...)            rt_memheap_alloc(&pm_heap, __VA_ARGS__)
#define PM_REALLOC(...)          rt_memheap_realloc(&pm_heap,__VA_ARGS__)
#define PM_FREE                  rt_memheap_free

#define RT_PM_MALLOC                        PM_ALLOC
#define RT_PM_REALLOC                       PM_REALLOC
#define RT_PM_FREE                          PM_FREE

#define PKG_USING_PERSIMMON_LIB  //--By PackingTools
#define PKG_USING_REALTHREAD_PACKAGES_LIB  //--By PackingTools

#define PKG_USING_BSAL
#define PKG_BSAL_ROLE_PERIPHERAL
#define PKG_BSAL_ROLE_BROADCASTER
#define PKG_BSAL_SAMPLE_BAS_BLUFI_COMBINE
#define PKG_BSAL_SRV_BAS
#define PKG_BSAL_SRV_BLUFI
#define PKG_BSAL_RTK_STACK
#define PKG_BSAL_STACK_NAME "realtek"
#define PKG_USING_BSAL_LATEST_VERSION

#ifdef __cplusplus
}
#endif

#endif
