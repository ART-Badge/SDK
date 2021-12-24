/*
 * File      : pm_wheelarc.h
 * COPYRIGHT (C) 2012-2021, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-27     realthread   the first version
 */

#pragma once

#include <vector>
#include <pm_widget.h>
#include <pm_sigslot.h>
#include <pm_translator.h>
#include <pm_animator.h>
#include <pm_font.h>

namespace persim
{

class WheelArc : public Widget
{
public:
    PM_DECLARE_CLASSNAME(WheelArc)

    enum EventFlags
    {
        Changed   = 1,
        StartMove = 2,
        EndMove   = 3,
        Tap       = 4,
    };

    WheelArc();
    virtual ~WheelArc();

    void setNorSize(const Size &size);
    void setSelSize(const Size &size);

    void setRadius(unsigned int radius);
    void setOffset(unsigned int offset);

    void setShowNum(unsigned int num);
    void showSelBox(bool enable = true);

    void setDirection(Direction direction);
    void enableCycle(bool enable = true);
    void setArcDirection(bool positive);

    void selectItem(unsigned int index);
    unsigned int selectIndex(void);

    void enableTapEvent(bool enable = true);
    void enablePaintItem(bool enable = true);

    Signal<unsigned int, unsigned int> bindEvent;

protected:
    virtual void paintEvent(PaintEvent* event);
    virtual void paintBottomItem(const Rect &rect, unsigned int index, const Rect &clipBox);
    virtual void paintTopItem(const Rect &rect, unsigned int index, const Rect &clipBox);
    virtual unsigned int getItemNum(void) = 0;

    bool _paintItem;

private:
    virtual bool gestureEvent(GestureEvent *event);
    void paintItemEvent();

    int fixIndex(int index);
    void passFinish(int index);

    void onAnimation(int progress);
    void stopAnimation(void);
    void animFinished(void);

    bool _isShowSelBox, _ArcDirection, _stateMove, _tapEvent;
    int _itemOffset, _itemAnimOffset;
    int _selIndex, _selIndexNew, _selIndexOld;
    unsigned int _radius, _offset, _showNum, _itemNum;
    Size _norSize, _selSize;
    Vector<Rect> _drawRect;
    Direction _direction;
    ValueAnimator<int> *_anim;
};

class WheelArcString : public WheelArc
{
public:
    PM_DECLARE_CLASSNAME(WheelArcString)

    WheelArcString();
    virtual ~WheelArcString();

    void empty();
    void addString(const String &str);
    void addString(const Vector<LocalString> &str);
    const String string(unsigned int index);
    void setNorFontColor(const Color &nor);
    void setSelFontColor(const Color &sel);
    void setNorFont(const Font &norFont);
    void setSelFont(const Font &selFont);

protected:
    virtual void paintBottomItem(const Rect &rect, unsigned int index, const Rect &clipBox);
    virtual void paintTopItem(const Rect &rect, unsigned int index, const Rect &clipBox);

    virtual unsigned int getItemNum(void);

private:
    Vector<LocalString> _strings;
    Color norColor, selColor;
    Font norFont, selFont;
};

class WheelArcWidget : public WheelArc
{
public:
    PM_DECLARE_CLASSNAME(WheelArcWidget)

    WheelArcWidget();
    virtual ~WheelArcWidget();

    void addWidget(Widget *widget);
    void rmWidget(Widget *widget);
    void rmWidget(unsigned int index);
    void empty();
    const String getName(unsigned int index);

protected:
    void fixShowItem();
    virtual void paintEvent(PaintEvent* event);
    virtual void paintBottomItem(const Rect &rect, unsigned int index, const Rect &clipBox);
    // virtual void paintTopItem(const Rect &rect, unsigned int index, const Rect &clipBox);

    virtual unsigned int getItemNum(void);
};

} // namespace persim
