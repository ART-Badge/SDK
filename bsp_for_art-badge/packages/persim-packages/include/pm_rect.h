#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

class Size
{
public:
    Size();
    Size(int width, int height);
    Size(const Size &other);

    int width() const { return m_width; }
    int height() const { return m_height; }
    bool isEmpty() const;

    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);

    bool operator==(const Size &other) const;
    bool operator!=(const Size &other) const;
    Size operator+(const Size &other) const;
    Size operator-(const Size &other) const;
    Size &operator+=(const Size &other);
    Size &operator-=(const Size &other);

private:
    int m_width, m_height;
};

class Point
{
public:
    Point();
    Point(int x, int y);
    Point(const Point &other);

    bool isEmpty() const { return !(m_x || m_y); }
    int x() const { return m_x; }
    int y() const { return m_y; }
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setPoint(int x, int y);
    void translate(int dx, int dy);
    void translate(const Point &offset);
    int modules() const;

    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;
    Point operator+(const Point &other) const;
    Point operator-(const Point &other) const;
    Point operator-() const;
    Point &operator+=(const Point &other);
    Point &operator-=(const Point &other);

private:
    int m_x, m_y;
};

class PointF
{
public:
    PointF() : m_x(0), m_y(0) {}
    PointF(float x, float y) : m_x(x), m_y(y) {}
    PointF(const Point &point);
    PointF(const PointF &other);

    bool isEmpty() const { return !(m_x || m_y); }
    float x() const { return m_x; }
    float y() const { return m_y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setPoint(float x, float y) { setX(x), setY(y); }
    void translate(float dx, float dy);
    void translate(const PointF &offset);
    float modules() const;
    inline float dotProduct(const PointF &b) const;
    inline float crossProduct(const PointF &b) const;
    inline bool isAcute(const PointF &b) const;

    PointF &operator+=(const PointF &other);
    PointF &operator-=(const PointF &other);
    PointF &operator*=(float scale);
    PointF &operator/=(float diver);
    operator Point() const;

private:
    float m_x, m_y;
};

class Rect
{
public:
    Rect();
    Rect(int x, int y, int width, int height);
    Rect(const Point &point, const Size &size);
    Rect(const Point &topLeft, const Point &bottomRight);
    Rect(const Rect &other);

    int x() const { return m_x0; }
    int y() const { return m_y0; }
    int width() const { return m_x1 - m_x0 + 1; }
    int height() const { return m_y1 - m_y0 + 1; }
    int left() const { return m_x0; }
    int top() const { return m_y0; }
    int right() const { return m_x1; }
    int bottom() const { return m_y1; }

    Size size() const;
    Point center() const;
    Rect center(const Size &size) const;
    Rect center(const Rect &rect) const { return center(rect.size()); }
    Rect normalized() const;
    Point topLeft() const { return Point(m_x0, m_y0); }
    Point topRight() const { return Point(m_x1, m_y0); }
    Point bottomLeft() const { return Point(m_x0, m_y1); }
    Point bottomRight() const { return Point(m_x1, m_y1); }

    bool isEmpty() const;
    bool contains(int x, int y) const;
    bool contains(const Point &point) const;
    bool intersects(const Rect &rect) const;

    void clear();
    void setX(int x) { m_x0 = x; }
    void setY(int y) { m_y0 = y; }
    void setWidth(int width);
    void setHeight(int height);
    void setCoords(int x1, int y1, int x2, int y2);
    void setSize(int width, int height);
    void setSize(const Size &size);
    void setTop(int top) { setY(top); }
    void setLeft(int left) { setX(left); }
    void setBottom(int bottom) { m_y1 = bottom; }
    void setRight(int right) { m_x1 = right; }
    void moveTo(int x, int y);
    void moveTo(const Point &position);
    void move(const Point &offset);
    void move(int dx, int dy);
    void translate(const Point &offset) { move(offset); }
    void translate(int dx, int dy) { move(dx, dy); }

    Rect intersected(const Rect &rect) const;
    Rect united(const Rect &rect) const;

    bool operator==(const Rect &other) const;
    bool operator!=(const Rect &other) const;
    Rect operator&(const Rect &other) const;
    Rect operator|(const Rect &other) const;
    Rect operator+(const Point &offset) const;
    Rect operator-(const Point &offset) const;
    Rect &operator&=(const Rect &other);
    Rect &operator|=(const Rect &other);
    Rect &operator+=(const Point &offset);
    Rect &operator-=(const Point &offset);

private:
    int m_x0, m_y0, m_x1, m_y1;
};

class RectF
{
public:
    RectF();
    RectF(float x, float y, float width, float height);
    float x() const { return m_x0; }
    float y() const { return m_y0; }
    float width() const { return m_x1 - m_x0 + 1; }
    float height() const { return m_y1 - m_y0 + 1; }

    void setX(float x) { m_x0 = x; }
    void setY(float y) { m_y0 = y; }
    void setWidth(float width);
    void setHeight(float height);

private:
    float m_x0, m_y0, m_x1, m_y1;
};

class Line
{
public:
    Line(int x1, int y1, int x2, int y2);
    Line(const Point &point1, const Point &point2);
    bool isEmpty() const;
    int x1() const { return p1.x(); }
    int y1() const { return p1.y(); }
    int x2() const { return p2.x(); }
    int y2() const { return p2.y(); }
    Point &point1() { return p1; }
    Point &point2() { return p2; }
    const Point &point1() const { return p1; }
    const Point &point2() const { return p2; }
    int dx() const { return p2.x() - p1.x(); }
    int dy() const { return p2.y() - p1.y(); }
    void setPoint1(const Point &point) { p1 = point; }
    void setPoint2(const Point &point) { p2 = point; }
    void translate(const Point &offset);
    void translate(int dx, int dy);
    bool operator==(const Line &other) const;
    bool operator!=(const Line &other) const;
    Line operator+(const Point &offset) const;

private:
    Point p1, p2;
};

class LineF
{
public:
    enum IntersectType
    {
        NoIntersection,
        BoundedIntersection,
        UnboundedIntersection
    };
    LineF() {}
    LineF(float x1, float y1, float x2, float y2);
    LineF(const PointF &point1, const PointF &point2);
    inline bool isEmpty() const;
    inline bool fuzzyEmpty() const;
    float x1() const { return p1.x(); }
    float y1() const { return p1.y(); }
    float x2() const { return p2.x(); }
    float y2() const { return p2.y(); }
    PointF &point1() { return p1; }
    PointF &point2() { return p2; }
    const PointF &point1() const { return p1; }
    const PointF &point2() const { return p2; }
    float dx() const { return p2.x() - p1.x(); }
    float dy() const { return p2.y() - p1.y(); }
    void setPoint1(const PointF &point) { p1 = point; }
    void setPoint2(const PointF &point) { p2 = point; }
    void translate(const PointF &offset);
    void translate(float dx, float dy);
    inline void setLength(float len);
    LineF unitVector() const;
    inline LineF normalVector() const;
    float angle() const;
    float angleTo(const LineF &l) const;
    float length() const;
    float length2() const;
    LineF::IntersectType intersects(const LineF &l, PointF *point) const;
    bool operator==(const LineF &other) const;
    bool operator!=(const LineF &other) const;

private:
    PointF p1, p2;
};

inline Point operator*(const Point &lhs, float rhs)
{
    return Point(int(lhs.x() * rhs), int(lhs.y() * rhs));
}

inline Point operator*(const Point &lhs, int rhs)
{
    return Point(lhs.x() * rhs, lhs.y() * rhs);
}

inline bool operator==(const PointF &lhs, const PointF &rhs)
{
    return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

inline bool operator!=(const PointF &lhs, const PointF &rhs)
{
    return !(lhs == rhs);
}

inline PointF operator+(const PointF &lhs, const PointF &rhs)
{
    return PointF(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

inline PointF operator-(const PointF &lhs, const PointF &rhs)
{
    return PointF(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

inline PointF operator*(const PointF &lhs, const PointF &rhs)
{
    return PointF(lhs.x() * rhs.x(), lhs.y() * rhs.y());
}

inline PointF operator/(const PointF &lhs, const PointF &rhs)
{
    return PointF(lhs.x() / rhs.x(), lhs.y() / rhs.y());
}

inline PointF operator*(const PointF &lhs, int rhs)
{
    return PointF(lhs.x() * rhs, lhs.y() * rhs);
}

inline PointF operator*(const PointF &lhs, float rhs)
{
    return PointF(lhs.x() * rhs, lhs.y() * rhs);
}

inline PointF operator/(const PointF &lhs, float rhs)
{
    return PointF(lhs.x() / rhs, lhs.y() / rhs);
}

inline PointF operator-(const PointF &p)
{
    return PointF(-p.x(), -p.y());
}

inline float PointF::dotProduct(const PointF &b) const
{
    return x() * b.x() + y() * b.y();
}

inline float PointF::crossProduct(const PointF &b) const
{
    return x() * b.y() - b.x() * y();
}

inline bool PointF::isAcute(const PointF &b) const
{
    const float epsilon = -4e-6f;
    return dotProduct(b) >= epsilon && crossProduct(b) >= epsilon;
}

inline float LineF::length2() const
{
    const float x = dx(), y = dy();
    return x * x + y * y;
}

inline bool LineF::isEmpty() const
{
    return dx() == 0 && dy() == 0;
}

inline bool LineF::fuzzyEmpty() const
{
    return pFuzzyZero(dx()) && pFuzzyZero(dy());
}

inline void LineF::setLength(float len)
{
    if (fuzzyEmpty())
        return;
    LineF v = unitVector();
    p2.setPoint(p1.x() + v.dx() * len, p1.y() + v.dy() * len);
}

inline LineF LineF::normalVector() const
{
    return LineF(p1, p1 + PointF(dy(), -dx()));
}

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Size &);
const Logger &operator<<(const Logger &, const Point &);
const Logger &operator<<(const Logger &, const PointF &);
const Logger &operator<<(const Logger &, const Rect &);
const Logger &operator<<(const Logger &, const RectF &);
const Logger &operator<<(const Logger &, const Line &);
const Logger &operator<<(const Logger &, const LineF &);
#endif

PERSIMMON_NAMESPACE_END
