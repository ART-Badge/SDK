#pragma once

#include "jsvm/jsvm.h"

namespace jsvm
{
class global_vm
{
public:
    global_vm();
    ~global_vm();
};

class locker
{
public:
    locker() { lock(); }
    ~locker() { unlock(); }

    static void lock();
    static bool try_lock(int time = 0);
    static void unlock();
};

bool available();
void print(const JsValue &value);
} // namespace jsvm
