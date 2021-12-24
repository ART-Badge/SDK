#pragma once

#include "pm_string.h"
#include <utility>

PERSIMMON_NAMESPACE_BEGIN

#ifdef PKG_PERSIMMON_CPP_LOGGER

class Logger
{
public:
    class holdspc
    {
    public:
        holdspc(const Logger &log) : _log(log) { log._depth++; }
        ~holdspc()
        {
            if (_log._depth && !--_log._depth)
                _log << "";
        }

    private:
        const Logger &_log;
    };
    Logger();
    ~Logger();
    const Logger &operator<<(int value) const;
    const Logger &operator<<(float value) const;
    const Logger &operator<<(double value) const;
    const Logger &operator<<(uint32_t value) const;
    const Logger &operator<<(const char *msg) const;
    const Logger &operator<<(const String &msg) const;
    const Logger &operator<<(bool value) const;
    const Logger &operator<<(const void *ptr) const;

private:
    inline char space() const;

private:
    mutable int _depth;
};

template<class T1, class T2>
const Logger &operator<<(const Logger &log, const std::pair<T1, T2> &pair)
{
    Logger::holdspc spc(log);
    return log << "std::pair(" << pair.first << ", " << pair.second << ")";
}

#define plog_debug() Logger()

#else

class Logger
{
public:
    class holdspc
    {
    public:
        holdspc(const Logger &) {}
    };
    template<class T> const Logger &operator<<(const T &value) { return *this; }
};

#define plog_debug() Logger()

#endif

PERSIMMON_NAMESPACE_END
