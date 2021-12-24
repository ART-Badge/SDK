#include <rtthread.h>
#include <easyflash.h>

int easyflash_init_auto(void)
{
    if (easyflash_init() != EF_NO_ERR)
    {
        return -1;
    }
    return 0;
}
INIT_APP_EXPORT(easyflash_init_auto);
