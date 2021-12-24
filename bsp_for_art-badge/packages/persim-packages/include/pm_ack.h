/*
 * File      : pm_ack.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2021, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-11     realthread   first version
 */
#pragma once

#include "pm_internal.h"
#include "rtthread.h"

PERSIMMON_NAMESPACE_BEGIN

class ACK
{
public:
    ACK();
    virtual ~ACK();

    bool send(rt_ubase_t value);
    bool receive(rt_ubase_t *value, rt_int32_t timeout = RT_WAITING_FOREVER);

private:
    struct rt_mailbox _ack_mb;
    rt_base_t _ack_buffer;
};

PERSIMMON_NAMESPACE_END
