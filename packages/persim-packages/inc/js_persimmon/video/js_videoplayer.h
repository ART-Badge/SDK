#pragma once

#include <pm_videoplayer.h>
#include "js_object.h"
#include <pm_image.h>
#include <js_callbacks.h>

/*struct video_callback_info
{
    int cmd;
    void *data;
    void *param1;
    void *param2;
} typedef video_cbinfo_t;*/

class JsVideoPlayer : public Persimmon::VideoPlayer
{
public:
    JsVideoPlayer();
    virtual ~JsVideoPlayer();

    void setJsFunction(js_value_t object, const char *func);

    void onChanged(unsigned int id);
    void onTap(unsigned int id);
    virtual void setData(void *data);

private:
    js_value_t jsCallFunc;
    js_value_t jsObj;
    js_value_t jsCustomProperty;
};
