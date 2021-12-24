#pragma once

#include "pm_transform.h"
#include "pm_shareddata.h"
#include "pm_vector.h"

PERSIMMON_NAMESPACE_BEGIN

class TransformOperations
{
public:
    enum Operation
    {
        Scale,
        Rotate,
        Shear,
        Skew,
        Translate
    };

    bool isEmpty() const;
    bool hasMatrix() const { return !m_matrix.isIdentity(); }
    void scale(float sx, float sy);
    void rotate(float angle);
    void shear(float sh, float sv);
    void skew(float angleX, float angleY);
    void translate(float tx, float ty);
    const Transform &matrix() const;

    TransformOperations blend(const TransformOperations &from, float progress) const;

private:
    void push(Operation op, float a, float b);
    class Private : public SharedData
    {
    public:
        Private();
        virtual Private *copy() const;
        struct Node
        {
            Operation op;
            float a, b;
        };
        Vector<Node> operations;
    };
    SharedDataRef<Private> m_d;
    mutable Transform m_matrix;
};

PERSIMMON_NAMESPACE_END
