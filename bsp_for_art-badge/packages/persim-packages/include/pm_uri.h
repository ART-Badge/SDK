/*
 * File      : uri.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-17     realthread   the first version
 */
#pragma once

#include <pm_string.h>
#include <pm_vector.h>
#include <utility>

PERSIMMON_NAMESPACE_BEGIN

class Uri
{
protected:
    typedef persim::String String;

public:
    typedef std::pair<String, String> Query;
    typedef persim::Vector<Query> QueryList;

    Uri(const String &uri = String());
    ~Uri();

    String encode() const;
    const String &scheme() const { return _scheme; }
    const String &authority() const { return _authority; }
    const String &path() const { return _path; }
    String query(const String &name) const;

    void setScheme(const String &scheme);
    void setAuthority(const String &authority);
    void setPath(const String &path);
    void setQuery(const String &query);
    void addQuery(const String &name, const String &value);

    const QueryList &querys() const { return _querys; }

private:
    String _scheme, _authority, _path;
    QueryList _querys;
};

#ifdef PKG_PERSIMMON_CPP_LOGGER
const Logger &operator<<(const Logger &, const Uri &);
#endif

PERSIMMON_NAMESPACE_END
