#pragma once

#include <pm_internal.h>
#include <pm_variant.h>
#include <pm_pointer.h>
#include "pm_widget.h"

PERSIMMON_NAMESPACE_BEGIN

#define WIDGET_PROPERTY(type_, name_, get_, set_)    \
    {                                                \
        (#name_), Variant::type<type_>(), get_, set_ \
    }

#define PROPERTY_COUNT(array) int(sizeof(array) / sizeof(array[0]))

struct PropertyDescriptor;

class Property
{
public:
    typedef void (*setter)(Widget *, const Variant &);
    typedef Variant (*getter)(Widget *);
    inline Property() : m_obj(), m_get(), m_set() {}
    inline Property(Widget *o, getter g, setter s, Variant::Type t);
    inline Property(Widget *o, const PropertyDescriptor &descriptor);
    bool isNull() const { return !m_obj; }
    void set(const Variant &v) { (m_obj && m_set) ? m_set(m_obj, v) : void(0); }
    Variant get() const { return (m_obj && m_get) ? m_get(m_obj) : Variant(); }
    Variant::Type type() const { return m_type; }

private:
    Pointer<Widget> m_obj;
    getter m_get;
    setter m_set;
    Variant::Type m_type;
};

struct PropertyDescriptor
{
    const char *name;
    Variant::Type type;
    Property::getter getter;
    Property::setter setter;
};

Property::Property(Widget *o, getter g, setter s, Variant::Type t)
    : m_obj(o), m_get(g), m_set(s), m_type(t)
{
}
Property::Property(Widget *o, const PropertyDescriptor &descriptor)
    : m_obj(o), m_get(descriptor.getter), m_set(descriptor.setter), m_type(descriptor.type)
{
}

PERSIMMON_NAMESPACE_END
