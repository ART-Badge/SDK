#pragma once

#include "pm_system.h"

PERSIMMON_NAMESPACE_BEGIN

class RawAllocator;

class PersimSystemDefault : public PersimSystem
{
public:
    explicit PersimSystemDefault(PaintDevice *screen             = nullptr,
                                 const RawAllocator *chunk_alloc = nullptr,
                                 const RawAllocator *local_alloc = nullptr);
    virtual ~PersimSystemDefault();
};

PERSIMMON_NAMESPACE_END
