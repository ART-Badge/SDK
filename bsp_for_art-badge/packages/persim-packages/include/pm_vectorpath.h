#pragma once

#include "pm_rect.h"
#include "pm_vector.h"
#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

class Pen;
class Font;
class Transform;

class VectorPath
{
public:
    enum VertexType
    {
        MoveToVertex,
        LineToVertex,
        CubicToVertex
    };

    struct VertexNode
    {
        float x, y;
        VertexType type;
        bool isMoveTo() const { return type == MoveToVertex; }
        bool isLineTo() const { return type == LineToVertex; }
        bool isCurveTo() const { return type == CubicToVertex; }

        operator PointF() const { return PointF(x, y); }
        VertexNode(const PointF &vertex, VertexType type_)
        {
            x    = vertex.x();
            y    = vertex.y();
            type = type_;
        }
        bool operator==(const VertexNode &e) const
        {
            return pFuzzyEqual(x, e.x) && pFuzzyEqual(y, e.y);
        }
        inline bool operator!=(const VertexNode &e) const { return !operator==(e); }
    };

    VectorPath();
    bool isEmpty() const;
    void clear();
    void append(const VectorPath &path);
    void moveTo(const PointF &point);
    void moveTo(float x, float y) { moveTo(PointF(x, y)); }
    void lineTo(const PointF &point);
    void lineTo(float x, float y) { lineTo(PointF(x, y)); }
    void conicTo(const PointF &c, const PointF &end);
    void cubicTo(const PointF &control1, const PointF &control2, const PointF &end);
    void arcTo(const PointF &center, float radiusX, float radiusY, float startAngle,
               float stopAngle);
    void addRect(PointF topLeft, PointF bottomRight);
    void addRect(float x, float y, float width, float height);
    void addText(float x, float y, const Font &font, const String &text);
    void addText(const PointF &point, const Font &font, const String &text);
    void translate(float tx, float ty);
    void translate(const PointF &offset) { translate(offset.x(), offset.y()); }
    void transform(const Transform &transform);
    VectorPath outline(const Pen &pen) const;
    const Vector<VertexNode> &path() const { return m_path; }

private:
    void firstInit();

private:
    Vector<VertexNode> m_path;
};

inline void VectorPath::addText(const PointF &point, const Font &font, const String &text)
{
    addText(point.x(), point.y(), font, text);
}

PERSIMMON_NAMESPACE_END
