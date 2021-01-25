#include <trace.h>
#include <board.h>
#include <rtl876x_pinmux.h>
#include "rtl876x_io_dlps.h"
#include <dlps.h>

#if F_BT_DLPS_EN
bool can_enter_dlps = true;

void data_uart_dlps_exit_cb(void)
{
    Pad_ControlSelectValue(DATA_UART_TX_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(DATA_UART_RX_PIN, PAD_PINMUX_MODE);

}

void data_uart_dlps_enter_cb(void)
{
    Pad_ControlSelectValue(DATA_UART_TX_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(DATA_UART_RX_PIN, PAD_SW_MODE);
    System_WakeUpPinEnable(DATA_UART_RX_PIN, PAD_WAKEUP_POL_LOW, 0);
}

bool data_uart_dlps_check_cb(void)
{
    if (can_enter_dlps)
    {
        return true;
    }
    else
    {
        DlpsErrorCode = DLPS_ERROR_APP;
        return false;
    }
}

void System_Handler(void)
{
    if (System_WakeUpInterruptValue(DATA_UART_RX_PIN) == SET)
    {
        can_enter_dlps = false;
    }
    Pad_ClearAllWakeupINT();
}

void data_uart_can_enter_dlps(bool enter)
{
    can_enter_dlps = enter;
}

void data_uart_dlps_init(void)
{
    DLPS_IORegister();
    if (dlps_check_cb_reg(data_uart_dlps_check_cb) == false)
    {
        APP_PRINT_ERROR0("data_uart_dlps_init: dlps_check_cb_reg register failed");
    }
    DLPS_IORegUserDlpsEnterCb(data_uart_dlps_enter_cb);
    DLPS_IORegUserDlpsExitCb(data_uart_dlps_exit_cb);
}
#endif

