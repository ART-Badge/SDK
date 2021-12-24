
#define DBG_TAG    "dlps_app"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#include "rtl876x_io_dlps.h"
#include "rtl876x_rcc.h"
#include "rtl876x_nvic.h"
#include "rtl876x_gpio.h"
#include "dlps.h"

#include "dlps_app.h"

/* What is the advertising interval when device is discoverable (units of 625us, 160=100ms)*/
#define DEFAULT_ADVERTISING_INTERVAL_MIN            320 /* 20ms */
#define DEFAULT_ADVERTISING_INTERVAL_MAX            320 /* 20ms */

bool g_allowed_dfu_enter_dlps = true;
uint8_t g_ota_mode;
uint8_t g_keystatus;

/**
* @brief    driver_init() contains the initialization of peripherals.
*
*               Both new architecture driver and legacy driver initialization method can be used.
*
* @return  void
*/
static void driver_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
    GPIO_InitTypeDef Gpio_Struct;
    GPIO_StructInit(&Gpio_Struct);

    Gpio_Struct.GPIO_Pin = GPIO_GetPin(KEY);
    Gpio_Struct.GPIO_Mode = GPIO_Mode_IN;
    Gpio_Struct.GPIO_ITCmd = ENABLE;
    Gpio_Struct.GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
    Gpio_Struct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    Gpio_Struct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_ENABLE;
    Gpio_Struct.GPIO_DebounceTime = 20;
    GPIO_Init(&Gpio_Struct);

    g_keystatus = GPIO_ReadInputDataBit(GPIO_GetPin(KEY));
    GPIO_MaskINTConfig(GPIO_GetPin(KEY), DISABLE);
    GPIO_INTConfig(GPIO_GetPin(KEY), ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;
    //NVIC_InitStruct.NVIC_IRQChannel = GPIO5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannel = KEY_IRQ;//P0_3
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void KEY_INT_Handle(void)//P0_3
{
    log_i("Enter GPIO0_Handler!");
    GPIO_MaskINTConfig(GPIO_GetPin(KEY), ENABLE);
    g_keystatus = GPIO_ReadInputDataBit(GPIO_GetPin(KEY));

    if (g_keystatus == 0)
    {
        GPIO->INTPOLARITY |= GPIO_GetPin(KEY);
    }
    else
    {
        GPIO->INTPOLARITY &= ~GPIO_GetPin(KEY);
    }
    GPIO_ClearINTPendingBit(GPIO_GetPin(KEY));
    GPIO_MaskINTConfig(GPIO_GetPin(KEY), DISABLE);
}

/**
 * @brief    System_Handler
 * @note     system handle to judge which pin is wake source
 * @return   void
 */
void System_Handler(void)
{
    uint8_t tmpVal;
    log_i("System_Handler");

    //APP_PRINT_INFO0("System_Handler");
    NVIC_DisableIRQ(System_IRQn);

    KEY_INT_Handle();//P2_4  edge triggle , can't interrupt after dlps

    // need clear debounce bit here.
    tmpVal = btaon_fast_read_safe(0x2b);
    btaon_fast_write_safe(0x2b, (tmpVal | BIT7));

    NVIC_ClearPendingIRQ(System_IRQn);

}

/**
 * @brief    pinmux configuration
 * @return   void
 */
static void pinmux_configuration(void)
{
    Pinmux_Config(KEY, DWGPIO);
    return;
}
/**
 * @brief    pad configuration
 * @return   void
 */
void pad_configuration(void)
{
    Pad_Config(KEY, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    return;
}

#if DLPS_EN
/**
 * @brief this function will be called before enter DLPS
 *
 *  set PAD and wakeup pin config for enterring DLPS
 *
 * @param none
 * @return none
 * @retval void
*/
void dfu_enter_dlps_set(void)
{
    Pad_Config(KEY, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    System_WakeUpDebounceTime(0x8);

	System_WakeUpPinEnable(KEY, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_ENABLE);

#if (DEBUG_TEST_WDG_APP_ENABLE == 1)
    extern void reset_test_dlps_enter1(void);
    reset_test_dlps_enter1();
#endif
    log_i("DLPS ENTER");
}

/**
 * @brief this function will be called after exit DLPS
 *
 *  set PAD and wakeup pin config for enterring DLPS
 *
 * @param none
 * @return none
 * @retval void
*/
void dfu_exit_dlps_init(void)
{
    Pad_Config(KEY, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

#if (DEBUG_TEST_WDG_APP_ENABLE == 1)
    extern void reset_test_dlps_exit1(void);
    reset_test_dlps_exit1();
#endif
    log_i("DLPS EXIT");
}

/**
 * @brief DLPS CallBack function
 * @param none
* @return true : allow enter dlps
 * @retval void
*/
bool dlps_dfu_check(void)
{
    return g_allowed_dfu_enter_dlps;
}
#endif

/**
* @brief    PwrMgr_Init() contains the setting about power mode.
*
* @return  void
*/
void pwr_mgr_init(void)
{
#if DLPS_EN
    if (false == dlps_check_cb_reg(dlps_dfu_check))
    {
        log_e("Error: dlps_check_cb_reg(dlps_dfu_check) failed!\n");
    }
    DLPS_IORegUserDlpsEnterCb(dfu_enter_dlps_set);
    DLPS_IORegUserDlpsExitCb(dfu_exit_dlps_init);
    DLPS_IORegister();
    lps_mode_set(LPM_DLPS_MODE);
    log_i("DLPS enable");
#else
    lps_mode_set(LPM_ACTIVE_MODE);
    DBG_DIRECT("DLPS DISABLE");
#endif
}

void dlps_cpu_init(void)
{
	rt_pin_mode(KEY, PIN_MODE_INPUT_PULLUP);
	log_i("KEY leval is:%d\n",rt_pin_read(KEY));
    pinmux_configuration();
    pad_configuration();
	pwr_mgr_init();
}
