/*
 * File      : pm_wheel.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <vector>
#include <pm_widget.h>
#include <pm_sigslot.h>
#include <pm_timer.h>
#include <pm_image.h>
#include <pm_font.h>
#include <pm_translator.h>
#include "encoder/pm_encoder.h"

PERSIMMON_NAMESPACE_BEGIN

template<class> class ValueAnimator;

class Wheel : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Wheel)

    Wheel();
    virtual ~Wheel();

    void fixSelItem(void);
    void fixSelItem(unsigned int num);
    void fixItemPtich(bool actEnable = true);
    void selectItem(unsigned int num);

    virtual void paintEvent(PaintEvent *event);

    virtual void act(unsigned int num)
    {
        // rt_kprintf(" act num : %d\n", num);
    }

    virtual void end(unsigned int num)
    {
        // rt_kprintf(" end num : %d\n", num);
    }

    virtual void tap(unsigned int num)
    {
        // rt_kprintf(" tap num : %d\n", num);
    }

    bool isLoop() const { return m_isLoop; }
    void setLoop(bool enabled);

    void setNorShowSize(unsigned int normal);
    void setSelShowSize(unsigned int select);
    void setShowItems(unsigned int items);
    void setItemSize(unsigned int size);
    int getCenterSelNum(void);
    void setDirection(Direction dir);
    void enableBindTap(bool tap = true);
    void enableTapSwitch(bool tap = true);
    void switchNextItem(void);
    void switchPrevItem(void);
    void setInertialSeeds(unsigned int seeds);
    void setRadius(int radius);
    int updateMovePitch(int pitch);
    int moveUpPitch() const;
    int moveDownPitch() const;
    int limitedPitch(int pitch, int offset = 0) const;
    int calcCenterIndex(int pitch) const;

protected:
    virtual bool gestureEvent(GestureEvent *event);
    virtual bool encoderEvent(EncoderEvent* event);

    void onAnimation(int progress);
    void animFinished();
    void stopAnimation(void);
    void switchItem(bool type);

    Direction direction;
    int movePitch, gestPitch;
    int selNum;
    int lastIndex;
    int norSize, selSize, showItems, itemSize;
    int firstItemPitch, firstItemNum, itemMovePitch;
    int centerNum, oldNum;

    int lastPitch;
    int animMovePitch;
    unsigned int inertialSeeds;
    bool isFirstRender, m_isLoop;
    bool bindTapEnable, bindTapSwitch;
    ValueAnimator<int> *m_anim;
    int m_radius;
};

class WheelImage : public Wheel
{
public:
    PM_DECLARE_CLASSNAME(WheelImage)

    WheelImage();
    virtual ~WheelImage();

    void addImage(const Image &img);
    void addImage(const Vector<String> &imagePath);
    void clear(void) { image.clear(); }

    void remove(int index) { image.erase(image.begin() + index); }

    void setForeImage(const Image &img)
    {
        delete foreImage;
        *foreImage = img;
    }

    virtual void act(unsigned int num) { bindChanged(num); }

    virtual void tap(unsigned int num) { bindTap(num); }

    virtual void paintEvent(PaintEvent *event);

    Signal<unsigned int> bindChanged, bindTap;

protected:
    Vector<Image> image;

private:
    Image *foreImage;
};

class WheelString : public Wheel
{
public:
    PM_DECLARE_CLASSNAME(WheelString)

    WheelString();
    virtual ~WheelString();
    int checkIndex(int index, int firstIndex, int lastIndex);

    void addString(const String &str);
    void addString(const Vector<String> &str);
    void deleteString(int index);
    void empty();
    void setCenterSuffix(const char *str);
    void setNorFont(const Font &norFont);
    void setSelFont(const Font &selFont);
    void signalEnableString(bool enable = true);
    void setNorFontColor(const Color &nor);
    void setSelFontColor(const Color &sel);
    void getLimitedIndex(int &firstIndex, int &lastIndex);
    virtual void act(unsigned int num);
    virtual void tap(unsigned int num);
    virtual void paintEvent(PaintEvent *event);

    Signal<unsigned int> bindChanged, bindTap;
    Signal<const String &> bindChangedS, bindTapS;

protected:
    Vector<LocalString> m_normalList;
    String m_center_suffix;

private:
    Color norColor, selColor;
    bool signalString;
    Font norFont, selFont;
};

class CarouselImage : public Wheel
{
public:
    PM_DECLARE_CLASSNAME(CarouselImage)

    CarouselImage();
    virtual ~CarouselImage();

    void addImage(Image *img);
    void addString(const char *str);
    void removeItem(const char *str);
    void emptyItem(void);
    void selectItem(const char *str);

    String getItemString(int index)
    {
        String none;

        if (index >= 0 && index < (int)this->str.size())
        {
            return this->str[index];
        }

        return none;
    }

    void setSpaceDeep(int spaceDeep) { this->spaceDeep = spaceDeep; }

    void setSpaceX(int spaceX) { this->spaceX = spaceX; }

    void setVisualDistance(int visualDistance) { this->visualDistance = visualDistance; }

    void setVisualHeight(int visualHeight) { this->visualHeight = visualHeight; }

    void setZoom(double zoom) { this->zoom = zoom; }

    void setForeImage(Image *img)
    {
        delete foreImage;
        foreImage = img;
    }

    virtual void act(unsigned int num) { bindChanged(num); }

    virtual void tap(unsigned int num) { bindTap(num); }

    virtual void paintEvent(PaintEvent *event);

    Signal<int> bindChanged, bindTap;

protected:
    std::vector<String> str;
    std::vector<Image *> image;

private:
    Image *foreImage;
    double zoom;
    int spaceDeep;
    int spaceX;
    int visualDistance;
    int visualHeight;
};

PERSIMMON_NAMESPACE_END
