#include <rtdevice.h>
#include "ringtones.h"
#include "beep.h"
#include "string.h"

void ringtones_traversal(void)
{
    beep_init();

    for (int i = 0; i < sizeof(MusicList) / sizeof(MusicList[0]); i++)
    {
        beep_on();
        rt_kprintf("beep:::name:%s\n", MusicList[i].name);

        for (int j = 0; j < MusicList[i].length; j++)
        {
            beep_set(MusicList[i].mc[j].Freq, 10);

            rt_thread_mdelay(MusicList[i].mc[j].Time);
        }

        beep_off();
        rt_thread_mdelay(1000);
    }
}
MSH_CMD_EXPORT(ringtones_traversal, ringtones_traversal);

void Audio_PlayMusic(const char *name)
{
    beep_init();

    for (int i = 0; i < sizeof(MusicList) / sizeof(MusicList[0]); i++)
    {
        if (strcmp(name, MusicList[i].name) == 0)
        {
            for (int j = 0; j < MusicList[i].length; j++)
            {
                beep_on();
                beep_set(MusicList[i].mc[j].Freq, 3);
                rt_thread_mdelay(MusicList[i].mc[j].Time);
                beep_off();
            }

            break;
        }
    }
}
