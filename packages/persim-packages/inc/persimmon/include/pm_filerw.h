/*
 * File      : pm_filerw.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-08     realthread   the first version
 */

#pragma once

#include <rtgui/filerw.h>

namespace Persimmon
{

class Filerw
{
public:
    Filerw();
    Filerw(const char *name, const char *mode = "rb");
    virtual ~Filerw();

    int load(const char *name, const char *mode = "rb");
    rtgui_filerw_t *getFilerw(void);
    int seek(rt_off_t offset, int whence);
    int read(void *buffer, rt_size_t size, rt_size_t count);
    int write(const void *buffer, rt_size_t size, rt_size_t count);
    int tell();
    int eof();
    int size();

    int getData(char **script);

private:
    rtgui_filerw_t *context;
};

}
