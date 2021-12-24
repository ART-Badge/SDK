/*
 * File      : pm_application.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_string.h>

#define App() (::persim::Application::instance())

PERSIMMON_NAMESPACE_BEGIN

class Event;
class Widget;

class Application : private utils::noncopyable<Application>
{
public:
    Application();
    virtual ~Application();

    void exec();
    int waitEvent();
    bool eventLoop(int flags);
    bool eventLoop() { return eventLoop(waitEvent()); }
    Widget *window() const;
    void setWindow(Widget *root);
    bool postEvent(Event *event);
    void sendEvent(Event *event);
    void sendEvent(Widget *receiver, Event *event);
    void postTouch(int x, int y, bool pressed);
    void postEncoder(int data);

    void fullScreenDraw(bool enable);
    void fpsIncrease();
    int fps() const;

    static Application *instance() { return _instance; }
    static const char *version();

protected:
    virtual bool event(Event *event);

private:
    class Private;
    Private *_imp;
    static Application *_instance;
    friend class Widget;
};

PERSIMMON_NAMESPACE_END
