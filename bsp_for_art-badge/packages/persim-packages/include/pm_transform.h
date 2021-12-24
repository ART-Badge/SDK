#pragma once

#include "pm_rect.h"

//#define TRANSFROM_CUSTOMIZED_FOR_SIFLY

PERSIMMON_NAMESPACE_BEGIN

class Transform
{
public:
    enum Flags
    {
        Identity  = 0,
        Translate = 0x01,
        Scale     = 0x02,
        Rotate    = 0x04,
        Shear     = 0x08,
        Project   = 0x10
    };
    struct DecomposedData
    {
        float sx, sy; // scale
        float angle;
        float ra, rb, rc, rd; // remainder
        float tx, ty;         // translate
    };
#ifdef TRANSFROM_CUSTOMIZED_FOR_SIFLY
    struct TransformData
    {
        bool valid;
        float sx, sy;   // scale
        float angle;    // rotate
        float tx1, ty1, tx2, ty2;   // translate
    };
#endif
    Transform();
    bool isAffine() const;
    bool isIdentity() const;
    bool isInvertible() const;
    void reset();
    void translate(float tx, float ty);
    inline void translate(const PointF &offset);
    void rotate(float angle);
    void scale(float sx, float sy);
    void shear(float sh, float sv);
    void skew(float angleX, float angleY);
    Transform &multiply(const Transform &rhs);
    void map(float *x, float *y) const;
    PointF map(float x, float y) const;
    PointF map(const PointF &point) const { return map(point.x(), point.y()); }
    Point map(int x, int y) const;
    Point map(const Point &point) const { return map(point.x(), point.y()); }
    Line map(const Line &line) const;
    Rect boundRect(const Rect &rect) const;
    float determinant() const;
    Transform inverse() const;
    Transform translated(float tx, float ty) const;
    inline Transform translated(const PointF &offset) const;

    float m11() const { return matrix[0][0]; }
    float m12() const { return matrix[0][1]; }
    float m13() const { return matrix[0][2]; }
    float m21() const { return matrix[1][0]; }
    float m22() const { return matrix[1][1]; }
    float m23() const { return matrix[1][2]; }
    float m31() const { return matrix[2][0]; }
    float m32() const { return matrix[2][1]; }
    float m33() const { return matrix[2][2]; }
    void setM11(float v) { matrix[0][0] = v; }
    void setM12(float v) { matrix[0][1] = v; }
    void setM13(float v) { matrix[0][2] = v; }
    void setM21(float v) { matrix[1][0] = v; }
    void setM22(float v) { matrix[1][1] = v; }
    void setM23(float v) { matrix[1][2] = v; }
    void setM31(float v) { matrix[2][0] = v; }
    void setM32(float v) { matrix[2][1] = v; }
    void setM33(float v) { matrix[2][2] = v; }
    float *data() const { return (float *)matrix; }

    void setMatrix(float m11, float m12, float m13, float m21, float m22, float m23, float m31,
                   float m32, float m33);

    float xScale() const;
    float yScale() const;
    bool decompose(DecomposedData &) const;
    void recompose(const DecomposedData &);
    void blend(const Transform &from, float progress);

    static bool squareToQuad(const PointF *quad, Transform &trans);
#ifdef TRANSFROM_CUSTOMIZED_FOR_SIFLY
    void transformData(TransformData &data) const;
#endif
private:
    float matrix[3][3];
    uint8_t m_flags;
#ifdef TRANSFROM_CUSTOMIZED_FOR_SIFLY
    TransformData m_data;
#endif
};

inline void Transform::translate(const PointF &offset)
{
    translate(offset.x(), offset.y());
}

inline Transform Transform::translated(const PointF &offset) const
{
    return translated(offset.x(), offset.y());
}

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Transform &);
#endif

PERSIMMON_NAMESPACE_END
