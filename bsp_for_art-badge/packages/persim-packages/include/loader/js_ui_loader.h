#pragma once

#include "js_utility.h"
#include "pm_bytearray.h"

namespace persim
{
class Widget;
}

class JsUiLoader
{
public:
    enum LoadMode
    {
        Normal,
        Page
    };
    JsUiLoader(const JsValue &object, const String &url);
    persim::Widget *parse(LoadMode mode = Normal) const;

private:
    JsValue m_object;
    persim::ByteArray m_data;
};

class JsUiLoaderBase
{
public:
    virtual ~JsUiLoaderBase();

protected:
    JsEventCallback makeCallback(const JsValue &obj, const char *name);
};
