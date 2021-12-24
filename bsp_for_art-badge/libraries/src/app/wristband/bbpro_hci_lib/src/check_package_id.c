/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      check_package_id.c
* @brief     This file provides check package id APIs for hci protocol communication.
* @details
* @author  chenjie_jin
* @date     2019-1-30
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "check_package_id.h"

/* Defines -------------------------------------------------------------------*/
#define RTL8752CJF              0x09

/* Gloabls -------------------------------------------------------------------*/
static uint8_t cur_package_id = 0x00;

/**
  * @brief  Check IC package ID.
  * @param  None.
  * @retval None.
  */
void check_package_id(void)
{
    uint8_t package_id = *(uint8_t *)0x002028f3;
    uint8_t inverse_package_id = *(uint8_t *)0x002028f4;

    if ((package_id ^ 0xff) == inverse_package_id)
    {
        cur_package_id = package_id;
    }
    else
    {
        cur_package_id = 0;
    }
}

/**
  * @brief  Check package ID is authorized.
  * @param  None.
  * @retval trur or false.
  */
bool is_package_id_authorized(void)
{
    if (cur_package_id == RTL8752CJF)
    {
        return true;
    }

    return false;
}

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

