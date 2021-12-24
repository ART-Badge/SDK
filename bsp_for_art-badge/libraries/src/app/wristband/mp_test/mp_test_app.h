/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      peripheral_app.h
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _MP_TEST_APP__
#define _MP_TEST_APP__

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_le.h>
#include <profile_server.h>

/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_SERVER_ID mp_test_simp_srv_id; /**< Simple ble service id*/
extern T_SERVER_ID mp_test_bas_srv_id;  /**< Battery service id */

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void mp_test_task_handle_io_msg(T_IO_MSG io_msg);
T_APP_RESULT mp_test_task_gap_callback(uint8_t cb_type, void *p_cb_data);
T_APP_RESULT mp_test_task_profile_callback(T_SERVER_ID service_id, void *p_data);


#ifdef __cplusplus
}
#endif

#endif

