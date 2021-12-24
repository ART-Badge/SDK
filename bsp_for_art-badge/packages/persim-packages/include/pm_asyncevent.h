#pragma once

#include "pm_event.h"
#include "pm_semaphore.h"
#include "pm_application.h"
#include "pm_interruptlocker.h"

PERSIMMON_NAMESPACE_BEGIN

namespace async
{

template<class T> class worker : public PromiseEvent
{
public:
    worker() : PromiseEvent(), _ack(nullptr) {}
    virtual ~worker()
    {
        emit();
        Ack::detach(_ack);
    }

protected:
    void sync(const T &value) { _ack->v = value; }
    void emit() { _ack->emit(); }

private:
    struct Ack
    {
        T v;
        Ack(worker<T> *wk) : v(), w(wk), sem("pmack")
        {
            if (wk)
                wk->_ack = this;
        }
        void emit() { sem.release(); }
        bool take(int time = -1) { return w && sem.take(time); }
        bool try_take() { return w && sem.try_take(); }
        static void detach(Ack *d)
        {
            bool inuse;
            {
                InterruptLocker locker;
                inuse = d->w != nullptr;
                if (inuse)
                    d->w = nullptr;
            }
            if (!inuse)
                delete d;
        }

    private:
        worker<T> *w;
        Semaphore sem;
    };
    Ack *_ack;
    template<class> friend class await;
};

template<class T> class await
{
    typedef typename worker<T>::Ack Ack;

public:
    await(worker<T> *w) : _d(new Ack(w)) {}
    ~await()
    {
        if (_d)
            Ack::detach(_d);
    }
    await(await<T> &other)
    {
        _d       = other._d;
        other._d = nullptr;
    }
    const T &receive()
    {
        PM_ASSERT(_d);
        _d->take();
        return _d->v;
    }
    bool take(int time)
    {
        PM_ASSERT(_d);
        return _d->take(time);
    }
    bool try_take()
    {
        PM_ASSERT(_d);
        return _d->try_take(time);
    }

private:
    await<T> &operator=(await<T> &other);
    Ack *_d;
};

template<class T> await<T> post(worker<T> *event)
{
    await<T> ack(event);
    App()->postEvent(static_cast<Event *>(event));
    return ack;
}

class request : public PromiseEvent
{
public:
    request();
    virtual ~request();
    void post();
};

} // namespace async

PERSIMMON_NAMESPACE_END
