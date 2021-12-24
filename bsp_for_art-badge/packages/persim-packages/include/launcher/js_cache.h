#pragma once

#include <jsvm/jsvm.h>
#include <pm_hashmap.h>
#include <pm_respack.h>

class JsScriptCache
{
public:
    JsScriptCache();
    ~JsScriptCache();
    JsValue parse(const String &path, const persim::ResPack &res = persim::ResPack());
    bool remove(const String &path);
    void clear();
    void insert(const JsValue &code, const String &path, const String &res);

    static JsScriptCache *instance();

private:
    JsValue lookup(const String &path) const;

private:
    static JsScriptCache *_instance;
    persim::HashMap<class JsScriptNode> _map;
};
