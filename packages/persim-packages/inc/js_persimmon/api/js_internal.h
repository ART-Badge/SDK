#ifndef JS_INTERNAL_H__
#define JS_INTERNAL_H__

#include <rtdevice.h>
#include <rtthread.h>
#include <js_util.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef rt_device_t(*js_device_require)(const char *name);

void js_device_require_init(void *func);
rt_device_t js_device_find(const char *name);

int js_file_init(js_value_t obj);
int js_storage_init(js_value_t obj);
int js_navigate_init(js_value_t obj);
int js_systeminfo_init(js_value_t obj);

int js_serial_init(js_value_t obj);
int js_serial_deinit(void);

int js_animation_init(js_value_t obj);
int js_animator_init(js_value_t obj);

int js_timeout_init(js_value_t obj);

int js_audio_player_init(js_value_t obj);

int js_videoplayer_init(js_value_t obj);

int js_wifi_init(js_value_t obj);

int js_download_init(js_value_t obj);
int js_download_deinit(void);

int js_untar_init(js_value_t obj);
int js_untar_deinit(void);

int js_iic_bus_init(js_value_t obj);

int js_image_init(js_value_t obj);

int js_freetype_init(js_value_t obj);

int make_directory(const char *dir);

int js_canvas_init(js_value_t obj);

int js_canvas_lite_init(js_value_t obj);
int js_translate_init(js_value_t obj);

int js_load_init(js_value_t obj);

int js_filerw_init(js_value_t obj);
#ifdef PKG_PERSIMMON_USING_LOCAL
int js_sys_lang_init(js_value_t obj);
#endif

int js_load_panel_init(js_value_t obj);

#ifdef __cplusplus
}
#endif

#endif
