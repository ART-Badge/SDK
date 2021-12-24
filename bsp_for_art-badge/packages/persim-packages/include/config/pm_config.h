#pragma once

#include <rtthread.h>

#ifndef PKG_PERSIMMON_NDEBUG
#ifdef _MSC_VER
#include <cassert>
#define PM_ASSERT(expr) assert(expr)
#else
#define PM_ASSERT(expr) RT_ASSERT(expr)
#endif

#ifdef PKG_PERSIMMON_WHOLE_LOG
#define PM_LOG_LEVEL DBG_LOG
#else
#define PM_LOG_LEVEL DBG_INFO
#endif
#else
#define PM_LOG_LEVEL DBG_WARNING
#endif

#ifndef DBG_LEVEL
#define DBG_LEVEL PM_LOG_LEVEL
#endif

#ifndef PM_LOG_TAG
#define PM_LOG_TAG "persim"
#endif

#ifndef DBG_TAG
#define DBG_TAG PM_LOG_TAG
#endif

#include <rtdbg.h>

#ifndef PM_LOGE
#define PM_LOGE(...) LOG_E(__VA_ARGS__)
#endif
#ifndef PM_LOGW
#define PM_LOGW(...) LOG_W(__VA_ARGS__)
#endif
#ifndef PM_LOGI
#define PM_LOGI(...) LOG_I(__VA_ARGS__)
#endif
#ifndef PM_LOGD
#define PM_LOGD(...) LOG_D(__VA_ARGS__)
#endif

#ifndef PM_LOGRAW
#ifndef RT_USING_ULOG
#define PM_LOGRAW(...) rt_kprintf(__VA_ARGS__)
#else
#define PM_LOGRAW(...) LOG_RAW(__VA_ARGS__)
#endif
#endif
