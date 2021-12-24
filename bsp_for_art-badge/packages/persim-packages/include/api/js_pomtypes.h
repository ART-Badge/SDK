#pragma once

#include <pm_variant.h>
#include <pm_vector.h>
#include <jsvm/jsvm.h>

namespace pom
{
class VariantAdapter
{
public:
    VariantAdapter();
    virtual ~VariantAdapter();
    virtual persim::Variant toVariant(const JsValue &value) const   = 0;
    virtual JsValue toJsValue(const persim::Variant &variant) const = 0;
};

class TypeAdapters
{
public:
    TypeAdapters();
    ~TypeAdapters();
    const VariantAdapter *adapter(persim::Variant::Type type) const;
    template<typename T> void reg(VariantAdapter *adapter)
    {
        insert(persim::Variant::type<T>(), adapter);
    }

private:
    void insert(persim::Variant::Type type, VariantAdapter *adapter);
    struct Node
    {
        persim::Variant::Type type;
        VariantAdapter *adapter;
    };
    persim::Vector<Node> m_adapters;
    class Private;
};
} // namespace pom
