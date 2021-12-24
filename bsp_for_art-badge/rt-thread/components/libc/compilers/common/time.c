/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-21     zhangjun     copy from minilibc
 * 2020-09-07     Meco Man     combine gcc armcc iccarm
 * 2021-02-05     Meco Man     add timegm()
 * 2021-02-07     Meco Man     fixed gettimeofday()
 * 2021-02-08     Meco Man     add settimeofday() stime()
 * 2021-02-10     Meco Man     add ctime_r() and re-implement ctime()
 * 2021-02-11     Meco Man     fix bug #3183 - align days[] and months[] to 4 bytes
 * 2021-02-12     Meco Man     add errno
 * 2012-12-08     Bernard      <clock_time.c> fix the issue of _timevalue.tv_usec initialization,
 *                             which found by Rob <rdent@iinet.net.au>
 * 2021-02-12     Meco Man     move all of the functions located in <clock_time.c> to this file
 * 2021-03-15     Meco Man     fixed a bug of leaking memory in asctime()
 * 2021-05-01     Meco Man     support fixed timezone
 */

#include "sys/time.h"
#include <rtthread.h>

#ifdef RT_USING_DEVICE
#include <rtdevice.h>
#endif

#define DBG_TAG    "TIME"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef RT_LIBC_FIXED_TIMEZONE
#define RT_LIBC_FIXED_TIMEZONE 8 /* UTC+8 */
#endif

/* seconds per day */
#define SPD 24*60*60

/* days per month -- nonleap! */
static const short __spm[13] =
{
    0,
    (31),
    (31 + 28),
    (31 + 28 + 31),
    (31 + 28 + 31 + 30),
    (31 + 28 + 31 + 30 + 31),
    (31 + 28 + 31 + 30 + 31 + 30),
    (31 + 28 + 31 + 30 + 31 + 30 + 31),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31),
};

ALIGN(4) static const char days[] = "Sun Mon Tue Wed Thu Fri Sat ";
ALIGN(4) static const char months[] = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";

static int __isleap(int year)
{
    /* every fourth year is a leap year except for century years that are
     * not divisible by 400. */
    /*  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)); */
    return (!(year % 4) && ((year % 100) || !(year % 400)));
}

static void num2str(char *c, int i)
{
    c[0] = i / 10 + '0';
    c[1] = i % 10 + '0';
}

/**
 * Get time from RTC device (without timezone, UTC+0)
 * @param tv: struct timeval
 * @return the operation status, RT_EOK on successful
 */
static rt_err_t get_timeval(struct timeval *tv)
{
#ifdef RT_USING_RTC
    static rt_device_t device = RT_NULL;
    rt_err_t rst = -RT_ERROR;

    if (tv == RT_NULL)
        return -RT_EINVAL;

    /* default is 0 */
    tv->tv_sec = 0;
    tv->tv_usec = 0;

    /* optimization: find rtc device only first */
    if (device == RT_NULL)
    {
        device = rt_device_find("rtc");
    }

    /* read timestamp from RTC device */
    if (device != RT_NULL)
    {
        if (rt_device_open(device, 0) == RT_EOK)
        {
            rst = rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &tv->tv_sec);
            rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIMEVAL, tv);
            rt_device_close(device);
        }
    }
    else
    {
        /* LOG_W will cause a recursive printing if ulog timestamp function is enabled */
        rt_kprintf("Cannot find a RTC device to provide time!\r\n");
        return -RT_ENOSYS;
    }

    return rst;

#else
    /* LOG_W will cause a recursive printing if ulog timestamp function is enabled */
    rt_kprintf("Cannot find a RTC device to provide time!\r\n");
    return -RT_ENOSYS;
#endif /* RT_USING_RTC */
}

/**
 * Set time to RTC device (without timezone)
 * @param tv: struct timeval
 * @return the operation status, RT_EOK on successful
 */
static int set_timeval(struct timeval *tv)
{
#ifdef RT_USING_RTC
    static rt_device_t device = RT_NULL;
    rt_err_t rst = -RT_ERROR;

    if (tv == RT_NULL)
        return -RT_EINVAL;

    /* optimization: find rtc device only first */
    if (device == RT_NULL)
    {
        device = rt_device_find("rtc");
    }

    /* read timestamp from RTC device */
    if (device != RT_NULL)
    {
        if (rt_device_open(device, 0) == RT_EOK)
        {
            rst = rt_device_control(device, RT_DEVICE_CTRL_RTC_SET_TIME, &tv->tv_sec);
            rt_device_control(device, RT_DEVICE_CTRL_RTC_SET_TIMEVAL, tv);
            rt_device_close(device);
        }
    }
    else
    {
        LOG_W("Cannot find a RTC device to provide time!");
        return -RT_ENOSYS;
    }

    return rst;

#else
    LOG_W("Cannot find a RTC device to provide time!");
    return -RT_ENOSYS;
#endif /* RT_USING_RTC */
}

struct tm *gmtime_r(const time_t *timep, struct tm *r)
{
    time_t i;
    register time_t work = *timep % (SPD);
    r->tm_sec = work % 60;
    work /= 60;
    r->tm_min = work % 60;
    r->tm_hour = work / 60;
    work = *timep / (SPD);
    r->tm_wday = (4 + work) % 7;
    for (i = 1970;; ++i)
    {
        register time_t k = __isleap(i) ? 366 : 365;
        if (work >= k)
            work -= k;
        else
            break;
    }
    r->tm_year = i - 1900;
    r->tm_yday = work;

    r->tm_mday = 1;
    if (__isleap(i) && (work > 58))
    {
        if (work == 59)
            r->tm_mday = 2; /* 29.2. */
        work -= 1;
    }

    for (i = 11; i && (__spm[i] > work); --i)
        ;
    r->tm_mon = i;
    r->tm_mday += work - __spm[i];

    r->tm_isdst = 0;
    return r;
}
RTM_EXPORT(gmtime_r);

struct tm* gmtime(const time_t* t)
{
    static struct tm tmp;
    return gmtime_r(t, &tmp);
}
RTM_EXPORT(gmtime);

struct tm* localtime_r(const time_t* t, struct tm* r)
{
    time_t local_tz;

    local_tz = *t + RT_LIBC_FIXED_TIMEZONE * 3600;
    return gmtime_r(&local_tz, r);
}
RTM_EXPORT(localtime_r);

struct tm* localtime(const time_t* t)
{
    static struct tm tmp;
    return localtime_r(t, &tmp);
}
RTM_EXPORT(localtime);

time_t mktime(struct tm * const t)
{
    time_t timestamp;

    timestamp = timegm(t);
    timestamp = timestamp - 3600 * RT_LIBC_FIXED_TIMEZONE;
    return timestamp;
}
RTM_EXPORT(mktime);

char* asctime_r(const struct tm *t, char *buf)
{
    /* "Wed Jun 30 21:49:08 1993\n" */
    *(int*) buf = *(int*) (days + (t->tm_wday << 2));
    *(int*) (buf + 4) = *(int*) (months + (t->tm_mon << 2));
    num2str(buf + 8, t->tm_mday);
    if (buf[8] == '0')
        buf[8] = ' ';
    buf[10] = ' ';
    num2str(buf + 11, t->tm_hour);
    buf[13] = ':';
    num2str(buf + 14, t->tm_min);
    buf[16] = ':';
    num2str(buf + 17, t->tm_sec);
    buf[19] = ' ';
    num2str(buf + 20, (t->tm_year + 1900) / 100);
    num2str(buf + 22, (t->tm_year + 1900) % 100);
    buf[24] = '\n';
    buf[25] = '\0';
    return buf;
}
RTM_EXPORT(asctime_r);

char* asctime(const struct tm *timeptr)
{
    static char buf[26];
    return asctime_r(timeptr, buf);
}
RTM_EXPORT(asctime);

char *ctime_r(const time_t * tim_p, char * result)
{
    struct tm tm;
    return asctime_r(localtime_r(tim_p, &tm), result);
}
RTM_EXPORT(ctime_r);

char* ctime(const time_t *tim_p)
{
    return asctime(localtime(tim_p));
}
RTM_EXPORT(ctime);

/**
 * Returns the current time.
 *
 * @param time_t * t the timestamp pointer, if not used, keep NULL.
 *
 * @return The value ((time_t)-1) is returned if the calendar time is not available.
 *         If timer is not a NULL pointer, the return value is also stored in timer.
 *
 */
RT_WEAK time_t time(time_t *t)
{
    struct timeval now;

    if(get_timeval(&now) == RT_EOK)
    {
        if (t)
        {
            *t = now.tv_sec;
        }
        return now.tv_sec;
    }
    else
    {
        rt_set_errno(EFAULT);
        return ((time_t)-1);
    }
}
RTM_EXPORT(time);

RT_WEAK clock_t clock(void)
{
    return rt_tick_get();
}
RTM_EXPORT(clock);

int stime(const time_t *t)
{
    struct timeval tv;

    if (!t)
    {
        rt_set_errno(EFAULT);
        return -1;
    }

    tv.tv_sec = *t;
    if (set_timeval(&tv) == RT_EOK)
    {
        return 0;
    }
    else
    {
        rt_set_errno(EFAULT);
        return -1;
    }
}
RTM_EXPORT(stime);

time_t timegm(struct tm * const t)
{
    register time_t day;
    register time_t i;
    register time_t years = t->tm_year - 70;

    if (t->tm_sec > 60)
    {
        t->tm_min += t->tm_sec / 60;
        t->tm_sec %= 60;
    }
    if (t->tm_min > 60)
    {
        t->tm_hour += t->tm_min / 60;
        t->tm_min %= 60;
    }
    if (t->tm_hour > 24)
    {
        t->tm_mday += t->tm_hour / 24;
        t->tm_hour %= 24;
    }
    if (t->tm_mon > 12)
    {
        t->tm_year += t->tm_mon / 12;
        t->tm_mon %= 12;
    }
    while (t->tm_mday > __spm[1 + t->tm_mon])
    {
        if (t->tm_mon == 1 && __isleap(t->tm_year + 1900))
        {
            --t->tm_mday;
        }
        t->tm_mday -= __spm[t->tm_mon];
        ++t->tm_mon;
        if (t->tm_mon > 11)
        {
            t->tm_mon = 0;
            ++t->tm_year;
        }
    }

    if (t->tm_year < 70)
        return (time_t) - 1;

    /* Days since 1970 is 365 * number of years + number of leap years since 1970 */
    day = years * 365 + (years + 1) / 4;

    /* After 2100 we have to substract 3 leap years for every 400 years
     This is not intuitive. Most mktime implementations do not support
     dates after 2059, anyway, so we might leave this out for it's
     bloat. */
    if (years >= 131)
    {
        years -= 131;
        years /= 100;
        day -= (years >> 2) * 3 + 1;
        if ((years &= 3) == 3)
            years--;
        day -= years;
    }

    day += t->tm_yday = __spm[t->tm_mon] + t->tm_mday - 1 +
                        (__isleap(t->tm_year + 1900) & (t->tm_mon > 1));

    /* day is now the number of days since 'Jan 1 1970' */
    i = 7;
    t->tm_wday = (day + 4) % i; /* Sunday=0, Monday=1, ..., Saturday=6 */

    i = 24;
    day *= i;
    i = 60;
    return ((day + t->tm_hour) * i + t->tm_min) * i + t->tm_sec;
}
RTM_EXPORT(timegm);

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    /* The use of the timezone structure is obsolete;
     * the tz argument should normally be specified as NULL.
     * The tz_dsttime field has never been used under Linux.
     * Thus, the following is purely of historic interest.
     */
    if(tz != RT_NULL)
    {
        tz->tz_dsttime = DST_NONE;
        tz->tz_minuteswest = -(RT_LIBC_FIXED_TIMEZONE * 60);
    }

    if (tv != RT_NULL && get_timeval(tv) == RT_EOK)
    {
        return 0;
    }
    else
    {
        rt_set_errno(EFAULT);
        return -1;
    }
}
RTM_EXPORT(gettimeofday);

int settimeofday(const struct timeval *tv, const struct timezone *tz)
{
    /* The use of the timezone structure is obsolete;
     * the tz argument should normally be specified as NULL.
     * The tz_dsttime field has never been used under Linux.
     * Thus, the following is purely of historic interest.
     */
    if (tv != RT_NULL
        && tv->tv_sec >= 0
        && tv->tv_usec >= 0
        && set_timeval((struct timeval *)tv) == RT_EOK)
    {
        return 0;
    }
    else
    {
        rt_set_errno(EINVAL);
        return -1;
    }
}
RTM_EXPORT(settimeofday);

/* inherent in the toolchain */
RTM_EXPORT(difftime);
RTM_EXPORT(strftime);

#ifdef RT_USING_POSIX
static struct timeval _timevalue;
static int clock_time_system_init()
{
    time_t time;
    rt_tick_t tick;
    rt_device_t device;

    time = 0;
    device = rt_device_find("rtc");
    if (device != RT_NULL)
    {
        /* get realtime seconds */
        rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &time);
    }

    /* get tick */
    tick = rt_tick_get();

    _timevalue.tv_usec = (tick%RT_TICK_PER_SECOND) * MICROSECOND_PER_TICK;
    _timevalue.tv_sec = time - tick/RT_TICK_PER_SECOND - 1;

    return 0;
}
INIT_COMPONENT_EXPORT(clock_time_system_init);

int clock_getres(clockid_t clockid, struct timespec *res)
{
    int ret = 0;

    if (res == RT_NULL)
    {
        rt_set_errno(EINVAL);
        return -1;
    }

    switch (clockid)
    {
    case CLOCK_REALTIME:
        res->tv_sec = 0;
        res->tv_nsec = NANOSECOND_PER_SECOND/RT_TICK_PER_SECOND;
        break;

#ifdef RT_USING_CPUTIME
    case CLOCK_CPUTIME_ID:
        res->tv_sec  = 0;
        res->tv_nsec = clock_cpu_getres();
        break;
#endif

    default:
        ret = -1;
        rt_set_errno(EINVAL);
        break;
    }

    return ret;
}
RTM_EXPORT(clock_getres);

int clock_gettime(clockid_t clockid, struct timespec *tp)
{
    int ret = 0;

    if (tp == RT_NULL)
    {
        rt_set_errno(EINVAL);
        return -1;
    }

    switch (clockid)
    {
    case CLOCK_REALTIME:
        {
            /* get tick */
            int tick = rt_tick_get();

            tp->tv_sec  = _timevalue.tv_sec + tick / RT_TICK_PER_SECOND;
            tp->tv_nsec = (_timevalue.tv_usec + (tick % RT_TICK_PER_SECOND) * MICROSECOND_PER_TICK) * 1000;
        }
        break;

#ifdef RT_USING_CPUTIME
    case CLOCK_CPUTIME_ID:
        {
            float unit = 0;
            long long cpu_tick;

            unit = clock_cpu_getres();
            cpu_tick = clock_cpu_gettime();

            tp->tv_sec  = ((int)(cpu_tick * unit)) / NANOSECOND_PER_SECOND;
            tp->tv_nsec = ((int)(cpu_tick * unit)) % NANOSECOND_PER_SECOND;
        }
        break;
#endif
    default:
        rt_set_errno(EINVAL);
        ret = -1;
    }

    return ret;
}
RTM_EXPORT(clock_gettime);

int clock_settime(clockid_t clockid, const struct timespec *tp)
{
    int second;
    rt_tick_t tick;
    rt_device_t device;

    if ((clockid != CLOCK_REALTIME) || (tp == RT_NULL))
    {
        rt_set_errno(EINVAL);

        return -1;
    }

    /* get second */
    second = tp->tv_sec;
    /* get tick */
    tick = rt_tick_get();

    /* update timevalue */
    _timevalue.tv_usec = MICROSECOND_PER_SECOND - (tick % RT_TICK_PER_SECOND) * MICROSECOND_PER_TICK;
    _timevalue.tv_sec = second - tick/RT_TICK_PER_SECOND - 1;

    /* update for RTC device */
    device = rt_device_find("rtc");
    if (device != RT_NULL)
    {
        /* set realtime seconds */
        rt_device_control(device, RT_DEVICE_CTRL_RTC_SET_TIME, &second);
    }
    else
        return -1;

    return 0;
}
RTM_EXPORT(clock_settime);

int clock_time_to_tick(const struct timespec *time)
{
    int tick;
    int nsecond, second;
    struct timespec tp;

    RT_ASSERT(time != RT_NULL);

    tick = RT_WAITING_FOREVER;
    if (time != NULL)
    {
        /* get current tp */
        clock_gettime(CLOCK_REALTIME, &tp);

        if ((time->tv_nsec - tp.tv_nsec) < 0)
        {
            nsecond = NANOSECOND_PER_SECOND - (tp.tv_nsec - time->tv_nsec);
            second  = time->tv_sec - tp.tv_sec - 1;
        }
        else
        {
            nsecond = time->tv_nsec - tp.tv_nsec;
            second  = time->tv_sec - tp.tv_sec;
        }

        tick = second * RT_TICK_PER_SECOND + nsecond * RT_TICK_PER_SECOND / NANOSECOND_PER_SECOND;
        if (tick < 0) tick = 0;
    }

    return tick;
}
RTM_EXPORT(clock_time_to_tick);

#endif /* RT_USING_POSIX */
