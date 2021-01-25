
#ifndef __JS_PERSIM_H__
#define __JS_PERSIM_H__

#include <rthw.h>
#include <rtthread.h>
#include <rtgui/event.h>
#include <rtgui/rtgui_server.h>
#include <finsh.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JS_WIN  0x01
#define JS_WIN_CLOSE  0x01

void rtgui_logo_show(int timeSec);

void rtgui_logo_show_time(int ms);
int js_app_ready_sem_release(void);

int js_persim_thread_init(void);

#ifdef __cplusplus
}
#endif

#endif
