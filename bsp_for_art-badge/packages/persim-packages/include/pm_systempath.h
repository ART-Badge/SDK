#pragma once

#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

#define APP_SYSTEM_INTERNAL     "system.apps"
#define APP_USER_INTERNAL       "user.apps"

class SystemPath
{
public:
    enum PathTags
    {
        Fonts = 0,
        Icons,
        Images,
        Modules,
        Widgets,
        Config,
        Share,
        AppRoot,
        Download,
        Temp,
        SystemApps,
        UserApps,
        Data,
        SystemDials,
        UserDials,
        CustomPath,
        UnknowPath = CustomPath + 4
    };
    enum PermissionMode
    {
        Read      = 1,
        Write     = 2,
        ReadWrite = Read | Write
    };
    SystemPath();
    const char *path(int tag) const;
    const char *authority(int tag) const;
    int permission(int tag) const;
    void setItem(int tag, const char *path, const char *authority, int mode);
    void setPath(int tag, const char *path);
    void setAuthority(int tag, const char *authority);
    void setPermission(int tag, int mode);
    String normalize(const String &path, int permission);

private:
    class Private;
    Private *_p;
};

PERSIMMON_NAMESPACE_END
