#pragma once

#include "config/pm_config.h"
#include <stdint.h>

#define USING_PERSIMMON_NAMESPACE using namespace persim;
#define PERSIMMON_NAMESPACE_BEGIN \
    namespace persim              \
    {
#define PERSIMMON_NAMESPACE_END }

//-------- start of debug macros ----------
#ifndef PM_ASSERT
#define PM_ASSERT(expr) ((void)0)
#endif

#ifdef PKG_PERSIMMON_CRT_DEBUG
#define PM_CRT_ASSERT(expr) PM_ASSERT(expr)
#else
#define PM_CRT_ASSERT(expr) ((void)0)
#endif

#ifndef PM_LOGE
#define PM_LOGE(...)
#endif

#ifndef PM_LOGW
#define PM_LOGW(...)
#endif

#ifndef PM_LOGI
#define PM_LOGI(...)
#endif

#ifndef PM_LOGD
#define PM_LOGD(...)
#endif

#ifndef PM_LOGRAW
#define PM_LOGRAW(...)
#endif
//--------- end of debug macros -----------

#define PM_UNUSED(var) ((void)(var))

#ifdef _MSC_VER
#define __cpp11_version__ 199711L
#else
#define __cpp11_version__ 201103L
#endif

#if __cplusplus < __cpp11_version__
class nullptr_t
{
public:
    template<class T> inline operator T *() const { return 0; }
    template<class C, class T> inline operator T C::*() const { return 0; }

private:
    void operator&() const;
};
#define nullptr nullptr_t()
#endif

PERSIMMON_NAMESPACE_BEGIN

class Logger;

enum LayoutDirection
{
    LeftToRight = 0,
    RightToLeft = 1,
    LayoutAuto  = 2
};

enum AlignmentFlag
{
    AlignLeft           = 0x0001,
    AlignRight          = 0x0002,
    AlignHCenter        = 0x0004,
    AlignJustify        = 0x0008,
    AlignTop            = 0x0010,
    AlignBottom         = 0x0020,
    AlignVCenter        = 0x0040,
    AlignBaseline       = 0x0080,
    AlignCenter         = AlignHCenter | AlignVCenter,
    AlignHorizontalMask = 0x000f,
    AlignVerticalMask   = 0x00f0,
    AlignmentMask       = 0x00ff
};

enum TextWrapFlag
{
    TextMultiLine = 0x0100,
    TextWordWrap  = 0x0200,
    TextCharWrap  = 0x0400,
    TextWrapMask  = 0x0f00
};

class PixelFormat
{
public:
    enum Format
    {                // built-in pixel formats
        RGB565,      // RGB format, 16bpp
        RGB888,      // RGB format, 24bpp
        BGR888,      // BGR format, 24bpp
        XRGB8888,    // RGB format, 32bpp, X is a useless channel
        ARGB8888,    // ARGB format, 32bpp, support an alpha channel
        ARGB4444,    // ARGB format, 16bpp, support an alpha channel
        A8,          // alpha format, 8bpp
        A4,          // alpha format, 4bpp
        A2,          // alpha format, 2bpp
        Mono,        // binary color type, not yet supported
        UnknowFormat // other formats, users can extend
    };
    static int depth(int pixfmt);
    static int pitch(int pixfmt, int length = 1);
    static const char *name(int pixfmt);
};

template<typename T> inline const T &pmin(const T &x, const T &y)
{
    return x < y ? x : y;
}

template<typename T> inline const T &pmax(const T &x, const T &y)
{
    return x > y ? x : y;
}

template<typename T> inline T pabs(const T &x)
{
    return x < 0 ? -x : x;
}

template<typename T> inline const T &pbound(const T &min, const T &val, const T &max)
{
    return pmax(min, pmin(val, max));
}

inline bool pFuzzyZero(float a, float limit)
{
    return a > -limit && a < limit;
}

inline bool pFuzzyZero(float a)
{
    return pFuzzyZero(a, 1e-5f);
}

inline bool pFuzzyEqual(float a, float b)
{
    return (pabs(a - b) * 100000.f <= pmin(pabs(a), pabs(b)));
}

namespace utils
{
/* Borrowed from Boost::noncopyable. Add the template to give each noncopyable
 * a different signature. */
template<typename T> class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}

private: // emphasize the following members are private
    noncopyable(const noncopyable &);
    noncopyable &operator=(const noncopyable &);
};

#define DEFINE_CLASS_ENUM_FLAG_OPERATORS(ENUMTYPE)                                 \
    friend inline ENUMTYPE operator|(ENUMTYPE a, ENUMTYPE b)                       \
    {                                                                              \
        return ENUMTYPE(((int)a) | ((int)b));                                      \
    }                                                                              \
    friend inline ENUMTYPE operator|=(ENUMTYPE &a, ENUMTYPE b)                     \
    {                                                                              \
        return (ENUMTYPE &)(((int &)a) |= ((int)b));                               \
    }                                                                              \
    friend inline ENUMTYPE operator&(ENUMTYPE a, ENUMTYPE b)                       \
    {                                                                              \
        return ENUMTYPE(((int)a) & ((int)b));                                      \
    }                                                                              \
    friend inline ENUMTYPE operator&=(ENUMTYPE &a, ENUMTYPE b)                     \
    {                                                                              \
        return (ENUMTYPE &)(((int &)a) &= ((int)b));                               \
    }                                                                              \
    friend inline ENUMTYPE operator~(ENUMTYPE a) { return (ENUMTYPE)(~((int)a)); } \
    friend inline ENUMTYPE operator^(ENUMTYPE a, ENUMTYPE b)                       \
    {                                                                              \
        return ENUMTYPE(((int)a) ^ ((int)b));                                      \
    }                                                                              \
    friend inline ENUMTYPE operator^=(ENUMTYPE &a, ENUMTYPE b)                     \
    {                                                                              \
        return (ENUMTYPE &)(((int &)a) ^= ((int)b));                               \
    }

#define DEFINE_CLASS_ENUM_FLAG_OPERATORS2(ENUMTYPE)                                             \
    inline ENUMTYPE operator|(ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) | ((int)b)); } \
    inline ENUMTYPE operator|=(ENUMTYPE &a, ENUMTYPE b)                                         \
    {                                                                                           \
        return (ENUMTYPE &)(((int &)a) |= ((int)b));                                            \
    }                                                                                           \
    inline ENUMTYPE operator&(ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) & ((int)b)); } \
    inline ENUMTYPE operator&=(ENUMTYPE &a, ENUMTYPE b)                                         \
    {                                                                                           \
        return (ENUMTYPE &)(((int &)a) &= ((int)b));                                            \
    }                                                                                           \
    inline ENUMTYPE operator~(ENUMTYPE a) { return (ENUMTYPE)(~((int)a)); }                     \
    inline ENUMTYPE operator^(ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) ^ ((int)b)); } \
    inline ENUMTYPE operator^=(ENUMTYPE &a, ENUMTYPE b)                                         \
    {                                                                                           \
        return (ENUMTYPE &)(((int &)a) ^= ((int)b));                                            \
    }
}; // namespace utils

PERSIMMON_NAMESPACE_END
