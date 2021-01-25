/*
 * File      : pm_resource.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-10     realthread   the first version
 */

#pragma once

#include <rtthread.h>
#include <rtgui/rtgui_resource.h>
#include <rtgui/hash_table.h>
#include <string/string_table.h>
#include <string>

#ifdef PKG_PERSIMMON_USING_LOCAL
#include "CLocStrHelper.h"
#endif

namespace Persimmon
{

class Resource
{
public:
    Resource(const char *url, const char *langue = NULL);
    virtual~Resource();

    const std::string& langue(void);
    bool isReady(void);
#ifdef GUIENGINE_USING_RES
    bool addResource(const char *res_path);
    bool rmResource(const char *res_path);
    res_list_t **getResList(void);
#endif
    void clearResource(void);
    rtgui_hash_table_t *getImageCache(void);
    struct string_table *getXmlCache(void);
    struct string_table *getJsCache(void);
#ifdef PKG_PERSIMMON_USING_LOCAL
    void cLocStrHelperInit(void);
    CLocStrHelper* getCLocStrHelper(void);
#endif

private:
    std::string _langue;
#ifdef GUIENGINE_USING_RES
    res_list_t *res_list;
#endif
    rtgui_hash_table_t *images;
    struct string_table *ezxml_cache;
    struct string_table *js_cache;
#ifdef PKG_PERSIMMON_USING_LOCAL
    CLocStrHelper* locStrHelper;
#endif
};

}
