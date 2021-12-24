#pragma once

#include "pm_iodevice.h"

PERSIMMON_NAMESPACE_BEGIN

class File : public IODevice
{
public:
    File();
    explicit File(const class Uri &uri);
    explicit File(const String &path, const ResPack &res = ResPack());
    virtual ~File();
    void setName(const String &path, const ResPack &res = ResPack());
    bool exists();
    virtual bool open(OpenFlags flags);
    virtual bool close();
    virtual size_t tell() const;
    virtual bool seek(size_t pos);
    virtual size_t size() const;
    virtual ByteArray read(size_t maxsize);
    virtual size_t read(void *buffer, size_t maxsize);
    virtual size_t write(const void *buffer, size_t maxsize);
    virtual String path() const;
    virtual const ResPack &package() const;
    virtual IODevice *copy() const;
    virtual bool ftruncate(size_t length);
    void swap(File &file);
    using IODevice::readAll;

    static bool exists(const String &path, const ResPack &res = ResPack());
    static ByteArray readAll(const String &path, const ResPack &res = ResPack());
    static int posixMode(OpenFlags flags);

private:
    class ResFile *_fd;
    ResPack _res;
    String _path;
};

class MemFile : public IODevice
{
public:
    MemFile(const void *ptr, size_t size);
    virtual bool open(OpenFlags flags);
    virtual bool close();
    virtual size_t tell() const;
    virtual bool seek(size_t pos);
    virtual size_t size() const;
    virtual size_t read(void *buffer, size_t maxsize);
    virtual ByteArray read(size_t maxsize);
    virtual size_t write(const void *buffer, size_t maxsize);
    virtual String path() const;
    virtual IODevice *copy() const;
    virtual bool ftruncate(size_t length);

private:
    size_t _pos, _size;
    const uint8_t *_ptr;
    const String _path;
};

class DfsFile : public IODevice
{
public:
    DfsFile();
    explicit DfsFile(const String &path);
    virtual ~DfsFile();

    void setName(const String &path);
    bool exists();
    virtual bool open(OpenFlags flags);
    virtual bool close();
    virtual size_t tell() const;
    virtual bool seek(size_t pos);
    virtual size_t size() const;
    virtual size_t read(void *buffer, size_t maxsize);
    virtual size_t write(const void *buffer, size_t maxsize);
    virtual String path() const;
    virtual IODevice *copy() const;
    virtual bool ftruncate(size_t length);
    using IODevice::read;
    using IODevice::readAll;

    static bool exists(const String &path);
    static ByteArray readAll(const String &path);
    static bool copy(const String &srcPath, const String &dstPath);
    static bool copy(IODevice *src, const String &dstPath);

private:
    int _fd;
    String _path;
};

PERSIMMON_NAMESPACE_END
