/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      hci_app.h
* @brief     This file provides hci application layer code.
* @details
* @author    elliot chen
* @date      2018-11-16
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __LOCALPLAY_H
#define __LOCALPLAY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

#define MAX_MUSIC_CNT       20

typedef struct
{
    uint32_t name_addr;
    uint32_t name_len;
} T_MUSIC;

typedef struct
{
    uint16_t had_scaned_list_num;
    uint16_t cur_select;
    T_MUSIC music[MAX_MUSIC_CNT];
} T_MEDIA_INFO;

typedef struct
{
    uint32_t addr;
    uint32_t name_len;
} T_BT_DEV;


extern T_MEDIA_INFO media_info;
void save_media_list(uint8_t *name, uint32_t len);
void prepare_get_media_list(void);

#ifdef __cplusplus
}
#endif

#endif /*__HCI_APP_H*/

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

