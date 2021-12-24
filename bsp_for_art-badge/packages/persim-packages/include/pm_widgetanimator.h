#pragma once

#include "pm_animator.h"
#include "pm_widget.h"
#include "pm_registermap.h"
#include "pm_property.h"

PERSIMMON_NAMESPACE_BEGIN

class VariantEvaluator
{
public:
    virtual Variant evaluate(float progress, const Variant &start, const Variant &end)
    {
        return start;
    }
};

class WidgetAnimator : public PropertyAnimator
{
public:
    WidgetAnimator(Widget *widget, const char *property);
    WidgetAnimator(Widget *widget, const String &property);
    WidgetAnimator(const Property &property);
    bool isNull() const { return m_property.isNull(); }
    Variant value() const { return m_property.get(); }
    Variant::Type valueType() const { return m_property.type(); }
    const Variant &startValue() const { return m_start; }
    const Variant &endValue() const { return m_end; }
    void setStartValue(const Variant &value) { m_start = value; }
    void setEndValue(const Variant &value) { m_end = value; }
    void setKeyFrame(int progress, const Variant &value);
    void setValueLimits(const Variant &start, const Variant &end)
    {
        m_start = start;
        m_end   = end;
    }

private:
    struct KeyFrame
    {
        int progress;
        Variant value;
        KeyFrame(int p, const Variant &v) : progress(p), value(v) {}
        bool operator==(int k) const { return progress == k; }
        struct Less;
    };
    void init(Widget *widget, const char *property);
    using PropertyAnimator::playFrame;
    virtual void playFrame(float progress);
    Property m_property;
    Variant m_start, m_end;
    VariantEvaluator *m_evaluator;
    Vector<KeyFrame> m_keyframes;
};

class VariantEvaluatorList
{
public:
    static VariantEvaluatorList *instance();
    VariantEvaluatorList();
    VariantEvaluator *find(Variant::Type type) const { return m_map.find(type); }
    template<class T> void insert(VariantEvaluator *evalutor)
    {
        m_map.insert(Variant::type<T>(), evalutor);
    }

private:
    RegisterMap<Variant::Type, VariantEvaluator> m_map;
    static VariantEvaluatorList *_instance;
};

PERSIMMON_NAMESPACE_END
