#ifndef __CSYSTEM_LANG_H__
#define __CSYSTEM_LANG_H__

#include "pm_string.h"
#include "pm_vector.h"
#include "jsvm/jsvm.h"

class CSystemLang
{
public:
    CSystemLang() {}
    virtual ~CSystemLang();
    static CSystemLang *instance()
    {
        if (_instance == nullptr)
        {
            _instance = new CSystemLang();
        }
        return _instance;
    }
    int SetSystemLanguage(const char *lang);
    const persim::String &GetSystemLanguage(void);
    void onLanguageChange(JsValue func);
    void offLanguageChange(JsValue func);

private:
    void languageChange(void);
    static CSystemLang *_instance;
    persim::String language;
    persim::Vector<JsValue> _callbackFunc;
};

#endif
