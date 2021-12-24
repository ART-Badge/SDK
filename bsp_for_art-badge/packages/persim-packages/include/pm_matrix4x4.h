#pragma once

#include "pm_rect.h"

PERSIMMON_NAMESPACE_BEGIN

class Point3
{
public:
    Point3();
    Point3(float x, float y, float z);
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
    void setX(float x) { _x = x; }
    void setY(float y) { _y = y; }
    void setZ(float z) { _z = z; }
    float dotProduct(const Point3 &rhs) const;
    float operator*(const Point3 &rhs) const { return dotProduct(rhs); }
    Point3 operator-() const { return Point3(-x(), -y(), -z()); }
    Point3 operator+(float d) const { return Point3(x() + d, y() + d, z() + d); }
    inline Point3 operator+(const Point3 &rhs) const;

private:
    float _x, _y, _z;
};

class Matrix4x4
{
public:
    Matrix4x4();
    Matrix4x4(const float m[4][4]);
    ~Matrix4x4() {}

    const float &m11() const { return _m[0][0]; }
    const float &m12() const { return _m[0][1]; }
    const float &m13() const { return _m[0][2]; }
    const float &m14() const { return _m[0][3]; }
    const float &m21() const { return _m[1][0]; }
    const float &m22() const { return _m[1][1]; }
    const float &m23() const { return _m[1][2]; }
    const float &m24() const { return _m[1][3]; }
    const float &m31() const { return _m[2][0]; }
    const float &m32() const { return _m[2][1]; }
    const float &m33() const { return _m[2][2]; }
    const float &m34() const { return _m[2][3]; }
    const float &m41() const { return _m[3][0]; }
    const float &m42() const { return _m[3][1]; }
    const float &m43() const { return _m[3][2]; }
    const float &m44() const { return _m[3][3]; }
    const float *data() const { return (float *)_m; }

    void translate(float dx, float dy, float dz);
    void translate(const Point3 &d) { translate(d.x(), d.y(), d.z()); }
    void rotateX(float theta);
    void rotateY(float theta);
    void rotateZ(float theta);
    void scale(float sx, float sy, float sz);
    void axisRotate(const Point3 &ux, const Point3 &uy, const Point3 &uz);
    void projection(float fov, float aspect, float zn, float zf);
    Point3 map(const Point3 &point);

    Matrix4x4 product(const Matrix4x4 &rhs) const;
    Matrix4x4 operator*(const Matrix4x4 &rhs) const { return product(rhs); }
    Matrix4x4 &operator*=(const Matrix4x4 &rhs) { return *this = *this * rhs; }

private:
    float _m[4][4];
};

inline Point3 Point3::operator+(const Point3 &rhs) const
{
    return Point3(x() + rhs.x(), y() + rhs.y(), z() + rhs.z());
}

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Point3 &);
const Logger &operator<<(const Logger &, const Matrix4x4 &);
#endif

PERSIMMON_NAMESPACE_END
