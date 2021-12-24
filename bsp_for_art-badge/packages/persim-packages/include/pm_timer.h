/*
 * File      : pm_timer.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-05     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_sigslot.h>

PERSIMMON_NAMESPACE_BEGIN

class Timer : utils::noncopyable<Timer>
{
public:
    Timer(int interval = 1000);
    ~Timer();
    void setTimeout(int timeout);
    void setInterval(int timeout);
    int interval() const { return m_interval; }
    bool isPeriodic() const { return m_periodic; }
    void start();
    void stop();
    Signal<void> timeout;

    void *operator new(size_t size);
    void operator delete(void *ptr);

private:
    uint32_t m_isrun : 1;
    uint32_t m_periodic : 1;
    uint32_t m_interval : 30;
    uint32_t m_deadline;
    struct
    {
        Timer *next, *prev;
    } m_node;
    friend class TimerManager;
};

class SyncTimer : utils::noncopyable<SyncTimer>
{
public:
    SyncTimer();
    ~SyncTimer();
    bool isPeriodic() const { return m_periodic; }
    void start(bool isPeriodic = false);
    void stop();
    Signal<void> timeout;

    void *operator new(size_t size);
    void operator delete(void *ptr);

private:
    uint32_t m_periodic : 1;
    uint32_t m_isrun : 1;
    struct
    {
        SyncTimer *next, *prev;
    } m_node;
    friend class TimerManager;
};

void pm_timerTickService();
void pm_syncTimerTickService();

PERSIMMON_NAMESPACE_END
