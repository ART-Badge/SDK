/*
 * File      : uri.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-17     realthread   the first version
 */
#pragma once

#include <string>

class Uri
{
public:
    Uri() {}
    Uri(const char* uri);
    Uri(std::string &uri);
    ~Uri();

    void uriInit(const char* uri);
    void uriInit(std::string &uri);
    std::string uri(void);
    std::string getScheme(void);
    std::string getAuthority(bool dec = true);
    std::string getPath(bool dec = true);
    std::string getQuery(bool dec = false);
    std::string getQuery(const char* name, bool dec = true);
    std::string getQuery(std::string &name, bool dec = true);

    void setScheme(const char* scheme);
    void setScheme(std::string &scheme);
    void setAuthority(const char* authority);
    void setAuthority(std::string &authority);
    void setPath(const char* path);
    void setPath(std::string &path);
    void setQuery(const char* query);
    void setQuery(std::string &query);
    void addQuery(const char* name, const char* value);
    void addQuery(std::string &name, std::string &value);

private:
    std::string scheme, authority, path, query;
};

void parseQuerys(void *obj, std::string querys);

std::string UriDecode(const std::string & sSrc);
std::string UriEncode(const std::string & sSrc);
