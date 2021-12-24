/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      hub_task.h
   * @brief     Routines to create App task and handle events & messages
   * @author    howie
   * @date      2019-12-26
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _MP_TEST_TASK_H_
#define _MP_TEST_TASK_H_

#include "stdbool.h"
#include "app_msg.h"


/**
 * @brief  Initialize App task
 * @return void
 */
void mp_test_task_init(void);
bool send_msg_to_mp_test_task(T_IO_MSG *p_msg);


#endif

