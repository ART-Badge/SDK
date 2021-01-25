/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      communicate_task.c
   * @brief     Routines to create App task and handle events & messages
   * @author    howie
   * @date      2019-12-26
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <os_msg.h>
#include <os_task.h>
#include <mp_test_task.h>
#include <app_msg.h>
#include "trace.h"
#include "string.h"
#include <gap.h>
#include <gap_le.h>
#include <gap_msg.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <mp_test_app.h>
#include <mp_test_timer.h>
#include "os_timer.h"
#include <mp_test_task.h>

void *mp_test_connect_timer = NULL;


static void mp_test_timerout(void *pxTimer)
{
    APP_PRINT_INFO0("[MP TEST]mp_test_timerout");
    T_IO_MSG msg;
    msg.type = IO_MSG_TYPE_WRISTBNAD;
    msg.subtype = 0;
    send_msg_to_mp_test_task(&msg);
}

void mp_test_timer_init(void)
{
    os_timer_create(&mp_test_connect_timer, "mp_test_timer", 0,
                    2000, false,
                    mp_test_timerout);
}



/** @} */ /* End of group MP_TEST_TASK */
/** @} */ /* End of group PERIPH_DEMO */

