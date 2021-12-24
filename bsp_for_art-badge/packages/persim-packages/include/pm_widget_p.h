#pragma once

#include "pm_font.h"
#include "pm_image.h"

PERSIMMON_NAMESPACE_BEGIN

#ifndef Impl
#define Impl(o) ((o)->_imp)
#endif

class Widget::Private
{
public:
    // status & flags
    uint32_t flags : 16;
    uint32_t textFlags : 12;
    uint32_t ontop : 1;
    uint32_t visible : 1;
    uint32_t layoutDirection : 2;

    uint32_t acceptGest : 8;
    uint32_t focusGest : 8;
    uint32_t opacity : 8;
    uint32_t gesture : 1;
    uint32_t scrollable : 1;

    // style
    Font font;
    Color bgColor, fgColor;
    Image bgImage;

    // geometry
    Rect rect;

    // widget tree
    Widget *parent;
    WidgetList children;

    // name
    String name;

    Private();
    ~Private();

    void setAlwaysTop(Widget *widget);
    void setAlwaysTop(const WidgetList::iterator &it);
    int insert(Widget *widget, int position);
    int remove(Widget *widget);
    Widget *remove(int index);
    WidgetList::iterator remove(const WidgetList::iterator &iter);
};

PERSIMMON_NAMESPACE_END
