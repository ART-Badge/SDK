/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      headset.h
* @brief
* @details
* @author
* @date
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __HEADSET_H
#define __HEADSET_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

#define MAX_HEADSET_CNT       20

typedef struct
{
    uint32_t store_mac_addr;
    uint32_t store_name_addr;
    uint32_t store_name_len;
} T_HEADSET_DEV;

typedef struct
{
    uint16_t had_scaned_list_num;
    uint16_t had_scaned_none_name_list_num;
    uint16_t cur_select;
    T_HEADSET_DEV headset[MAX_HEADSET_CNT];
} T_HEADSET_INFO;


void save_headset_list(uint8_t *name, uint32_t len);
void prepare_get_headset_list(void);

extern T_HEADSET_INFO headset_info;

#ifdef __cplusplus
}
#endif

#endif /*__HEADSET_H*/

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

