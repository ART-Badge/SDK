/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/

#ifndef DECODE_H
#define DECODE_H

#include <rtthread.h>

#define SEMIBREVE_LEN 1600
/****    播放效果     ****/
#define SOUND_SIGNATURE       0    //调号：升0个半音
#define SOUND_OCTACHORD       1    //升降八度：升一个八度
#define SOUND_SPACE           4/5  //不同音符的时值

#define SONG_NAME_LENGTH_MAX  30
#define SONG_DATA_LENGTH_MAX  600

struct beep_song
{
    const uint8_t name[SONG_NAME_LENGTH_MAX];
    const uint8_t data[SONG_DATA_LENGTH_MAX];
};

struct beep_song_data
{
    rt_uint16_t freq;
    rt_uint16_t sound_len;
    rt_uint16_t nosound_len;
};

int beep_song_decode_init(void);
uint16_t beep_song_get_len(const struct beep_song *song);
int beep_song_get_name(const struct beep_song *song, char *name);
uint16_t beep_song_get_data(const struct beep_song *song, uint16_t index, struct beep_song_data *data);

#endif
