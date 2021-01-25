/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     data_uart_dlps.h
* @brief    Data uart dlps reference api.
* @details
* @author   jane
* @date     2016-02-18
* @version  v0.1
*********************************************************************************************************
*/
#ifndef _DATA_UART_DLPS_H_
#define  _DATA_UART_DLPS_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */
/** @defgroup DATA_UART_CMD
  * @{
  */
/** @defgroup DATA_UART_DLPS Data Uart DLPS
  * @{
  */
/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup DATA_UART_DLPS_Exported_Functions Data Uart DLPS Exported Functions
    * @{
    */

/**
 * @brief  Initializes the Data uart dlps.
 * @return  void
 *
 * <b>Example usage</b>
 * \code{.c}
    void pwr_mgr_init(void)
    {
#if F_BT_DLPS_EN
        data_uart_dlps_init();
        lps_mode_set(LPM_DLPS_MODE);
        lps_mode_pause();
#endif
    }
 * \endcode
 */
void data_uart_dlps_init(void);

/**
 * @brief  Set data uart dlps
 *
 * @param[in] enter   Whether allow data uart to enter dlps.
 *            @arg true Allow data uart to enter dlps
 *            @arg false Not allow data uart to enter dlps
 * @return  void
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_dlps(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
#if F_BT_DLPS_EN
        if (p_parse_value->dw_param[0] == 0)
        {
            lps_mode_pause();
            data_uart_print("Active Mode\r\n");
        }
        else
        {
            lps_mode_resume();
            data_uart_can_enter_dlps(true);
            data_uart_print("LPS Mode\r\n");
        }
#endif
        return (RESULT_SUCESS);
    }
 * \endcode
 */
void data_uart_can_enter_dlps(bool enter);
/** End of DATA_UART_DLPS_Exported_Functions
  * @}
  */

/** @} */ /* End of group DATA_UART_DLPS */
/** @} */ /* End of group DATA_UART_CMD */
#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

