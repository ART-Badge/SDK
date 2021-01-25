#pragma once

#include <js_util.h>

class JsObject
{
public:
    JsObject();
    JsObject(js_value_t obj);
    virtual ~JsObject();

    js_value_t getJsObject(void)
    {
        return js_acquire_value(js_object);
    }
    void setJsObject(js_value_t obj);

    js_value_t getProperty(const char* name);

    void setProperty(const char* name, const js_value_t value);
    void setProperty(const char* name, const char* value);
    void setProperty(const char* name, bool value);
    void setProperty(const char* name, double value);
    void setProperty(const char* name, JsObject &obj);
    void setPrototype(const js_value_t value);

    bool callFunction(const char* name, const js_value_t args_p[], js_size_t args_count);
    void addFunction(const char* name, void *func);

    virtual void initJsObject(void);

    /* dump js value information */
    void dump(void);

protected:
    js_value_t js_object;
};

class JsEvent : public JsObject
{
public:
    JsEvent();
    virtual ~JsEvent();

    void setType(const char *type);
    void setTimeStamp(void);

    void setTarget(const char *id, const char *tagName, JsObject *dataset = NULL);
    void setCurrentTarget(const char *id, const char *tagName, JsObject *dataset = NULL);

    void setDetail(JsObject *detail);
    void setTouchs(JsObject *touchs, int size);

private:
};
