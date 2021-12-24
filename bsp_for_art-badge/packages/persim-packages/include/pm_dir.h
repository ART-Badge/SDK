#pragma once

#include "pm_string.h"

struct dirent;

PERSIMMON_NAMESPACE_BEGIN

class Dir
{
public:
    enum ListFilter
    {
        Normal = 0,
        NoDot  = 1,
        NoFile = 2,
        NoDir  = 4
    };
    DEFINE_CLASS_ENUM_FLAG_OPERATORS(ListFilter);

    Dir(const String &path = String());

    bool isAbsolute() const;
    bool isDirectory() const;
    String dirName() const;
    String filePath(const String &name) const;
    String urlPath(const String &name) const;
    String path() const;
    void setPath(const String &path);
    String absolutePath() const;

    bool exists(const String &path = String()) const;

    bool mkdir(const String &path);
    bool mkpath(const String &path);
    bool rename(const String &oldName, const String &newName) const;
    bool remove(const String &name);
    bool rmpath(const String &name);

    class FileInfo
    {
    public:
        FileInfo(const Dir &dir, ListFilter filter = Normal);
        ~FileInfo();
        bool hasNext() const { return _d && _de; }
        void next();
        bool isDir() const;
        bool isFile() const;
        String fileName() const;

    private:
        void *_d;
        struct dirent *_de;
        ListFilter _filter;
    };
    FileInfo fileInfo(ListFilter filter = Normal) const;

    static String cleanPath(const String &path);

private:
    String _path;
};

PERSIMMON_NAMESPACE_END
