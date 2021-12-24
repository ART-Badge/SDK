#ifndef _RTL876X_MODULE_GPIO_BUTTON__
#define _RTL876X_MODULE_GPIO_BUTTON__
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#include "rtl876x_tim.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "rtl876x.h"
#include "trace.h"


void gpio_button_init(void);
void gpio_button_enter_dlps(void);
void gpio_button_exit_dlps(void);

#endif //_RTL876X_MODULE_GPIO_BUTTON__


