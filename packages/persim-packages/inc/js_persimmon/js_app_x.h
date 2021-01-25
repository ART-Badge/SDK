
#pragma once

#include <vector>

#include <pm_page.h>
#include "js_object.h"
#include <pm_resource.h>

namespace Persimmon
{
class Page;

class JsAppX : public Resource, public JsObject, public Container
{
public:
    JsAppX(const char *url, const char *langue = NULL);
    virtual ~JsAppX();

    void onLaunch(js_value_t value = 0);
    void onShow(js_value_t value = 0);
    void onHidePage(void);
    void onHide(void);
    void onExitPage(void);
    void onExit(void);
    void onAppChoice(std::string apps, std::string path, std::string query);

    const std::string &getUrl(void);
    const char *getDirName(void);
    std::vector<Page*> *getPages(void);
    void navigateTo(js_value_t page = 0);

private:
    std::string _url;
    std::vector<Page*> _pages;
    bool _isLaunch, _isShow;
};
}
