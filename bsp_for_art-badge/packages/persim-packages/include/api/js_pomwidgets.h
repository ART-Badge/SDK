#pragma once

#include <pm_widget.h>
#include <pm_vector.h>
#include <jsvm/jsvm.h>
#include "api/js_pomtypes.h"

namespace pom
{
class WidgetAdapters
{
public:
    WidgetAdapters(const TypeAdapters &types);
    ~WidgetAdapters();
    void reg(const persim::Widget &widget);
    JsValue prototype(const persim::Widget *widget);

private:
    struct Node
    {
        persim::Widget::TypeTag type;
        JsValue propotype;
        // VariantAdapter *adapter;
    };
    persim::Vector<Node> m_adapters;
    const TypeAdapters &m_types;
    class Private;
};
} // namespace pom
