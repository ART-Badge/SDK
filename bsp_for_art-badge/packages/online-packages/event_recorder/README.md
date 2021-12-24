# 事件记录器


## what

本软件包是一款轻量级的用于记录各种事件的调试工具，支持事件的实时记录与回放。可用于产品的压力测试、功能自动演示等场景。

事件的存储目前使用 [FlashDB](https://packages.rt-thread.org/detail.html?package=FlashDB) 的 TSDB 作为存储介质，所以默认会以依赖 FlashDB，后期也可以增加其他存储介质。

## how

### STEP1 启用并配置软件包

参考 menuconfig 配置描述

### STEP2 始化软件包

执行 `event_recorder_init()` 即可

### STEP3 注册事件源

记录事件前需要先注册事件源，可以通过 `event_source_register` 函数

```C
event_source_register(1, "touch", touch_repaly);
```

第三个参数为事件回放的时候的回调，可以在回调中发送历史记录的事件，大致写法如下：

```C
static void touch_repaly(uint8_t id, const uint8_t *value, size_t value_len)
{
    struct touch_event_value *event = (struct touch_event_value *) (value);
    rtgui_send_touch(event->x, event->y, event->pressed);
    rt_kprintf("touch event now replay");
}
```

### STEP4 启动录制

录制事件时可以通过如下函数启动，需要指定 `事件记录文件` 的存储位置，并作为入参传入

```c
int event_recorder_start(const char *recorder_dir, const char *recorder_name);
```

### STEP5 停止录制

通过 `event_recorder_stop` 函数即可

### STEP5 启动回放

当录制结束后，可以通过下面的 API 启动回放，回放时需传入录制时的 `事件记录文件` 路径

```C
int event_replay_start(const char *dir, const char *name, uint32_t time_s);
```

参数 `time_s` 为回放的时间，设置为 0 时为永久重复回放。设置为 `>0` 时，为回放持续的时间，单位为秒，到时间后，将会自动停止回放。

### STEP6 停止回放

如果希望提前结束回放，执行 `event_replay_stop` API 即可