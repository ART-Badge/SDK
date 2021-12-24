#pragma once

#include <jsvm/jsvm.h>
#include <jsvm/utils.h>

PERSIMMON_NAMESPACE_BEGIN
class Point;
class TouchEvent;
PERSIMMON_NAMESPACE_END

class JsEvent
{
    typedef persim::String String;

public:
    JsEvent();
    JsEvent(const String &id, const char *type, const JsValue &detail = JsValue());
    ~JsEvent() {}
    const JsValue &value() const { return _value; }

    void setType(const char *type);
    void setTimeStamp();

    void setTarget(const String &id, const char *tagName = nullptr, JsValue dataset = JsValue());
    void setCurrentTarget(const String &id, const char *tagName = nullptr,
                          JsValue dataset = JsValue());

    void setDetail(const JsValue &detail);
    void setTouch(const char *type, const persim::Point &point, uint32_t ts);
    void setTouch(const char *type, const persim::TouchEvent *event);
    void setTouchs(const JsValue *touchs, int count);
    void setProperty(const char *name, JsValue value) { _value.setProperty(name, value); }

private:
    JsValue _value;
};

class JsEventCallback
{
public:
    JsEventCallback() {}
    JsEventCallback(const JsValue &object, const char *func);
    JsEventCallback(const JsValue &object, const JsValue &func);
    JsEventCallback(const class JsPage *page, const JsValue &func);
    bool isFunction() const { return _function.isFunction(); }
    bool call(const JsEvent &event) const;
    const JsValue &object() const { return _object; }

private:
    JsValue _object, _function;
};

class JsEventHandler
{
public:
    const JsEventCallback &callback() const { return _callback; }
    void setCallback(const JsEventCallback &cb) { _callback = cb; }
    template<class T1, class T2> void setCallback(T1 o, T2 f) { _callback = JsEventCallback(o, f); }

private:
    JsEventCallback _callback;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
PERSIMMON_NAMESPACE_BEGIN
const Logger &operator<<(const Logger &, const JsEvent &);
PERSIMMON_NAMESPACE_END
#endif
