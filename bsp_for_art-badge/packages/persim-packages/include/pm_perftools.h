#pragma once

#include <rtconfig.h>

#ifdef PM_LOAD_TIME
#undef PM_LOAD_TIME
#ifndef PKG_PERSIMMON_USE_LOAD_TIME
#define PKG_PERSIMMON_USE_LOAD_TIME
#endif
#endif

#ifdef __cplusplus
#include "pm_internal.h"
#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

class Rect;

#ifdef PKG_PERSIMMON_USE_LOAD_TIME

class LoadTimerHelper
{
public:
    inline LoadTimerHelper();
    inline uint32_t time() const;
    inline uint32_t start() const { return _start; }
    inline void print(char tail = '\n') const;

private:
    uint32_t _start;
};

class LoadTimeGeneric
{
public:
    LoadTimeGeneric(const char *id, const String &msg, int type = -1);
    LoadTimeGeneric(const char *id, const char *prompt, const String &_msg, int type = -1);
    ~LoadTimeGeneric();

private:
    int _type;
    const char *_id, *_prompt;
    String _msg;
    LoadTimerHelper _timer;
};

class LoadTimePage
{
public:
    enum ResourceEnums
    {
        Xml,
        Image,
        Font,
        Module,
        Ui,
        Js,
        Page,
        App,
        UnknowResource,
        CountedResource = Ui
    };
    LoadTimePage(const String &name);
    ~LoadTimePage();

    static uint32_t *resourceTime() { return _res_time; }
    static bool setLogFilter(int type, bool status);
    static bool setLogFilter(const char *name, bool status);

private:
    String _name;
    LoadTimerHelper _timer;
    static uint32_t _res_time[CountedResource];
};

class LoadTimeUi
{
public:
    LoadTimeUi(const String &name);
    ~LoadTimeUi();

private:
    String _name;
    LoadTimerHelper _timer;
    uint32_t _res_time[LoadTimePage::CountedResource];
};

class LoadTimeXml
{
public:
    LoadTimeXml(const String &name);
    ~LoadTimeXml();

private:
    String _name;
    LoadTimerHelper _timer;
};

class LoadTimeImage
{
public:
    LoadTimeImage(const String &name);
    ~LoadTimeImage();

private:
    String _name;
    LoadTimerHelper _timer;
};

class LoadTimeFont
{
public:
    LoadTimeFont(const String &name);
    ~LoadTimeFont();

private:
    String _name;
    LoadTimerHelper _timer;
};

class LoadTimeModule
{
public:
    LoadTimeModule(const String &name);
    ~LoadTimeModule();

private:
    String _name;
    LoadTimerHelper _timer;
};

class LoadTimeDirect
{
public:
    LoadTimeDirect(int module, const String &name);
    ~LoadTimeDirect();

private:
    int _module;
    String _name;
    uint32_t _timer;
};

#define PM_LOAD_TIME_ALIAS(type, name, ...) LoadTime##type __Plt##type##__##name(__VA_ARGS__)

#define PM_LOAD_TIME(type, ...) PM_LOAD_TIME_ALIAS(type, , __VA_ARGS__)

#else

#define PM_LOAD_TIME_ALIAS(...)
#define PM_LOAD_TIME(...)

#endif

#ifdef PKG_PERSIMMON_USE_RENDER_TIME

class RenderTimeCounter
{
public:
    RenderTimeCounter(const class Widget *w, const Rect &rect);
    RenderTimeCounter(const char *type, const Rect &rect);
    ~RenderTimeCounter();
    static void begin();
    static void print();

private:
    int _index;
};

#define PM_RENDER_TIME_WIDGET(w, r) RenderTimeCounter __Rtc_Widget__(w, r)
#define PM_RENDER_TIME_MSG(msg, r)  RenderTimeCounter __Rtc_Msg__(msg, r)
#define PM_RENDER_TIME_BEGIN()      RenderTimeCounter::begin()
#define PM_RENDER_TIME_PRINT()      RenderTimeCounter::print()

#else

#define PM_RENDER_TIME_BEGIN()
#define PM_RENDER_TIME_WIDGET(w, r)
#define PM_RENDER_TIME_MSG(msg, r)
#define PM_RENDER_TIME_PRINT()

#endif

PERSIMMON_NAMESPACE_END
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PKG_PERSIMMON_USE_LOAD_TIME
int pm_plt_set_log_filter(const char *name, int status);
void pm_plt_print_help(void (*print)(const char *, const char *, int));
#endif

#ifdef PKG_PERSIMMON_USE_RENDER_TIME
void pm_render_time_enable(int status);
#endif

#ifdef __cplusplus
}
#endif
