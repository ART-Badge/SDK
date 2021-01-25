#ifndef JS_EVENT_H__
#define JS_EVENT_H__

#include <rtgui/event.h>
#include <js_util.h>
#include <js_callbacks.h>

#define PM_CMD_JS_CALLBACK     0x10000
#define PM_CMD_UI_CALLBACK     0x10001
#define PM_CMD_EMQ_CALLBACK    0x10002

#ifdef __cplusplus
extern "C" {
#endif

int js_set_app(struct rtgui_app *app);
struct rtgui_app *js_get_app(void);
rt_bool_t js_callback_send_wait(js_callback_func callback, void* parameter, rt_int32_t timeout);
rt_bool_t js_callback_send(js_callback_func callback, void *parameter);
int js_handle_callback(struct rtgui_event_command* event);
rt_bool_t js_ui_command(const char *cmd_string, js_value_t value);

#ifdef __cplusplus
}
#endif

#endif
