#pragma once

#include "pm_internal.h"
#include <cmath>

#define _PM_PI_D (3.14159265358979323846)
#define _PM_PI   float(_PM_PI_D)
#define _PM_PI2  float(_PM_PI_D * 2)

PERSIMMON_NAMESPACE_BEGIN

inline float pabs(float x)
{
    return std::abs(x);
}

inline float psin(float x)
{
    return std::sin(x);
}

inline float pcos(float x)
{
    return std::cos(x);
}

inline float ptan(float x)
{
    return std::tan(x);
}

inline float patan2(float x, float y)
{
    return std::atan2(x, y);
}

inline float phypot(float x, float y)
{
    return std::sqrt(x * x + y * y);
}

inline float pceil(float x)
{
    return std::ceil(x);
}

inline float pfloor(float x)
{
    return std::floor(x);
}

inline float pround(float x)
{
#ifdef __CC_ARM
    return x >= 0 ? pfloor(x + 0.5f) : pceil(x - 0.5f);
#else
    return round(x);
#endif
}

inline float pisfinite(float x)
{
#ifdef __CC_ARM
    return isfinite(x);
#else
    return std::isfinite(x);
#endif
}

inline float pinvsqrt(float x)
{
    return 1.f / std::sqrt(x);
}

inline float psqrt(float x)
{
    return std::sqrt(x);
}

inline float plog(float x)
{
    return std::log(x);
}

inline float ppow(float x, float y)
{
    return std::pow(x, y);
}

inline float pdeg(float x)
{
    return x * float(180 / _PM_PI_D);
}

inline float prad(float x)
{
    return x * float(_PM_PI_D / 180);
}

int psqrt_i32(int x);

#ifdef PKG_PERSIMMON_FAST_MATH
float pInvSqrtFast(float x);
float pSinFast(float x);
float pCosFast(float x);
float pAtan2Fast(float x, float y);
#endif

PERSIMMON_NAMESPACE_END
