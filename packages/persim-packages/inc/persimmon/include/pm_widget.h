/*
 * File      : pm_widget.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <rtgui/rtgui.h>
#include <rtgui/rtgui_system.h>
#include <gesture.h>

#include <persimmon.h>
#include <pm_rect.h>
#include <pm_image.h>

#include <string>

namespace Persimmon
{
namespace utils
{

/* Borrowed from Boost::noncopyable. Add the template to give each noncopyable
* a different signature. */
template <typename T>
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:  // emphasize the following members are private
    noncopyable( const noncopyable& );
    noncopyable& operator=( const noncopyable& );
};

#define DEFINE_CLASS_ENUM_FLAG_OPERATORS(ENUMTYPE) \
    friend inline ENUMTYPE operator | (ENUMTYPE a,  ENUMTYPE b) \
           { return ENUMTYPE(((int)a)|((int)b)); } \
    friend inline ENUMTYPE operator |= (ENUMTYPE &a,  ENUMTYPE b) \
           { return (ENUMTYPE &)(((int &)a) |= ((int)b)); } \
    friend inline ENUMTYPE operator & (ENUMTYPE a,  ENUMTYPE b) \
           { return ENUMTYPE(((int)a)&((int)b)); } \
    friend inline ENUMTYPE operator &= (ENUMTYPE &a,  ENUMTYPE b) \
           { return (ENUMTYPE &)(((int &)a) &= ((int)b)); } \
    friend inline ENUMTYPE operator ~ (ENUMTYPE a) \
           { return (ENUMTYPE)(~((int)a)); } \
    friend inline ENUMTYPE operator ^ (ENUMTYPE a,  ENUMTYPE b) \
           { return ENUMTYPE(((int)a)^((int)b)); } \
    friend inline ENUMTYPE operator ^= (ENUMTYPE &a,  ENUMTYPE b) \
           { return (ENUMTYPE &)(((int &)a) ^= ((int)b)); }

#define DEFINE_CLASS_ENUM_FLAG_OPERATORS2(ENUMTYPE) \
    inline ENUMTYPE operator | (ENUMTYPE a,  ENUMTYPE b) \
           { return ENUMTYPE(((int)a)|((int)b)); } \
    inline ENUMTYPE operator |= (ENUMTYPE &a,  ENUMTYPE b) \
           { return (ENUMTYPE &)(((int &)a) |= ((int)b)); } \
    inline ENUMTYPE operator & (ENUMTYPE a,  ENUMTYPE b) \
           { return ENUMTYPE(((int)a)&((int)b)); } \
    inline ENUMTYPE operator &= (ENUMTYPE &a,  ENUMTYPE b) \
           { return (ENUMTYPE &)(((int &)a) &= ((int)b)); } \
    inline ENUMTYPE operator ~ (ENUMTYPE a) \
           { return (ENUMTYPE)(~((int)a)); } \
    inline ENUMTYPE operator ^ (ENUMTYPE a,  ENUMTYPE b) \
           { return ENUMTYPE(((int)a)^((int)b)); } \
    inline ENUMTYPE operator ^= (ENUMTYPE &a,  ENUMTYPE b) \
           { return (ENUMTYPE &)(((int &)a) ^= ((int)b)); }
}

class Window;

/**
 * Persimmon UI Widget
 */
class Widget: private utils::noncopyable<Widget>
{
public:
    enum WidgetFlags
    {
        WidgetDestoryLater = 0x400
    };

    Widget(int type = 0);
    virtual ~Widget();

    virtual Widget* depthCopy(void) const;
    void destoryLater(void);

    int getType(void);

    void setParent(Widget *w);
    Widget* getParent() const;
    
    void setName(std::string &str);
    const std::string &getName(void);

    void setInterestGesture(enum rtgui_gesture_type tp);
    enum rtgui_gesture_type interestGesture(void);

    Widget* setFont(struct rtgui_font* font)
    {
        _gc.font = font;
        invalidate();
        return this;
    }
    Widget* setFont(int size);

    struct rtgui_font* getFont(void)
    {
        return _gc.font;
    }

    void setBackground(rtgui_color_t color);
    void setBackground(Image *image);
    void setBackground(struct rtgui_dc* bg);
    void setTransparentBackground(Image *image);

    rtgui_color_t backgroundColor() const
    {
        return _gc.background;
    }
    rtgui_color_t foregroundColor() const
    {
        return _gc.foreground;
    }

    void setProvisionalBackground(struct rtgui_dc* bg)
    {
        provisionalBgDc = bg;
        invalidate();
    }

    void setForeground(rtgui_color_t color)
    {
        _gc.foreground = color;
        invalidate();
    }

    void setTextAlign(int align)
    {
        _gc.textalign = align;
        invalidate();
    }

    Rect getRect(void) const;
    void getRect(struct rtgui_rect* rect) const;
    virtual void setRect(const Rect &rect);
    void setRect(const rtgui_rect_t *rect)
    {
        setRect(Rect(rect));
    }
    void setRect(int x, int y, int width, int height)
    {
        setRect(Rect(x, y, width, height));
    }
    void setPosition(const Point &position);
    void setPosition(int x, int y)
    {
        setPosition(Point(x, y));
    }
    void setWidth(int width);
    void setHeight(int height);
    void setPositionX(int pos);
    void setPositionY(int pos);

    int getWidth() const;
    int getHeight() const;
    int getPositionX() const;
    int getPositionY() const;
    Point getAbsPosition() const;  /* get the absolute position of the widget */
    Point getPosition(void) const; /* get the position of the widget relative to the parent */
    Size getSize(void) const;
    void setSize(const Size &size);
    Rect getGeometry() const;      /* get the shape rectangle relative to the position of the parent */
    Rect getExtent(void) const;    /* get the shape rectangle of the absolute position */
    void getExtent(struct rtgui_rect* rect) const;

    Widget* getVisiableParent(void);

    bool isInAnimation(void)
    {
        return false ;
    }

    bool isTransparent(void)
    {
        return ((_gc.background & 0xFF000000) != 0xFF000000) ? true : false;
    }

    bool isExposed();

    void refresh(bool update = true);

    void invalidate(const Rect &rect);
    void invalidate()
    {
        invalidate(getRect());
    }
    rt_bool_t isInvalid();
    Rect invalidRect();
    void clearInvalid();
    void updateInvalid();
    void repaint(void);

    void setOpacity(int opacity);
    int opacity()
    {
        return m_opacity;
    }

    void move(int x, int y);
    void setAbsPosition(int x, int y);

    virtual void render(struct rtgui_dc* dc, const Point &dcPoint = Point(),
                        const Rect &srcRect = Rect(), RenderFlag flags = DrawNormal);
    virtual void renderBackground(struct rtgui_dc* dc, const Point &dcPoint = Point(),
                                  const Rect &srcRect = Rect(), RenderFlag flags = DrawNormal);

    struct rtgui_dc* getBufferDrawing(rt_uint8_t pixel_format, RenderFlag flags = DrawNormal, const Rect &rect = Rect() );
    struct rtgui_dc* getBufferDrawing(RenderFlag flags = DrawNormal);
    void bufferDrawing(struct rtgui_dc *buf, RenderFlag flags = DrawNormal, const Rect& rect = Rect());

    int flags() const;
    void setFlags(int flags, bool enabled = true);
    bool isVisiable() const;
    void hide();
    void show();
    virtual rt_bool_t onHide(struct rtgui_event *event);
    virtual rt_bool_t onShow(struct rtgui_event *event);

    virtual rt_bool_t eventHandler(struct rtgui_event *event);

    virtual Widget* getMouseOwner(const rtgui_gesture *gesture);

    virtual std::string dumpWidgets() const;
    std::string escapeDumpValue() const;

    virtual bool handleGestureEvent(struct rtgui_event_gesture *gev,
                                    const struct rtgui_gesture *gest);

    virtual void setGestureRect(const Rect &rect, bool enable = true)
    {
        gestureRect = rect;
        enableGestureRect = enable;
    }

    void renderFloating(struct rtgui_dc *dc);

    void setNumber(int num)
    {
        number = num;
    }
    int getNumber(void)
    {
        return number;
    }

    Widget* getPageWidgetByName(const char *name);

    void setTypeName(const char *str) { m_typeName = str; }
    const char* typeName() const { return m_typeName; }

    virtual std::string dumpValue() const { return std::string(); }

    virtual void setData(void *data);
    virtual void getData(void *ret, const std::string &name) const;

    bool _isVisiable;
    int _widgetType;
    Rect extent, extent_visiable;
    rtgui_gc_t _gc;
    /* widget flag */
    rt_int32_t flag;

protected:
    static void renderParentBackground(Widget* widget, Widget* parent, struct rtgui_dc* dc, const Point &dcPoint, const Rect &srcRect);

    struct rtgui_dc *backgroundDc, *provisionalBgDc;
    std::string name;

private:
    void renderOpacity(struct rtgui_dc* dc, const Point &dcPoint = Point(),
                       const Rect &srcRect = Rect(), RenderFlag flags = DrawNormal);

private:
    /* Set by Container::addChild. */
    Widget *parent;

    bool enableGestureRect;
    Rect gestureRect;
    Rect m_invalidRect;

    friend class Window;
    enum rtgui_gesture_type gest_tp;
    int number;
    int m_opacity;
    const char *m_typeName;
};

}

DEFINE_CLASS_ENUM_FLAG_OPERATORS2(rtgui_gesture_type);
