#pragma once

#include "pm_transformoperations.h"
#include "pm_variant.h"
#include "jsvm/jsvm.h"

using persim::TransformOperations;
using persim::Variant;

class JsTransform : public JsNativeClassT<JsTransform>
{
public:
    JsTransform(const TransformOperations &tr = TransformOperations());
    const TransformOperations &transform() const { return m_transform; }
    TransformOperations &transform() { return m_transform; }

    static JsValue newTransform(const TransformOperations &tr = TransformOperations());
    static JsValue newTransform(float a, float b, float c, float d, float tx, float ty);

    static Variant toVariant(const JsValue &value);
    static JsValue fromVariant(const persim::Variant &variant);

private:
    TransformOperations m_transform;
};
