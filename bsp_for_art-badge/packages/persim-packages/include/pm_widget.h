/*
 * File      : pm_widget.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_color.h>
#include <pm_rect.h>
#include <pm_event.h>
#include <pm_vector.h>
#include <pm_pointer.h>

class JsPage;
class JsValue;

PERSIMMON_NAMESPACE_BEGIN

#define PM_DECLARE_CLASSNAME(type)                          \
    virtual const char *className() const { return #type; } \
    virtual TypeTag typeTag() const { return Widget::typeTag<type>(); }

class Font;
class Texture;
class Pixmap;
class Image;
class Property;
class TouchEvent;
class GestureEvent;
class EncoderEvent;
struct PropertyDescriptor;

class Direction
{
    enum
    {
        Cycle = 2
    };

public:
    enum ModeFlags
    {
        Vertical        = 0,
        Horizontal      = 1,
        VerticalCycle   = Vertical | Cycle,
        HorizontalCycle = Horizontal | Cycle
    };
    Direction(ModeFlags type = Vertical) : m_type(type) {}
    Direction(bool hori, bool cycle) : m_type((hori ? Horizontal : Vertical) | (cycle ? Cycle : 0))
    {
    }
    bool isHorizontal() const { return m_type & Horizontal; }
    bool isCycle() const { return m_type & Cycle; }
    bool operator==(const Direction &rhs) { return m_type == rhs.m_type; }
    bool operator!=(const Direction &rhs) { return !(*this == rhs); }

private:
    uint8_t m_type;
};

/**
 * Persimmon UI Widget
 */
class Widget : public GuardedObject
{
public:
    typedef const void *TypeTag;
    typedef Vector<Widget *> WidgetList;

    PM_DECLARE_CLASSNAME(Widget)

    enum WidgetFlags
    {
        PendingGeometryEvent = 1,
        PendingVisibleEvent  = 2
    };
    DEFINE_CLASS_ENUM_FLAG_OPERATORS(WidgetFlags);

    explicit Widget(Widget *parent = nullptr);
    virtual ~Widget();
    void deleteLater();

    int flags() const;
    Widget *parent() const;
    const String &name() const;
    void setName(const String &str);
    int textFlags() const;
    Color backgroundColor() const;
    Color foregroundColor() const;
    const Image &backgroundImage() const;
    const Font &font() const;
    int opacity() const;
    LayoutDirection layoutDirection() const;
    bool isTransparent() { return !backgroundColor().isOpaque(); }
    bool isVisible() const;
    bool gestureEnabled() const;
    bool scrollEnabled() const; 

    void setParent(Widget *widget);
    void setFlags(int flags, bool enabled = true);
    void setTextAlign(int flags);
    void setTextWrap(int flags);
    void setTextFlags(int flags);
    void setBackground(const Color &color);
    void setBackground(const Image &image);
    void setForeground(const Color &color);
    void setFont(const Font &font);
    void setOpacity(int opacity);
    void setLayoutDirection(LayoutDirection direction);
    void setVisible(bool status);
    void setGesture(bool status);
    void setScroll(bool status);

    bool insert(Widget *widget, int position = -1);
    void insertTop(Widget *widget);
    void remove(Widget *widget);
    Widget *remove(int index);
    void deleteAllChild();
    void deleteAllChildLater();

    Widget *findChild(const String &name) const;
    int findIndex(const String &name) const;
    int findIndex(const Widget *widget) const;
    const WidgetList &children() const;

    int width() const;
    int height() const;
    int x() const;
    int y() const;
    Point position() const;
    const Rect &geometry() const;
    Rect rect() const { return Rect(Point(0, 0), size()); }
    Size size() const;

    void setWidth(int width);
    void setHeight(int height);
    void setPositionX(int pos);
    void setPositionY(int pos);
    void setPosition(const Point &position);
    void setPosition(int x, int y) { setPosition(Point(x, y)); }
    void setGeometry(const Rect &rect);
    void setSize(const Size &size);
    void move(const Point &offset);
    void move(int x, int y) { move(Point(x, y)); }

    Rect mapToParent(const Rect &rect) const;
    Point mapToParent(const Point &point = Point()) const;
    Rect mapToGlobal(const Rect &rect) const;
    Point mapToGlobal(const Point &point = Point()) const;

    void acceptGesture(int type);
    void focusGesture(int type);
    void rejectGesture(int type);

    void update(const Rect &rect);
    void update() { update(rect()); }

    virtual void setData(JsPage *page, const JsValue &data);
    virtual void getData(JsValue &ret, const String &name) const;

    Property property(const String &name);
    virtual Property property(const char *name);
    virtual Vector<PropertyDescriptor> propertyDescriptors() const;

    void render(class Painter *painter, const Point &offset = Point(), const Rect &clip = Rect(),
                bool drawChild = true);

    template<class T> static TypeTag typeTag()
    {
        static const char v = 0;
        return &v;
    }

protected:
    virtual bool event(Event *event);
    virtual bool touchEvent(TouchEvent *event);
    virtual bool gestureEvent(GestureEvent *event);
    virtual bool encoderEvent(EncoderEvent *event);
    virtual void paintEvent(PaintEvent *event);

private:
    class Private;
    Private *_imp;
    friend class WidgetPrivate;
    friend class WidgetManager;
    friend class GestureManager;
    friend class EncoderManager;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &log, const Widget *w);
#endif

PERSIMMON_NAMESPACE_END
