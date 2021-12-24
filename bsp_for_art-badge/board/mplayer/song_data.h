/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-17     flybreak      the first version
 */

#ifndef SONG_DATA_H
#define SONG_DATA_H

#include "decode.h"

// typedef struct
// {
//     uint16_t Freq;
//     uint16_t Time;
//     uint16_t Volume;
// } MusicNode_t;

// /*音符频率对应表*/
// enum ToneMap
// {
//     L1 = 262,
//     L1h = 277,
//     L2 = 294,
//     L2h = 311,
//     L3 = 330,
//     L4 = 349,
//     L4h = 370,
//     L5 = 392,
//     L5h = 415,
//     L6 = 440,
//     L6h = 466,
//     L7 = 494,
//     M1 = 523,
//     M1h = 554,
//     M2 = 587,
//     M2h = 622,
//     M3 = 659,
//     M4 = 698,
//     M4h = 740,
//     M5 = 784,
//     M5h = 831,
//     M6 = 880,
//     M6h = 932,
//     M7 = 988,
//     H1 = 1046,
//     H1h = 1109,
//     H2 = 1175,
//     H2h = 1245,
//     H3 = 1318,
//     H4 = 1397,
//     H4h = 1480,
//     H5 = 1568,
//     H5h = 1661,
//     H6 = 1760,
//     H6h = 1865,
//     H7 = 1976,
// };


// #define MUSIC_DEF(name) static const MusicNode_t Music_##name[] =

// /*开机音*/
// MUSIC_DEF(Startup)
// {
//     {M1, 80},
//     {M6, 80},
//     {M3, 80},
// };

// /*关机音*/
// MUSIC_DEF(Shutdown)
// {
//     {M3, 80},
//     {M6, 80},
//     {M1, 80},
// };

// /*错误提示音*/
// MUSIC_DEF(Error)
// {
//     {100, 80},
//     {0,   80},
//     {100, 80},
// };

// /*连接成功音*/
// MUSIC_DEF(Connect)
// {
//     {H1, 80},
//     {H2, 80},
//     {H3, 80},
// };

// /*断开连接音*/
// MUSIC_DEF(Disconnect)
// {
//     {H3, 80},
//     {H2, 80},
//     {H1, 80},
// };

// /*信号不稳定提示音*/
// MUSIC_DEF(UnstableConnect)
// {
//     {H1, 80},
//     {0, 80},
//     {H1, 80},
// };

// /*充电启动提示音*/
// MUSIC_DEF(BattChargeStart)
// {
//     {L1, 80},
//     {L3, 80},
// };

// /*充电关闭提示音*/
// MUSIC_DEF(BattChargeEnd)
// {
//     {L3, 80},
//     {L1, 80},
// };

// /*设备插入提示音*/
// MUSIC_DEF(DeviceInsert)
// {
//     /*C4,A3,F3,F4*/
//     {M1, 180},
//     {L6, 80},
//     {L4, 80},
//     {M4, 160},
// };

// /*设备拔出提示音*/
// MUSIC_DEF(DevicePullout)
// {
//     /*A4,F4,E4*/
//     {L6, 80},
//     {L4, 80},
//     {L3, 80},
// };

// /*无操作提示音*/
// MUSIC_DEF(NoOperationWarning)
// {
//     {4000, 40},
//     {0, 80},
//     {4000, 40},
//     {0, 80},
//     {4000, 40},
// };

// /*列表数据类型定义*/
// typedef struct
// {
//     const MusicNode_t* mc;
//     uint16_t length;
//     const char* name;
// } MusicList_t;

// #define ADD_MUSIC(mc) {Music_##mc,(sizeof(Music_##mc) / sizeof(Music_##mc[0])), #mc}

// /*提示音地址储存列表*/
// static const MusicList_t MusicList[] =
// {
//     ADD_MUSIC(Startup),
//     ADD_MUSIC(Shutdown),
//     ADD_MUSIC(Error),
//     ADD_MUSIC(Connect),
//     ADD_MUSIC(Disconnect),
//     ADD_MUSIC(UnstableConnect),
//     ADD_MUSIC(BattChargeStart),
//     ADD_MUSIC(BattChargeEnd),
//     ADD_MUSIC(DeviceInsert),
//     ADD_MUSIC(DevicePullout),
//     ADD_MUSIC(NoOperationWarning),
// };


const struct beep_song song0 =
{
    .name = "JLJT",
    .data = {
        0x17, 0x0D, 0x19, 0x0D, 0x1A, 0x0D, 0x19, 0x03, 0x1A, 0x0D,
        0x19, 0x03, 0x1A, 0x0D, 0x1F, 0x02, 0x19, 0x0D, 0x1A, 0x03,
        0x17, 0x0D, 0x17, 0x0D, 0x19, 0x03, 0x1A, 0x0D, 0x19, 0x03,
        0x1A, 0x0D, 0x19, 0x03, 0x1A, 0x0D, 0x21, 0x03, 0x1F, 0x0D,
        0x20, 0x03, 0x1A, 0x0D, 0x17, 0x0D, 0x19, 0x03, 0x1A, 0x0D,
        0x19, 0x03, 0x1A, 0x0D, 0x19, 0x03, 0x1A, 0x0D, 0x1F, 0x03,
        0x19, 0x0D, 0x1A, 0x03, 0x17, 0x0C, 0x19, 0x02, 0x15, 0x0C,
        0x16, 0x02, 0x17, 0x02, 0x1F, 0x02, 0x1A, 0x03, 0x21, 0x02,
        0x20, 0x0D, 0x21, 0x0E, 0x20, 0x04, 0x1F, 0x0D, 0x20, 0x03,
        0x1A, 0x16, 0x1A, 0x02, 0x1A, 0x02, 0x1A, 0x02, 0x1A, 0x04,
        0x1F, 0x04, 0x20, 0x04, 0x21, 0x04, 0x1A, 0x02, 0x1A, 0x02,
        0x1A, 0x0D, 0x19, 0x03, 0x19, 0x0D, 0x1A, 0x03, 0x1A, 0x02,
        0x1A, 0x02, 0x1A, 0x02, 0x1A, 0x0E, 0x1F, 0x0E, 0x20, 0x0E,
        0x21, 0x04, 0x1A, 0x02, 0x1A, 0x02, 0x1A, 0x0D, 0x22, 0x03,
        0x22, 0x0D, 0x21, 0x03, 0x1A, 0x02, 0x1A, 0x02, 0x1A, 0x02,
        0x1A, 0x0E, 0x1F, 0x0E, 0x20, 0x0E, 0x21, 0x04, 0x1A, 0x02,
        0x1A, 0x02, 0x1A, 0x0D, 0x19, 0x03, 0x19, 0x0D, 0x1A, 0x03,
        0x00, 0x00
    }
};

const struct beep_song song1 =
{
    .name = "两只老虎",
    .data = {
        0x15, 0x02, 0x16, 0x02, 0x17, 0x02, 0x15, 0x02, 0x15, 0x02,
        // 1         2            3           1           1
        0x16, 0x02, 0x17, 0x02, 0x15, 0x02, 0x17, 0x02, 0x18, 0x02,
        // 2         3            1           3           4
        0x19, 0x01, 0x17, 0x02, 0x18, 0x02, 0x19, 0x01, 0x19, 0x03,
        // 5   -     3            4           5    -      5_
        0x1A, 0x03, 0x19, 0x03, 0x18, 0x03, 0x17, 0x02, 0x15, 0x16,
        // 6_        5_           4_          3           1
        0x19, 0x03, 0x1A, 0x03, 0x19, 0x03, 0x18, 0x03, 0x17, 0x02,
        // 5_        6_           5_          4_          3
        0x15, 0x16, 0x15, 0x02, 0x0F, 0x02, 0x15, 0x01, 0x15, 0x02,
        // 1         1            5.          1    -      1
        0x0F, 0x02, 0x15, 0x01, 0x00, 0x00
        // 5.        1     -
    }
};
#endif