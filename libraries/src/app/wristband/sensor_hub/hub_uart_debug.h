#ifndef _HUB_UART_DEBUG_H_
#define _HUB_UART_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"
#include "trace.h"
#include "app_msg.h"

#define RECEIVE_BUF_MAX_LENGTH      100
#define AT_CMD_MAX_LENGTH           76
#define AT_CMD_MIN_LENGTH           2


void uart_debug_add_hub_task(void);
void wristband_uart_debug_driver_init(void);
void wristband_uart_debug_enter_dlps(void);
void wristband_uart_debug_exit_dlps(void);
bool wristband_uart_debug_allowed_enter_dlps_check(void);
bool wristband_uart_debug_system_wakeup_dlps_check(void);
void uart_debug_event_handler(T_IO_MSG msg);

#ifdef __cplusplus
}
#endif
#endif /* _HUB_UART_DEBUG_H_ */

