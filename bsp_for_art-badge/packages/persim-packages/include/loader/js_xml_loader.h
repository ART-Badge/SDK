#pragma once

#include "pm_widget.h"
#include "js_ui_loader.h"
#include "js_panel.h"

struct ezxml;

class UiXmlLoader : public JsUiLoaderBase
{
public:
    UiXmlLoader();
    virtual ~UiXmlLoader();
    virtual const char *typeName() const                             = 0;
    virtual persim::Widget *parse(const JsValue &object, ezxml *xml) = 0;

protected:
    void parseWidget(const JsValue &object, persim::Widget *w, ezxml *xml);
    virtual void parseProperty(persim::Widget *w, const char *prop, ezxml *xml);
    virtual void parseBind(const JsValue &object, persim::Widget *w, const char *name,
                           const char *value);

private:
    void parseBinds(const JsValue &object, persim::Widget *w, ezxml *xml);
    void parsePropertys(persim::Widget *w, ezxml *xml);

private:
    UiXmlLoader *_next;
    friend class UiXmlLoaderManager;
};

class UiXmlLoaderManager
{
public:
    UiXmlLoaderManager();
    persim::Widget *parse(const JsValue &object, ezxml *xml);
    persim::Widget *parse(const JsValue &object, const persim::ByteArray &data,
                          JsUiLoader::LoadMode mode);
    void registered(UiXmlLoader *loader);

    static void destory();

private:
    class Private;
    Private *_p;
};
