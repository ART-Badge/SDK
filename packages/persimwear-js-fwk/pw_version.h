/*
 * File      : pw_version.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-24     realthread   the first version
 */

#ifndef __PW_VERSION_H__
#define __PW_VERSION_H__

#include "rtconfig.h"

#ifndef PERSIMWEAR_MAINVERSION
#define PERSIMWEAR_MAINVERSION   1
#define PERSIMWEAR_SUBVERSION    0
#define PERSIMWEAR_REVISION      2
#endif

#define _PERSIMWEAR_STR_(s)                #s
#define __PERSIMWEAR_STR(s)                _PERSIMWEAR_STR_(s)

#define PERSIMWEAR_VERSION      __PERSIMWEAR_STR(PERSIMWEAR_MAINVERSION) "." \
                                __PERSIMWEAR_STR(PERSIMWEAR_SUBVERSION) "." \
                                __PERSIMWEAR_STR(PERSIMWEAR_REVISION)


#endif
