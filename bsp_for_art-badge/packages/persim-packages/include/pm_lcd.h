#pragma once

#ifdef __cplusplus
#include "pm_screendevice.h"

struct rt_device;

PERSIMMON_NAMESPACE_BEGIN

class LCDDevice : public ScreenDevice
{
public:
    LCDDevice(rt_device *device = nullptr);
    virtual ~LCDDevice();
    virtual void flush(const Rect &rect);
	void LCDDevice::setFramebuffer(uint8_t *ptr)
	{
		data.pixels = ptr;
	}
private:
    rt_device *_device;
};

PERSIMMON_NAMESPACE_END

extern "C" void pm_set_graphic_device(struct rt_device *device);

#else

void pm_set_graphic_device(struct rt_device *device);

#endif
