#pragma once

#include <pm_widget.h>
#include "api/js_pomtypes.h"
#include "js_pomwidgets.h"
#include "jsvm/jsvm.h"

namespace pom
{
class WidgetObject : public JsNativeClassT<WidgetObject>
{
public:
    WidgetObject(persim::Widget *widget) : m_widget(widget) {}
    virtual ~WidgetObject() {}
    persim::Widget *widget() const { return m_widget; }

private:
    persim::Pointer<persim::Widget> m_widget;
};

class POM
{
public:
    TypeAdapters &types() { return m_types; }
    WidgetAdapters &widgets() { return m_widgets; }
    JsValue object(persim::Widget *widget);
    static POM *instance();

protected:
    POM();

private:
    TypeAdapters m_types;
    WidgetAdapters m_widgets;
    static POM *m_instance;
};
} // namespace pom
