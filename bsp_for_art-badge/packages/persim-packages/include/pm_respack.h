#pragma once

#include "pm_string.h"
#include "pm_shareddata.h"

#ifdef PKG_PERSIMMON_USING_LOCAL
class CLocStrHelper;
#endif

PERSIMMON_NAMESPACE_BEGIN

class ResPack
{
public:
    class Locker;
    class TempGuard;

    ResPack();
    explicit ResPack(const String &path, const String &table = String());
    ResPack(const ResPack &pack);
    ~ResPack();
    bool isEmpty() const;
    String path() const;
    intptr_t id() const;
    bool select(const String &table);
    void unlink();
    void setGlobal() const;
#ifdef PKG_PERSIMMON_USING_LOCAL
    CLocStrHelper *locStrHelper() const;
#endif
    ResPack &operator=(const ResPack &pack);
    bool operator==(const ResPack &pack) const { return _d == pack._d; }
    bool operator!=(const ResPack &pack) const { return _d != pack._d; }

    static const ResPack &emptyPackage();
    static const ResPack &activePackage();
    static void selectTable(const String &lang);

private:
    class Data;
    SharedDataRef<Data> _d;
    friend class ResFileRes;
    friend class ResPackTable;
};

class ResPack::TempGuard
{
public:
    explicit TempGuard(const ResPack &pack);
    ~TempGuard();

private:
    const ResPack _back;
};

PERSIMMON_NAMESPACE_END
