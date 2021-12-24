#pragma once

#include "pm_string.h"
#ifdef PKG_PERSIMMON_USING_LOCAL
#include <CLocStrHelper.h>
#endif

PERSIMMON_NAMESPACE_BEGIN

#ifdef PKG_PERSIMMON_USING_LOCAL
class LocalString
{
public:
    LocalString() {}
    LocalString(const char *text) : _localString(text) {}
    LocalString(const String &text) : _localString(text) {}
    LocalString(const LocalString &other) : _localString(other._localString.GetStrName()) {}
    ~LocalString() {}
    LocalString &operator=(const LocalString &other)
    {
        _localString.SetStrName(other._localString.GetStrName());
        return *this;
    }
    LocalString &operator=(const char *text)
    {
        _localString.SetStrName(text);
        return *this;
    }
    LocalString &operator=(const String &text)
    {
        *this = text.c_str();
        return *this;
    }
    LocalString &operator+=(const char *text)
    {
        String str = _localString.name_str() + text;
        _localString.SetStrName(str.c_str());
        return *this;
    }
    LocalString &operator+=(const String &text)
    {
        String str = _localString.name_str() + text;
        _localString.SetStrName(str.c_str());
        return *this;
    }
    operator String() const { return tr(); }
    String tr() const { return _localString.loc_str(); }
    void clear() { *this = String(); }

private:
    mutable CLocString _localString;
};
#else
typedef String LocalString;
#endif

PERSIMMON_NAMESPACE_END
