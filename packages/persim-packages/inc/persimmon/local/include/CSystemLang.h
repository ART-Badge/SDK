#ifndef __CSYSTEM_LANG_H__
#define __CSYSTEM_LANG_H__

#include <pm_font.h>

class CSystemLang
{
public:
    CSystemLang();
    virtual ~CSystemLang();
    int SetSystemLanguage(const char* lang);
    int GetSystemLanguage(char* lang);
    const  rtgui_font* GetSysFont(void);

private:
    struct rtgui_font *font;
};

#endif
