#include <rtthread.h>
#include <fal.h>
rt_mutex_t flash_mutex = RT_NULL;

int fal_init_auto(void)
{
   flash_mutex = rt_mutex_create("flash_mutex", RT_IPC_FLAG_FIFO);
    if (flash_mutex == RT_NULL)
    {
        rt_kprintf("create flash mutex failed!!!.\n");
        return -1;
    } 
    return fal_init();
}
INIT_ENV_EXPORT(fal_init_auto);
