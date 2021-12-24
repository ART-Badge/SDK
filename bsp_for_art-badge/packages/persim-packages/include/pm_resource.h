/*
 * File      : pm_resource.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-10     realthread   the first version
 */

#pragma once

#include <pm_string.h>

struct res_list;
struct string_table;
class CLocStrHelper;

PERSIMMON_NAMESPACE_BEGIN

class ResPack;

class Resource
{
public:
    Resource(const String &url, const char *language = nullptr);
    virtual ~Resource();

    bool isReady() const;
    String filePath() const;
    String dirPath() const;
    String dirName() const;
#ifdef GUIENGINE_USING_RES
    const ResPack &package() const;
#endif
    String basePath(const String &path = String()) const;
    String pagesPath(const String &path) const;
    String modulesPath(const String &path) const;
    String fontsPath(const String &path) const;
    String valuesPath(const String &path) const;
    String imagesPath(const String &path) const;
#ifdef PKG_PERSIMMON_USING_LOCAL
    CLocStrHelper *locStrHelper() const;
#endif

private:
    struct ResourcePrivate *_impl;
};

namespace resource
{
String basePath(const String &path = String());
String pagesPath(const String &path);
String modulesPath(const String &path);
String fontsPath(const String &path);
String valuesPath(const String &path);
String imagesPath(const String &path);
#ifdef PKG_PERSIMMON_USING_LOCAL
CLocStrHelper *locStrHelper();
#endif
} // namespace resource

PERSIMMON_NAMESPACE_END
