/*
 * File      : pm_mem_config.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-22     realthread   the first version
 */
#ifndef __PM_MEM_CONFIG_H__
#define __PM_MEM_CONFIG_H__

#include <rtgui/rtgui_system.h>

#ifndef PERSIM_MALLOC
	#define PERSIM_MALLOC     rtgui_malloc
#endif

#ifndef PERSIM_REALLOC
	#define PERSIM_REALLOC    rtgui_realloc
#endif

#ifndef PERSIM_FREE
	#define PERSIM_FREE       rtgui_free
#endif

#endif /* __PM_MEM_CONFIG_H__ */
