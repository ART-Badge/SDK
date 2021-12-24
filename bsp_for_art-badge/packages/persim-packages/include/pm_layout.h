#pragma once

#include "pm_widget.h"

PERSIMMON_NAMESPACE_BEGIN

class Layout
{
public:
    Layout(Widget *widget = nullptr);
    virtual ~Layout();

    int spacing() const { return _spacing; }
    Widget *container() const { return _widget; }
    void setSpacing(int spacing);
    void setContainer(Widget *widget);
    void setMargins(int left, int top, int right, int bottom);
    virtual bool event(Event *event);
    virtual void layout() = 0;

private:
    int _spacing;
    Widget *_widget;
};

class AxialLayout : public Layout
{
public:
    AxialLayout(const Size &size = Size(), Widget *widget = nullptr);

    const Size &size() const { return _size; }
    int origin() const { return _origin; }
    int length() const { return _length; }
    void setOrigin(int origin);
    void setSize(const Size &size);
    virtual bool event(Event *event);
    virtual void layout();

protected:
    virtual void updater(Widget *widget, int offset) = 0;
    virtual int itemSize(Widget *widget)             = 0;

private:
    bool geometryEvent(GeometryEvent *event);
    bool childEvent(ChildEvent *event);

private:
    int _origin, _length;
    Size _size;
};

class HAxialLayout : public AxialLayout
{
public:
    HAxialLayout(const Size &size = Size(), Widget *widget = nullptr);

protected:
    virtual void updater(Widget *widget, int offset);
    virtual int itemSize(Widget *widget);
};

class VAxialLayout : public AxialLayout
{
public:
    VAxialLayout(const Size &size = Size(), Widget *widget = nullptr);

protected:
    virtual void updater(Widget *widget, int offset);
    virtual int itemSize(Widget *widget);

private:
    Size _size;
};

class HAxialExpandedLayout : public HAxialLayout
{
public:
    HAxialExpandedLayout(const Size &size = Size(), Widget *widget = nullptr);

protected:
    virtual void layout();
};

class VAxialExpandedLayout : public VAxialLayout
{
public:
    VAxialExpandedLayout(const Size &size = Size(), Widget *widget = nullptr);

protected:
    virtual void layout();
};

PERSIMMON_NAMESPACE_END
