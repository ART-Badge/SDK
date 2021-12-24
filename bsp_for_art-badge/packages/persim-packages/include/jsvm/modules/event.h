#pragma once

#include "jsvm/jsvm.h"

namespace jsvm
{

class Event : public JsNativeClassT<Event>
{
public:
    Event();
    bool emit(const char *name, const JsValueList &args);
    bool emit(const char *name, const JsValue *args, int argc);
    bool emit(const String &name, const JsValueList &args);
    bool emit(const String &name, const JsValue *args, int argc);
    void on(const String &name, const JsValue &function);
    void remove(const String &name, bool destory = true);
    void remove(const String &name, const JsValue &function);
    void lastOnly(const String &name, bool enabled);
    JsValue names() const;

private:
    class EventPrivate *_impl;
};

} // namespace jsvm
