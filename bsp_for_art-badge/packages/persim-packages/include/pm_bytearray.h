#pragma once

#include "pm_shareddata.h"

PERSIMMON_NAMESPACE_BEGIN

class ByteArray
{
public:
    ByteArray(size_t size = 0);
    ByteArray(const uint8_t *data, size_t size);
    ByteArray(const ByteArray &array);
    ByteArray &operator=(const ByteArray &array);
    bool isEmpty() const;
    bool isStatic() const;
    size_t size() const;
    void clear();
    void resize(size_t size);
    void push_back(uint8_t byte);
    uint8_t *data();
    const uint8_t *constData() const;
    const char *charData() const { return reinterpret_cast<const char *>(constData()); }
    uint8_t operator[](size_t index) const { return constData()[index]; }
    uint8_t *detach();

private:
    SharedDataRef<class ByteArrayData> p;
};

PERSIMMON_NAMESPACE_END
