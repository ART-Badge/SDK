#pragma once

#include "pm_widget.h"
#include "js_ui_loader.h"
#include "js_panel.h"

class UiPldLoader : public JsUiLoaderBase
{
public:
    UiPldLoader();
    virtual ~UiPldLoader();
    virtual int typeId() const                                                 = 0;
    virtual persim::Widget *parse(const JsValue &object, const uint8_t *&data) = 0;

protected:
    void parseWidget(const JsValue &object, persim::Widget *w, const uint8_t *&data);
    virtual void parseProperty(persim::Widget *w, int id, const uint8_t *&data);
    virtual void parseBind(const JsValue &object, persim::Widget *w, const String &name,
                           const String &value);

private:
    void parseBinds(const JsValue &object, persim::Widget *w, const uint8_t *&data);
    void parsePropertys(persim::Widget *w, const uint8_t *&data);

private:
    friend class UiXmlLoaderManager;
};

class UiPldLoaderManager
{
public:
    UiPldLoaderManager();
    persim::Widget *parse(const JsValue &object, const uint8_t *&data);
    persim::Widget *parse(const JsValue &object, const persim::ByteArray &data,
                          JsUiLoader::LoadMode mode);
    void registered(UiPldLoader *loader);

    static void destory();

private:
    class Private;
    Private *_p;
};
