/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     check_package_id.h
* @brief
* @details
* @author   chenjie_jin
* @date     2019-1-30
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __CHECK_PACKAGE_ID_H
#define __CHECK_PACKAGE_ID_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */
#include "stdbool.h"
#include "stdint.h"

void check_package_id(void);
bool is_package_id_authorized(void);

#ifdef __cplusplus
}
#endif

#endif /* __CHECK_PACKAGE_ID_H */

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

