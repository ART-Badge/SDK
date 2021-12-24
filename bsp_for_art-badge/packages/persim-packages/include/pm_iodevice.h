#pragma once

#include <algorithm>
#include "pm_internal.h"
#include "pm_bytearray.h"
#include "pm_string.h"
#include "pm_respack.h"

PERSIMMON_NAMESPACE_BEGIN

class IODevice
{
public:
    enum OpenFlags
    {
        NoneFlag     = 0,
        Text         = 0x01,
        ExistingOnly = 0x02,
        ReadOnly     = 0x04,
        WriteOnly    = 0x08,
        ReadWrite    = ReadOnly | WriteOnly,
        Append       = 0x010,
        Truncate     = 0x20
    };
    DEFINE_CLASS_ENUM_FLAG_OPERATORS(OpenFlags);

    IODevice();
    virtual ~IODevice();
    virtual bool open(OpenFlags flags);
    virtual bool close();
    virtual size_t tell() const                       = 0;
    virtual bool seek(size_t pos)                     = 0;
    virtual size_t size() const                       = 0;
    virtual size_t read(void *buffer, size_t maxsize) = 0;
    virtual ByteArray read(size_t maxsize);
    virtual size_t write(const void *buffer, size_t maxsize) = 0;
    size_t write(const ByteArray &array);
    virtual String path() const;
    virtual const ResPack &package() const;
    virtual IODevice *copy() const = 0;
    ByteArray readAll();

    bool isOpened() const { return flags() != 0; }
    bool isWritable() const { return flags() > ReadOnly; }
    bool isReadable() const { return flags() & ReadOnly; }

protected:
    void swap(IODevice &dev) { std::swap(m_flags, dev.m_flags); }
    OpenFlags flags() const { return OpenFlags(m_flags); }

private:
    uint8_t m_flags;
};

inline bool operator==(const IODevice &lhs, const IODevice &rhs)
{
    const ResPack &lp(lhs.package()), &rp(rhs.package());
    return lp == rp && lhs.path() == rhs.path();
}

inline bool operator!=(const IODevice &lhs, const IODevice &rhs)
{
    return (lhs == rhs);
}

PERSIMMON_NAMESPACE_END
