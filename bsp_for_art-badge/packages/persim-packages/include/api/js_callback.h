#ifndef JS_EVENT_H__
#define JS_EVENT_H__

#include <js_util.h>
#include <js_callbacks.h>

#define PM_CMD_JS_CALLBACK  0x10000
#define PM_CMD_UI_CALLBACK  0x10001
#define PM_CMD_EMQ_CALLBACK 0x10002

#ifdef __cplusplus
extern "C" {
#endif

bool js_callback_send_wait(js_callback_func callback, void *parameter, rt_int32_t timeout);
bool js_callback_send(js_callback_func callback, void *parameter);

#ifdef __cplusplus
}
#endif

#endif
